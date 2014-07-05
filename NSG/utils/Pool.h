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
#include <algorithm>
#include <cassert>

namespace NSG
{
	template <typename T, size_t NODES_PER_CHUNK > 
	class Pool
	{
	public:
		Pool();
		~Pool();
		Pool(const Pool&) = delete;
    	Pool& operator = (const Pool&) = delete;
		void AllocMemory();
		T* Alloc();
		void Free(void* p);
	private:
		struct NodeList
		{
			NodeList* nextNode_;
		};

		typedef NodeList* PNodeList;
		PNodeList pFreeList_;

		struct ChunkList
		{
			ChunkList* nextChunk_;
		};

		typedef ChunkList* PChunkList;
		PChunkList pChunkList_;

		size_t chunkSize_;
		size_t nodeSize_;
	};

	template< typename T, size_t NODES_PER_CHUNK> 
	Pool<T, NODES_PER_CHUNK>::Pool()
	: pChunkList_(0),
	pFreeList_(0),
	chunkSize_(0),
	nodeSize_(std::max(sizeof(NodeList), sizeof(T)))
	{
		chunkSize_ = sizeof(ChunkList) + nodeSize_ * NODES_PER_CHUNK;
		AllocMemory();
	}

	template< typename T, size_t NODES_PER_CHUNK> 
	Pool<T, NODES_PER_CHUNK>::~Pool()
	{
		PChunkList pNext = pChunkList_->nextChunk_;

		while(pChunkList_)
		{
			::free(pChunkList_);
			pChunkList_ = pNext;
			if(pChunkList_)
				pNext = pChunkList_->nextChunk_;
		}
	}

	template< typename T, size_t NODES_PER_CHUNK> 
	void Pool<T, NODES_PER_CHUNK>::AllocMemory()
	{
		char* p = (char*)::malloc(chunkSize_);

		if(!pChunkList_)
		{
			pChunkList_ = (PChunkList)p;
			pChunkList_->nextChunk_ = nullptr;
		}
		else
		{
			((PChunkList)p)->nextChunk_ = pChunkList_;
			pChunkList_ = (PChunkList)p;
		}

		p += sizeof(ChunkList);
		pFreeList_ = (PNodeList)p;

		// Constructs the empty list.
		PNodeList pOld = pFreeList_;

		for (size_t i=0; i<NODES_PER_CHUNK; i++) 
		{
			pOld = (PNodeList)p;
			p += nodeSize_;
			pOld->nextNode_ = (PNodeList)p;
		}

		pOld->nextNode_ = nullptr;
	}

	template< typename T, size_t NODES_PER_CHUNK> 
	T* Pool<T, NODES_PER_CHUNK>::Alloc()
	{
		if(!pFreeList_) 
			AllocMemory();

		PNodeList p = pFreeList_;
		pFreeList_ = pFreeList_->nextNode_;
		return (T*)p;
	}

	template< typename T, size_t NODES_PER_CHUNK> 
	void Pool<T, NODES_PER_CHUNK>::Free(void* obj)
	{	   
		assert(obj != nullptr);
		PNodeList p = static_cast< PNodeList >(obj);
		p->nextNode_ = pFreeList_;
		pFreeList_ = p;
	}
}
