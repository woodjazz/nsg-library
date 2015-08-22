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
float turn = 0;
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;
    auto window = Window::Create();
    auto resource = Resource::GetOrCreateClass<ResourceFile>("data/scene.xml");
    auto data = AppData::Create();
    auto engine = Engine::Create();

    auto scene = std::make_shared<Scene>();
    auto font = std::make_shared<FontAtlas>();
    auto loadingNode = scene->CreateChild<SceneNode>();
    auto loadingMaterial = Material::Create();
    {
        auto xmlResource = Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.xml");
        font->Set(xmlResource);
        auto atlasResource = Resource::GetOrCreate<ResourceFile>("data/AnonymousPro32.png");
        auto atlasTexture = std::make_shared<Texture2D>(atlasResource);
        font->SetTexture(atlasTexture);

        auto camera = scene->CreateChild<Camera>();
        camera->SetPosition(Vector3(0, 0, 2));
        camera->EnableOrtho();
        camera->SetWindow(nullptr);
        loadingNode->SetMesh(font->GetOrCreateMesh("Loading...", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT));
        loadingMaterial->SetTextMap(atlasTexture);
        loadingNode->SetMaterial(loadingMaterial);
        window->SetScene(scene.get());
    }
    engine->RenderFrame(); // force load


    PFollowCamera followCamera;
    PSceneNode player;

    float speed = 0;
    bool buttonA = false;
    bool isFalling = false;

    FSM::Machine fsm;

    auto playerControl = std::make_shared<PlayerControl>();

    auto slotMoved = playerControl->SigMoved()->Connect([&](float x, float y)
    {
        speed = y;
        turn = -x;
    });

    auto slotButtonA = playerControl->SigButtonA()->Connect([&](bool pressed)
    {
        buttonA = pressed;
    });

    auto loaded = false;
    auto allReady = false;
    auto load = [&]()
    {
        data->Load(resource);
        auto scene = data->scenes_[0];
        auto camera = scene->GetChild<Camera>("Camera", false);
        followCamera = std::make_shared<FollowCamera>(camera);
        player = scene->GetChild<SceneNode>("RigMomo", true);
        followCamera->Track(player);
        followCamera->SetOffset(Vector3(0, 20, -40));

        struct State : FSM::State
        {
            bool loop_;
            float time_;
            PScene scene_;
            PSceneNode player_;
            PRigidBody rigidBody_;
            PAnimation animation_;
            State(const char* animName)
                : loop_(true), time_(0)
            {
                scene_ = AppData::GetPtr()->scenes_[0];
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
            Idle() : State("Momo_IdleNasty")
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
        } static idle;

        struct Walk : State
        {
            Walk() : State("Momo_Walk")
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
        } static walk;

        struct WalkBack : State
        {
            WalkBack() : State("Momo_WalkBack")
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
        } static walkBack;

        struct Run : State
        {
            Run() : State("Momo_Run")
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
        } static run;

        struct TurnL : State
        {
            TurnL() : State("Momo_Turn.R")
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
        } static turnL;

        struct TurnR : State
        {
            TurnR() : State("Momo_Turn.L")
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
        } static turnR;

        struct Jump : State
        {
            Jump() : State("Momo_Jump")
            {
                loop_ = false;
            }
            void Begin() override
            {
                State::Begin();
                rigidBody_->ApplyForce(VECTOR3_UP * 2000.f);
            }
            void End() override
            {
                State::End();
            }
        } static jump;

        struct Glide : State
        {
            Vector3 gravity_;
            Glide() : State("Momo_Glide")
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
        } static glide;

        struct Fall : State
        {
            float turnFactor_;
            Fall() : State("Momo_FallUp")
            {
            }
            void Begin() override
            {
                State::Begin();
            }
            void End() override
            {
                State::End();
            }
        } static fall;

        fsm.SetInitialState(fall);
        idle.AddTransition(walk).When([&]() { return speed > 0; });
        idle.AddTransition(walkBack).When([&]() { return speed < 0; });
        idle.AddTransition(turnL).When([&]() { return turn < 0; });
        idle.AddTransition(turnR).When([&]() { return turn > 0; });
        idle.AddTransition(jump).When([&]() { return buttonA; });
        jump.AddTransition(glide).When([&]() { return buttonA && isFalling; });
        jump.AddTransition(fall).When([&]() { return !buttonA && isFalling; });
        glide.AddTransition(fall).When([&]() { return !buttonA; });
        fall.AddTransition(idle).When([&]() { return !isFalling; });
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
    };

    auto slotUpdate = engine->SigUpdate()->Connect([&](float deltaTime)
    {
        if (!loaded)
        {
            if (AppData::AreReady())
            {
                load();
                loaded = true;
            }
        }
        else if(!allReady)
        {
            allReady = AppData::AreReady();
            if(allReady)
                window->SetScene(AppData::GetPtr()->scenes_[0].get());
        }
        else
        {
            auto rigidBody = player->GetRigidBody();
            auto v = rigidBody->GetLinearVelocity();
            isFalling = v.y < 0;
            fsm.Update();
            //LOGI("%s", ToString(v).c_str());
            //LOGI("%f", speed);
        }
    });

    return engine->Run();
}
