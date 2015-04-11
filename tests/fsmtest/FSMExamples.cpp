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
using namespace NSG::FSM;

namespace FSMExample0 
{
    struct BaseState : State 
    {
        const char* name_;

        BaseState(const char* name) : name_(name) 
        {
        }

        void Begin() 
        {
            printf("Begin %s\n", name_);
        }

        void Stay() 
        {
            printf("Stay %s\n", name_);
        }

        void End() 
        {
            printf("End %s\n", name_);
        }
    };

    struct Idle : BaseState 
    {
        Idle() : BaseState("Idle") {}
    };

    struct Walk : BaseState 
    {
        Walk() : BaseState("Walk") {}
    };

    struct Run : BaseState 
    {
        Run() : BaseState("Run") {}
    };

    struct Jump : BaseState 
    {
        Jump() : BaseState("Jump") {}
    };

    struct Land : BaseState 
    {
        Land() : BaseState("Land") {}
    };

    class Entity {
    public:
        Entity() : fsm_(idle_), velocity_(0.0), enableJump_(false) 
        {

            idle_.AddTransition(walk_).When([this]() { return velocity_ > 0.1; });
            idle_.AddTransition(jump_).When([this]() { return enableJump_; });

            walk_.AddTransition(run_).When([this]() { return velocity_ > 2; });
            walk_.AddTransition(idle_).When([this]() { return velocity_ < 0.1; });
            walk_.AddTransition(jump_).When([this]() { return enableJump_; });

            run_.AddTransition(walk_).When([this]() { return velocity_ < 2; });
            run_.AddTransition(jump_).When([this]() { return enableJump_; });

            jump_.AddTransition(land_).When([this]() { return !enableJump_; });

            land_.AddTransition(idle_);
        }

        const State* GetState() const 
        {
            return fsm_.GetState();
        }

        void Update(double velocity, bool jump) 
        {
            velocity_ = velocity;
            enableJump_ = jump;
            fsm_.Update();
            fsm_.Update(); // give a chance to process end state
        }

        Idle idle_;
        Walk walk_;
        Run run_;
        Jump jump_;
        Land land_;

    private:
        Machine fsm_;
        double velocity_;
        bool enableJump_;
    };

