/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Globalization;

namespace Flood.RPC.Serialization
{
    /// <summary>
    /// JSON protocol implementation for thrift.
    ///
    /// This is a full-featured protocol supporting Write and Read.
    ///
    /// Please see the C++ class header for a detailed description of the
    /// protocol's wire format.
    ///
    /// Adapted from the Java version.
    /// </summary>
    public class JSONSerializer : Serializer
    {
        private static byte[] COMMA = new byte[] { (byte)',' };
        private static byte[] COLON = new byte[] { (byte)':' };
        private static byte[] LBRACE = new byte[] { (byte)'{' };
        private static byte[] RBRACE = new byte[] { (byte)'}' };
        private static byte[] LBRACKET = new byte[] { (byte)'[' };
        private static byte[] RBRACKET = new byte[] { (byte)']' };
        private static byte[] QUOTE = new byte[] { (byte)'"' };
        private static byte[] BACKSLASH = new byte[] { (byte)'\\' };
        private static byte[] ZERO = new byte[] { (byte)'0' };

        private byte[] ESCSEQ = new byte[] { (byte)'\\', (byte)'u', (byte)'0', (byte)'0' };

        private byte[] JSON_CHAR_TABLE = {
    0,  0,  0,  0,  0,  0,  0,  0,(byte)'b',(byte)'t',(byte)'n',  0,(byte)'f',(byte)'r',  0,  0, 
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
    1,  1,(byte)'"',  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  };

        private char[] ESCAPE_CHARS = "\"\\bfnrt".ToCharArray();

        private byte[] ESCAPE_CHAR_VALS = {
    (byte)'"', (byte)'\\', (byte)'\b', (byte)'\f', (byte)'\n', (byte)'\r', (byte)'\t',
  };

        private const int DEF_STRING_SIZE = 16;

        private static byte[] NAME_BOOL = new byte[] { (byte)'t', (byte)'f' };
        private static byte[] NAME_BYTE = new byte[] { (byte)'i', (byte)'8' };
        private static byte[] NAME_I16 = new byte[] { (byte)'i', (byte)'1', (byte)'6' };
        private static byte[] NAME_I32 = new byte[] { (byte)'i', (byte)'3', (byte)'2' };
        private static byte[] NAME_I64 = new byte[] { (byte)'i', (byte)'6', (byte)'4' };
        private static byte[] NAME_DOUBLE = new byte[] { (byte)'d', (byte)'b', (byte)'l' };
        private static byte[] NAME_STRUCT = new byte[] { (byte)'r', (byte)'e', (byte)'c' };
        private static byte[] NAME_STRING = new byte[] { (byte)'s', (byte)'t', (byte)'r' };
        private static byte[] NAME_MAP = new byte[] { (byte)'m', (byte)'a', (byte)'p' };
        private static byte[] NAME_LIST = new byte[] { (byte)'l', (byte)'s', (byte)'t' };
        private static byte[] NAME_SET = new byte[] { (byte)'s', (byte)'e', (byte)'t' };

        private static byte[] GetTypeNameForTypeID(TType typeID)
        {
            switch (typeID)
            {
                case TType.Bool:
                    return NAME_BOOL;
                case TType.Byte:
                    return NAME_BYTE;
                case TType.I16:
                    return NAME_I16;
                case TType.I32:
                    return NAME_I32;
                case TType.I64:
                    return NAME_I64;
                case TType.Double:
                    return NAME_DOUBLE;
                case TType.String:
                    return NAME_STRING;
                case TType.DataObject:
                    return NAME_STRUCT;
                case TType.Map:
                    return NAME_MAP;
                case TType.List:
                    return NAME_LIST;
                default:
                    throw new SerializerException(SerializerException.NOT_IMPLEMENTED,
                                                 "Unrecognized type");
            }
        }

