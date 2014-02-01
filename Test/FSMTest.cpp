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
#include "FSM.h"
#include "Log.h"
#undef NDEBUG
#include <assert.h>

using namespace NSG::FSM;

static void FSMTest0()
{
    struct INITIALIZING : public State {};
    struct BUSY : public State {};
    struct ERROR : public State {};
    struct READY : public State {};

    INITIALIZING initializing;
    BUSY busy;
    ERROR error;
    READY ready;

    bool condition1 = false;
    bool condition2 = false;
    bool condition3 = false;

    initializing.AddTransition(busy).When([&]() { return condition1; });
    busy.AddTransition(ready).When([&]() { return condition2; });
    ready.AddTransition(error).When([&]() { return condition3; });
    error.AddTransition(initializing).When([&]() { return !condition3; });

    Machine machine(initializing);

    assert(machine.GetState() == &initializing);

    machine.Update();

    assert(machine.GetState() == &initializing);

    condition1 = true;

    machine.Update();

    assert(machine.GetState() == &busy);

    condition2 = true;

    machine.Update();

    assert(machine.GetState() == &ready);

    machine.Update();

    assert(machine.GetState() == &ready);

    condition3 = true;

    machine.Update();

    assert(machine.GetState() == &error);

    condition3 = false;

    machine.Update();

    assert(machine.GetState() == &initializing);

    machine.Update();

    assert(machine.GetState() == &busy);

    machine.Update();

    assert(machine.GetState() == &ready);

    machine.Update();

    assert(machine.GetState() == &ready);
}

static void FSMTest1()
{
    struct CounterState : public State 
    {

        CounterState() : nStart_(0), nStay_(0), nEnd_(0) 
        {
        }

        void Begin() 
        {
            ++nStart_;
        }

        void Stay() 
        {
            ++nStay_;
        }

        void End() 
        {
            ++nEnd_;
        }

        int nStart_;
        int nStay_;
        int nEnd_;
    };

    struct INITIALIZING : public CounterState {};
    struct BUSY : public CounterState {};
    struct ERROR : public CounterState {};
    struct READY : public CounterState {};

    INITIALIZING initializing;
    BUSY busy;
    ERROR error;
    READY ready;

    bool condition1 = false;
    bool condition2 = false;
    bool condition3 = false;

    initializing.AddTransition(busy).When([&]() { return condition1; });
    busy.AddTransition(ready).When([&]() { return condition2; });
    ready.AddTransition(error).When([&]() { return condition3; });
    error.AddTransition(initializing).When([&]() { return !condition3; });

    Machine machine(initializing);

    assert(machine.GetState() == &initializing);

    assert(initializing.nStart_ == 1 && initializing.nStay_ == 0 && initializing.nEnd_ == 0);

    machine.Update();

    assert(initializing.nStart_ == 1 && initializing.nStay_ == 1 && initializing.nEnd_ == 0);

    assert(machine.GetState() == &initializing);

    condition1 = true;

    machine.Update();

    assert(initializing.nStart_ == 1 && initializing.nStay_ == 1 && initializing.nEnd_ == 1);

    assert(machine.GetState() == &busy);

    assert(busy.nStart_ == 1 && busy.nStay_ == 0 && busy.nEnd_ == 0);

    condition2 = true;

    machine.Update();

    assert(busy.nStart_ == 1 && busy.nStay_ == 0 && busy.nEnd_ == 1);

    assert(machine.GetState() == &ready);

    assert(ready.nStart_ == 1 && ready.nStay_ == 0 && ready.nEnd_ == 0);

    machine.Update();

    assert(ready.nStart_ == 1 && ready.nStay_ == 1 && ready.nEnd_ == 0);

    assert(machine.GetState() == &ready);

    condition3 = true;

    machine.Update();

    assert(ready.nStart_ == 1 && ready.nStay_ == 1 && ready.nEnd_ == 1);

    assert(machine.GetState() == &error);

    assert(error.nStart_ == 1 && error.nStay_ == 0 && error.nEnd_ == 0);

    condition3 = false;

    machine.Update();

    assert(error.nStart_ == 1 && error.nStay_ == 0 && error.nEnd_ == 1);

    assert(machine.GetState() == &initializing);

    assert(initializing.nStart_ == 2 && initializing.nStay_ == 1 && initializing.nEnd_ == 1);

    machine.Update();

    assert(initializing.nStart_ == 2 && initializing.nStay_ == 1 && initializing.nEnd_ == 2);

    assert(machine.GetState() == &busy);

    assert(busy.nStart_ == 2 && busy.nStay_ == 0 && busy.nEnd_ == 1);

    machine.Update();

    assert(busy.nStart_ == 2 && busy.nStay_ == 0 && busy.nEnd_ == 2);

    assert(machine.GetState() == &ready);

    assert(ready.nStart_ == 2 && ready.nStay_ == 1 && ready.nEnd_ == 1);

    machine.Update();

    assert(machine.GetState() == &ready);

    assert(ready.nStart_ == 2 && ready.nStay_ == 2 && ready.nEnd_ == 1);
}

