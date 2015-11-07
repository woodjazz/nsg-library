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
#include "MemoryManager.h"
#include "Log.h"
#ifdef USE_POOLS
#include "Pool.h"
#include "Check.h"
#include <thread>
#include <cstdlib>
#include <vector>
#include <atomic>
#include <new>

namespace NSG
{
    void AtExit();
    struct PoolData
    {
        size_t objSize_;
        IPool* pool_;
        PoolData() : objSize_(0), pool_(nullptr) {}
        PoolData(IPool& pool) : objSize_(pool.GetObjSize()), pool_(&pool) {}
    };
    struct Pools;
    static Pools* poolsObj = nullptr;

    void* AllocateMemoryFromHeap(std::size_t count)
    {
        size_t newSize = sizeof(MemHeader) + count;
        //LOGI("Allocating %u from heap.", (unsigned)newSize);
        void* p = std::malloc(newSize);
        if (p)
        {
            MemHeader* header = (MemHeader*)p;
			header->poolPointer_ = &AllocateMemoryFromHeap;
            void* memBlock = (char*)p + sizeof(MemHeader);
            return memBlock;
        }
        LOGE("Allocation of %lu from heap has FAILED.(bad_alloc)", newSize);
        return nullptr;
    }

    void ReleaseMemoryFromHeap(void* ptr)
    {
        void* memObj = (char*)ptr - sizeof(MemHeader);
        MemHeader* header = (MemHeader*)memObj;
		if (header->poolPointer_ == &AllocateMemoryFromHeap)
        {
            //LOGI("Releasing memory to heap");
            std::free(memObj);
        }
    }

    struct Pools
    {
        friend void AtExit();
        static const size_t MinPoolSize = 1;
        Pool < MinPoolSize << 0, 1000 > pool1_;
        Pool < MinPoolSize << 1, 1000 > pool2_;
        Pool < MinPoolSize << 2, 1000 > pool4_;
        Pool < MinPoolSize << 3, 50000 > pool8_;
        Pool < MinPoolSize << 4, 50000 > pool16_;
        Pool < MinPoolSize << 5, 50000 > pool32_;
        Pool < MinPoolSize << 6, 90000 > pool64_;
        Pool < MinPoolSize << 7, 90000 > pool128_;
        Pool < MinPoolSize << 8, 20000 > pool256_;
        Pool < MinPoolSize << 9, 20000 > pool512_;
        Pool < MinPoolSize << 10, 1000 > pool1024_;
        Pool < MinPoolSize << 11, 1000 > pool2048_;
        Pool < MinPoolSize << 12, 100 > pool4096_;
        Pool < MinPoolSize << 13, 100 > pool8192_;
        Pool < MinPoolSize << 14, 50 > pool16384_;
        Pool < MinPoolSize << 15, 2 > pool32768_;
		static const size_t MaxPoolSize = MinPoolSize << 15;
        static const size_t MaxPools = 16;
        PoolData* pools_;
        size_t nPools_;
        void* begin_;
        void* end_;
        Pools()
        {
            begin_ = this;
            end_ = (char*)this + sizeof(Pools);

            PoolData pools[] = { pool1_, pool2_, pool4_, pool8_, pool16_, pool32_, pool64_, pool128_, pool256_, pool512_, pool1024_, pool2048_, pool4096_, pool8192_, pool16384_, pool32768_ };
            static_assert(MaxPools == sizeof(pools) / sizeof(PoolData), "Number of pools is incorrect");
            static char memPoolData[sizeof(PoolData) * MaxPools] = { 0 };
			pools_ = new(memPoolData)PoolData;
            int i = 0;
            for (auto& obj : pools)
                pools_[i++] = std::move(obj);
            nPools_ = MaxPools;
            std::sort(&pools_[0], &pools_[MaxPools], [](const PoolData & a, const PoolData & b) { return a.objSize_ < b.objSize_; });
            poolsObj = this;
            //#if !defined(IOS) && !defined(ANDROID) && !defined(IS_TARGET_LINUX)
            //std::atexit(AtExit);
            //#endif
        }

        ~Pools()
		{
			poolsObj = nullptr;
			delete[] pools_;
		}

        IPool* GetBestPool(std::size_t count)
        {
            assert(count >= 0);
            #if 0
            size_t slot = (size_t)std::ceil(std::log2(count));
            if (slot < nPools_)
                return pools_[slot].pool_;
            return nullptr;
            #else
			if (count <= Pools::MaxPoolSize)
				for (size_t i = 0; i < nPools_; i++)
					if (pools_[i].objSize_ > count)
						return pools_[i].pool_;
            return nullptr;
            #endif
        }

        inline bool IsPool(void* p)
        {
            return p >= begin_ && p < end_;
        }
    };

    static void* AllocateMemory(std::size_t count)
    {
#if 0
		static struct Initializer
		{
			char memPools_[sizeof(Pools)];
			Pools* p_;
			Initializer()
			{
				p_ = new(&memPools_[0])Pools;
			}
			~Initializer()
			{
				p_->~Pools();
			}
		} initializer;
#endif
		if (poolsObj)
		{
			IPool* pool = poolsObj->GetBestPool(count);
			if (pool)
			{
				void* p = pool->Allocate(count);
				if (p) return p;
			}
		}
        return AllocateMemoryFromHeap(count);
    }

    static void ReleaseMemory(void* ptr, std::size_t count)
    {
        if (ptr)
        {
			if (poolsObj)
            {
                void* memObj = (char*)ptr - sizeof(MemHeader);
                MemHeader* header = (MemHeader*)memObj;
				if (header->poolPointer_ != &AllocateMemoryFromHeap)
                {
                    IPool* pool = (IPool*)header->poolPointer_;
					if (poolsObj->IsPool((void*)pool))
					{
						pool->DeAllocate(ptr);
						return;
					}
                }
            }
			ReleaseMemoryFromHeap(ptr);
        }
    }

	static Pools* memoryPools = nullptr;
	void InitilizeMemoryManager()
	{
		static char memPools[sizeof(Pools)];
		memoryPools = new(&memPools[0])Pools;
	}

	void DestroyMemoryManager()
	{
#if (defined(DEBUG) || defined (_DEBUG)) && !defined(NDEBUG)
		memoryPools->~Pools();
#endif
	}
}

using namespace NSG;

void* operator new(std::size_t count)
{
    return AllocateMemory(count);
}

#if _MSC_VER
void operator delete(void* ptr)
#else
void operator delete(void* ptr) noexcept
#endif
{
    ReleaseMemory(ptr, 0);
}

#if _MSC_VER
void operator delete(void* ptr, std::size_t count)
#else
void operator delete(void* ptr, std::size_t count) noexcept
#endif
{
    ReleaseMemory(ptr, count);
}

void* operator new[](size_t count)
{
	return AllocateMemory(count);
}

#if _MSC_VER
void operator delete[](void* ptr)
#else
void operator delete[](void* ptr) noexcept
#endif
{
    ReleaseMemory(ptr, 0);
}
#else
//void AtExit()
//{
//	LOGI("*** AtExit called ***");
//}

namespace NSG
{
	void InitilizeMemoryManager()
	{
	}

	void DestroyMemoryManager()
	{
	}
}
#endif
