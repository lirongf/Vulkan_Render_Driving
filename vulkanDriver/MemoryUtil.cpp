#ifndef  MEMORYUTIL_CPP
#define MEMORYUTIL_CPP
#include "MemoryUtil.h"
namespace mainNameSpace::VulkanDriver {

	void MemoryUtil::copyBuffer(Device::Ptr mDevice, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize size) {
		
		CommandPool::Ptr commandPool = CommandPool::create(mDevice,VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
		CommandBuffer::Ptr commandBuffer = CommandBuffer::create(mDevice, commandPool);

		commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, {});
		VkBufferCopy copyInfo{};
		copyInfo.size = size;
		commandBuffer->copyBufferToBuffer(srcBuffer, dstBuffer, 1, { copyInfo });

		commandBuffer->end();

		commandBuffer->submitSync(mDevice->getGraphicQueue(), VK_NULL_HANDLE);
	}


	void MemoryUtil::setImageLayout(Device::Ptr mDevice,Image::Ptr mImage, VkImageLayout newLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresrouceRange, CommandPool::Ptr commandPool ) {
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.oldLayout =mImage->getLayout();
		imageMemoryBarrier.newLayout = newLayout;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.image = mImage->getImage();
		imageMemoryBarrier.subresourceRange = subresrouceRange;

		switch (mImage->getLayout())
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			imageMemoryBarrier.srcAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		default:
			break;
		}

		switch (newLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: 
			if (imageMemoryBarrier.srcAccessMask == 0) {
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: 
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: {
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}
			break;
		default:
			break;
		}
		mImage->setLayout(newLayout);
		CommandPool::Ptr compool = (commandPool == nullptr) ? CommandPool::create(mDevice): commandPool;
		
		CommandBuffer::Ptr commandBuffer = CommandBuffer::create(mDevice, compool);
		commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, {});
		commandBuffer->transferImageLayout(imageMemoryBarrier, srcStageMask, dstStageMask);
		commandBuffer->end();

		commandBuffer->submitSync(mDevice->getGraphicQueue());
		commandBuffer.reset();
	}

	void MemoryUtil::fillImageData(Device::Ptr mDevice, Image::Ptr mImage, size_t size, void* pData, const CommandPool::Ptr& commandPool) {
		assert(pData);
		assert(size);

		auto stageBuffer = Buffer::createStageBuffer(mDevice, size, pData);

		auto commandBuffer = CommandBuffer::create(mDevice, commandPool);
		commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, {});
		commandBuffer->copyBufferToImage(stageBuffer->getBuffer(), mImage->getImage(), mImage->getLayout(),mImage->getWidth(),mImage->getHeight());
		commandBuffer->end();
		commandBuffer->submitSync(mDevice->getGraphicQueue());
	}

}
#endif
