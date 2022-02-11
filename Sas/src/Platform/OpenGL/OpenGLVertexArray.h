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

		virtual void   AddVertexBufer(const std::shared_ptr<VertexBuffer>& vertexBuffer)  override;
		virtual void   SetIndexBufer(const std::shared_ptr<IndexBuffer>& indexBuffer)  override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:	
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	};

	

}