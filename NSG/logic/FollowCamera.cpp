/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "FollowCamera.h"
#include "Camera.h"
#include "Engine.h"
#include "Util.h"

namespace NSG
{
    FollowCamera::FollowCamera(PCamera camera)
        : camera_(camera),
        engine_(nullptr)
    {
        SetEngine(Engine::GetPtr().get());

        slotEngineCreated_ = Engine::SigReady()->Connect([this](Engine * engine)
        {
            if (!engine_)
                SetEngine(engine);
        });
    }

    FollowCamera::~FollowCamera()
    {

    }

    void FollowCamera::SetEngine(Engine* engine)
    {
        if (engine_ != engine)
        {
            engine_ = engine;

            if (engine)
            {
                slotUpdate_ = engine->SigUpdate()->Connect([&](float deltaTime)
                {
                    OnUpdate(deltaTime);
                });
            }
        }
        else
        {
            slotUpdate_ = nullptr;
        }
    }

    void FollowCamera::OnUpdate(float deltaTime)
    {
        if(!trackNode_)
            return;
        
        camera_->SetGlobalLookAt(trackNode_->GetGlobalPosition());
        auto pos = camera_->GetGlobalPosition();
        auto targetPos = trackNode_->GetGlobalPosition() + offset_;
        camera_->SetGlobalPosition(Lerp(pos, targetPos, deltaTime));
    }

    void FollowCamera::Track(PNode node)
    {
    	trackNode_ = node;
    }

    void FollowCamera::SetOffset(const Vector3& offset)
   	{
   		offset_ = offset;
   	}
}