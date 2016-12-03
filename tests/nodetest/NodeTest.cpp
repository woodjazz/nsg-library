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
#include "NSG.h"
using namespace NSG;

static const Vector3 WORLD_X_COORD(1, 0, 0);
static const Vector3 WORLD_Y_COORD(0, 1, 0);
static const Vector3 WORLD_Z_COORD(0, 0, 1);

static void Test01()
{
    PNode pA(new Node("A"));
    PNode pB = pA->GetOrCreateChild<Node>("B");
    pA->SetPosition(Vertex3(1, 0, 0));
    pB->SetPosition(Vertex3(-2, 0, 0));

    CHECK_CONDITION(pA->GetPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pA->GetGlobalPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pB->GetPosition() == Vertex3(-2, 0, 0));
    CHECK_CONDITION(pB->GetGlobalPosition() == Vertex3(-1, 0, 0));

    pA->SetPosition(Vertex3(2, 0, 0));

    CHECK_CONDITION(pA->GetPosition() == Vertex3(2, 0, 0));
    CHECK_CONDITION(pA->GetGlobalPosition() == Vertex3(2, 0, 0));
    CHECK_CONDITION(pB->GetPosition() == Vertex3(-2, 0, 0));
    CHECK_CONDITION(pB->GetGlobalPosition() == Vertex3(0, 0, 0));

    pA->SetPosition(Vertex3(1, 0, 0));

    CHECK_CONDITION(pA->GetPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pA->GetGlobalPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pB->GetPosition() == Vertex3(-2, 0, 0));
    CHECK_CONDITION(pB->GetGlobalPosition() == Vertex3(-1, 0, 0));

    pB->SetScale(Vertex3(2, 2, 2));

    CHECK_CONDITION(pA->GetPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pA->GetGlobalPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pB->GetPosition() == Vertex3(-2, 0, 0));
    CHECK_CONDITION(pB->GetGlobalPosition() == Vertex3(-1, 0, 0));

    pA->SetScale(Vertex3(2, 2, 2));

    CHECK_CONDITION(pA->GetPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pA->GetGlobalPosition() == Vertex3(1, 0, 0));
    CHECK_CONDITION(pB->GetPosition() == Vertex3(-2, 0, 0));
    CHECK_CONDITION(pB->GetGlobalPosition() == Vertex3(-3, 0, 0));
}

static void Test02()
{
    PNode pA(new Node());
    PNode pB = pA->GetOrCreateChild<Node>("B");
    pA->SetPosition(Vertex3(1, 0, 0));
    pB->SetGlobalPosition(Vertex3(-2, 0, 0));

    CHECK_CONDITION(pB->GetPosition() == Vertex3(-3, 0, 0));
    CHECK_CONDITION(pB->GetGlobalPosition() == Vertex3(-2, 0, 0));
}


