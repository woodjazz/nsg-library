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
#include "Object.h"
#include <string>

namespace NSG
{
	class Path;
	class Resource : public std::enable_shared_from_this<Resource>, public Object
	{
	public:
		static PResource CreateFrom(const pugi::xml_node& node);
		virtual ~Resource();
		const char* const GetData() const { return buffer_.c_str(); }
		size_t GetBytes() const { return buffer_.size(); }
        void ReleaseResources() override;
		const std::string& GetBuffer() const { return buffer_; }
		virtual void Load(const pugi::xml_node& node) {}
		void SaveExternal(pugi::xml_node& node, const Path& path, const Path& outputDir);
		void Save(pugi::xml_node& node);
		const std::string& GetName() const { return name_; }
		void SetSerializable(bool serializable);
		bool IsSerializable() const;
		void SetName(const std::string& name);
	protected:
		Resource(const std::string& name);
		std::string buffer_;
		std::string name_;
		bool serializable_;
	};
}