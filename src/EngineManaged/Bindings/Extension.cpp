/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Extension.h"
#include "Reflection.h"
#include "Memory.h"
#include "Serialization.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::ExtensionMetadata::ExtensionMetadata(::ExtensionMetadata* native)
{
    Name = clix::marshalString<clix::E_UTF8>(native->name);
    Description = clix::marshalString<clix::E_UTF8>(native->description);
    Priority = native->priority;
}

Flood::ExtensionMetadata::ExtensionMetadata(System::IntPtr native)
{
    auto __native = (::ExtensionMetadata*)native.ToPointer();
    Name = clix::marshalString<clix::E_UTF8>(__native->name);
    Description = clix::marshalString<clix::E_UTF8>(__native->description);
    Priority = __native->priority;
}

Flood::Extension::Extension(::Extension* native)
{
    NativePtr = native;
}

Flood::Extension::Extension(System::IntPtr native)
{
    auto __native = (::Extension*)native.ToPointer();
    NativePtr = __native;
}

Flood::Extension::Extension()
{
}

Flood::Class^ Flood::Extension::GetType()
{
    auto ret = NativePtr->getType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::Class^ Flood::Extension::GetStaticType()
{
    auto ret = NativePtr->getStaticType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::ExtensionMetadata Flood::Extension::GetMetadata()
{
    auto ret = NativePtr->getMetadata();
    return Flood::ExtensionMetadata((::ExtensionMetadata*)ret);
}

void Flood::Extension::OnInit()
{
    NativePtr->onInit();
}

void Flood::Extension::OnCleanup()
{
    NativePtr->onCleanup();
}

