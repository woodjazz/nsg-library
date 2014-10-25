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
#include "Context.h"
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
            CHECK_CONDITION(pResource_->IsLoaded(), __FILE__, __LINE__);
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
        : scene_(App::this_->GetCurrentScene()),
          path_(path)
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
		PSceneNode root = scene_->CreateSceneNode("RootNode");
		RecursiveLoad(scene, scene->mRootNode, root.get(), data);
        LoadAnimations(scene);
        LoadBones(scene, data);
		MarkProgramAsSkinableNodes();
    }

    void SceneConverter::LoadAnimations(const aiScene* sc)
    {
        for (unsigned i = 0; i < sc->mNumAnimations; i++)
        {
            const aiAnimation* anim = sc->mAnimations[i];
            std::string animName = anim->mName.C_Str();
            if (!scene_->HasAnimation(animName))
            {
                PAnimation outAnim = scene_->CreateAnimation(animName);
                AnimationConverter obj(scene_, anim, outAnim);
            }
            else
            {
                TRACE_LOG("Animation " << animName << " already exists!!!");
            }
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
            PMesh mesh = data.meshes_.at(i);
            LoadBones(sc, aiMesh, mesh);
            GetBlendData(mesh, aiMesh);
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

    void SceneConverter::LoadBones(const aiScene* sc, const aiMesh* aiMesh, PMesh mesh)
    {
        std::set<aiNode*> necessary;
        std::set<aiNode*> rootNodes;

        aiNode* rootNode = nullptr;

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

			{
				std::vector<PNode> nodes;
				Node::GetChildrenRecursive(scene_, boneName, nodes);
				CHECK_CONDITION(nodes.size() == 1, __FILE__, __LINE__);
				nodes[0]->SetBoneOffsetMatrix(ToMatrix(bone->mOffsetMatrix));
			}


            necessary.insert(boneNode);
            rootNode = boneNode;

            for (;;)
            {
                boneNode = boneNode->mParent;
                if (!boneNode)
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

        MakeSkeleton(mesh, *rootNodes.begin(), necessary);
    }

    void SceneConverter::MakeSkeleton(PMesh mesh, const aiNode* rootBone, const std::set<aiNode*>& bones)
    {
        PSkeleton skeleton(new Skeleton(mesh));
        std::vector<PNode> nodes;
        Node::GetChildrenRecursive(scene_, rootBone->mName.C_Str(), nodes);
        CHECK_CONDITION(nodes.size() == 1, __FILE__, __LINE__);
        PNode root = nodes[0];
        std::vector<PNode> nodeBones;
        for (auto obj : bones)
        {
            std::string name = obj->mName.C_Str();
            nodes.clear();
            Node::GetChildrenRecursive(scene_, name, nodes);
            CHECK_CONDITION(nodes.size() == 1, __FILE__, __LINE__);
            nodeBones.push_back(nodes[0]);
        }
        skeleton->SetRoot(root);
        skeleton->SetBones(nodeBones);
        mesh->SetSkeleton(skeleton);
    }

	void SceneConverter::MarkProgramAsSkinableNodes()
    {
		std::vector<SceneNode*> nodes = Node::GetChildrenRecursiveOfType<SceneNode>(scene_);
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
        if(!skeleton) return;
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
        sceneNode->SetName(nd->mName.C_Str());

        {
            aiVector3t<float> scaling;
            aiQuaterniont<float> rotation;
            aiVector3t<float> position;

            nd->mTransformation.Decompose(scaling, rotation, position);

            sceneNode->SetPosition(Vertex3(position.x, position.y, position.z));
            sceneNode->SetOrientation(Quaternion(rotation.w, rotation.x, rotation.y, rotation.z));
            sceneNode->SetScale(Vertex3(scaling.x, scaling.y, scaling.z));
        }

        for (size_t i = 0; i < nd->mNumMeshes; ++i)
        {
            const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[i]];

            std::stringstream ss;
            ss << sceneNode->GetName() << "_" << i;

            PSceneNode meshSceneNode = scene_->CreateSceneNode(ss.str());
            sceneNode->AddChild(meshSceneNode);
            unsigned int meshIndex = nd->mMeshes[i];
            meshSceneNode->Set(data.meshes_.at(meshIndex));
            unsigned int materialIndex = mesh->mMaterialIndex;
            meshSceneNode->Set(data.materials_.at(materialIndex));
        }

        {
            // Lights
            const aiLight* light = GetLight(sc, nd->mName);
            if (light)
            {
                PLightConverter obj(new LightConverter(light, scene_.get()));
                sceneNode->AddChild(obj->GetLight());
            }
        }

        {
            // Cameras
            const aiCamera* camera = GetCamera(sc, nd->mName);
            if (camera)
            {
                PCameraConverter obj(new CameraConverter(camera, scene_.get()));
                sceneNode->AddChild(obj->GetCamera());
            }
        }

        for (size_t i = 0; i < nd->mNumChildren; ++i)
        {
            const aiNode* ndChild = nd->mChildren[i];
            PSceneNode child = sceneNode->CreateChild(ndChild->mName.C_Str());
            RecursiveLoad(sc, ndChild, child.get(), data);
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
        PResourceFile resource(ResourceFileManager::this_->GetOrCreate(filename));
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
