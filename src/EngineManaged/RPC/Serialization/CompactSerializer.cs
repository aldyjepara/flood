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
 *
 * Contains some contributions under the Thrift Software License.
 * Please see doc/old-thrift-license.txt in the Thrift distribution for
 * details.
 */

using System;
using System.Text;
using System.Collections.Generic;

namespace Flood.RPC.Serialization
{
    public class CompactSerializer : Serializer
    {
        private static DataObject ANONYMOUS_STRUCT = new DataObject("");
        private static Field TSTOP = new Field("", TType.Stop, (short)0);

        private static byte[] ttypeToCompactType = new byte[16];

        /**
         * All of the on-wire type codes.
         */
        private static class Types
        {
            public const byte STOP = 0x00;
            public const byte BOOLEAN_TRUE = 0x01;
            public const byte BOOLEAN_FALSE = 0x02;
            public const byte BYTE = 0x03;
            public const byte I16 = 0x04;
            public const byte I32 = 0x05;
            public const byte I64 = 0x06;
            public const byte DOUBLE = 0x07;
            public const byte BINARY = 0x08;
            public const byte LIST = 0x09;
            public const byte SET = 0x0A;
            public const byte MAP = 0x0B;
            public const byte DATA_OBJECT = 0x0C;
        }

        /** 
         * Used to keep track of the last field for the current and previous structs,
         * so we can do the delta stuff.
         */
        private Stack<short> lastField_ = new Stack<short>(15);

        private short lastFieldId_ = 0;

        /**
         * If we encounter a boolean field begin, save the TField here so it can 
         * have the value incorporated.
         */
        private Nullable<Field> booleanField_;

        /**
         * If we Read a field header, and it's a boolean field, save the boolean 
         * value here so that ReadBool can use it.
         */
        private  Nullable<Boolean> boolValue_;


        public CompactSerializer()
        {
            ttypeToCompactType[(int)TType.Stop] = Types.STOP;
            ttypeToCompactType[(int)TType.Bool] = Types.BOOLEAN_TRUE;
            ttypeToCompactType[(int)TType.Byte] = Types.BYTE;
            ttypeToCompactType[(int)TType.I16] = Types.I16;
            ttypeToCompactType[(int)TType.I32] = Types.I32;
            ttypeToCompactType[(int)TType.I64] = Types.I64;
            ttypeToCompactType[(int)TType.Double] = Types.DOUBLE;
            ttypeToCompactType[(int)TType.String] = Types.BINARY;
            ttypeToCompactType[(int)TType.List] = Types.LIST;
            ttypeToCompactType[(int)TType.Map] = Types.MAP;
            ttypeToCompactType[(int)TType.DataObject] = Types.DATA_OBJECT;
        }

        public void reset()
        {
            lastField_.Clear();
            lastFieldId_ = 0;
        }

        #region Write Methods


        /** 
         * Writes a byte without any possibility of all that field header nonsense. 
         * Used internally by other writing methods that know they need to Write a byte.
         */
        private byte[] byteDirectBuffer = new byte[1];
        private void WriteByteDirect(byte b)
        {
            byteDirectBuffer[0] = b;
            Buffer.Write(byteDirectBuffer);
        }

        /** 
         * Writes a byte without any possibility of all that field header nonsense.
         */
        private void WriteByteDirect(int n)
        {
            WriteByteDirect((byte)n);
        }

        /**
         * Write an i32 as a varint. Results in 1-5 bytes on the wire.
         * TODO: make a permanent buffer like WriteVarint64?
         */
        byte[] i32buf = new byte[5];
        private void WriteVarint32(uint n)
        {
            int idx = 0;
            while (true)
            {
                if ((n & ~0x7F) == 0)
                {
                    i32buf[idx++] = (byte)n;
                    // WriteByteDirect((byte)n);
                    break;
                    // return;
                }
                else
                {
                    i32buf[idx++] = (byte)((n & 0x7F) | 0x80);
                    // WriteByteDirect((byte)((n & 0x7F) | 0x80));
                    n >>= 7;
                }
            }
            Buffer.Write(i32buf, 0, idx);
        }

        /**
        * Write a message header to the wire. Compact Protocol messages contain the
        * protocol version so we can migrate forwards in the future if need be.
        */
        public override void WriteProcedureCallBegin(ProcedureCall message)
        {
            WriteVarint32((uint)message.Id);
        }

        /**
         * Write a struct begin. This doesn't actually put anything on the wire. We 
         * use it as an opportunity to put special placeholder markers on the field
         * stack so we can get the field id deltas correct.
         */
        public override void WriteDataObjectBegin(DataObject strct)
        {
            lastField_.Push(lastFieldId_);
            lastFieldId_ = 0;
        }

