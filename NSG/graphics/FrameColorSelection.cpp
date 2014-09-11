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
#include "FrameColorSelection.h"
#include "FrameBuffer.h"
#include "Log.h"
#include "Check.h"
#include "SceneNode.h"
#include "Camera.h"
#include "App.h"
#include "Graphics.h"
#include "Pass.h"
#include "Context.h"
#include "Material.h"
#include "Technique.h"
#include "Mesh.h"
#include "Program.h"

namespace NSG
{
    FrameColorSelection::FrameColorSelection(UseBuffer buffer)
        : buffer_(buffer),
          material_(new Material("FrameColorSelection")),
          windowWidth_(0),
          windowHeight_(0),
          pixelX_(0),
          pixelY_(0)
    {

        std::pair<int32_t, int32_t> windowSize = App::this_->GetViewSize();
        windowWidth_ = windowSize.first;
        windowHeight_ = windowSize.second;

        unsigned int frameBufferFlags = FrameBuffer::COLOR;
        if (buffer_ == UseBuffer::DEPTH)
            frameBufferFlags |= FrameBuffer::DEPTH;
        else if (buffer_ == UseBuffer::DEPTH_STENCIL)
            frameBufferFlags |= FrameBuffer::STENCIL;

        frameBuffer_ = PFrameBuffer(new FrameBuffer(windowWidth_, windowHeight_, frameBufferFlags));

        PPass pass(new Pass);
		Program* program = new Program;
        pass->SetProgram(PProgram(program));

        PTechnique technique(new Technique);
        technique->Add(pass);
        material_->SetTechnique(technique);
        pass->SetBlendMode(BLEND_NONE);
    }

    FrameColorSelection::~FrameColorSelection()
    {
        Context::RemoveObject(this);
    }

    bool FrameColorSelection::IsValid()
    {
        return material_->IsReady() && frameBuffer_->IsReady();
    }

    void FrameColorSelection::Begin(float screenX, float screenY)
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        pixelX_ = (GLint)((1 + screenX) / 2.0f * windowWidth_);
        pixelY_ = (GLint)((1 + screenY) / 2.0f * windowHeight_);

        Graphics::this_->SetFrameBuffer(frameBuffer_->GetId());

        if (buffer_ == UseBuffer::DEPTH_STENCIL)
            Graphics::this_->ClearAllBuffers();
		else if (buffer_ == UseBuffer::DEPTH)
            Graphics::this_->ClearBuffers(true, true, false);
        else
            Graphics::this_->ClearBuffers(true, false, false);

#ifndef ANDROID
        glEnable(GL_SCISSOR_TEST);
        glScissor(pixelX_, pixelY_, 1, 1);
#endif
        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    void FrameColorSelection::End()
    {
        CHECK_GL_STATUS(__FILE__, __LINE__);

        glReadPixels(pixelX_, pixelY_, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &selected_);
        Graphics::this_->SetFrameBuffer(0);

#ifndef ANDROID
        glDisable(GL_SCISSOR_TEST);
#endif
        CHECK_GL_STATUS(__FILE__, __LINE__);
    }

    GLushort FrameColorSelection::GetSelected() const
    {
        GLushort b3 = (GLushort)selected_[3] / 0x10;
        GLushort b2 = (GLushort)selected_[2] / 0x10;
        GLushort b1 = (GLushort)selected_[1] / 0x10;
        GLushort b0 = (GLushort)selected_[0] / 0x10;
        GLushort index = b3 << 12 | b2 << 8 | b1 << 4 | b0;
        return index;
    }

    Color FrameColorSelection::TransformSelectedId2Color(GLushort id)
    {
        Color color;
        color[0] = (id & 0x000F) / 15.0f;
        color[1] = ((id & 0x00F0) >> 4) / 15.0f;
        color[2] = ((id & 0x0F00) >> 8) / 15.0f;
        color[3] = ((id & 0xF000) >> 12) / 15.0f;
        return color;
    }

    bool FrameColorSelection::Render(GLushort id, float screenX, float screenY, const std::vector<SceneNode*>& nodes)
    {
        if (IsReady())
        {
            Begin(screenX, screenY);
            {
                material_->SetColor(TransformSelectedId2Color(id));

                Graphics::this_->Set(material_.get());

                for (auto& obj : nodes)
                {
					Graphics::this_->SetNode(obj);
                    Graphics::this_->Set(obj->GetMesh().get());
                    material_->GetTechnique()->Render();
                }
            }
            End();
            return true;
        }
        return false;
    }

    bool FrameColorSelection::Hit(GLushort id, float screenX, float screenY, const std::vector<SceneNode*>& nodes)
    {
        if (Render(id, screenX, screenY, nodes))
            return id == GetSelected();

        return false;
    }
}
