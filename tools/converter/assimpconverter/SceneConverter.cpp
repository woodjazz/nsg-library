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
#include "SceneConverter.h"
#include "SceneNode.h"
#include "ResourceFileManager.h"
#include "Check.h"
#include "Util.h"
#include "Material.h"
#include "Technique.h"
#include "Pass.h"
#include "MeshConverter.h"
#include "ModelMesh.h"
#include "MaterialConverter.h"
#include "LightConverter.h"
#include "CameraConverter.h"
#include "AnimationConverter.h"
#include "UtilConverter.h"
#include "App.h"
#include "Scene.h"
#include "Skeleton.h"
#include "assimp/IOStream.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "pugixml.hpp"
#include <chrono>
#include <thread>
#include <fstream>
#include <set>

namespace NSG
{
    class MyIOStream : public Assimp::IOStream
    {
        friend class SceneConverter;
    protected:
        MyIOStream(PResourceFile pResource)
            : pResource_(pResource),
              pos_(0)
        {
			CHECK_CONDITION(pResource_->IsReady(), __FILE__, __LINE__);
        }

    public:
        ~MyIOStream()
        {
        }

        size_t Read(void* pvBuffer, size_t pSize, size_t pCount)
        {
            const char* pData = pResource_->GetData();
            size_t totalBytes = pResource_->GetBytes();
            size_t bytes2Read = pSize * pCount;
            if (pos_ + bytes2Read > totalBytes)
            {
                return 0;
            }
            else
            {
                memcpy(pvBuffer, pData + pos_, bytes2Read);
                pos_ += bytes2Read;
                return pCount;
            }
        }

        size_t Write(const void* pvBuffer, size_t pSize, size_t pCount)
        {
            CHECK_ASSERT(false && "Write is not supported!!!", __FILE__, __LINE__);
            return 0;
        }

        aiReturn Seek(size_t pOffset, aiOrigin pOrigin)
        {
            switch (pOrigin)
            {
            case aiOrigin_SET:
                pos_ = pOffset;
                break;

            case aiOrigin_CUR:
                pos_ += pOffset;
                break;

            case aiOrigin_END:
                pos_ += pOffset;
                break;

            default:
                CHECK_ASSERT(false && "Incorrect pOrigin for Seek", __FILE__, __LINE__);
                return aiReturn_FAILURE;
            }

            return aiReturn_SUCCESS;
        }

        size_t Tell() const
        {
            return pos_;
        }

        size_t FileSize() const
        {
            return pResource_->GetBytes();
        }

        void Flush()
        {
        }

    private:
        PResourceFile pResource_;
        size_t pos_;
    };

    SceneConverter::SceneConverter(const Path& path)
        : path_(path)
    {
    }

    SceneConverter::~SceneConverter()
    {
    }

    bool SceneConverter::Load()
    {
        unsigned flags =
            aiProcess_JoinIdenticalVertices |
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_LimitBoneWeights |
            aiProcess_ImproveCacheLocality |
            aiProcess_RemoveRedundantMaterials |
            aiProcess_FixInfacingNormals |
            aiProcess_FindInvalidData |
            aiProcess_GenUVCoords |
            aiProcess_FindInstances |
            aiProcess_ValidateDataStructure |
            aiProcess_OptimizeMeshes;

        Assimp::Importer importer;
        importer.SetIOHandler(this);
        const aiScene* scene = importer.ReadFile(path_.GetFilePath().c_str(), flags);
        if (scene)
        {
            Load(scene);
        }
        else
        {
            TRACE_LOG("Error: Cannot recognize load file " << path_.GetFilePath() << " with error " << importer.GetErrorString());
        }

        importer.SetIOHandler(nullptr);

        return scene != nullptr;
    }

    void SceneConverter::Load(const aiScene* scene)
    {
        CachedData data;
        LoadMeshesAndMaterials(scene, data);
		scene_ = std::make_shared<Scene>(scene->mRootNode->mName.C_Str());
		RecursiveLoad(scene, scene->mRootNode, scene_.get(), data);
        LoadAnimations(scene);
        LoadBones(scene, data);
    }

