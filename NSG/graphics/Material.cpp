#include "Material.h"
#include "Mesh.h"
#include "Check.h"
#include "Camera.h"
#include "Texture.h"
#include "Scene.h"
#include "Graphics.h"
#include "Technique.h"
#include "Pass.h"
#include "Program.h"
#include "Util.h"
#include "ResourceXMLNode.h"
#include "InstanceBuffer.h"
#include "pugixml.hpp"
#include <sstream>
#include <algorithm>


namespace NSG
{
    MapAndVector<std::string, Material> Material::materials_;

    Material::Material(const std::string& name)
        : Object(name),
          ambient_(1, 1, 1, 1),
          diffuse_(1, 1, 1, 1),
          specular_(1, 1, 1, 1),
          shininess_(1),
          parallaxScale_(0.05f),
          color_(1, 1, 1, 1),
          u0Offset_(1, 0, 0, 0),
          v0Offset_(0, 1, 0, 0),
          serializable_(true),
          blendFilterMode_(BlendFilterMode::ADDITIVE),
          isBatched_(false)
    {
        technique_ = std::make_shared<Technique>(this);
        SetProgramFlags(0, (int)ProgramFlag::NONE); // in order to force program creation
    }

    Material::~Material()
    {
    }

    void Material::SetProgramFlags(unsigned passIndex, const ProgramFlags& flags)
    {
        auto pass = technique_->GetPass(passIndex);
        auto program = pass->GetProgram();
        program->SetFlags(flags);
    }

    void Material::EnableProgramFlags(unsigned passIndex, const ProgramFlags& flags)
    {
        auto pass = technique_->GetPass(passIndex);
        auto program = pass->GetProgram();
        program->EnableFlags(flags);
    }

    void Material::DisableProgramFlags(unsigned passIndex, const ProgramFlags& flags)
    {
        auto pass = technique_->GetPass(passIndex);
        auto program = pass->GetProgram();
        program->DisableFlags(flags);
    }

    PMaterial Material::Clone(const std::string& name)
    {
        auto material = std::make_shared<Material>(name);
        for (size_t index = 0; index < MaterialTexture::MAX_TEXTURES_MAPS; index++)
            material->texture_[index] = texture_[index];
        material->ambient_ = ambient_;
        material->diffuse_ = diffuse_;
        material->specular_ = specular_;
        material->shininess_ = shininess_;
        material->parallaxScale_ = parallaxScale_;
        material->color_ = color_;
        material->technique_->CopyPasses(technique_->GetConstPasses());
        material->serializable_ = serializable_;
        material->blendFilterMode_ = blendFilterMode_;
        material->blurFilter_ = blurFilter_;
        material->waveFilter_ = waveFilter_;
        return material;
    }