        /**
         * Write a struct end. This doesn't actually put anything on the wire. We use
         * this as an opportunity to pop the last field from the current struct off
         * of the field stack.
         */
        public override void WriteDataObjectEnd()
        {
            lastFieldId_ = lastField_.Pop();
        }

        /**
         * Write a field header containing the field id and field type. If the
         * difference between the current field id and the last one is small (< 15),
         * then the field id will be encoded in the 4 MSB as a delta. Otherwise, the
         * field id will follow the type header as a zigzag varint.
         */
        public override void WriteFieldBegin(Field field)
        {
            if (field.Type == TType.Bool)
            {
                // we want to possibly include the value, so we'll wait.
                booleanField_ = field;
            }
            else
            {
                WriteFieldBeginInternal(field, 0xFF);
            }
        }

        /**
         * The workhorse of WriteFieldBegin. It has the option of doing a 
         * 'type override' of the type header. This is used specifically in the 
         * boolean field case.
         */
        private void WriteFieldBeginInternal(Field field, byte typeOverride)
        {
            // short lastField = lastField_.Pop();

            // if there's a type override, use that.
            byte typeToWrite = typeOverride == 0xFF ? getCompactType(field.Type) : typeOverride;

            // check if we can use delta encoding for the field id
            if (field.ID > lastFieldId_ && field.ID - lastFieldId_ <= 15)
            {
                // Write them together
                WriteByteDirect((field.ID - lastFieldId_) << 4 | typeToWrite);
            }
            else
            {
                // Write them separate
                WriteByteDirect(typeToWrite);
                WriteI16(field.ID);
            }

            lastFieldId_ = field.ID;
            // lastField_.push(field.id);
        }

        /**
         * Write the STOP symbol so we know there are no more fields in this struct.
         */
        public override void WriteFieldStop()
        {
            WriteByteDirect(Types.STOP);
        }

        /**
         * Write a map header. If the map is empty, omit the key and value type 
         * headers, as we don't need any additional information to skip it.
         */
        public override void WriteMapBegin(TMap map)
        {
            if (map.Count == 0)
            {
                WriteByteDirect(0);
            }
            else
            {
                WriteVarint32((uint)map.Count);
                WriteByteDirect(getCompactType(map.KeyType) << 4 | getCompactType(map.ValueType));
            }
        }

        /** 
         * Write a array header.
         */
        public override void WriteListBegin(TList list)
        {
            WriteCollectionBegin(list.ElementType, list.Count);
        }

        /** 
         * Write a array header.
         */
        public override void WriteArrayBegin(TArray array)
        {
            WriteCollectionBegin(array.ElementType, array.Count);
        }


        /**
         * Write a set header.
         */
        public override void WriteSetBegin(TSet set)
        {
            WriteCollectionBegin(set.ElementType, set.Count);
        }

        /**
        * Write a collection header.
        */
        public override void WriteCollectionBegin(TCollection collection)
        {
            WriteCollectionBegin(collection.ElementType, collection.Count);
        }

        /**
         * Write a boolean value. Potentially, this could be a boolean field, in 
         * which case the field header info isn't written yet. If so, decide what the
         * right type header is for the value and then Write the field header. 
         * Otherwise, Write a single byte.
         */
        public override void WriteBool(Boolean b)
        {
            if (booleanField_ != null)
            {
                // we haven't written the field header yet
                WriteFieldBeginInternal(booleanField_.Value, b ? Types.BOOLEAN_TRUE : Types.BOOLEAN_FALSE);
                booleanField_ = null;
            }
            else
            {
                // we're not part of a field, so just Write the value.
                WriteByteDirect(b ? Types.BOOLEAN_TRUE : Types.BOOLEAN_FALSE);
            }
        }

        /** 
         * Write a byte. Nothing to see here!
         */
        public override void WriteByte(byte b)
        {
            WriteByteDirect(b);
        }

        /**
         * Write an I16 as a zigzag varint.
         */
        public override void WriteI16(short i16)
        {
            WriteVarint32(intToZigZag(i16));
        }

        /**
         * Write an i32 as a zigzag varint.
         */
        public override void WriteI32(int i32)
        {
            WriteVarint32(intToZigZag(i32));
        }

        /**
         * Write an i64 as a zigzag varint.
         */
        public override void WriteI64(long i64)
        {
            WriteVarint64(longToZigzag(i64));
        }

