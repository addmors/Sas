#include "ssph.h"
#include "OpenGLBuffer.h"

#include  <glad/glad.h>


namespace Sas {
	
	////////////////////////////////////////////////////////////
	//VertexBuffer /////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		SS_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		SS_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		SS_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RenderID);
	}
	void OpenGLVertexBuffer::Bind() const
	{
		SS_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		SS_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		SS_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	////////////////////////////////////////////////////////////
	// IndexBuffer /////////////////////////////////////////////
	////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		SS_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RenderID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		SS_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RenderID);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		SS_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		SS_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}