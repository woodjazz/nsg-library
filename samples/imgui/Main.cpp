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

#include "NSG.h"
using namespace NSG;

struct Sample : App
{

	void Start()
	{
        IMGUISkin()->fontFile_ = "font/FreeSans.ttf";
    }

    void RenderGUIFrame()
    {
        static float delta = -1;
        static Vertex3 camControlPoint0(-3, 3, 0);
        static Vertex3 camControlPoint1(0, 2, 0);
        static Vertex3 camControlPoint2(0, 0, 0);
        static Vertex3 camControlPoint3(3, -2, 0);

        static bool menu = false;

        if(delta > 1) delta = 1;

        Vertex3 position = glm::catmullRom(
            camControlPoint0,
            camControlPoint1,
            camControlPoint2,
            camControlPoint3,
            delta);

        if(!menu)
        {
            static bool show_menu_button = true;
            static bool menu_choosen = false;
            menu = false;
            IMGUINode()->SetPosition(Vertex3(0,0,0));

            if(show_menu_button)
            {
                IMGUISpacer(0, 80);
                menu_choosen = IMGUIButton("Menu", 20, 20);
            }

            if(menu_choosen)
            {
                show_menu_button = false;
                IMGUILabel("Are you sure?", 100, 50);
                IMGUIBeginHorizontal(100, 50);
                if(IMGUIButton("Yes", 50))
                {
                    menu = true;
                    delta = 0;
                    menu_choosen = false;
                    show_menu_button = true;

                }
                if(IMGUIButton("No", 50))
                {
                    menu_choosen = false;
                    show_menu_button = true;
                }
                IMGUIEndArea();
            }
        }
        else
        {
            int fontSize = IMGUISkin()->fontSize_;
            IMGUISkin()->fontSize_ = 12;
            IMGUINode()->SetPosition(position);
            
            IMGUISpacer(0, 5);

            IMGUIBeginVertical(0, 70);

                static const int MAX_FIELDS = 15;
                static std::string field[MAX_FIELDS];
                for(int i=0; i<MAX_FIELDS; i++)
                {
                    std::stringstream label;
                    label << "Label " << i << ":";

                    IMGUIBeginHorizontal(0,25);
                        IMGUILabel(label.str(), 50); 
		                field[i] = IMGUITextField(field[i], 50);
                    IMGUIEndArea();
                }

            IMGUIEndArea();

            IMGUISkin()->fontSize_ = fontSize;

            IMGUISpacer(0, 5);

            static bool exit = false;

            IMGUIBeginHorizontal(0, 20);
            if(IMGUIButton("Exit"))
                exit = true;
            IMGUIEndArea();

            if(exit)
            {
                delta -= App::this_->GetDeltaTime();
            }
            else
            {
                delta += App::this_->GetDeltaTime();
            }

            if(delta < 0)
            {
                menu = exit = false;
            }
        }
    }
};

NSG_MAIN(Sample);
