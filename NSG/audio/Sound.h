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
#include "Util.h"
#include "MapAndVector.h"

struct Mix_Chunk;

namespace NSG
{
    class Sound : public Object
    {
    public:
        static void Clear(); 
        static PSound Create(const std::string& name = GetUniqueName("Sound"));
        static PSound GetOrCreate(const std::string& name = GetUniqueName("Sound"));
        static PSound Get(const std::string& name);
        static std::vector<PSound> GetSounds();
		Sound(const std::string& name);
        ~Sound();
        void Set(PResource resource);
        PResource GetResource() const { return resource_; }
        virtual bool Play(bool loop = false);
		virtual void Stop();
		virtual void Pause();
		virtual void Resume();
        virtual bool IsPlaying() const;
        static std::vector<PSound> LoadSounds(PResource resource, const pugi::xml_node& node);
        static void SaveSounds(pugi::xml_node& node);
        void Set(PResourceXMLNode xmlResource);
    private:
        void Save(pugi::xml_node& node);
        bool IsValid() override;
        void AllocateResources() override;
        void ReleaseResources() override;
        PResource resource_;
        Mix_Chunk* sound_;
        int channel_;
        PResourceXMLNode xmlResource_;
        static MapAndVector<std::string, Sound> sounds_;
    };
}