    void SceneConverter::LoadAnimations(const aiScene* sc)
    {
        for (unsigned i = 0; i < sc->mNumAnimations; i++)
        {
            const aiAnimation* anim = sc->mAnimations[i];
            std::string animName = anim->mName.C_Str();
            if(animName.empty())
                animName = GetUniqueName("Animation");
            PAnimation outAnim = scene_->GetOrCreateAnimation(animName);
            AnimationConverter obj(scene_, anim, outAnim);
        }
    }

    const aiLight* SceneConverter::GetLight(const aiScene* sc, const aiString& name) const
    {
        for (size_t i = 0; i < sc->mNumLights; ++i)
        {
            const aiLight* light = sc->mLights[i];
            if (light->mName == name)
                return light;
        }

        return nullptr;
    }

    const aiCamera* SceneConverter::GetCamera(const aiScene* sc, const aiString& name) const
    {
        for (size_t i = 0; i < sc->mNumCameras; ++i)
        {
            const aiCamera* camera = sc->mCameras[i];
            if (camera->mName == name)
                return camera;
        }
        return nullptr;
    }

    void SceneConverter::LoadMeshesAndMaterials(const aiScene* sc, CachedData& data)
    {
        for (size_t i = 0; i < sc->mNumMeshes; ++i)
        {
            const struct aiMesh* mesh = sc->mMeshes[i];
            MeshConverter obj(mesh);
            data.meshes_.push_back(obj.GetMesh());
        }

        for (size_t i = 0; i < sc->mNumMaterials; ++i)
        {
            const aiMaterial* material = sc->mMaterials[i];
            MaterialConverter obj(material, path_.GetPath());
            data.materials_.push_back(obj.GetMaterial());
        }
    }

    void SceneConverter::LoadBones(const aiScene* sc, CachedData& data)
    {
        for (size_t i = 0; i < sc->mNumMeshes; ++i)
        {
            const struct aiMesh* aiMesh = sc->mMeshes[i];
            PModelMesh mesh = data.meshes_.at(i);
            LoadBones(sc, aiMesh, mesh);
            GetBlendData(mesh, aiMesh);
			if (mesh->GetSkeleton())
				MarkProgramAsSkinableNodes(mesh.get());
        }
    }

    aiNode* SceneConverter::GetNode(const std::string& name, aiNode* rootNode)
    {
        if (!rootNode)
            return nullptr;
        else if (name == rootNode->mName.C_Str())
            return rootNode;

        for (unsigned i = 0; i < rootNode->mNumChildren; ++i)
        {
            aiNode* found = GetNode(name, rootNode->mChildren[i]);
            if (found)
                return found;
        }
        return nullptr;
    }

    const aiNode* SceneConverter::GetMeshNode(const aiScene* sc, const aiNode* node, const aiMesh* aiMesh)
    {
        for (size_t i = 0; i < node->mNumMeshes; ++i)
        {
            unsigned meshIndex = node->mMeshes[i];
            if (aiMesh == sc->mMeshes[meshIndex])
                return node;
        }

        for (size_t i = 0; i < node->mNumChildren; ++i)
        {
            const aiNode* child = node->mChildren[i];
            const aiNode* result = GetMeshNode(sc, child, aiMesh);
            if (result) return result;
        }

        return nullptr;

    }

