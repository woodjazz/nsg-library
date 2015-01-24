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

namespace BlenderConverter
{
    using namespace NSG;
    BScene::BScene(const Path& path, const Path& outputDir)
        : path_(path),
          outputDir_(outputDir)
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
                LoadSkeleton(objArmature);
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

    PTexture BScene::CreateTexture(const std::string& fileName)
    {
        Path outputPath;
        outputPath.SetPath(outputDir_.GetPath());
        outputPath.SetFileName(fileName);
        Path inputPath;
        inputPath.SetPath(path_.GetPath());
        inputPath.SetFileName(fileName);
        CHECK_CONDITION(NSGCopyFile(inputPath, outputPath), __FILE__, __LINE__);

        std::vector<std::string> dirs = Path::GetDirs(outputPath.GetPath());
        Path relativePath;
        if (!dirs.empty())
            relativePath.SetPath(dirs.back());
        relativePath.SetFileName(fileName);

        auto resource = std::make_shared<ResourceFile>(relativePath);
        return std::make_shared<Texture>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
    }


    void BScene::LoadMaterial(const Blender::Material* mt)
    {
        std::string name = B_IDNAME(mt);
        auto material = App::this_->GetOrCreateMaterial(name);
        auto technique = material->GetTechnique();
        auto pass = technique->GetPass(0);
        auto program = pass->GetProgram();
        ProgramFlags flags = (int)ProgramFlag::PER_VERTEX_LIGHTING;
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
                    Blender::MTex* mtex = mt->mtex[i];
                    Blender::Image* ima = mtex->tex->ima;
                    if (!ima) continue;
                    std::string imageName = B_IDNAME(ima);
                    auto texture = CreateTexture(imageName);
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

            if (obj->adt)
            {
                float animfps = sc_->r.frs_sec / sc_->r.frs_sec_base;
                LoadAnimData(sceneNode, obj->adt, animfps);
            }
        }
    }

    struct TrackData
    {
        PAnimationTrack track;
        std::vector<PBSpline> keyframes;
    };
    typedef std::shared_ptr<TrackData> PTrackData;
    std::map<std::string, PTrackData> tracks;

    void BScene::LoadAnimData(PSceneNode sceneNode, const Blender::AnimData* adt, float animfps)
    {
        tracks.clear();
        std::string name(B_IDNAME(adt->action));
        if (scene_->HasAnimation(name))
        {
            auto anim = scene_->GetOrCreateAnimation(name);
            //gkAnimationPlayer* play = anim->Ad->addAnimation(act, GKB_IDNAME(adt->action));
            //play->setWeight(adt->act_influence);
        }
        else
        {
            auto anim = scene_->GetOrCreateAnimation(name);
            auto length = ConvertAction(adt->action, animfps);
            anim->SetLength(length);
        }
    }

    float BScene::ConvertAction(const Blender::bAction* action, float animfps)
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
                chan_name = "GKMainObjectChannel";
            }

            auto it = tracks.find(chan_name);
            if (it == tracks.end())
                trackData = tracks[chan_name] = std::make_shared<TrackData>();
            else
                trackData = it->second;

            if (bfc->bezt)
            {
                int code = -1;
                if (transform_name == "rotation_quaternion")
                {
                    if (bfc->array_index == 0) code = SC_ROT_QUAT_W;
                    else if (bfc->array_index == 1) code = SC_ROT_QUAT_X;
                    else if (bfc->array_index == 2) code = SC_ROT_QUAT_Y;
                    else if (bfc->array_index == 3) code = SC_ROT_QUAT_Z;
                }
                else if (transform_name == "rotation_euler")
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
                break; //FIX: Momo_WalkBack fcurve is broken in uncompressed 256a.

            bfc = bfc->next;
        }

        return trackLength;
    }

    PBSpline BScene::ConvertSpline(const Blender::BezTriple* bez, int access, int mode, int totvert, float xoffset, float xfactor, float yoffset, float yfactor)
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
        start = FLT_MAX;
        end = -FLT_MAX;

        float tstart, tend;

        Blender::FCurve* bfc = (Blender::FCurve*)action->curves.first;
        while (bfc)
        {
            GetSplineStartEnd(bfc->bezt, bfc->totvert, tstart, tend);
            if (start > tstart) start = tstart;
            if (end < tend) end = tend;

            //GK_ASSERT(!bfc->next || (bfc->next && bfc->next->prev == bfc));
            if (bfc->next == 0 || bfc->next->prev != bfc)
                break; //FIX: Momo_WalkBack fcurve is broken in uncompressed 256a.

            bfc = bfc->next;
        }
    }

    void BScene::GetSplineStartEnd(const Blender::BezTriple* bez, int totvert, float& start, float& end)
    {
        start = FLT_MAX;
        end = -FLT_MAX;

        const Blender::BezTriple* bezt = bez;
        for (int c = 0; c < totvert; c++, bezt++)
        {
            if (start > bezt->vec[1][0]) start = bezt->vec[1][0];
            if (end < bezt->vec[1][0]) end = bezt->vec[1][0];
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
        // create bone lists && transforms
        const Blender::Bone* bone = static_cast<const Blender::Bone*>(ar->bonebase.first);
        while (bone)
        {
            if (!bone->parent)
                BuildBoneTree(bone, nullptr, sceneNode);
            bone = bone->next;
        }
    }

    void BScene::BuildBoneTree(const Blender::Bone* cur, const Blender::Bone* prev, PSceneNode parent)
    {
        Matrix4 parBind = IDENTITY_MATRIX;
        if (prev != 0 && parent != 0)
            parBind = glm::inverse(ToMatrix(prev->arm_mat));

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
            // recurse
            BuildBoneTree(chi, cur, bone);
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
        Blender::Mesh* me = (Blender::Mesh*)obj->data;
        std::string meshName = B_IDNAME(me);
        auto mesh = App::this_->GetModelMesh(B_IDNAME(me));
        if (!mesh)
        {
            mesh = App::this_->GetOrCreateModelMesh(B_IDNAME(me));
            ConvertMesh(me, mesh);
            LoadBones(obj, me, mesh);
        }
        sceneNode->SetMesh(mesh);
        SetMaterial(obj, sceneNode);
    }

    void BScene::LoadBones(const Blender::Object* obj, const Blender::Mesh* me, PModelMesh mesh)
    {
        if (me->dvert && obj->parent && obj->parent->type == OB_ARMATURE)
        {
            VertexsData& vertexes = mesh->GetVertexsData();
            CHECK_ASSERT(me->totvert == vertexes.size(), __FILE__, __LINE__);
            int dgi = 0;
            const Blender::bDeformGroup* dg = (const Blender::bDeformGroup*)obj->defbase.first;
            while (dg)
            {
                const Blender::MDeformVert* dvert = me->dvert;
                for (int n = 0; n < me->totvert; n++)
                {
                    const Blender::MDeformVert& dv = dvert[n];
                    int nWeights = glm::clamp(dv.totweight, 0, 4);
                    for (int w = 0; w < nWeights; w++)
                    {
                        const Blender::MDeformWeight& deform = dv.dw[w];
                        vertexes[n].bonesID_[w] = float(deform.def_nr + 1);
                        vertexes[n].bonesWeight_[w] = deform.weight;
                    }
                }
                dg = dg->next;
                ++dgi;
            }
        }
    }

    void BScene::LoadSkeleton(const Blender::Object* obj)
    {
        CHECK_ASSERT(obj->type == OB_MESH && obj->parent != 0 && obj->parent->type == OB_ARMATURE, __FILE__, __LINE__);
        const Blender::Mesh* me = (Blender::Mesh*)obj->data;
        auto mesh = App::this_->GetModelMesh(B_IDNAME(me));
        CHECK_ASSERT(mesh, __FILE__, __LINE__);

        const Blender::Object* obAr = obj->parent;
        const Blender::bArmature* ar = static_cast<const Blender::bArmature*>(obAr->data);

        auto skeleton(std::make_shared<Skeleton>(mesh));
        std::vector<PWeakNode> nodeBones;

        PNode rootNode = scene_->GetChild<Node>(B_IDNAME(obAr), true);
        nodeBones.push_back(rootNode);

        const Blender::Bone* bone = static_cast<const Blender::Bone*>(ar->bonebase.first);
        while (bone)
        {
            if (!bone->parent)
                MakeBonesList(bone, nodeBones);
            bone = bone->next;
        }
        skeleton->SetRoot(rootNode);
        mesh->SetSkeleton(skeleton);
        skeleton->SetBones(nodeBones);
    }

    void BScene::MakeBonesList(const Blender::Bone* bone, std::vector<PWeakNode>& nodeBones)
    {
        PNode node = scene_->GetChild<Node>(bone->name, true);
        node->SetBoneOffsetMatrix(Matrix4(ToMatrix(bone->bone_mat)));
        nodeBones.push_back(node);

        Blender::Bone* chi = static_cast<Blender::Bone*>(bone->childbase.first);
        while (chi)
        {
            MakeBonesList(chi, nodeBones);
            chi = chi->next;
        }
    }

    void BScene::ConvertMesh(Blender::Mesh* me, PModelMesh mesh)
    {
        CHECK_ASSERT(!me->mface && "Legacy conversion is not allowed", __FILE__, __LINE__);
        CHECK_ASSERT(me->mvert && me->mpoly, __FILE__, __LINE__);

        // UV-Layer-Data
        Blender::MTexPoly* mtpoly[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        Blender::MLoopUV* muvs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        int totlayer = 0;

        GetLayersBMmesh(me, mtpoly, muvs, &me->mloopcol, totlayer);

        int nVertexes = me->totvert;
        int nuvs = glm::clamp(totlayer, 0, 2);

        VertexsData vertexData(nVertexes);
        for (int i = 0; i < nVertexes; i++)
        {
            vertexData[i].position_ = Vertex3(me->mvert[i].co[0], me->mvert[i].co[1], me->mvert[i].co[2]);
            vertexData[i].normal_ = glm::normalize(Vertex3(me->mvert[i].no[0], me->mvert[i].no[1], me->mvert[i].no[2]));
        }

        bool hasColorVertex = me->mloopcol && me->totcol;
        Indexes indexes;
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

            int index[4] = { me->mloop[indexBase].v, me->mloop[indexBase + 1].v, me->mloop[indexBase + 2].v, 0};

            if (hasColorVertex)
            {
                Color color(me->mloopcol[indexBase].r, me->mloopcol[indexBase].g, me->mloopcol[indexBase].b, me->mloopcol[indexBase].a);
                vertexData[index[indexBase]].color_ = color;
            }

            bool isQuad = curpoly.totloop == 4;

            if (isQuad)
            {
                index[3] = me->mloop[indexBase + 3].v;

                indexes.push_back(index[0]);
                indexes.push_back(index[1]);
                indexes.push_back(index[2]);

                indexes.push_back(index[0]);
                indexes.push_back(index[2]);
                indexes.push_back(index[3]);
            }
            else
            {
                indexes.push_back(index[0]);
                indexes.push_back(index[1]);
                indexes.push_back(index[2]);
            }

            for (int i = 0; i < nloops; i++)
            {
                int li = indexBase + i;
                int vi = me->mloop[li].v;
                if (nuvs > 0)
                    vertexData[vi].uv_[0] = Vertex2(muvs[0][li].uv[0], muvs[0][li].uv[1]);
                if (nuvs > 1)
                    vertexData[vi].uv_[1] = Vertex2(muvs[1][li].uv[0], muvs[1][li].uv[1]);
            }
        }
        mesh->SetData(vertexData, indexes);
    }
    #if 0
    void BScene::CalcNormal(TempFace* tri)
    {
        Vector3 v1 = tri->v1.position_ - tri->v2.position_;
        Vector3 v2 = tri->v2.position_ - tri->v0.position_;
        Vector3 n = glm::cross(v1, v2);
        if (glm::length2(n) > 0)
            n = glm::normalize(n);
        else if (glm::length2(v1) > 0)
            n = glm::normalize(v1);
        else
        {
            CHECK_ASSERT(glm::length2(v2) > 0, __FILE__, __LINE__);
            n = glm::normalize(v2);
        }
        tri->v0.normal_ = tri->v1.normal_ = tri->v2.normal_ = n;
    }
    #endif
    void BScene::GetLayersBMmesh(Blender::Mesh* mesh, Blender::MTexPoly** eightLayerArray, Blender::MLoopUV** uvEightLayerArray, Blender::MLoopCol** oneMCol, int& validLayers)
    {
        CHECK_ASSERT(mesh, __FILE__, __LINE__);

        validLayers = 0;

        // Poly-Data: Textures,...
        Blender::CustomDataLayer* layers = (Blender::CustomDataLayer*)mesh->pdata.layers;
        if (layers)
        {
            // push valid layers
            for (int i = 0; i < mesh->pdata.totlayer && validLayers < 8; i++)
            {
                if (layers[i].type == CD_MTEXPOLY && eightLayerArray)
                {
                    Blender::MTexPoly* mtf = (Blender::MTexPoly*)layers[i].data;
                    if (mtf)
                        eightLayerArray[validLayers++] = mtf;
                }
            }
        }
        else
        {
            // TODO: Check this
            if (eightLayerArray && mesh->mtpoly)
                eightLayerArray[validLayers++] = mesh->mtpoly;
            // TODO: Check this
            if (oneMCol && mesh->mloopcol)
                *oneMCol = mesh->mloopcol;
        }

        // this have to be equal to validLayers afterwards,right?
        int uvValidLayers = 0;
        // Loop-Data: UV-Data,..
        layers = (Blender::CustomDataLayer*)mesh->ldata.layers;
        if (layers)
        {
            // push valid layers
            for (int i = 0; i < mesh->ldata.totlayer && uvValidLayers < 8; i++)
            {
                if (layers[i].type == CD_MLOOPUV && uvEightLayerArray)
                {
                    Blender::MLoopUV* mtf = (Blender::MLoopUV*)layers[i].data;
                    if (mtf)
                        uvEightLayerArray[uvValidLayers++] = mtf;
                }
            }
        }
        else
        {
            // TODO: Check this
            if (oneMCol && mesh->mloopcol)
                *oneMCol = mesh->mloopcol;
        }

        if (uvValidLayers != validLayers)
        {
            TRACE_LOG("Warning! gkMeshConverter gkLoaderUtilsGetLayers: validLayers!=validUvLayers!");
        }
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
        scene_->Save(doc);
        TRACE_LOG("Saving file: " << outputFile.GetFullAbsoluteFilePath());
        return doc.save_file(outputFile.GetFullAbsoluteFilePath().c_str());
    }

}

