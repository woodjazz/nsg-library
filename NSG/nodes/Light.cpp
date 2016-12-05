#include "Light.h"
#include "Log.h"
#include "Check.h"
#include "Resource.h"
#include "Batch.h"
#include "Material.h"
#include "Util.h"
#include "Scene.h"
#include "Frustum.h"
#include "Renderer.h"
#include "Window.h"
#include "RenderingContext.h"
#include "Sphere.h"
#include "Texture.h"
#include "ShadowCamera.h"
#include "FrameBuffer.h"
#include "StringConverter.h"
#include "imgui.h"
#include "Maths.h"
#include "pugixml.hpp"
#include <assert.h>
#include <algorithm>
#include <sstream>

namespace NSG
{
Light::Light(const std::string& name)
    : SceneNode(name),
      type_(LightType::POINT),
      energy_(1),
      color_(1),
      diffuse_(true),
      specular_(true),
      spotCutOff_(45),
      diffuseColor_(1),
      specularColor_(1),
      shadowColor_(0, 0, 0, 1),
      distance_(30),
      shadows_(true),
      shadowClipStart_(0.1f), // same minimum as blender
      shadowClipEnd_(30.f), // same as distance_
      onlyShadow_(false),
      shadowBias_(1),
      slopeScaledBias_(1),
      shadowSplits_(1),
      invRange_(1.f / distance_),
      signalSetType_(new SignalLight)
{
    FrameBuffer::Flags flags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::COLOR_CUBE_TEXTURE | FrameBuffer::DEPTH));
    for (int i = 0; i < ShadowCamera::MAX_SPLITS; i++)
    {
        shadowCamera_[i] = PShadowCamera(new ShadowCamera(this));
        shadowFrameBuffer_[i] = PFrameBuffer(new FrameBuffer(GetUniqueName("LightCubeFrameBuffer"), flags));
        CHECK_ASSERT(TextureWrapMode::CLAMP_TO_EDGE == GetShadowMap(i)->GetWrapMode());
        shadowFrameBuffer_[i]->EnableAutoSize(false);
    }

    CalculateRange();
}

Light::~Light()
{
    SignalBeingDestroy()->Run(this);
}

void Light::SetEnergy(float energy)
{
    if (energy_ != energy)
    {
        energy_ = energy;
        CalculateColor();
        SetUniformsNeedUpdate();
    }
}

float Light::GetEnergy() const
{
    return energy_;
}

void Light::SetColor(const Color& color)
{
    if (color_ != color)
    {
        color_ = color;
        CalculateColor();
        SetUniformsNeedUpdate();
    }
}

const Color& Light::GetColor() const
{
    return color_;
}

void Light::SetShadowColor(Color color)
{
    if (shadowColor_ != color)
    {
        shadowColor_ = color;
        SetUniformsNeedUpdate();
    }
}

void Light::EnableDiffuseColor(bool enable)
{
    if (diffuse_ != enable)
    {
        diffuse_ = enable;
        CalculateColor();
        SetUniformsNeedUpdate();
    }
}

void Light::EnableSpecularColor(bool enable)
{
    if (specular_ != enable)
    {
        specular_ = enable;
        CalculateColor();
        SetUniformsNeedUpdate();
    }
}

void Light::SetSpotCutOff(float spotCutOff)
{
    if (spotCutOff_ != spotCutOff)
    {
        spotCutOff_ = spotCutOff;
        SetUniformsNeedUpdate();
    }
}

void Light::SetType(LightType type)
{
    if (type_ != type)
    {
        for (int i = 0; i < ShadowCamera::MAX_SPLITS; i++)
        {
            if (type == LightType::POINT)
            {
                FrameBuffer::Flags flags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::COLOR_CUBE_TEXTURE | FrameBuffer::DEPTH));
                shadowFrameBuffer_[i] = PFrameBuffer(new FrameBuffer(GetUniqueName("LightCubeFrameBuffer"), flags));
            }
            else if (type_ == LightType::POINT)
            {
                FrameBuffer::Flags flags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
                shadowFrameBuffer_[i] = PFrameBuffer(new FrameBuffer(GetUniqueName("Light2DFrameBuffer"), flags));
            }

            //GetShadowMap(i)->SetWrapMode(TextureWrapMode::REPEAT);
            CHECK_ASSERT(TextureWrapMode::CLAMP_TO_EDGE == GetShadowMap(i)->GetWrapMode());
        }

        type_ = type;
        CalculateRange();
        OnDirty();
        SetUniformsNeedUpdate();
        signalSetType_->Run(this);
    }
}

void Light::Save(pugi::xml_node& node) const
{
    node.append_attribute("name").set_value(GetName().c_str());
    node.append_attribute("nodeType").set_value("Light");
    node.append_attribute("type").set_value(ToString(type_));
    node.append_attribute("energy").set_value(energy_);
    node.append_attribute("color").set_value(ToString(color_).c_str());
    node.append_attribute("diffuse").set_value(diffuse_);
    node.append_attribute("specular").set_value(specular_);
    node.append_attribute("spotCutOff").set_value(spotCutOff_);
    node.append_attribute("position").set_value(ToString(GetPosition()).c_str());
    node.append_attribute("orientation").set_value(ToString(GetOrientation()).c_str());
    node.append_attribute("distance").set_value(distance_);
    node.append_attribute("shadows").set_value(shadows_);
    node.append_attribute("shadowClipStart").set_value(shadowClipStart_);
    node.append_attribute("shadowClipEnd").set_value(shadowClipEnd_);
    node.append_attribute("onlyShadow").set_value(onlyShadow_);
    node.append_attribute("shadowColor").set_value(ToString(shadowColor_).c_str());
    node.append_attribute("shadowBias").set_value(shadowBias_);
    node.append_attribute("slopeScaledBias").set_value(slopeScaledBias_);

    SaveChildren(node);
}

