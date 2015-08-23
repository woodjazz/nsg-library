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

#include "NSG.h"
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
    auto window = Window::Create();
    PSceneNode player;
    LoaderApp loader(Resource::GetOrCreateClass<ResourceFile>("data/scene.xml"));
    auto slotLoaded = loader.Load()->Connect([&]()
    {
        auto scene = loader.GetScene(0);
        auto camera = scene->GetChild<Camera>("Camera", false);
        static auto followCamera = std::make_shared<FollowCamera>(camera);
        player = scene->GetChild<SceneNode>("RigMomo", true);
        followCamera->Track(player);
        followCamera->SetOffset(Vector3(0, 20, -40));
        static float turn = 0;

        struct State : FSM::State
        {
            bool loop_;
            float time_;
            PScene scene_;
            PSceneNode player_;
            PRigidBody rigidBody_;
            PAnimation animation_;
            State(const char* animName, PScene scene)
                : loop_(true), time_(0), scene_(scene)
            {
                player_ = scene_->GetChild<SceneNode>("RigMomo", true);
                rigidBody_ = player_->GetRigidBody();
                animation_ = scene_->GetAnimationFor(animName, player_);
            }
            void Begin() override
            {
                scene_->PlayAnimation(animation_, loop_);
            }
            void Stay() override
            {
                time_ += Engine::GetPtr()->GetDeltaTime();
            }
            void End() override
            {
                time_ = 0;
                scene_->StopAnimation(animation_);
            }
        };

        struct Idle : State
        {
            Idle(PScene scene) : State("Momo_IdleNasty", scene)
            {
            }
            void Begin() override
            {
                State::Begin();
            }
            void Stay() override
            {
                State::Stay();
                rigidBody_->SetLinearVelocity(VECTOR3_ZERO);
                rigidBody_->SetAngularVelocity(VECTOR3_ZERO);
            }
        } static idle(scene);

        struct Walk : State
        {
            Walk(PScene scene) : State("Momo_Walk", scene)
            {
            }
            void Begin() override
            {
                State::Begin();
            }
            void Stay() override
            {
                State::Stay();
                auto forwardDir = player_->GetGlobalOrientation() * VECTOR3_UP;
                rigidBody_->SetLinearVelocity(-5.f * forwardDir);
                rigidBody_->SetAngularVelocity(Vector3(0, 2.f * turn, 0));
            }
        } static walk(scene);

        struct WalkBack : State
        {
            WalkBack(PScene scene) : State("Momo_WalkBack", scene)
            {
            }
            void Begin() override
            {
                State::Begin();
            }
            void Stay() override
            {
                State::Stay();
                auto forwardDir = player_->GetGlobalOrientation() * VECTOR3_UP;
                rigidBody_->SetLinearVelocity(3.f * forwardDir);
                rigidBody_->SetAngularVelocity(Vector3(0, 2.f * turn, 0));
            }
        } static walkBack(scene);

        struct Run : State
        {
            Run(PScene scene) : State("Momo_Run", scene)
            {
            }
            void Begin() override
            {
                State::Begin();
            }
            void Stay() override
            {
                State::Stay();
                auto forwardDir = player_->GetGlobalOrientation() * VECTOR3_UP;
                rigidBody_->SetLinearVelocity(-15.f * forwardDir);
                rigidBody_->SetAngularVelocity(Vector3(0, 2.f * turn, 0));
            }
        } static run(scene);

        struct TurnL : State
        {
            TurnL(PScene scene) : State("Momo_Turn.R", scene)
            {
            }
            void Begin() override
            {
                State::Begin();
            }
            void Stay() override
            {
                State::Stay();
                rigidBody_->SetLinearVelocity(VECTOR3_ZERO);
                rigidBody_->SetAngularVelocity(Vector3(0, 2.f * turn, 0));
            }
        } static turnL(scene);

        struct TurnR : State
        {
            TurnR(PScene scene) : State("Momo_Turn.L", scene)
            {
            }
            void Begin() override
            {
                State::Begin();
            }
            void Stay() override
            {
                State::Stay();
                rigidBody_->SetLinearVelocity(VECTOR3_ZERO);
                rigidBody_->SetAngularVelocity(Vector3(0, 2.f * turn, 0));
            }
        } static turnR(scene);

        static bool buttonA = false;
        struct Jump : State
        {
            Jump(PScene scene) : State("Momo_Jump", scene)
            {
                loop_ = false;
            }
            void Begin() override
            {
                State::Begin();
                rigidBody_->ApplyForce(VECTOR3_UP * 2000.f);
                buttonA = false;
            }
            void End() override
            {
                State::End();
            }
        } static jump(scene);

        struct Glide : State
        {
            Vector3 gravity_;
            Glide(PScene scene) : State("Momo_Glide", scene)
            {
            }
            void Begin() override
            {
                gravity_ = scene_->GetPhysicsWorld()->GetGravity();
                rigidBody_->SetGravity(VECTOR3_ZERO);
                State::Begin();
            }
            void Stay() override
            {
                State::Stay();
                auto forwardDir = player_->GetGlobalOrientation() * VECTOR3_UP;
                rigidBody_->SetLinearVelocity(-20.f * forwardDir);
                rigidBody_->SetAngularVelocity(Vector3(0, 2.f * turn, 0));
            }
            void End() override
            {
                rigidBody_->SetGravity(gravity_);
                State::End();
            }
        } static glide(scene);

        struct Fall : State
        {
            float turnFactor_;
            Fall(PScene scene) : State("Momo_FallUp", scene)
            {
            }
            void Begin() override
            {
                State::Begin();
                rigidBody_->SetLinearVelocity(rigidBody_->GetLinearVelocity() - VECTOR3_UP);
            }
            void End() override
            {
                State::End();
            }
        } static fall(scene);

        static auto IsFalling = [&]()
        {
            return player->GetRigidBody()->GetLinearVelocity().y < 0;
        };

        static float speed = 0;
        static FSM::Machine fsm(fall);
        idle.AddTransition(walk).When([&]() { return speed > 0; });
        idle.AddTransition(walkBack).When([&]() { return speed < 0; });
        idle.AddTransition(turnL).When([&]() { return turn < 0; });
        idle.AddTransition(turnR).When([&]() { return turn > 0; });
        idle.AddTransition(jump).When([&]() { return buttonA; });
        jump.AddTransition(fall).When([&]() { return IsFalling(); });
        jump.AddTransition(glide).When([&]() { return buttonA; });
        fall.AddTransition(idle).When([&]() { return !IsFalling(); });
        fall.AddTransition(glide).When([&]() { return buttonA; });
        glide.AddTransition(fall).When([&]() { return !buttonA; });
        turnL.AddTransition(idle).When([&]() { return turn == 0; });
        turnL.AddTransition(turnR).When([&]() { return turn > 0; });
        turnL.AddTransition(walk).When([&]() { return speed > 0; });
        turnR.AddTransition(idle).When([&]() { return turn == 0; });
        turnR.AddTransition(turnL).When([&]() { return turn < 0; });
        turnR.AddTransition(walk).When([&]() { return speed > 0; });
        walk.AddTransition(run).When([&]() { return speed > 0 && walk.time_ > 1.3f; });
        walk.AddTransition(idle).When([&]() { return speed == 0 && walk.time_ > 1; });
        walk.AddTransition(idle).When([&]() { return speed < 0; });
        walk.AddTransition(jump).When([&]() { return buttonA; });
        walkBack.AddTransition(idle).When([&]() { return speed >= 0; });
        run.AddTransition(walk).When([&]() { return speed == 0 && run.time_ > 1; });
        run.AddTransition(jump).When([&]() { return buttonA; });
        window->SetScene(scene.get());
        fsm.Go();

        static auto playerControl = std::make_shared<PlayerControl>();

        static auto slotMoved = playerControl->SigMoved()->Connect([&](float x, float y)
        {
            speed = y;
            turn = -x;
        });

        static auto slotButtonA = playerControl->SigButtonA()->Connect([&](bool pressed)
        {
            buttonA = pressed;
        });
    });

    auto scene = std::make_shared<Scene>();
    auto font = std::make_shared<FontAtlas>();
    auto loadingNode = scene->CreateChild<SceneNode>();
    auto loadingMaterial = Material::Create();
    font->Set(Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.xml"));
    auto atlasTexture = std::make_shared<Texture2D>(Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.png"));
    font->SetTexture(atlasTexture);
    auto camera = scene->CreateChild<Camera>();
    camera->SetPosition(Vector3(0, 0, 1));
    camera->EnableOrtho();
    camera->SetWindow(nullptr);
    loadingNode->SetMesh(font->GetOrCreateMesh("Loading...", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT));
    loadingMaterial->SetTextMap(atlasTexture);
    loadingNode->SetMaterial(loadingMaterial);
    window->SetScene(scene.get());
    auto engine = Engine::Create();
    return engine->Run();
}
