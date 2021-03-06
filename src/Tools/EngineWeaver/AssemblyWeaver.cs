﻿using EngineWeaver.Util;
using Mono.Cecil;

namespace EngineWeaver
{
    public class AssemblyWeaver
    {
        private AssemblyDefinition destAssembly;

        public AssemblyWeaver(string destAssemblyPath)
        {
            destAssembly = CecilUtils.GetAssemblyDef(destAssemblyPath);
        }

        public void AddAssembly(string origAssemblyPath)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);
            var copier = new CecilCopier(origAssembly.MainModule, destAssembly.MainModule);

            foreach (var origType in origAssembly.MainModule.Types)
                if(origType.BaseType != null)
                    copier.Copy(origType);

            copier.Process();
        }

        public void Write(string outputAssemblyPath)
        {
            var writerParameters = new WriterParameters();
            writerParameters.WriteSymbols = destAssembly.MainModule.HasSymbols;
            writerParameters.SymbolWriterProvider = new Mono.Cecil.Pdb.PdbWriterProvider();
            destAssembly.Write(outputAssemblyPath, writerParameters);
        }

    }
}
