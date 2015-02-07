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
#include "BScene.h"
#include "BlenderDefines.h"
#include "UtilConverter.h"
#include "pugixml.hpp"
#include "bMain.h"
#include "Blender.h"
#include "bBlenderFile.h"
#include "NSG.h"
#include <cstdio>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

namespace BlenderConverter
{
    using namespace NSG;
	BScene::BScene(const Path& path, const Path& outputDir, bool embedResources)
        : path_(path),
          outputDir_(outputDir),
		  embedResources_(embedResources)
    {
    }

    BScene::~BScene()
    {
    }

    bool BScene::Load()
    {
        bParse::bBlenderFile obj(path_.GetFullAbsoluteFilePath().c_str());
        obj.parse(false);
        bParse::bMain* data = obj.getMain();
        bParse::bListBasePtr* it = data->getScene();
        if (it->size())
        {
            LoadMaterials(data);
            sc_ = (Blender::Scene*)it->at(0);
            scene_ = std::make_shared<Scene>(B_IDNAME(sc_));
            scene_->SetOrientation(glm::angleAxis<float>(-PI / 2.f, Vertex3(1, 0, 0)));
            const Blender::World* world = sc_->world;
            scene_->GetPhysicsWorld()->SetGravity(Vector3(0, -world->gravity, 0));
            Color ambient(world->ambr, world->ambg, world->ambb, 1);
            scene_->SetAmbientColor(ambient);
            const Blender::Base* base = (const Blender::Base*)sc_->base.first;
            while (base)
            {
                const Blender::Object* obj = base->object;
                ConvertObject(obj, scene_);
                base = base->next;
            }

            for (auto& objArmature : armatureLinker_)
                CreateSkeleton(objArmature);

			base = (const Blender::Base*)sc_->base.first;
			while (base)
			{
				const Blender::Object* obj = base->object;
				CreateAnimation(obj);
				base = base->next;
			}

            return true;
        }
        return false;
    }

    void BScene::LoadMaterials(bParse::bMain* data)
    {
        bParse::bListBasePtr* it = data->getMat();
        int n = it->size();
        for (int i = 0; i < n; i++)
        {
            auto material = (Blender::Material*)it->at(i);
            LoadMaterial(material);
        }
    }

    PTexture BScene::CreateTexture(const Blender::Image* ima)
    {
        std::string imageName = B_IDNAME(ima);
        if (!ima->packedfile)
        {
            Path outputPath;
            outputPath.SetPath(outputDir_.GetPath());
            outputPath.SetFileName(imageName);
#if 0
            Path inputPath;
            inputPath.SetPath(path_.GetPath());
            inputPath.SetFileName(imageName);
            if (!NSGCopyFile(inputPath, outputPath))
            {
                TRACE_LOG("Failed to copy file: " << imageName);
            }
#endif
            std::vector<std::string> dirs = Path::GetDirs(outputPath.GetPath());
            Path relativePath;
            if (!dirs.empty())
                relativePath.SetPath(dirs.back());
            relativePath.SetFileName(imageName);

            auto resource = App::this_->GetOrCreateResourceFile(relativePath.GetFilePath());
            return std::make_shared<Texture>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
        }
        else
        {
			auto resource = App::this_->GetOrCreateResourceMemory(imageName);
			resource->SetData((const char*)ima->packedfile->data, ima->packedfile->size);
			return std::make_shared<Texture>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
        }
    }

