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
#pragma once
#include "Log.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <atomic>
#include <new>

namespace NSG
{
    struct IPool
    {
        virtual void* Allocate(std::size_t count) = 0;
        virtual void DeAllocate(void* ptr) = 0;
        virtual ~IPool() {};
        virtual size_t GetObjSize() const = 0;
        virtual unsigned GetAllocatedObjects() const = 0;
        virtual bool PointerInPool(void* p) const = 0;
    };

    struct MemHeader
    {
        void* poolPointer_;
    };

    /// A lock-free thread safe pool that does not suffer the ABA problem
    template <size_t OBJECT_SIZE, size_t OBJECTS_PER_CHUNK >
    class Pool : public IPool
    {
    public:
        struct MemObj
        {
            MemHeader header_;
            union
            {
                MemObj* nextMemObj_;
                char memBlock_[OBJECT_SIZE];
            };
        };

        static const size_t ChunkSize = sizeof(MemObj)* OBJECTS_PER_CHUNK;

        Pool();
        ~Pool();
        Pool(const Pool&) = delete;
        Pool& operator = (const Pool&) = delete;
        void* Allocate(std::size_t count) override;
        void DeAllocate(void* ptr) override;
        unsigned GetAllocatedObjects() const override { return allocatedObjs_; }
        size_t GetObjSize() const override { return OBJECT_SIZE; }
        bool PointerInPool(void* p) const override { return p >= begin_ && p < end_; }
        void LogStatus();
    private:
        typedef MemObj* PMemObj;
        std::atomic<PMemObj> freeList_;
        void* begin_;
        void* end_;
        std::atomic<unsigned> allocatedObjs_;
    };

    template< size_t OBJECT_SIZE, size_t OBJECTS_PER_CHUNK>
    Pool<OBJECT_SIZE, OBJECTS_PER_CHUNK>::Pool()
        : freeList_(nullptr),
          allocatedObjs_(0)
    {
        TRACE_PRINTF("Creating pool: object size=%u objs/chunk=%u\n", OBJECT_SIZE, OBJECTS_PER_CHUNK);
        void* p = std::malloc(ChunkSize);
        if (!p)
            throw std::bad_alloc();
        freeList_ = (PMemObj)p;
        begin_ = p;
        end_ = (char*)begin_ + ChunkSize;

        // Constructs the empty list.
        PMemObj next = freeList_;
        PMemObj previous;
        for (size_t i = 0; i < OBJECTS_PER_CHUNK; i++)
        {
            previous = next;
            next = (PMemObj)((char*)next + sizeof(MemObj));
            previous->header_.poolPointer_ = this;
            previous->nextMemObj_ = next;
        }
        previous->nextMemObj_ = nullptr;
    }

    template< size_t OBJECT_SIZE, size_t OBJECTS_PER_CHUNK>
    Pool<OBJECT_SIZE, OBJECTS_PER_CHUNK>::~Pool()
    {
        TRACE_PRINTF("Destroying pool: object size=%u objs/chunk=%u\n", OBJECT_SIZE, OBJECTS_PER_CHUNK);
        std::free(begin_);
    }

    template< size_t OBJECT_SIZE, size_t OBJECTS_PER_CHUNK>
    void Pool<OBJECT_SIZE, OBJECTS_PER_CHUNK>::LogStatus()
    {
        TRACE_PRINTF("Pool Status: object size=%u  objs/chunk=%d still allocated objs=%u\n", OBJECT_SIZE, OBJECTS_PER_CHUNK, allocatedObjs_);
    }

    template< size_t OBJECT_SIZE, size_t OBJECTS_PER_CHUNK>
    void* Pool<OBJECT_SIZE, OBJECTS_PER_CHUNK>::Allocate(std::size_t count)
    {
        assert(count <= OBJECT_SIZE);
        PMemObj p;
        do
        {
            p = freeList_;
            if (!p) return nullptr;
        }
        while (!std::atomic_compare_exchange_weak(&freeList_, &p, p->nextMemObj_)); 
        ++allocatedObjs_;
        return (void*)(p->memBlock_);
    }

    template< size_t OBJECT_SIZE, size_t OBJECTS_PER_CHUNK>
    void Pool<OBJECT_SIZE, OBJECTS_PER_CHUNK>::DeAllocate(void* obj)
    {
        if (obj && obj >= begin_ && obj < end_)
        {
            obj = (char*)obj - sizeof(MemHeader);
            PMemObj p((PMemObj)obj);
            do
            {
                p->nextMemObj_ = freeList_;
            }
            while (!std::atomic_compare_exchange_weak(&freeList_, &(p->nextMemObj_), p));
            --allocatedObjs_;
        }
    }
}