    void Material::SetColor(Color color)
    {
        if (color != color_)
        {
            color_ = color;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetDiffuseColor(Color diffuse)
    {
        if (diffuse_ != diffuse)
        {
            diffuse_ = diffuse;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetSpecularColor(Color specular)
    {
        if (specular_ != specular)
        {
            specular_ = specular;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetAmbientColor(Color ambient)
    {
        if (ambient_ != ambient)
        {
            ambient_ = ambient;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetShininess(float shininess)
    {
        if (shininess_ != shininess)
        {
            shininess_ = shininess;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetParallaxScale(float parallaxScale)
    {
        if (parallaxScale_ != parallaxScale)
        {
            parallaxScale_ = parallaxScale;
            SetUniformsNeedUpdate();
        }
    }

	void Material::SetUVTransform(const Vector4& u0Offset, const Vector4& v0Offset)
    {
		if (u0Offset != u0Offset_ || v0Offset != v0Offset_)
        {
			u0Offset_ = u0Offset;
			v0Offset_ = v0Offset;
            SetUniformsNeedUpdate();
        }
    }


    bool Material::SetTexture(size_t index, PTexture texture)
    {
        CHECK_ASSERT(index >= 0 && index < MaterialTexture::MAX_TEXTURES_MAPS, __FILE__, __LINE__);
        if (texture_[index] != texture)
        {
            texture_[index] = texture;
            SetUniformsNeedUpdate();
            Invalidate();
            return true;
        }
        return false;
    }


    void Material::SetDiffuseMap(PTexture texture)
    {
        if (SetTexture(MaterialTexture::DIFFUSE_MAP, texture))
        {
            if (texture)
                technique_->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::DIFFUSEMAP);
            else
                technique_->GetPass(0)->GetProgram()->DisableFlags((int)ProgramFlag::DIFFUSEMAP);
        }
    }

    void Material::SetNormalMap(PTexture texture)
    {
        if (SetTexture(MaterialTexture::NORMAL_MAP, texture))
        {
            if (texture)
                technique_->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::NORMALMAP | (int)ProgramFlag::PER_PIXEL_LIGHTING);
            else
                technique_->GetPass(0)->GetProgram()->DisableFlags((int)ProgramFlag::NORMALMAP);
        }
    }

    void Material::SetLightMap(PTexture texture)
    {
        if (SetTexture(MaterialTexture::LIGHT_MAP, texture))
        {
            if (texture)
                technique_->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::LIGHTMAP);
            else
                technique_->GetPass(0)->GetProgram()->DisableFlags((int)ProgramFlag::LIGHTMAP);
        }
    }

    void Material::SetSpecularMap(PTexture texture)
    {
        if (SetTexture(MaterialTexture::SPECULAR_MAP, texture))
        {
            if (texture)
                technique_->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::SPECULARMAP | (int)ProgramFlag::PER_PIXEL_LIGHTING);
            else
                technique_->GetPass(0)->GetProgram()->DisableFlags((int)ProgramFlag::SPECULARMAP);
        }
    }

    void Material::SetAOMap(PTexture texture)
    {
        if (SetTexture(MaterialTexture::AO_MAP, texture))
        {
            if (texture)
                technique_->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::AOMAP  | (int)ProgramFlag::PER_PIXEL_LIGHTING);
            else
                technique_->GetPass(0)->GetProgram()->DisableFlags((int)ProgramFlag::AOMAP);
        }
    }

    void Material::SetDisplacementMap(PTexture texture)
    {
        if (SetTexture(MaterialTexture::DISPLACEMENT_MAP, texture))
        {
            if (texture)
            {
                technique_->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::DISPLACEMENTMAP  | (int)ProgramFlag::PER_PIXEL_LIGHTING);
                // Do not generate mipmaps for displacement map
                TextureFlags flags = texture->GetFlags();
                flags &= ~(int)TextureFlag::GENERATE_MIPMAPS;
                texture->SetFlags(flags);
                texture->SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            }
            else
                technique_->GetPass(0)->GetProgram()->DisableFlags((int)ProgramFlag::DISPLACEMENTMAP);
        }
    }

    void Material::SetTextMap(PTexture texture)
    {
        if (SetTexture(MaterialTexture::DIFFUSE_MAP, texture))
        {
            if (texture)
            {
                auto pass = technique_->GetPass(0);
                pass->SetBlendMode(BLEND_MODE::BLEND_ALPHA);
                technique_->GetPass(0)->GetProgram()->EnableFlags((int)ProgramFlag::TEXT);
                texture->SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            }
            else
            {
                auto pass = technique_->GetPass(0);
                pass->SetBlendMode(BLEND_MODE::BLEND_NONE);
                technique_->GetPass(0)->GetProgram()->DisableFlags((int)ProgramFlag::TEXT);
            }
        }
    }

    void Material::SetupBlur()
    {
        auto texture = texture_[0];
        if (texture && texture->IsReady())
        {
            blurFilter_.blurRadius_.x = 1.f / (float)texture_[0]->GetWidth();
            blurFilter_.blurRadius_.y = 1.f / (float)texture_[0]->GetHeight();
        }
    }

    bool Material::IsValid()
    {
        bool isReady = true;
        if (xmlResource_)
            isReady = xmlResource_->IsReady();
        for (int index = 0; index < MaterialTexture::MAX_TEXTURES_MAPS; index++)
            if (texture_[index])
                isReady = isReady && texture_[index]->IsReady();
        if (isReady)
            SetupBlur();
        return isReady;
    }

    void Material::AllocateResources()
    {
        isBatched_ = Graphics::this_->HasInstancedArrays() && technique_->GetNumPasses() == 1 && !IsTransparent();
        if (isBatched_)
            instanceBuffer_ = std::make_shared<InstanceBuffer>();
    }

    void Material::ReleaseResources()
    {
        instanceBuffer_ = nullptr;
        lastBatch_.Clear();

        for (int index = 0; index < MaterialTexture::MAX_TEXTURES_MAPS; index++)
            if (texture_[index])
                texture_[index]->Invalidate();
    }

    void Material::Save(pugi::xml_node& node)
    {
        if (!serializable_)
            return;

        pugi::xml_node child = node.append_child("Material");

        child.append_attribute("name").set_value(name_.c_str());

        for (int index = 0; index < MaterialTexture::MAX_TEXTURES_MAPS; index++)
        {
            if (texture_[index] && texture_[index]->IsSerializable())
            {
                std::stringstream ss;
                ss << "Texture" << index;
                pugi::xml_node childTexture = child.append_child(ss.str().c_str());
                texture_[index]->Save(childTexture);
            }
        }

        {
            std::stringstream ss;
            ss << ambient_;
            child.append_attribute("ambient") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << diffuse_;
            child.append_attribute("diffuse") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << specular_;
            child.append_attribute("specular") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << shininess_;
            child.append_attribute("shininess") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << color_;
            child.append_attribute("color") = ss.str().c_str();
        }

        technique_->Save(child);
    }

    void Material::LoadFrom(PResource resource, const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();

        for (int index = 0; index < MaterialTexture::MAX_TEXTURES_MAPS; index++)
        {
            texture_[index] = nullptr;
            std::stringstream ss;
            ss << "Texture" << index;
            pugi::xml_node childTexture = node.child(ss.str().c_str());
            if (childTexture)
                texture_[index] = Texture::CreateFrom(resource, childTexture);
        }

        SetAmbientColor(GetVertex4(node.attribute("ambient").as_string()));
        SetDiffuseColor(GetVertex4(node.attribute("diffuse").as_string()));
        SetSpecularColor(GetVertex4(node.attribute("specular").as_string()));
        SetShininess(node.attribute("shininess").as_float());
        SetColor(GetVertex4(node.attribute("color").as_string()));

        pugi::xml_node childTechnique = node.child("Technique");
        if (childTechnique)
            technique_->Load(childTechnique);
    }

    void Material::SetFilterBlendMode(BlendFilterMode mode)
    {
        if (blendFilterMode_ != mode)
        {
            blendFilterMode_ = mode;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetFilterBlur(const BlurFilter& data)
    {
        if (blurFilter_ != data)
        {
            blurFilter_ = data;
            SetUniformsNeedUpdate();
            SetupBlur();
        }
    }

    void Material::SetFilterWave(const WaveFilter& data)
    {
        if (waveFilter_ != data)
        {
            waveFilter_ = data;
            SetUniformsNeedUpdate();
        }
    }

    PTexture Material::GetTextureWith(PResource resource) const
    {
        for (size_t i = 0; i < MaterialTexture::MAX_TEXTURES_MAPS; i++)
        {
            if (texture_[i] && resource == texture_[i]->GetResource())
                return texture_[i];
        }
        return nullptr;
    }

    bool Material::IsTransparent() const
    {
        return technique_->IsTransparent();
    }

    bool Material::IsText() const
    {
        return technique_->IsText();
    }

    void Material::SetSolid(bool solid)
    {
        technique_->GetPass(0)->SetDrawMode(solid ? DrawMode::SOLID : DrawMode::WIREFRAME);
    }

    bool Material::IsBatched()
    {
        if (IsReady())
        {
            return isBatched_;
        }
        return false;
    }

    void Material::BachedNodeHasChanged()
    {
        if (!lastBatch_.IsEmpty())
        {
            lastBatch_.Clear();
            isBatched_ = false; //disable batching since the scenenode is changing dynamically
            technique_->Invalidate();
        }
    }

    void Material::UpdateBatchBuffer(const Batch& batch)
    {
        if (!(lastBatch_ == batch))
        {
            instanceBuffer_->UpdateBatchBuffer(batch);
            lastBatch_ = batch;
        }
    }

    PMaterial Material::Create(const std::string& name, const ProgramFlags& flags)
    {
        auto material = materials_.Create(name);
        if ((int)ProgramFlag::NONE != flags)
            material->SetProgramFlags(0, flags);
        return material;
    }

    PMaterial Material::GetOrCreate(const std::string& name, const ProgramFlags& flags)
    {
        auto material = materials_.GetOrCreate(name);
        if ((int)ProgramFlag::NONE != flags)
            material->SetProgramFlags(0, flags);
        return material;
    }

    PMaterial Material::Get(const std::string& name)
    {
        return materials_.Get(name);
    }

    std::vector<PMaterial> Material::GetMaterials()
    {
        return materials_.GetObjs();
    }

    PTexture Material::GetTextureWithResource(PResource resource)
    {
        auto materials = materials_.GetObjs();
        for (auto& material : materials)
        {
            auto texture = material->GetTextureWith(resource);
            if (texture)
                return texture;
        }

        return nullptr;
    }

    std::vector<PMaterial> Material::LoadMaterials(PResource resource, const pugi::xml_node& node)
    {
        std::vector<PMaterial> result;
        pugi::xml_node objs = node.child("Materials");
        if (objs)
        {
            pugi::xml_node child = objs.child("Material");
            while (child)
            {
                std::string name = child.attribute("name").as_string();
                auto material(Material::GetOrCreate(name));
                auto xmlResource = Resource::Create<ResourceXMLNode>(name);
                xmlResource->Set(resource, material, "Materials", name);
                xmlResource->IsReady(); //force load resources for textures
                material->Set(xmlResource);
                result.push_back(material);
                child = child.next_sibling("Material");
            }
        }
        return result;
    }

    void Material::SaveMaterials(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Materials");
        auto materials = Material::GetMaterials();
        for (auto& obj : materials)
            obj->Save(child);
    }

    void Material::Set(PResourceXMLNode xmlResource)
    {
        if (xmlResource != xmlResource_)
        {
            xmlResource_ = xmlResource;
            Invalidate();
        }
    }


}
