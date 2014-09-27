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
#include "ResourceFile.h"
#include "Check.h"
#include "Util.h"
#include "Context.h"
#include "Technique.h"
#include "Pass.h"
#include "MeshConverter.h"
#include "MaterialConverter.h"
#include "LightConverter.h"
#include "CameraConverter.h"
#include "assimp/IOStream.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "pugixml.hpp"
#include <chrono>
#include <thread>

namespace NSG
{
    static void CopyMat(const aiMatrix4x4* from, glm::mat4& to)
    {
        to[0][0] = from->a1; to[1][0] = from->a2;
        to[2][0] = from->a3; to[3][0] = from->a4;
        to[0][1] = from->b1; to[1][1] = from->b2;
        to[2][1] = from->b3; to[3][1] = from->b4;
        to[0][2] = from->c1; to[1][2] = from->c2;
        to[2][2] = from->c3; to[3][2] = from->c4;
        to[0][3] = from->d1; to[1][3] = from->d2;
        to[2][3] = from->d3; to[3][3] = from->d4;
    }


    class MyIOStream : public Assimp::IOStream
    {
        friend class SceneConverter;
    protected:
        MyIOStream(PResourceFile pResource)
            : pResource_(pResource),
              pos_(0)
        {
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

    SceneConverter::SceneConverter(PResourceFile resource)
        : pResource_(resource)
    {
        CHECK_ASSERT(resource->IsLoaded(), __FILE__, __LINE__);

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
            aiProcess_OptimizeMeshes;

        Assimp::Importer importer;
        importer.SetIOHandler(this);
        const aiScene* scene = importer.ReadFile(resource->GetFilename().c_str(), flags);
        Load(scene);
        importer.SetIOHandler(nullptr);
    }

    SceneConverter::~SceneConverter()
    {
    }

    void SceneConverter::Load(const aiScene* scene)
    {
        LoadMeshesAndMaterials(scene);
        RecursiveLoad(scene, scene->mRootNode, this);
    }

    const aiLight* SceneConverter::GetLight(const aiScene* sc, const aiString& name) const
    {
        for (size_t i = 0; i < sc->mNumLights; ++i)
        {
            const aiLight* light = sc->mLights[i];
            if(light->mName == name)
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

    void SceneConverter::LoadMeshesAndMaterials(const aiScene* sc)
    {
        for (size_t i = 0; i < sc->mNumMeshes; ++i)
        {
            const struct aiMesh* mesh = sc->mMeshes[i];
            MeshConverter obj(mesh);
        }

        for (size_t i = 0; i < sc->mNumMaterials; ++i)
        {
            const aiMaterial* material = sc->mMaterials[i];
            MaterialConverter obj(material);
        }
    }

    void SceneConverter::RecursiveLoad(const aiScene* sc, const aiNode* nd, SceneNode* sceneNode)
    {
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

            PSceneNode meshSceneNode = CreateSceneNode(ss.str());
            sceneNode->AddChild(meshSceneNode);
            meshSceneNode->SetMeshIndex(nd->mMeshes[i]);
            meshSceneNode->SetMaterialIndex(mesh->mMaterialIndex);
        }

        {
            const aiLight* light = GetLight(sc, nd->mName);
            if(light)
            {
                PLightConverter obj(new LightConverter(light, this));
                sceneNode->AddChild(obj);
            }
        }

		{
			const aiCamera* camera = GetCamera(sc, nd->mName);
			if (camera)
			{
				PCameraConverter obj(new CameraConverter(camera, this));
				sceneNode->AddChild(obj);
			}
		}


        for (size_t i = 0; i < nd->mNumChildren; ++i)
        {
            const aiNode* ndChild = nd->mChildren[i];
			PSceneNode child = sceneNode->CreateChild(ndChild->mName.C_Str());
            RecursiveLoad(sc, ndChild, child.get());
        }
    }

    bool SceneConverter::Exists(const char* filename) const
    {
        return true;
    }

    char SceneConverter::getOsSeparator() const
    {
        return '/';
    }

    Assimp::IOStream* SceneConverter::Open(const char* filename, const char* mode)
    {
        return new MyIOStream(pResource_);
    }

    void SceneConverter::Close(Assimp::IOStream* pFile)
    {
        delete pFile;
    }

    bool SceneConverter::Save(const std::string& filename)
    {
        pugi::xml_document doc;
        Scene::Save(doc);
        return doc.save_file(filename.c_str());
    }
}
