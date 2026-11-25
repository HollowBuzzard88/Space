#include "SP_pch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <sstream>

namespace Space
{
    std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
    {
        return std::make_shared<OpenGLShader>(filepath);
    }
    std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
    {
        return std::make_shared<OpenGLShader>(name, vertexPath, fragmentPath);
    }

    uint64_t OpenGLShader::GetID() const { return m_ShaderID; }
    const std::string& OpenGLShader::GetName() const { return m_Name; }

    void OpenGLShader::Bind() const { glUseProgram(m_ShaderID); }
    void OpenGLShader::Unbind() const { glUseProgram(0); }


    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        SP_CORE_ERROR("Unknown ShaderType", type);
        return 0;
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
        }
        else
        {
            //TODO: Throw error
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string &source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Find first '#type'

        while (pos != std::string::npos)
        {
            size_t endOfLine = source.find_first_of("\r\n", pos);
            size_t begin = pos + typeTokenLength + 1;   // Start of shader type name
            std::string type = source.substr(begin, endOfLine - begin);     // #type pixel
                                                                            // ^--->^
            size_t nextLinePos = source.find_first_not_of("\r\n", endOfLine); // Start of shader code
            pos = source.find(typeToken, nextLinePos);

            shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ?
                source.substr(nextLinePos) :
                source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
    {
        GLuint program = glCreateProgram();

        std::vector<GLuint> shaderIDs;
        shaderIDs.reserve(shaderSources.size());

        uint8_t i = 0;
        for (const auto& [type, source] : shaderSources)
        {
            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, nullptr);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(program, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);
                
                SP_CORE_ERROR("{0}", infoLog.data());
                break;
            }

            glAttachShader(program, shader);
            shaderIDs.push_back(shader);

            i++;
        }

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            for (auto id : shaderIDs)
                glDeleteShader(id);
                
            SP_CORE_ERROR("{0}", infoLog.data());
            return;
        }

        for (auto id : shaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_ShaderID = program;
    }

    void OpenGLShader::Compile(const char* vertexSrc, const char* fragmentSrc)
    {
        GLuint program = glCreateProgram();

        auto CheckShaderCompile = [](GLuint shader)
        {
            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                SP_CORE_ERROR("{0}", infoLog.data());
                return;
            }
        };

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSrc, NULL);
        glCompileShader(vertex);
        CheckShaderCompile(vertex);
        glAttachShader(program, vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSrc, NULL);
        glCompileShader(fragment);
        CheckShaderCompile(fragment);
        glAttachShader(program, fragment);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            SP_CORE_ERROR("{0}", infoLog.data());
            return;
        }

        glDetachShader(program, vertex);
        glDetachShader(program, fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        m_ShaderID = program;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        // Extract file name as shader name
        size_t lastSlash = filepath.find_last_of("/\\");
        lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
        size_t lastDot = filepath.rfind('.');
        size_t count = (lastDot == std::string::npos) ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        m_Name = name;

        std::string v = ReadFile(vertexSrc), f = ReadFile(fragmentSrc);
        Compile(v.c_str(), f.c_str());
    }

    OpenGLShader::~OpenGLShader() { glDeleteProgram(m_ShaderID); }

    GLint OpenGLShader::GetUniformLocation(const std::string& name)
    {
        auto it = m_UniformLocationCache.find(name);
        if (it != m_UniformLocationCache.end()) // uniform was already recorded in the cache
            return it->second;
        
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        if (location == -1)
            SP_CORE_ERROR("Shader '{0}': uniform '{1}' not found (misspelled, or omptimized out)", m_Name, name);
        
        m_UniformLocationCache[name] = location;
        return location;
    }

    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetBool(const std::string& name, bool value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value)
    {
        glUniform2fv(GetUniformLocation(name), 1, &value[0]);
    }
    void OpenGLShader::SetVec2(const std::string& name, float x, float y)
    {
        glUniform2f(GetUniformLocation(name), x, y);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3fv(GetUniformLocation(name), 1, &value[0]);
    }
    void OpenGLShader::SetVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(GetUniformLocation(name), 1, &value[0]);
    }
    void OpenGLShader::SetVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(GetUniformLocation(name), x, y, z, w);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& mat)
    {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    // ----------------------------------------------------------------------------------------
    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
}
