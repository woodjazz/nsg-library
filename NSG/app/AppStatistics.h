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
#include "Types.h"
#include "Singleton.h"
#include <string>

namespace NSG 
{
	class AppStatistics : public Singleton<AppStatistics>
	{
	public:
		AppStatistics();
		~AppStatistics();
		void NewFrame();
		void AddVertexBuffer(bool dynamic);
		void AddIndexBuffer(bool dynamic);
		void RemoveVertexBuffer(bool dynamic);
		void RemoveIndexBuffer(bool dynamic);
		void NewDrawCall();
		void NewTriangles(size_t n);
		void Show();
	private:
		enum class Stats {FPS, DRAW_CALLS, TRIANGLES, STATIC_VBO, STATIC_IBO, DYNAMIC_VBO, DYNAMIC_IBO, MAX_STATS};
		static const int LINES = 7;
		std::string label_[(int)Stats::MAX_STATS];
		size_t stats_[(int)Stats::MAX_STATS];
		size_t frames_;
        bool collect_;
        TimePoint startTime_;
        PPass pass_;
        PMaterial material_;
        PTextMesh text_[(int)Stats::MAX_STATS];
        PNode node_[(int)Stats::MAX_STATS];
 	};
}
