/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "SignalSlots.h"
#include <set>

namespace NSG
{
	struct SigCollector
	{
		static bool alive_;
		static std::set<ISignal*> signals_;
		static std::set<ISignal*>::iterator it_;
		~SigCollector()
		{
			SigCollector::alive_ = false;
		}
		static void Insert(ISignal* obj)
		{
			if (alive_)
				it_ = signals_.insert(it_, obj);
		}
		static void Remove(ISignal* obj)
		{
			if (alive_)
			{
				it_ = signals_.find(obj);
				it_ = signals_.erase(it_);
			}
		}

		static void FreeFirst(int n)
		{
			if (alive_)
			{
				if (alive_)
				{
					//tries to free at least one slot
					while (n-- && it_ != signals_.end())
						(*it_++)->FreeSlots();
					if (it_ == signals_.end())
						it_ = signals_.begin();
				}
			}
		}
		
		static void FreeAtLeastOneDestroyedSlot()
		{
			if (alive_)
			{
				//tries to free at least one slot
				while (it_ != signals_.end())
					if ((*it_++)->FreeSlots())
						break;
				if (it_ == signals_.end())
					it_ = signals_.begin();
			}
		}

		static void FreeAllDestroyedSlots()
		{
			if (alive_)
			{
				for (auto obj : signals_)
					obj->FreeSlots();
			}
		}

	};
	
	static SigCollector sigCollector;
	bool SigCollector::alive_ = true;
	std::set<ISignal*> SigCollector::signals_;
	std::set<ISignal*>::iterator SigCollector::it_ = signals_.end();

	bool SignalsAlive()
	{
		return SigCollector::alive_;
	}

	ISignal::ISignal()
	{
		SigCollector::Insert(this);
	}

	ISignal::~ISignal()
	{
		SigCollector::Remove(this);
	}

	void ISignal::FreeFirst(int n)
	{
		SigCollector::FreeFirst(n);
	}

	void ISignal::FreeAtLeastOneDestroyedSlot()
	{
		SigCollector::FreeAtLeastOneDestroyedSlot();
	}

	void ISignal::FreeAllDestroyedSlots()
	{
		SigCollector::FreeAllDestroyedSlots();
	}
}