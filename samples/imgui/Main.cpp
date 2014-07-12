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

static const char* vShader = STRINGIFY(
    uniform mat4 u_mvp;
    attribute vec2 a_texcoord;
    attribute vec4 a_position;
    varying vec2 v_texcoord;

    void main()
    {
        gl_Position = u_mvp * a_position;
        v_texcoord = a_texcoord;
    }
);

static const char* fShader = STRINGIFY(
    varying vec2 v_texcoord;
    uniform sampler2D u_texture0;
    uniform vec4 u_color;
    void main()
    {
        gl_FragColor = texture2D(u_texture0, v_texcoord) * u_color;
    }
);


struct Window0 : IMGUI::IWindow
{
    int fontSize_;
	PMaterial material_;
    Color color_;
    void StartGUIWindow() 
    {
#if 1
		material_ = IMGUISkin()->windowTechnique_->GetPass(0)->GetMaterial();
        fontSize_ = IMGUISkin()->fontSize_;
        IMGUISkin()->fontSize_ = 18;
        color_ = material_->GetColor();
		material_->SetColor(Color(0, 0, 0, 0.7f));
#endif
    }

    void RenderGUIWindow()
    {
		const int MAX_TEXT_SIZE = 15;
        static const int MAX_FIELDS = 10;
        static std::string field[MAX_FIELDS];
        for(int i=0; i<MAX_FIELDS; i++)
        {
            std::stringstream label;
            label << "Label " << i << ":";

            IMGUIBeginHorizontal(150,50);
            IMGUILabel(MAX_TEXT_SIZE, label.str(), 50);
            field[i] = IMGUITextField(MAX_TEXT_SIZE, field[i], 50);
            IMGUIEndArea();
        }
    }

    void EndGUIWindow()
    {
#if 1
        IMGUISkin()->fontSize_ = fontSize_;
        material_->SetColor(color_);
#endif
    }
};

struct Statistics : public AppStatistics
{
    PTexture newTexture_;
    PTexture oldTexture_;
    PMaterial material_;
    PProgram oldProgram_;
    PProgram newProgram_;
    Color color_;


    void StartGUIWindow()
    {
#if 1
        if(!newTexture_)
        {
            material_ = IMGUISkin()->windowTechnique_->GetPass(0)->GetMaterial();

            newTexture_ = PTexture(new TextureFile("blackBump.png"));
            newProgram_ = PProgram(new Program(vShader, fShader));

            oldTexture_ = material_->GetTexture0();
            oldProgram_ = material_->GetProgram();
            color_ = material_->GetColor();
      }

        material_->SetProgram(newProgram_);        
        material_->SetTexture0(newTexture_);  
        material_->SetColor(Color(1,1,1,1));
#endif
    }

    void EndGUIWindow()
    {
#if 1
        material_->SetTexture0(oldTexture_);  
        material_->SetProgram(oldProgram_);
        material_->SetColor(color_);
#endif
    }
};

struct Sample : App
{

    Statistics statistics_;
    Window0 window0_;

	void Start()
	{
        IMGUISkin()->fontFile_ = "font/FreeSans.ttf";
        
        {
            PTexture texture(new TextureFile("metal.png"));
            PMaterial material = IMGUISkin()->activeTechnique_->GetPass(0)->GetMaterial();
            //material->SetColor(Color(1,1,1,0.5f));
            material->SetTexture0(texture);

            material = IMGUISkin()->normalTechnique_->GetPass(0)->GetMaterial();
            material->SetTexture0(texture);        

            material = IMGUISkin()->hotTechnique_->GetPass(0)->GetMaterial();
            material->SetTexture0(texture);        
        }

/*        {
            PTexture texture(new TextureFile("blackBump.png"));
            PMaterial material = IMGUISkin()->windowTechnique_->GetPass(0)->GetMaterial();
            material->SetTexture0(texture);  
            material->SetColor(Color(1,1,1,1));
        }*/
    }

    void RenderGUIWindow()
    {
#if 1		
		const int MAX_TEXT_SIZE = 15;

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
                //AppConfiguration::this_->showStatistics_ = true;
                IMGUISpacer(0, 80);
				menu_choosen = IMGUIButton(MAX_TEXT_SIZE, "Menu", 20, 20);
            }

            if(menu_choosen)
            {
                //AppConfiguration::this_->showStatistics_ = false;
                show_menu_button = false;
				IMGUILabel(MAX_TEXT_SIZE, "Are you sure?", 100, 50);
                IMGUIBeginHorizontal(100, 50);
				if (IMGUIButton(MAX_TEXT_SIZE, "Yes", 50))
                {
                    menu = true;
                    delta = 0;
                    menu_choosen = false;
                    show_menu_button = true;

                }
				if (IMGUIButton(MAX_TEXT_SIZE, "No", 50))
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
					IMGUILabel(MAX_TEXT_SIZE, label.str(), 50);
					field[i] = IMGUITextField(MAX_TEXT_SIZE, field[i], 50);
                    IMGUIEndArea();
                }

            IMGUIEndArea();

            IMGUISkin()->fontSize_ = fontSize;

            IMGUISpacer(0, 5);

            static bool exit = false;

            IMGUIBeginHorizontal(0, 20);
			if (IMGUIButton(MAX_TEXT_SIZE, "Exit"))
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
#if 1        
		IMGUIWindow(&statistics_, 50, 75);
        IMGUIWindow(&window0_, 60, 60);
#endif        

    }
};

NSG_MAIN(Sample);