        /**
         * Write a double to the wire as 8 bytes.
         */
        public override void WriteDouble(double dub)
        {
            byte[] data = new byte[] { 0, 0, 0, 0, 0, 0, 0, 0 };
            fixedLongToBytes(BitConverter.DoubleToInt64Bits(dub), data, 0);
            Buffer.Write(data);
        }

        /**
         * Write a string to the wire with a varint size preceding.
         */
        public override void WriteString(String str)
        {
            byte[] bytes = UTF8Encoding.UTF8.GetBytes(str);
            WriteBinary(bytes, 0, bytes.Length);
        }

        /**
         * Write a byte array, using a varint for the size. 
         */
        public override void WriteBinary(byte[] bin)
        {
            WriteBinary(bin, 0, bin.Length);
        }

        private void WriteBinary(byte[] buf, int offset, int length)
        {
            WriteVarint32((uint)length);
            Buffer.Write(buf, offset, length);
        }

        //
        // These methods are called by structs, but don't actually have any wire 
        // output or purpose.
        // 

        public override void WriteProcedureCallEnd() { }
        public override void WriteMapEnd() { }
        public override void WriteListEnd() { }
        public override void WriteArrayEnd() { }
        public override void WriteSetEnd() { }
        public override void WriteCollectionEnd() { }
        public override void WriteFieldEnd() { }

        //
        // Internal writing methods
        //

        /**
         * Abstract method for writing the start of lists and sets. List and sets on 
         * the wire differ only by the type indicator.
         */
        protected void WriteCollectionBegin(TType elemType, int size)
        {
            if (size <= 14)
            {
                WriteByteDirect(size << 4 | getCompactType(elemType));
            }
            else
            {
                WriteByteDirect(0xf0 | getCompactType(elemType));
                WriteVarint32((uint)size);
            }
        }

        /**
         * Write an i64 as a varint. Results in 1-10 bytes on the wire.
         */
        byte[] varint64out = new byte[10];
        private void WriteVarint64(ulong n)
        {
            int idx = 0;
            while (true)
            {
                if ((n & ~(ulong)0x7FL) == 0)
                {
                    varint64out[idx++] = (byte)n;
                    break;
                }
                else
                {
                    varint64out[idx++] = ((byte)((n & 0x7F) | 0x80));
                    n >>= 7;
                }
            }
            Buffer.Write(varint64out, 0, idx);
        }

        /**
         * Convert l into a zigzag long. This allows negative numbers to be 
         * represented compactly as a varint.
         */
        private ulong longToZigzag(long n)
        {
            return (ulong)(((ulong)n << 1) ^ ((ulong)n >> 63));
        }

        /**
         * Convert n into a zigzag int. This allows negative numbers to be 
         * represented compactly as a varint.
         */
        private uint intToZigZag(int n)
        {
            return (uint)(((uint)n << 1) ^ ((uint)n >> 31));
        }

        /**
         * Convert a long into little-endian bytes in buf starting at off and going 
         * until off+7.
         */
        private void fixedLongToBytes(long n, byte[] buf, int off)
        {
            buf[off + 0] = (byte)(n & 0xff);
            buf[off + 1] = (byte)((n >> 8) & 0xff);
            buf[off + 2] = (byte)((n >> 16) & 0xff);
            buf[off + 3] = (byte)((n >> 24) & 0xff);
            buf[off + 4] = (byte)((n >> 32) & 0xff);
            buf[off + 5] = (byte)((n >> 40) & 0xff);
            buf[off + 6] = (byte)((n >> 48) & 0xff);
            buf[off + 7] = (byte)((n >> 56) & 0xff);
        }

        #endregion

        #region ReadMethods

        /**
   * Read a message header. 
   */
        public override ProcedureCall ReadProcedureCallBegin()
        {
            int messageId = (int)ReadVarint32();
            return new ProcedureCall(messageId);
        }

        /**
         * Read a struct begin. There's nothing on the wire for this, but it is our
         * opportunity to push a new struct begin marker onto the field stack.
         */
        public override DataObject ReadDataObjectBegin()
        {
            lastField_.Push(lastFieldId_);
            lastFieldId_ = 0;
            return ANONYMOUS_STRUCT;
        }

        /**
         * Doesn't actually consume any wire data, just removes the last field for 
         * this struct from the field stack.
         */
        public override void ReadDataObjectEnd()
        {
            // consume the last field we Read off the wire.
            lastFieldId_ = lastField_.Pop();
        }

