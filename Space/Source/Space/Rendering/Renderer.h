#pragma once

#include "Space/Rendering/Mesh.h"
#include "Space/Rendering/Shader.h"

namespace Space
{
    class Renderer
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);
        
        static void Draw(Mesh element); // Add elements to the renderQueue
        static void Render();

        static API GetAPI() { return s_API; }
    private:
        static std::vector<Mesh> m_drawQueue;
        static API s_API;

        static unsigned int VBO, VAO;
        static std::vector<float> verts;

        static Shader ShaderProgram;
    };
}
