#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Space/Core/AssetManager.h"

#include "Space/Rendering/VertexArray.h"
#include "Space/Rendering/Shader.h"

#include "Space/UI/Color.h"

namespace Space
{
    class UIRenderer;
    class Renderer;

    class Widget
    {
    public:
        explicit Widget(const glm::vec2& position, const glm::vec2& size, const Color color)
            : m_Position(position), m_Size(size), m_Color(color)
        {
            m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position + m_Size * 0.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Size, 1.0f));
        }

        virtual ~Widget() = default;

        void SetPosition(const glm::vec2& p) { m_Position = p; }
        void SetSize(const glm::vec2& s) { m_Size = s; }
        void SetColor(const Color& c) { m_Color = c; }

        // Called before the widget is rendered
        virtual void EarlyUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer) = 0;

        // Called after the widget is rendered allowing for additions to the widget above the main body
        virtual void LateUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer) = 0;
    public:
        glm::vec2 GetPosition() const { return m_Position; }
        glm::vec2 GetSize() const { return m_Size; }
        Color GetColor() const { return m_Color; }

        std::shared_ptr<VertexArray> GetVertexArray() const { return m_VA; }
        std::shared_ptr<Shader> GetShader() const { return m_Shader; }

        glm::mat4 GetTransform() const { return m_Transform; }
    protected:
        glm::vec2 m_Position, m_Size;
        Color m_Color;

        std::shared_ptr<VertexArray> m_VA;
        std::shared_ptr<Shader> m_Shader;

        glm::mat4 m_Transform;
    };
}
