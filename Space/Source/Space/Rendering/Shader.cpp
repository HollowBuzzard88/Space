#include "SP_pch.h"
#include "Space/Rendering/Shader.h"

#include <glad/glad.h>

namespace Space
{
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        char* vShaderCode = nullptr;
        char* fShaderCode = nullptr;

        auto ReadFileRaw = [](const char* path) -> char*
        {
            FILE* file = fopen(path, "rb");
            if (!file)
            {
                std::cout << "ERROR::SHADER_FILE::NOT_FOUND: " << path << "\n";
                return nullptr;
            }

            fseek(file, 0, SEEK_END);
            long length = ftell(file);
            fseek(file, 0, SEEK_SET);

            char* buffer = (char*)malloc(length + 1);
            if (buffer)
            {
                size_t readBytes = fread(buffer, 1, length, file);
                buffer[readBytes] = '\0'; // Null termination at end of file
            }
            fclose(file);
            return buffer;
        };

        vShaderCode = ReadFileRaw(vertexPath);
        fShaderCode = ReadFileRaw(fragmentPath);

        // When ReadFileRaw() returns an error
        if (!vShaderCode || !fShaderCode)
        {
            free(vShaderCode);
            free(fShaderCode);
            ID = 0;
            return;
        }

        // Compile Shaders
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileError(vertex, "VERTEX");

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileError(fragment, "FRAGMENT");

        // Link Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        CheckCompileError(ID, "PROGRAM");

        // Delete no-longer-needed Shaders because they're now linked to the program
        free(vShaderCode);
        free(fShaderCode);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::Activate()
    {
        if (ID == 0)
        {
            std::cout << "WARNING: trying to use invalid shader program\n";
            return;
        }
        glUseProgram(ID);
    }

    // ------------------------------------------------------------------------
    void Shader::SetBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::SetFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::SetVec2(const std::string &name, glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void Shader::SetVec3(const std::string &name, glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void Shader::SetVec4(const std::string &name, glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void Shader::SetMat2(const std::string &name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::SetMat3(const std::string &name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::SetMat4(const std::string &name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


    void Shader::CheckCompileError(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
            }
        }
        else
        {
            glGetShaderiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
            }
        }
    }
}