    Matrix4 SceneConverter::GetOffsetMatrix(const aiMesh* mesh, const aiNode* rootNode, const aiNode* node, const std::string& boneName)
    {
        for (unsigned j = 0; j < mesh->mNumBones; ++j)
        {
            aiBone* bone = mesh->mBones[j];
            if (boneName == bone->mName.C_Str())
            {
                aiMatrix4x4 offset = bone->mOffsetMatrix;
                return ToMatrix(offset);
            }
        }
        return IDENTITY_MATRIX;
    }
    void SceneConverter::LoadBones(const aiScene* sc, const aiMesh* aiMesh, PModelMesh mesh)
    {
        std::set<aiNode*> necessary;
        std::set<aiNode*> rootNodes;

        aiNode* rootNode = nullptr;
        const aiNode* meshNode = GetMeshNode(sc, sc->mRootNode, aiMesh);
        CHECK_ASSERT(meshNode, __FILE__, __LINE__);
        const aiNode* meshParentNode = meshNode->mParent;

        for (unsigned i = 0; i < aiMesh->mNumBones; ++i)
        {
            aiBone* bone = aiMesh->mBones[i];
            std::string boneName(bone->mName.C_Str());
            aiNode* boneNode = GetNode(boneName, sc->mRootNode);

            if (!boneNode)
            {
                TRACE_LOG("Could not find scene node for bone " << boneName);
                return;
            }

            necessary.insert(boneNode);
            rootNode = boneNode;

            for (;;)
            {
                boneNode = boneNode->mParent;
                if (!boneNode || boneNode == meshNode || boneNode == meshParentNode)
                    break;
                rootNode = boneNode;
                necessary.insert(boneNode);
            }

            if (rootNodes.find(rootNode) == rootNodes.end())
                rootNodes.insert(rootNode);
        }

        // If we find multiple root nodes, try to remedy by using their parent instead
        if (rootNodes.size() > 1)
        {
            aiNode* commonParent = (*rootNodes.begin())->mParent;
            for (auto i = rootNodes.begin(); i != rootNodes.end(); ++i)
            {
                if (*i != commonParent)
                {
                    if (!commonParent || (*i)->mParent != commonParent)
                    {
                        TRACE_LOG("Skeleton with multiple root nodes found, not supported");
                        return;
                    }
                }
            }
            rootNodes.clear();
            rootNodes.insert(commonParent);
            necessary.insert(commonParent);
        }

        if (rootNodes.empty())
            return;

        aiNode* rootBone = *rootNodes.begin();
        std::vector<aiNode*> bones;
        GetFinal(bones, necessary, rootBone);
        MakeSkeleton(aiMesh, mesh, rootBone, bones);
    }

    void SceneConverter::GetFinal(std::vector<aiNode*>& dest, const std::set<aiNode*>& necessary, aiNode* node)
    {
        if (necessary.find(node) != necessary.end())
            dest.push_back(node);

        for (unsigned i = 0; i < node->mNumChildren; ++i)
            GetFinal(dest, necessary, node->mChildren[i]);
    }

    void SceneConverter::MakeSkeleton(const aiMesh* aiMesh, PModelMesh mesh, const aiNode* rootBone, const std::vector<aiNode*>& bones)
    {
        PSkeleton skeleton(new Skeleton(mesh));
        PNode root = scene_->GetChild<Node>(rootBone->mName.C_Str(), true);
        std::vector<PWeakNode> nodeBones;
        for (auto bone : bones)
        {
            PNode node = scene_->GetChild<Node>(bone->mName.C_Str(), true);
            node->SetBoneOffsetMatrix(GetOffsetMatrix(aiMesh, rootBone, bone, node->GetName()));
            nodeBones.push_back(node);
        }
        skeleton->SetRoot(root);
        skeleton->SetBones(nodeBones);
        mesh->SetSkeleton(skeleton);
    }

	void SceneConverter::MarkProgramAsSkinableNodes(const Mesh* mesh)
    {
		auto& nodes = mesh->GetConstSceneNodes();

		for (auto obj : nodes)
		{
			PMaterial material = obj->GetMaterial();
			if (material)
			{
				PTechnique technique = material->GetTechnique();
				technique->EnableProgramFlags((int)ProgramFlag::SKINNED);
			}
		}
    }

    void SceneConverter::GetBlendData(PMesh mesh, const aiMesh* aiMesh) const
    {
        PSkeleton skeleton = mesh->GetSkeleton();
        if (!skeleton) return;
        std::vector<std::vector<unsigned>> blendIndices;
        std::vector<std::vector<float>> blendWeights;

        blendIndices.resize(aiMesh->mNumVertices);
        blendWeights.resize(aiMesh->mNumVertices);

        CHECK_CONDITION(mesh->GetVertexsData().size() == aiMesh->mNumVertices, __FILE__, __LINE__);

        for (unsigned i = 0; i < aiMesh->mNumBones; ++i)
        {
            aiBone* bone = aiMesh->mBones[i];
            std::string boneName = bone->mName.C_Str();
            unsigned index = skeleton->GetBoneIndex(boneName);
            if (index == -1)
            {
                TRACE_LOG("Bone " << boneName << " not found");
                return;
            }
            for (unsigned j = 0; j < bone->mNumWeights; ++j)
            {
                unsigned vertex = bone->mWeights[j].mVertexId;
                blendIndices[vertex].push_back(index);
                blendWeights[vertex].push_back(bone->mWeights[j].mWeight);
                if (blendWeights[vertex].size() > 4)
                {
                    TRACE_LOG("More than 4 bone influences on vertex");
                    return;
                }
            }
        }

        CHECK_CONDITION(mesh->GetVertexsData().size() == blendIndices.size(), __FILE__, __LINE__);
        CHECK_CONDITION(mesh->GetVertexsData().size() == blendWeights.size(), __FILE__, __LINE__);

        skeleton->SetBlendData(blendIndices, blendWeights);
    }

