#include "material_viewer.h"

#include "renderer/material.h"
#include "renderer/texture.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    MaterialViewer::MaterialViewer(WeakPtr<Material> material) :
        show_window_(true),
        material_(material)
    {

    }

    //------------------------------------------------------------------------------------------------------
    MaterialViewer::~MaterialViewer()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MaterialViewer::NewFrame()
    {
        
    }

    //------------------------------------------------------------------------------------------------------
    void MaterialViewer::RenderMenu()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MaterialViewer::RenderWindow()
    {
        if (material_.expired())
        {
            show_window_ = false;
        }

        if (show_window_)
        {
            SharedPtr<Material> material = material_.lock();

            char buf[256];
            sprintf(buf, "MaterialView: %s###Material%i", material->GetName().c_str(), reinterpret_cast<uintptr_t>(material.get()), reinterpret_cast<uintptr_t>(material.get()));

            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(450.0f, 350.0f), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_FirstUseEver);

            if (focus_)
            {
                ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_Always);
                ImGui::SetNextWindowFocus();
                focus_ = false;
            }

            if (ImGui::Begin(buf, &show_window_))
            {
                char buf[128];
                sprintf(buf, "%s", material->GetName().c_str());
                if (ImGui::InputText("Name", buf, 128))
                {
                    material->SetName(buf);
                }

                ImGui::Separator();

                DirectX::XMFLOAT3 temp_float3 = material->GetColorEmissive();
                if (ImGui::ColorEdit3("Emissive Color", &temp_float3.x))
                {
                    material->SetColorEmissive(temp_float3);
                }

                temp_float3 = material->GetColorAmbient();
                if (ImGui::ColorEdit3("Ambient Color", &temp_float3.x))
                {
                    material->SetColorAmbient(temp_float3);
                }

                temp_float3 = material->GetColorDiffuse();
                if (ImGui::ColorEdit3("Diffuse Color", &temp_float3.x))
                {
                    material->SetColorDiffuse(temp_float3);
                }

                temp_float3 = material->GetColorSpecular();
                if (ImGui::ColorEdit3("Specular Color", &temp_float3.x))
                {
                    material->SetColorSpecular(temp_float3);
                }

                ImGui::Separator();

                float temp_float = material->GetSpecularScale();
                if (ImGui::SliderFloat("Specular Scale", &temp_float, 0.0f, 10.0f))
                {
                    material->SetSpecularScale(temp_float);
                }

                temp_float = material->GetSpecularPower();
                if (ImGui::SliderFloat("Specular Power", &temp_float, 1.0f, 250.0f))
                {
                    material->SetSpecularPower(temp_float);
                }

                temp_float = material->GetOpacity();
                if (ImGui::SliderFloat("Opacity", &temp_float, 0.0f, 1.0f))
                {
                    material->SetOpacity(temp_float);
                }

                temp_float = material->GetBumpIntensity();
                if (ImGui::SliderFloat("Bump Intensity", &temp_float, 0.0f, 10.0f))
                {
                    material->SetBumpIntensity(temp_float);
                }

                ImGui::Separator();

                ImGui::Text("Texture Emissive:");
                ImGui::SameLine(150.0f);
                if (material->GetTextureEmissive().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(material->GetTextureEmissive().lock()->GetImage().lock()->GetFilePath().c_str()))
                    {
                        ShellExecute(0, 0, material->GetTextureEmissive().lock()->GetImage().lock()->GetFilePath().c_str(), 0, 0, SW_SHOW);
                    }
                }

                ImGui::Text("Texture Ambient:");
                ImGui::SameLine(150.0f);
                if (material->GetTextureAmbient().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(material->GetTextureAmbient().lock()->GetImage().lock()->GetFilePath().c_str()))
                    {
                        ShellExecute(0, 0, material->GetTextureAmbient().lock()->GetImage().lock()->GetFilePath().c_str(), 0, 0, SW_SHOW);
                    }
                }

                ImGui::Text("Texture Diffuse:");
                ImGui::SameLine(150.0f);
                if (material->GetTextureDiffuse().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(material->GetTextureDiffuse().lock()->GetImage().lock()->GetFilePath().c_str()))
                    {
                        ShellExecute(0, 0, material->GetTextureDiffuse().lock()->GetImage().lock()->GetFilePath().c_str(), 0, 0, SW_SHOW);
                    }
                }

                ImGui::Text("Texture Specular:");
                ImGui::SameLine(150.0f);
                if (material->GetTextureSpecular().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(material->GetTextureSpecular().lock()->GetImage().lock()->GetFilePath().c_str()))
                    {
                        ShellExecute(0, 0, material->GetTextureSpecular().lock()->GetImage().lock()->GetFilePath().c_str(), 0, 0, SW_SHOW);
                    }
                }

                ImGui::Text("Texture Specular Power:");
                ImGui::SameLine(150.0f);
                if (material->GetTextureSpecularPower().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(material->GetTextureSpecularPower().lock()->GetImage().lock()->GetFilePath().c_str()))
                    {
                        ShellExecute(0, 0, material->GetTextureSpecularPower().lock()->GetImage().lock()->GetFilePath().c_str(), 0, 0, SW_SHOW);
                    }
                }

                ImGui::Text("Texture Normal:");
                ImGui::SameLine(150.0f);
                if (material->GetTextureNormal().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(material->GetTextureNormal().lock()->GetImage().lock()->GetFilePath().c_str()))
                    {
                        ShellExecute(0, 0, material->GetTextureNormal().lock()->GetImage().lock()->GetFilePath().c_str(), 0, 0, SW_SHOW);
                    }
                }

                ImGui::Text("Texture Opacity:");
                ImGui::SameLine(150.0f);
                if (material->GetTextureOpacity().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(material->GetTextureOpacity().lock()->GetImage().lock()->GetFilePath().c_str()))
                    {
                        ShellExecute(0, 0, material->GetTextureOpacity().lock()->GetImage().lock()->GetFilePath().c_str(), 0, 0, SW_SHOW);
                    }
                }
            }

            ImGui::End();
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool MaterialViewer::IsShown() const
    {
        return show_window_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Material> MaterialViewer::GetMaterial() const
    {
        return material_;
    }

    //------------------------------------------------------------------------------------------------------
    void MaterialViewer::Focus()
    {
        focus_ = true;
    }
}