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
#include "pugixml.hpp"
#include <sstream>
#include <algorithm>


namespace NSG
{
    Material::Material(const std::string& name)
        : ambient_(1, 1, 1, 1),
          diffuse_(1, 1, 1, 1),
          specular_(1, 1, 1, 1),
          shininess_(1),
          parallaxScale_(0.05f),
          color_(1, 1, 1, 1),
          name_(name),
          serializable_(true)
    {
    }

    Material::~Material()
    {
        Invalidate();
    }

    PMaterial Material::Clone(const std::string& name)
    {
        auto material = std::make_shared<Material>(name);
        material->texture0_ = texture0_;
        material->texture1_ = texture1_;
        material->texture2_ = texture2_;
        material->texture3_ = texture3_;
        material->texture4_ = texture4_;
        material->texture5_ = texture5_;
        material->ambient_ = ambient_;
        material->diffuse_ = diffuse_;
        material->specular_ = specular_;
        material->shininess_ = shininess_;
        material->parallaxScale_ = parallaxScale_;
        material->color_ = color_;
        material->GetTechnique()->CopyPasses(GetTechnique()->GetConstPasses());
        material->serializable_ = serializable_;
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

    bool Material::SetTexture0(PTexture texture)
    {
        if (texture0_ != texture)
        {
            texture0_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
            return true;
        }
        return false;
    }

    bool Material::SetTexture1(PTexture texture)
    {
        if (texture1_ != texture)
        {
            texture1_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
            return true;
        }
        return false;
    }

    bool Material::SetTexture2(PTexture texture)
    {
        if (texture2_ != texture)
        {
            texture2_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
            return true;
        }
        return false;
    }

    bool Material::SetTexture3(PTexture texture)
    {
        if (texture3_ != texture)
        {
            texture3_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
            return true;
        }
        return false;
    }

    bool Material::SetTexture4(PTexture texture)
    {
        if (texture4_ != texture)
        {
            texture4_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
            return true;
        }
        return false;
    }

    bool Material::SetTexture5(PTexture texture)
    {
        if (texture5_ != texture)
        {
            texture5_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
            return true;
        }
        return false;
    }

    void Material::SetDiffuseMap(PTexture texture)
    {
        if (SetTexture0(texture))
        {
            if (texture)
                GetTechnique()->EnableProgramFlags((int)ProgramFlag::DIFFUSEMAP);
            else
                GetTechnique()->DisableProgramFlags((int)ProgramFlag::DIFFUSEMAP);
        }
    }

    void Material::SetNormalMap(PTexture texture)
    {
        if (SetTexture1(texture))
        {
            if (texture)
                GetTechnique()->EnableProgramFlags((int)ProgramFlag::NORMALMAP | (int)ProgramFlag::PER_PIXEL_LIGHTING);
            else
                GetTechnique()->DisableProgramFlags((int)ProgramFlag::NORMALMAP);
        }
    }

    void Material::SetLightMap(PTexture texture)
    {
        if (SetTexture2(texture))
        {
            if (texture)
                GetTechnique()->EnableProgramFlags((int)ProgramFlag::LIGHTMAP);
            else
                GetTechnique()->DisableProgramFlags((int)ProgramFlag::LIGHTMAP);
        }
    }

    void Material::SetSpecularMap(PTexture texture)
    {
        if (SetTexture3(texture))
        {
            if (texture)
                GetTechnique()->EnableProgramFlags((int)ProgramFlag::SPECULARMAP  | (int)ProgramFlag::PER_PIXEL_LIGHTING);
            else
                GetTechnique()->DisableProgramFlags((int)ProgramFlag::SPECULARMAP);
        }
    }

    void Material::SetAOMap(PTexture texture)
    {
        if (SetTexture4(texture))
        {
            if (texture)
                GetTechnique()->EnableProgramFlags((int)ProgramFlag::AOMAP  | (int)ProgramFlag::PER_PIXEL_LIGHTING);
            else
                GetTechnique()->DisableProgramFlags((int)ProgramFlag::AOMAP);
        }
    }

    void Material::SetDisplacementMap(PTexture texture)
    {
        if (SetTexture5(texture))
        {
            if (texture)
            {
                GetTechnique()->EnableProgramFlags((int)ProgramFlag::DISPLACEMENTMAP  | (int)ProgramFlag::PER_PIXEL_LIGHTING);
                // Do not generate mipmaps for displacement map
                TextureFlags flags = texture->GetFlags();
                flags &= ~(int)TextureFlag::GENERATE_MIPMAPS;
                texture->SetFlags(flags);
                texture->SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            }
            else
                GetTechnique()->DisableProgramFlags((int)ProgramFlag::DISPLACEMENTMAP);
        }
    }

    bool Material::IsValid()
    {
        bool isReady = true;

        if (texture0_)
            isReady = texture0_->IsReady();

        if (texture1_)
            isReady = isReady && texture1_->IsReady();

        if (texture2_)
            isReady = isReady && texture2_->IsReady();

        if (texture3_)
            isReady = isReady && texture3_->IsReady();

        if (texture4_)
            isReady = isReady && texture4_->IsReady();

        if (texture5_)
            isReady = isReady && texture5_->IsReady();

        return isReady;
    }

    void Material::AllocateResources()
    {
        GetTechnique();
    }

    PTechnique Material::GetTechnique()
    {
        if (!technique_)
            technique_ = std::make_shared<Technique>(shared_from_this());

        return technique_;
    }

    void Material::Save(pugi::xml_node& node)
    {
        if (!serializable_)
            return;

        pugi::xml_node child = node.append_child("Material");

        {
            std::stringstream ss;
            ss << name_;
            child.append_attribute("name") = ss.str().c_str();
        }

        if (texture0_ && texture0_->IsSerializable())
        {
            pugi::xml_node childTexture = child.append_child("Texture0");
            texture0_->Save(childTexture);
        }

        if (texture1_ && texture1_->IsSerializable())
        {
            pugi::xml_node childTexture = child.append_child("Texture1");
            texture1_->Save(childTexture);
        }

        if (texture2_ && texture2_->IsSerializable())
        {
            pugi::xml_node childTexture = child.append_child("Texture2");
            texture2_->Save(childTexture);
        }

        if (texture3_ && texture3_->IsSerializable())
        {
            pugi::xml_node childTexture = child.append_child("Texture3");
            texture3_->Save(childTexture);
        }

        if (texture4_ && texture4_->IsSerializable())
        {
            pugi::xml_node childTexture = child.append_child("Texture4");
            texture4_->Save(childTexture);
        }

        if (texture5_ && texture5_->IsSerializable())
        {
            pugi::xml_node childTexture = child.append_child("Texture5");
            texture5_->Save(childTexture);
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

        GetTechnique()->Save(child);
    }

    void Material::Load(const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();

        pugi::xml_node childTexture0 = node.child("Texture0");
        if (childTexture0)
            texture0_ = Texture::CreateFrom(childTexture0);

        pugi::xml_node childTexture1 = node.child("Texture1");
        if (childTexture1)
            texture1_ = Texture::CreateFrom(childTexture1);

        pugi::xml_node childTexture2 = node.child("Texture2");
        if (childTexture2)
            texture2_ = Texture::CreateFrom(childTexture2);

        pugi::xml_node childTexture3 = node.child("Texture3");
        if (childTexture3)
            texture3_ = Texture::CreateFrom(childTexture3);

        pugi::xml_node childTexture4 = node.child("Texture4");
        if (childTexture4)
            texture4_ = Texture::CreateFrom(childTexture4);

        pugi::xml_node childTexture5 = node.child("Texture5");
        if (childTexture5)
            texture5_ = Texture::CreateFrom(childTexture5);

        SetAmbientColor(GetVertex4(node.attribute("ambient").as_string()));
        SetDiffuseColor(GetVertex4(node.attribute("diffuse").as_string()));
        SetSpecularColor(GetVertex4(node.attribute("specular").as_string()));
        SetShininess(node.attribute("shininess").as_float());
        SetColor(GetVertex4(node.attribute("color").as_string()));

        pugi::xml_node childTechnique = node.child("Technique");
        if (childTechnique)
            GetTechnique()->Load(childTechnique);
    }
}