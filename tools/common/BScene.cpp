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
#include "BScene.h"
#include "BGroup.h"
#include "BlenderDefines.h"
#include "UtilConverter.h"
#include "pugixml.hpp"
#include "bMain.h"
#include "Blender.h"
#include "bBlenderFile.h"
#include "ResourceConverter.h"
#include "NSG.h"
#include "poly2tri.h"
#include <cstdio>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#ifdef WIN32
void _cdecl TranslateSEtoCE(unsigned int code, PEXCEPTION_POINTERS pep)
{
    switch (code)
    {
        case EXCEPTION_ACCESS_VIOLATION:
            throw std::runtime_error("EXCEPTION_ACCESS_VIOLATION");
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            throw std::runtime_error("EXCEPTION_INT_DIVIDE_BY_ZERO");
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            throw std::runtime_error("EXCEPTION_FLT_DIVIDE_BY_ZERO");
            break;
        case EXCEPTION_FLT_OVERFLOW:
            throw std::runtime_error("EXCEPTION_FLT_OVERFLOW");
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            throw std::runtime_error("EXCEPTION_FLT_UNDERFLOW");
            break;
        case EXCEPTION_STACK_OVERFLOW:
            throw std::runtime_error("EXCEPTION_STACK_OVERFLOW");
            break;
        default:
            throw std::runtime_error("Windows Exception");
    }
}
#endif

namespace BlenderConverter
{
    using namespace NSG;
    BScene::BScene(const Path& path, const Path& outputDir, bool embedResources)
        : path_(path),
          outputDir_(outputDir),
          embedResources_(embedResources),
          defaultMaterial_(Material::Create("__DefaultBlenderMaterial__"))
    {
        defaultMaterial_->SetRenderPass(RenderPass::PERPIXEL);

        #ifdef WIN32
        _set_se_translator(TranslateSEtoCE);
        #endif
    }

    BScene::~BScene()
    {
    }

    bool BScene::Load()
    {
        try
        {
            bParse::bBlenderFile obj(path_.GetFullAbsoluteFilePath().c_str());
            obj.parse(false);
            if (obj.ok())
            {
                bParse::bMain* data = obj.getMain();
                auto materials = LoadMaterials(data);
                sounds_ = LoadSounds(data);
                CreateScenes(data);
                CreateAnimations(data);
                return true;
            }
        }
        catch (std::exception& e)
        {
            LOGE("EXCEPTION %s", e.what());
        }
        catch (...)
        {
            LOGE("UNKNOWN EXCEPTION");
        }

        return false;
    }

    void BScene::CreateScenes(bParse::bMain* data)
    {
        bParse::bListBasePtr* it = data->getScene();
        auto n = it->size();
        for (int i = 0; i < n; i++)
        {
            armatureLinker_.clear();
            physics_.clear();
            const Blender::Scene* bscene = (const Blender::Scene*)it->at(i);
            auto scene = CreateScene(bscene);
            scenes_.push_back(scene);
            bscenes_.push_back(bscene);
            const Blender::Base* base = (const Blender::Base*)bscene->base.first;
            while (base)
            {
                const Blender::Object* obj = base->object;
                if (!(obj->restrictflag & OB_RESTRICT_RENDER)) // object-level restrictions in OUTLINER
                    ConvertObject(obj, scene, bscene);
                base = base->next;
            }
            ConvertGroups(data, scene, bscene);
            ConvertGroupInstances(data, scene, bscene);
            for (auto& objArmature : armatureLinker_)
                CreateSkeleton(scene, objArmature);
            for (auto& physic : physics_)
                LoadPhysics(scene, physic);
        }
    }

    PScene BScene::CreateScene(const Blender::Scene* bscene)
    {
        auto scene = std::make_shared<Scene>(B_IDNAME(bscene));
        scene->SetOrientation(glm::angleAxis<float>(-PI / 2.f, Vertex3(1, 0, 0)));
        const Blender::World* world = bscene->world;
        if (world)
        {
            scene->GetPhysicsWorld()->SetGravity(Vector3(0, -world->gravity, 0));
            Color ambient(world->ambr, world->ambg, world->ambb, 1);
            scene->SetAmbientColor(ambient);
            Color horizon(world->horr, world->horg, world->horb, 1);
            scene->SetHorizonColor(horizon);
        }
        return scene;
    }

    std::vector<NSG::PSound> BScene::LoadSounds(bParse::bMain* data)
    {
        std::vector<PSound> result;
        bParse::bListBasePtr* it = data->getSound();
        int n = it->size();
        for (int i = 0; i < n; i++)
        {
            auto sound = (Blender::bSound*)it->at(i);
            result.push_back(LoadSound(sound));
        }
        return result;
    }

    NSG::PSound BScene::LoadSound(Blender::bSound* sound)
    {
        std::string soundName = B_IDNAME(sound);
        if (!sound->packedfile)
        {
            Path path;
            path.SetPath(path_.GetPath());
            path.SetFileName(sound->name);
            auto resource = Resource::GetOrCreateClass<ResourceFile>(path.GetFilePath());
            auto sound = Sound::Create(soundName);
            sound->Set(resource);
            return sound;
        }
        else
        {
            auto resource = Resource::GetOrCreateClass<ResourceConverter>(sound->name);
            resource->SetData((const char*)sound->packedfile->data, sound->packedfile->size);
            auto sound = Sound::Create(soundName);
            sound->Set(resource);
            return sound;
        }
    }

    std::vector<NSG::PMaterial> BScene::LoadMaterials(bParse::bMain* data)
    {
        std::vector<PMaterial> result;
        bParse::bListBasePtr* it = data->getMat();
        int n = it->size();
        for (int i = 0; i < n; i++)
        {
            auto material = (Blender::Material*)it->at(i);
            result.push_back(LoadMaterial(material));
        }
        return result;
    }

    PTexture BScene::CreateTexture(const Blender::Image* ima, const std::string& imageName)
    {
        CHECK_ASSERT(!imageName.empty() && imageName != "", __FILE__, __LINE__);
        if (!ima->packedfile)
        {
            Path path;
            path.SetPath(path_.GetPath());
            path.SetFileName(imageName);
            auto resource = Resource::GetOrCreateClass<ResourceFile>(path.GetFilePath());
            return std::make_shared<Texture2D>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
        }
        else
        {
            auto resource = Resource::GetOrCreateClass<ResourceConverter>(imageName);
            resource->SetData((const char*)ima->packedfile->data, ima->packedfile->size);
            return std::make_shared<Texture2D>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
        }
    }

