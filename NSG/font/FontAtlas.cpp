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
#include "FontAtlas.h"
#include "Check.h"
#include "Mesh.h"
#include "RenderingContext.h"
#include "ResourceFile.h"
#include "SharedFromPointer.h"
#include "TextMesh.h"
#include "Texture2D.h"
#include "UTF8String.h"
#include "Window.h"
#include "pugixml.hpp"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <sstream>

namespace NSG {
template <>
std::map<std::string, PWeakTextMesh>
    WeakFactory<std::string, TextMesh>::objsMap_;

FontAtlas::FontAtlas(const std::string& name)
    : Object(name), viewWidth_(0), viewHeight_(0), height_(0) {}

FontAtlas::~FontAtlas() {}

void FontAtlas::SetWindow(PWindow window) {
    if (window) {
        SetViewSize(window->GetWidth(), window->GetHeight());

        slotViewChanged_ = window->SigSizeChanged()->Connect(
            [&](int width, int height) { SetViewSize(width, height); });
    } else {
        slotViewChanged_ = nullptr;
        SetViewSize(0, 0);
    }
}

PTextMesh FontAtlas::GetOrCreateMesh(const std::string& text,
                                     HorizontalAlignment hAlign,
                                     VerticalAlignment vAlign) {
    auto mesh = FontAtlas::GetOrCreateClass<TextMesh>(text);
    mesh->SetAtlas(SharedFromPointer(this));
    mesh->SetText(text, hAlign, vAlign);
    return mesh;
}

bool FontAtlas::IsValid() {
    auto mainWindow = Window::GetMainWindow();
    if (mainWindow && mainWindow->IsReady()) {
        SetWindow(SharedFromPointer(mainWindow));
        return viewWidth_ > 0 && viewHeight_ > 0;
    }
    return false;
}

void FontAtlas::ReleaseResources() { charsMap_.clear(); }

void FontAtlas::SetViewSize(int width, int height) {
    if (viewWidth_ != width || viewHeight_ != height) {
        viewWidth_ = width;
        viewHeight_ = height;
        auto objs = FontAtlas::GetObjs();
        for (auto& obj : objs)
            obj->Invalidate();
    }
}

void FontAtlas::GenerateMeshData(const std::string& text,
                                 VertexsData& vertexsData, Indexes& indexes,
                                 GLfloat& screenWidth, GLfloat& screenHeight) {
    vertexsData.clear();
    indexes.clear();
    screenWidth = screenHeight = 0;

    CHECK_ASSERT(viewWidth_ > 0 && viewHeight_ > 0);

    float sx = 2.0f / viewWidth_;
    float sy = 2.0f / viewHeight_;

    GLsizei textureWidth = texture_->GetWidth();
    GLsizei textureHeight = texture_->GetHeight();

    float x = 0;

    int index = 0;

    const char* p = text.c_str();

    screenHeight = height_ * sy;

    while (*p) {
        int idx = (unsigned char)(*p++);

        const CharInfo& charInfo = charsMap_[idx];

        // Front Face CCW
        VertexData vertex[4];
        {
            float w = (float)charInfo.rect.z * sx;
            float h = -(float)charInfo.rect.w * sy;

            float offsetX = (float)charInfo.offset.x * sx;
            float offsetY = -(float)charInfo.offset.y * sy;

            float ux = (float)charInfo.rect.x / textureWidth;
            float uy = (float)charInfo.rect.y / textureHeight;
            float uw = (float)charInfo.rect.z / textureWidth;
            float uh = (float)charInfo.rect.w / textureHeight;

            vertex[0].position_ = Vertex3(offsetX, offsetY, 0);
            vertex[0].uv_[0] = Vertex2(ux, uy);

            vertex[1].position_ = Vertex3(offsetX + w, offsetY, 0);
            vertex[1].uv_[0] = Vertex2(ux + uw, uy);

            vertex[2].position_ = Vertex3(offsetX, offsetY + h, 0);
            vertex[2].uv_[0] = Vertex2(ux, uy + uh);

            vertex[3].position_ = Vertex3(offsetX + w, offsetY + h, 0);
            vertex[3].uv_[0] = Vertex2(ux + uw, uy + uh);
        }

        for (int i = 0; i < 4; i++) {
            vertex[i].position_.x += x;
            vertexsData.push_back(vertex[i]);
        }

        indexes.push_back(index + 0);
        indexes.push_back(index + 2);
        indexes.push_back(index + 1);

        indexes.push_back(index + 1);
        indexes.push_back(index + 2);
        indexes.push_back(index + 3);

        index += 4;

        x += charInfo.width * sx;
    }

    screenWidth = x;
}
}
