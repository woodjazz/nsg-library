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
#include "SceneNode.h"
#include <vector>
#include <set>
#include <random>

namespace NSG
{
    class ParticleSystem : public SceneNode
    {
    public:
        ParticleSystem(const std::string& name);
        ~ParticleSystem();
        void Update(float deltaTime);
        void AllocateResources() override;
        void ReleaseResources() override;
        float GetLifeTime() const;
    private:
        void Initialize(PParticle particle);
        float GetParticlesPerFrame(float deltaTime);
        PParticle GenerateParticle();
        void Try2GenerateParticles(float deltaTime);
        void RemoveParticle(PParticle particle);
        void SetInitialVelocity(PParticle particle);
        void ReStartLoop();
        PMesh particleMesh_;
        PMaterial particleMaterial_;
        ParticleSystemEmitFrom emitFrom_;
        float fps_;
        size_t amount_;
        float start_; //secs
        float end_; //secs
        float lifetime_; //secs
        float lifetimeRandom_;
        float currentTime_;
        bool loop_;
        std::vector<PParticle> particles_;
        std::random_device rd_;
        mutable std::mt19937 randGenerator_;
        size_t generated_; // particles generated after one update
        int collisionGroup_;
        int collisionMask_;

        struct VelocityParams
        {
            Vector3 initialSpeed_;
            float normalSpeed_;
            float tangetSpeed_;
            float objectSpeed_;
            float randomSpeed_;
            VelocityParams();
        };
        VelocityParams velocityParams_;

        struct PhysicsParams
        {
            float mass_;
            PhysicsShape shape_;
            PhysicsParams();
        };
        PhysicsParams physicsParams_;

        size_t currentVertex_;
        float triggerParticles_;
        std::set<PParticle> disabled_;
        ParticleSystemDistribution distribution_;
    };
}