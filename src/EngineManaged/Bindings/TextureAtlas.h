/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Texture/TextureAtlas.h>
#include "Rect.h"
#include "ResourceHandle.h"
#include "Image.h"

namespace Flood
{
    ref class Handle;
    ref class Image;
    ref class MaxRectsBinPack;
    ref class TextureAtlas;
    value struct Rect;
    value struct SubTexture;

    public value struct SubTexture
    {
    public:
        SubTexture(::SubTexture* native);
        SubTexture(System::IntPtr native);
        Flood::Rect Rect;
        bool IsRotated;
    };

    public ref class TextureAtlas
    {
    public:
        property ::TextureAtlas* NativePtr;

        TextureAtlas(::TextureAtlas* native);
        TextureAtlas(System::IntPtr native);
        TextureAtlas(unsigned int maxSize);
        bool AddImage(Flood::ResourceHandle<Flood::Image^> imageHandle);
        bool GetImageSubTexture(Flood::ResourceHandle<Flood::Image^> imageHandle, [System::Runtime::InteropServices::Out] Flood::SubTexture% subTexture);
        Flood::ResourceHandle<Flood::Image^> GetAtlasImageHandle();
    };
}
