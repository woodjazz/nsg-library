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
#pragma once
#include "Types.h"
#include <string>
namespace NSG 
{
    class App;
	class Object
	{
	public:
		Object(const std::string& name);
		virtual ~Object();
		void Invalidate();
		bool IsReady();
		virtual void LoadFrom(PResource resource, const pugi::xml_node& node) {}
		static void InvalidateAll();
		const std::string& GetName() const { return name_; }
		SignalEmpty::PSignal signalAllocated_;
		SignalEmpty::PSignal signalReleased_;
	protected:
		std::string name_;
	private:
		static SignalEmpty::PSignal signalInvalidateAll_;
		virtual bool IsValid() { return true; }
		virtual void AllocateResources() {}
		virtual void ReleaseResources()	{}
		std::string GetType() const;
		std::string GetNameType() const;
		bool isValid_;
		bool resourcesAllocated_;
		SignalEmpty::PSlot slotInvalidateAll_;
	};
}