static void FSMTest2()
{
    struct INITIALIZING : public State {};

    struct INITIALIZE_JOBS : public State {};
    struct DO_JOBS : public State {};

    struct BUSY : public Machine {
        BUSY(State& subState) : Machine(subState) {}
    };

    struct READY : public State {};
    struct ERROR : public State {};

    INITIALIZING initializing;
    INITIALIZE_JOBS initialize_jobs;
    DO_JOBS do_jobs;
    BUSY busy(initialize_jobs);
    READY ready;

    bool condition1 = false;
    bool condition2 = false;
    bool condition3 = false;

    initializing.AddTransition(busy).When([&]() { return condition1; });
    initialize_jobs.AddTransition(do_jobs);

    busy.AddTransition(ready).When([&]() { return condition2; });
    ready.AddTransition(busy).When([&]() { return condition3; });

    Machine machine(initializing);

    assert(machine.GetState() == &initializing);

    machine.Update();

    assert(machine.GetState() == &initializing);

    condition1 = true;

    machine.Update();

    assert(machine.GetState() == &busy);
    assert(machine.GetState()->GetState() == &initialize_jobs);

    condition2 = true;

    machine.Update();

    assert(machine.GetState() == &ready);

    machine.Update();

    assert(machine.GetState() == &ready);

    condition3 = true;
    condition2 = false;

    machine.Update();

    assert(machine.GetState() == &busy);
    assert(machine.GetState()->GetState() == &initialize_jobs);

    machine.Update();

    assert(machine.GetState() == &busy);
    assert(machine.GetState()->GetState() == &do_jobs);

    machine.Update();

    assert(machine.GetState() == &busy);
    assert(machine.GetState()->GetState() == &do_jobs);

    condition2 = true;

    machine.Update();

    assert(machine.GetState() == &ready);

    machine.Update();

    condition3 = false;

    machine.Update();

    assert(machine.GetState() == &ready);

    machine.Update();

    assert(machine.GetState() == &ready);

    condition3 = true;
    condition2 = false;

    machine.Update();

    assert(machine.GetState() == &busy);
    assert(machine.GetState()->GetState() == &initialize_jobs);

    machine.Update();

    assert(machine.GetState() == &busy);
    assert(machine.GetState()->GetState() == &do_jobs);

    machine.Update();

    assert(machine.GetState() == &busy);
    assert(machine.GetState()->GetState() == &do_jobs);

    condition2 = true;

    machine.Update();

    assert(machine.GetState() == &ready);
}

void FSMTest()
{
    printf("FSMTest0\n");
    FSMTest0();
    printf("FSMTest1\n");
    FSMTest1();
    printf("FSMTest2\n");
    FSMTest2();
}
