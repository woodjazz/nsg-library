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
#include "Model.h"
#include "SceneNode.h"
#include "ResourceFile.h"
#include "Check.h"
#include "Util.h"
#include "Context.h"
#include "Technique.h"
#include "Pass.h"
#include "ModelMesh.h"
#include "ModelMaterial.h"
#include "assimp/IOStream.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
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

	class ModelSceneNode : public SceneNode
	{
	public:
		ModelSceneNode(Model* model)
		: model_(model)
		{

		}

		~ModelSceneNode()
		{
		}

		void Render()
		{
	    	if(model_->IsReady())
	    	{
	        	SceneNode::Render();
	        }
		}
#if 0
		void Render2Select()
		{
	    	if(model_->IsReady())
	    	{
	        	SceneNode::Render2Select();
	        }
		}
#endif		

	private:
		Model* model_;
	};


	class MyIOStream : public Assimp::IOStream
	{
		friend class Model;
		protected:
		MyIOStream(PResource pResource)
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
			if(pos_ + bytes2Read > totalBytes)
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
			switch(pOrigin)
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
		PResource pResource_;
		size_t pos_;
	};	

	Model::Model(const char* filename)
	: pResource_(new ResourceFile(filename)),
    filename_(filename)
	{
        pRoot_= PSceneNode(new ModelSceneNode(this));
	}

	Model::~Model()
	{
		Context::this_->Remove(this);
	}

	bool Model::IsValid()
	{
		return pResource_->IsLoaded();
	}

	void Model::AllocateResources()
	{
        pModelRoot_ = PSceneNode(new SceneNode());
        pModelRoot_->SetParent(pRoot_);

		Assimp::Importer importer;
		importer.SetIOHandler(this);
	    const aiScene* pScene = importer.ReadFile(filename_.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);	

	    RecursiveLoad(pScene, pScene->mRootNode, pModelRoot_);
	    
	    importer.SetIOHandler(nullptr);	
	}

	void Model::ReleaseResources()
	{
		pModelRoot_ = nullptr;
		children_.clear();
	}

    void Model::SetBehavior(PBehavior pBehavior)
    {
    	pRoot_->SetBehavior(pBehavior);
    }

	void Model::RecursiveLoad(const aiScene* sc, const aiNode* nd, PSceneNode pSceneNode)
	{
		Matrix4 localModel;
		CopyMat(&nd->mTransformation, localModel);
		Vertex3 position;
		Quaternion q;
		Vertex3 scale;
		DecomposeMatrix(localModel, position, q, scale);
		pSceneNode->EnableUpdate(false);
		pSceneNode->SetPosition(position);
		pSceneNode->SetOrientation(q);
		pSceneNode->SetScale(scale);
		pSceneNode->EnableUpdate(true);
		pSceneNode->Update(false);

		for (size_t i=0; i < nd->mNumMeshes; ++i) 
		{
			PSceneNode pMeshSceneNode(new SceneNode());
            children_.push_back(pMeshSceneNode);
			pMeshSceneNode->SetParent(pSceneNode);

			const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[i]];

			PModelMesh pMesh(new ModelMesh(GL_STATIC_DRAW, mesh));

            PTechnique technique(new Technique);
            pMeshSceneNode->Set(technique);
            PPass pass(new Pass);
            pass->Add(pMeshSceneNode.get(), pMesh);
            technique->Add(pass);
			PModelMaterial pMaterial(new ModelMaterial(sc->mMaterials[mesh->mMaterialIndex]));
            pass->Set(pMaterial);
		}

		for (size_t i=0; i<nd->mNumChildren; ++i) 
		{
			PSceneNode pChild(new SceneNode());
			children_.push_back(pChild);
			pChild->SetParent(pSceneNode);

			RecursiveLoad(sc, nd->mChildren[i], pChild);
		}
	}

	bool Model::Exists(const char* filename) const 
	{
		return true;
	}

	char Model::getOsSeparator() const 
	{ 
		return '/'; 
	}

	Assimp::IOStream* Model::Open(const char* filename, const char* mode) 
	{
		return new MyIOStream(pResource_); 
	}

	void Model::Close(Assimp::IOStream* pFile) 
	{ 
		delete pFile; 
	}

	//////////////////////////////////////////////////////////////////////////////////

}