#include "SP_pch.h"
#include "Space/Rendering/Renderer.h"

namespace Space
{
    void Renderer::Init()
    {
        m_GraphicsAPI = GraphicsAPI::Create();
        m_GraphicsAPI->Init();

        m_DrawQueue.reserve(1000);
    }

    void Renderer::BeginScene(const glm::mat4& viewProjectionMatrix)
    {
        m_ViewProjectionMatrix = viewProjectionMatrix;
        m_DrawQueue.clear();

        begunScene = true;
    }

    void Renderer::Submit(std::shared_ptr<Shader>& shader, std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        if (!begunScene) return; //TODO: Add assertion/error

        RenderCommand cmd;
        cmd.shader = shader.get();
        //cmd.material = material;
        cmd.vertexArray = vertexArray.get();
        cmd.transform = transform;

        uint64_t key = static_cast<uint64_t>(shader->GetID()) << 32;
        cmd.sortKey = key;

        m_DrawQueue.push_back(cmd);
    }

    void Renderer::EndScene()
    {
        if (!begunScene) return; //TODO: Add assertion/error

        SortQueue();
        Flush();

        begunScene = false;
    }

    void Renderer::SortQueue()
    {
        // Group identical Shader IDs together
        std::sort(m_DrawQueue.begin(), m_DrawQueue.end(),
                [](const RenderCommand& a, const RenderCommand& b)
                {
                    return a.sortKey < b.sortKey;
                });
    }

    void Renderer::Flush()
    {
        m_GraphicsAPI->SetClearColor({0.92f, 0.25f, 0.2f, 1.0f});
        m_GraphicsAPI->Clear();

        Shader* lastShader = nullptr;

        for (const auto& cmd : m_DrawQueue)
        {
            // Only bind the shader if it changed from the previous command
            if (cmd.shader != lastShader)
            {
                cmd.shader->Bind();
                cmd.shader->SetMat4("u_ViewProj", m_ViewProjectionMatrix);
                lastShader = cmd.shader;
            }

            //cmd.material->Bind();
            cmd.shader->SetMat4("u_Transform", cmd.transform);

            cmd.vertexArray->Bind();
            m_GraphicsAPI->DrawIndexed(cmd.vertexArray);
        }
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0) return;

        m_GraphicsAPI->SetViewport(0, 0, width, height);
    }

    void Renderer::Shutdown()
    {

    }
}
