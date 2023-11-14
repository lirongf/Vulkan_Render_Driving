#ifndef EVENT_H
#define EVENT_H
#include "../base.h"
#include "Device.h"
namespace mainNameSpace::VulkanDriver {
	class Event
	{
	public:
		using Ptr = std::shared_ptr<Event>;
		static Ptr create(const Device::Ptr& device,bool deviceOnly) { return std::make_shared<Event>(device, deviceOnly); }
		Event(const Device::Ptr& device, bool deviceOnly);
		~Event();
		[[nodiscard]] auto getEvent() const { return mEvent; }
	private:
		VkEvent mEvent{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };
	};
}

#endif // !EVENT_H