    PMaterial BScene::LoadMaterial(const Blender::Material* mt)
    {
        std::string name = B_IDNAME(mt);
        auto material = Material::GetOrCreate(name);

        float alpha = 1.f; //full opaque
        float alphaForSpecular = 1.f;
        const int TRANSPARENCY = 0x10000;
        bool transparent = mt->mode & TRANSPARENCY ? true : false;
        if (transparent)
        {
            alpha = mt->alpha;
            alphaForSpecular = mt->spectra;
        }

        ColorRGB diffuseColor(mt->r, mt->g, mt->b);
        material->SetDiffuseColor(diffuseColor);
        material->SetAlpha(alpha);
        auto diffuseIntensity = mt->ref;
        material->SetDiffuseIntensity(diffuseIntensity);

        ColorRGB specularColor(mt->specr, mt->specg, mt->specb);
        material->SetSpecularColor(specularColor);
        material->SetAlphaForSpecular(alphaForSpecular);
        auto specularIntensity = mt->spec;
        material->SetSpecularIntensity(specularIntensity);

        auto ambientIntensity = mt->amb;
        material->SetAmbientIntensity(ambientIntensity);
        material->SetShininess(mt->har);

        material->EnableTransparent(transparent);//mt->game.alpha_blend & GEMAT_ALPHA)

        if (mt->mode & MA_WIRE)
            material->SetFillMode(FillMode::WIREFRAME);
        else
            material->SetFillMode(FillMode::SOLID);

        bool shadeless = mt->mode & MA_SHLESS ? true : false;
        material->SetShadeless(shadeless);
        bool receiveShadow = mt->mode & MA_SHADOW ? true : false;
        material->ReceiveShadows(receiveShadow);
        bool castShadow = mt->mode2 & 1 ? true : false;
        material->CastShadow(castShadow);

        if (mt->game.flag & GEMAT_BACKCULL && !transparent)
            material->SetCullFaceMode(CullFaceMode::BACK);
        else if (mt->game.flag & GEMAT_INVISIBLE && !transparent)
            material->SetCullFaceMode(CullFaceMode::FRONT_AND_BACK);
        else
            material->SetCullFaceMode(CullFaceMode::DISABLED);

        if (mt->game.flag & GEMAT_TEXT)
            material->SetRenderPass(RenderPass::TEXT);
        else if (mt->mode & MA_VERTEXCOLP)
            material->SetRenderPass(RenderPass::VERTEXCOLOR);
        else if (shadeless)
            material->SetRenderPass(RenderPass::UNLIT);
        else
            material->SetRenderPass(RenderPass::PERPIXEL);

        if (mt->game.face_orientation & GEMAT_HALO)
            material->SetBillboardType(BillboardType::SPHERICAL);
        else if (mt->game.face_orientation & GEMAT_BILLBOARD)
            material->SetBillboardType(BillboardType::CYLINDRICAL);
        else
            material->SetBillboardType(BillboardType::NONE);

        // textures
        for (int i = 0; i < MAX_MTEX; i++)
        {
            int disabled = mt->septex & (1 << i);

            if (disabled || !mt->mtex[i] || !mt->mtex[i]->tex)
                continue;

            if (mt->mtex[i]->tex->type == TEX_IMAGE)
            {
                const Blender::MTex* mtex = mt->mtex[i];
                const Blender::Image* ima = mtex->tex->ima;
                if (!ima) continue;
                std::string imageName = B_IDNAME(ima);
                if (imageName.empty() || imageName == "")
                    continue;
                auto texture = CreateTexture(ima, imageName);
                if (mtex->uvname)
                    texture->SetUVName(mtex->uvname);

                switch (mtex->blendtype)
                {
                    case MTEX_BLEND:
                        texture->SetBlendType(TextureBlend::MIX);
                        break;
                    case MTEX_MUL:
                        texture->SetBlendType(TextureBlend::MUL);
                        break;
                    case MTEX_ADD:
                        texture->SetBlendType(TextureBlend::ADD);
                        break;
                    case MTEX_SUB:
                        texture->SetBlendType(TextureBlend::SUB);
                        break;
                    case MTEX_DIV:
                        texture->SetBlendType(TextureBlend::DIV);
                        break;
                    case MTEX_DARK:
                        texture->SetBlendType(TextureBlend::DARK);
                        break;
                    case MTEX_DIFF:
                        texture->SetBlendType(TextureBlend::DIFF);
                        break;
                    case MTEX_LIGHT:
                        texture->SetBlendType(TextureBlend::LIGHT);
                        break;
                    case MTEX_SCREEN:
                        texture->SetBlendType(TextureBlend::SCREEN);
                        break;
                    case MTEX_OVERLAY:
                        texture->SetBlendType(TextureBlend::OVERLAY);
                        break;
                    case MTEX_BLEND_HUE:
                        texture->SetBlendType(TextureBlend::BLEND_HUE);
                        break;
                    case MTEX_BLEND_SAT:
                        texture->SetBlendType(TextureBlend::BLEND_SAT);
                        break;
                    case MTEX_BLEND_VAL:
                        texture->SetBlendType(TextureBlend::BLEND_VAL);
                        break;
                    case MTEX_BLEND_COLOR:
                        texture->SetBlendType(TextureBlend::BLEND_COLOR);
                        break;
                    default:
                        break;
                }

                if ((mtex->mapto & MAP_EMIT) || (mtex->maptoneg & MAP_EMIT))
                {
                    texture->SetMapType(TextureType::EMIT);
                    material->SetTexture(texture);
                }
                else if ((mtex->mapto & MAP_NORM) || (mtex->maptoneg & MAP_NORM))
                {
                    texture->SetMapType(TextureType::NORM);
                    material->SetTexture(texture);
                }
                else if ((mtex->mapto & MAP_SPEC) || (mtex->maptoneg & MAP_SPEC))
                {
                    texture->SetMapType(TextureType::SPEC);
                    material->SetTexture(texture);
                }
                else if ((mtex->mapto & MAP_AMB) || (mtex->maptoneg & MAP_AMB))
                {
                    texture->SetMapType(TextureType::AMB);
                    material->SetTexture(texture);
                }
                else if ((mtex->mapto & MAP_COL) || (mtex->maptoneg & MAP_COL))
                {
                    texture->SetMapType(TextureType::COL);
                    material->SetTexture(texture);
                }
            }
        }

        return material;
    }

    void BScene::ConvertGroups(bParse::bMain* data, PScene scene, const Blender::Scene* bscene)
    {
        bParse::bListBasePtr* it = data->getGroup();
        int n = it->size();
        for (int i = 0; i < n; i++)
        {
            auto bgroup = (Blender::Group*)it->at(i);
            auto groupName = B_IDNAME(bgroup);
            auto group = std::make_shared<BGroup>(groupName);
            auto result = groups_.insert(Groups::value_type(groupName, group));
            if (!result.second)
            {
                LOGW("Insert for group=%s failed!!!\n", groupName);
                continue;
            }
            auto bgobj = (Blender::GroupObject*)bgroup->gobject.first;
            while (bgobj)
            {
                Blender::Object* bobj = bgobj->ob;
                if (bobj)
                {
                    auto objName = B_IDNAME(bobj);
                    // is object a group-instance?
                    if ((bobj->transflag & OB_DUPLIGROUP) && bobj->dup_group != 0)
                    {
                        auto groupNode = ConvertObject(bobj, scene, bscene);
                        // Owning group
                        Blender::Group* bgobj = bobj->dup_group;
                        std::string instGroupName(B_IDNAME(bgobj));
                        if (groupNode)
                            group->AddGroup(instGroupName, groupNode);
                    }
                    else
                    {
                        auto sceneNode = scene->GetChild<SceneNode>(objName, true);
                        if (sceneNode)
                            group->AddSceneNode(sceneNode);
                    }
                }
                bgobj = bgobj->next;
            }
        }
    }

    void BScene::ConvertGroupInstances(const std::string& groupName, PSceneNode parent)
    {
        auto it = groups_.find(groupName);
        if (it != groups_.end())
        {
            PBGroup bgroup = it->second;
            bgroup->CreateObjects(parent);
            auto& groupInstances = bgroup->GetGroupInstances();
            for (auto& groupInstance : groupInstances)
                ConvertGroupInstances(groupInstance.name, groupInstance.node);
        }

    }

    void BScene::ConvertGroupInstances(bParse::bMain* data, PScene scene, const Blender::Scene* bscene)
    {
        std::vector<const Blender::Object*> groups;
        const Blender::Base* base = (const Blender::Base*)bscene->base.first;
        while (base)
        {
            const Blender::Object* bobj = base->object;
            if (!(bobj->restrictflag & OB_RESTRICT_RENDER)) // object-level restrictions in OUTLINER
            {
                if ((bobj->transflag & OB_DUPLIGROUP) && bobj->dup_group != 0)
                    groups.push_back(bobj);
            }
            base = base->next;
        }

        for (auto& group : groups)
        {
            std::string nodeName = B_IDNAME(group);
            auto instancedGroup = scene->GetChild<SceneNode>(nodeName, true);
            if (!instancedGroup)
                continue;
            const Blender::Group* bgobj = group->dup_group; // Owning group
            std::string groupName(B_IDNAME(bgobj));
            ConvertGroupInstances(groupName, instancedGroup);
        }
    }

    bool BScene::IsKnownType(const Blender::Object* obj) const
    {
        switch (obj->type)
        {
            case OB_EMPTY:
                return true;
            case OB_LAMP:
                return true;
            case OB_CAMERA:
                return true;
            case OB_MESH:
                return true;
            case OB_ARMATURE:
                return true;
            case OB_CURVE:
                return true;
            default:
                break;
        }
        return false;
    }

    PSceneNode BScene::ConvertObject(const Blender::Object* obj, PScene scene, const Blender::Scene* bscene)
    {
        PSceneNode parent = scene;
        PSceneNode sceneNode;
        if (obj && IsKnownType(obj))
        {
            if (obj->type == OB_MESH && obj->parent && obj->parent->type == OB_ARMATURE)
                armatureLinker_.push_back(obj);
            else if (obj->body_type != OB_BODY_TYPE_NO_COLLISION)
                physics_.push_back(obj);

            if (obj->type >= 0 && obj->parent)
            {
                auto parentName = B_IDNAME(obj->parent);
                parent = scene->GetChild<SceneNode>(parentName, true);
                if (!parent)
                    parent = scene;
            }

            switch (obj->type)
            {
                case OB_EMPTY:
                    sceneNode = CreateSceneNode(obj, parent);
                    break;
                case OB_LAMP:
                    sceneNode = CreateLight(obj, parent);
                    break;
                case OB_CAMERA:
                    sceneNode = CreateCamera(obj, parent, bscene);
                    break;
                case OB_MESH:
                    sceneNode = CreateMesh(obj, parent);
                    break;
                case OB_ARMATURE:   // SceneNode + Skeleton
                    sceneNode = CreateSkeletonBones(obj, parent);
                    break;
                case OB_CURVE:
                    break;
                default:
                    break;
            }
        }

        return sceneNode;
    }

