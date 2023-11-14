#ifndef BUFFERSTATE_H
#define BUFFERSTATE_H

#include "../base.h"
#include "../vulkanDriver/Buffer.h"


//记录所有顶点的相关信息，包括顶点布局，index顶点，Vertex顶点数据
namespace mainNameSpace::VulkanDriverFactory {
	enum VertexElementFormat
	{
		single,
		vector2,
		vector3,
		vector4,
		color,
		byte4,
		short2,
		short4,
		halfvector2,
		halfvector4,
		nbyte4
	};
	struct VertexAttributeElement {
		uint32_t location;
		VertexElementFormat format;
		uint32_t offset;
	};
	struct VertexDeclaration
	{
		uint32_t vertexStride;
		std::vector<VertexAttributeElement> vertexElements;
		bool isntance;
		
	};
	class BufferState {
	public:
		using Ptr = std::shared_ptr<BufferState>;
		static Ptr create() { return std::make_shared<BufferState>(); }
		VkFormat static getVertexFormat(VertexElementFormat format);
	public:

		BufferState();

		~BufferState();

		void setVertexBuffers(std::vector<VulkanDriver::Buffer::Ptr> buffers) {
			mVertexBuffers = buffers;
		};

		void setIndexBuffer(VulkanDriver::Buffer::Ptr indexBuffer) {
			mIndexBuffer = indexBuffer;
		}

		[[nodiscard]] VulkanDriver::Buffer::Ptr getIndexBuffer() const { return mIndexBuffer; }

		[[nodiscard]] std::vector<VulkanDriver::Buffer::Ptr> getVertexBuffers() const {
			return mVertexBuffers;
		}

		[[nodiscard]] std::vector<VkVertexInputBindingDescription> getVertexBindDescription() {
			return vertexBindDescription;
		}

		[[nodiscard]] std::vector<VkVertexInputAttributeDescription> getAttributeDescription() {
			return vertexAttributeDescription;
		}

		void setVertexDeclarations(std::vector<VertexDeclaration> declarations);
		
	private:
		std::vector<VertexDeclaration> mDeclarations;
		std::vector<VkVertexInputBindingDescription> vertexBindDescription;
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescription;
		VulkanDriver::Buffer::Ptr mIndexBuffer{ nullptr };
		std::vector<VulkanDriver::Buffer::Ptr> mVertexBuffers;
	};
}

#endif // ! BUFFERSTATE_H