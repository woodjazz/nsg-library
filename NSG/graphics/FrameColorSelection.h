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
#include "Types.h"
#include "Constants.h"
#include "Object.h"

namespace NSG
{
    class SceneNode;
    class FrameColorSelection : public Object
    {
    public:
		FrameColorSelection(const std::string& name, UseBuffer buffer = UseBuffer::DEPTH);
        ~FrameColorSelection();
        GLushort GetSelected() const;
        void SetLocations();
        void AssignValues();
        bool Render(GLushort id, float screenX, float screenY, const std::vector<SceneNode*>& nodes);
        bool Hit(GLushort id, float screenX, float screenY, const std::vector<SceneNode*>& nodes);
    private:
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        Color TransformSelectedId2Color(GLushort id);
        void Begin(float screenX, float screenY);
        void End();
		std::string name_;
		App& app_;
        UseBuffer buffer_;
        PFrameBuffer frameBuffer_;
        PMaterial material_;
        int windowWidth_;
        int windowHeight_;
        GLubyte selected_[4];
        GLint pixelX_;
        GLint pixelY_;
		unsigned int frameBufferFlags_;
    };
}

