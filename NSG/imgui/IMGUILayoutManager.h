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
#include "BoundingBox.h"
#include <list>
#include <map>
#include <vector>

namespace NSG
{
    namespace IMGUI
    {
        struct IWindow;
        struct LayoutArea
        {
            IdType id_;
            float percentageX_;
            float percentageY_;
            PNode pNode_;
            LayoutType type_;
            float textOffsetX_;
            unsigned int cursor_character_position_;
            LayoutArea* parent_;
            PNode childrenRoot_; //node used to perform the scrolling
            bool isScrollable_;
            bool isXScrollable_;
            bool isYScrollable_;
            float scrollFactorAreaX_;
            float scrollFactorAreaY_;
            PTextMesh textMesh_;
            PTextMesh cursorMesh_;

/*
            struct Sorting : public std::binary_function<PLayoutArea, PLayoutArea, bool>
            {
                bool operator()(const PLayoutArea& a, const PLayoutArea& b) const
                {
                    return a->id_ < b->id_;
                }
            };
*/
            //std::set<PLayoutArea, Sorting> children_; // ordered by id_ (line number or __COUNTER__)
            std::vector<PLayoutArea> children_;

            LayoutArea(IdType id, LayoutArea* parent, PNode pNode, LayoutType type, float percentageX, float percentageY);
            void CalculateScrollAreaFactor();
            bool IsVisible() const;
            void Set(const Vertex3& position, const Vertex3& scale);
            bool IsInside(const Vertex3& point) const;
            bool HasSizeChanged(float percentageX, float percentageY) const;

        };

        struct WindowManager
        {
            State& uistate_;
            IdType id_;
            IdType lastId_;
            typedef std::list<PArea> NestedAreas;
            NestedAreas nestedAreas_;
            struct AreaKey
            {
                IdType id_;
                LayoutType type_;
				bool operator < (const AreaKey& obj) const
				{
					return (id_ < obj.id_ || (!(obj.id_ < id_) && type_ < obj.type_));
				}
            };
            typedef std::map<AreaKey, PLayoutArea> AREAS;
            AREAS areas_;
            bool layoutChanged_;
            size_t retrievedAreas_;
            bool isStable_;
            PNode pRootNode_;
            bool visible_;
            float percentageX_;
            float percentageY_;
            IWindow* userWindow_;
            IdType hotitem_;
            IdType activeitem_;
            IdType kbditem_;
            IdType lastwidget_;
            Window* currentWindow_;
            bool created_; // true if the user is calling IMGUIWindow

            WindowManager(IWindow* userWindow, IdType id, PNode pRootNode, float percentageX, float percentageY);
            PLayoutArea InsertNewArea(IdType id, LayoutType type, float percentageX, float percentageY);
            void Begin();
            void End();
            PLayoutArea GetAreaForControl(IdType id, LayoutType type, float percentageX, float percentageY);
			PLayoutArea GetArea(IdType id, LayoutType type) const;
            void BeginWindow(float percentageX, float percentageY);
            void EndWindow();
			void BeginHorizontalArea(float percentageX, float percentageY, Style& style);
			void BeginVerticalArea(float percentageX, float percentageY, Style& style);
            float EndArea(float scroll);
            void Spacer(float percentageX, float percentageY);
            bool IsReady() const;
            bool IsStable() const;
            size_t GetNestingLevel();
            void RecalculateLayout(LayoutArea* area);
            IdType GetValidId();
        };


        class LayoutManager
        {
        public:
            LayoutManager(PNode pRootNode);
            void Render();
            void RenderUserWindow();
            PLayoutArea GetAreaForControl(IdType id, LayoutType type, float percentageX, float percentageY);
            void BeginHorizontalArea(float percentageX, float percentageY, Style& style);
            void BeginVerticalArea(float percentageX, float percentageY, Style& style);
            float EndArea(float scroll);
            void Spacer(float percentageX, float percentageY);
            size_t GetNestingLevel();
            bool IsReady() const;
			void Window(IMGUI::IWindow* obj, float percentageX, float percentageY);
            void SetWindowFocus(float x, float y);
            IdType GetValidId();
            bool IsCurrentWindowActive() const;
            PNode GetCurrentWindowNode() const;
            PWindowManager GetCurrentWindowManager() const;
            void MarkAllWindowsAsNonCreated();
			void RemoveAllNonCreatedWindows();

        private:

            typedef std::map<IWindow*, PWindowManager> WindowManagers;

            WindowManagers windowManagers_;
            PWindowManager currentWindowManager_;
            IWindow* focusedUserWindow_;
            bool focusHasChanged_;
            std::list<IWindow*> windowsSequence_;  //Contains the sequence in what the windows are rendered

        };
    }
}