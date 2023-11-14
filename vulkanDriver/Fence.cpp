#ifndef FENCE_CPP
#define FENCE_CPP
#include "Fence.h"

namespace mainNameSpace::VulkanDriver {

	void Fence::waiteFences(const Device::Ptr& device,std::vector<Fence::Ptr> fences, uint64_t timeout) {
		std::vector<VkFence> vkfence{};
		for (const Fence::Ptr fence : fences)
			vkfence.push_back(fence->getFence());
		vkWaitForFences(device->getDevice(), 1, vkfence.data(), VK_TRUE, timeout);
	}

	Fence::Fence(const Device::Ptr& device, bool signaled) {
		mDevice = device;

		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		//VK_FENCE_CREATE_SIGNALED_BIT指定在信号状态下创建围栏对象。否则，它将在无信号状态下创建。
		createInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

		if (vkCreateFence(mDevice->getDevice(), &createInfo, nullptr, &mFence) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create fence");
		}
	}

	Fence::~Fence() {
		if (mFence != VK_NULL_HANDLE) {
			vkDestroyFence(mDevice->getDevice(), mFence, nullptr);
		}
		mDevice.reset();
	}

	void Fence::resetFence() {
		vkResetFences(mDevice->getDevice(), 1, &mFence);
	}

	void Fence::waiteFence(uint64_t timeout) {
		vkWaitForFences(mDevice->getDevice(), 1, &mFence, VK_TRUE, timeout);
	}
}

#endif