    void BScene::GetFrames(const Blender::bAction* action, std::vector<float>& fra)
    {
        const Blender::FCurve* fcu = (const Blender::FCurve*)action->curves.first;
        for (; fcu; fcu = fcu->next)
        {
            for (int i = 0; i < fcu->totvert; i++)
            {
                float f = fcu->bezt[i].vec[1][0];
                if (std::find(fra.begin(), fra.end(), f) == fra.end())
                    fra.push_back(f);
            }
        }
        std::sort(fra.begin(), fra.end()); // keep the keys in ascending order
    }

    float BScene::GetTracks(const Blender::bAction* action, float animfps, BTracks& tracks)
    {
        std::string name(B_IDNAME(action));
        float start, end;
        GetActionStartEnd(action, start, end);
        float trackLength = (end - start) / animfps;

        const Blender::FCurve* bfc = (const Blender::FCurve*)action->curves.first;

        while (bfc)
        {
            PTrackData trackData;
            std::string rnap(bfc->rna_path);
            std::string chan_name;
            std::string transform_name;

            // Pose action
            if (rnap.substr(0, 10) == "pose.bones")
            {
                size_t i = rnap.rfind('\"');
                chan_name = rnap.substr(12, i - 12);
                transform_name = rnap.substr(i + 3, rnap.length() - i + 3);
            }
            else
            {
                transform_name = rnap;
                chan_name = "NSGMainObjectChannel";
            }

            auto it = tracks.find(chan_name);
            if (it == tracks.end())
                trackData = tracks[chan_name] = std::make_shared<TrackData>();
            else
                trackData = it->second;

            if (bfc->bezt)
            {
                SPLINE_CHANNEL_CODE code = SPLINE_CHANNEL_CODE::NONE;
                if (transform_name == "rotation_quaternion")
                {
                    if (bfc->array_index == 0) code = SC_ROT_QUAT_W;
                    else if (bfc->array_index == 1) code = SC_ROT_QUAT_X;
                    else if (bfc->array_index == 2) code = SC_ROT_QUAT_Y;
                    else if (bfc->array_index == 3) code = SC_ROT_QUAT_Z;
                }
                else if (transform_name == "rotation_euler")// && obj->rotmode == ROT_MODE_EUL)
                {
                    if (bfc->array_index == 0) code = SC_ROT_EULER_X;
                    else if (bfc->array_index == 1) code = SC_ROT_EULER_Y;
                    else if (bfc->array_index == 2) code = SC_ROT_EULER_Z;
                }
                else if (transform_name == "location")
                {
                    if (bfc->array_index == 0) code = SC_LOC_X;
                    else if (bfc->array_index == 1) code = SC_LOC_Y;
                    else if (bfc->array_index == 2) code = SC_LOC_Z;
                }
                else if (transform_name == "scale")
                {
                    if (bfc->array_index == 0) code = SC_SCL_X;
                    else if (bfc->array_index == 1) code = SC_SCL_Y;
                    else if (bfc->array_index == 2) code = SC_SCL_Z;
                }

                // ignore any other codes
                if (code != SPLINE_CHANNEL_CODE::NONE && bfc->totvert > 0)
                {
                    auto spline = ConvertSpline(bfc->bezt, code, bfc->bezt->ipo, bfc->totvert, -start, 1.0f / animfps, 0, 1);
                    trackData->keyframes.push_back(spline);
                }
            }

            if (bfc->next == 0 || bfc->next->prev != bfc)
                break;

            bfc = bfc->next;
        }

        return trackLength;
    }

    void BScene::CreateAnimations(bParse::bMain* data)
    {
        CHECK_ASSERT(scenes_.size(), __FILE__, __LINE__);
        CHECK_ASSERT(scenes_.size() == bscenes_.size(), __FILE__, __LINE__);
        const Blender::Scene* firstBScene(bscenes_.at(0));
        PScene firstScene(scenes_.at(0));
        //float animfps = firstBScene->r.frs_sec / firstBScene->r.frs_sec_base;
        bParse::bListBasePtr* it = data->getAction();
        auto n = it->size();
        for (int i = 0; i < n; i++)
        {
            const Blender::bAction* action = (const Blender::bAction*)it->at(i);
            CreateAnimation(action, firstBScene, firstScene);
        }
    }

    void BScene::CreateAnimation(const Blender::bAction* action, const Blender::Scene* bscene, PScene scene)
    {
        float animfps = bscene->r.frs_sec / bscene->r.frs_sec_base;
        std::string name(B_IDNAME(action));
        if (!scene->HasAnimation(name))
        {
            auto anim = scene->GetOrCreateAnimation(name);
            BTracks tracks;
            auto length = GetTracks(action, animfps, tracks);
            ConvertTracks(scene, action, anim, tracks, length);
            anim->SetLength(length);
        }
    }

    void BScene::ConvertTracks(PScene scene, const Blender::bAction* action, PAnimation anim, BTracks& tracks, float length)
    {
        for (auto& btrack : tracks)
        {
            std::string channelName = btrack.first;
            AnimationTrack track;
            if (scene->GetName() == channelName)
                track.node_ = scene;
            else
                track.node_ = scene->GetChild<Node>(channelName, true);

            if (!track.node_.lock())
            {
                LOGW("Skipping animation track %s whose scene node was not found", channelName.c_str());
                continue;
            }

            track.channelMask_ = ConvertChannel(action, btrack.second, track, length);
            anim->AddTrack(track);
        }
    }

    AnimationChannelMask BScene::ConvertChannel(const Blender::bAction* action, PTrackData trackData, AnimationTrack& track, float timeFrameLength)
    {
        AnimationChannelMask mask = 0;
        //float inc = timeFrameLength;

        //if (trackData->keyframes.size())
        //    inc = timeFrameLength / trackData->keyframes[0]->getNumVerts();

        float start, end;
        GetActionStartEnd(action, start, end);
        float totalFramesLength = end - start;

        std::vector<float> frames;
        GetFrames(action, frames);
        std::vector<float> framesTime;
        for (auto& f : frames)
        {
            float t = timeFrameLength * (f / totalFramesLength);
            framesTime.push_back(t);
        }
        if (framesTime.size())
            framesTime[0] = 0;
        if (framesTime.size() > 1)
            framesTime[framesTime.size() - 1] = timeFrameLength;

        for (auto t : framesTime)
        {
            if (std::isnan(t))
                continue;
            float delta = t / timeFrameLength;
            AnimationKeyFrame keyframe;
            keyframe.time_ = t;
            Vector3 pos;
            Quaternion q;
            Vector3 scale(1);
            Vector3 eulerAngles;

            for (auto& spline : trackData->keyframes)
            {
                if (!spline)
                    continue;

                SPLINE_CHANNEL_CODE code = spline->GetCode();

                switch (code)
                {
                    case SC_ROT_QUAT_W:
                        q.w = spline->interpolate(delta, t);
                        break;
                    case SC_ROT_QUAT_X:
                        q.x = spline->interpolate(delta, t);
                        break;
                    case SC_ROT_QUAT_Y:
                        q.y = spline->interpolate(delta, t);
                        break;
                    case SC_ROT_QUAT_Z:
                        q.z = spline->interpolate(delta, t);
                        break;
                    case SC_ROT_EULER_X:
                        eulerAngles.x = spline->interpolate(delta, t);
                        break;
                    case SC_ROT_EULER_Y:
                        eulerAngles.y = spline->interpolate(delta, t);
                        break;
                    case SC_ROT_EULER_Z:
                        eulerAngles.z = spline->interpolate(delta, t);
                        break;
                    case SC_LOC_X:
                        pos.x = spline->interpolate(delta, t);
                        break;
                    case SC_LOC_Y:
                        pos.y = spline->interpolate(delta, t);
                        break;
                    case SC_LOC_Z:
                        pos.z = spline->interpolate(delta, t);
                        break;
                    case SC_SCL_X:
                        scale.x = spline->interpolate(delta, t);
                        break;
                    case SC_SCL_Y:
                        scale.y = spline->interpolate(delta, t);
                        break;
                    case SC_SCL_Z:
                        scale.z = spline->interpolate(delta, t);
                        break;
                    default:
                        break;
                }
            }

            if (glm::length2(eulerAngles))
            {
                CHECK_ASSERT(q == QUATERNION_IDENTITY, __FILE__, __LINE__);
                q = Quaternion(eulerAngles);
            }
            q = glm::normalize(q);
            scale = glm::normalize(scale);

            Matrix4 transform = glm::translate(glm::mat4(), pos) * glm::mat4_cast(q) * glm::scale(glm::mat4(1.0f), scale);
            Matrix4 totalTransform = track.node_.lock()->GetTransform() * transform;
            DecomposeMatrix(totalTransform, keyframe.position_, keyframe.rotation_, keyframe.scale_);
            track.keyFrames_.push_back(keyframe);
        }

        for (auto& kf : track.keyFrames_)
        {
            if (glm::epsilonNotEqual(kf.position_, VECTOR3_ZERO, Vector3(0.0001f)) != glm::bvec3(false))
                mask |= (int)AnimationChannel::POSITION;

            if (glm::epsilonNotEqual(kf.scale_, VECTOR3_ONE, Vector3(0.0001f)) != glm::bvec3(false))
                mask |= (int)AnimationChannel::POSITION;

            Vector3 angles = glm::eulerAngles(kf.rotation_);
            if (glm::epsilonNotEqual(angles, VECTOR3_ZERO, Vector3(0.001f)) != glm::bvec3(false))
                mask |= (int)AnimationChannel::ROTATION;
        }

        return mask;
    }

