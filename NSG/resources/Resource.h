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
#include "AppData.h"
#include "WeakFactory.h"
#include "Util.h"
#include <string>

namespace NSG
{
	class Resource : public WeakFactory<std::string, Resource>, public std::enable_shared_from_this<Resource>, public Object
	{
	public:
		static PResource CreateFrom(PResource resource, const pugi::xml_node& node);
		virtual ~Resource();
		void SetBuffer(const std::string& buffer) { buffer_ = buffer; }
		const char* const GetData() const { return buffer_.c_str(); }
		int GetBytes() const;
        void ReleaseResources() override;
		const std::string& GetBuffer() const { return buffer_; }
		void SaveExternal(pugi::xml_node& node, const Path& path, const Path& outputDir);
		void Save(pugi::xml_node& node);
		const std::string& GetName() const { return name_; }
		void SetSerializable(bool serializable);
		bool IsSerializable() const;
		void SetName(const std::string& name);
		static std::vector<PResource> LoadResources(PResource resource, const pugi::xml_node& node);
        static void SaveResources(pugi::xml_node& node);
		static void SaveResourcesExternally(pugi::xml_node& node, const Path& path, const Path& outputDir);
		Resource(const std::string& name);
	protected:
		std::string buffer_;
		bool serializable_;
	};
}