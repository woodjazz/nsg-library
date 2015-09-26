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
using namespace NSG;

static PCharacter SetupScene(PScene scene)
{
    scene->SetAmbientColor(ColorRGB(0.f));
    auto materialFloor = Material::Create();
	materialFloor->SetRenderPass(RenderPass::LIT);
    materialFloor->SetDiffuseColor(ColorRGB(0.1f));
    materialFloor->CastShadow(false);
    auto boxMesh(Mesh::Create<BoxMesh>());
    auto sphereMesh(Mesh::Create<SphereMesh>());

    auto nodeFloor = scene->CreateChild<SceneNode>("floor");
    nodeFloor->SetMesh(boxMesh);
    nodeFloor->SetMaterial(materialFloor);
    auto scale = Vertex3(100, 2, 100);
    nodeFloor->SetScale(scale);
    auto body = nodeFloor->GetOrCreateRigidBody();
    auto floorShape = Shape::Create(ShapeKey(boxMesh, scale));
    body->AddShape(floorShape);

    auto materialBox = Material::Create();
	materialBox->SetRenderPass(RenderPass::LIT);
    materialBox->SetDiffuseColor(COLOR_RED);

    auto nodeBox = scene->CreateChild<SceneNode>("Player");
    nodeBox->SetMesh(sphereMesh);
    nodeBox->SetMaterial(materialBox);
    auto characterScale = Vertex3(1, 3, 1);
    nodeBox->SetScale(characterScale);
    nodeBox->SetPosition(Vertex3(0, 6, 5));
    auto controller = nodeBox->GetOrCreateCharacter();
    std::swap(characterScale.y, characterScale.z);
    auto controllerShape = Shape::GetOrCreate(ShapeKey(PhysicsShape::SH_BOX, characterScale));
    controllerShape->SetBB(BoundingBox(-1, 1));
    auto q = AngleAxis(Radians(90.f), Vertex3(1, 0, 0));
    controller->AddShape(controllerShape, VECTOR3_ZERO, q);

    auto materialObstacle = Material::Create();
	materialObstacle->SetRenderPass(RenderPass::LIT);
    materialObstacle->SetDiffuseColor(COLOR_RED);

    {
        auto node = scene->CreateChild<SceneNode>("Obstacle0");
        node->SetMesh(boxMesh);
        node->SetMaterial(materialObstacle);
        node->SetPosition(Vertex3(0, 1, -14));
        auto scale = Vector3(2, 10.5f, 2);
        node->SetScale(scale);
        auto body = node->GetOrCreateRigidBody();
        auto shape = Shape::GetOrCreate(ShapeKey(boxMesh, scale));
        body->AddShape(shape);
    }

    {
        auto node = scene->CreateChild<SceneNode>("Obstacle1");
        node->SetMesh(boxMesh);
        node->SetMaterial(materialObstacle);
        node->SetPosition(Vertex3(15, 1, -10));
        auto scale = Vector3(2, 10.5f, 2);
        node->SetScale(scale);
        auto body = node->GetOrCreateRigidBody();
        auto shape = Shape::GetOrCreate(ShapeKey(boxMesh, scale));
        body->AddShape(shape);
    }

    {
        auto node = scene->CreateChild<SceneNode>("StepObstacle");
        node->SetMesh(boxMesh);
        node->SetMaterial(materialObstacle);
        node->SetPosition(Vertex3(10, 2, 0));
        auto scale = Vector3(2, 1.2f, 2);
        node->SetScale(scale);
        auto body = node->GetOrCreateRigidBody();
        auto shape = Shape::GetOrCreate(ShapeKey(boxMesh, scale));
        body->AddShape(shape);
    }

    {
        auto node = scene->CreateChild<SceneNode>("Obstacle2");
        node->SetMesh(boxMesh);
        node->SetMaterial(materialObstacle);
        node->SetPosition(Vertex3(10, 1, 15));
        auto scale = Vector3(2, 10.5f, 2);
        node->SetScale(scale);
        auto body = node->GetOrCreateRigidBody();
        auto shape = Shape::GetOrCreate(ShapeKey(boxMesh, scale));
        body->AddShape(shape);
    }

    auto camera = scene->CreateChild<Camera>("Camera");
    camera->SetPosition(Vertex3(0, 9, 25));
    camera->SetGlobalLookAtPosition(Vector3(0, -1, -1));
    camera->SetFarClip(200);
    static PCameraControl control;
    control = std::make_shared<CameraControl>(camera);
    auto light = scene->CreateChild<Light>("Sun");
    light->SetType(LightType::DIRECTIONAL);
    light->SetPosition(Vertex3(10, 6, -10));
    light->SetGlobalLookAtDirection(Vector3(0, -1, 0));
    light->SetShadowColor(Color(0, 1, 0, 1));

    //static auto followCamera = std::make_shared<FollowCamera>(camera);
    //followCamera->Track(controller.get(), 10);

    return controller;
}

static void Test01()
{
    static auto scene = std::make_shared<Scene>();
    static auto character = SetupScene(scene);
	static auto window = Window::Create("0", 0, 0, 10, 10, (int)WindowFlag::HIDDEN);
    window->SetScene(scene.get());

    struct GoForward : FSM::State
    {
        void Stay() override
        {
            character->SetForwardSpeed(5);
            character->SetAngularSpeed(0);
        }
    } forward;

    struct TurnRight : FSM::State
    {
		void Begin() override
		{
			character->Rotate(-90);
		}
        void Stay() override
        {
            character->SetForwardSpeed(0);
        }
    } turnRight;

    struct Exit : FSM::State
    {
        void Begin() override
        {
            window = nullptr;
        }
    } exit;

    static auto goal = scene->GetChild<SceneNode>("Obstacle2", false);
    FSM::Machine fsm(forward);
    auto ExitFunc = [&]()
    {
        auto collider = character->StepForwardCollides();
        return collider && goal.get() == collider->GetSceneNode();
    };

    turnRight.AddTransition(exit).When([&]() { return ExitFunc(); });
    forward.AddTransition(exit).When([&]() { return ExitFunc(); });

	forward.AddTransition(turnRight).When([&]() { return nullptr != character->StepForwardCollides(); });
	turnRight.AddTransition(forward).When([&]() { return nullptr == character->StepForwardCollides(); });

    fsm.Go();
    Engine::Create()->Run();
}

static void Test02()
{
    auto scene = std::make_shared<Scene>();
    auto character = SetupScene(scene);
    auto window = Window::Create();
    window->SetScene(scene.get());

    bool buttonA = false;
    float speed = 0;
    float turn = 0;

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

    auto slotUpdate = Engine::SigUpdate()->Connect([&](float deltaTime)
    {
        character->SetForwardSpeed(5 * speed);
        character->SetAngularSpeed(turn * 90);
        if (buttonA)
            character->SetJumpSpeed(20);

    });

    auto engine = Engine::Create();
    engine->Run();
}

void Test()
{
    Test01();
    //Test02();
}
