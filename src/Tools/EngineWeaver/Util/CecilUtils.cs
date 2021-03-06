﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Mono.Cecil;
using Mono.Cecil.Pdb;
using System.IO;

namespace EngineWeaver.Util
{
    public class CecilUtils
    {
        private static Dictionary<string, AssemblyDefinition> assemblies = new Dictionary<string, AssemblyDefinition>();
        private static Dictionary<string, TypeDefinition> types = new Dictionary<string, TypeDefinition>();

        public static AssemblyDefinition GetAssemblyDef(Assembly assembly)
        {
            var assemblyPath = assembly.Location;
            if(assemblyPath == null)
                throw new NullReferenceException();

            return GetAssemblyDef(assemblyPath);
        }

        public static AssemblyDefinition GetAssemblyDef(string assemblyPath)
        {
            if (assemblies.ContainsKey(assemblyPath))
                return assemblies[assemblyPath];
          
            var assemblyResolver = new DefaultAssemblyResolver();
            var assemblyLocation = Path.GetDirectoryName(assemblyPath);
            assemblyResolver.AddSearchDirectory(assemblyLocation);

            var readerParameters = new ReaderParameters { AssemblyResolver = assemblyResolver };

            var pdbName = Path.ChangeExtension(assemblyPath, "pdb");
            if (File.Exists(pdbName))
            {
                var symbolReaderProvider = new PdbReaderProvider();
                readerParameters.SymbolReaderProvider = symbolReaderProvider;
                readerParameters.ReadSymbols = true;
            }

            var assemblyDef = AssemblyDefinition.ReadAssembly(assemblyPath, readerParameters);

            assemblies.Add(assemblyPath,assemblyDef);

            return assemblyDef;
        }

        public static TypeDefinition GetTypeDef(Type type)
        {
            var assemblyPath = type.Assembly.Location;
            if(assemblyPath == null)
                throw new NullReferenceException();

            var typePath = assemblyPath + type.FullName;
            if(types.ContainsKey(typePath))
                return types[typePath];

            var assemblyDef = GetAssemblyDef(assemblyPath);

            var module = assemblyDef.MainModule;
            var typeDef = module.Import(type).Resolve();
            types.Add(typePath, typeDef);

            return typeDef;
        }

        public static MethodDefinition GetTypeMethodDef(TypeDefinition destType, MethodReference origMethod)
        {
            foreach (var destMethod in destType.Methods)
            {
                if(destMethod.Name != origMethod.Name)
                    continue;

                if(destMethod.Parameters.Count != origMethod.Parameters.Count)
                    continue;

                var @continue = false;
                for (var i = 0; i < destMethod.Parameters.Count; i++)
                {
                    if (destMethod.Parameters[i].ParameterType.FullName != origMethod.Parameters[i].ParameterType.FullName)
                    {
                        @continue = true;
                        break;
                    }
                }

                if(@continue)
                    continue;

                return destMethod;
            }

            return null;
        }

        public static FieldDefinition GetTypeFieldDef(TypeDefinition destType, FieldReference origField)
        {
            foreach (var destField in destType.Fields)
            {
                if(destField.Name != origField.Name)
                    continue;

                if(destField.FieldType.FullName != origField.FieldType.FullName)
                    continue;

                return destField;
            }

            return null;
        }
    }
}
