/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "FontTTFAtlas.h"
#include "Check.h"
#include "Resource.h"
#include "ResourceFile.h"
#include "Texture.h"
#include "Texture2D.h"
#include "UTF8String.h"
#include "Util.h"
#include "stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_truetype.h"
extern unsigned char* stbi_write_png_to_mem(unsigned char* pixels,
                                            int stride_bytes, int x, int y,
                                            int n, int* out_len);
namespace NSG {
FontTTFAtlas::FontTTFAtlas(const std::string& name)
    : FontAtlas(name), sChar_(32), eChar_(126), bitmapWidth_(512),
      bitmapHeight_(512) {
    height_ = 12;
}

FontTTFAtlas::~FontTTFAtlas() {}

void FontTTFAtlas::SetTTF(PResourceFile ttfResource) {
    if (ttfResource_ != ttfResource) {
        ttfResource_ = ttfResource;
        Invalidate();
    }
}

bool FontTTFAtlas::IsValid() {
    return FontAtlas::IsValid() && ttfResource_ && ttfResource_->IsReady();
}

void FontTTFAtlas::AllocateResources() { ParseTTF(); }

void FontTTFAtlas::ParseTTF() {
    CHECK_CONDITION(ttfResource_->IsReady());
    const unsigned char* data = (const unsigned char*)ttfResource_->GetData();
    std::string tempBitmap;
    tempBitmap.resize(bitmapWidth_ * bitmapHeight_);
    const int numChars = eChar_ - sChar_ + 1; // characters to bake
    std::vector<stbtt_bakedchar> cdata;
    cdata.resize(numChars);
    stbtt_BakeFontBitmap(data, 0, (float)height_,
                         (unsigned char*)&tempBitmap[0], bitmapWidth_,
                         bitmapHeight_, sChar_, numChars, &cdata[0]);

    int len;
    unsigned char* png =
        stbi_write_png_to_mem((unsigned char*)&tempBitmap[0], 0, bitmapWidth_,
                              bitmapHeight_, 1, &len);
    auto memResource =
        Resource::GetOrCreateClass<Resource>(GetUniqueName("FontTTFAtlas"));
    memResource->SetBuffer(std::string((const char*)png, len));
    free(png);
    CHECK_CONDITION(memResource->IsReady());
    texture_ = std::make_shared<Texture2D>(memResource);
    CHECK_CONDITION(texture_->IsReady());
    int code = sChar_;
    for (auto& obj : cdata) {
        CharInfo charInfo{
            (int)(obj.xadvance + 0.5f), Vertex2{obj.xoff, obj.yoff},
            Rect{(float)obj.x0, (float)obj.y0, (float)(obj.x1 - obj.x0),
                 (float)(obj.y1 - obj.y0)}};
        charsMap_.insert(CharsMap::value_type(code++, charInfo));
    }
}
}
