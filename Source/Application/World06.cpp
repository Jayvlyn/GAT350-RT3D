#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>
#include "Framework/Components/CameraComponent.h"

#define INTERLEAVE

namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_shadow.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(2048, 2048);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }
        
        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        // set postprocess gui
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        // INVERT
        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            (effect) ? m_params |= INVERT_MASK : m_params &= ~INVERT_MASK;
        }
        // GRAYSCALE
        effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect))
        {
            (effect) ? m_params |= GRAYSCALE_MASK : m_params &= ~GRAYSCALE_MASK;
        }
        // COLOR TINT
        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Color Tint", &effect))
        {
            (effect) ? m_params |= COLORTINT_MASK : m_params &= ~COLORTINT_MASK;
        }
        if (effect)
        {
            ImGui::ColorEdit3("Tint", &m_colorTint[0]);
        }
        // GRAIN
        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect))
        {
            (effect) ? m_params |= GRAIN_MASK : m_params &= ~GRAIN_MASK;
        }
        // SCANLINE
        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect))
        {
            (effect) ? m_params |= SCANLINE_MASK : m_params &= ~SCANLINE_MASK;
        }
        if (effect)
        {
            ImGui::DragFloat("Intensity", &scanlineIntensity, 0.01, 0, 1);
            ImGui::DragFloat("Spacing", &scanlineSpacing, 0.1, 0, 100);
        }
        // KERNEL
        effect = m_params & KERNEL_MASK;
        if (ImGui::Checkbox("Kernel", &effect))
        {
            (effect) ? m_params |= KERNEL_MASK : m_params &= ~KERNEL_MASK;
        }

        ImGui::End();

        // Set postprocess shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("colorTint", m_colorTint);
            program->SetUniform("time", m_time);
            program->SetUniform("scanlineSpacing", scanlineSpacing);
            program->SetUniform("scanlineIntensity", scanlineIntensity);
            program->SetUniform("kernelOffset", kernelOffset);
        }
        
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({0, 0, 0});
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);


        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
