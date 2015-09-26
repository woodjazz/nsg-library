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
#include "Particle.h"
#include "RigidBody.h"

namespace NSG
{
    Particle::Particle(const std::string& name)
        : SceneNode(name),
          age_(0)
    {
        DisableFlags((int)SceneNodeFlag::ALLOW_RAY_QUERY);
    }

    Particle::~Particle()
    {
    }

    void Particle::Update(float deltaTime)
    {
        age_ += deltaTime;

        //if (IsReady())
        {

        }
    }

    void Particle::Reset()
    {
        age_ = 0;
        SetPosition(VECTOR3_ZERO);
        SetOrientation(QUATERNION_IDENTITY);
        auto rb = GetRigidBody();
        rb->Reset();
        rb->SyncWithNode();
    }

    void Particle::Enable()
    {
        Hide(false, false);
        auto rb = GetRigidBody();
        rb->SyncWithNode();
        rb->AddToWorld();
    }

    void Particle::Disable()
    {
        Reset();
        Hide(true, false);
        auto rb = GetRigidBody();
        rb->RemoveFromWorld();
    }

    void Particle::SetVelocity(const Vector3& v)
    {
        auto rb = GetRigidBody();
        rb->SetLinearVelocity(v);
    }
}