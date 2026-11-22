#include "SP_pch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Space
{
    /////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) { return std::make_shared<OpenGLVertexBuffer>(size); }
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) { return std::make_shared<OpenGLVertexBuffer>(vertices, size); }

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        //TODO: Change GL_DYNAMIC_DRAW to GL_STATIC_DRAW or GL_STREAM_DRAW based on purpose (see more: https://learnopengl.com/Getting-started/Hello-Triangle)
        this->Unbind();
    }
    
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        glCreateBuffers(1, &m_RendererID);
        this->Bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        //TODO: Change GL_STATIC_DRAW to GL_DYNAMIC_DRAW or GL_STREAM_DRAW based on purpose (see more: https://learnopengl.com/Getting-started/Hello-Triangle)
        this->Unbind();
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }
    void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        this->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    /////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
    
    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) { return std::make_shared<OpenGLIndexBuffer>(indices, size); }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        //TODO: Change GL_STATIC_DRAW to GL_DYNAMIC_DRAW or GL_STREAM_DRAW based on purpose (see more: https://learnopengl.com/Getting-started/Hello-Triangle)
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }
    void OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
}
