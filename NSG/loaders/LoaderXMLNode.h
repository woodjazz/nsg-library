/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Object.h"
#include "pugixml.hpp"

namespace NSG
{
	class LoaderXMLNode : public Object
	{
	public:
		LoaderXMLNode(const std::string& name);
		~LoaderXMLNode();
		void Set(LoaderXML* loaderXML, PObject obj, const std::string& type, const std::string& nameAttValue);
		bool Load();
	private:
		bool IsValid() override;
		void AllocateResources() override;
		void ReleaseResources() override;
		LoaderXML* loaderXML_;
		PWeakObject obj_;
		std::string type_;
		std::string nameAttValue_;
		PResource resource_;
		pugi::xml_node node_;
	};
}