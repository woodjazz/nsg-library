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
#include "Resource.h"
#include "Check.h"
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
			const unsigned char* pData = pResource_->GetData();
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

	Model::Model()
	{

	}

	Model::~Model()
	{
		
	}

	void Model::Load(const char* filename)
	{
		Assimp::Importer importer;
		importer.SetIOHandler(this);
		const aiScene* pScene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);	
  		importer.SetIOHandler(nullptr);	
  		RecursiveLoad(pScene, pScene->mRootNode);
	}

	void Model::RecursiveLoad(const aiScene *sc, const aiNode* nd)
	{
		Matrix4 localModel;
		CopyMat(&nd->mTransformation, localModel);

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
		PResource pResource(new Resource(filename));

		while(!pResource->IsReady())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		return new MyIOStream(pResource); 
	}

	void Model::Close(Assimp::IOStream* pFile) 
	{ 
		delete pFile; 
	}

}