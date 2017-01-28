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
#pragma once
#include <cstddef>
#include <chrono>
namespace NSG 
{
	class Tick
	{
	public:
		Tick(size_t fps);
		~Tick();
		void Initialize();
		void PerformTicks();
    protected:
		virtual void InitializeTicks() = 0;
		virtual void BeginTicks() = 0;
		virtual void DoTick(float delta) = 0;
		virtual void EndTicks() = 0;
	private:
	    typedef std::chrono::steady_clock::time_point TimePoint;
	    typedef std::chrono::milliseconds Milliseconds;
        TimePoint current_;
        TimePoint next_;
        Milliseconds ticks_;
        float fixed_;
        size_t fps_;
	};
}
