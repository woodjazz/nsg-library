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
#include "TextMesh.h"
#include "FontAtlasTextureManager.h"
#include "Program.h"
#include "FontAtlasTexture.h"
#include "Graphics.h"
#include "Context.h"
#include "App.h"
#include "ResourceMemory.h"
#include <algorithm>

namespace NSG
{
    TextMesh::TextMesh(const std::string& textureFilename)
        : Mesh(textureFilename, true),
          screenWidth_(0),
          screenHeight_(0),
          textureFilename_(textureFilename),
          hAlignment_(LEFT_ALIGNMENT),
          vAlignment_(BOTTOM_ALIGNMENT),
          alignmentOffsetX_(0),
          alignmentOffsetY_(0),
          maxLength_(0)
    {
        SetSerializable(false);
		if (textureFilename.empty())
			pProgram_ = App::this_->GetOrCreateProgram("NSGInternalTextProgram");
		else
			pProgram_ = App::this_->GetOrCreateProgram(textureFilename);
		pProgram_->SetFlags((int)ProgramFlag::TEXT);
        pAtlas_ = FontAtlasTextureManager::this_->GetAtlas(textureFilename);

		slotViewChanged_ = App::this_->signalViewChanged_->Connect([&](int width, int height)
		{
			Invalidate();
		});
    }

    TextMesh::~TextMesh()
    {
    }

    bool TextMesh::Has(const std::string& textureFilename) const
    {
        return textureFilename_ == textureFilename;
    }

    bool TextMesh::IsValid()
    {
        return pProgram_->IsReady() && pAtlas_->IsReady() && !vertexsData_.empty();
    }

    void TextMesh::UpdateBuffers()
    {
        CHECK_ASSERT(!isStatic_ && "Trying to update an static buffer", __FILE__, __LINE__);

        if (vertexsData_.empty())
            return;

        CHECK_GL_STATUS(__FILE__, __LINE__);

        CHECK_ASSERT(!indexes_.empty(), __FILE__, __LINE__);
        CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);

        VertexsData tmpVertexData(vertexsData_);
        Move(tmpVertexData, alignmentOffsetX_, alignmentOffsetY_);
        pVBuffer_->UpdateData(tmpVertexData);
        pIBuffer_->UpdateData(indexes_);

        //graphics_.RedoVAO(pProgram_.get(), pVBuffer_.get(), pIBuffer_.get());

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }


    void TextMesh::AllocateResources()
    {
        CHECK_ASSERT(!vertexsData_.empty(), __FILE__, __LINE__);
        CHECK_ASSERT(!indexes_.empty(), __FILE__, __LINE__);

        CHECK_GL_STATUS(__FILE__, __LINE__);
        CHECK_ASSERT(GetSolidDrawMode() != GL_TRIANGLES || indexes_.size() % 3 == 0, __FILE__, __LINE__);
        CHECK_ASSERT(!pVBuffer_, __FILE__, __LINE__);
        CHECK_ASSERT(!pIBuffer_, __FILE__, __LINE__);

        VertexsData tmpVertexData(vertexsData_);
        Move(tmpVertexData, alignmentOffsetX_, alignmentOffsetY_);

        const size_t VERTEX_PER_CHAR = 6;
        const GLsizeiptr MAX_BYTES_VERTEX_BUFFER = VERTEX_PER_CHAR * maxLength_ * sizeof(VertexData);
        GLsizeiptr bytesNeeded = sizeof(VertexData) * vertexsData_.size();
        CHECK_ASSERT(bytesNeeded <= MAX_BYTES_VERTEX_BUFFER, __FILE__, __LINE__);
        if (isStatic_)
            pVBuffer_ = PVertexBuffer(new VertexBuffer(MAX_BYTES_VERTEX_BUFFER, MAX_BYTES_VERTEX_BUFFER, tmpVertexData, GL_STATIC_DRAW));
        else
            pVBuffer_ = PVertexBuffer(new VertexBuffer(MAX_BYTES_VERTEX_BUFFER, MAX_BYTES_VERTEX_BUFFER, tmpVertexData, GL_DYNAMIC_DRAW));

        const size_t INDEXES_PER_CHAR = 6;
        const GLsizeiptr MAX_BYTES_INDEX_BUFFER = INDEXES_PER_CHAR * maxLength_ * sizeof(IndexType);
        bytesNeeded = sizeof(IndexType) * indexes_.size();
        CHECK_ASSERT(bytesNeeded <= MAX_BYTES_INDEX_BUFFER, __FILE__, __LINE__);

        if (isStatic_)
            pIBuffer_ = PIndexBuffer(new IndexBuffer(MAX_BYTES_INDEX_BUFFER, MAX_BYTES_INDEX_BUFFER, indexes_, GL_STATIC_DRAW));
        else
            pIBuffer_ = PIndexBuffer(new IndexBuffer(MAX_BYTES_INDEX_BUFFER, MAX_BYTES_INDEX_BUFFER, indexes_, GL_DYNAMIC_DRAW));

        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void TextMesh::ReleaseResources()
    {
        Mesh::ReleaseResources();

        text_.clear(); // Force SetText (when window's resizes)
    }

    GLfloat TextMesh::GetWidthForCharacterPosition(unsigned int charPos) const
    {
        return pAtlas_->GetWidthForCharacterPosition(text_.c_str(), charPos);
    }

    unsigned int TextMesh::GetCharacterPositionForWidth(float width) const
    {
        return pAtlas_->GetCharacterPositionForWidth(text_.c_str(), width);
    }

    void TextMesh::Move(VertexsData& obj, float offsetX, float offsetY)
    {
        auto it = obj.begin();
        while (it != obj.end())
        {
            VertexData& data = *it;
            data.position_.x += offsetX;
            data.position_.y += offsetY;
            ++it;
        }
    }

    bool TextMesh::SetText(const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign)
    {
        if (text.size() > maxLength_)
        {
            maxLength_ = text.size();
            Invalidate();
            return false;;
        }

        bool changed = false;

        if (text_ != text)
        {
            if (pAtlas_->GenerateMesh(text, vertexsData_, indexes_, screenWidth_, screenHeight_))
            {
                text_ = text;

                changed = true;
            }
        }

        if (changed || hAlign != hAlignment_ || vAlign != vAlignment_)
        {
            if (hAlign == CENTER_ALIGNMENT)
                alignmentOffsetX_ = -screenWidth_ / 2;
            else if (hAlign == RIGHT_ALIGNMENT)
                alignmentOffsetX_ = -screenWidth_;
            else
                alignmentOffsetX_ = 0;

            if (vAlign == MIDDLE_ALIGNMENT)
                alignmentOffsetY_ = screenHeight_ / 2;
            else if (vAlign == TOP_ALIGNMENT)
                alignmentOffsetY_ = -screenHeight_;
            else
                alignmentOffsetY_ = screenHeight_;

            hAlignment_ = hAlign;
            vAlignment_ = vAlign;

            changed = true;
        }

        if (changed && IsReady())
        {
            UpdateBuffers();
            return true;
        }

        return false;
    }

    GLenum TextMesh::GetWireFrameDrawMode() const
    {
        return GL_LINE_LOOP;
    }

    GLenum TextMesh::GetSolidDrawMode() const
    {
        return GL_TRIANGLES;
    }

    size_t TextMesh::GetNumberOfTriangles() const
    {
        return vertexsData_.size()/3;
    }

}
