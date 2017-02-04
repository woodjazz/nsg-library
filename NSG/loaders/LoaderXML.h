/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "StrongFactory.h"
#include "pugixml.hpp"
#include <string>
#include <set>

namespace NSG
{
	class LoaderXML : public Object, public StrongFactory<std::string, LoaderXML>
	{
	public:
		LoaderXML(const std::string& name);
		~LoaderXML();
		void Set(PResource resource);
		SignalEmpty::PSignal Load(PResource resource);
		SignalFloat::PSignal SigProgress() { return signalProgress_; }
		PScene GetScene(int idx) const;
		std::vector<PAnimation> GetAnimations() const { return animations_; }
		const pugi::xml_document& GetDocument() const { return doc_; }
		pugi::xml_node GetNode(const std::string& type, const std::string& name) const;
		PResource GetResource() const { return resource_; }
	private:
		void Load();
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        bool AreReady();
		PResource resource_;
		pugi::xml_document doc_;
		bool loaded_;
		SignalUpdate::PSlot slotUpdate_;
		SignalEmpty::PSignal signalLoaded_;
		SignalFloat::PSignal signalProgress_;
		std::vector<PResource> resources_;
		std::vector<PMesh> meshes_;
		std::vector<PMaterial> materials_;
		std::vector<PShape> shapes_;
		std::vector<PScene> scenes_;
		std::vector<PSkeleton> skeletons_;
		std::vector<PAnimation> animations_;
		std::set<PObject> objects_;
	};
}
