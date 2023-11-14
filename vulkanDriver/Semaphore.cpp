#ifndef SEMAPHORE_CPP
#define SEMAPHORE_CPP
#include "Semaphore.h"

namespace mainNameSpace::VulkanDriver {

	Semaphore::Semaphore(const Device::Ptr& device) {
		mDevice = device;

		VkSemaphoreCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		//createInfo.flags

		if (vkCreateSemaphore(mDevice->getDevice(), &createInfo, nullptr, &mSemaphore) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create Semaphore");
		}
	}

	Semaphore::~Semaphore() {
		if (mSemaphore != VK_NULL_HANDLE) {
			vkDestroySemaphore(mDevice->getDevice(), mSemaphore, nullptr);
		}
		mDevice.reset();
	}
}




#endif // !SEMAPHORE_CPP



