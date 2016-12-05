/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Scene.h"
#include "Util.h"
#include "SphereMesh.h"
#include "QuadMesh.h"
#include "Material.h"
#include "Pass.h"
#include "Program.h"
#include "RigidBody.h"
#include "Shape.h"
#include "PhysicsWorld.h"
#include "AppConfiguration.h"
#include "Check.h"

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
      shape_(SH_EMPTY)
{
};

ParticleSystem::ParticleSystem(const std::string& name)
    : SceneNode(name),
      emitFrom_(PS_EF_VERTS),
      amount_(1500),
      start_(0),
      end_(2),
      animationEndTime_(5),
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
      distribution_(ParticleSystemDistribution::RANDOM),
      gravity_(0, -9.81f, 0)
{
    particleMesh_ = Mesh::CreateClass<QuadMesh>("NSGParticleMesh");
    particleMesh_->Set(1.f);
    particleMaterial_ = std::make_shared<Material>(GetUniqueName(name + "Particle"));
    particleMaterial_->SetRenderPass(RenderPass::UNLIT);
    particleMaterial_->SetBillboardType(BillboardType::SPHERICAL);
    DisableFlags((int)SceneNodeFlag::ALLOW_RAY_QUERY);
    particles_.reserve(amount_);
}

ParticleSystem::~ParticleSystem()
{
    SignalBeingDestroy()->Run(this);
}

void ParticleSystem::SetParticleMaterial(PMaterial material)
{
    if (particleMaterial_ != material)
    {
        particleMaterial_ = material;
        Invalidate();
    }
}

float ParticleSystem::GetParticlesPerFrame(float deltaTime)
{
    auto remainingTime = end_ - start_ - currentTime_;
    auto remainingFrames = remainingTime / deltaTime;
    auto remainingParticles = amount_ - generated_;
    CHECK_ASSERT(remainingFrames < std::numeric_limits<float>::max());
    if (remainingFrames <= 0)
        return (float)remainingParticles;
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
        particle->SetMaterial(particleMaterial_);
        particle->SetMesh(particleMesh_);
        auto rb = particle->GetOrCreateRigidBody();
        rb->SetGravity(gravity_);
        rb->SetCollisionMask(collisionGroup_, collisionMask_);
        rb->SetMass(physicsParams_.mass_);
        auto scale = particle->GetGlobalScale();
        auto shape = Shape::GetOrCreate(ShapeKey(physicsParams_.shape_, scale));
        rb->AddShape(shape);
        particle->Hide(false);
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

void ParticleSystem::Invalidate()
{
    particles_.clear();
    ClearAllChildren();
    ReStartLoop();
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
    if (mesh_ && mesh_->IsReady())
    {
        Try2GenerateParticles(deltaTime);
        for (auto& particle : particles_)
        {
            if (!particle->IsHidden())
            {
                auto lifeTime = GetLifeTime();
                auto age = particle->GetAge();
                if (age < lifeTime)
                    particle->Update(deltaTime);
                else
                    RemoveParticle(particle);
            }
        }

        currentTime_ += deltaTime;

        if (loop_ && currentTime_ > animationEndTime_)
            ReStartLoop();
    }
}

void ParticleSystem::Initialize(PParticle particle)
{
    if (emitFrom_ == PS_EF_VERTS)
    {
        auto mesh = GetMesh();
        auto& vertexes = mesh->GetConstVertexsData();
        if (distribution_ == ParticleSystemDistribution::RANDOM)
        {
            CHECK_ASSERT(vertexes.size() < std::numeric_limits<float>::max());
            std::uniform_real_distribution<float> dis(0, (float)vertexes.size());
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
            velocity = velocity + velocityParams_.objectSpeed_ * rb->GetLinearVelocity();
    }

    if (velocityParams_.randomSpeed_)
    {
        std::uniform_real_distribution<float> dis(0.0f, velocityParams_.randomSpeed_);
        auto random = dis(randGenerator_);
        velocity = velocity * random;
    }

    if (emitFrom_ == PS_EF_VERTS)
    {
        auto mesh = GetMesh();
        auto& vertexes = mesh->GetConstVertexsData();
        auto& vertex = vertexes[currentVertex_];

        if (velocityParams_.normalSpeed_)
        {
            auto& normal = vertex.normal_;
            velocity = velocity + normal * velocityParams_.normalSpeed_;
        }

        if (velocityParams_.tangetSpeed_)
        {
            auto& tangent = vertex.tangent_;
            velocity = velocity + tangent * velocityParams_.tangetSpeed_;
        }
    }

    particle->SetVelocity(velocity);
}

SignalParticleSystem::PSignal ParticleSystem::SignalBeingDestroy()
{
    static SignalParticleSystem::PSignal sig(new SignalParticleSystem);
    if(sig->alive_.ok)
        return sig;
    else
        return SignalParticleSystem::PSignal(new SignalParticleSystem);
    return sig;
}


}
