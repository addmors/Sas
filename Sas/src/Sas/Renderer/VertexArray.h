#pragma once
#include <memory>
#include "Buffer.h"
namespace Sas {
	class VertexArray {
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void   AddVertexBufer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void   SetIndexBufer(const Ref<IndexBuffer>& indexBuffer) = 0;
		
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		static Ref<VertexArray> Create();
	};

}