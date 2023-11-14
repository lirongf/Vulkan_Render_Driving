#ifndef COMMANDPOOL_CPP
#define COMMANDPOOL_CPP

#include "CommandPool.h"

namespace mainNameSpace::VulkanDriver {
	CommandPool::CommandPool(const Device::Ptr& device, VkCommandPoolCreateFlagBits flag) {
		mDevice = device;
		VkCommandPoolCreateInfo createInfo{};
		//VK_COMMAND_POOL_CREATE_TRANIENT_BIT һ����ʹ��
		//VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BITͨ������vkResetCommandBuffer�����ڵ���vkBeginCommandBufferʱͨ����ʽ���á�
		//VK_COMMAND_POOL_CREATE_PROTECTED_BITָ���ӳ��з��������������ܱ������������ʱ��
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = device->getGraphicQueueFamily();
		createInfo.flags = flag;
		if (vkCreateCommandPool(mDevice->getDevice(), &createInfo, nullptr, &mCommandPool) != VK_SUCCESS) {
			throw std::runtime_error("Error:  failed to create command pool");
		}
	}

	CommandPool::~CommandPool() {
		if (mCommandPool != VK_NULL_HANDLE) {
			vkDestroyCommandPool(mDevice->getDevice(), mCommandPool, nullptr);
		}
	}
}
#endif // !COMMANDPOOL_CPP


