#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "../base.h"
#include "Device.h"
namespace mainNameSpace::VulkanDriver {
	class Semaphore
	{
	public:
		using Ptr = std::shared_ptr<Semaphore>;
		static Ptr create(const Device::Ptr& device) { return std::make_shared<Semaphore>(device); }
		//Semaphore��һ��ͬ��ԭ����Ҫ���������GPU�ڵ�ͬ����������Queue�ڵ�ͬ��Ҳ������Queue֮�����ͬ����
		Semaphore(const Device::Ptr& device);

		~Semaphore();

		[[nodiscard]] auto getSemaphore() const { return mSemaphore; }

	private:
		VkSemaphore mSemaphore{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };
	};
}

#endif // !SEMAPHORE_H