void Light::Load(const pugi::xml_node& node)
{
    name_ = node.attribute("name").as_string();
    SetType(ToLightType(node.attribute("type").as_string()));
    SetEnergy(node.attribute("energy").as_float());
    SetColor(ToVertex3(node.attribute("color").as_string()));
    EnableDiffuseColor(node.attribute("diffuse").as_bool());
    EnableSpecularColor(node.attribute("specular").as_bool());
    SetSpotCutOff(node.attribute("spotCutOff").as_float());
    Vertex3 position = ToVertex3(node.attribute("position").as_string());
    SetPosition(position);
    Quaternion orientation = ToQuaternion(node.attribute("orientation").as_string());
    SetOrientation(orientation);
    SetDistance(node.attribute("distance").as_float());
    EnableShadows(node.attribute("shadows").as_bool());
    SetShadowClipStart(node.attribute("shadowClipStart").as_float());
    SetShadowClipEnd(node.attribute("shadowClipEnd").as_float());
    SetOnlyShadow(node.attribute("onlyShadow").as_bool());
    SetShadowColor(Vector4(ToVertex3(node.attribute("shadowColor").as_string()), 1.0));
    SetBias(node.attribute("shadowBias").as_float());
    SetSlopeScaledBias(node.attribute("slopeScaledBias").as_float());
    LoadChildren(node);
}

void Light::FillShaderDefines(std::string& defines, PassType passType, const Material* material) const
{
    if (PassType::LIT == passType)
    {
        if (LightType::POINT == type_)
            defines += "HAS_POINT_LIGHT\n";
        else if (LightType::DIRECTIONAL == type_)
            defines += "HAS_DIRECTIONAL_LIGHT\n";
        else
        {
            defines += "HAS_SPOT_LIGHT\n";
        }

        if(material)
        {
            if (DoShadows() && material->ReceiveShadows())
            {
                if (LightType::POINT == type_)
                    defines += "CUBESHADOWMAP\n";
                else
                    defines += "SHADOWMAP\n";
            }

            if (HasSpecularColor() && material->HasSpecularColor())
                defines += "SPECULAR\n";
        }
    }
    else if (PassType::SHADOW == passType)
    {
        if (LightType::POINT == type_)
            defines += "SHADOW_POINT_PASS\n";
        else if (LightType::DIRECTIONAL == type_)
            defines += "SHADOW_DIR_PASS\n";
        else
            defines += "SHADOW_SPOT_PASS\n";
    }
}

SignalLight::PSignal Light::SignalBeingDestroy()
{
    static SignalLight::PSignal sig(new SignalLight);
    if(sig->alive_.ok)
        return sig;
    else
        return SignalLight::PSignal(new SignalLight);
}

void Light::CalculateColor()
{
    diffuseColor_ = diffuse_ ? color_ * energy_ : Color(0);
    specularColor_ = specular_ ? color_ * energy_ : Color(0);
}

bool Light::HasSpecularColor() const
{
    return specularColor_.r || specularColor_.g || specularColor_.b;
}

void Light::SetDistance(float distance)
{
    if (distance_ != distance)
    {
        distance_ = distance;
        CalculateRange();
        SetUniformsNeedUpdate();
    }
}

bool Light::DoShadows() const
{
    return shadows_;
}

FrameBuffer* Light::GetShadowFrameBuffer(int idx) const
{
    CHECK_ASSERT(idx < ShadowCamera::MAX_SPLITS);
    return shadowFrameBuffer_[idx].get();
}

PTexture Light::GetShadowMap(int idx) const
{
    CHECK_ASSERT(idx < ShadowCamera::MAX_SPLITS);
    return shadowFrameBuffer_[idx]->GetColorTexture();
}

void Light::CalculateRange()
{
    if (LightType::SPOT == type_)
        invRange_ = 1.f / Clamp((shadowClipEnd_ - shadowClipStart_), 0.f, distance_);
    else if (LightType::POINT == type_)
        invRange_ = 1.f / std::max(distance_, EPSILON);
    else
        invRange_ = 1.f / std::numeric_limits<float>::max(); // shall not be used
}

void Light::SetShadowClipStart(float value)
{
    if (shadowClipStart_ != value)
    {
        shadowClipStart_ = value;
        CalculateRange();
    }
}

void Light::SetShadowClipEnd(float value)
{
    if (shadowClipEnd_ != value)
    {
        shadowClipEnd_ = value;
        CalculateRange();
    }
}

ShadowCamera* Light::GetShadowCamera(int idx) const
{
    CHECK_ASSERT(idx < ShadowCamera::MAX_SPLITS);
    return shadowCamera_[idx].get();
}
}
