#ifndef EVENT_CPP
#define EVNET_CPP
#include "Event.h"
namespace mainNameSpace::VulkanDriver {
	Event::Event(const Device::Ptr& device, bool deviceOnly) {
		mDevice = device;
		VkEventCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;

		createInfo.flags = deviceOnly ? VK_EVENT_CREATE_DEVICE_ONLY_BIT : 0;
		if (vkCreateEvent(mDevice->getDevice(), &createInfo, nullptr, &mEvent) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create fence");
		}

	}

	Event::~Event() {
		if (mEvent != VK_NULL_HANDLE) {
			vkDestroyEvent(mDevice->getDevice(), mEvent, nullptr);
		}
		mDevice.reset();
	}

}
#endif // !EVENT_CPP


