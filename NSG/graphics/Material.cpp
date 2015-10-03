#include "Material.h"
#include "SphereMesh.h"
#include "Check.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Scene.h"
#include "Graphics.h"
#include "Pass.h"
#include "Program.h"
#include "Util.h"
#include "FrameBuffer.h"
#include "StringConverter.h"
#include "InstanceBuffer.h"
#include "Editor.h"
#include "imgui.h"
#include "pugixml.hpp"
#include <sstream>
#include <algorithm>

namespace NSG
{
    template<> std::map<std::string, PWeakMaterial> WeakFactory<std::string, Material>::objsMap_ = std::map<std::string, PWeakMaterial> {};

    Material::Material(const std::string& name)
        : Object(name),
          diffuseColor_(1, 1, 1, 1),
          diffuseIntensity_(1),
          specularColor_(1, 1, 1, 1),
          specularIntensity_(1),
          ambientIntensity_(1),
          shininess_(1),
          serializable_(true),
          blendFilterMode_(BlendFilterMode::ADDITIVE),
          isBatched_(false),
          fillMode_(FillMode::SOLID),
          alpha_(1),
          alphaForSpecular_(1),
          isTransparent_(false),
          emitIntensity_(0),
          renderPass_(RenderPass::LIT),
          billboardType_(BillboardType::NONE),
          flipYTextureCoords_(false),
          shadeless_(false),
          cullFaceMode_(CullFaceMode::DEFAULT),
          friction_(0.5f), // same as Blender
          signalPhysicsSet_(new SignalEmpty()),
          castShadow_(true),
          receiveShadows_(true),
          shadowBias_(0)
    {
    }

    Material::~Material()
    {
    }

    PMaterial Material::Clone(const std::string& name)
    {
        auto material = std::make_shared<Material>(name);
        for (size_t index = 0; index < MaterialTexture::MAX_MAPS; index++)
            material->texture_[index] = texture_[index];
        material->diffuseIntensity_ = diffuseIntensity_;
        material->specularIntensity_ = specularIntensity_;
        material->ambientIntensity_ = ambientIntensity_;
        material->diffuseColor_ = diffuseColor_;
        material->specularColor_ = specularColor_;
        material->shininess_ = shininess_;
        material->serializable_ = serializable_;
        material->blendFilterMode_ = blendFilterMode_;
        material->blurFilter_ = blurFilter_;
        material->waveFilter_ = waveFilter_;
        material->instanceBuffer_ = instanceBuffer_;
        material->lastBatch_ = lastBatch_;
        material->isBatched_ = isBatched_;
        material->fillMode_ = fillMode_;
        material->alpha_ = alpha_;
        material->alphaForSpecular_ = alphaForSpecular_;
        material->isTransparent_ = isTransparent_;
        material->emitIntensity_ = emitIntensity_;
        material->renderPass_ = renderPass_;
        material->billboardType_ = billboardType_;
        material->flipYTextureCoords_ = flipYTextureCoords_;
        material->shadeless_ = shadeless_;
        material->cullFaceMode_ = cullFaceMode_;
        material->friction_ = friction_;
        material->castShadow_ = castShadow_;
        material->receiveShadows_ = receiveShadows_;
        material->shadowBias_ = shadowBias_;
        return material;
    }

