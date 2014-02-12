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
#include "QueuedTask.h"
#include <assert.h>

namespace NSG 
{
    namespace Task 
    {
        QueuedTask::Data::Data(int id, PTask pTask) 
            : id_(id), pTask_(pTask), canceled_(false) 
        {
        }

        QueuedTask::QueuedTask() : taskAlive_(true) 
        {
            thread_ = Thread([this](){InternalTask();});
        }

        QueuedTask::~QueuedTask() 
        {
		    taskAlive_ = false;
            condition_.notify_one();
            thread_.join();
        }

        QueuedTask::PData QueuedTask::Pop() 
        {
            std::unique_lock<Mutex> lck(mtx_);
            while(queue_.empty() && taskAlive_) 
            {
                condition_.wait(lck);
            }
            if(!queue_.empty()) 
            {
                PData pData(queue_.front());
                queue_.pop_front();
                return pData;
            }
            return nullptr;
        }

        bool QueuedTask::IsEmpty() const 
        {
            std::lock_guard<Mutex> guard(mtx_);
            return queue_.empty();
        }
       

        void QueuedTask::InternalTask() 
        {
            while(taskAlive_ || !IsEmpty()) 
            {
                PData pData = Pop();
                
                if(nullptr == pData || pData->canceled_)
                    continue;
                
                try 
                {
                    pData->pTask_->Run();
                } 
                catch(std::exception& e) 
                {
                    pData->pTask_->Exception(e);
                }
            }
        }

        int QueuedTask::AddTask(PTask pTask) 
        {
            std::lock_guard<Mutex> guard(mtx_);
            static int s_id(0);
            int id = ++s_id;
            PData pData(new Data(id, pTask));
            queue_.push_back(pData);
            keyDataMap_.insert(MAP_ID_DATA::value_type(id, pData));
            condition_.notify_one();
            return id;
        }

	    bool QueuedTask::CancelTask(int id) 
        {
		    std::lock_guard<Mutex> guard(mtx_);
            auto it = keyDataMap_.find(id);
            if(it != keyDataMap_.end()) 
            {
                it->second->canceled_ = true;
                keyDataMap_.erase(it);
                return true;
            }
            return false;
	    }

        void QueuedTask::CancelAllTasks() 
        {
            std::lock_guard<Mutex> guard(mtx_);
            for(auto& obj : queue_) 
            {
                obj->canceled_ = true;
            }
        }
    }
}