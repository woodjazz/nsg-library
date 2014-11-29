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
#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>
#include "GLES2Includes.h"
#include "SharedPointers.h"
#include "Mesh.h"
#include "FontAtlasTexture.h"
#include "Types.h"

namespace NSG
{
    class TextMesh : public Mesh
    {
    public:
		TextMesh(const std::string& textureFilename);
        ~TextMesh();
        bool Has(const std::string& textureFilename) const;
        bool SetText(const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign);
        GLfloat GetWidth() const { return screenWidth_; }
        GLfloat GetHeight() const { return screenHeight_; }
        GLfloat GetWidthForCharacterPosition(unsigned int charPos) const;
        unsigned int GetCharacterPositionForWidth(float width) const;
        PTexture GetTexture() const { return pAtlas_->GetTexture(); }
        PProgram GetProgram() const { return pProgram_; }
        GLenum GetWireFrameDrawMode() const override;
        GLenum GetSolidDrawMode() const override;
        virtual size_t GetNumberOfTriangles() const override;
        HorizontalAlignment GetTextHorizontalAlignment() const { return hAlignment_; }
        VerticalAlignment GetTextVerticalAlignment() const { return vAlignment_; }
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        void UpdateBuffers();
        void Move(VertexsData& obj, float offsetX, float offsetY);
        PFontAtlasTexture pAtlas_;
        PProgram pProgram_;

        std::string text_;
        GLfloat screenWidth_;
        GLfloat screenHeight_;
        std::string textureFilename_;
        HorizontalAlignment hAlignment_;
        VerticalAlignment vAlignment_;
        float alignmentOffsetX_;
        float alignmentOffsetY_;
        size_t maxLength_;
		SignalViewChanged::PSlot slotViewChanged_;
    };
}
