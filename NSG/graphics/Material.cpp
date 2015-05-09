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
          color_(1, 1, 1, 1),
          uvTransform_(1, 1, 0, 0),
          serializable_(true),
          blendFilterMode_(BlendFilterMode::ADDITIVE),
          isBatched_(false),
          fillMode_(FillMode::SOLID),
          blendMode_(BLEND_MODE::NONE),
          shadercommand_(ShaderCommand::LIGHTING),
          billboardType_(BillboardType::NONE),
          flipYTextureCoords_(false)
    {
        technique_ = std::make_shared<Technique>(this);
    }

    Material::~Material()
    {
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
        material->color_ = color_;
        material->technique_->CopyPasses(technique_->GetConstPasses());
        material->serializable_ = serializable_;
        material->blendFilterMode_ = blendFilterMode_;
        material->blurFilter_ = blurFilter_;
        material->waveFilter_ = waveFilter_;
        material->instanceBuffer_ = instanceBuffer_;
        material->lastBatch_ = lastBatch_;
        material->isBatched_ = isBatched_;
        material->fillMode_ = fillMode_;
        material->blendMode_ = blendMode_;
        material->shadercommand_ = shadercommand_;
        material->xmlResource_ = xmlResource_;

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

    void Material::SetUVTransform(const Vector4& uvTransform)
    {
        if (uvTransform != uvTransform_)
        {
            uvTransform_ = uvTransform;
            SetUniformsNeedUpdate();
        }
    }

    bool Material::SetTexture(PTexture texture)
    {
        auto type = texture->GetMapType();
        CHECK_ASSERT(type != TextureType::UNKNOWN, __FILE__, __LINE__);
        MaterialTexture index = MaterialTexture::DIFFUSE_MAP;
        switch (type)
        {
            case TextureType::COL:
                index = MaterialTexture::DIFFUSE_MAP;
                break;
            case TextureType::NORM:
                index = MaterialTexture::NORMAL_MAP;
                break;
            case TextureType::SPEC:
                index = MaterialTexture::SPECULAR_MAP;
                break;
            case TextureType::EMIT:
                index = MaterialTexture::LIGHT_MAP;
                break;
            case TextureType::AMB:
                index = MaterialTexture::AO_MAP;
                break;
            default:
                break;
        }
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

    PTexture Material::GetTexture(MaterialTexture index) const
    {
        CHECK_ASSERT(index >= 0 && index < MaterialTexture::MAX_TEXTURES_MAPS, __FILE__, __LINE__);
        return texture_[index];
    }


    void Material::SetTextMap(PTexture texture)
    {
        if (SetTexture(texture))
        {

            if (texture)
            {
                SetBlendMode(BLEND_MODE::ALPHA);
                SetShaderCommand(ShaderCommand::TEXT);
                texture->SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            }
            else
            {
                SetBlendMode(BLEND_MODE::NONE);
                SetShaderCommand(ShaderCommand::LIGHTING);
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

    static const char* TEXTURE_NAME = "Texture";
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
                std::string s(TEXTURE_NAME);
                s += ToString(index);
                pugi::xml_node childTexture = child.append_child(s.c_str());
                texture_[index]->Save(childTexture);
            }
        }

        child.append_attribute("ambient").set_value(ToString(ambient_).c_str());
        child.append_attribute("diffuse").set_value(ToString(diffuse_).c_str());
        child.append_attribute("specular").set_value(ToString(specular_).c_str());
        child.append_attribute("shininess").set_value(shininess_);
        child.append_attribute("color").set_value(ToString(color_).c_str());
        child.append_attribute("fillMode").set_value(ToString((int)fillMode_).c_str());
        child.append_attribute("blendMode").set_value(ToString((int)blendMode_).c_str());
        technique_->Save(child);
    }

    void Material::LoadFrom(PResource resource, const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();

        for (int index = 0; index < MaterialTexture::MAX_TEXTURES_MAPS; index++)
        {
            texture_[index] = nullptr;
            std::string s(TEXTURE_NAME);
            s += ToString(index);
            pugi::xml_node childTexture = node.child(s.c_str());
            if (childTexture)
                texture_[index] = Texture::CreateFrom(resource, childTexture);
        }

        SetAmbientColor(GetVertex4(node.attribute("ambient").as_string()));
        SetDiffuseColor(GetVertex4(node.attribute("diffuse").as_string()));
        SetSpecularColor(GetVertex4(node.attribute("specular").as_string()));
        SetShininess(node.attribute("shininess").as_float());
        SetColor(GetVertex4(node.attribute("color").as_string()));
        SetFillMode((FillMode)node.attribute("fillMode").as_int());
        SetBlendMode((BLEND_MODE)node.attribute("blendMode").as_int());

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
		return blendMode_ == BLEND_MODE::ALPHA || billboardType_ != BillboardType::NONE || shadercommand_ == ShaderCommand::TEXT;
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

    PMaterial Material::Create(const std::string& name)
    {
        return materials_.Create(name);
    }

    PMaterial Material::GetOrCreate(const std::string& name)
    {
        return materials_.GetOrCreate(name);
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

    void Material::SetLightingMode(LightingMode mode)
    {
        technique_->SetLightingMode(mode);
    }

    bool Material::HasLightMap() const
    {
        return nullptr != GetTexture(MaterialTexture::LIGHT_MAP);
    }

    void Material::FillShaderDefines(std::string& defines, const Light* light)
    {
        for (int index = 0; index < MaterialTexture::MAX_TEXTURES_MAPS; index++)
        {
            auto texture = GetTexture((MaterialTexture)index);
            if (texture)
            {
                auto type = texture->GetMapType();
                CHECK_ASSERT(TextureType::UNKNOWN != type, __FILE__, __LINE__);
                switch (type)
                {
                    case TextureType::COL:
                        defines += "#define DIFFUSEMAP\n";
                        break;
                    case TextureType::NORM:
                        defines += "#define NORMALMAP\n";
                        break;
                    case TextureType::SPEC:
                        defines += "#define SPECULARMAP\n";
                        break;
                    case TextureType::EMIT:
                        defines += "#define LIGHTMAP\n";
                        break;
                    case TextureType::AMB:
                        defines += "#define AOMAP\n";
                        break;
                    default:
                        break;
                }
            }
        }

        if (IsBatched())
            defines += "#define INSTANCED\n";

        switch (shadercommand_)
        {
            case ShaderCommand::LIGHTING:
                technique_->FillShaderDefines(defines, light);
                break;
            case ShaderCommand::TEXT:
                defines += "#define TEXT\n";
                break;
            case ShaderCommand::BLEND:
                defines += "#define BLEND\n";
                break;
            case ShaderCommand::BLUR:
                defines += "#define BLUR\n";
                break;
            case ShaderCommand::WAVE:
                defines += "#define WAVE\n";
                break;
            case ShaderCommand::SHOW_TEXTURE0:
                defines += "#define SHOW_TEXTURE0\n";
                break;
        }

        switch (billboardType_)
        {
            case BillboardType::NONE:
                break;
            case BillboardType::SPHERICAL:
                defines += "#define SPHERICAL_BILLBOARD\n";
                break;
            case BillboardType::CYLINDRICAL:
                defines += "#define CYLINDRICAL_BILLBOARD\n";
                break;
            default:
                CHECK_ASSERT(!"Unknown billboard type!!!", __FILE__, __LINE__);
                break;
        }

        if(flipYTextureCoords_)
            defines += "#define FLIP_Y\n";
    }
}
