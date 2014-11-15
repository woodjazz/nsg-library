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
#include "Arena.h"
#include <algorithm>
#include <cassert>

namespace NSG
{
    struct IPool
    {
        virtual void* Allocate(std::size_t count) = 0;
        virtual void Release(void* ptr) = 0;
		virtual bool IsPointerFromPool(void* p) const = 0;
        virtual ~IPool() {};
    };

    template <typename T, size_t NODES_PER_CHUNK >
    class Pool : public IPool
    {
    public:
		Pool(IArena& arena);
        ~Pool();
        Pool(const Pool&) = delete;
        Pool& operator = (const Pool&) = delete;
        void AllocMemory();
        T* Alloc();
        void Free(void* p);
        void* Allocate(std::size_t count) override
        {
        	assert(count <= sizeof(T));
        	return (void*)Alloc();
        }
        void Release(void* ptr) override
        {
        	Free(ptr);
        }
		bool IsPointerFromPool(void* p) const
		{
			Header* header = (Header*)((char*)p - sizeof(Header));
			return header->thisPool0_ == this && header->thisPool1_ == this && header->thisPool2_ == this;
		}
    private:
		struct Header
		{
			void* thisPool0_;
			void* thisPool1_;
			void* thisPool2_;
		};
        struct NodeList
        {
			Header header_;
			union
			{
				NodeList* nextNode_;
				char memBlock_[sizeof(T)];
			};
        };

        typedef NodeList* PNodeList;
        PNodeList freeList_;

        struct ChunkList
        {
            ChunkList* nextChunk_;
        };

        typedef ChunkList* PChunkList;
        PChunkList chunkList_;

        size_t nodeSize_;
        size_t chunkSize_;
		IArena& arena_;
    };

    template< typename T, size_t NODES_PER_CHUNK>
    Pool<T, NODES_PER_CHUNK>::Pool(IArena& arena)
        : chunkList_(nullptr),
          freeList_(nullptr),
		  nodeSize_(sizeof(NodeList)),
          chunkSize_(sizeof(ChunkList) + nodeSize_ * NODES_PER_CHUNK),
          arena_(arena)
    {
        AllocMemory();
    }

    template< typename T, size_t NODES_PER_CHUNK>
    Pool<T, NODES_PER_CHUNK>::~Pool()
    {
        PChunkList pNext = chunkList_->nextChunk_;
        while (chunkList_)
        {
            arena_.Deallocate((char*)chunkList_, chunkSize_);
            chunkList_ = pNext;
            if (chunkList_)
                pNext = chunkList_->nextChunk_;
        }
    }

    template< typename T, size_t NODES_PER_CHUNK>
    void Pool<T, NODES_PER_CHUNK>::AllocMemory()
    {
        char* p = arena_.Allocate(chunkSize_);
        if (!chunkList_)
        {
            chunkList_ = (PChunkList)p;
            chunkList_->nextChunk_ = nullptr;
        }
        else
        {
            ((PChunkList)p)->nextChunk_ = chunkList_;
            chunkList_ = (PChunkList)p;
        }

        p += sizeof(ChunkList);
        freeList_ = (PNodeList)p;

        // Constructs the empty list.
        PNodeList old = freeList_;
		old->header_.thisPool0_ = this;
		old->header_.thisPool1_ = this;
		old->header_.thisPool2_ = this;

        for (size_t i = 0; i < NODES_PER_CHUNK; i++)
        {
            old = (PNodeList)p;
            p += nodeSize_;
            old->nextNode_ = (PNodeList)p;
			old->nextNode_->header_.thisPool0_ = this;
			old->nextNode_->header_.thisPool1_ = this;
			old->nextNode_->header_.thisPool2_ = this;
        }
        old->nextNode_ = nullptr;
    }

    template< typename T, size_t NODES_PER_CHUNK>
    T* Pool<T, NODES_PER_CHUNK>::Alloc()
    {
		if (!freeList_)
			return nullptr;

        PNodeList p = freeList_;
        freeList_ = freeList_->nextNode_;
		assert(IsPointerFromPool(p->memBlock_));
		return (T*)(p->memBlock_);
    }

    template< typename T, size_t NODES_PER_CHUNK>
    void Pool<T, NODES_PER_CHUNK>::Free(void* obj)
    {
		assert(IsPointerFromPool(obj));
		obj = (char*)obj - sizeof(Header);
	    PNodeList p = static_cast<PNodeList>(obj);
	    p->nextNode_ = freeList_;
	    freeList_ = p;
    }
}
