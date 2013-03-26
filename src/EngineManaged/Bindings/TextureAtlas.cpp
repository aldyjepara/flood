/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "TextureAtlas.h"
#include "Image.h"
#include "Rect.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::SubTexture::SubTexture(::SubTexture* native)
{
    Rect = Flood::Rect((::Rect*)&native->rect);
    IsRotated = native->isRotated;
}

Flood::SubTexture::SubTexture(System::IntPtr native)
{
    auto __native = (::SubTexture*)native.ToPointer();
    Rect = Flood::Rect((::Rect*)&__native->rect);
    IsRotated = __native->isRotated;
}

Flood::TextureAtlas::TextureAtlas(::TextureAtlas* native)
{
    NativePtr = native;
}

Flood::TextureAtlas::TextureAtlas(System::IntPtr native)
{
    auto __native = (::TextureAtlas*)native.ToPointer();
    NativePtr = __native;
}

Flood::TextureAtlas::TextureAtlas(unsigned int maxSize)
{
    auto arg0 = (uint)(uint32)maxSize;
    NativePtr = new ::TextureAtlas(arg0);
}

bool Flood::TextureAtlas::AddImage(Flood::ResourceHandle<Flood::Image^> imageHandle)
{
    auto arg0 = (HandleId)imageHandle.Id;
    auto ret = ((::TextureAtlas*)NativePtr)->addImage(arg0);
    return ret;
}

bool Flood::TextureAtlas::GetImageSubTexture(Flood::ResourceHandle<Flood::Image^> imageHandle, [System::Runtime::InteropServices::Out] Flood::SubTexture% subTexture)
{
    auto arg0 = (HandleId)imageHandle.Id;
    ::SubTexture arg1;
    auto ret = ((::TextureAtlas*)NativePtr)->getImageSubTexture(arg0, arg1);
    subTexture = Flood::SubTexture((::SubTexture*)&arg1);
    return ret;
}

Flood::ResourceHandle<Flood::Image^> Flood::TextureAtlas::GetAtlasImageHandle()
{
    auto ret = ((::TextureAtlas*)NativePtr)->getAtlasImageHandle();
    return Flood::ResourceHandle<Flood::Image^>(ret.id);
}
