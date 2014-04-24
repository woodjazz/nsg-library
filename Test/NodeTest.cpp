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
#include "NSG/Node.h"
#include "NSG/Log.h"
#include "NSG/Constants.h"
#include "NSG/Util.h"
#undef NDEBUG
#include <assert.h>

using namespace NSG;

static void Test01()
{
	PNode pA(new Node());
	PNode pB(new Node());
    pB->SetParent(pA);
	pA->SetPosition(Vertex3(1,0,0));
	pB->SetPosition(Vertex3(-2,0,0));
	
	assert(pA->GetPosition() == Vertex3(1,0,0));
	assert(pA->GetGlobalPosition() == Vertex3(1,0,0));
	assert(pB->GetPosition() == Vertex3(-2,0,0));
	assert(pB->GetGlobalPosition() == Vertex3(-1,0,0));

    pA->SetPosition(Vertex3(2,0,0));

	assert(pA->GetPosition() == Vertex3(2,0,0));
	assert(pA->GetGlobalPosition() == Vertex3(2,0,0));
	assert(pB->GetPosition() == Vertex3(-2,0,0));
	assert(pB->GetGlobalPosition() == Vertex3(0,0,0));

    pA->SetPosition(Vertex3(1,0,0));

	assert(pA->GetPosition() == Vertex3(1,0,0));
	assert(pA->GetGlobalPosition() == Vertex3(1,0,0));
	assert(pB->GetPosition() == Vertex3(-2,0,0));
	assert(pB->GetGlobalPosition() == Vertex3(-1,0,0));
	
	pB->SetScale(Vertex3(2,2,2));

   	assert(pA->GetPosition() == Vertex3(1,0,0));
	assert(pA->GetGlobalPosition() == Vertex3(1,0,0));
	assert(pB->GetPosition() == Vertex3(-2,0,0));
	assert(pB->GetGlobalPosition() == Vertex3(-1,0,0));

    pA->SetScale(Vertex3(2,2,2));

	assert(pA->GetPosition() == Vertex3(1,0,0));
	assert(pA->GetGlobalPosition() == Vertex3(1,0,0));
	assert(pB->GetPosition() == Vertex3(-2,0,0));
	assert(pB->GetGlobalPosition() == Vertex3(-3,0,0));
}

static void Test02()
{
	PNode pA(new Node());
	PNode pB(new Node());
    pB->SetParent(pA);
	pA->SetPosition(Vertex3(1,0,0));
	pB->SetGlobalPosition(Vertex3(-2,0,0));

    assert(pB->GetPosition() == Vertex3(-3,0,0));
    assert(pB->GetGlobalPosition() == Vertex3(-2,0,0));
}

static void Test03()
{
	PNode pA(new Node());

    assert(pA->GetLookAtDirection() == Vertex3(0,0,-1));

    pA->SetLookAt(Vertex3(10,0,0));

    assert(glm::distance(pA->GetLookAtDirection(), Vertex3(1,0,0)) < 2*glm::epsilon<float>());
}

static void Test04()
{
    Vertex3 v1(1,0,0);

	PNode pA(new Node());
    pA->SetPosition(Vertex3(2,0,0));
    pA->SetOrientation(glm::angleAxis(PI, Vertex3(0, 0, 1)));
    Vertex3 v2(pA->GetGlobalModelMatrix() * Vertex4(v1,1));

    assert(glm::distance(v2, Vertex3(1,0,0)) < 2*glm::epsilon<float>());
    
    pA->SetScale(Vertex3(2,2,2));

    v2 = Vertex3(pA->GetGlobalModelMatrix() * Vertex4(v1,1));

    assert(glm::distance(v2, Vertex3(0,0,0)) < 2*glm::epsilon<float>());
}

static void Test05()
{
    Vertex3 v1(0,0,0);

	PNode pA(new Node());
    pA->SetPosition(Vertex3(2,0,0));

    PNode pB(new Node());
    pB->SetParent(pA);
    pB->SetPosition(Vertex3(1,0,0));
    pB->SetOrientation(glm::angleAxis(PI, Vertex3(0, 0, 1)));

    Vertex3 v2(pB->GetGlobalModelMatrix() * Vertex4(v1,1));

    assert(glm::distance(v2, Vertex3(3,0,0)) < 2*glm::epsilon<float>());
    
    pA->SetScale(Vertex3(2,2,2));

    v2 = Vertex3(pB->GetGlobalModelMatrix() * Vertex4(v1,1));

    assert(glm::distance(v2, Vertex3(4,0,0)) < 2*glm::epsilon<float>());
}

static void Test06()
{
    Vertex3 v1(0,0,0);

	PNode pA(new Node());
    pA->SetPosition(Vertex3(0,0,0));

    PNode pB(new Node());
    pB->SetParent(pA);
    pB->SetPosition(Vertex3(-1,0,0));
    pB->SetScale(Vertex3(0.5f, 1, 1));

    Vertex3 v2(pB->GetGlobalModelMatrix() * Vertex4(v1,1));

    assert(glm::distance(v2, Vertex3(-1,0,0)) < 2*glm::epsilon<float>());
}

static void Test07()
{
	PNode pA(new Node());
    Vertex3 position = Vertex3(2,1,3);
    pA->SetPosition(position);
    Vertex3 scale = Vertex3(3,4,5);
    pA->SetScale(scale);
    Quaternion q = glm::angleAxis(PI, Vertex3(0, 1, 0));
    pA->SetOrientation(q);

    Vertex3 position1;
    Quaternion q1;
    Vertex3 scale1;

    Matrix4 m = pA->GetGlobalModelMatrix();

    DecomposeMatrix(m, position1, q1, scale1);

    Vertex3 a1 = glm::eulerAngles(q1);
    Vertex3 a2 = glm::eulerAngles(q);

    assert(glm::distance(a1, a2) < 2*glm::epsilon<float>());

    assert(position == position1);

    assert(scale == scale1);
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
}