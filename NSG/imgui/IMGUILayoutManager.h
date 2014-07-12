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
#include <set>

namespace NSG
{
    namespace IMGUI
    {
        struct IWindow;
        struct LayoutArea
        {
            GLushort id_;
            int percentageX_;
            int percentageY_;
            PNode pNode_;
            LayoutType type_;
            float textOffsetX_;
            unsigned int cursor_character_position_;
            LayoutArea *parent_;
            PNode childrenRoot_; //node used to perform the scrolling
            bool isScrollable_;
            bool isXScrollable_;
            bool isYScrollable_;
            float scrollFactorAreaX_;
            float scrollFactorAreaY_;
            Area *controlArea_;

            struct Sorting : public std::binary_function<PLayoutArea, PLayoutArea, bool>
            {
                bool operator()(const PLayoutArea &a, const PLayoutArea &b) const
                {
                    return a->id_ < b->id_;
                }
            };

            std::set<PLayoutArea, Sorting> children_; // ordered by id_ (line number or __COUNTER__)

            LayoutArea(GLushort id, LayoutArea *parent, PNode pNode, LayoutType type, int percentageX, int percentageY);
            void CalculateScrollAreaFactor();
            bool IsVisible() const;
            void Set(const Vertex3 &position, const Vertex3 &scale);
            bool IsInside(const Vertex3 &point) const;
        };

        struct WindowManager
        {
            State &uistate_;
            GLushort id_;
            GLushort lastId_;
            typedef std::list<PArea> NestedAreas;
            NestedAreas nestedAreas_;
            typedef std::map<GLushort, PLayoutArea> AREAS;
            AREAS areas_;
            bool layoutChanged_;
            size_t visibleAreas_;
            bool newControlAdded_;
            bool isStable_;
            PNode pRootNode_;
            bool visible_;
            int percentageX_;
            int percentageY_;
            IWindow *userWindow_;
            PTextManager pTextManager_;
            GLushort hotitem_;
            GLushort activeitem_;
            GLushort kbditem_;
            GLushort lastwidget_;
            Window *currentWindow_;

            WindowManager(IWindow *userWindow, GLushort id, PNode pRootNode, int percentageX, int percentageY);
            PLayoutArea InsertNewArea(GLushort id, LayoutType type, int percentageX, int percentageY);
            void Reset();
            void Begin();
            void End();
            PLayoutArea GetAreaForControl(GLushort id, LayoutType type, int percentageX, int percentageY);
            PLayoutArea GetArea(GLushort id) const;
            void BeginWindow(GLushort id, int percentageX, int percentageY);
            void EndWindow();
            void InsertArea(PArea area);
            void BeginHorizontalArea(GLushort id, int percentageX, int percentageY);
            void BeginVerticalArea(GLushort id, int percentageX, int percentageY);
            float EndArea(float scroll);
            void Spacer(GLushort id, int percentageX, int percentageY);
            bool IsReady() const;
            bool IsStable() const;
            size_t GetNestingLevel();
            void RecalculateLayout(PLayoutArea pCurrentArea);
            GLushort GetValidId(GLushort id);
            PTextMesh GetCurrentTextMesh(GLushort item, int maxLength);
            void Invalidate();

        };


        class LayoutManager
        {
        public:
            LayoutManager(PNode pRootNode);
            void Render();
            void RenderUserWindow();
            PLayoutArea GetAreaForControl(GLushort id, LayoutType type, int percentageX, int percentageY);
            void BeginHorizontalArea(GLushort id, int percentageX, int percentageY);
            void BeginVerticalArea(GLushort id, int percentageX, int percentageY);
            float EndArea(float scroll);
            void Spacer(GLushort id, int percentageX, int percentageY);
            size_t GetNestingLevel();
            bool IsReady() const;
            void Window(GLushort id, IMGUI::IWindow *obj, int percentageX, int percentageY);
            void Invalidate();
            void SetWindowFocus(float x, float y);
            GLushort GetValidId(GLushort id);
            PTextMesh GetCurrentTextMesh(GLushort item, int maxLength);
            bool IsCurrentWindowActive() const;
            bool IsFirstOnTopOfSecond(IWindow *first, IWindow *second) const;
            PNode GetCurrentWindowNode() const;
            PWindowManager GetCurrentWindowManager() const;

        private:

            typedef std::map<IWindow *, PWindowManager> WindowManagers;

            WindowManagers windowManagers_;
            PWindowManager currentWindowManager_;
            IWindow *focusedUserWindow_;
            bool focusHasChanged_;
            std::list<IWindow *> windowsSequence_; //Contains the sequence in what the windows are rendered

        };
    }
}