        private static TType GetTypeIDForTypeName(byte[] name)
        {
            TType result = TType.Stop;
            if (name.Length > 1)
            {
                switch (name[0])
                {
                    case (byte)'d':
                        result = TType.Double;
                        break;
                    case (byte)'i':
                        switch (name[1])
                        {
                            case (byte)'8':
                                result = TType.Byte;
                                break;
                            case (byte)'1':
                                result = TType.I16;
                                break;
                            case (byte)'3':
                                result = TType.I32;
                                break;
                            case (byte)'6':
                                result = TType.I64;
                                break;
                        }
                        break;
                    case (byte)'l':
                        result = TType.List;
                        break;
                    case (byte)'m':
                        result = TType.Map;
                        break;
                    case (byte)'r':
                        result = TType.DataObject;
                        break;
                    case (byte)'s':
                        if (name[1] == (byte)'t')
                        {
                            result = TType.String;
                        }
                        break;
                    case (byte)'t':
                        result = TType.Bool;
                        break;
                }
            }
            if (result == TType.Stop)
            {
                throw new SerializerException(SerializerException.NOT_IMPLEMENTED,
                                             "Unrecognized type");
            }
            return result;
        }

        ///<summary>
        /// Base class for tracking JSON contexts that may require
        /// inserting/Reading additional JSON syntax characters
        /// This base context does nothing.
        ///</summary>
        protected class JSONBaseContext
        {
            protected JSONSerializer proto;

            public JSONBaseContext(JSONSerializer proto)
            {
                this.proto = proto;
            }

            public virtual void Write() { }

            public virtual void Read() { }

            public virtual bool EscapeNumbers() { return false; }
        }

        ///<summary>
        /// Context for JSON lists. Will insert/Read commas before each item except
        /// for the first one
        ///</summary>
        protected class JSONListContext : JSONBaseContext
        {
            public JSONListContext(JSONSerializer serializer)
                : base(serializer)
            {

            }

            private bool first = true;

            public override void Write()
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    proto.Buffer.Write(COMMA, 0, COMMA.Length);
                }
            }

