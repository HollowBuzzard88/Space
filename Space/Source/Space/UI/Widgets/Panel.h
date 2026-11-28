#pragma once

#include "Space/UI/Widget.h"

namespace Space
{
    class Panel : public Widget
    {
    public:
        Panel(const glm::vec2& position, const glm::vec2& size, const Color& color)
            : Widget(position, size, color)
        {
            m_VA = VertexArray::Create();
            float vertices[4 * 2] =
            {
               -0.5f, -0.5f,
                0.5f, -0.5f,
                0.5f,  0.5f,
               -0.5f,  0.5f
            };
            uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

            std::shared_ptr<VertexBuffer> VB = VertexBuffer::Create(vertices, sizeof(vertices));
            VB->SetLayout({
                { ShaderDataType::Float2, "inPos" }
            });
            m_VA->AddVertexBuffer(VB);

            std::shared_ptr<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
            m_VA->SetIndexBuffer(IB);

            m_Shader = Shader::Create(AssetManager::GetAbsolutePath("Shaders/UIFlatColor.glsl"));
        }

        virtual void EarlyUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer) override
        {
            m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position + m_Size / 2.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Size, 1.0f));
        }

        virtual void LateUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer) override
        {
        }
    };
}
