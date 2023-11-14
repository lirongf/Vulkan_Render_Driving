#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include "../base.h"
#include "Device.h"
#include "CommandPool.h"
#include "Event.h"
namespace mainNameSpace::VulkanDriver {
	class CommandBuffer
	{
	public:
		using Ptr = std::shared_ptr<CommandBuffer>;
		static Ptr create(const Device::Ptr& device, const CommandPool::Ptr& commandPool, bool asSecondary = false) {
			return std::make_shared<CommandBuffer>(device, commandPool, asSecondary);
		}

		CommandBuffer(const Device::Ptr& device, const CommandPool::Ptr& commandPool, bool asSecondary = false);

		~CommandBuffer();

		void begin(VkCommandBufferUsageFlags flag, const VkCommandBufferInheritanceInfo& inheritance);

		void end();
		
		void beginRenderPass(const VkRenderPassBeginInfo& renderPassBeginInfo, const VkSubpassContents& subPassContents);

		void endRenderPass();

		void bindGraphicPipeline(const VkPipeline& pipeline);

		void bindVertexBuffer(const std::vector<VkBuffer>& buffers, uint32_t firstBinding, std::vector<VkDeviceSize>& offsets);

		void bindIndexBuffer(const VkBuffer& buffer, VkDeviceSize offset, VkIndexType indexformat);

		void bindDescriptorSet(VkPipelineBindPoint bindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCout, VkDescriptorSet* descriptorSets);

		void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);

		void drawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance);

		void copyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t copyRegions, const std::vector<VkBufferCopy>& copyInfos);

		void copyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t width, uint32_t height);

		void submitSync(VkQueue queue, VkFence fence = VK_NULL_HANDLE);

		void transferImageLayout(const VkImageMemoryBarrier& imageMemoryBarrier, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask);

		void setEvent(Event::Ptr Event);

		void waitEvent(Event::Ptr Event);
		[[nodiscard]] VkCommandBuffer getCommandBuffer() { return mCommandBuffer; };

	private:
		VkCommandBuffer mCommandBuffer{ VK_NULL_HANDLE };
		Device::Ptr mdevice{ nullptr };
		CommandPool::Ptr mCommandPool{ nullptr };
	};
}
#endif