            public override void Read()
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    proto.ReadJSONSyntaxChar(COMMA);
                }
            }
        }

        ///<summary>
        /// Context for JSON records. Will insert/Read colons before the value portion
        /// of each record pair, and commas before each key except the first. In
        /// addition, will indicate that numbers in the key position need to be
        /// escaped in quotes (since JSON keys must be strings).
        ///</summary>
        protected class JSONPairContext : JSONBaseContext
        {
            public JSONPairContext(JSONSerializer proto)
                : base(proto)
            {

            }

            private bool first = true;
            private bool colon = true;

            public override void Write()
            {
                if (first)
                {
                    first = false;
                    colon = true;
                }
                else
                {
                    var buf = colon ? COLON : COMMA;
                    proto.Buffer.Write(buf, 0, buf.Length);
                    colon = !colon;
                }
            }

            public override void Read()
            {
                if (first)
                {
                    first = false;
                    colon = true;
                }
                else
                {
                    proto.ReadJSONSyntaxChar(colon ? COLON : COMMA);
                    colon = !colon;
                }
            }

            public override bool EscapeNumbers()
            {
                return colon;
            }
        }

        ///<summary>
        /// Holds up to one byte from the transport
        ///</summary>
        protected class LookaheadReader
        {
            protected JSONSerializer proto;

            public LookaheadReader(JSONSerializer proto)
            {
                this.proto = proto;
            }

            private bool hasData;
            private byte[] data = new byte[1];

            ///<summary>
            /// Return and consume the next byte to be Read, either taking it from the
            /// data buffer if present or getting it from the transport otherwise.
            ///</summary>
            public byte Read()
            {
                if (hasData)
                {
                    hasData = false;
                }
                else
                {
                    proto.Buffer.Read(data, 0, 1);
                }
                return data[0];
            }

            ///<summary>
            /// Return the next byte to be Read without consuming, filling the data
            /// buffer if it has not been filled alReady.
            ///</summary>
            public byte Peek()
            {
                if (!hasData)
                {
                    proto.Buffer.Read(data, 0, 1);
                }
                hasData = true;
                return data[0];
            }
        }

        // Default encoding
        protected Encoding utf8Encoding = UTF8Encoding.UTF8;

        // Stack of nested contexts that we may be in
        protected Stack<JSONBaseContext> contextStack = new Stack<JSONBaseContext>();

        // Current context that we are in
        protected JSONBaseContext context;

        // Reader that manages a 1-byte buffer
        protected LookaheadReader reader;

        ///<summary>
        /// Push a new JSON context onto the stack.
        ///</summary>
        protected void PushContext(JSONBaseContext c)
        {
            contextStack.Push(context);
            context = c;
        }

        ///<summary>
        /// Pop the last JSON context off the stack
        ///</summary>
        protected void PopContext()
        {
            context = contextStack.Pop();
        }

        ///<summary>
        /// JSONProtocol Constructor
        ///</summary>
        public JSONSerializer(System.IO.Stream stream)
            : base(stream)
        {
            context = new JSONBaseContext(this);
            reader = new LookaheadReader(this);
        }

        // Temporary buffer used by several methods
        private byte[] tempBuffer = new byte[4];

        ///<summary>
        /// Read a byte that must match b[0]; otherwise an exception is thrown.
        /// Marked protected to avoid synthetic accessor in JSONListContext.Read
        /// and JSONPairContext.Read
        ///</summary>
        protected void ReadJSONSyntaxChar(byte[] b)
        {
            byte ch = reader.Read();
            if (ch != b[0])
            {
                throw new SerializerException(SerializerException.INVALID_DATA,
                                             "Unexpected character:" + (char)ch);
            }
        }

        ///<summary>
        /// Convert a byte containing a hex char ('0'-'9' or 'a'-'f') into its
        /// corresponding hex value
        ///</summary>
        private static byte HexVal(byte ch)
        {
            if ((ch >= '0') && (ch <= '9'))
            {
                return (byte)((char)ch - '0');
            }
            else if ((ch >= 'a') && (ch <= 'f'))
            {
                return (byte)((char)ch - 'a');
            }
            else
            {
                throw new SerializerException(SerializerException.INVALID_DATA,
                                             "Expected hex character");
            }
        }

        ///<summary>
        /// Convert a byte containing a hex value to its corresponding hex character
        ///</summary>
        private static byte HexChar(byte val)
        {
            val &= 0x0F;
            if (val < 10)
            {
                return (byte)((char)val + '0');
            }
            else
            {
                return (byte)((char)val + 'a');
            }
        }

        ///<summary>
        /// Write the bytes in array buf as a JSON characters, escaping as needed
        ///</summary>
        private void WriteJSONString(byte[] b)
        {
            context.Write();
            Buffer.Write(QUOTE, 0, QUOTE.Length);
            int len = b.Length;
            for (int i = 0; i < len; i++)
            {
                if ((b[i] & 0x00FF) >= 0x30)
                {
                    if (b[i] == BACKSLASH[0])
                    {
                        Buffer.Write(BACKSLASH, 0, BACKSLASH.Length);
                        Buffer.Write(BACKSLASH, 0, BACKSLASH.Length);
                    }
                    else
                    {
                        Buffer.Write(b, i, 1);
                    }
                }
                else
                {
                    tempBuffer[0] = JSON_CHAR_TABLE[b[i]];
                    if (tempBuffer[0] == 1)
                    {
                        Buffer.Write(b, i, 1);
                    }
                    else if (tempBuffer[0] > 1)
                    {
                        Buffer.Write(BACKSLASH, 0, BACKSLASH.Length);
                        Buffer.Write(tempBuffer, 0, 1);
                    }
                    else
                    {
                        Buffer.Write(ESCSEQ, 0, ESCSEQ.Length);
                        tempBuffer[0] = HexChar((byte)(b[i] >> 4));
                        tempBuffer[1] = HexChar(b[i]);
                        Buffer.Write(tempBuffer, 0, 2);
                    }
                }
            }
            Buffer.Write(QUOTE, 0, QUOTE.Length);
        }

        ///<summary>
        /// Write out number as a JSON value. If the context dictates so, it will be
        /// wrapped in quotes to output as a JSON string.
        ///</summary>
        private void WriteJSONInteger(long num)
        {
            context.Write();
            String str = num.ToString();

            bool escapeNum = context.EscapeNumbers();
            if (escapeNum)
                Buffer.Write(QUOTE, 0, QUOTE.Length);

            var strBytes = utf8Encoding.GetBytes(str);
            Buffer.Write(strBytes, 0, strBytes.Length);

            if (escapeNum)
                Buffer.Write(QUOTE, 0, QUOTE.Length);
        }

        ///<summary>
        /// Write out a double as a JSON value. If it is NaN or infinity or if the
        /// context dictates escaping, Write out as JSON string.
        ///</summary>
        private void WriteJSONDouble(double num)
        {
            context.Write();
            String str = num.ToString(CultureInfo.InvariantCulture);
            bool special = false;

            switch (str[0])
            {
                case 'N': // NaN
                case 'I': // Infinity
                    special = true;
                    break;
                case '-':
                    if (str[1] == 'I')
                    { // -Infinity
                        special = true;
                    }
                    break;
            }

            bool escapeNum = special || context.EscapeNumbers();

            if (escapeNum)
                Buffer.Write(QUOTE, 0, QUOTE.Length);

            var strBytes = utf8Encoding.GetBytes(str);
            Buffer.Write(strBytes, 0, strBytes.Length);

            if (escapeNum)
                Buffer.Write(QUOTE, 0, QUOTE.Length);
        }
        ///<summary>
        /// Write out contents of byte array b as a JSON string with base-64 encoded
        /// data
        ///</summary>
        private void WriteJSONBase64(byte[] b)
        {
            context.Write();
            Buffer.Write(QUOTE, 0, QUOTE.Length);

            int len = b.Length;
            int off = 0;

            while (len >= 3)
            {
                // Encode 3 bytes at a time
                Base64Utils.encode(b, off, 3, tempBuffer, 0);
                Buffer.Write(tempBuffer, 0, 4);
                off += 3;
                len -= 3;
            }
            if (len > 0)
            {
                // Encode remainder
                Base64Utils.encode(b, off, len, tempBuffer, 0);
                Buffer.Write(tempBuffer, 0, len + 1);
            }

            Buffer.Write(QUOTE, 0, QUOTE.Length);
        }

        private void WriteJSONObjectStart()
        {
            context.Write();
            Buffer.Write(LBRACE, 0, LBRACE.Length);
            PushContext(new JSONPairContext(this));
        }

        private void WriteJSONObjectEnd()
        {
            PopContext();
            Buffer.Write(RBRACE, 0, RBRACE.Length);
        }

        private void WriteJSONArrayStart()
        {
            context.Write();
            Buffer.Write(LBRACKET, 0, LBRACKET.Length);
            PushContext(new JSONListContext(this));
        }

        private void WriteJSONArrayEnd()
        {
            PopContext();
            Buffer.Write(RBRACKET, 0, RBRACKET.Length);
        }

        public override void WriteProcedureCallBegin(ProcedureCall message)
        {
            WriteJSONArrayStart();
            WriteJSONInteger(message.Id);
            WriteJSONInteger(message.SequenceNumber);
        }

        public override void WriteProcedureCallEnd()
        {
            WriteJSONArrayEnd();
        }

        public override void WriteDataObjectBegin(DataObject str)
        {
            WriteJSONObjectStart();
        }

        public override void WriteDataObjectEnd()
        {
            WriteJSONObjectEnd();
        }

        public override void WriteFieldBegin(Field field)
        {
            WriteJSONInteger(field.ID);
            WriteJSONObjectStart();
            WriteJSONString(GetTypeNameForTypeID(field.Type));
        }

        public override void WriteFieldEnd()
        {
            WriteJSONObjectEnd();
        }

        public override void WriteFieldStop() { }

        public override void WriteMapBegin(TMap map)
        {
            WriteJSONArrayStart();
            WriteJSONString(GetTypeNameForTypeID(map.KeyType));
            WriteJSONString(GetTypeNameForTypeID(map.ValueType));
            WriteJSONInteger(map.Count);
            WriteJSONObjectStart();
        }

        public override void WriteMapEnd()
        {
            WriteJSONObjectEnd();
            WriteJSONArrayEnd();
        }

        public override void WriteListBegin(TList list)
        {
            WriteJSONArrayStart();
            WriteJSONString(GetTypeNameForTypeID(list.ElementType));
            WriteJSONInteger(list.Count);
        }

        public override void WriteListEnd()
        {
            WriteJSONArrayEnd();
        }

        public override void WriteArrayBegin(TArray array)
        {
            WriteJSONArrayStart();
            WriteJSONString(GetTypeNameForTypeID(array.ElementType));
            WriteJSONInteger(array.Count);
        }

        public override void WriteArrayEnd()
        {
            WriteJSONArrayEnd();
        }

        public override void WriteSetBegin(TSet set)
        {
            WriteJSONArrayStart();
            WriteJSONString(GetTypeNameForTypeID(set.ElementType));
            WriteJSONInteger(set.Count);
        }

        public override void WriteSetEnd()
        {
            WriteJSONArrayEnd();
        }

        public override void WriteCollectionBegin(TCollection collection)
        {
            WriteJSONArrayStart();
            WriteJSONString(GetTypeNameForTypeID(collection.ElementType));
            WriteJSONInteger(collection.Count);
        }

        public override void WriteCollectionEnd()
        {
            WriteJSONArrayEnd();
        }

        public override void WriteBool(bool b)
        {
            WriteJSONInteger(b ? (long)1 : (long)0);
        }

        public override void WriteByte(byte b)
        {
            WriteJSONInteger((long)b);
        }

        public override void WriteI16(short i16)
        {
            WriteJSONInteger((long)i16);
        }

        public override void WriteI32(int i32)
        {
            WriteJSONInteger((long)i32);
        }

        public override void WriteI64(long i64)
        {
            WriteJSONInteger(i64);
        }

        public override void WriteDouble(double dub)
        {
            WriteJSONDouble(dub);
        }

        public override void WriteString(String str)
        {
            byte[] b = utf8Encoding.GetBytes(str);
            WriteJSONString(b);
        }

        public override void WriteBinary(byte[] bin)
        {
            WriteJSONBase64(bin);
        }

        /**
         * Reading methods.
         */

        ///<summary>
        /// Read in a JSON string, unescaping as appropriate.. Skip Reading from the
        /// context if skipContext is true.
        ///</summary>
        private byte[] ReadJSONString(bool skipContext)
        {
            MemoryStream buffer = new MemoryStream();


            if (!skipContext)
            {
                context.Read();
            }
            ReadJSONSyntaxChar(QUOTE);
            while (true)
            {
                byte ch = reader.Read();
                if (ch == QUOTE[0])
                {
                    break;
                }
                if (ch == ESCSEQ[0])
                {
                    ch = reader.Read();
                    if (ch == ESCSEQ[1])
                    {
                        ReadJSONSyntaxChar(ZERO);
                        ReadJSONSyntaxChar(ZERO);
                        Buffer.Read(tempBuffer, 0, 2);
                        ch = (byte)((HexVal((byte)tempBuffer[0]) << 4) + HexVal(tempBuffer[1]));
                    }
                    else
                    {
                        int off = Array.IndexOf(ESCAPE_CHARS, (char)ch);
                        if (off == -1)
                        {
                            throw new SerializerException(SerializerException.INVALID_DATA,
                                                         "Expected control char");
                        }
                        ch = ESCAPE_CHAR_VALS[off];
                    }
                }
                buffer.Write(new byte[] { (byte)ch }, 0, 1);
            }
            return buffer.ToArray();
        }

        ///<summary>
        /// Return true if the given byte could be a valid part of a JSON number.
        ///</summary>
        private bool IsJSONNumeric(byte b)
        {
            switch (b)
            {
                case (byte)'+':
                case (byte)'-':
                case (byte)'.':
                case (byte)'0':
                case (byte)'1':
                case (byte)'2':
                case (byte)'3':
                case (byte)'4':
                case (byte)'5':
                case (byte)'6':
                case (byte)'7':
                case (byte)'8':
                case (byte)'9':
                case (byte)'E':
                case (byte)'e':
                    return true;
            }
            return false;
        }

        ///<summary>
        /// Read in a sequence of characters that are all valid in JSON numbers. Does
        /// not do a complete regex check to validate that this is actually a number.
        ////</summary>
        private String ReadJSONNumericChars()
        {
            StringBuilder strbld = new StringBuilder();
            while (true)
            {
                byte ch = reader.Peek();
                if (!IsJSONNumeric(ch))
                {
                    break;
                }
                strbld.Append((char)reader.Read());
            }
            return strbld.ToString();
        }

        ///<summary>
        /// Read in a JSON number. If the context dictates, Read in enclosing quotes.
        ///</summary>
        private long ReadJSONInteger()
        {
            context.Read();
            if (context.EscapeNumbers())
            {
                ReadJSONSyntaxChar(QUOTE);
            }
            String str = ReadJSONNumericChars();
            if (context.EscapeNumbers())
            {
                ReadJSONSyntaxChar(QUOTE);
            }
            try
            {
                return Int64.Parse(str);
            }
            catch (FormatException)
            {
                throw new SerializerException(SerializerException.INVALID_DATA,
                                             "Bad data encounted in numeric data");
            }
        }

        ///<summary>
        /// Read in a JSON double value. Throw if the value is not wrapped in quotes
        /// when expected or if wrapped in quotes when not expected.
        ///</summary>
        private double ReadJSONDouble()
        {
            context.Read();
            if (reader.Peek() == QUOTE[0])
            {
                byte[] arr = ReadJSONString(true);
                double dub = Double.Parse(utf8Encoding.GetString(arr,0,arr.Length), CultureInfo.InvariantCulture);

                if (!context.EscapeNumbers() && !Double.IsNaN(dub) &&
                    !Double.IsInfinity(dub))
                {
                    // Throw exception -- we should not be in a string in this case
                    throw new SerializerException(SerializerException.INVALID_DATA,
                                                 "Numeric data unexpectedly quoted");
                }
                return dub;
            }
            else
            {
                if (context.EscapeNumbers())
                {
                    // This will throw - we should have had a quote if escapeNum == true
                    ReadJSONSyntaxChar(QUOTE);
                }
                try
                {
                    return Double.Parse(ReadJSONNumericChars());
                }
                catch (FormatException)
                {
                    throw new SerializerException(SerializerException.INVALID_DATA,
                                                 "Bad data encounted in numeric data");
                }
            }
        }

        //<summary>
        /// Read in a JSON string containing base-64 encoded data and decode it.
        ///</summary>
        private byte[] ReadJSONBase64()
        {
            byte[] b = ReadJSONString(false);
            int len = b.Length;
            int off = 0;
            int size = 0;
            while (len >= 4)
            {
                // Decode 4 bytes at a time
                Base64Utils.decode(b, off, 4, b, size); // NB: decoded in place
                off += 4;
                len -= 4;
                size += 3;
            }
            // Don't decode if we hit the end or got a single leftover byte (invalid
            // base64 but legal for skip of regular string type)
            if (len > 1)
            {
                // Decode remainder
                Base64Utils.decode(b, off, len, b, size); // NB: decoded in place
                size += len - 1;
            }
            // Sadly we must copy the byte[] (any way around this?)
            byte[] result = new byte[size];
            Array.Copy(b, 0, result, 0, size);
            return result;
        }

        private void ReadJSONObjectStart()
        {
            context.Read();
            ReadJSONSyntaxChar(LBRACE);
            PushContext(new JSONPairContext(this));
        }

        private void ReadJSONObjectEnd()
        {
            ReadJSONSyntaxChar(RBRACE);
            PopContext();
        }

        private void ReadJSONArrayStart()
        {
            context.Read();
            ReadJSONSyntaxChar(LBRACKET);
            PushContext(new JSONListContext(this));
        }

        private void ReadJSONArrayEnd()
        {
            ReadJSONSyntaxChar(RBRACKET);
            PopContext();
        }

        public override ProcedureCall ReadProcedureCallBegin()
        {
            ProcedureCall message = new ProcedureCall();
            ReadJSONArrayStart();
            message.Id = (int)ReadJSONInteger();
            message.SequenceNumber = (int)ReadJSONInteger();
            return message;
        }

        public override void ReadProcedureCallEnd()
        {
            ReadJSONArrayEnd();
        }

        public override DataObject ReadDataObjectBegin()
        {
            ReadJSONObjectStart();
            return new DataObject();
        }

        public override void ReadDataObjectEnd()
        {
            ReadJSONObjectEnd();
        }

        public override Field ReadFieldBegin()
        {
            Field field = new Field();
            byte ch = reader.Peek();
            if (ch == RBRACE[0])
            {
                field.Type = TType.Stop;
            }
            else
            {
                field.ID = (short)ReadJSONInteger();
                ReadJSONObjectStart();
                field.Type = GetTypeIDForTypeName(ReadJSONString(false));
            }
            return field;
        }

        public override void ReadFieldEnd()
        {
            ReadJSONObjectEnd();
        }

        public override TMap ReadMapBegin()
        {
            TMap map = new TMap();
            ReadJSONArrayStart();
            map.KeyType = GetTypeIDForTypeName(ReadJSONString(false));
            map.ValueType = GetTypeIDForTypeName(ReadJSONString(false));
            map.Count = (int)ReadJSONInteger();
            ReadJSONObjectStart();
            return map;
        }

        public override void ReadMapEnd()
        {
            ReadJSONObjectEnd();
            ReadJSONArrayEnd();
        }

        public override TList ReadListBegin()
        {
            TList list = new TList();
            ReadJSONArrayStart();
            list.ElementType = GetTypeIDForTypeName(ReadJSONString(false));
            list.Count = (int)ReadJSONInteger();
            return list;
        }

        public override void ReadListEnd()
        {
            ReadJSONArrayEnd();
        }

        public override TArray ReadArrayBegin()
        {
            TArray array = new TArray();
            ReadJSONArrayStart();
            array.ElementType = GetTypeIDForTypeName(ReadJSONString(false));
            array.Count = (int)ReadJSONInteger();
            return array;
        }

        public override void ReadArrayEnd()
        {
            ReadJSONArrayEnd();
        }

        public override TSet ReadSetBegin()
        {
            TSet set = new TSet();
            ReadJSONArrayStart();
            set.ElementType = GetTypeIDForTypeName(ReadJSONString(false));
            set.Count = (int)ReadJSONInteger();
            return set;
        }

        public override void ReadSetEnd()
        {
            ReadJSONArrayEnd();
        }

        public override TCollection ReadCollectionBegin()
        {
            TCollection collection = new TCollection();
            ReadJSONArrayStart();
            collection.ElementType = GetTypeIDForTypeName(ReadJSONString(false));
            collection.Count = (int)ReadJSONInteger();
            return collection;
        }

        public override void ReadCollectionEnd()
        {
            ReadJSONArrayEnd();
        }

        public override bool ReadBool()
        {
            return (ReadJSONInteger() == 0 ? false : true);
        }

        public override byte ReadByte()
        {
            return (byte)ReadJSONInteger();
        }

        public override short ReadI16()
        {
            return (short)ReadJSONInteger();
        }

        public override int ReadI32()
        {
            return (int)ReadJSONInteger();
        }

        public override long ReadI64()
        {
            return (long)ReadJSONInteger();
        }

        public override double ReadDouble()
        {
            return ReadJSONDouble();
        }

        public override String ReadString()
        {
            var buf = ReadJSONString(false);
            return utf8Encoding.GetString(buf,0,buf.Length);
        }

        public override byte[] ReadBinary()
        {
            return ReadJSONBase64();
        }

    }
}