    PBSpline BScene::ConvertSpline(const Blender::BezTriple* bez, SPLINE_CHANNEL_CODE access, int mode, int totvert, float xoffset, float xfactor, float yoffset, float yfactor)
    {
        auto spline = std::make_shared<BSpline>(access);

        switch (mode)
        {
            case 0://BEZT_IPO_CONST:
                spline->setInterpolationMethod(BSpline::BEZ_CONSTANT);
                break;
            case 1://BEZT_IPO_LIN:
                spline->setInterpolationMethod(BSpline::BEZ_LINEAR);
                break;
            case 2://BEZT_IPO_BEZ:
                spline->setInterpolationMethod(BSpline::BEZ_CUBIC);
                break;
            default:
                return nullptr;
        }

        const Blender::BezTriple* bezt = bez;
        for (int c = 0; c < totvert; c++, bezt++)
        {
            BezierVertex v;

            v.h1[0] = (bezt->vec[0][0] + xoffset) * xfactor;
            v.h1[1] = (bezt->vec[0][1] + yoffset) * yfactor;
            v.cp[0] = (bezt->vec[1][0] + xoffset) * xfactor;
            v.cp[1] = (bezt->vec[1][1] + yoffset) * yfactor;
            v.h2[0] = (bezt->vec[2][0] + xoffset) * xfactor;
            v.h2[1] = (bezt->vec[2][1] + yoffset) * yfactor;

            spline->addVertex(v);
        }

        return spline;
    }


    void BScene::GetActionStartEnd(const Blender::bAction* action, float& start, float& end)
    {
        start = std::numeric_limits<float>::max();
        end = -start;
        float tstart, tend;
        Blender::FCurve* bfc = (Blender::FCurve*)action->curves.first;
        while (bfc)
        {
            GetSplineStartEnd(bfc->bezt, bfc->totvert, tstart, tend);
            if (start > tstart) start = tstart;
            if (end < tend) end = tend;

            if (bfc->next == 0 || bfc->next->prev != bfc)
                break; //FIX: Momo_WalkBack fcurve is broken in uncompressed 256a.

            bfc = bfc->next;
        }
    }

    void BScene::GetSplineStartEnd(const Blender::BezTriple* bez, int totvert, float& start, float& end)
    {
        start = std::numeric_limits<float>::max();
        end = -start;
        const Blender::BezTriple* bezt = bez;
        for (int c = 0; c < totvert; c++, bezt++)
        {
            if (start > bezt->vec[1][0])
                start = bezt->vec[1][0];
            if (end < bezt->vec[1][0])
                end = bezt->vec[1][0];
        }
    }

    void BScene::ExtractGeneral(const Blender::Object* obj, PSceneNode sceneNode)
    {
        Quaternion q(Vector3(obj->rot[0], obj->rot[1], obj->rot[2]));
        Vector3 pos(obj->loc[0], obj->loc[1], obj->loc[2]);
        Vector3 scale(obj->size[0], obj->size[1], obj->size[2]);

        if (sceneNode->GetParent()->IsArmature())
        {
            Quaternion parent_q;
            Vector3 parent_pos;
            Vector3 parent_scale;

            auto parent = sceneNode->GetParent();
            Matrix4 parentinv = glm::translate(glm::mat4(), parent->GetPosition()) * glm::mat4_cast(parent->GetOrientation()) * glm::scale(glm::mat4(1.0f), parent->GetScale());
            parentinv = glm::inverse(parentinv);
            DecomposeMatrix(parentinv, parent_pos, parent_q, parent_scale);
            pos = parent_pos + parent_q * (parent_scale * pos);
            q = parent_q * q;
            scale = parent_scale * scale;
        }

        sceneNode->SetPosition(pos);
        sceneNode->SetOrientation(q);
        sceneNode->SetScale(scale);

    }

    PhysicsShape BScene::GetShapeType(short boundtype) const
    {
        PhysicsShape shapeType = PhysicsShape::SH_EMPTY;

        switch (boundtype)
        {
            case OB_BOUND_BOX:
                shapeType = PhysicsShape::SH_BOX;
                break;
            case OB_BOUND_SPHERE:
                shapeType = PhysicsShape::SH_SPHERE;
                break;
            case OB_BOUND_CONE:
                shapeType = PhysicsShape::SH_CONE;
                break;
            case OB_BOUND_CYLINDER:
                shapeType = PhysicsShape::SH_CYLINDER;
                break;
            case OB_BOUND_CONVEX_HULL:
                shapeType = PhysicsShape::SH_CONVEX_TRIMESH;
                break;
            case OB_BOUND_TRIANGLE_MESH:
                shapeType = PhysicsShape::SH_TRIMESH;
                break;
            case OB_BOUND_CAPSULE:
                shapeType = PhysicsShape::SH_CAPSULE;
                break;
            default:
                shapeType = PhysicsShape::SH_EMPTY;
                break;
        }

        return shapeType;
    }

    PhysicsBody BScene::GetBodyType(char bodyType) const
    {
        PhysicsBody type = PhysicsBody::BODY_UNKNOWN;

        switch (bodyType)
        {
            case OB_BODY_TYPE_NO_COLLISION:
                type = PhysicsBody::BODY_NO_COLLISION;
                break;
            case OB_BODY_TYPE_STATIC:
                type = PhysicsBody::BODY_STATIC;
                break;
            case OB_BODY_TYPE_DYNAMIC:
                type = PhysicsBody::BODY_DYNAMIC;
                break;
            case OB_BODY_TYPE_RIGID:
                type = PhysicsBody::BODY_RIGID;
                break;
            case OB_BODY_TYPE_SOFT:
                type = PhysicsBody::BODY_SOFT;
                break;
            case OB_BODY_TYPE_OCCLUDER:
                type = PhysicsBody::BODY_OCCLUDER;
                break;
            case OB_BODY_TYPE_SENSOR:
                type = PhysicsBody::BODY_SENSOR;
                break;
            case OB_BODY_TYPE_NAVMESH:
                type = PhysicsBody::BODY_NAVMESH;
                break;
            case OB_BODY_TYPE_CHARACTER:
                type = PhysicsBody::BODY_CHARACTER;
                break;
            default:
                type = PhysicsBody::BODY_UNKNOWN;
                break;
        }

        return type;
    }

    bool BScene::IsRigidBody(PhysicsBody bodyType) const
    {
        switch (bodyType)
        {
            case PhysicsBody::BODY_STATIC:
            case PhysicsBody::BODY_DYNAMIC:
            case PhysicsBody::BODY_RIGID:
            case PhysicsBody::BODY_SENSOR:
            case PhysicsBody::BODY_CHARACTER:
                return true;
        }
        return false;
    }

    bool BScene::ShapeNeedsMesh(PhysicsShape shapeType) const
    {
        return shapeType == PhysicsShape::SH_TRIMESH || shapeType == PhysicsShape::SH_CONVEX_TRIMESH;
    }

    PSceneNode BScene::GetParentWithRigidBody(PSceneNode sceneNode) const
    {
        auto parent = sceneNode->GetParent();
        while (parent)
        {
            auto parentSceneNode = std::dynamic_pointer_cast<SceneNode>(parent);
            auto rigidBody = parentSceneNode->GetRigidBody();
            if (rigidBody)
                return parentSceneNode;
            parent = parent->GetParent();
        }
        return nullptr;
    }

