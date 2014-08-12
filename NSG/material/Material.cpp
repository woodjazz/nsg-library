#include "Material.h"
#include "Mesh.h"
#include "Check.h"
#include "Camera.h"
#include "Context.h"
#include "Texture.h"
#include "Scene.h"
#include "Graphics.h"
#include <assert.h>
#include <sstream>
#include <algorithm>


namespace NSG
{
    Material::Material()
        : ambient_(1, 1, 1, 1),
          diffuse_(1, 1, 1, 1),
          specular_(1, 1, 1, 1),
          shininess_(1),
          color_(1, 1, 1, 1),
          enableCullFace_(false)
    {
    }

    Material::~Material()
    {
        Context::RemoveObject(this);
    }

    void Material::SetColor(Color color)
    {
        if(color != color_)
        {
            color_ = color;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetDiffuseColor(Color diffuse)
    {
        if(diffuse_ != diffuse)
        {
            diffuse_ = diffuse;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetSpecularColor(Color specular)
    {
        if(specular_ != specular)
        {
            specular_ = specular;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetAmbientColor(Color ambient)
    {
        if(ambient_ != ambient)
        {
            ambient_ = ambient;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetShininess(float shininess)
    {
        if(shininess_ != shininess)
        {
            shininess_ = shininess;
            SetUniformsNeedUpdate();
        }
    }

    void Material::EnableCullFace(bool enable)
    {
        if(enableCullFace_ != enable)
        {
            enableCullFace_ = enable;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetProgram(PProgram pProgram)
    {
        if (pProgram_ != pProgram)
        {
            pProgram_ = pProgram;
            SetUniformsNeedUpdate();
            Invalidate();

        }
    }

    void Material::SetTexture0(PTexture pTexture)
    {
        if (pTexture0_ != pTexture)
        {
            pTexture0_ = pTexture;
            SetUniformsNeedUpdate();
            Invalidate();
        }
    }

    void Material::SetTexture1(PTexture pTexture)
    {
        if (pTexture1_ != pTexture)
        {
            pTexture1_ = pTexture;
            SetUniformsNeedUpdate();
            Invalidate();
        }
    }

    bool Material::IsValid()
    {
        bool isReady = pProgram_ && pProgram_->IsReady();

        if (isReady)
        {
            if (pTexture0_)
            {
                isReady = pTexture0_->IsReady();
            }
            else
            {
                pTexture0_ = Context::this_->GetWhiteTexture();
                isReady = pTexture0_->IsReady();
            }

            if (isReady && pTexture1_)
            {
                isReady = pTexture1_->IsReady();
            }
        }

        return isReady;
    }

    void Material::AllocateResources()
    {
    }

    void Material::ReleaseResources()
    {
    }

    void Material::Use()
    {
        Graphics::this_->SetCullFace(enableCullFace_);
    }
}