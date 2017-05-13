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
#include "FontXMLAtlas.h"
#include "Check.h"
#include "ResourceFile.h"
#include "Texture.h"
#include "UTF8String.h"

namespace NSG {
FontXMLAtlas::FontXMLAtlas(const std::string& name) : FontAtlas(name) {}

FontXMLAtlas::~FontXMLAtlas() {}

void FontXMLAtlas::SetXML(PResourceFile xmlResource) {
    if (xmlResource_ != xmlResource) {
        xmlResource_ = xmlResource;
        Invalidate();
    }
}

void FontXMLAtlas::SetTexture(PTexture texture) {
    if (texture_ != texture) {
        texture_ = texture;
        Invalidate();
    }
}

bool FontXMLAtlas::IsValid() {
    return FontAtlas::IsValid() && xmlResource_ && xmlResource_->IsReady() &&
           texture_ && texture_->IsReady();
}

void FontXMLAtlas::AllocateResources() { ParseXML(); }

void FontXMLAtlas::ParseXML() {
    LOGI("FontXMLAtlas::Parsing: %s", xmlResource_->GetName().c_str());
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(
        (void*)xmlResource_->GetData(), xmlResource_->GetBytes());
    if (!result) {
        LOGE("XML parsed with errors, attr value: [%s]",
             doc.child("node").attribute("attr").value());
        LOGE("Error description: %s", result.description());
    } else {
        pugi::xml_node node = doc.child("Font");
        height_ = node.attribute("height").as_int();
        node = node.child("Char");
        while (node) {
            CharInfo charInfo;
            charInfo.width = node.attribute("width").as_int();

            {
                std::string s = node.attribute("offset").value();
                std::sscanf(s.c_str(), "%f %f", &charInfo.offset.x,
                            &charInfo.offset.y);
            }

            {
                std::string s = node.attribute("rect").value();
                std::sscanf(s.c_str(), "%f %f %f %f", &charInfo.rect.x,
                            &charInfo.rect.y, &charInfo.rect.z,
                            &charInfo.rect.w);
            }

            const char* code = node.attribute("code").value();
            UTF8String utf8(code);
            unsigned unicode = utf8.AtUTF8(0);
            charsMap_[unicode] = charInfo;
            node = node.next_sibling("Char");
        }

        LOGI("FontXMLAtlas::Parsing done.");
    }
}
}