    void BScene::LoadMaterial(const Blender::Material* mt)
    {
        std::string name = B_IDNAME(mt);
        auto material = App::this_->GetOrCreateMaterial(name);
        auto technique = material->GetTechnique();
        auto pass = technique->GetPass(0);
        auto program = pass->GetProgram();
        ProgramFlags flags = (int)ProgramFlag::PER_PIXEL_LIGHTING;
        program->SetFlags(flags);
        material->SetDiffuseColor(Color(mt->r, mt->g, mt->b, mt->alpha));
        material->SetSpecularColor(Color(mt->specr, mt->specg, mt->specb, mt->alpha));
        material->SetAmbientColor(Color(mt->ambr, mt->ambg, mt->ambb, mt->alpha));

        pass->EnableDepthBuffer(mt->mode & MA_ZTRA ? true : false);
        if (mt->game.alpha_blend & GEMAT_ALPHA)
            pass->SetBlendMode(BLEND_ALPHA);
        else
            pass->SetBlendMode(BLEND_NONE);

        if (mt->mode & MA_WIRE)
            pass->SetDrawMode(DrawMode::WIREFRAME);
        else
            pass->SetDrawMode(DrawMode::SOLID);

        // textures
        if (mt->mtex != 0)
        {
            for (int i = 0; i < MAX_MTEX; i++)
            {
                if (!mt->mtex[i] || !mt->mtex[i]->tex)
                    continue;

                if (mt->mtex[i]->tex->type == TEX_IMAGE)
                {
                    const Blender::MTex* mtex = mt->mtex[i];
                    const Blender::Image* ima = mtex->tex->ima;
                    if (!ima) continue;
                    auto texture = CreateTexture(ima);
                    if ((mtex->mapto & MAP_COL) || (mtex->maptoneg & MAP_COL))
                        material->SetDiffuseMap(texture);
                    if ((mtex->mapto & MAP_NORM) || (mtex->maptoneg & MAP_NORM))
                        material->SetNormalMap(texture);
                    if ((mtex->mapto & MAP_SPEC) || (mtex->maptoneg & MAP_SPEC))
                        material->SetSpecularMap(texture);
                    if ((mtex->mapto & MAP_AMB) || (mtex->maptoneg & MAP_AMB))
                        material->SetAOMap(texture);
                    if ((mtex->mapto & MAP_DISPLACE) || (mtex->maptoneg & MAP_DISPLACE))
                        material->SetDisplacementMap(texture);
                }
            }
        }
    }

    void BScene::ConvertObject(const Blender::Object* obj, PSceneNode sceneNode)
    {
        if (obj)
        {
            if (obj->type == OB_MESH && obj->parent != 0 && obj->parent->type == OB_ARMATURE)
                armatureLinker_.push_back(obj);

            switch (obj->type)
            {
                case OB_EMPTY:
                    CreateSceneNode(obj, sceneNode);
                    break;
                case OB_LAMP:
                    CreateLight(obj, sceneNode);
                    break;
                case OB_CAMERA:
                    CreateCamera(obj, sceneNode);
                    break;
                case OB_MESH:
                    CreateMesh(obj, sceneNode);
                    break;
                case OB_ARMATURE:   // SceneNode + Skeleton
                    CreateSkeleton(obj, sceneNode);
                    break;
                case OB_CURVE:
                    break;
                default:
                    break;
            }
        }
    }

