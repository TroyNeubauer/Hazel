#pragma once

#include <memory>
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	class VertexArray
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual size_t Bytes() const = 0;

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> buffer) = 0;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> buffer) = 0;

		virtual void CalculateNormals() = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();


		virtual ~VertexArray() {}
	private:
	};

}