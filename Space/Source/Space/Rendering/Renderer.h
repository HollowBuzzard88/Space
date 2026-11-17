#pragma once

#include "Space/Rendering/Mesh.h"

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
        static unsigned int ShaderProgram;
        static std::vector<float> verts;

        static constexpr const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        static constexpr const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n\0";
    };
}