	void BScene::GetFrames(const Blender::Object* ob, std::vector<float> &fra)
	{
		if (ob->adt && ob->adt->action) 
		{
			const Blender::FCurve* fcu = (const Blender::FCurve*)ob->adt->action->curves.first;
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
	}

	float BScene::GetTracks(const Blender::Object *obj, float animfps, BTracks& tracks)
	{
		CHECK_ASSERT(obj->adt && obj->adt->action, __FILE__, __LINE__);

		std::string name(B_IDNAME(obj->adt->action));
		float start, end;
		GetActionStartEnd(obj->adt->action, start, end);
		float trackLength = (end - start) / animfps;

		const Blender::FCurve* bfc = (const Blender::FCurve*)obj->adt->action->curves.first;

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
				else if (transform_name == "rotation_euler" && obj->rotmode == ROT_MODE_EUL)
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
				if (code != -1 && bfc->totvert > 0)
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


	void BScene::CreateAnimation(const Blender::Object* obj)
    {
		if (obj->adt && obj->adt->action)
        {
			std::string name(B_IDNAME(obj->adt->action));
			if (!scene_->HasAnimation(name))
			{
				float animfps = sc_->r.frs_sec / sc_->r.frs_sec_base;
				auto anim = scene_->GetOrCreateAnimation(name);
				BTracks tracks;
				auto length = GetTracks(obj, animfps, tracks);
				ConvertTracks(obj, anim, tracks, length);
				anim->SetLength(length);
			}
        }
    }

	void BScene::ConvertTracks(const Blender::Object* obj, PAnimation anim, BTracks& tracks, float length)
    {
        for (auto& btrack : tracks)
        {
            std::string channelName = btrack.first;
            AnimationTrack track;
            if (scene_->GetName() == channelName)
                track.node_ = scene_;
            else
                track.node_ = scene_->GetChild<Node>(channelName, true);

            if (!track.node_.lock())
            {
                TRACE_LOG("Warning: skipping animation track " << channelName << " whose scene node was not found");
                continue;
            }

            track.channelMask_ = ConvertChannel(obj, btrack.second, track, length);
            anim->AddTrack(track);
        }
    }

	AnimationChannelMask BScene::ConvertChannel(const Blender::Object* obj, PTrackData trackData, AnimationTrack& track, float timeFrameLength)
    {
        AnimationChannelMask mask = 0;
		float inc = timeFrameLength;

        if (trackData->keyframes.size())
			inc = timeFrameLength / trackData->keyframes[0]->getNumVerts();

		float start, end;
		GetActionStartEnd(obj->adt->action, start, end);
		float totalFramesLength = end - start;

		std::vector<float> frames;
		GetFrames(obj, frames);
		std::vector<float> framesTime;
		for (auto&f : frames)
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
			float delta = t / timeFrameLength;
            AnimationKeyFrame keyframe;
            keyframe.time_ = t;
            Vector3 pos;
            Quaternion q;
            Vector3 scale(1);
            Vector3 eulerAngles;

            for (auto& spline : trackData->keyframes)
            {
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
        Matrix4 m = ToMatrix(obj->obmat);
        Quaternion q;
        Vector3 pos;
        Vector3 scale;
        DecomposeMatrix(m, pos, q, scale);
        sceneNode->SetPosition(pos);
        sceneNode->SetOrientation(q);
        sceneNode->SetScale(scale);
        LoadPhysics(obj, sceneNode);
    }

    void BScene::LoadPhysics(const Blender::Object* obj, PSceneNode sceneNode)
    {
        PhysicsShape shapeType = PhysicsShape::SH_UNKNOWN;
        int boundtype = obj->collision_boundtype;

        if (obj->type != OB_MESH)
        {
            if (!(obj->gameflag & OB_ACTOR))
                boundtype = 0;
        }

        if (!(obj->gameflag & OB_BOUNDS))
        {
            if (obj->body_type == OB_BODY_TYPE_STATIC)
            {
                boundtype = OB_BOUND_TRIANGLE_MESH;
            }
            else
                boundtype = OB_BOUND_CONVEX_HULL;
        }

        Blender::Object* parent = obj->parent;
        while (parent && parent->parent)
            parent = parent->parent;

        if (parent && (obj->gameflag & OB_CHILD) == 0)
            boundtype = 0;

        if (!boundtype)
            return;

        auto rigBody = sceneNode->GetOrCreateRigidBody();
        rigBody->SetLinearDamp(obj->damping);
        rigBody->SetAngularDamp(obj->rdamping);
        rigBody->SetMargin(obj->margin);

        if (obj->type == OB_MESH)
        {
            const Blender::Mesh* me = (const Blender::Mesh*)obj->data;
            if (me)
            {
                const Blender::Material* ma = GetMaterial(obj, 0);
                if (ma)
                {
                    rigBody->SetRestitution(ma->reflect);
                    rigBody->SetFriction(ma->friction);
                }
            }
        }

        if (obj->body_type == OB_BODY_TYPE_STATIC || boundtype == OB_BOUND_TRIANGLE_MESH)
            rigBody->SetMass(0);
        else
            rigBody->SetMass(obj->mass);

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
                if (obj->type == OB_MESH)
                    shapeType = PhysicsShape::SH_TRIMESH;
                else
                    shapeType = PhysicsShape::SH_SPHERE;
                break;
            case OB_BOUND_CAPSULE:
                shapeType = PhysicsShape::SH_CAPSULE;
                break;
        }

        rigBody->SetShape(shapeType);
    }


    PSceneNode BScene::CreateSceneNode(const Blender::Object* obj, PSceneNode parent)
    {
        auto sceneNode = parent->CreateChild<SceneNode>(B_IDNAME(obj));
        ExtractGeneral(obj, sceneNode);
        return sceneNode;
    }

    void BScene::CreateSkeleton(const Blender::Object* obj, PSceneNode parent)
    {
        auto sceneNode = CreateSceneNode(obj, parent);
        const Blender::bArmature* ar = static_cast<const Blender::bArmature*>(obj->data);
		CHECK_ASSERT(ar->flag & ARM_RESTPOS && "Armature has to be in rest position. Go to blender and change it.", __FILE__, __LINE__);
		std::string armatureName = B_IDNAME(ar);
		armatureBones_.clear();
        // create bone lists && transforms
        const Blender::Bone* bone = static_cast<const Blender::Bone*>(ar->bonebase.first);
        while (bone)
        {
            if (!bone->parent)
				BuildBoneTree(armatureName, bone, sceneNode);
            bone = bone->next;
        }
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

    void BScene::CreateCamera(const Blender::Object* obj, PSceneNode parent)
    {
        CHECK_ASSERT(obj->data, __FILE__, __LINE__);
        auto camera = parent->CreateChild<Camera>(B_IDNAME(obj));
        ExtractGeneral(obj, camera);
        Blender::Camera* bcamera = static_cast<Blender::Camera*>(obj->data);

        if (bcamera->type == CAM_ORTHO)
            camera->EnableOrtho();

        camera->SetNearClip(bcamera->clipsta);
        camera->SetFarClip(bcamera->clipend);
        //camera->SetAspectRatio((int)bcamera->sensor_x, (int)bcamera->sensor_y);
        camera->SetHalfHorizontalFov(glm::radians(bcamera->lens));
        //isMainCamera = sc_->camera == obj;
    }

    void BScene::CreateLight(const Blender::Object* obj, PSceneNode parent)
    {
        CHECK_ASSERT(obj->data, __FILE__, __LINE__);
        auto light = parent->CreateChild<Light>(B_IDNAME(obj));
        ExtractGeneral(obj, light);
        Blender::Lamp* la = static_cast<Blender::Lamp*>(obj->data);

        light->SetDiffuseColor(Color(la->r, la->g, la->b, 1.f));
        if (la->mode & LA_NO_DIFF)
            light->SetDiffuseColor(COLOR_BLACK);

        light->SetSpecularColor(Color(la->r, la->g, la->b, 1.f));
        if (la->mode & LA_NO_SPEC)
            light->SetSpecularColor(COLOR_BLACK);

        //la->energy;
        //if (la->mode & LA_NEG)
        //  -la->energy;

        light->SetAttenuation(1.f, la->att1 / la->dist, la->att2 / (la->dist * la->dist));

        LightType type = LightType::POINT;
        if (la->type != LA_LOCAL)
            type = la->type == LA_SPOT ? LightType::SPOT : LightType::DIRECTIONAL;
        light->SetType(type);
        float cutoff = glm::degrees<float>(la->spotsize);
        light->SetSpotCutOff(cutoff > 128 ? 128 : cutoff);
        //falloff = 128.f * la->spotblend;
    }

    void BScene::CreateMesh(const Blender::Object* obj, PSceneNode parent)
    {
        CHECK_ASSERT(obj->data, __FILE__, __LINE__);

        auto sceneNode = parent->CreateChild<SceneNode>(B_IDNAME(obj));
        ExtractGeneral(obj, sceneNode);
        const Blender::Mesh* me = (const Blender::Mesh*)obj->data;
        std::string meshName = B_IDNAME(me);
        auto mesh = App::this_->GetModelMesh(B_IDNAME(me));
        if (!mesh)
        {
            mesh = App::this_->GetOrCreateModelMesh(B_IDNAME(me));
            ConvertMesh(obj, me, mesh);
        }
        sceneNode->SetMesh(mesh);
        SetMaterial(obj, sceneNode);
    }

	const Blender::Object* BScene::GetAssignedArmature(const Blender::Object *obj) const
	{
		Blender::Object* ob_arm = nullptr;

		if (obj->parent && obj->partype == PARSKEL && obj->parent->type == OB_ARMATURE) 
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
		while(def)
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
										TRACE_LOG("Warning detected vertex with more than " << MAX_BONES_PER_VERTEX << " bones assigned. New bones will be ignored!!!");
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
							float invsumw = 1.0f / sumw;
							int idx = 0;
							for (auto& bw : boneWeightPerVertex)
							{
								vertexes[n].bonesID_[idx] = float(bw.first);
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
		while(link) 
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

	void BScene::CreateSkeleton(const Blender::Object* obj)
	{
		const Blender::Object* obAr = GetAssignedArmature(obj);
		CHECK_ASSERT(obAr, __FILE__, __LINE__);
		CHECK_ASSERT(obj->type == OB_MESH, __FILE__, __LINE__);
		const Blender::Mesh* me = (Blender::Mesh*)obj->data;
		auto mesh = App::this_->GetModelMesh(B_IDNAME(me));
		CHECK_ASSERT(mesh, __FILE__, __LINE__);
		
		const Blender::bArmature* arm = static_cast<const Blender::bArmature*>(obAr->data);
		CHECK_ASSERT(arm->flag & ARM_RESTPOS && "Armature has to be in rest position. Go to blender and change it.", __FILE__, __LINE__);
		std::string armatureName = B_IDNAME(arm);

		auto skeleton(std::make_shared<Skeleton>(mesh));
		PSceneNode armatureNode = scene_->GetChild<SceneNode>(B_IDNAME(obAr), true);

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
		MarkProgramAsSkinableNodes(mesh.get());
		CreateOffsetMatrices(obj);
	}

	void BScene::CreateOffsetMatrices(const Blender::Object* obj)
	{
		const Blender::Object* obAr = GetAssignedArmature(obj);
		CHECK_ASSERT(obAr, __FILE__, __LINE__);
		const Blender::bPose* pose = obAr->pose;
		PSceneNode armatureNode = scene_->GetChild<SceneNode>(B_IDNAME(obAr), true);

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

    void BScene::MarkProgramAsSkinableNodes(const Mesh* mesh)
    {
        auto& nodes = mesh->GetConstSceneNodes();

        for (auto obj : nodes)
        {
            PMaterial material = obj->GetMaterial();
            if (material)
            {
                Technique* technique = material->GetTechnique().get();
                technique->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::SKINNED);
            }
        }
    }

    void BScene::ConvertMesh(const Blender::Object* obj, const Blender::Mesh* me, PModelMesh mesh)
    {
        CHECK_ASSERT(!me->mface && "Legacy conversion is not allowed", __FILE__, __LINE__);
        CHECK_ASSERT(me->mvert && me->mpoly, __FILE__, __LINE__);

        // UV-Layer-Data
        Blender::MLoopUV* muvs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        int totlayer = GetUVLayersBMmesh(me, muvs);

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
            int nloops = curpoly.totloop;
            int indexBase = curpoly.loopstart;

            // skip if face is not a triangle || quad
            if (nloops < 3 || nloops > 4)
            {
                TRACE_LOG("Only triangles or quads are converted! (loops = " << nloops << ")");
                continue;
            }

            for (int i = 0; i < nloops; i++)
            {
                int li = indexBase + i;
                int vi = me->mloop[li].v;
                for (int j = 0; j < nuvs; j++)
                    vertexData[vi].uv_[j] = Vertex2(muvs[j][li].uv[0], muvs[j][li].uv[1]);
            }

            if (hasColorVertex)
            {
                for (int i = 0; i < nloops; i++)
                {
                    int li = indexBase + i;
                    int vi = me->mloop[li].v;
                    Color color(me->mloopcol[li].r, me->mloopcol[li].g, me->mloopcol[li].b, me->mloopcol[li].a);
                    vertexData[vi].color_ = color;
                }
            }

            int index[4];
            for (int i = 0; i < nloops; i++)
                index[i] = me->mloop[indexBase + i].v;

            bool isQuad = curpoly.totloop == 4;
            bool calcFaceNormal = !(curpoly.flag & ME_SMOOTH);

            if (isQuad)
                mesh->AddQuad(vertexData[index[0]], vertexData[index[1]], vertexData[index[2]], vertexData[index[3]], calcFaceNormal);
            else
                mesh->AddTriangle(vertexData[index[0]], vertexData[index[1]], vertexData[index[2]], calcFaceNormal);
        }
    }

    int BScene::GetUVLayersBMmesh(const Blender::Mesh* mesh, Blender::MLoopUV** uvEightLayerArray)
    {
        CHECK_ASSERT(mesh, __FILE__, __LINE__);

        int validLayers = 0;
        Blender::CustomDataLayer* layers = (Blender::CustomDataLayer*)mesh->ldata.layers;
        if (layers)
        {
            for (int i = 0; i < mesh->ldata.totlayer && validLayers < 8; i++)
            {
                if (layers[i].type == CD_MLOOPUV && uvEightLayerArray)
                {
                    Blender::MLoopUV* mtf = (Blender::MLoopUV*)layers[i].data;
                    if (mtf)
                        uvEightLayerArray[validLayers++] = mtf;
                }
            }
        }
        return validLayers;
    }

    const Blender::Material* BScene::GetMaterial(const Blender::Object* ob, int index) const
    {
        if (!ob || ob->totcol == 0) return 0;

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

    void BScene::SetMaterial(const Blender::Object* obj, PSceneNode sceneNode)
    {
        const Blender::Material* mt = GetMaterial(obj, 0);
        std::string name = B_IDNAME(mt);
        auto material = App::this_->GetMaterial(name);
        sceneNode->SetMaterial(material);
    }

    bool BScene::Save()
    {
        Path outputFile(outputDir_);
        outputFile.SetName("b" + path_.GetName());
        outputFile.SetExtension("xml");

        pugi::xml_document doc;

		if (embedResources_)
		{
			auto appNode = App::this_->Save(doc);
			scene_->Save(appNode);
		}
		else
		{
			auto appNode = App::this_->SaveWithExternalResources(doc, path_, outputDir_);
			scene_->Save(appNode);
		}
        TRACE_LOG("Saving file: " << outputFile.GetFullAbsoluteFilePath());
        return doc.save_file(outputFile.GetFullAbsoluteFilePath().c_str());
    }

}