        /**
         * Read a field header off the wire. 
         */
        public override Field ReadFieldBegin()
        {
            byte type = ReadByte();

            // if it's a stop, then we can return immediately, as the struct is over.
            if (type == Types.STOP)
            {
                return TSTOP;
            }

            short fieldId;

            // mask off the 4 MSB of the type header. it could contain a field id delta.
            short modifier = (short)((type & 0xf0) >> 4);
            if (modifier == 0)
            {
                // not a delta. look ahead for the zigzag varint field id.
                fieldId = ReadI16();
            }
            else
            {
                // has a delta. add the delta to the last Read field id.
                fieldId = (short)(lastFieldId_ + modifier);
            }

            Field field = new Field("", getTType((byte)(type & 0x0f)), fieldId);

            // if this happens to be a boolean field, the value is encoded in the type
            if (isBoolType(type))
            {
                // save the boolean value in a special instance variable.
                boolValue_ = (byte)(type & 0x0f) == Types.BOOLEAN_TRUE ? true : false;
            }

            // push the new field onto the field stack so we can keep the deltas going.
            lastFieldId_ = field.ID;
            return field;
        }

        /** 
         * Read a map header off the wire. If the size is zero, skip Reading the key
         * and value type. This means that 0-length maps will yield TMaps without the
         * "correct" types.
         */
        public override TMap ReadMapBegin()
        {
            int size = (int)ReadVarint32();
            byte keyAndValueType = size == 0 ? (byte)0 : ReadByte();
            return new TMap(getTType((byte)(keyAndValueType >> 4)), getTType((byte)(keyAndValueType & 0xf)), size);
        }

        /**
         * Read a array header off the wire. If the array size is 0-14, the size will 
         * be packed into the element type header. If it's a longer array, the 4 MSB
         * of the element type header will be 0xF, and a varint will follow with the
         * true size.
         */
        public override TList ReadListBegin()
        {
            byte size_and_type = ReadByte();
            int size = (size_and_type >> 4) & 0x0f;
            if (size == 15)
            {
                size = (int)ReadVarint32();
            }
            TType type = getTType(size_and_type);
            return new TList(type, size);
        }

        /**
         * Read a array header off the wire. If the array size is 0-14, the size will 
         * be packed into the element type header. If it's a longer array, the 4 MSB
         * of the element type header will be 0xF, and a varint will follow with the
         * true size.
         */
        public override TArray ReadArrayBegin()
        {
            byte size_and_type = ReadByte();
            int size = (size_and_type >> 4) & 0x0f;
            if (size == 15)
            {
                size = (int)ReadVarint32();
            }
            TType type = getTType(size_and_type);
            return new TArray(type, size);
        }

        /**
         * Read a set header off the wire. If the set size is 0-14, the size will 
         * be packed into the element type header. If it's a longer set, the 4 MSB
         * of the element type header will be 0xF, and a varint will follow with the
         * true size.
         */
        public override TSet ReadSetBegin()
        {
            return new TSet(ReadListBegin());
        }

        /**
         * Read a collection header off the wire. If the collection size is 0-14, the size will 
         * be packed into the element type header. If it's a longer collection, the 4 MSB
         * of the element type header will be 0xF, and a varint will follow with the
         * true size.
         */
        public override TCollection ReadCollectionBegin()
        {
            return new TCollection(ReadListBegin());
        }

        /**
         * Read a boolean off the wire. If this is a boolean field, the value should
         * already have been Read during ReadFieldBegin, so we'll just consume the
         * pre-stored value. Otherwise, Read a byte.
         */
        public override Boolean ReadBool()
        {
            if (boolValue_ != null)
            {
                bool result = boolValue_.Value;
                boolValue_ = null;
                return result;
            }
            return ReadByte() == Types.BOOLEAN_TRUE;
        }

        byte[] byteRawBuf = new byte[1];
        /**
         * Read a single byte off the wire. Nothing interesting here.
         */
        public override byte ReadByte()
        {
            Buffer.Read(byteRawBuf, 0, 1);
            return byteRawBuf[0];
        }

        /**
         * Read an i16 from the wire as a zigzag varint.
         */
        public override short ReadI16()
        {
            return (short)zigzagToInt(ReadVarint32());
        }

        /**
         * Read an i32 from the wire as a zigzag varint.
         */
        public override int ReadI32()
        {
            return zigzagToInt(ReadVarint32());
        }

        /**
         * Read an i64 from the wire as a zigzag varint.
         */
        public override long ReadI64()
        {
            return zigzagToLong(ReadVarint64());
        }

        /**
         * No magic here - just Read a double off the wire.
         */
        public override double ReadDouble()
        {
            byte[] longBits = new byte[8];
            Buffer.Read(longBits, 0, 8);
            return BitConverter.Int64BitsToDouble(bytesToLong(longBits));
        }

