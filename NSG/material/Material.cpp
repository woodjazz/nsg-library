#include "Material.h"
#include "Mesh.h"
#include "Check.h"
#include "Camera.h"
#include "Context.h"
#include "Texture.h"
#include "Scene.h"
#include "Graphics.h"
#include "Technique.h"
#include "Pass.h"
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
          color_(1, 1, 1, 1),
          name_(name),
          serializable_(true)
    {
    }

    Material::~Material()
    {
        Context::RemoveObject(this);
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

    void Material::SetTexture0(PTexture texture)
    {
        if (texture0_ != texture)
        {
            texture0_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
        }
    }

    void Material::SetTexture1(PTexture texture)
    {
        if (texture1_ != texture)
        {
            texture1_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
        }
    }

    void Material::SetTexture2(PTexture texture)
    {
        if (texture2_ != texture)
        {
            texture2_ = texture;
            SetUniformsNeedUpdate();
            Invalidate();
        }
    }

    bool Material::IsValid()
    {
        bool isReady = false;

        if (!texture0_)
            texture0_ = Context::this_->GetWhiteTexture();

        isReady = texture0_->IsReady();

        if (isReady && texture1_)
            isReady = texture1_->IsReady();

        if (isReady && texture2_)
            isReady = texture2_->IsReady();

        return isReady;
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

        if (technique_)
            technique_->Save(child);
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

        SetAmbientColor(GetVertex4(node.attribute("ambient").as_string()));
        SetDiffuseColor(GetVertex4(node.attribute("diffuse").as_string()));
        SetSpecularColor(GetVertex4(node.attribute("specular").as_string()));
        SetShininess(node.attribute("shininess").as_float());
        SetColor(GetVertex4(node.attribute("color").as_string()));

        pugi::xml_node childTechnique = node.child("Technique");
        if (childTechnique)
        {
            PTechnique technique(new Technique);
            SetTechnique(technique);
            technique->Load(childTechnique);
        }
    }
}