    void BScene::LoadPhysics(PScene scene, const Blender::Object* obj)
    {
        std::string name = B_IDNAME(obj);
        auto sceneNode = scene->GetChild<SceneNode>(name, true);
        if (!sceneNode)
        {
            LOGE("Cannot find scene node with name %s", name.c_str());
            return;
        }

        PMesh mesh = sceneNode->GetMesh();
        PRigidBody rigBody;

        PhysicsBody bodyType = GetBodyType(obj->body_type);
        if (IsRigidBody(bodyType))
        {
            CHECK_ASSERT(obj->type != OB_MESH || mesh, __FILE__, __LINE__);

            //const Blender::RigidBodyOb* rigidbody_object = obj->rigidbody_object;
            //const Blender::RigidBodyCon* rigidbody_constraint = obj->rigidbody_constraint;

            rigBody = sceneNode->GetOrCreateRigidBody();
            rigBody->SetLinearDamp(obj->damping);
            rigBody->SetAngularDamp(obj->rdamping);

            rigBody->SetMass(bodyType == PhysicsBody::BODY_STATIC ? 0 : obj->mass);
            rigBody->SetKinematic(bodyType == PhysicsBody::BODY_DYNAMIC || bodyType == PhysicsBody::BODY_CHARACTER);
            rigBody->SetTrigger(bodyType == PhysicsBody::BODY_SENSOR);

            const Blender::Material* ma = GetMaterial(obj, 0);
            if (ma)
            {
                rigBody->SetRestitution(ma->reflect);
                rigBody->SetFriction(ma->friction);
            }

            Vector3 linearFactor(1);
            if (obj->gameflag2 & OB_LOCK_RIGID_BODY_X_AXIS)
                linearFactor.x = 0;
            if (obj->gameflag2 & OB_LOCK_RIGID_BODY_Y_AXIS)
                linearFactor.y = 0;
            if (obj->gameflag2 & OB_LOCK_RIGID_BODY_Z_AXIS)
                linearFactor.z = 0;

            rigBody->SetLinearFactor(linearFactor);

            Vector3 angularFactor(1);
            if (obj->gameflag2 & OB_LOCK_RIGID_BODY_X_ROT_AXIS)
                angularFactor.x = 0;
            if (obj->gameflag2 & OB_LOCK_RIGID_BODY_Y_ROT_AXIS)
                angularFactor.y = 0;
            if (obj->gameflag2 & OB_LOCK_RIGID_BODY_Z_ROT_AXIS)
                angularFactor.z = 0;

            rigBody->SetAngularFactor(angularFactor);
        }

        PhysicsShape shapeType = GetShapeType(obj->collision_boundtype);
        bool needsMesh = ShapeNeedsMesh(shapeType);
        if (needsMesh && !mesh)
        {
            LOGE("Cannot create physics shape without mesh");
            return;
        }

        if (obj->gameflag & OB_COLLISION)
        {
            Vector3 offsetPos;
            Quaternion offsetRot;

            if (!rigBody)
            {
                auto parent = GetParentWithRigidBody(sceneNode);
                if (parent)
                {
                    rigBody = parent->GetRigidBody();
                    auto m = parent->GetGlobalModelInvMatrix() * sceneNode->GetGlobalModelMatrix();
                    Vector3 scale;
                    DecomposeMatrix(m, offsetPos, offsetRot, scale);
                }
            }

            if (!rigBody)
            {
                LOGE("Cannot create shape for obj %s because rigidbody has not been found", name.c_str());
                return;
            }

            auto scale = sceneNode->GetGlobalScale();
            PShape shape = Shape::GetOrCreate(needsMesh ? ShapeKey(mesh, scale) : ShapeKey(shapeType, scale));

            BoundingBox bb;
            if (needsMesh)
                bb = mesh->GetBB();
            else            {
                //LOGW("Cannot calculate physics bounding box.");
                bb = BoundingBox(-scale, scale);
            }
            CHECK_ASSERT(bb.IsDefined(), __FILE__, __LINE__);
            shape->SetBB(bb);

            shape->SetMargin(obj->margin);
            rigBody->AddShape(shape, offsetPos, offsetRot);
        }
    }

    PSceneNode BScene::CreateSceneNode(const Blender::Object* obj, PSceneNode parent)
    {
        auto sceneNode = parent->CreateChild<SceneNode>(B_IDNAME(obj));
        ExtractGeneral(obj, sceneNode);
        return sceneNode;
    }

    PSceneNode BScene::CreateSkeletonBones(const Blender::Object* obj, PSceneNode parent)
    {
        auto sceneNode = CreateSceneNode(obj, parent);
        sceneNode->SetAsArmature(true);
        const Blender::bArmature* ar = static_cast<const Blender::bArmature*>(obj->data);
        std::string armatureName = B_IDNAME(ar);
        if (!(ar->flag & ARM_RESTPOS))
        {
            LOGE("Cannot create skeleton: %s. Armature has to be in rest position. Go to blender and change it.", armatureName.c_str());
            return nullptr;
        }
        armatureBones_.clear();
        // create bone lists && transforms
        const Blender::Bone* bone = static_cast<const Blender::Bone*>(ar->bonebase.first);
        while (bone)
        {
            if (!bone->parent)
                BuildBoneTree(armatureName, bone, sceneNode);
            bone = bone->next;
        }

        return sceneNode;
    }

    void BScene::BuildBoneTree(const std::string& armatureName, const Blender::Bone* cur, PSceneNode parent)
    {
        auto& list = armatureBones_[armatureName];

        auto it = std::find_if(list.begin(), list.end(), [&](PWeakNode node)
        {
            auto p = node.lock();
            return p == parent;
        });

        if (it == list.end())
            list.push_back(parent);

        Matrix4 parBind = IDENTITY_MATRIX;
        if (cur->parent)
            parBind = glm::inverse(ToMatrix(cur->parent->arm_mat));
        else
        {
            CHECK_ASSERT(parent->IsArmature(), __FILE__, __LINE__);
            parBind = glm::translate(glm::mat4(), parent->GetPosition()) * glm::mat4_cast(parent->GetOrientation()) * glm::scale(glm::mat4(1.0f), parent->GetScale());
            parBind = glm::inverse(parBind);
        }

        CHECK_ASSERT(!parent->GetChild<SceneNode>(cur->name, false), __FILE__, __LINE__);
        PSceneNode bone = parent->CreateChild<SceneNode>(cur->name);

        Matrix4 bind = parBind * ToMatrix(cur->arm_mat);

        Quaternion rot; Vector3 loc, scl;
        DecomposeMatrix(bind, loc, rot, scl);

        bone->SetPosition(loc);
        bone->SetOrientation(rot);
        bone->SetScale(scl);

        Blender::Bone* chi = static_cast<Blender::Bone*>(cur->childbase.first);
        while (chi)
        {
            BuildBoneTree(armatureName, chi, bone);
            chi = chi->next;
        }
    }

    PSceneNode BScene::CreateCamera(const Blender::Object* obj, PSceneNode parent, const Blender::Scene* bscene)
    {
        CHECK_ASSERT(obj->data, __FILE__, __LINE__);
        auto camera = parent->CreateChild<Camera>(B_IDNAME(obj));
        ExtractGeneral(obj, camera);
        Blender::Camera* bcamera = static_cast<Blender::Camera*>(obj->data);

        if (bscene->camera == obj)
            camera->GetScene()->SetMainCamera(camera);

        if (bcamera->type == CAM_ORTHO)
            camera->EnableOrtho();

        float fov = 2.f * atan(0.5f * bcamera->sensor_x / bcamera->lens);
        if (bcamera->sensor_fit == '\x02')
        {
            camera->SetSensorFit(CameraSensorFit::VERTICAL);
            fov = 2.f * atan(0.5f * bcamera->sensor_y / bcamera->lens);
        }
        else if (bcamera->sensor_fit == '\x01')
            camera->SetSensorFit(CameraSensorFit::HORIZONTAL);
        else
            camera->SetSensorFit(CameraSensorFit::AUTOMATIC);

        camera->SetOrthoScale(bcamera->ortho_scale);

        camera->SetNearClip(bcamera->clipsta);
        camera->SetFarClip(bcamera->clipend);

        camera->SetFOV(glm::degrees(fov));

        return camera;
    }

