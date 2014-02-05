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
#include <chrono>

namespace NSG 
{
	class Tick
	{
	public:
		Tick();
		~Tick();
		void Initialize(int fps);
		void PerformTick();
		virtual void BeginTick() = 0;
		virtual void DoTick(float delta) = 0;
		virtual void EndTick() = 0;
	private:
		typedef std::chrono::milliseconds Milliseconds;
		typedef std::chrono::seconds Seconds;
        typedef std::chrono::steady_clock Clock;
        typedef Clock::time_point TimePoint;

        TimePoint current_;
        TimePoint next_;
        Milliseconds ticks_;
        float fixed_;
        bool init_;
	};
}