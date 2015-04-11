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

static void FSMTest0()
{
    struct INITIALIZING : public State {};
    struct BUSY : public State {};
    struct ERROR1 : public State {};
    struct READY : public State {};

    INITIALIZING initializing;
    BUSY busy;
    ERROR1 error;
    READY ready;

    bool condition1 = false;
    bool condition2 = false;
    bool condition3 = false;

    initializing.AddTransition(busy).When([&]() { return condition1; });
    busy.AddTransition(ready).When([&]() { return condition2; });
    ready.AddTransition(error).When([&]() { return condition3; });
    error.AddTransition(initializing).When([&]() { return !condition3; });

    Machine machine(initializing);

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    condition1 = true;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);
    
    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    condition2 = true;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    condition3 = true;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &error, __FILE__, __LINE__);

    condition3 = false;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &error, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);
    
    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);
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
    struct ERROR1 : public CounterState {};
    struct READY : public CounterState {};

    INITIALIZING initializing;
    BUSY busy;
    ERROR1 error;
    READY ready;

    bool condition1 = false;
    bool condition2 = false;
    bool condition3 = false;

    initializing.AddTransition(busy).When([&]() { return condition1; });
    busy.AddTransition(ready).When([&]() { return condition2; });
    ready.AddTransition(error).When([&]() { return condition3; });
    error.AddTransition(initializing).When([&]() { return !condition3; });

    Machine machine(initializing);

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    CHECK_CONDITION(initializing.nStart_ == 1 && initializing.nStay_ == 0 && initializing.nEnd_ == 0, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(initializing.nStart_ == 1 && initializing.nStay_ == 1 && initializing.nEnd_ == 0, __FILE__, __LINE__);

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    condition1 = true;

    machine.Update();
    
    CHECK_CONDITION(initializing.nStart_ == 1 && initializing.nStay_ == 1 && initializing.nEnd_ == 1, __FILE__, __LINE__);
  
    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    CHECK_CONDITION(busy.nStart_ == 1 && busy.nStay_ == 0 && busy.nEnd_ == 0, __FILE__, __LINE__);

    condition2 = true;

    machine.Update();

    CHECK_CONDITION(busy.nStart_ == 1 && busy.nStay_ == 0 && busy.nEnd_ == 1, __FILE__, __LINE__);

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    CHECK_CONDITION(ready.nStart_ == 1 && ready.nStay_ == 0 && ready.nEnd_ == 0, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(ready.nStart_ == 1 && ready.nStay_ == 1 && ready.nEnd_ == 0, __FILE__, __LINE__);

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    condition3 = true;

    machine.Update();
    machine.Update();

    CHECK_CONDITION(ready.nStart_ == 1 && ready.nStay_ == 1 && ready.nEnd_ == 1, __FILE__, __LINE__);

    CHECK_CONDITION(machine.GetState() == &error, __FILE__, __LINE__);

    CHECK_CONDITION(error.nStart_ == 1 && error.nStay_ == 0 && error.nEnd_ == 0, __FILE__, __LINE__);

    condition3 = false;

    machine.Update();
    machine.Update();

    CHECK_CONDITION(error.nStart_ == 1 && error.nStay_ == 0 && error.nEnd_ == 1, __FILE__, __LINE__);

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    CHECK_CONDITION(initializing.nStart_ == 2 && initializing.nStay_ == 1 && initializing.nEnd_ == 1, __FILE__, __LINE__);

    machine.Update();
    machine.Update();

    CHECK_CONDITION(initializing.nStart_ == 2 && initializing.nStay_ == 1 && initializing.nEnd_ == 2, __FILE__, __LINE__);

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    CHECK_CONDITION(busy.nStart_ == 2 && busy.nStay_ == 0 && busy.nEnd_ == 1, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(busy.nStart_ == 2 && busy.nStay_ == 0 && busy.nEnd_ == 2, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    CHECK_CONDITION(ready.nStart_ == 2 && ready.nStay_ == 1 && ready.nEnd_ == 1, __FILE__, __LINE__);

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    CHECK_CONDITION(ready.nStart_ == 2 && ready.nStay_ == 2 && ready.nEnd_ == 1, __FILE__, __LINE__);
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
    struct ERROR1 : public State {};

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

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    condition1 = true;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &initializing, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);
    CHECK_CONDITION(machine.GetState()->GetState() == &initialize_jobs, __FILE__, __LINE__);

    condition2 = true;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    condition3 = true;
    condition2 = false;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);
    CHECK_CONDITION(machine.GetState()->GetState() == &initialize_jobs, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);
    CHECK_CONDITION(machine.GetState()->GetState() == &do_jobs, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);
    CHECK_CONDITION(machine.GetState()->GetState() == &do_jobs, __FILE__, __LINE__);

    condition2 = true;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    condition3 = false;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);
   

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    condition3 = true;
    condition2 = false;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);
    CHECK_CONDITION(machine.GetState()->GetState() == &initialize_jobs, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);
    CHECK_CONDITION(machine.GetState()->GetState() == &initialize_jobs, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState()->GetState() == &do_jobs, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);
    CHECK_CONDITION(machine.GetState()->GetState() == &do_jobs, __FILE__, __LINE__);

    condition2 = true;

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &busy, __FILE__, __LINE__);

    machine.Update();

    CHECK_CONDITION(machine.GetState() == &ready, __FILE__, __LINE__);
}

void FSMTest()
{
    FSMTest0();
    FSMTest1();
    FSMTest2();
}