    void Material::SetDiffuseColor(ColorRGB color)
    {
        if (diffuseColor_ != Color(color, alpha_))
        {
            diffuseColor_ = Color(color, alpha_);
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetSpecularColor(ColorRGB color)
    {
        if (specularColor_ != Color(color, alphaForSpecular_))
        {
            specularColor_ = Color(color, alphaForSpecular_);
            SetUniformsNeedUpdate();
        }
    }

    bool Material::HasSpecularColor() const
    {
        return specularColor_.a && (specularColor_.r || specularColor_.g || specularColor_.b);
    }

    void Material::SetAmbientIntensity(float intensity)
    {
        if (ambientIntensity_ != intensity)
        {
            ambientIntensity_ = intensity;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetDiffuseIntensity(float intensity)
    {
        if (diffuseIntensity_ != intensity)
        {
            diffuseIntensity_ = intensity;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetSpecularIntensity(float intensity)
    {
        if (specularIntensity_ != intensity)
        {
            specularIntensity_ = intensity;
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

    bool Material::SetTexture(PTexture texture)
    {
        auto type = texture->GetMapType();
        CHECK_ASSERT(type != TextureType::UNKNOWN);
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
        CHECK_ASSERT(index >= 0 && index < MaterialTexture::MAX_MAPS);
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
        CHECK_ASSERT(index >= 0 && index < MaterialTexture::MAX_MAPS);
        return texture_[index];
    }

    void Material::SetAlpha(float alpha)
    {
        if (alpha_ != alpha)
        {
            alpha_ = alpha;
            diffuseColor_.a = alpha;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetEmitIntensity(float emitIntensity)
    {
        if (emitIntensity != emitIntensity_)
        {
            emitIntensity_ = emitIntensity;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetAlphaForSpecular(float alphaForSpecular)
    {
        if (alphaForSpecular_ != alphaForSpecular)
        {
            alphaForSpecular_ = alphaForSpecular;
            specularColor_.a = alphaForSpecular;
            SetUniformsNeedUpdate();
        }
    }

    void Material::EnableTransparent(bool enable)
    {
        if (isTransparent_ != enable)
        {
            isTransparent_ = enable;
            SetUniformsNeedUpdate();
        }
    }

    void Material::SetTextMap(PTexture texture)
    {
        if (SetTexture(texture))
        {
            if (texture)
            {
                EnableTransparent(true);
                SetRenderPass(RenderPass::TEXT);
                texture->SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
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
        for (int index = 0; index < MaterialTexture::MAX_MAPS; index++)
            if (texture_[index])
                isReady = isReady && texture_[index]->IsReady();
        if (isReady)
            SetupBlur();
        return isReady;
    }

    void Material::AllocateResources()
    {
        isBatched_ = Graphics::GetPtr()->HasInstancedArrays() && !IsTransparent();
        if (isBatched_)
            instanceBuffer_ = std::make_shared<InstanceBuffer>();
    }

    void Material::ReleaseResources()
    {
        instanceBuffer_ = nullptr;
        lastBatch_.Clear();

        for (int index = 0; index < MaterialTexture::MAX_MAPS; index++)
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
        child.append_attribute("shadeless").set_value(shadeless_);
        child.append_attribute("castShadow").set_value(castShadow_);
        child.append_attribute("receiveShadows").set_value(receiveShadows_);
        child.append_attribute("cullFaceMode").set_value(ToString(cullFaceMode_));
        child.append_attribute("friction").set_value(friction_);

        for (int index = 0; index < MaterialTexture::MAX_MAPS; index++)
        {
            if (texture_[index] && texture_[index]->IsSerializable())
            {
                auto childTexture = child.append_child(TEXTURE_NAME);
                texture_[index]->Save(childTexture);
            }
        }

        child.append_attribute("ambientIntensity").set_value(ambientIntensity_);
        child.append_attribute("diffuseIntensity").set_value(diffuseIntensity_);
        child.append_attribute("specularIntensity").set_value(specularIntensity_);
        child.append_attribute("diffuse").set_value(ToString(ColorRGB(diffuseColor_)).c_str());
        child.append_attribute("specular").set_value(ToString(specularColor_).c_str());
        child.append_attribute("shininess").set_value(shininess_);
        child.append_attribute("emitIntensity").set_value(emitIntensity_);
        child.append_attribute("fillMode").set_value(ToString(fillMode_));
        child.append_attribute("alpha").set_value(alpha_);
        child.append_attribute("alphaForSpecular").set_value(alphaForSpecular_);
        child.append_attribute("isTransparent").set_value(isTransparent_);
        child.append_attribute("renderPass").set_value(ToString(renderPass_));
        child.append_attribute("shadowBias").set_value(shadowBias_);
    }

    void Material::Load(const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();
        shadeless_ = node.attribute("shadeless").as_bool();
        castShadow_ = node.attribute("castShadow").as_bool();
        receiveShadows_ = node.attribute("receiveShadows").as_bool();
        cullFaceMode_ = ToCullFaceMode(node.attribute("cullFaceMode").as_string());
        SetFriction(node.attribute("friction").as_float());

        auto childTexture = node.child(TEXTURE_NAME);
        while (childTexture)
        {
            SetTexture(Texture2D::CreateFrom(childTexture));
            childTexture = childTexture.next_sibling(TEXTURE_NAME);
        }

        SetAmbientIntensity(node.attribute("ambientIntensity").as_float());
        SetDiffuseIntensity(node.attribute("diffuseIntensity").as_float());
        SetSpecularIntensity(node.attribute("specularIntensity").as_float());
        SetDiffuseColor(ToVertex3(node.attribute("diffuse").as_string()));
        SetSpecularColor(ToVertex3(node.attribute("specular").as_string()));
        SetShininess(node.attribute("shininess").as_float());
        SetEmitIntensity(node.attribute("emitIntensity").as_float());
        SetFillMode(ToFillMode(node.attribute("fillMode").as_string()));
        SetAlpha(node.attribute("alpha").as_float());
        SetAlphaForSpecular(node.attribute("alphaForSpecular").as_float());
        EnableTransparent(node.attribute("isTransparent").as_bool());
        SetRenderPass(ToRenderPass(node.attribute("renderPass").as_string()));
        SetBias(node.attribute("shadowBias").as_float());
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
        for (size_t i = 0; i < MaterialTexture::MAX_MAPS; i++)
        {
            if (texture_[i] && resource == texture_[i]->GetResource())
                return texture_[i];
        }
        return nullptr;
    }

    bool Material::IsTransparent() const
    {
        return isTransparent_ || billboardType_ != BillboardType::NONE || renderPass_ == RenderPass::TEXT;
    }

    bool Material::IsLighted() const
    {
        return !shadeless_ && (renderPass_ == RenderPass::LIT);
    }

    bool Material::IsBatched() const
    {
        if (((Material*)this)->IsReady()) //FIX this awful cast
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

    PTexture Material::GetTextureWithResource(PResource resource)
    {
        auto materials = Material::GetObjs();
        for (auto& material : materials)
        {
            auto texture = material->GetTextureWith(resource);
            if (texture)
                return texture;
        }

        return nullptr;
    }

    void Material::SaveMaterials(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Materials");
        auto materials = Material::GetObjs();
        for (auto& obj : materials)
            obj->Save(child);
    }

    bool Material::HasLightMap() const
    {
        return nullptr != GetTexture(MaterialTexture::LIGHT_MAP);
    }

    void Material::FillShaderDefines(std::string& defines, PassType passType, const Mesh* mesh, bool allowInstancing) const
    {
        defines += "MATERIAL_" + GetName() + "\n"; // just to have a shader variance per material
        bool defaultPass = PassType::DEFAULT == passType;
        bool litPass = PassType::LIT == passType;

        if (defaultPass || litPass)
        {
            switch (renderPass_)
            {
                case RenderPass::VERTEXCOLOR:
                    defines += "VERTEXCOLOR\n";
                    break;
                case RenderPass::UNLIT:
                    defines += "UNLIT\n";
                    break;
                case RenderPass::TEXT:
                    defines += "TEXT\n";
                    break;
                case RenderPass::BLEND:
                    defines += "BLEND\n";
                    break;
                case RenderPass::BLUR:
                    defines += "BLUR\n";
                    break;
                case RenderPass::WAVE:
                    defines += "WAVE\n";
                    break;
                case RenderPass::SHOW_TEXTURE0:
                    defines += "SHOW_TEXTURE0\n";
                    break;
                case RenderPass::LIT:
                    if (defaultPass)
                        defines += "AMBIENT\n";
                    else
                        defines += "LIT\n";
                    break;
                default:
                    CHECK_ASSERT(!"INCORRECT LIT PASS!!!");
                    break;

            }

            for (int index = 0; index < MaterialTexture::MAX_MAPS; index++)
            {
                auto texture = GetTexture((MaterialTexture)index);
                if (texture)
                {
                    if (texture->GetUseAlpha())
                        defines += "USEALPHA\n";
                    int uvIndex = mesh->GetUVIndex(texture->GetUVName());
                    auto type = texture->GetMapType();
                    CHECK_ASSERT(TextureType::UNKNOWN != type);
                    auto channels = texture->GetChannels();
                    switch (type)
                    {
                        case TextureType::COL:
                            defines += "DIFFUSEMAP" + ToString(uvIndex) + "\n";
                            break;
                        case TextureType::NORM:
                            if (!defaultPass)
                                defines += "NORMALMAP" + ToString(uvIndex) + "\n";
                            break;
                        case TextureType::SPEC:
                            if (!defaultPass)
                                defines += "SPECULARMAP" + ToString(uvIndex) + "\n";
                            break;
                        case TextureType::EMIT:
                            if (defaultPass)
                            {
                                defines += "LIGHTMAP" + ToString(uvIndex) + "\n";
                                defines += "LIGHTMAP_CHANNELS" + ToString(channels) + "\n";
                            }
                            break;
                        case TextureType::AMB:
                            if (defaultPass)
                            {
                                defines += "AOMAP" + ToString(uvIndex) + "\n";
                                defines += "AOMAP_CHANNELS" + ToString(channels) + "\n";
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        if (IsBatched() && mesh->IsStatic() && allowInstancing)
            defines += "INSTANCED\n";

        switch (billboardType_)
        {
            case BillboardType::NONE:
                break;
            case BillboardType::SPHERICAL:
                defines += "SPHERICAL_BILLBOARD\n";
                break;
            case BillboardType::CYLINDRICAL:
                defines += "CYLINDRICAL_BILLBOARD\n";
                break;
            default:
                CHECK_ASSERT(!"Unknown billboard type!!!");
                break;
        }

        if (flipYTextureCoords_)
            defines += "FLIP_Y\n";
    }

    void Material::SetShadeless(bool shadeless)
    {
        shadeless_ = shadeless;
    }

    void Material::SetFriction(float friction)
    {
        if (friction_ != friction)
        {
            friction_ = friction;
            signalPhysicsSet_->Run();
        }
    }

    bool Material::HasTextures() const
    {
        for (int i = 0; i < (int)MaterialTexture::MAX_MAPS; i++)
            if (texture_[i])
                return true;
        return false;
    }

    void Material::ShowGUIProperties(Editor* editor)
    {
        std::string header = "Material:" + GetName();
        if (ImGui::CollapsingHeader(header.c_str()))
        {
            if (ImGui::TreeNode("Preview"))
            {
                auto texture = editor->GetMaterialPreview(shared_from_this());
                if (texture && texture->IsReady())
                    ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), ImVec2(0, 0), ImVec2(1, -1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
                ImGui::TreePop();
            }

            if (HasTextures() && ImGui::TreeNode("Textures"))
            {
                for (int i = 0; i < (int)MaterialTexture::MAX_MAPS; i++)
                {
                    auto texture = GetTexture((MaterialTexture)i);
                    if (texture && texture->IsReady())
                    {
                        auto type = texture->GetMapType();
                        if (ImGui::TreeNode(ToString(type)))
                        {
                            auto width = std::min(64.f, (float)texture->GetWidth());
                            auto height = std::min(64.f, (float)texture->GetHeight());
                            ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2(width, height), ImVec2(0, 0), ImVec2(1, -1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
                            ImGui::TreePop();
                        }
                    }
                }
                ImGui::TreePop();
            }

            auto shadeless = IsShadeless();
            ImGui::Checkbox("Shadeless", &shadeless);
            SetShadeless(shadeless);
            ImGui::SameLine();

            auto isTransparent = IsTransparent();
            ImGui::Checkbox("Transparent", &isTransparent);
            EnableTransparent(isTransparent);

            ImGui::SameLine();
            auto isFlipped = IsYFlipped();
            ImGui::Checkbox("Flip Y", &isFlipped);
            FlipYTextureCoords(isFlipped);

            auto renderPass = GetRenderPass();
            ImGui::Combo("Render Pass", (int*)&renderPass, "Vertex Color\0Unlit\0Lit\0Text\0Blend\0Blur\0Wave\0Show Diffuse\0");
            SetRenderPass(renderPass);
            auto fillMode = GetFillMode();
            ImGui::Combo("Fill Mode", (int*)&fillMode, "Solid\0Wireframe\0");
            SetFillMode(fillMode);
            auto cullFaceMode = GetCullFaceMode();
            ImGui::Combo("Culling", (int*)&cullFaceMode, "Back\0Front\0Front and back\0Disabled");
            SetCullFaceMode(cullFaceMode);
            auto billboardType = GetBillboardType();
            ImGui::Combo("Billboard", (int*)&billboardType, "None\0Spherical\0Cylindrical\0");
            SetBillboardType(billboardType);

            auto ambientIntensity = GetAmbientIntensity();
            ImGui::SliderFloat("##ambInt", &ambientIntensity, 0.0f, 1.0f, "Ambient Intensity %.3f");
            SetAmbientIntensity(ambientIntensity);

            auto emitIntensity = GetEmitIntensity();
            ImGui::SliderFloat("##emitInt", &emitIntensity, 0.0f, 1.0f, "Emit Intensity %.3f");
            SetEmitIntensity(emitIntensity);

            auto diffuseColor = GetDiffuseColor();
            ImGui::ColorEdit4("Diffuse", &diffuseColor[0]);
            SetDiffuseColor(ColorRGB(diffuseColor));
            SetAlpha(diffuseColor.a);

            auto diffuseIntensity = GetDiffuseIntensity();
            ImGui::SliderFloat("##difInt", &diffuseIntensity, 0.0f, 1.0f, "Diffuse Intensity %.3f");
            SetDiffuseIntensity(diffuseIntensity);

            auto specularColor = GetSpecularColor();
            ImGui::ColorEdit4("Specular", &specularColor[0]);
            SetSpecularColor(ColorRGB(specularColor));
            SetAlphaForSpecular(specularColor.a);

            auto specularIntensity = GetSpecularIntensity();
            ImGui::SliderFloat("##speInt", &specularIntensity, 0.0f, 1.0f, "Specular Intensity %.3f");
            SetSpecularIntensity(specularIntensity);

            auto shininess = GetShininess();
            ImGui::SliderFloat("##shinin", &shininess, 0.0f, 511.0f, "Specular Shininess %.0f");
            SetShininess(shininess);

            auto friction = GetFriction();
            ImGui::SliderFloat("##rbFric", &friction, 0.0f, 100.0f, "Rigbody Friction %.1f");
            SetFriction(friction);

            auto castShadow = CastShadow();
            ImGui::Checkbox("Cast Shadow", &castShadow);
            CastShadow(castShadow);
            ImGui::SameLine();

            auto receiveShadows = ReceiveShadows();
            ImGui::Checkbox("Receive Shadows", &receiveShadows);
            ReceiveShadows(receiveShadows);

            auto shadowBias = GetBias();
            ImGui::SliderFloat("##bias", &shadowBias, 0.0f, 10.0f, "Shadow Bias %.3f");
            SetBias(shadowBias);
        }
    }
}