    PSceneNode BScene::CreateLight(const Blender::Object* obj, PSceneNode parent)
    {
        CHECK_ASSERT(obj->data, __FILE__, __LINE__);
        auto light = parent->CreateChild<Light>(B_IDNAME(obj));
        ExtractGeneral(obj, light);
        Blender::Lamp* la = static_cast<Blender::Lamp*>(obj->data);

        light->SetShadowColor(Color(la->shdwr, la->shdwg, la->shdwb, 1.f));
        light->SetOnlyShadow(la->mode & LA_ONLYSHADOW ? true : false);

        if (la->type == LA_SPOT)
        {
            bool bufferedShadows = la->mode & LA_SHAD_BUF ? true : false;
            bool rayTracedShadows = la->mode & LA_SHAD_RAY ? true : false;
            light->EnableShadows(bufferedShadows || rayTracedShadows);
            light->SetShadowClipStart(la->clipsta);
            light->SetShadowClipEnd(la->clipend);
        }
        else
            light->EnableShadows(la->mode & LA_SHAD_RAY ? true : false);

        // TODO: CHECK IF THIS IS A GOOD APROXIMATION
        float bias = la->bias * 0.001f;
        light->SetBias(bias);

        if (la->mode & LA_NEG)
            light->SetEnergy(-la->energy);
        else
            light->SetEnergy(la->energy);

        light->SetColor(Color(la->r, la->g, la->b, 1.f));
        light->EnableDiffuseColor(!(la->mode & LA_NO_DIFF));
        light->EnableSpecularColor(!(la->mode & LA_NO_SPEC));
        LightType type = LightType::POINT;
        if (la->type != LA_LOCAL)
            type = la->type == LA_SPOT ? LightType::SPOT : LightType::DIRECTIONAL;
        light->SetType(type);
        float cutoff = glm::degrees<float>(la->spotsize);
        light->SetSpotCutOff(cutoff > 128 ? 128 : cutoff);
        light->SetDistance(la->dist);
        //falloff = 128.f * la->spotblend;
        return light;
    }

    PSceneNode BScene::CreateMesh(const Blender::Object* obj, PSceneNode parent)
    {
        CHECK_ASSERT(obj->data, __FILE__, __LINE__);
        const Blender::Mesh* me = (const Blender::Mesh*)obj->data;
        auto nMaterials = obj->totcol;
        int materialIndex = 0;
        PSceneNode firstNode;
        std::string suffix;
        do
        {
            std::string nodeName = B_IDNAME(obj) + suffix;
            std::string meshName = B_IDNAME(me) + suffix;
            auto sceneNode = parent->CreateChild<SceneNode>(nodeName);
            if (materialIndex == 0)
            {
                firstNode = sceneNode;
                ExtractGeneral(obj, sceneNode);
            }
            auto mesh = Mesh::GetClass<ModelMesh>(meshName);
            bool meshOk = true;
            if (!mesh)
            {
                mesh = Mesh::CreateClass<ModelMesh>(meshName);
                meshOk = ConvertMesh(obj, me, mesh, materialIndex);
            }
            if (meshOk)
                sceneNode->SetMesh(mesh);
            SetMaterial(obj, sceneNode, materialIndex);
            parent = firstNode;
            ++materialIndex;
            suffix = ToString(materialIndex);
        }
        while (nMaterials > materialIndex);
        return firstNode;
    }

    const Blender::Object* BScene::GetAssignedArmature(const Blender::Object* obj) const
    {
        Blender::Object* ob_arm = nullptr;

        //if (obj->parent && obj->partype == PARSKEL && obj->parent->type == OB_ARMATURE)
        if (obj->parent && obj->parent->type == OB_ARMATURE)
            ob_arm = obj->parent;
        else
        {
            const Blender::ModifierData* mod = (const Blender::ModifierData*)obj->modifiers.first;
            for (; mod; mod = mod->next)
                if (mod->type == eModifierType_Armature)
                    ob_arm = ((const Blender::ArmatureModifierData*)mod)->object;
        }
        return ob_arm;
    }

    void BScene::GetBoneIndexByDeformGroupIndex(const Blender::Object* obj, const Blender::Object* obAr, std::vector<std::pair<int, std::string>>& list)
    {
        list.clear();
        int idx = 0;
        const Blender::bDeformGroup* def = (const Blender::bDeformGroup*)obj->defbase.first;
        while (def)
        {
            std::pair<int, std::string> data = { -1, def->name };
            if (IsBoneDefGroup(obAr, def))
                data.first = idx++;
            list.push_back(data);
            def = def->next;
        }
    }

    void BScene::AssignBonesAndWeights(const Blender::Object* obj, const Blender::Mesh* me, VertexsData& vertexes)
    {
        const Blender::Object* obAr = GetAssignedArmature(obj);
        if (me->dvert && obAr)
        {
            std::vector<std::pair<int, std::string>> jointList;
            GetBoneIndexByDeformGroupIndex(obj, obAr, jointList);
            const Blender::bDeformGroup* dg = (const Blender::bDeformGroup*)obj->defbase.first;
            while (dg)
            {
                if (IsBoneDefGroup(obAr, dg))
                {
                    const Blender::MDeformVert* dvert = me->dvert;
                    for (int n = 0; n < me->totvert; n++)
                    {
                        float sumw = 0.0f;
                        std::map<int, float> boneWeightPerVertex;
                        const Blender::MDeformVert& dv = dvert[n];
                        int nWeights = glm::clamp(dv.totweight, 0, (int)MAX_BONES_PER_VERTEX);
                        for (int w = 0; w < nWeights; w++)
                        {
                            const Blender::MDeformWeight& deform = dv.dw[w];
                            if (deform.def_nr >= jointList.size())
                                continue; // it looks like we can have out of bound indexes in blender
                            else if (deform.def_nr >= 0)
                            {
                                int joint_index = jointList[deform.def_nr].first;
                                if (joint_index != -1 && deform.weight > 0.0f)
                                {
                                    if (boneWeightPerVertex.size() == MAX_BONES_PER_VERTEX && boneWeightPerVertex.end() == boneWeightPerVertex.find(joint_index))
                                    {
                                        LOGW("Detected vertex with more than %d bones assigned. New bones will be ignored.", MAX_BONES_PER_VERTEX);
                                    }
                                    else
                                    {
                                        boneWeightPerVertex[joint_index] += deform.weight;
                                        sumw += deform.weight;
                                    }
                                }
                            }
                        }
                        if (sumw > 0.0f)
                        {
                            auto invsumw = 1.0f / sumw;
                            int idx = 0;
                            for (auto& bw : boneWeightPerVertex)
                            {
                                auto boneIndex(bw.first);
                                vertexes[n].bonesID_[idx] = (float)boneIndex;
                                vertexes[n].bonesWeight_[idx] = bw.second * invsumw;
                                ++idx;
                                if (idx > MAX_BONES_PER_VERTEX)
                                    break;
                            }
                        }
                    }
                }
                dg = dg->next;
            }
        }
    }

    const void* BScene::FindByString(const Blender::ListBase* listbase, const char* id, int offset) const
    {
        const Blender::Link* link = (const Blender::Link*)listbase->first;
        const char* id_iter;
        while (link)
        {
            id_iter = ((const char*)link) + offset;
            if (id[0] == id_iter[0] && strcmp(id, id_iter) == 0)
                return link;
            link = link->next;
        }
        return nullptr;
    }

    const Blender::bPoseChannel* BScene::GetPoseChannelByName(const Blender::bPose* pose, const char* name) const
    {
        if (!name || (name[0] == '\0'))
            return nullptr;

        return (const Blender::bPoseChannel*)FindByString(&(pose)->chanbase, name, offsetof(Blender::bPoseChannel, name));
    }

    const Blender::Bone* BScene::GetBoneFromDefGroup(const Blender::Object* obj, const Blender::bDeformGroup* def) const
    {
        const Blender::bPoseChannel* pchan = GetPoseChannelByName(obj->pose, def->name);
        return pchan ? pchan->bone : nullptr;
    }

    bool BScene::IsBoneDefGroup(const Blender::Object* obj, const Blender::bDeformGroup* def) const
    {
        return GetBoneFromDefGroup(obj, def) != nullptr;
    }

