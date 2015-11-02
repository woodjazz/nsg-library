#include "FontAtlas.h"
#include "Check.h"
#include "Texture2D.h"
#include "TextMesh.h"
#include "ResourceFile.h"
#include "Mesh.h"
#include "Graphics.h"
#include "Window.h"
#include "UTF8String.h"
#include "pugixml.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <cmath>

namespace NSG
{
    template<> std::map<std::string, PWeakTextMesh> WeakFactory<std::string, TextMesh>::objsMap_;

    FontAtlas::FontAtlas(const std::string& name)
        : Object(name),
          viewWidth_(0),
          viewHeight_(0),
          height_(0)
    {
        auto graphics = Graphics::GetPtr();
        if(graphics)
        {
            auto window = graphics->GetWindow();
            if (window)
                SetWindow(window);
        }
    }

    FontAtlas::~FontAtlas()
    {
    }

    void FontAtlas::Set(PResourceFile xmlResource)
    {
        if (xmlResource_ != xmlResource)
        {
            xmlResource_ = xmlResource;
            Invalidate();
        }
    }

    void FontAtlas::SetTexture(PTexture texture)
    {
        if (texture_ != texture)
        {
            texture_ = texture;
            Invalidate();
        }
    }

    void FontAtlas::SetWindow(Window* window)
    {
        if (window)
        {
            SetViewSize(window->GetWidth(), window->GetHeight());

            slotViewChanged_ = window->SigSizeChanged()->Connect([&](int width, int height)
            {
                SetViewSize(width, height);
            });
        }
        else
        {
            slotViewChanged_ = nullptr;
            SetViewSize(0, 0);
        }
    }

    PTextMesh FontAtlas::GetOrCreateMesh(const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign)
    {
        auto mesh = FontAtlas::GetOrCreateClass<TextMesh>(text);
        mesh->SetAtlas(shared_from_this());
        mesh->SetText(text, hAlign, vAlign);
        return mesh;
    }

    bool FontAtlas::IsValid()
    {
        return viewWidth_ > 0 && viewHeight_ > 0 && xmlResource_->IsReady() &&
               texture_ && texture_->IsReady();
    }

    void FontAtlas::AllocateResources()
    {
        ParseXML();
    }

    void FontAtlas::ReleaseResources()
    {
        charsMap_.clear();
    }

    void FontAtlas::SetViewSize(int width, int height)
    {
        if (viewWidth_ != width || viewHeight_ != height)
        {
            viewWidth_ = width;
            viewHeight_ = height;
            auto objs = FontAtlas::GetObjs();
            for (auto& obj : objs)
                obj->Invalidate();
        }
    }

    void FontAtlas::ParseXML()
    {
        LOGI("FontAtlas::Parsing: %s", xmlResource_->GetName().c_str());
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer((void*)xmlResource_->GetData(), xmlResource_->GetBytes());
        if (!result)
        {
            LOGE("XML parsed with errors, attr value: [%s]", doc.child("node").attribute("attr").value());
            LOGE("Error description: %s", result.description());
            LOGE("Error offset: %td", result.offset);
        }
        else
        {
            pugi::xml_node node = doc.child("Font");
            height_ = node.attribute("height").as_int();
            node = node.child("Char");
            while (node)
            {
                CharInfo charInfo;
                charInfo.width = node.attribute("width").as_int();

                {
                    std::string s = node.attribute("offset").value();
                    std::sscanf(s.c_str(), "%f %f", &charInfo.offset.x, &charInfo.offset.y);
                }

                {
                    std::string s = node.attribute("rect").value();
                    std::sscanf(s.c_str(), "%f %f %f %f", &charInfo.rect.x, &charInfo.rect.y, &charInfo.rect.z, &charInfo.rect.w);
                }

                const char* code = node.attribute("code").value();
                UTF8String utf8(code);
                unsigned unicode = utf8.AtUTF8(0);
                charsMap_[unicode] = charInfo;
                node = node.next_sibling("Char");
            }

            LOGI("FontAtlas::Parsing done.");
        }
    }

    void FontAtlas::GenerateMeshData(const std::string& text, VertexsData& vertexsData, Indexes& indexes, GLfloat& screenWidth, GLfloat& screenHeight)
    {
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

        while (*p)
        {
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

            for (int i = 0; i < 4; i++)
            {
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