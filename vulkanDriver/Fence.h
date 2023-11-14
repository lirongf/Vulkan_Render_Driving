#ifndef FENCE_H
#define FENCE_H
#include "../base.h"
#include "Device.h"
namespace mainNameSpace::VulkanDriver {
	class Fence
	{
	public:
		using Ptr = std::shared_ptr<Fence>;
		static Ptr create(const Device::Ptr& device, bool signaled = true, uint64_t timeout = UINT64_MAX) {
			return std::make_shared<Fence>(device, signaled);
		}
		static void waiteFences(const Device::Ptr& device,std::vector<Fence::Ptr> fences, uint64_t timeout = UINT64_MAX);
	public:
		//主要被用来完成CPU和GPU之间的同步
		Fence(const Device::Ptr& device, bool signaled = true);
		~Fence();
		[[nodiscard]] auto getFence() const { return mFence; }

		void resetFence();

		void waiteFence(uint64_t timeout = UINT64_MAX);
	private:
		VkFence mFence{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };
	};
}
#endif

