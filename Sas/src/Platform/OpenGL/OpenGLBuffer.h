#pragma once
#include "Renderer/Buffer.h"

namespace Sas {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
	private:
		uint32_t m_RenderID;

	};
	

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
		inline virtual uint32_t GetCount() const override { return m_Count; };
	private:
		uint32_t m_RenderID;
		uint32_t m_Count;
		
	};

}