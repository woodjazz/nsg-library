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
#include "SharedPointers.h"
#include <list>
#include <map>

namespace NSG 
{
	namespace IMGUI
	{
		struct LayoutArea
		{
			GLushort id_;
			int percentage_;
			PNode pNode_;
			enum Type {Vertical, Horizontal, Control};
			Type type_;
			float textOffsetX_;
			unsigned int cursor_character_position_; 

            struct Sorting : public std::binary_function<PLayoutArea, PLayoutArea, bool>
            {
                bool operator()(const PLayoutArea& a, const PLayoutArea& b) const
                {
                    return a->id_ < b->id_;
                }
            };

			std::set<PLayoutArea, Sorting> children_; // ordered by id_ (line number)

			LayoutArea(GLushort id, PNode pNode, Type type, int percentage) 
			: id_(id), percentage_(percentage), pNode_(pNode), type_(type), textOffsetX_(0), cursor_character_position_(0)
			{
			}
		};

		class LayoutManager
		{
        public:
			LayoutManager(PNode pRootNode, PNode pCurrentNode); 
			PLayoutArea InsertNewArea(GLushort id, LayoutArea::Type type, int percentage);
			void Reset();
			void Begin();
			void End();
			PLayoutArea GetAreaForControl(GLushort id, LayoutArea::Type type, int percentage);
			void BeginHorizontal(GLushort id, int percentage);
			void EndHorizontal();
			void BeginVertical(GLushort id, int percentage);
			void EndVertical();
			void Spacer(GLushort id, int percentage);
			void RecalculateLayout(PLayoutArea pCurrentArea);
			bool IsStable() const;
		private:
			std::list<PLayoutArea> nestedAreas_;
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