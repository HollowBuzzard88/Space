#pragma once

#include <glm/glm.hpp>

class VertexArray;
class Shader;
//class Material;

namespace Space
{
    struct RenderCommand
    {
        uint64_t sortKey;
        VertexArray* vertexArray;
        Shader* shader;
        //Material* material;
        glm::mat4 transform;
        glm::vec4 color;
    };
}