    void BScene::CreateSkeleton(PScene scene, const Blender::Object* obj)
    {
        const Blender::Object* obAr = GetAssignedArmature(obj);
        if (!obAr)
        {
            LOGW("Failed to get armature.");
            return;
        }

        if (obj->totcol > 1)
        {
            LOGE("Cannot create skeleton for mesh with multiple materials");
            return;
        }

        CHECK_ASSERT(obAr, __FILE__, __LINE__);
        CHECK_ASSERT(obj->type == OB_MESH, __FILE__, __LINE__);
        const Blender::Mesh* me = (Blender::Mesh*)obj->data;
        auto mesh = Mesh::GetClass<ModelMesh>(B_IDNAME(me));
        if (!mesh)
        {
            LOGW("Trying to create skeleton without mesh.");
            return;
        }

        const Blender::bArmature* arm = static_cast<const Blender::bArmature*>(obAr->data);
        std::string armatureName = B_IDNAME(arm);
        if (!(arm->flag & ARM_RESTPOS))
        {
            LOGE("Cannot create skeleton: %s. Armature has to be in rest position. Go to blender and change it.", armatureName.c_str());
            return;
        }

        auto skeleton(std::make_shared<Skeleton>(mesh));
        std::string obArName = B_IDNAME(obAr);
        PSceneNode armatureNode = scene->GetChild<SceneNode>(obArName, true);
        if (!armatureNode)
        {
            LOGE("Cannot find armature node with name = %s", obArName.c_str());
            return;
        }

        std::vector<NSG::PWeakNode> boneList;
        std::vector<std::pair<int, std::string>> jointList;
        GetBoneIndexByDeformGroupIndex(obj, obAr, jointList);
        for (auto& joint : jointList)
        {
            if (joint.first != -1)
            {
                auto bone = armatureNode->GetChild<SceneNode>(joint.second, true);
                boneList.push_back(bone);
            }
        }
        skeleton->SetRoot(armatureNode);
        mesh->SetSkeleton(skeleton);
        skeleton->SetBones(boneList);
        CreateOffsetMatrices(obj, armatureNode);
    }

    void BScene::CreateOffsetMatrices(const Blender::Object* obj, PSceneNode armatureNode)
    {
        const Blender::Object* obAr = GetAssignedArmature(obj);
        CHECK_ASSERT(obAr, __FILE__, __LINE__);
        const Blender::bPose* pose = obAr->pose;
        const Blender::bDeformGroup* def = (const Blender::bDeformGroup*)obj->defbase.first;
        while (def)
        {
            if (IsBoneDefGroup(obAr, def))
            {
                const Blender::bPoseChannel* pchan = GetPoseChannelByName(pose, def->name);
                PSceneNode bone = armatureNode->GetChild<SceneNode>(pchan->bone->name, true);

                Matrix4 offsetMatrix = Matrix4(glm::inverse(ToMatrix(obj->obmat) * ToMatrix(pchan->bone->arm_mat)));
                Matrix4 bindShapeMatrix = armatureNode->GetTransform();
                bone->SetBoneOffsetMatrix(offsetMatrix * bindShapeMatrix);
            }
            def = def->next;
        }
    }

