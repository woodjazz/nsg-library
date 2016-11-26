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
#include "Tick.h"
#include "Check.h"

namespace NSG
{
	typedef std::chrono::steady_clock Clock;

	Tick::Tick(size_t fps) 
	: ticks_(0), 
	fixed_(0), 
    fps_(fps)
	{
		CHECK_ASSERT(fps_ > 0);
	}

	Tick::~Tick()
	{
	}

	void Tick::Initialize()
	{
	    ticks_ = Milliseconds(1000 / fps_);
	    fixed_ = 1.0f / (float)fps_;
		InitializeTicks();
		current_ = next_ = Clock::now();
	}

	void Tick::PerformTicks()
	{
		bool lock = false;
		int loop = 0;
		const int MAX_LOOP = 10;

		BeginTicks();

		while ((current_ = Clock::now()) > next_ && loop < MAX_LOOP)
		{
			DoTick(fixed_);
			Milliseconds duration = std::chrono::duration_cast<Milliseconds>(Clock::now() - current_);
			if (duration > ticks_)
			{
				lock = true;
				break;
			}
			next_ += ticks_;
			++loop;
		}

		if (lock || current_ > next_)
		{
			current_ = next_ = Clock::now(); // sync tick back
		}

		EndTicks();
	}
}