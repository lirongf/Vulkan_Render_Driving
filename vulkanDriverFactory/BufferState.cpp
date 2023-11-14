
#ifndef BUFFERSTATE_CPP
#define BUFFERSTATE_CPP

#include "BufferState.h"

namespace mainNameSpace::VulkanDriverFactory {
	VkFormat BufferState::getVertexFormat(VertexElementFormat format){
		switch (format)
		{
		case VertexElementFormat::single:
			return VkFormat::VK_FORMAT_R32_SFLOAT;
			break;
		case VertexElementFormat::vector2:
			return VkFormat::VK_FORMAT_R32G32_SFLOAT;
			break;
		case VertexElementFormat::vector3:
			return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
			break;
		case VertexElementFormat::vector4:
			return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case VertexElementFormat::color:
			return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case VertexElementFormat::byte4:
			return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
			break;
		case VertexElementFormat::short2:
			return VkFormat::VK_FORMAT_R16G16_SFLOAT;
			break;
		case VertexElementFormat::short4:
			return VkFormat::VK_FORMAT_R8G8B8A8_UINT;
			break;
		case VertexElementFormat::halfvector2:
			return VkFormat::VK_FORMAT_R16G16_UNORM;
			break;
		case VertexElementFormat::halfvector4:
			return VkFormat::VK_FORMAT_R16G16B16A16_UNORM;
			break;
		case VertexElementFormat::nbyte4:
			return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
			break;
		default:
			break;
		};
		return VkFormat::VK_FORMAT_UNDEFINED;
	}
	BufferState::BufferState(){
		
	}
	BufferState::~BufferState(){
		mVertexBuffers.clear();
		mIndexBuffer.reset();
	}

	void BufferState::setVertexDeclarations(std::vector<VertexDeclaration> declarations) {
		mDeclarations = declarations;
		vertexBindDescription.clear();
		vertexAttributeDescription.clear();
		uint32_t i = 0;
		vertexBindDescription.resize(mDeclarations.size());
		for (const VertexDeclaration &dec : mDeclarations) {
			VkVertexInputBindingDescription bindDescript{};
			bindDescript.binding = i;
			bindDescript.inputRate = dec.isntance ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
			bindDescript.stride = dec.vertexStride * sizeof(float);
			for (const VertexAttributeElement& element : dec.vertexElements) {
				VkVertexInputAttributeDescription attributeDescript{};
				attributeDescript.binding = i;
				attributeDescript.format = BufferState::getVertexFormat(element.format);
				attributeDescript.location = element.location;
				attributeDescript.offset = element.offset * sizeof(float);
				vertexAttributeDescription.push_back(attributeDescript);
			}
			vertexBindDescription[i] = bindDescript;
			i++;
		}
		
	}
}

#endif // ! GLFWWindow_CPP