        /**
         * Reads a byte[] (via ReadBinary), and then UTF-8 decodes it.
         */
        public override String ReadString()
        {
            int length = (int)ReadVarint32();

            if (length == 0)
            {
                return "";
            }

            return Encoding.UTF8.GetString(ReadBinary(length));
        }

        /**
         * Read a byte[] from the wire. 
         */
        public override byte[] ReadBinary()
        {
            int length = (int)ReadVarint32();
            if (length == 0) return new byte[0];

            byte[] buf = new byte[length];
            Buffer.Read(buf, 0, length);
            return buf;
        }

        /**
         * Read a byte[] of a known length from the wire. 
         */
        private byte[] ReadBinary(int length)
        {
            if (length == 0) return new byte[0];

            byte[] buf = new byte[length];
            Buffer.Read(buf, 0, length);
            return buf;
        }

        //
        // These methods are here for the struct to call, but don't have any wire 
        // encoding.
        //
        public override void ReadProcedureCallEnd() { }
        public override void ReadFieldEnd() { }
        public override void ReadMapEnd() { }
        public override void ReadArrayEnd() { }
        public override void ReadListEnd() { }
        public override void ReadSetEnd() { }
        public override void ReadCollectionEnd() { }

        //
        // Internal Reading methods
        //

        /**
         * Read an i32 from the wire as a varint. The MSB of each byte is collection
         * if there is another byte to follow. This can Read up to 5 bytes.
         */
        private uint ReadVarint32()
        {
            uint result = 0;
            int shift = 0;
            while (true)
            {
                byte b = ReadByte();
                result |= (uint)(b & 0x7f) << shift;
                if ((b & 0x80) != 0x80) break;
                shift += 7;
            }
            return result;
        }

        /**
         * Read an i64 from the wire as a proper varint. The MSB of each byte is collection 
         * if there is another byte to follow. This can Read up to 10 bytes.
         */
        private ulong ReadVarint64()
        {
            int shift = 0;
            ulong result = 0;
            while (true)
            {
                byte b = ReadByte();
                result |= (ulong)(b & 0x7f) << shift;
                if ((b & 0x80) != 0x80) break;
                shift += 7;
            }
            
            return result;
        }

        #endregion

        //
        // encoding helpers
        //

        /**
         * Convert from zigzag int to int.
         */
        private int zigzagToInt(uint n)
        {
            return (int)(n >> 1) ^ (-(int)(n & 1));
        }

        /** 
         * Convert from zigzag long to long.
         */
        private long zigzagToLong(ulong n)
        {
            return (long)(n >> 1) ^ (-(long)(n & 1));
        }

        /**
         * Note that it's important that the mask bytes are long literals, 
         * otherwise they'll default to ints, and when you shift an int left 56 bits,
         * you just get a messed up int.
         */
        private long bytesToLong(byte[] bytes)
        {
            return
              ((bytes[7] & 0xffL) << 56) |
              ((bytes[6] & 0xffL) << 48) |
              ((bytes[5] & 0xffL) << 40) |
              ((bytes[4] & 0xffL) << 32) |
              ((bytes[3] & 0xffL) << 24) |
              ((bytes[2] & 0xffL) << 16) |
              ((bytes[1] & 0xffL) << 8) |
              ((bytes[0] & 0xffL));
        }

        //
        // type testing and converting
        //

        private Boolean isBoolType(byte b)
        {
            int lowerNibble = b & 0x0f;
            return lowerNibble == Types.BOOLEAN_TRUE || lowerNibble == Types.BOOLEAN_FALSE;
        }

        /**
         * Given a TCompactProtocol.Types constant, convert it to its corresponding 
         * TType value.
         */
        private TType getTType(byte type)
        {
            switch ((byte)(type & 0x0f))
            {
                case Types.STOP:
                    return TType.Stop;
                case Types.BOOLEAN_FALSE:
                case Types.BOOLEAN_TRUE:
                    return TType.Bool;
                case Types.BYTE:
                    return TType.Byte;
                case Types.I16:
                    return TType.I16;
                case Types.I32:
                    return TType.I32;
                case Types.I64:
                    return TType.I64;
                case Types.DOUBLE:
                    return TType.Double;
                case Types.BINARY:
                    return TType.String;
                case Types.LIST:
                    return TType.List;
                case Types.MAP:
                    return TType.Map;
                case Types.DATA_OBJECT:
                    return TType.DataObject;
                default:
                    throw new SerializerException("don't know what type: " + (byte)(type & 0x0f));
            }
        }

        /**
         * Given a TType value, find the appropriate TCompactProtocol.Types constant.
         */
        private byte getCompactType(TType ttype)
        {
            return ttypeToCompactType[(int)ttype];
        }
    }
}
