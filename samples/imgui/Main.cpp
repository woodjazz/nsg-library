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
        IMGUISkin()->alphaFactor_ = 0;
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
                IMGUIBeginHorizontal(0);
                IMGUIBeginVertical(0);
                IMGUISpacer(80);
                menu_choosen = IMGUIButton("Menu", 0);
                IMGUIEndVertical();
                IMGUISpacer(80);
                IMGUIEndHorizontal();
            }

            if(menu_choosen)
            {
                show_menu_button = false;
                IMGUIBeginVertical(0);
                IMGUILabel("Are you sure?", 0);
                IMGUIBeginHorizontal(0);
                if(IMGUIButton("Yes", 0))
                {
                    IMGUISkin()->alphaFactor_ = 1;
                    menu = true;
                    delta = 0;
                    menu_choosen = false;
                    show_menu_button = true;

                }
                if(IMGUIButton("No", 0))
                {
                    menu_choosen = false;
                    show_menu_button = true;
                }
                IMGUIEndHorizontal();
                IMGUIEndVertical();
            }
            
            if(IMGUISkin()->alphaFactor_ < 1)
                IMGUISkin()->alphaFactor_ += 0.01f;
            else
                IMGUISkin()->alphaFactor_ = 1;
        }
        else
        {
            IMGUIBeginVertical(100);
            int fontSize = IMGUISkin()->fontSize_;
            IMGUISkin()->fontSize_ = 12;
            IMGUINode()->SetPosition(position);
            
            IMGUISpacer(5);

            static const int MAX_FIELDS = 5;
            static std::string field[MAX_FIELDS];
            for(int i=0; i<MAX_FIELDS; i++)
            {
                std::stringstream label;
                label << "Label " << i << ":";

                IMGUIBeginHorizontal(0);
                IMGUISpacer(20);
                IMGUILabel(label.str(), 0); 
		        field[i] = IMGUITextField(field[i], 0);
                IMGUISpacer(20);
                IMGUIEndHorizontal();
            }
            IMGUISkin()->fontSize_ = fontSize;

            IMGUISpacer(5);

            static bool exit = false;

            IMGUIBeginHorizontal(30);
            //IMGUISpacer(30);
            if(IMGUIButton("Exit", 0))
                exit = true;
            //IMGUISpacer(30);
            IMGUIEndHorizontal();

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
                IMGUISkin()->alphaFactor_ = 0;
            }
            IMGUIEndVertical();
        }
    }
};

NSG_MAIN(Sample);