    bool BScene::ConvertMeshLegacy(const Blender::Object* obj, const Blender::Mesh* me, PModelMesh mesh, int materialIndex)
    {
        CHECK_ASSERT(me->mface && "This is not legacy data!!!", __FILE__, __LINE__);

        if (!me->mvert)
            return false;

        Blender::MTFace* mtface[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        char* uvNames[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
        Blender::MCol* mcol = nullptr;
        int totlayer = GetUVLayersBMmeshLegacy(me, mtface, uvNames, &mcol);
        for (int i = 0; i < MAX_UVS; i++)
            if (uvNames[i])
                mesh->SetUVName(i, uvNames[i]);

        int nVertexes = me->totvert;
        int nuvs = glm::clamp(totlayer, 0, 2);

        VertexsData vertexData(nVertexes);
        for (int i = 0; i < nVertexes; i++)
        {
            vertexData[i].position_ = Vertex3(me->mvert[i].co[0], me->mvert[i].co[1], me->mvert[i].co[2]);
            vertexData[i].normal_ = glm::normalize(Vertex3(me->mvert[i].no[0], me->mvert[i].no[1], me->mvert[i].no[2]));
        }

        AssignBonesAndWeights(obj, me, vertexData);

        bool hasColorVertex = mcol != nullptr;

        for (int fi = 0; fi < me->totface; fi++)
        {
            const Blender::MFace& curface = me->mface[fi];
            if (curface.mat_nr != materialIndex)
                continue;

            bool isQuad = curface.v4 != 0;

            // skip if face is not a triangle || quad
            if (!curface.v3)
            {
                LOGW("Skipping face: Only triangles or quads are converted.");
                LOGI("To solve it: triangulate the object with a modifier.")
                continue;
            }

            for (int i = 0; i < nuvs; i++)
            {
                const Blender::MTFace* face = mtface[i];
                if (face)
                {
                    vertexData[curface.v1].uv_[i] = Vertex2(face[fi].uv[0][0], face[fi].uv[0][1]);
                    vertexData[curface.v2].uv_[i] = Vertex2(face[fi].uv[1][0], face[fi].uv[1][1]);
                    vertexData[curface.v3].uv_[i] = Vertex2(face[fi].uv[2][0], face[fi].uv[2][1]);
                    if (isQuad)
                        vertexData[curface.v4].uv_[i] = Vertex2(face[fi].uv[3][0], face[fi].uv[3][1]);
                }
            }

            if (hasColorVertex)
            {
                {
                    unsigned r = glm::clamp<unsigned>(mcol[0].r, 0, 255);
                    unsigned g = glm::clamp<unsigned>(mcol[0].g, 0, 255);
                    unsigned b = glm::clamp<unsigned>(mcol[0].b, 0, 255);
                    unsigned a = glm::clamp<unsigned>(mcol[0].a, 0, 255);
                    Color color(r, g, b, a);
                    vertexData[curface.v1].color_ = color / 255.f;
                }
                {
                    unsigned r = glm::clamp<unsigned>(mcol[1].r, 0, 255);
                    unsigned g = glm::clamp<unsigned>(mcol[1].g, 0, 255);
                    unsigned b = glm::clamp<unsigned>(mcol[1].b, 0, 255);
                    unsigned a = glm::clamp<unsigned>(mcol[1].a, 0, 255);
                    Color color(r, g, b, a);
                    vertexData[curface.v2].color_ = color / 255.f;
                }
                {
                    unsigned r = glm::clamp<unsigned>(mcol[2].r, 0, 255);
                    unsigned g = glm::clamp<unsigned>(mcol[2].g, 0, 255);
                    unsigned b = glm::clamp<unsigned>(mcol[2].b, 0, 255);
                    unsigned a = glm::clamp<unsigned>(mcol[2].a, 0, 255);
                    Color color(r, g, b, a);
                    vertexData[curface.v3].color_ = color / 255.f;
                }
                if (isQuad)
                {
                    unsigned r = glm::clamp<unsigned>(mcol[3].r, 0, 255);
                    unsigned g = glm::clamp<unsigned>(mcol[3].g, 0, 255);
                    unsigned b = glm::clamp<unsigned>(mcol[3].b, 0, 255);
                    unsigned a = glm::clamp<unsigned>(mcol[3].a, 0, 255);
                    Color color(r, g, b, a);
                    vertexData[curface.v4].color_ = color / 255.f;
                }
            }

            int index[4];
            index[0] = curface.v1;
            index[1] = curface.v2;
            index[2] = curface.v3;
            index[3] = curface.v4;

            bool calcFaceNormal = !(curface.flag & ME_SMOOTH);

            if (isQuad)
                mesh->AddQuad(vertexData[index[0]], vertexData[index[1]], vertexData[index[2]], vertexData[index[3]], calcFaceNormal);
            else
                mesh->AddTriangle(vertexData[index[0]], vertexData[index[1]], vertexData[index[2]], calcFaceNormal);

            if (mcol)
                mcol += 4;
        }

        return true;
    }

    bool BScene::ConvertMesh(const Blender::Object* obj, const Blender::Mesh* me, PModelMesh mesh, int materialIndex)
    {
        if (me->mface)
            return ConvertMeshLegacy(obj, me, mesh, materialIndex);

        if (!me->mvert || !me->mpoly)
            return false;

        // UV-Layer-Data
        Blender::MLoopUV* muvs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        char* uvNames[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
        int totlayer = GetUVLayersBMmesh(me, muvs, uvNames);
        for (int i = 0; i < MAX_UVS; i++)
            if (uvNames[i])
                mesh->SetUVName(i, uvNames[i]);

        int nVertexes = me->totvert;
        int nuvs = glm::clamp(totlayer, 0, 2);

        VertexsData vertexData(nVertexes);
        for (int i = 0; i < nVertexes; i++)
        {
            vertexData[i].position_ = Vertex3(me->mvert[i].co[0], me->mvert[i].co[1], me->mvert[i].co[2]);
            vertexData[i].normal_ = glm::normalize(Vertex3(me->mvert[i].no[0], me->mvert[i].no[1], me->mvert[i].no[2]));
        }

        AssignBonesAndWeights(obj, me, vertexData);

        bool hasColorVertex = me->mloopcol && me->totcol;
        for (int fi = 0; fi < me->totpoly; fi++)
        {
            const Blender::MPoly& curpoly = me->mpoly[fi];
            if (curpoly.mat_nr != materialIndex)
                continue;
            int nloops = curpoly.totloop;
            int indexBase = curpoly.loopstart;

            if (muvs)
            {
                for (int i = 0; i < nloops; i++)
                {
                    int li = indexBase + i;
                    int vi = me->mloop[li].v;
                    for (int j = 0; j < nuvs; j++)
                        vertexData[vi].uv_[j] = Vertex2(muvs[j][li].uv[0], muvs[j][li].uv[1]);
                }
            }

            if (hasColorVertex)
            {
                for (int i = 0; i < nloops; i++)
                {
                    int li = indexBase + i;
                    int vi = me->mloop[li].v;
                    unsigned r = glm::clamp<unsigned>(me->mloopcol[li].r, 0, 255);
                    unsigned g = glm::clamp<unsigned>(me->mloopcol[li].g, 0, 255);
                    unsigned b = glm::clamp<unsigned>(me->mloopcol[li].b, 0, 255);
                    unsigned a = glm::clamp<unsigned>(me->mloopcol[li].a, 0, 255);
                    Color color(r, g, b, a);
                    vertexData[vi].color_ = color / 255.f;
                }
            }

            std::vector<int> index;
            index.resize(nloops);
            for (int i = 0; i < nloops; i++)
                index[i] = me->mloop[indexBase + i].v;

            bool averageFaceNormal = !(curpoly.flag & ME_SMOOTH);

            if (nloops == 4)
                mesh->AddQuad(vertexData[index[0]], vertexData[index[1]], vertexData[index[2]], vertexData[index[3]], averageFaceNormal);
            else if (nloops == 3)
                mesh->AddTriangle(vertexData[index[0]], vertexData[index[1]], vertexData[index[2]], averageFaceNormal);
            else
                Tessellate(mesh, vertexData, index);
        }
        return true;
    }

    struct PointP2T
    {
        Vector3 point3D;
        p2t::Point point2D; // flatten point
        unsigned int magic;
        int index;
    };

    static const unsigned int BLEND_TESS_MAGIC = 0x83ed9ac3;
    static PointP2T& GetActualPointStructure(p2t::Point& point)
    {
        unsigned int pointOffset = offsetof(PointP2T, point2D);
        PointP2T& pointStruct = *reinterpret_cast< PointP2T* >( reinterpret_cast< char* >( &point ) - pointOffset );
        if ( pointStruct.magic != static_cast<int>( BLEND_TESS_MAGIC ) )
            throw std::runtime_error("Point returned by poly2tri was probably not one of ours. This indicates we need a new way to store vertex information");
        return pointStruct;
    }

    void BScene::Tessellate(PModelMesh mesh, const VertexsData& vertexData, const std::vector<int>& indexes)
    {
        std::vector<PointP2T> points;
        for (auto index : indexes)
            points.push_back(PointP2T {vertexData[index].position_, p2t::Point(), BLEND_TESS_MAGIC, index});
        Plane plane(points[0].point3D, points[1].point3D, points[2].point3D);
        auto m = GeneratePointTransformMatrix(plane, points[0].point3D);
        int pointIdx = 0;
        for (auto& point : points)
        {
            //Flatten points
            Vector4 tmp = m * Vector4(point.point3D, 1);
            //Flatten points should always belong to XY plane
            if (std::abs(tmp.z) > 0.00001f)
            {
                // But some times they are not.
                // This is because points in the blender loop are not part of the same plane
                LOGW("Detected loop where not all their points are coplanar!!!");
                LOGW("Loop contains %d points. And point %d is not coplanar!!!", points.size(), pointIdx + 1);
            }
            point.point2D = p2t::Point(tmp.x, tmp.y);
            ++pointIdx;
        }

        std::vector<p2t::Point*> pointRefs;
        for (auto& point : points)
            pointRefs.push_back(&point.point2D);

        p2t::CDT cdt(pointRefs);
        cdt.Triangulate();
        std::vector<p2t::Triangle*> triangles = cdt.GetTriangles();
        for ( unsigned int i = 0; i < triangles.size( ); ++i )
        {
            p2t::Triangle& Triangle = *triangles[ i ];
            PointP2T& pointA = GetActualPointStructure(*Triangle.GetPoint(0));
            PointP2T& pointB = GetActualPointStructure(*Triangle.GetPoint(1));
            PointP2T& pointC = GetActualPointStructure(*Triangle.GetPoint(2));
            mesh->AddTriangle(vertexData[pointA.index], vertexData[pointB.index], vertexData[pointC.index], plane.GetNormal());
        }
    }

    int BScene::GetUVLayersBMmesh(const Blender::Mesh* mesh, Blender::MLoopUV** uvEightLayerArray, char** uvNames)
    {
        CHECK_ASSERT(mesh, __FILE__, __LINE__);

        int validLayers = 0;
        Blender::CustomDataLayer* layers = (Blender::CustomDataLayer*)mesh->ldata.layers;
        if (layers)
        {
            for (int i = 0; i < mesh->ldata.totlayer && validLayers < 8; i++)
            {
                Blender::CustomDataLayer& layer = layers[i];
                if (layers[i].type == CD_MLOOPUV && uvEightLayerArray)
                {
                    Blender::MLoopUV* mtf = (Blender::MLoopUV*)layer.data;
                    if (mtf)
                    {
                        uvNames[validLayers] = layer.name;
                        uvEightLayerArray[validLayers++] = mtf;
                    }
                }
            }
        }
        return validLayers;
    }

    int BScene::GetUVLayersBMmeshLegacy(const Blender::Mesh* mesh, Blender::MTFace** eightLayerArray, char** uvNames, Blender::MCol** oneMCol)
    {
        CHECK_ASSERT(mesh, __FILE__, __LINE__);

        int validLayers = 0;

        Blender::CustomDataLayer* layers = (Blender::CustomDataLayer*)mesh->fdata.layers;
        if (layers)
        {
            for (int i = 0; i < mesh->fdata.totlayer && validLayers < 8; i++)
            {
                Blender::CustomDataLayer& layer = layers[i];
                if (layer.type == CD_MTFACE && eightLayerArray)
                {
                    Blender::MTFace* mtf = (Blender::MTFace*)layers[i].data;
                    if (mtf)
                    {
                        uvNames[validLayers] = layer.name;
                        eightLayerArray[validLayers++] = mtf;
                    }
                }
                else if (layer.type == CD_MCOL)
                {
                    if (oneMCol && !(*oneMCol))
                        *oneMCol = static_cast<Blender::MCol*>(layer.data);
                }
            }
        }
        else
        {
            if (eightLayerArray && mesh->mtface)
                eightLayerArray[validLayers++] = mesh->mtface;
            if (oneMCol && mesh->mcol)
                *oneMCol = mesh->mcol;
        }

        return validLayers;
    }


    const Blender::Material* BScene::GetMaterial(const Blender::Object* ob, int index) const
    {
        if (!ob || ob->totcol == 0) return nullptr;

        index = glm::clamp<int>(index, 0, ob->totcol - 1);
        Blender::Material* ma = nullptr;

        int inObject = ob->matbits && ob->matbits[index] ? 1 : 0;

        if (!inObject)
            inObject = ob->colbits & (1 << index);

        if (inObject)
            ma = (Blender::Material*)ob->mat[index];
        else
        {
            Blender::Mesh* me = (Blender::Mesh*)ob->data;
            if (me && me->mat && me->mat[index])
                ma = me->mat[index];
        }

        return ma;
    }

    void BScene::SetMaterial(const Blender::Object* obj, PSceneNode sceneNode, int materialIndex)
    {
        const Blender::Material* mt = GetMaterial(obj, materialIndex);
        std::string name = B_IDNAME(mt);
        auto material = Material::Get(name);
        if (material)
            sceneNode->SetMaterial(material);
        else
            sceneNode->SetMaterial(defaultMaterial_);
    }

    bool BScene::Save(bool compress)
    {
        Path outputFile(outputDir_);
        outputFile.SetName("b" + path_.GetName());
        outputFile.SetExtension("xml");
        pugi::xml_document doc;
        GenerateXML(doc);
        return FileSystem::SaveDocument(outputFile, doc, compress);
    }

    void BScene::GenerateXML(pugi::xml_document& doc)
    {
        pugi::xml_node appNode = doc.append_child("App");
        if (embedResources_)
            Resource::SaveResources(appNode);
        else
            Resource::SaveResourcesExternally(appNode, path_, outputDir_);
        Sound::SaveSounds(appNode);
        Mesh::SaveMeshes(appNode);
        Material::SaveMaterials(appNode);
        Shape::SaveShapes(appNode);
        for (auto& scene : scenes_)
            scene->Save(appNode);
    }


}

