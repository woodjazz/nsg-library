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
#include "Object.h"
#include "Check.h"
#include "App.h"

namespace NSG 
{
	Object::Object()
	: isValid_(false),
	resourcesAllocated_(false)
	{
		App::this_->AddObject(this);
	}
		
	Object::~Object()
	{
		App::this_->RemoveObject(this);
	}

	void Object::Invalidate()
	{
		isValid_ = false;
		if(resourcesAllocated_)
		{
			CHECK_GL_STATUS(__FILE__, __LINE__);
			ReleaseResources();
			CHECK_GL_STATUS(__FILE__, __LINE__);
			resourcesAllocated_ = false;
		}
	}

	bool Object::IsReady()
	{
		if(!isValid_)
		{
			CHECK_GL_STATUS(__FILE__, __LINE__);

			isValid_ = IsValid();

			if(isValid_)
			{
				CHECK_ASSERT(!resourcesAllocated_, __FILE__, __LINE__);
				AllocateResources();
				resourcesAllocated_ = true;
			}

			CHECK_GL_STATUS(__FILE__, __LINE__);
		}

		return isValid_;
	}
}