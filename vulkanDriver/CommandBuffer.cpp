#ifndef COMMANDBUFFER_CPP
#define COMMANDBUFFER_CPP
#include "CommandBuffer.h"

namespace mainNameSpace::VulkanDriver {

	CommandBuffer::CommandBuffer(const Device::Ptr& device, const CommandPool::Ptr& commandPool, bool asSecondary) {
		mdevice = device;
		mCommandPool = commandPool;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandBufferCount = 1;
		allocInfo.commandPool = mCommandPool->getCommandPool();
		allocInfo.level = asSecondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(mdevice->getDevice(), &allocInfo, &mCommandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Error: falied to create commandBuffer");
		}
	}

	CommandBuffer::~CommandBuffer() {
		if (mCommandBuffer != VK_NULL_HANDLE) {
			vkFreeCommandBuffers(mdevice->getDevice(), mCommandPool->getCommandPool(), 1, &mCommandBuffer);
		}
		mCommandPool.reset();
		mdevice.reset();
	}

	void CommandBuffer::begin(VkCommandBufferUsageFlags flag, const VkCommandBufferInheritanceInfo& inheritance) {
		
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT = 0x00000001,每次记录将只提交一次，并且命令缓冲区将在每次提交之间重置并再次记录。
		//VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT = 0x00000002,指定辅助命令缓冲区被视为完全在渲染过程中。如果这是主命令缓冲区，则忽略此位。
		//VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT = 0x00000004,指定命令缓冲区可以在处于挂起状态时重新提交到同一队列族的任何队列，并记录到多个主命令缓冲区中。
		beginInfo.flags = flag;
		beginInfo.pInheritanceInfo = &inheritance;

		if (vkBeginCommandBuffer(mCommandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to begin commandBuffer");
		}
	}

	void CommandBuffer::end() {
		if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to end Command Buffer");
		}
	}

	void CommandBuffer::beginRenderPass(const VkRenderPassBeginInfo& renderPassBeginInfo,const VkSubpassContents& subPassContents) {
		vkCmdBeginRenderPass(mCommandBuffer, &renderPassBeginInfo, subPassContents);
	}

	void CommandBuffer::bindGraphicPipeline(const VkPipeline& pipeline) {
		vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}

	void CommandBuffer::bindVertexBuffer(const std::vector<VkBuffer>& buffers, uint32_t firstBinding, std::vector<VkDeviceSize> &offsets) {
		vkCmdBindVertexBuffers(mCommandBuffer, firstBinding, static_cast<uint32_t>(buffers.size()), buffers.data(), offsets.data());
	}

	void CommandBuffer::bindIndexBuffer(const VkBuffer& buffer,VkDeviceSize offset,VkIndexType indexformat){
		vkCmdBindIndexBuffer(mCommandBuffer, buffer, offset, indexformat);
	}

	void CommandBuffer::bindDescriptorSet(VkPipelineBindPoint bindPoint,VkPipelineLayout layout,uint32_t firstSet,uint32_t setCout,VkDescriptorSet* descriptorSets) {
		vkCmdBindDescriptorSets(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, firstSet, setCout, descriptorSets, 0, nullptr);
	}

	void CommandBuffer::draw(uint32_t vertexCount,uint32_t instanceCount,uint32_t vertexOffset,uint32_t instanceOffset) {
		vkCmdDraw(mCommandBuffer, vertexCount, instanceCount, vertexOffset, instanceOffset);
	}
	
	void CommandBuffer::drawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance) {
		vkCmdDrawIndexed(mCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CommandBuffer::copyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t copyRegions, const std::vector<VkBufferCopy>& copyInfos) {
		vkCmdCopyBuffer(mCommandBuffer, srcBuffer, dstBuffer, copyRegions, copyInfos.data());
	}

	//TODO
	void CommandBuffer::copyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t width, uint32_t height) {
		VkBufferImageCopy region{};
		region.bufferOffset = 0;

		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { width, height, 1 };

		vkCmdCopyBufferToImage(mCommandBuffer, srcBuffer, dstImage, dstImageLayout, 1, &region);
	}

	void CommandBuffer::submitSync(VkQueue queue, VkFence fence) {
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mCommandBuffer;
		if (vkQueueSubmit(queue, 1, &submitInfo, fence) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to QueueSubmit commandBuffers");
		}
		vkQueueWaitIdle(queue);
	}

	void CommandBuffer::endRenderPass() {
		vkCmdEndRenderPass(mCommandBuffer);
	}
	//Pipeline Barrier同样是Vulkan中的一个同步原语，它能够提供更细粒度的控制，并且只能完成Queue内的同步不能够跨Queue。Pipeline Barrier可以被译为管线屏障，
	void CommandBuffer::transferImageLayout(const VkImageMemoryBarrier& imageMemoryBarrier, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {
		vkCmdPipelineBarrier(
			mCommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,//MemoryBarrier
			0, nullptr, //BufferMemoryBarrier
			1, &imageMemoryBarrier
		);
	}

	void CommandBuffer::setEvent(Event::Ptr Event) {
	
	}

	void CommandBuffer::waitEvent(Event::Ptr Event) {
		
	}
}
#endif
