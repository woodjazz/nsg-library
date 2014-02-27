#include "GLES2Material.h"
#include "Log.h"
#include "Camera.h"
#include <assert.h>

namespace NSG
{
	GLES2Material::GLES2Material(PGLES2Texture pTexture, PGLES2Program pProgram) 
	: pTexture_(pTexture), 
	pProgram_(pProgram),
    mvp_loc_(-1),
    m_loc_(-1),
	model_inv_transp_loc_(-1),
	color_difusse_loc_(-1),
	texture_loc_(-1),
	texcoord_loc_(-1),
	position_loc_(-1),
	normal_loc_(-1),
	color_loc_(-1),
	loaded_(false),
	diffuse_(1,1,1,1)
	{
        assert(glGetError() == GL_NO_ERROR);
	}

	GLES2Material::~GLES2Material()
	{
	}

	bool GLES2Material::IsReady()
	{
		if(pProgram_->IsReady() && (!pTexture_ || pTexture_->IsReady()))
		{
			if(!loaded_)
			{
				assert(glGetError() == GL_NO_ERROR);

				color_difusse_loc_ = pProgram_->GetUniformLocation("u_material.diffuse");
			    mvp_loc_ = pProgram_->GetUniformLocation("u_mvp");
			    m_loc_ = pProgram_->GetUniformLocation("u_m");
				model_inv_transp_loc_ = pProgram_->GetUniformLocation("u_model_inv_transp");
				texture_loc_ = pProgram_->GetUniformLocation("u_texture");
				texcoord_loc_ = pProgram_->GetAttributeLocation("a_texcoord");
				position_loc_ = pProgram_->GetAttributeLocation("a_position");
				normal_loc_ = pProgram_->GetAttributeLocation("a_normal");
				color_loc_ = pProgram_->GetAttributeLocation("a_color");

				assert(glGetError() == GL_NO_ERROR);

				loaded_ = true;
			}

			return true;
		}

		return false;
	}

	void GLES2Material::SetDiffuseColor(Color diffuse)
	{
		diffuse_ = diffuse;
	}

	UseMaterial::UseMaterial(GLES2Material& obj, PNode pNode)
	: obj_(obj),
	useProgram_(*obj.pProgram_)
	{
		if(obj.mvp_loc_ != -1)
		{
			Matrix4&& m = Camera::GetModelViewProjection(pNode);
			glUniformMatrix4fv(obj.mvp_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(obj.m_loc_ != -1)
		{
			const Matrix4& m = pNode->GetModelMatrix();
			glUniformMatrix4fv(obj.m_loc_, 1, GL_FALSE, glm::value_ptr(m));
		}

		if(obj.model_inv_transp_loc_ != -1)
		{
			const Matrix3& m = pNode->GetModelInvTranspMatrix();
			glUniformMatrix3fv(obj.model_inv_transp_loc_, 1, GL_FALSE, glm::value_ptr(m));			
		}

		if(obj.texture_loc_ != -1 && obj.pTexture_)
		{
			glActiveTexture(GL_TEXTURE0);
			obj.pTexture_->Bind();
			glUniform1i(obj.texture_loc_, 0);
		}

		if(obj.color_difusse_loc_ != -1)
		{
			glUniform4fv(obj.color_difusse_loc_, 1, &obj.diffuse_[0]);
		}
	}

	UseMaterial::~UseMaterial()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}