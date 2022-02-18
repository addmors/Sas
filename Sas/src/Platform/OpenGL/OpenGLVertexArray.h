#pragma once
#include <memory>
#include "Sas/Renderer/VertexArray.h"


namespace Sas {
	class OpenGLVertexArray : public VertexArray {
	public:
		virtual ~OpenGLVertexArray();

		OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void   AddVertexBufer(const Ref<VertexBuffer>& vertexBuffer)  override;
		virtual void   SetIndexBufer(const Ref<IndexBuffer>& indexBuffer)  override;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:	
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;

	};

	

}