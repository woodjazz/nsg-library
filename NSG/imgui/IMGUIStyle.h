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

namespace NSG 
{
	namespace IMGUI
	{
		struct Style
		{
			static PProgram unlitProgram_;
			static PMesh controlMesh_;

			bool enableActive_;
			bool enableHot_;
			bool enableFocus_;

			PTechnique activeTechnique_;
			PTechnique normalTechnique_;
			PTechnique hotTechnique_;

			Style();
			Style(const Style& obj);
		};

		struct LineStyle : Style
		{
			size_t pixels_;
			LineStyle();
			LineStyle(const LineStyle& obj);
		};

		struct AreaStyle : Style
		{
			bool showVScroll_;
			bool showHScroll_;
			PTechnique vScrollTechnique_;
			PTechnique hScrollTechnique_;

			AreaStyle();
			AreaStyle(const AreaStyle& obj);
		};

		struct SliderStyle : AreaStyle
		{
			PStyle thumbSliderStyle_;

			SliderStyle();
			SliderStyle(const SliderStyle& obj);
		};

		struct SizerStyle : Style
		{
			SizerStyle();
			SizerStyle(const SizerStyle& obj);
		};

		struct LabelStyle : Style
		{
			Color textColor_;
			std::string fontAtlasFile_;

			LabelStyle();
			LabelStyle(const LabelStyle& obj);
		};

		struct ButtonStyle : Style
		{
			Color textColor_;
			std::string fontAtlasFile_;

			ButtonStyle();
			ButtonStyle(const ButtonStyle& obj);
		};

		struct CheckButtonStyle : ButtonStyle
		{
			PStyle pressedStyle_;

			CheckButtonStyle();
			CheckButtonStyle(const CheckButtonStyle& obj);
		};

		struct TextStyle : Style
		{
			Color textColor_;
			std::string fontAtlasFile_;
			size_t textMaxLength_;

			TextStyle();
			TextStyle(const TextStyle& obj);
		};

		struct TitleStyle : Style
		{
			Color textColor_;
			size_t pixelsHeight_;
			std::string fontAtlasFile_;

			TitleStyle();
			TitleStyle(const TitleStyle& obj);
		};

		struct WindowStyle : AreaStyle
		{
			std::string fontAtlasFile_;
			size_t sizerPixels_;


			PTitleStyle titleStyle_;

			PSizerStyle sizerLeftTopStyle_;
			PSizerStyle sizerTopStyle_;
			PSizerStyle sizerRightTopStyle_;
			PSizerStyle sizerLeftStyle_;
			PSizerStyle sizerRightStyle_;
			PSizerStyle sizerLeftBottomStyle_;
			PSizerStyle sizerBottomStyle_;
			PSizerStyle sizerRightBottomStyle_;


			WindowStyle();
			WindowStyle(const WindowStyle& obj);
		};

	}
}