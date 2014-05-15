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
#include "GLES2Program.h"
#include "GLES2Mesh.h"
#include "Node.h"
#include "Types.h"
#include "GLES2GPUObject.h"

namespace NSG 
{
    class SceneNode;
    class GLES2FrameColorSelection : public GLES2GPUObject
    {
    public:
        GLES2FrameColorSelection(bool createDepthBuffer);
        ~GLES2FrameColorSelection();
        void ViewChanged(int32_t windowWidth, int32_t windowHeight);
        void Begin(float screenX, float screenY);
        void End();
        GLushort GetSelected() const;
        void Render(SceneNode* pSceneNode);
        void Render(GLushort id, GLES2Mesh* pMesh, Node* pNode);
        GLuint GetPositionLoc() const { return position_loc_; }
        GLuint GetMVPLoc() const { return mvp_loc_; }
        virtual bool IsValid();
        virtual void AllocateResources();
        virtual void ReleaseResources();
    private:
        Color TransformSelectedId2Color(GLushort id);
        PGLES2Program pProgram_;
        GLuint position_loc_;
        GLuint color_loc_;
        GLuint mvp_loc_;
        int32_t windowWidth_;
        int32_t windowHeight_;
        GLuint framebuffer_;
        GLuint colorRenderbuffer_;
        GLuint depthRenderBuffer_;
        GLubyte selected_[4];
        double screenX_;
        double screenY_;
        GLint pixelX_;
        GLint pixelY_;
        bool enabled_;
        bool createDepthBuffer_;
        GLfloat clear_color_[4];
        GLfloat clear_depth_;
        GLboolean blend_enable_;
        GLboolean depth_enable_;
    };

    typedef std::shared_ptr<GLES2FrameColorSelection> PGLES2FrameColorSelection;
}

