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
using namespace NSG::FSM;
static std::string text1;
static std::string text2;
static bool buttonPressed = false;
struct Test : public App 
{
    struct INITIALIZING : public State 
    {
    	void Begin() 
    	{
    		IMGUI::OnMouseDown(0, -0.5f, -0.5f);
    	}

    	void Stay()
    	{
            if(text1 == "")
            {
				IMGUI::OnMouseUp(0, -0.5f, -0.5f);
                IMGUI::OnChar('i');
            }
    		else if(text1 == "i")
    		{
    			IMGUI::OnChar('n');
    		}
    		else if(text1 == "in")
    		{
    			IMGUI::OnChar('i');
    		}
    		else if(text1 == "ini")
    		{
    			IMGUI::OnChar('t');
    		}
    	}

    	void End()
    	{
    		CHECK_ASSERT(text1 == "init", __FILE__, __LINE__);
    	}
    };

    struct STATE1 : public State 
    {
    	void Begin() 
    	{
    		CHECK_ASSERT(text1 == "init", __FILE__, __LINE__);
            IMGUI::OnMouseDown(0, 0.5f, -0.5f);
    	}

        void Stay()
        {
            if(text2 == "")
            {
                IMGUI::OnMouseUp(0, 0.5f, -0.5f);

                IMGUI::OnChar('s');
            }
            else if(text2 == "s")
            {
                IMGUI::OnChar('t');
            }
            else if(text2 == "st")
            {
                IMGUI::OnChar('1');
            }
        }

        void End()
        {
            CHECK_ASSERT(text2 == "st1", __FILE__, __LINE__);
        }
    };

    struct STATE2 : public State 
    {
    	void Begin() 
    	{
            IMGUI::OnMouseDown(0, 0, 0.5f);
    	}

        void Stay()
        {
            IMGUI::OnMouseUp(0, 0, 0.5f);
        }

        void End()
        {
            CHECK_ASSERT(text1 == "State2", __FILE__, __LINE__);
        }

    };

    struct END : public State 
    {
    	void Begin() 
    	{
    		text2 = "End";	
    	}
    };

    INITIALIZING initializing_;
    STATE1 state1_;
    STATE2 state2_;
    END end_;
    PMachine machine_;
	PScene scene_;

	void Start(int argc, char* argv[]) override
	{
		scene_ = GetCurrentScene();
		//IMGUI::Context::this_->pSkin_->buttonStyle_->fontFile_ = "font/FreeSans.ttf";
		//IMGUI::Context::this_->pSkin_->textStyle_->fontFile_ = "font/FreeSans.ttf";
	    initializing_.AddTransition(state1_).When([&]() { return text1 == "init"; });
	    state1_.AddTransition(state2_).When([&]() { return text2 == "st1"; });
	    state2_.AddTransition(end_).When([&]() { return text1 == "State2"; });
	    machine_ = PMachine(new Machine(initializing_));
	}

	void Update() override
	{
        if(IMGUI::IsReady())
        {
		    machine_->Update();
        }
	}

	void RenderGUIWindow() override
	{
		buttonPressed = IMGUIButton("Button", 100, 50);

        if(buttonPressed)
        {
            text1 = "State2";
        }
	        
        IMGUIBeginHorizontal(100, 50);

		    text1 = IMGUITextField(text1, 50);
			text2 = IMGUITextField(text2, 50);
	    
	    IMGUIEndArea();
	}

	bool ShallExit() const override
	{ 
		return machine_ && machine_->GetState() == &end_;
	}
};


NSG_MAIN(Test);