    void SceneConverter::RecursiveLoad(const aiScene* sc, const aiNode* nd, SceneNode* sceneNode, const CachedData& data)
    {
        //sceneNode->SetName(nd->mName.C_Str());

        {
            aiVector3t<float> scaling;
            aiQuaterniont<float> rotation;
            aiVector3t<float> position;

            nd->mTransformation.Decompose(scaling, rotation, position);

            sceneNode->SetPosition(Vertex3(position.x, position.y, position.z));
            sceneNode->SetOrientation(Quaternion(rotation.w, rotation.x, rotation.y, rotation.z));
            sceneNode->SetScale(Vertex3(scaling.x, scaling.y, scaling.z));
        }

		if (dynamic_cast<Light*>(sceneNode))
		{
			// Lights
			const aiLight* light = GetLight(sc, nd->mName);
			CHECK_ASSERT(light, __FILE__, __LINE__);
			LightConverter obj(light, dynamic_cast<Light*>(sceneNode));
		}
		else if (dynamic_cast<Camera*>(sceneNode))
		{
			// Cameras
			const aiCamera* camera = GetCamera(sc, nd->mName);
			CHECK_ASSERT(camera, __FILE__, __LINE__);
			CameraConverter obj(camera, dynamic_cast<Camera*>(sceneNode));
		}

        SceneNode* meshSceneNode = sceneNode;
        for (size_t i = 0; i < nd->mNumMeshes; ++i)
        {
            const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[i]];
            unsigned int meshIndex = nd->mMeshes[i];
            meshSceneNode->SetMesh(data.meshes_.at(meshIndex));
            unsigned int materialIndex = mesh->mMaterialIndex;
            meshSceneNode->SetMaterial(data.materials_.at(materialIndex));
			if (i + 1 < nd->mNumMeshes)
				meshSceneNode = meshSceneNode->GetOrCreateChild<SceneNode>(GetUniqueName(meshSceneNode->GetName())).get();
        }

        for (size_t i = 0; i < nd->mNumChildren; ++i)
        {
            const aiNode* ndChild = nd->mChildren[i];
			SceneNode* child = nullptr;
			const aiLight* light = GetLight(sc, ndChild->mName);
			const aiCamera* camera = GetCamera(sc, ndChild->mName);
			if (light)
			{
				CHECK_ASSERT(!camera, __FILE__, __LINE__);
				child = sceneNode->GetOrCreateChild<Light>(ndChild->mName.C_Str()).get();
			}
			else if (camera)
			{
				child = sceneNode->GetOrCreateChild<Camera>(ndChild->mName.C_Str()).get();
			}
			else
			{
				child = sceneNode->GetOrCreateChild<SceneNode>(ndChild->mName.C_Str()).get();
			}
            
			RecursiveLoad(sc, ndChild, child, data);
        }
    }

    bool SceneConverter::Exists(const char* filename) const
    {
        std::ifstream obj(filename);
        return obj.is_open();
    }

    char SceneConverter::getOsSeparator() const
    {
        return '/';
    }

    Assimp::IOStream* SceneConverter::Open(const char* filename, const char* mode)
    {
		auto resource = std::make_shared<ResourceFile>(filename);
        return new MyIOStream(resource);
    }

    void SceneConverter::Close(Assimp::IOStream* pFile)
    {
        delete pFile;
    }

    bool SceneConverter::Save(const std::string& filename)
    {
        pugi::xml_document doc;
        scene_->Save(doc);
        return doc.save_file(filename.c_str());
    }
}
