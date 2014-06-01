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
#include "NSG.h"
#include <vector>

using namespace NSG::Task;

struct BaseTask : Task 
{
    static std::vector<int> executedTasks_;
};

std::vector<int> BaseTask::executedTasks_;

struct Task0 : BaseTask 
{
    void Run() 
    {
        BaseTask::executedTasks_.push_back(0);
    }
};

struct Task1 : BaseTask 
{
    void Run() 
    {
        BaseTask::executedTasks_.push_back(1);
    }
};

struct Task2 : BaseTask 
{
    void Run() 
    {
        BaseTask::executedTasks_.push_back(2);
        std::this_thread::sleep_for(Milliseconds(200));
    }
};

struct Task3 : BaseTask 
{
    void Run() 
    {
        BaseTask::executedTasks_.push_back(3);
    }
};

struct Task4 : BaseTask 
{
    void Run() 
    {
        BaseTask::executedTasks_.push_back(4);
    }
};


static void QueuedTaskTest0()
{
    {
        QueuedTask tasks;

        Task0* p0(new Task0);
        Task1* p1(new Task1);
        Task2* p2(new Task2);
        Task3* p3(new Task3);

        PTask pTask0(p0);
        PTask pTask1(p1);
        PTask pTask2(p2);
        PTask pTask3(p3);

        tasks.AddTask(pTask0);
        tasks.AddTask(pTask1);
        tasks.AddTask(pTask2);
        tasks.AddTask(pTask0);
        tasks.AddTask(pTask3);
        tasks.AddTask(pTask3);
        tasks.AddTask(pTask1);
    }

    int expectedSequence[] = {0,1,2,0,3,3,1};
    CHECK_ASSERT(BaseTask::executedTasks_.size() == sizeof(expectedSequence)/sizeof(int), __FILE__, __LINE__);
    CHECK_ASSERT(std::equal(BaseTask::executedTasks_.begin(), BaseTask::executedTasks_.end(), expectedSequence), __FILE__, __LINE__);
    BaseTask::executedTasks_.clear();
}

static void QueuedTaskTest1()
{
    {
        QueuedTask tasks;

        Task0* p0(new Task0);
        Task1* p1(new Task1);
        Task2* p2(new Task2);
        Task3* p3(new Task3);
        Task4* p4(new Task4);

        PTask pTask0(p0);
        PTask pTask1(p1);
        PTask pTask2(p2);
        PTask pTask3(p3);
        PTask pTask4(p4);

        tasks.AddTask(pTask0);
        tasks.AddTask(pTask1);
        tasks.AddTask(pTask2);
        tasks.AddTask(pTask0);
        int id4 = tasks.AddTask(pTask4);
        tasks.AddTask(pTask3);
        tasks.AddTask(pTask3);
        tasks.AddTask(pTask1);

        std::this_thread::sleep_for(Milliseconds(100));
        tasks.CancelTask(id4);
    }

    int expectedSequence[] = {0,1,2,0,3,3,1};
    CHECK_ASSERT(BaseTask::executedTasks_.size() == sizeof(expectedSequence)/sizeof(int), __FILE__, __LINE__);
    CHECK_ASSERT(std::equal(BaseTask::executedTasks_.begin(), BaseTask::executedTasks_.end(), expectedSequence), __FILE__, __LINE__);
    BaseTask::executedTasks_.clear();
}

static void QueuedTaskTest2()
{
    {
        QueuedTask tasks;

        Task0* p0(new Task0);
        Task1* p1(new Task1);
        Task2* p2(new Task2);
        Task3* p3(new Task3);
        Task4* p4(new Task4);

        PTask pTask0(p0);
        PTask pTask1(p1);
        PTask pTask2(p2);
        PTask pTask3(p3);
        PTask pTask4(p4);

        tasks.AddTask(pTask2);
        tasks.AddTask(pTask1);
        tasks.AddTask(pTask2);
        tasks.AddTask(pTask0);
        tasks.AddTask(pTask4);
        tasks.AddTask(pTask4);
        tasks.AddTask(pTask4);
        tasks.AddTask(pTask4);
        tasks.AddTask(pTask3);
        tasks.AddTask(pTask3);
        tasks.AddTask(pTask1);

        std::this_thread::sleep_for(Milliseconds(100));
        tasks.CancelAllTasks();
    }

    int expectedSequence[] = {2};
    CHECK_ASSERT(BaseTask::executedTasks_.size() == sizeof(expectedSequence)/sizeof(int), __FILE__, __LINE__);
    CHECK_ASSERT(std::equal(BaseTask::executedTasks_.begin(), BaseTask::executedTasks_.end(), expectedSequence), __FILE__, __LINE__);
    BaseTask::executedTasks_.clear();
}

void QueuedTaskTest()
{
    QueuedTaskTest0();
    QueuedTaskTest1();
    QueuedTaskTest2();
}
