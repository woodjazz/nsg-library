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
#include "Sound.h"
#include "Audio.h"
#include "ResourceFile.h"
#include "ResourceXMLNode.h"
#include "Util.h"
#include "Path.h"
#include "Check.h"
#ifdef SDL
#include "SDL_mixer.h"
#endif
#include "pugixml.hpp"

namespace NSG
{
    MapAndVector<std::string, Sound> Sound::sounds_;

	Sound::Sound(const std::string& name)
        : Object(name),
          sound_(nullptr),
          channel_(-1)
    {
        OpenAudio();
    }

    Sound::~Sound()
    {
#ifdef SDL
        if(sound_)        
            Mix_FreeChunk(sound_);
#endif        
        CloseAudio();
    }

    PSound Sound::Create(const std::string& name)
    {
        return sounds_.Create(name);
    }

    PSound Sound::GetOrCreate(const std::string& name)
    {
        return sounds_.GetOrCreate(name);
    }

    PSound Sound::Get(const std::string& name)
    {
        return sounds_.Get(name);
    }

    std::vector<PSound> Sound::GetSounds()
    {
        return sounds_.GetObjs();
    }

    void Sound::Set(PResource resource)
    {
        if(resource_ != resource)
        {
            resource_ = resource;
            Invalidate();
        }
    }

    bool Sound::IsValid()
    {
        return resource_ && resource_->IsReady();
    }

    void Sound::AllocateResources()
    {
#ifdef SDL        
        SDL_RWops* assetHandle = SDL_RWFromConstMem(resource_->GetData(), int(resource_->GetBytes()));
        sound_ = Mix_LoadWAV_RW(assetHandle, 1);
		CHECK_CONDITION(sound_, __FILE__, __LINE__);
#endif        
    }

    void Sound::ReleaseResources()
    {
        if (resource_)
            return resource_->Invalidate();
    }

    bool Sound::IsPlaying() const
    {
#ifdef SDL        
        return sound_ && Mix_Playing(channel_) ? true : false;
#else
        return false;
#endif        
    }

    bool Sound::Play(bool loop)
    {
        if (IsReady())
        {
#ifdef SDL
            Stop();
            channel_ = Mix_PlayChannel(-1, sound_, loop ? -1 : 0);
            return true;
#endif
        }

        return false;
    }

    void Sound::Stop()
    {
#ifdef SDL
        if (IsPlaying())
        {
            Mix_HaltChannel(channel_);
            channel_ = -1;
        }
#endif
    }

    void Sound::Pause()
    {
#ifdef SDL
        if (IsPlaying())
        {
            Mix_Pause(channel_);
        }
#endif
    }

    void Sound::Resume()
    {
#ifdef SDL
        if (Mix_Paused(channel_))
        {
            Mix_Resume(channel_);
        }
#endif
    }

    std::vector<PSound> Sound::LoadSounds(PResource resource, const pugi::xml_node& node)
    {
        std::vector<PSound> result;
        pugi::xml_node objs = node.child("Sounds");
        if (objs)
        {
            pugi::xml_node child = objs.child("Sound");
            while (child)
            {
                std::string name = child.attribute("name").as_string();
                auto sound(Sound::GetOrCreate(name));
                std::string resourceName = child.attribute("resource").as_string();
                auto res = Resource::Get(resourceName);
                if (!res)
                {
                    auto newRes = Resource::Create<ResourceXMLNode>(resourceName);
                    newRes->Set(resource, nullptr, "Resources", resourceName);
                    sound->Set(newRes);
                }
                else
                    sound->Set(res);
                result.push_back(sound);
                child = child.next_sibling("Sound");
            }
        }
        return result;
    }

    void Sound::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Sound");
        child.append_attribute("name").set_value(name_.c_str());
        child.append_attribute("resource") = resource_->GetName().c_str();
    }

    void Sound::SaveSounds(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Sounds");
        auto sounds = Sound::GetSounds();
        for (auto& obj : sounds)
            obj->Save(child);
    }

    void Sound::Set(PResourceXMLNode xmlResource)
    {
        if (xmlResource != xmlResource_)
        {
            xmlResource_ = xmlResource;
            Invalidate();
        }
    }
}