#pragma once
#include "Space/Rendering/Shader.h"

#include <glad/glad.h>

namespace Space
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint64_t GetID() const override;
        virtual const std::string& GetName() const override;

        GLint GetUniformLocation(const std::string& name);

        virtual void SetBool(const std::string &name, bool value) override;
        virtual void SetInt(const std::string &name, int value) override;
        virtual void SetFloat(const std::string &name, float value) override;
        virtual void SetVec2(const std::string &name, const glm::vec2 &value) override;
        virtual void SetVec2(const std::string &name, float x, float y) override;
        virtual void SetVec3(const std::string &name, const glm::vec3 &value) override;
        virtual void SetVec3(const std::string &name, float x, float y, float z) override;
        virtual void SetVec4(const std::string &name, const glm::vec4 &value) override;
        virtual void SetVec4(const std::string &name, float x, float y, float z, float w) override;
        virtual void SetMat2(const std::string &name, const glm::mat2 &mat) override;
        virtual void SetMat3(const std::string &name, const glm::mat3 &mat) override;
        virtual void SetMat4(const std::string &name, const glm::mat4 &mat) override;
    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

        void Compile(std::unordered_map<GLenum, std::string> shaderSources);
        void Compile(const char* vertexSrc, const char* fragmentSrc);
    private:
        uint32_t m_ShaderID = 0;
        std::string m_Name;

        std::unordered_map<std::string, GLint> m_UniformLocationCache;
    };
}
