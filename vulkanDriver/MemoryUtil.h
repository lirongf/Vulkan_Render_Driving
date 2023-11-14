#ifndef  MEMORYUTIL_H
#define MEMORYUTIL_H
#include "../base.h"
#include "Buffer.h"
#include "Image.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
namespace mainNameSpace::VulkanDriver {
	class MemoryUtil
	{
	public:
		static void copyBuffer(Device::Ptr mDevice, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize size);

		static void setImageLayout(Device::Ptr mDevice, Image::Ptr mImage, VkImageLayout newLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresrouceRange, const CommandPool::Ptr commandPool);

		static void fillImageData(Device::Ptr mDevice, Image::Ptr mImage, size_t size, void* pData, const CommandPool::Ptr& commandPool);
	};
}
#endif // ! MEMORYUTIL_H



