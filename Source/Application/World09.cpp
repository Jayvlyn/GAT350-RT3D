#include "World09.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>
#include "Framework/Components/CameraComponent.h"
#include "Core/Random.h"
#include <iostream>

namespace nc
{
    bool World09::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_editor.json");
        m_scene->Load("scenes/scene_final.json");
        m_scene->Initialize();

        m_editor = std::make_unique<Editor>();

        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthBuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        auto materials = GET_RESOURCES(Material);
        for (auto material : materials)
        {
            material->depthTexture = texture;
        }

#pragma region ACTOR CREATION
        StringUtils* su = new StringUtils();
        
        std::string baseName = "ElvenHouse";
        
        // Row of Elven Houses
        for (int i = -40; i < 40; i += 15)
        {
            int num = random(1, 6);
            std::string classBase = baseName + std::to_string(num);
            auto actor = CREATE_CLASS_BASE(Actor, classBase);
            actor->name = su->CreateUnique(classBase);
            actor->transform.position = glm::vec3{ -15, -5, i };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }

        // Row of Elven Houses
        for (int i = -40; i < 40; i += 15)
        {
            int num = random(1, 6);
            std::string classBase = baseName + std::to_string(num);
            auto actor = CREATE_CLASS_BASE(Actor, classBase);
            actor->name = su->CreateUnique(classBase);
            actor->transform.position = glm::vec3{ 15, -5, i };
            //actor->transform.rotation = glm::quat{ glm::vec3{ 0, 9, 0 } };
            actor->transform.rotation = glm::quat{ glm::vec3{ 0, DegreesToRadians(-90), 0}};
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }

        // Elven Gate
        {
            auto actor = CREATE_CLASS_BASE(Actor, "ElvenGate");
            actor->name = su->CreateUnique("ElvenGate");
            actor->transform.position = glm::vec3{ 0, -4, 45 };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }

        // Large Elven House
        {
            auto actor = CREATE_CLASS_BASE(Actor, "ElvenHouse7");
            actor->name = su->CreateUnique("ElvenHouse7");
            actor->transform.position = glm::vec3{ 0, -4, -55 };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }


        
#pragma endregion

        
        return true;
    }

    void World09::Shutdown()
    {
    }

    void World09::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_editor->Update();
        m_editor->ProcessGui(m_scene.get());

        // CEL-SHADING GUI
        auto program = GET_RESOURCE(Program, "shaders/cel.prog");
        if(program)program->Use();
        program->SetUniform("levels", celSteps);

        ImGui::Begin("Cel-Shading");
        ImGui::DragInt("Steps", &celSteps, 1, 2, 100);
        ImGui::End();
        // ---------------
        
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World09::Draw(Renderer& renderer)
    {
        // *** PASS 1 ***
        //m_scene->GetActorByName("postprocess")->active = false;
        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        if(program)program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights)
        {
            if (light->castShadow)
            {
                glm::mat4 shadowMatrix = light->GetShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }
         
        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto model : models)
        {
            if (model->castShadow)
            {
                //glCullFace(GL_FRONT);
                program->SetUniform("model", model->m_owner->transform.GetMatrix());
                model->model->Draw();
            }
        }

        framebuffer->Unbind();

        // *** PASS 2 ***
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
