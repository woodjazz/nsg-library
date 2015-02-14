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
#include "ParticleSystem.h"
#include "Particle.h"
#include "Util.h"
#include "App.h"
#include "SphereMesh.h"
#include "Material.h"
#include "RigidBody.h"
#include "AppConfiguration.h"

namespace NSG
{
    ParticleSystem::VelocityParams::VelocityParams()
        : initialSpeed_(0),
          normalSpeed_(1),
          tangetSpeed_(0),
          objectSpeed_(0),
          randomSpeed_(0)
    {
    };

    ParticleSystem::PhysicsParams::PhysicsParams()
        : mass_(1),
          shape_(SH_SPHERE)
    {
    };

    ParticleSystem::ParticleSystem(const std::string& name)
        : SceneNode(name),
          emitFrom_(PS_EF_VERTS),
          fps_((float)AppConfiguration::this_->fps_),
          amount_(50),
          start_(0),
          end_(1),
          lifetime_(3),
          lifetimeRandom_(0.0f),
          currentTime_(0),
          loop_(true),
          randGenerator_(rd_()),
          generated_(0),
          collisionGroup_((int)CollisionMask::PARTICLE),
          collisionMask_((int)CollisionMask::ALL & ~(int)CollisionMask::PARTICLE),
          currentVertex_(0),
          triggerParticles_(0),
          distribution_(ParticleSystemDistribution::RANDOM)
    {
        particleMesh_ = App::this_->CreateSphereMesh(0.5f, 4);
        particleMaterial_ = std::make_shared<Material>(GetUniqueName(name + "Particle"));
        particleMaterial_->SetColor(Color(0, 1, 0, 1));
    }

    ParticleSystem::~ParticleSystem()
    {
        Invalidate();
    }

    float ParticleSystem::GetParticlesPerFrame(float deltaTime)
    {
        auto remainingTime = end_ - start_ - currentTime_;
        auto remainingFrames = remainingTime / deltaTime;
        auto remainingParticles = amount_ - generated_;
        if (remainingFrames <= 0)
            return remainingParticles;
        return remainingParticles / remainingFrames;
    }

    void ParticleSystem::Try2GenerateParticles(float deltaTime)
    {
        if (currentTime_ >= start_ && currentTime_ < end_ && generated_ < amount_)
        {
            triggerParticles_ += GetParticlesPerFrame(deltaTime);
            while (triggerParticles_ >= 1)
            {
                GenerateParticle();
                ++generated_;
                --triggerParticles_;
            }
        }
    }

    PParticle ParticleSystem::GenerateParticle()
    {
        PParticle particle;
        if (!disabled_.empty())
        {
            particle = *disabled_.begin();
            disabled_.erase(particle);
        }
        else
        {
            particle = CreateChild<Particle>(GetUniqueName(name_));
            particle->SetParticleSystem(this);
            particle->SetMaterial(particleMaterial_);
            particle->SetMesh(particleMesh_);
            auto rb = particle->GetOrCreateRigidBody();
            rb->SetCollisionMask(collisionGroup_, collisionMask_);
            rb->SetMass(physicsParams_.mass_);
            rb->SetShape(physicsParams_.shape_);
            particle->SetEnabled(false);
            particles_.push_back(particle);
        }
        Initialize(particle);
        particle->Enable();
        return particle;
    }

    void ParticleSystem::RemoveParticle(PParticle particle)
    {
        particle->Disable();
        disabled_.insert(particle);
    }

    void ParticleSystem::AllocateResources()
    {
        particles_.reserve(amount_);
    }

    void ParticleSystem::ReleaseResources()
    {
        particles_.clear();
        ClearAllChildren();
    }

    float ParticleSystem::GetLifeTime() const
    {
        if (lifetimeRandom_)
        {
            auto min = lifetime_ * (1.f - lifetimeRandom_);
            auto max = lifetime_;
            std::uniform_real_distribution<float> dis(min, max);
            auto random = dis(randGenerator_);
            return random;
        }
        else
            return lifetime_;
    }

    void ParticleSystem::ReStartLoop()
    {
        currentTime_ = 0;
        generated_ = 0;
    }

    void ParticleSystem::Update(float deltaTime)
    {
        if (IsReady())
        {
            Try2GenerateParticles(deltaTime);
            for (auto& particle : particles_)
            {
                if (particle->IsEnabled())
                {
                    auto lifeTime = GetLifeTime();
                    auto age = particle->GetAge();
                    if (age < lifeTime)
                        particle->Update(deltaTime);
                    else
                        RemoveParticle(particle);
                }
            }
        }

        currentTime_ += deltaTime;

        if (loop_ && currentTime_ > end_)
            ReStartLoop();
    }

    void ParticleSystem::Initialize(PParticle particle)
    {
        if (emitFrom_ == PS_EF_VERTS)
        {
            auto mesh = GetMesh();
            auto& vertexes = mesh->GetConstVertexsData();
            if (distribution_ == ParticleSystemDistribution::RANDOM)
            {
                std::uniform_real_distribution<float> dis(0, vertexes.size());
                currentVertex_ = (size_t)dis(randGenerator_);
            }
            else
            {
                ++currentVertex_;
                if (currentVertex_ >= vertexes.size())
                    currentVertex_ = 0;
            }

            auto& vertex = vertexes[currentVertex_];
            auto& position = vertex.position_;
            particle->SetPosition(position);
        }

        SetInitialVelocity(particle);
    }

    void ParticleSystem::SetInitialVelocity(PParticle particle)
    {
        Vector3 velocity(velocityParams_.initialSpeed_);
        if (velocityParams_.objectSpeed_)
        {
            auto rb = GetRigidBody();
            if (rb)
                velocity += velocityParams_.objectSpeed_ * rb->GetLinearVelocity();
        }

        if (velocityParams_.randomSpeed_)
        {
            std::uniform_real_distribution<float> dis(0.0f, velocityParams_.randomSpeed_);
            auto random = dis(randGenerator_);
            velocity *= random;
        }

        if (emitFrom_ == PS_EF_VERTS)
        {
            auto mesh = GetMesh();
            auto& vertexes = mesh->GetConstVertexsData();
            auto& vertex = vertexes[currentVertex_];

            if (velocityParams_.normalSpeed_)
            {
                auto& normal = vertex.normal_;
                velocity += normal * velocityParams_.normalSpeed_;
            }

            if (velocityParams_.tangetSpeed_)
            {
                auto& tangent = vertex.tangent_;
                velocity += tangent * velocityParams_.tangetSpeed_;
            }
        }

        particle->SetVelocity(velocity);
    }

}