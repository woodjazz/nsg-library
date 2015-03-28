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

namespace NSG 
{
	struct AppConfiguration
	{
		unsigned fps_;
		int x_; ///the x position of the window
		int y_; ///the y position of the window
		int width_; ///the width of the window
		int height_; ///the height of the window
		bool vertical_sync_;
		int swapInterval_; // See https://wiki.libsdl.org/SDL_GL_SetSwapInterval
		bool showStatistics_;
		bool pauseMusicOnBackground_;
		AppConfiguration();
	};
}