static void Test03()
{

    {
        Node node;
        CHECK_CONDITION(node.GetLookAtDirection() == Vector3::LookAt);

        Vector3 lookAt = -WORLD_X_COORD;
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(node.GetLookAtDirection().Distance(lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(vx.Distance(-WORLD_Z_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vy.Distance(WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vz.Distance(WORLD_X_COORD) < 2 * EPSILON);
    }

    {
        Node node;
        CHECK_CONDITION(node.GetLookAtDirection() == Vector3::LookAt);

        Vector3 lookAt = -WORLD_Z_COORD;
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(node.GetLookAtDirection().Distance(lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(vx.Distance(WORLD_X_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vy.Distance(WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vz.Distance(WORLD_Z_COORD) < 2 * EPSILON);
    }

    {
        Node node;
        CHECK_CONDITION(node.GetLookAtDirection() == Vector3::LookAt);

        Vector3 lookAt = -WORLD_Z_COORD;
        node.SetPosition(WORLD_Z_COORD);
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(node.GetLookAtDirection().Distance(lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(vx.Distance(WORLD_X_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vy.Distance(WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vz.Distance(WORLD_Z_COORD) < 2 * EPSILON);
    }

    {
        Node node;
        CHECK_CONDITION(node.GetLookAtDirection() == Vector3::LookAt);

        Vector3 lookAt = WORLD_Z_COORD;
        node.SetPosition(-WORLD_Z_COORD);
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(node.GetLookAtDirection().Distance(lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(vx.Distance(-WORLD_X_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vy.Distance(WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(vz.Distance(-WORLD_Z_COORD) < 2 * EPSILON);
    }

    {
        Node node;
        CHECK_CONDITION(node.GetLookAtDirection() == Vector3::LookAt);

        Vector3 lookAt = WORLD_Z_COORD;
        node.SetPosition(WORLD_X_COORD + WORLD_Y_COORD + WORLD_Z_COORD);
        node.SetGlobalLookAtPosition(lookAt);

        Vector3 expectedLookAt = Vector3(-0.7071068f, -0.7071068f, 0);
        Vector3 currentLookAt = node.GetLookAtDirection();
        CHECK_CONDITION(currentLookAt.Distance(expectedLookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        Vector3 expectedVy = Vector3(-0.7071068f, 0.7071068f, 0);
        Vector3 expectedVz = -expectedLookAt;
        Vector3 expectedVx = expectedVy.Cross(expectedVz);

        CHECK_CONDITION(vx.Distance(expectedVx) < 0.00001f);
		CHECK_CONDITION(vy.Distance(expectedVy) < 0.00001f);
		CHECK_CONDITION(vz.Distance(expectedVz) < 0.00001f);
    }

    {
        Node node;
        node.SetGlobalPosition(Vector3(0, 5, 5));
        node.SetGlobalLookAtPosition(Vector3::Zero);
        Vector3 dir = node.GetLookAtDirection();
        Vector3 expectedDir(Vector3(0, -1, -1).Normalize());
        CHECK_CONDITION(dir.Distance(expectedDir) < 2 * EPSILON);
    }

    {
        Node node;
        node.SetGlobalPosition(Vector3(5, 5, 0));
        node.SetGlobalLookAtPosition(Vector3::Zero);
        Vector3 dir = node.GetLookAtDirection();
        Vector3 expectedDir(Vector3(-1, -1, 0).Normalize());
        CHECK_CONDITION(dir.Distance(expectedDir) < 2 * EPSILON);
    }

    {
        PNode parent = std::make_shared<Node>("parent");
        parent->SetScale(Vector3(0.010f));
        PNode node0 = std::make_shared<Node>("node0");
        node0->SetParent(parent);
        node0->SetGlobalPosition(Vector3(0, 5, 5));
        node0->SetGlobalLookAtPosition(Vector3::Zero);
        Vector3 dir0 = node0->GetLookAtDirection();
        Quaternion q0 = node0->GetOrientation();

        PNode node1 = std::make_shared<Node>("node1");
        node1->SetParent(parent);
        node1->SetGlobalPosition(Vector3(5, 5, 0));
        node1->SetGlobalLookAtPosition(Vector3::Zero);
        Vector3 dir1 = node1->GetLookAtDirection();
        Quaternion q1 = node1->GetOrientation();

        {
            auto q = q0.Slerp(q1, 0.0f);
            Node node;
            node.SetGlobalPosition(node1->GetGlobalPosition());
            node.SetOrientation(q);
            Vector3 dir = node.GetLookAtDirection();
            auto dist = dir0.Distance(dir);
            CHECK_CONDITION(dist < 2 * EPSILON);
        }

        {
            auto q = q0.Slerp(q1, 0.5f);
            Node node;
            node.SetGlobalPosition(node1->GetGlobalPosition());
            node.SetOrientation(q);
            Vector3 dir = node.GetLookAtDirection();
            Vector3 expectedDir(-0.5f, dir0.y, -0.5f);
            CHECK_CONDITION(expectedDir.Distance(dir) < 2 * EPSILON);
        }


        {
            auto q = q0.Slerp(q1, 1.0f);
            Node node;
            node.SetGlobalPosition(node1->GetGlobalPosition());
            node.SetOrientation(q);
            Vector3 dir = node.GetLookAtDirection();
            CHECK_CONDITION(dir1.Distance(dir) < 2 * EPSILON);
        }

    }


}

static void Test04()
{
    Vertex3 v1(1, 0, 0);

    PNode pA(new Node());
    pA->SetPosition(Vertex3(2, 0, 0));
    pA->SetOrientation(Quaternion(PI, Vertex3(0, 0, 1)));
    Vertex3 v2(pA->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(v2.Distance(Vertex3(1, 0, 0)) < 2 * EPSILON);

    pA->SetScale(Vertex3(2, 2, 2));

    v2 = Vertex3(pA->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(v2.Distance(Vertex3(0, 0, 0)) < 2 * EPSILON);
}

static void Test05()
{
    Vertex3 v1(0, 0, 0);

    PNode pA(new Node());
    pA->SetPosition(Vertex3(2, 0, 0));

    PNode pB(pA->GetOrCreateChild<Node>("B"));
    pB->SetPosition(Vertex3(1, 0, 0));
    pB->SetOrientation(Quaternion(PI, Vertex3(0, 0, 1)));

    Vertex3 v2(pB->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(v2.Distance(Vertex3(3, 0, 0)) < 2 * EPSILON);

    pA->SetScale(Vertex3(2, 2, 2));

    v2 = Vertex3(pB->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(v2.Distance(Vertex3(4, 0, 0)) < 2 * EPSILON);
}

static void Test06()
{
    Vertex3 v1(0, 0, 0);

    PNode pA(new Node());
    pA->SetPosition(Vertex3(0, 0, 0));

    PNode pB(pA->GetOrCreateChild<Node>("B"));
    pB->SetPosition(Vertex3(-1, 0, 0));
    pB->SetScale(Vertex3(0.5f, 1, 1));

    Vertex3 v2(pB->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(v2.Distance(Vertex3(-1, 0, 0)) < 2 * EPSILON);
}

static void Test07()
{
    PNode pA(new Node());
    Vertex3 position = Vertex3(2, 1, 3);
    pA->SetPosition(position);
    Vertex3 scale = Vertex3(3, 4, 5);
    pA->SetScale(scale);
    Quaternion q = Quaternion(PI, Vertex3(0, 1, 0));
    pA->SetOrientation(q);

    Vertex3 position1;
    Quaternion q1;
    Vertex3 scale1;

    Matrix4 m = pA->GetGlobalModelMatrix();

    m.Decompose(position1, q1, scale1);

    Vertex3 a1 = q1.EulerAngles();
    Vertex3 a2 = q.EulerAngles();

    CHECK_CONDITION(a1.Distance(a2) < 2 * EPSILON);

    CHECK_CONDITION(position == position1);

    CHECK_CONDITION(scale == scale1);
}

static void Test08()
{
    auto parent = std::make_shared<Node>();
    {
        Vertex3 position = Vertex3(2, 1, 3);
        parent->SetPosition(position);
        Vertex3 scale = Vertex3(3, 4, 5);
        parent->SetScale(scale);
        auto q = Quaternion(PI, Vertex3(0, 1, 0));
        parent->SetOrientation(q);
    }

    auto node = std::make_shared<Node>();
    Vertex3 pos = Vertex3(2, 1, 3);
    Vertex3 scale = Vertex3(3, 4, 5);
    auto q = Quaternion(PI, Vertex3(0, 1, 0));
    node->SetPosition(pos);
    node->SetScale(scale);
    node->SetOrientation(q);

    Matrix4 m = node->GetGlobalModelMatrix();

    node->SetParent(parent);

    Matrix4 parentinv = parent->GetGlobalModelInvMatrix();
    Quaternion parent_q;
    Vector3 parent_pos;
    Vector3 parent_scale;
	parentinv.Decompose(parent_pos, parent_q, parent_scale);

    {
        node->SetPosition(parent_pos + parent_q * (parent_scale * pos));
        node->SetOrientation(parent_q * q);
        node->SetScale(parent_scale * scale);

		CHECK_CONDITION(node->GetGlobalPosition().Distance(pos) < 0.0001f);
        Vertex3 a1 = node->GetGlobalOrientation() * Vector3::LookAt;
        Vertex3 a2 = q * Vector3::LookAt;
		CHECK_CONDITION(a1.Distance(a2) < 0.01f);
		CHECK_CONDITION(node->GetGlobalScale().Distance(scale) < 0.0001f);
    }
    
    {
        Matrix4 m1 = parentinv * m;
        Quaternion q1;
        Vector3 pos1;
        Vector3 scale1;
		m1.Decompose(pos1, q1, scale1);
        node->SetPosition(pos1);
        node->SetOrientation(q1);
        node->SetScale(scale1);

        CHECK_CONDITION(node->GetGlobalPosition() == pos);
        Vertex3 a1 = node->GetGlobalOrientation() * Vector3::LookAt;
        Vertex3 a2 = q * Vector3::LookAt;
		CHECK_CONDITION(a1.Distance(a2) < 0.01f);
        CHECK_CONDITION(node->GetGlobalScale().Distance(scale) < 0.01f);
    }
}

static void Test09()
{
    Node dirPositiveX;
    Node dirNegativeX;
    Node dirPositiveY;
    Node dirNegativeY;
    Node dirPositiveZ;
    Node dirNegativeZ;
    dirPositiveX.SetLocalLookAtPosition(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
    dirNegativeX.SetLocalLookAtPosition(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
    dirPositiveY.SetLocalLookAtPosition(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
    dirNegativeY.SetLocalLookAtPosition(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
    dirPositiveZ.SetLocalLookAtPosition(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
    dirNegativeZ.SetLocalLookAtPosition(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f));

    CHECK_CONDITION(Vector3(1.0f, 0.0f, 0.0f).Distance(dirPositiveX.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Vector3(0.0f, -1.0f, 0.0f).Distance(dirPositiveX.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Vector3(-1.0f, 0.0f, 0.0f).Distance(dirNegativeX.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Vector3(0.0f, -1.0f, 0.0f).Distance(dirNegativeX.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Vector3(0.0f, 1.0f, 0.0f).Distance(dirPositiveY.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Vector3(0.0f, 0.0f, -1.0f).Distance(dirPositiveY.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Vector3(0.0f, -1.0f, 0.0f).Distance(dirNegativeY.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Vector3(0.0f, 0.0f, 1.0f).Distance(dirNegativeY.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Vector3(0.0f, 0.0f, 1.0f).Distance(dirPositiveZ.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Vector3(0.0f, -1.0f, 0.0f).Distance(dirPositiveZ.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Vector3(0.0f, 0.0f, -1.0f).Distance(dirNegativeZ.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Vector3(0.0f, -1.0f, 0.0f).Distance(dirNegativeZ.GetUpDirection()) < 0.001f);

}

void NodeTest()
{
    Test01();
    Test02();
    Test03();
    Test04();
    Test05();
    Test06();
    Test07();
    Test08();
    Test09();

}
