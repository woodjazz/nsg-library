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
		CHECK_CONDITION(node.GetLookAtDirection() == VECTOR3_LOOKAT_DIRECTION);

        Vector3 lookAt = -WORLD_X_COORD;
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(Distance(node.GetLookAtDirection(), lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(Distance(vx, -WORLD_Z_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vy, WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vz, WORLD_X_COORD) < 2 * EPSILON);
    }

    {
        Node node;
		CHECK_CONDITION(node.GetLookAtDirection() == VECTOR3_LOOKAT_DIRECTION);

        Vector3 lookAt = -WORLD_Z_COORD;
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(Distance(node.GetLookAtDirection(), lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(Distance(vx, WORLD_X_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vy, WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vz, WORLD_Z_COORD) < 2 * EPSILON);
    }

    {
        Node node;
		CHECK_CONDITION(node.GetLookAtDirection() == VECTOR3_LOOKAT_DIRECTION);

        Vector3 lookAt = -WORLD_Z_COORD;
        node.SetPosition(WORLD_Z_COORD);
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(Distance(node.GetLookAtDirection(), lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(Distance(vx, WORLD_X_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vy, WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vz, WORLD_Z_COORD) < 2 * EPSILON);
    }

    {
        Node node;
		CHECK_CONDITION(node.GetLookAtDirection() == VECTOR3_LOOKAT_DIRECTION);

        Vector3 lookAt = WORLD_Z_COORD;
        node.SetPosition(-WORLD_Z_COORD);
        node.SetGlobalLookAtPosition(lookAt);

        CHECK_CONDITION(Distance(node.GetLookAtDirection(), lookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        CHECK_CONDITION(Distance(vx, -WORLD_X_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vy, WORLD_Y_COORD) < 2 * EPSILON);
        CHECK_CONDITION(Distance(vz, -WORLD_Z_COORD) < 2 * EPSILON);
    }

    {
        Node node;
		CHECK_CONDITION(node.GetLookAtDirection() == VECTOR3_LOOKAT_DIRECTION);

        Vector3 lookAt = WORLD_Z_COORD;
        node.SetPosition(WORLD_X_COORD + WORLD_Y_COORD + WORLD_Z_COORD);
        node.SetGlobalLookAtPosition(lookAt);

        Vector3 expectedLookAt = Vector3(-0.7071068f, -0.7071068f, 0);
        Vector3 currentLookAt = node.GetLookAtDirection();
        CHECK_CONDITION(Distance(currentLookAt, expectedLookAt) < 2 * EPSILON);

        Vector3 vx = node.GetGlobalOrientation() * WORLD_X_COORD;
        Vector3 vy = node.GetGlobalOrientation() * WORLD_Y_COORD;
        Vector3 vz = node.GetGlobalOrientation() * WORLD_Z_COORD;

        Vector3 expectedVy = Vector3(-0.7071068f, 0.7071068f, 0);
        Vector3 expectedVz = -expectedLookAt;
        Vector3 expectedVx = Cross(expectedVy, expectedVz);

        CHECK_CONDITION(Distance(vx, expectedVx) < 0.00001f);
		CHECK_CONDITION(Distance(vy, expectedVy) < 0.00001f);
		CHECK_CONDITION(Distance(vz, expectedVz) < 0.00001f);
    }

    {
        Node node;
        node.SetGlobalPosition(Vector3(0, 5, 5));
        node.SetGlobalLookAtPosition(VECTOR3_ZERO);
        Vector3 dir = node.GetLookAtDirection();
        Vector3 expectedDir(Normalize(Vector3(0, -1, -1)));
        CHECK_CONDITION(Distance(dir, expectedDir) < 2 * EPSILON);
    }

    {
        Node node;
        node.SetGlobalPosition(Vector3(5, 5, 0));
        node.SetGlobalLookAtPosition(VECTOR3_ZERO);
        Vector3 dir = node.GetLookAtDirection();
        Vector3 expectedDir(Normalize(Vector3(-1, -1, 0)));
        CHECK_CONDITION(Distance(dir, expectedDir) < 2 * EPSILON);
    }

    {
        PNode parent = std::make_shared<Node>("parent");
        parent->SetScale(Vector3(0.010f));
        PNode node0 = std::make_shared<Node>("node0");
        node0->SetParent(parent);
        node0->SetGlobalPosition(Vector3(0, 5, 5));
        node0->SetGlobalLookAtPosition(VECTOR3_ZERO);
        Vector3 dir0 = node0->GetLookAtDirection();
        Quaternion q0 = node0->GetOrientation();

        PNode node1 = std::make_shared<Node>("node1");
        node1->SetParent(parent);
        node1->SetGlobalPosition(Vector3(5, 5, 0));
        node1->SetGlobalLookAtPosition(VECTOR3_ZERO);
        Vector3 dir1 = node1->GetLookAtDirection();
        Quaternion q1 = node1->GetOrientation();

        {
            Quaternion q = Slerp(q0, q1, 0.0f);
            Node node;
            node.SetGlobalPosition(node1->GetGlobalPosition());
            node.SetOrientation(q);
            Vector3 dir = node.GetLookAtDirection();
            CHECK_CONDITION(Distance(dir0, dir) < 2 * EPSILON);
        }

        {
            Quaternion q = Slerp(q0, q1, 0.5f);
            Node node;
            node.SetGlobalPosition(node1->GetGlobalPosition());
            node.SetOrientation(q);
            Vector3 dir = node.GetLookAtDirection();
            Vector3 expectedDir(-0.5f, dir0.y, -0.5f);
            CHECK_CONDITION(Distance(expectedDir, dir) < 2 * EPSILON);
        }


        {
            Quaternion q = Slerp(q0, q1, 1.0f);
            Node node;
            node.SetGlobalPosition(node1->GetGlobalPosition());
            node.SetOrientation(q);
            Vector3 dir = node.GetLookAtDirection();
            CHECK_CONDITION(Distance(dir1, dir) < 2 * EPSILON);
        }

    }


}

static void Test04()
{
    Vertex3 v1(1, 0, 0);

    PNode pA(new Node());
    pA->SetPosition(Vertex3(2, 0, 0));
    pA->SetOrientation(AngleAxis(PI, Vertex3(0, 0, 1)));
    Vertex3 v2(pA->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(Distance(v2, Vertex3(1, 0, 0)) < 2 * EPSILON);

    pA->SetScale(Vertex3(2, 2, 2));

    v2 = Vertex3(pA->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(Distance(v2, Vertex3(0, 0, 0)) < 2 * EPSILON);
}

static void Test05()
{
    Vertex3 v1(0, 0, 0);

    PNode pA(new Node());
    pA->SetPosition(Vertex3(2, 0, 0));

    PNode pB(pA->GetOrCreateChild<Node>("B"));
    pB->SetPosition(Vertex3(1, 0, 0));
    pB->SetOrientation(AngleAxis(PI, Vertex3(0, 0, 1)));

    Vertex3 v2(pB->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(Distance(v2, Vertex3(3, 0, 0)) < 2 * EPSILON);

    pA->SetScale(Vertex3(2, 2, 2));

    v2 = Vertex3(pB->GetGlobalModelMatrix() * Vertex4(v1, 1));

    CHECK_CONDITION(Distance(v2, Vertex3(4, 0, 0)) < 2 * EPSILON);
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

    CHECK_CONDITION(Distance(v2, Vertex3(-1, 0, 0)) < 2 * EPSILON);
}

static void Test07()
{
    PNode pA(new Node());
    Vertex3 position = Vertex3(2, 1, 3);
    pA->SetPosition(position);
    Vertex3 scale = Vertex3(3, 4, 5);
    pA->SetScale(scale);
    Quaternion q = AngleAxis(PI, Vertex3(0, 1, 0));
    pA->SetOrientation(q);

    Vertex3 position1;
    Quaternion q1;
    Vertex3 scale1;

    Matrix4 m = pA->GetGlobalModelMatrix();

    DecomposeMatrix(m, position1, q1, scale1);

    Vertex3 a1 = EulerAngles(q1);
    Vertex3 a2 = EulerAngles(q);

    CHECK_CONDITION(Distance(a1, a2) < 2 * EPSILON);

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
        Quaternion q = AngleAxis(PI, Vertex3(0, 1, 0));
        parent->SetOrientation(q);
    }

    auto node = std::make_shared<Node>();
    Vertex3 pos = Vertex3(2, 1, 3);
    Vertex3 scale = Vertex3(3, 4, 5);
    Quaternion q = AngleAxis(PI, Vertex3(0, 1, 0));
    node->SetPosition(pos);
    node->SetScale(scale);
    node->SetOrientation(q);

    Matrix4 m = node->GetGlobalModelMatrix();

    node->SetParent(parent);

    Matrix4 parentinv = parent->GetGlobalModelInvMatrix();
    Quaternion parent_q;
    Vector3 parent_pos;
    Vector3 parent_scale;
    DecomposeMatrix(parentinv, parent_pos, parent_q, parent_scale);

    {
        node->SetPosition(parent_pos + parent_q * (parent_scale * pos));
        node->SetOrientation(parent_q * q);
        node->SetScale(parent_scale * scale);

		CHECK_CONDITION(Distance(node->GetGlobalPosition(), pos) < 0.0001f);
		Vertex3 a1 = node->GetGlobalOrientation() * VECTOR3_LOOKAT_DIRECTION;
		Vertex3 a2 = q * VECTOR3_LOOKAT_DIRECTION;
		CHECK_CONDITION(Distance(a1, a2) < 0.01f);
		CHECK_CONDITION(Distance(node->GetGlobalScale(), scale) < 0.0001f);
    }
    
    {
        Matrix4 m1 = parentinv * m;
        Quaternion q1;
        Vector3 pos1;
        Vector3 scale1;
        DecomposeMatrix(m1, pos1, q1, scale1);
        node->SetPosition(pos1);
        node->SetOrientation(q1);
        node->SetScale(scale1);

        CHECK_CONDITION(node->GetGlobalPosition() == pos);
		Vertex3 a1 = node->GetGlobalOrientation() * VECTOR3_LOOKAT_DIRECTION;
		Vertex3 a2 = q * VECTOR3_LOOKAT_DIRECTION;
		CHECK_CONDITION(Distance(a1, a2) < 0.01f);
        CHECK_CONDITION(Distance(node->GetGlobalScale(), scale) < 0.01f);
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

    CHECK_CONDITION(Distance(Vector3(1.0f, 0.0f, 0.0f), dirPositiveX.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Distance(Vector3(0.0f, -1.0f, 0.0f), dirPositiveX.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Distance(Vector3(-1.0f, 0.0f, 0.0f), dirNegativeX.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Distance(Vector3(0.0f, -1.0f, 0.0f), dirNegativeX.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Distance(Vector3(0.0f, 1.0f, 0.0f), dirPositiveY.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Distance(Vector3(0.0f, 0.0f, -1.0f), dirPositiveY.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Distance(Vector3(0.0f, -1.0f, 0.0f), dirNegativeY.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Distance(Vector3(0.0f, 0.0f, 1.0f), dirNegativeY.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Distance(Vector3(0.0f, 0.0f, 1.0f), dirPositiveZ.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Distance(Vector3(0.0f, -1.0f, 0.0f), dirPositiveZ.GetUpDirection()) < 0.001f);

    CHECK_CONDITION(Distance(Vector3(0.0f, 0.0f, -1.0f), dirNegativeZ.GetLookAtDirection()) < 0.001f);
    CHECK_CONDITION(Distance(Vector3(0.0f, -1.0f, 0.0f), dirNegativeZ.GetUpDirection()) < 0.001f);

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