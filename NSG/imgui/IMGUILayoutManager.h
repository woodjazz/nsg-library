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
#include "Node.h"
#include "Types.h"
#include "Check.h"
#include <list>
#include <map>

namespace NSG 
{
	namespace IMGUI
	{
		struct LayoutArea
		{
			GLushort id_;
			int percentageX_;
			int percentageY_;
			PNode pNode_;
			LayoutType type_;
			bool isReadOnly_;
			float textOffsetX_;
			unsigned int cursor_character_position_; 
			LayoutArea* parent_;
			PNode childrenRoot_; //node used to perform the scrolling
			bool isScrollable_;
			float scrollFactorAreaX_;
			float scrollFactorAreaY_;

            struct Sorting : public std::binary_function<PLayoutArea, PLayoutArea, bool>
            {
                bool operator()(const PLayoutArea& a, const PLayoutArea& b) const
                {
                    return a->id_ < b->id_;
                }
            };

            std::set<PLayoutArea, Sorting> children_; // ordered by id_ (line number or __COUNTER__)

			LayoutArea(GLushort id, bool isReadOnly, LayoutArea* parent, PNode pNode, LayoutType type, int percentageX, int percentageY);
			void CalculateScrollAreaFactor();
		};

		class LayoutManager
		{
        public:
			LayoutManager(PNode pRootNode, PNode pCurrentNode); 
			PLayoutArea InsertNewArea(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY);
			void Reset();
			void Begin();
			void End();
			PLayoutArea GetAreaForControl(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY);
			PLayoutArea GetArea(GLushort id) const;
			void BeginHorizontalArea(GLushort id, int percentageX = 0, int percentageY = 0);
			void BeginVerticalArea(GLushort id, int percentageX = 0, int percentageY = 0);
			float EndArea(float scroll);
			void Spacer(GLushort id, int percentageX = 0, int percentageY = 0);
			void RecalculateLayout(PLayoutArea pCurrentArea);
			bool IsStable() const;
			size_t GetNestingLevel() const {return nestedAreas_.size(); }
		private:
			std::list<PArea> nestedAreas_;
			typedef std::map<GLushort, PLayoutArea> AREAS;
			AREAS areas_;
			bool layoutChanged_;
            size_t visibleAreas_;
            bool newControlAdded_;
            bool isStable_;
            PNode pRootNode_;
            PNode pCurrentNode_;
		};
	}
}	