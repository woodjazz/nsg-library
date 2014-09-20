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

struct Window0 : IMGUI::IWindow
{
    Window0()
    {
        IWindow::hasTitle_ = true;
        IWindow::resizable_ = true;
        IWindow::title_ = "Window0";
    }

    void StartGUIWindow() 
    {
    }

    void RenderGUIWindow()
    {
        static const int MAX_FIELDS = 10;
        static std::string field[MAX_FIELDS];
		static bool showFields = false;
		showFields = IMGUICheckButton(showFields, showFields ? "Hide Fields" : "Show Fields", 100, 20);
		if (showFields)
		{
			for (int i = 0; i < MAX_FIELDS; i++)
			{
				std::stringstream label;
				label << "Label " << i << ":";

				IMGUIBeginHorizontal(150, 50);
				IMGUILabel(label.str(), 50);
				float linePercentage = IMGUILine();
                //IMGUILabel("sdfkjhsdfhsdkjfh", 50 - linePercentage);
				field[i] = IMGUITextField(field[i], 50 - linePercentage);
				IMGUIEndArea();
			}
		}
    }

    void EndGUIWindow()
    {
    }
};

struct Sample : App
{
    Window0 window0_;
	IMGUI::PWindowStyle style_;
    PScene scene_;

    Sample()
    {
        AppConfiguration::this_->width_ = 320;
        AppConfiguration::this_->height_ = 200;
		AppConfiguration::this_->showStatistics_ = true;
    }

	void Start(int argc, char* argv[]) override
	{
		scene_ = GetCurrentScene();

		if (!style_)
		{
			IMGUISkin()->labelStyle_->fontAtlasFile_ = "data/font/andalus_regular_20.png";
			IMGUISkin()->buttonStyle_->fontAtlasFile_ = "data/font/andalus_regular_20.png";
			IMGUISkin()->textStyle_->fontAtlasFile_ = "data/font/andalus_regular_20.png";
			style_ = IMGUI::PWindowStyle(new IMGUI::WindowStyle);

			PTexture texture(new TextureFile("data/metal.png"));
			PMaterial material = style_->activeMaterial_;
            material->SetTexture0(texture);

			material = style_->normalMaterial_;
            material->SetTexture0(texture);        

			material = style_->hotMaterial_;
            material->SetTexture0(texture);   

			IMGUISkin()->windowStyle_ = style_;
        }
    }

    void RenderGUIWindow() override
    {	
#if 1
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
			IMGUINode()->SetPosition(Vertex3(0, 0, IMGUINode()->GetPosition().z));

            if(show_menu_button)
            {
				#if 1
				IMGUIBeginHorizontal(100, 40);
				{
					static float sliderValue = 0.5f;
					std::stringstream ss;
					ss << "value=" << sliderValue;
					IMGUILabel(ss.str(), 50, 100);
					sliderValue = IMGUIVSlider(sliderValue, 50, 100);
				}
				IMGUIEndArea();

                IMGUIBeginHorizontal(100, 40);
                {
                    static float sliderValue = 0.5f;
                    std::stringstream ss;
                    ss << "value=" << sliderValue;
                    IMGUILabel(ss.str(), 50, 100);
                    sliderValue = IMGUIHSlider(sliderValue, 50, 100);
                }
                IMGUIEndArea();
				#endif

				//menu_choosen = IMGUIButton("Menu", 20, 20);
				menu_choosen = IMGUIButton("ABCDEFGabcdefghijk0123456", 20, 20);
            }

            if(menu_choosen)
            {
                IMGUIWindow(&window0_, 60, 60);
                show_menu_button = false;
				IMGUILabel("Are you sure?", 100, 50);
                IMGUIBeginHorizontal(100, 50);
				if (IMGUIButton("Yes", 50))
                {
                    menu = true;
                    delta = 0;
                    menu_choosen = false;
                    show_menu_button = true;

                }
				if (IMGUIButton("No", 50))
                {
                    menu_choosen = false;
                    show_menu_button = true;
                }
                IMGUIEndArea();
            }
        }
        else
        {
            IMGUINode()->SetPosition(position);
            
            IMGUISpacer(100, 5);

            IMGUIBeginVertical(100, 70);

                static const int MAX_FIELDS = 15;
                static std::string field[MAX_FIELDS];
                for(int i=0; i<MAX_FIELDS; i++)
                {
                    std::stringstream label;
                    label << "Label " << i << ":";

					IMGUILine();
                    IMGUIBeginHorizontal(100, 25);
					IMGUILabel(label.str(), 50);
					field[i] = IMGUITextField(field[i], 50);
                    IMGUIEndArea();
                }

            IMGUIEndArea();

            IMGUISpacer(100, 5);

            static bool exit = false;

            IMGUIBeginHorizontal(100, 20);
			if (IMGUIButton("Exit"))
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
#endif
    }

};

NSG_MAIN(Sample);
