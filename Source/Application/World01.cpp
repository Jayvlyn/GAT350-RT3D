#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_time += dt;

        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;

        m_angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? dt * 30 : 0;
        m_angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? -dt * 30 : 0;
    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glPushMatrix();
        glTranslatef(m_position.x + 0.5, m_position.y, 0);

        glRotatef(m_angle, 0, 0, 1);

        glScalef((sin(m_time * 5) + 1) * 0.5f, (sin(m_time * 5) + 1) * 0.5f, (sin(m_time * 5) + 1) * 0.5f);

        glBegin(GL_QUAD_STRIP);

        glColor3f(1, 0, 0);
        glVertex2f(-0.5f, 0.5f);

        glColor3f(1, 0, 0);
        glVertex2f(0.5f, 0.5f);

        glColor3f(0, 1, 1);
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0, 1, 1);
        glVertex2f(0.5f, -0.5f);

        glEnd();

        glPopMatrix();
        
        //---

        glPushMatrix();
        glTranslatef(m_position.x - 0.5, m_position.y, 0);

        glRotatef(m_angle, 1, 0, 0);

        glScalef((sin(m_time * 5) + 1) * 0.5f, (sin(m_time * 5) + 1) * 0.5f, (sin(m_time * 5) + 1) * 0.5f);

        glBegin(GL_QUAD_STRIP);

        glColor3f(1, 0, 0);
        glVertex2f(-0.5f, 0.5f);

        glColor3f(1, 0, 0);
        glVertex2f(0.5f, 0.5f);

        glColor3f(0, 1, 1);
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0, 1, 1);
        glVertex2f(0.5f, -0.5f);

        glEnd();

        glPopMatrix();

        // post-render
        renderer.EndFrame();
    }
}
