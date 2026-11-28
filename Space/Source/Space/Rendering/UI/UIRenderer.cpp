#include "SP_pch.h"
#include "Space/Rendering/UI/UIRenderer.h"

#include "Space/Core/AssetManager.h"

namespace Space
{
    std::shared_ptr<UIRenderer> UIRenderer::s_Instance = nullptr;

    void UIRenderer::Init(std::shared_ptr<Renderer> renderer)
    {
        m_Renderer = renderer;

        m_DrawQueue.reserve(1000);


        m_QuadVA = VertexArray::Create();
        float v[4 * 2] = { -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f };
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<VertexBuffer> VB = VertexBuffer::Create(v, sizeof(v));
        VB->SetLayout({ { ShaderDataType::Float2, "inPos" } });
        m_QuadVA->AddVertexBuffer(VB);
        m_QuadVA->SetIndexBuffer(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_QuadShader = Shader::Create(AssetManager::GetAbsolutePath("Shaders/UIFlatColor.glsl"));


        m_TextRenderer = std::make_shared<TextRenderer>();

        if (!m_TextRenderer->Init(AssetManager::GetAbsolutePath("Fonts/consola.ttf"), 18.0f))
            m_TextRenderer->Init("C:/Windows/Fonts/consola.ttf");
    }

    void UIRenderer::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(position + size * 0.5f, 0.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
        
        m_QuadShader->Bind();
        m_QuadShader->SetMat4("uViewProj", m_ViewProjectionMatrix);
        m_QuadShader->SetMat4("uTransform", t);
        m_QuadShader->SetVec4("uCol", color);
        m_QuadVA->Bind();
        DrawIndexed(m_QuadVA.get());
    }

    void UIRenderer::SubmitLate(std::function<void()> fn)
    {
        if (!fn)
        {
            SP_CORE_ERROR("Invalid function");
        }

        m_LateCallbacks.push_back(std::move(fn));
    }

    void UIRenderer::BeginFrame(const glm::mat4& viewProjectionMatrix)
    {
        if (begunScene)
        {
            SP_CORE_WARN("Can't begin a frame before the last one is finished.");
            return;
        }

        m_ViewProjectionMatrix = viewProjectionMatrix;
        m_DrawQueue.clear();
        m_LateCallbacks.clear();
        m_Widgets.clear();

        begunScene = true;
    }

    void UIRenderer::Submit(Widget* widget)
    {
        if (!begunScene)
        {
            SP_CORE_ERROR("Unable to submit render call before 'BeginFrame()' has been called.");
            return;
        }

        m_Widgets.push_back(widget);
    }

    void UIRenderer::Submit(const std::vector<Widget*> widgets)
    {
        if (!begunScene)
        {
            SP_CORE_ERROR("Unable to submit render call before 'BeginFrame()' has been called.");
            return;
        }

        m_Widgets.append_range(widgets);
    }

    void UIRenderer::EndFrame()
    {
        if (!begunScene)
        {
            SP_CORE_ERROR("Unable to 'EndFrame()' before 'BeginFrame()' has been called.");
            return;
        }

        for (Widget* w : m_Widgets)
        {
            w->EarlyUpdate(s_Instance, m_Renderer);

            RenderCommand cmd;
            cmd.shader = w->GetShader().get();
            cmd.vertexArray = w->GetVertexArray().get();
            cmd.transform = w->GetTransform();
            cmd.color = w->GetColor();
            cmd.sortKey = static_cast<uint64_t>(w->GetShader()->GetID()) << 32;
            m_DrawQueue.push_back(cmd);
        }

        SortQueue();
        Flush();

        for (Widget* widget : m_Widgets) { widget->LateUpdate(s_Instance, m_Renderer); }
        for (auto& fn : m_LateCallbacks) { fn(); }

        m_Widgets.clear();
        m_LateCallbacks.clear();

        begunScene = false;
    }

    void UIRenderer::SortQueue()
    {
        // Group identical Shader IDs together
        std::stable_sort(m_DrawQueue.begin(), m_DrawQueue.end(),
                [](const RenderCommand& a, const RenderCommand& b)
                {
                    return a.sortKey < b.sortKey;
                });
    }

    void UIRenderer::Flush()
    {
        Shader* lastShader = nullptr;

        for (const auto& cmd : m_DrawQueue)
        {
            // Only bind the shader if it changed from the previous command
            if (cmd.shader != lastShader)
            {
                cmd.shader->Bind();
                cmd.shader->SetMat4("uViewProj", m_ViewProjectionMatrix);
                lastShader = cmd.shader;
            }

            //cmd.material->Bind();
            cmd.shader->SetMat4("uTransform", cmd.transform);
            cmd.shader->SetVec4("uCol", cmd.color);

            cmd.vertexArray->Bind();
            DrawIndexed(cmd.vertexArray);
        }
    }

    void UIRenderer::Shutdown()
    {

    }
}
