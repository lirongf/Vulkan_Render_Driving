#ifndef DEVICE_H
#define DEVICE_H

#include "../base.h"
#include "Instance.h"
#include "Surface.h"

namespace mainNameSpace::VulkanDriver {

	//开启的扩展
	//VK_KHR_SWAPCHAIN_EXTENSION_NAME,swapchain开启
	const std::vector<const char*> deviceRequiredExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_MAINTENANCE1_EXTENSION_NAME
	};

	//描述
	struct ST_DeviceQueueFamilyProperties
	{	
		bool graphic;
		bool compute;
		bool transfer;
		bool sparese_bind;
		bool video_encode;
		uint32_t index;
		uint32_t queueCount;
	};
	class Device {
	public:
		using Ptr = std::shared_ptr<Device>;
		static Ptr create(Instance::Ptr instance, Surface::Ptr surface) {
			return std::make_shared<Device>(instance, surface);
		}
		Device(Instance::Ptr instance, Surface::Ptr surface);
		~Device();
		[[nodiscard]] VkDevice getDevice() const { return mDevice; }
		[[nodiscard]] VkPhysicalDevice getPhysicalDevice() const { return mPhysicsDevice; }

		[[nodiscard]] uint32_t getGraphicQueueFamily() { return getGraphicQueueFamilyIdnex(); }
		[[nodiscard]] uint32_t getPresentQueueFamily() { return getPresentQueueFamilyIdnex(); }

		[[nodiscard]] auto getGraphicQueue() const { return mGraphicQueue; }
		[[nodiscard]] auto getPresentQueue() const { return mPresentQueue; }


	private:
		void pickPhysicalDevice();

		void getQueueFamiliesInfo();

		void createLogicDevice();

		bool checkPhysicalDevice(const VkPhysicalDevice& physicalDevice);

		uint32_t getGraphicQueueFamilyIdnex();

		uint32_t getPresentQueueFamilyIdnex();

		void getLogicDeviceFeatures(VkPhysicalDeviceFeatures& deviceFeatures);
	private:

		Instance::Ptr mInstance{ nullptr };
		Surface::Ptr mSurface{ nullptr };

		VkDevice mDevice{ VK_NULL_HANDLE };
		VkPhysicalDevice mPhysicsDevice{ VK_NULL_HANDLE };
		VkQueue	mGraphicQueue{ VK_NULL_HANDLE };
		VkQueue mPresentQueue{ VK_NULL_HANDLE };

		std::vector<ST_DeviceQueueFamilyProperties> queueFamiliesProperties;
	};
}
#endif // !DEVICE_H
