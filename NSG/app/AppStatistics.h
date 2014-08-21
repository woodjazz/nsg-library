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

#include "Singleton.h"
#include "IMGUI.h"
#include <vector>

namespace NSG 
{
	class AppStatistics : public Singleton<AppStatistics>, public IMGUI::IWindow
	{
	public:
		AppStatistics();
		~AppStatistics();
		void Reset();
		void AddVertexBuffer(bool dynamic);
		void AddIndexBuffer(bool dynamic);
		void RemoveVertexBuffer(bool dynamic);
		void RemoveIndexBuffer(bool dynamic);
		void NewDrawCall() { if(collect_) ++drawCalls_; }
		void NewTriangles(size_t n) { if(collect_) triangles_ += n; }
		void RenderGUIWindow();
	private:
  		size_t staticVertexBuffers_;
		size_t staticIndexBuffers_;
		size_t dynamicVertexBuffers_;
		size_t dynamicIndexBuffers_;
		size_t triangles_;
		size_t drawCalls_;
		size_t fps_;
		size_t frames_;
        bool collect_;
        TimePoint startTime_;
 	};
}