    void Run() 
    {
        Entity obj;
        CHECK_CONDITION(obj.GetState() == &obj.idle_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(1.9, false);
        CHECK_CONDITION(obj.GetState() == &obj.walk_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(2.5, false);
        CHECK_CONDITION(obj.GetState() == &obj.run_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(3.5, false);
        CHECK_CONDITION(obj.GetState() == &obj.run_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(5.0, false);
        CHECK_CONDITION(obj.GetState() == &obj.run_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(1.9, false);
        CHECK_CONDITION(obj.GetState() == &obj.walk_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(1.9, true);
        CHECK_CONDITION(obj.GetState() == &obj.jump_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(1.9, false);
        CHECK_CONDITION(obj.GetState() == &obj.land_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(1.9, false);
        CHECK_CONDITION(obj.GetState() == &obj.idle_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(1.9, false);
        CHECK_CONDITION(obj.GetState() == &obj.walk_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(0.9, false);
        CHECK_CONDITION(obj.GetState() == &obj.walk_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(0.3, false);
        CHECK_CONDITION(obj.GetState() == &obj.walk_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(0.0, false);
        CHECK_CONDITION(obj.GetState() == &obj.idle_, __FILE__, __LINE__);
        printf("\n");
    }
}

namespace FSMExample1 
{

    struct BaseState : State 
    {
        const char* name_;

        BaseState(const char* name) : name_(name) 
        {
        }

        void Begin() 
        {
            printf("Begin %s\n", name_);
        }

        void Stay() 
        {
            printf("Stay %s\n", name_);
        }

        void End() 
        {
            printf("End %s\n", name_);
        }
    };

    struct BaseMachine : Machine 
    {
        const char* name_;

        BaseMachine(State& initialState, const char* name) : Machine(initialState, false), name_(name) 
        {
        }

        void Begin() 
        {
            printf("Begin %s\n", name_);
        }

        void Stay() 
        {
            printf("Stay %s\n", name_);
        }

        void End() 
        {
            printf("End %s\n", name_);
        }
    };


    struct A : BaseState 
    {
        A() : BaseState("A") {}
    };

    struct B11 : BaseState 
    {
        B11() : BaseState("B11") {}
    };

    struct B12 : BaseState 
    {
        B12() : BaseState("B12") {}
    };

    struct B1 : BaseMachine 
    {
        B1(B11& initialState) : BaseMachine(initialState, "B1") {}
    };

    struct B2 : BaseState 
    {
        B2() : BaseState("B2") {}
    };

    struct B : BaseMachine 
    {
        B(B1& initialState) : BaseMachine(initialState, "B") {}
    };

    struct C1 : BaseState 
    {
        C1() : BaseState("C1") {}
    };

    struct C2 : BaseState 
    {
        C2() : BaseState("C2") {}
    };

    struct C3 : BaseState 
    {
        C3() : BaseState("C3") {}
    };

    struct C : BaseMachine 
    {
        C(C1& initialState) : BaseMachine(initialState, "C") {}
    };


    class Entity 
    {
    public:
        Entity() : b1_(b11_), b_(b1_), c_(c1_), fsm_(a_), t1_(false), t2_(false), t3_(false) 
        {
            a_.AddTransition(b_).When([this]() { return t1_; });
            b_.AddTransition(c_).When([this]() { return t2_; });
            b_.AddTransition(a_).When([this]() { return !t1_ && !t2_; });
            c_.AddTransition(a_).When([this]() { return t1_ && !t2_; });
            b1_.AddTransition(b2_).When([this]() { return t3_; });
            c1_.AddTransition(c2_);
            c2_.AddTransition(c3_).When([this]() { return t3_; });
            c3_.AddTransition(c1_);
            b11_.AddTransition(b12_);
            b12_.AddTransition(b11_);
        }

        const State* GetState() const 
        {
            return fsm_.GetState();
        }

        void Update(bool t1, bool t2, bool t3) 
        {
            t1_ = t1;
            t2_ = t2;
            t3_ = t3;
            fsm_.Update();
            fsm_.Update();
        }

        A a_;
        B11 b11_;
        B12 b12_;
        B1 b1_;
        B2 b2_;
        B b_;
        C1 c1_;
        C2 c2_;
        C3 c3_;
        C c_;

    private:
        Machine fsm_;
        bool t1_;
        bool t2_;
        bool t3_;
    };

    void Run() 
    {
        Entity obj;
        CHECK_CONDITION(obj.GetState() == &obj.a_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(false, false, false);
        CHECK_CONDITION(obj.GetState() == &obj.a_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, false, false);
        CHECK_CONDITION(obj.GetState() == &obj.b_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.b1_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState()->GetState() == &obj.b11_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, false, false);
        CHECK_CONDITION(obj.GetState() == &obj.b_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.b1_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState()->GetState() == &obj.b12_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, false, false);
        CHECK_CONDITION(obj.GetState() == &obj.b_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.b1_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState()->GetState() == &obj.b11_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, false, false);
        CHECK_CONDITION(obj.GetState() == &obj.b_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.b1_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState()->GetState() == &obj.b12_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(false, false, false);
        CHECK_CONDITION(obj.GetState() == &obj.a_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, true, false);
        CHECK_CONDITION(obj.GetState() == &obj.b_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.b1_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState()->GetState() == &obj.b11_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, true, false);
        CHECK_CONDITION(obj.GetState() == &obj.c_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.c1_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, true, false);
        CHECK_CONDITION(obj.GetState() == &obj.c_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.c2_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, true, false);
        CHECK_CONDITION(obj.GetState() == &obj.c_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.c2_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, true, true);
        CHECK_CONDITION(obj.GetState() == &obj.c_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.c3_, __FILE__, __LINE__);
        printf("\n");
        
        obj.Update(true, true, true);
        CHECK_CONDITION(obj.GetState() == &obj.c_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.c1_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, true, true);
        CHECK_CONDITION(obj.GetState() == &obj.c_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.c2_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, true, true);
        CHECK_CONDITION(obj.GetState() == &obj.c_, __FILE__, __LINE__);
        CHECK_CONDITION(obj.GetState()->GetState() == &obj.c3_, __FILE__, __LINE__);
        printf("\n");

        obj.Update(true, false, true);
        CHECK_CONDITION(obj.GetState() == &obj.a_, __FILE__, __LINE__);
        printf("\n");
    }
}

void FSMExamples() 
{
    FSMExample0::Run();
    printf("-------------------------------------------\n");
    FSMExample1::Run();
    printf("-------------------------------------------\n");
}