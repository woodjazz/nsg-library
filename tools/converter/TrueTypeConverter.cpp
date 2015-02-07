/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

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
#include "TrueTypeConverter.h"
#include "ResourceFile.h"
#include "App.h"
#include "ResourceMemory.h"
#include "Check.h"
#include "pugixml.hpp"
#include "stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stb_image_write.h"

namespace NSG
{
    TrueTypeConverter::TrueTypeConverter(const Path& path, int sChar, int eChar, int fontPixelsHeight, int bitmapWidth, int bitmapHeight)
        : path_(path),
          sChar_(sChar),
          eChar_(eChar),
          fontPixelsHeight_(fontPixelsHeight),
          bitmapWidth_(bitmapWidth),
          bitmapHeight_(bitmapHeight)
    {
    }

    bool TrueTypeConverter::Load()
    {
        auto resource = App::this_->GetOrCreateResourceFile(path_.GetFilePath());
        CHECK_CONDITION(resource->IsReady(), __FILE__, __LINE__);
        const unsigned char* data = (const unsigned char*)resource->GetData();
        //size_t totalBytes = resource->GetBytes();

        std::string tempBitmap;
        tempBitmap.resize(bitmapWidth_ * bitmapHeight_);
        int numChars = eChar_ - sChar_ + 1; // characters to bake
        cdata_.resize(numChars);
        stbtt_BakeFontBitmap(data, 0, (float)fontPixelsHeight_, (unsigned char*)&tempBitmap[0], bitmapWidth_, bitmapHeight_, sChar_, numChars, &cdata_[0]);
		texture_ = App::this_->GetOrCreateResourceMemory(path_.GetFilePath() + std::to_string(fontPixelsHeight_));
		texture_->SetData(tempBitmap.c_str(), tempBitmap.size());
        CHECK_CONDITION(texture_->IsReady(), __FILE__, __LINE__);
        return true;
    }

    bool TrueTypeConverter::Save(const Path& outputDir) const
    {
        std::string outName = path_.GetName();
        outName += std::to_string(fontPixelsHeight_);
        Path outputFile(outputDir);
        outputFile.SetName(outName);
        outputFile.SetExtension("xml");

        Path texturePath(outputFile);
        texturePath.SetExtension("png");
        {
            const char* p = texture_->GetData();
            const size_t channels = 1;
            size_t n = texture_->GetBytes() * channels;
            std::vector<char> image(n, 0);
            size_t idx = 0;
            while (idx < n)
            {
                image[idx + channels - 1] = *p++;
                idx += channels;
            }
            const unsigned char* img = (const unsigned char*)&image[0];
            stbi_write_png(texturePath.GetFullAbsoluteFilePath().c_str(), bitmapWidth_, bitmapHeight_, channels, img, 0);
        }

        {
            pugi::xml_document doc;
            pugi::xml_node fontNode = doc.append_child("Font");
            fontNode.append_attribute("bitmap").set_value(texturePath.GetFilename().c_str());
            fontNode.append_attribute("height").set_value(fontPixelsHeight_);
            int code = sChar_;
            pugi::char_t ch[2];
            ch[1] = '\0';
            for (auto& obj : cdata_)
            {
                pugi::xml_node charNode = fontNode.append_child("Char");
                charNode.append_attribute("width").set_value((int) (obj.xadvance + 0.5f));
                {
                    std::stringstream ss;
                    ss << obj.xoff << " " << obj.yoff;
                    charNode.append_attribute("offset") = ss.str().c_str();
                }
                {
                    std::stringstream ss;
                    ss << obj.x0 << " " << obj.y0 << " " << obj.x1 - obj.x0 << " " << obj.y1 - obj.y0;
                    charNode.append_attribute("rect") = ss.str().c_str();
                }
                {
                    ch[0] = code++;
                    charNode.append_attribute("code").set_value(ch);
                }
            }

            doc.save_file(outputFile.GetFullAbsoluteFilePath().c_str());
        }

        return true;
    }
}