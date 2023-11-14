#ifndef DEVICE_CPP
#define DEVICE_CPP

#include "Device.h"
//--------------PhysicsDevice---------------------
// Provided by VK_VERSION_1_0
//typedef enum VkPhysicalDeviceType {
//	VK_PHYSICAL_DEVICE_TYPE_OTHER = 0,设备与任何其他可用类型不匹配。
//	VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 1,设备通常嵌入主机或与主机紧密耦合。
//	VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 2,该设备通常是通过互连连接到主机的独立处理器。
//	VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3,设备通常是虚拟化环境中的虚拟节点。
//	VK_PHYSICAL_DEVICE_TYPE_CPU = 4,备通常在与主机相同的处理器上运行。
//} VkPhysicalDeviceType;

//----------------QueueFamilies---------------------
//typedef enum VkQueueFlagBits {
//VK_QUEUE_GRAPHICS_BIT = 0x00000001,
//VK_QUEUE_COMPUTE_BIT = 0x00000002,
//VK_QUEUE_TRANSFER_BIT = 0x00000004,
//VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
// Provided by VK_VERSION_1_1
//VK_QUEUE_PROTECTED_BIT = 0x00000010,
// Provided by VK_KHR_video_decode_queue
//VK_QUEUE_VIDEO_DECODE_BIT_KHR = 0x00000020,
// Provided by VK_KHR_video_encode_queue
//VK_QUEUE_VIDEO_ENCODE_BIT_KHR = 0x00000040,
// Provided by VK_NV_optical_flow
//VK_QUEUE_OPTICAL_FLOW_BIT_NV = 0x00000100,
//

namespace mainNameSpace::VulkanDriver {
	Device::Device(Instance::Ptr instance,Surface::Ptr surface) {
		mInstance = instance;
		mSurface = surface;
		pickPhysicalDevice();
		getQueueFamiliesInfo();
		createLogicDevice();
	}

	Device::~Device() {
		vkDestroyDevice(mDevice, nullptr);
		mSurface.reset();
		mInstance.reset();
	}

	void Device::pickPhysicalDevice() {

		//TODO  到时候换了
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, nullptr);
		
		if (deviceCount == 0) {
			throw std::runtime_error("VulkanError: no device");
		}
		std::vector<VkPhysicalDevice> AllDevices(deviceCount);
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, AllDevices.data());
		
		for (const auto& element : AllDevices) {
				
			if (checkPhysicalDevice(element)) {
				mPhysicsDevice = element;
				break;
			}
		}

		if (mPhysicsDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("deviceError: failed to get physical device");
		}
	}

	/*找到想要的Device*/
	bool Device::checkPhysicalDevice(const VkPhysicalDevice& physicalDevice) {
		//get Properties of Device
		//deviceType物理类型（独显 集中显卡等）
		//deviceName 物理设备名称
		//limits 物理限制
		//VkPhysicalDeviceSparseProperties（指定物理设备稀疏内存属性的结构）TODO
		//https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceSparseProperties.html
		VkPhysicalDeviceProperties  deviceProp;//deviceType deviceName limits( 所有limit) (neicun)
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProp);
		
		//get Features of Device
		//https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures.html
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

		//条件 找到独显  TODO
		if (deviceProp.deviceType== VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && deviceFeatures.samplerAnisotropy)
		{	
			return true;
		}
		return false;
	}

	void Device::getQueueFamiliesInfo() {
		//get all families
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicsDevice, &queueFamilyCount, nullptr);
	
		std::vector<VkQueueFamilyProperties> queueFamilies;
		queueFamilies.resize(queueFamilyCount);
		queueFamiliesProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicsDevice, &queueFamilyCount, queueFamilies.data());
		uint32_t index = 0;
		for (const auto& famaly : queueFamilies) {
			ST_DeviceQueueFamilyProperties st;
			st.index = index++;
			st.queueCount = famaly.queueCount;
			st.graphic = ((famaly.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0);
			st.compute = ((famaly.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0);
			st.transfer = ((famaly.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0);
			st.sparese_bind = ((famaly.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0);
			st.video_encode = ((famaly.queueFlags & VK_QUEUE_PROTECTED_BIT) != 0);
			queueFamiliesProperties[index - 1] = st;
		}
	}

	void Device::createLogicDevice() {
		
		//create Device Queue Createinfos
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		const float graphicQueueCreatePriority = 1.0;
		uint32_t graphicQueue = getGraphicQueueFamilyIdnex();
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = graphicQueue;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &graphicQueueCreatePriority;
		queueCreateInfos.push_back(queueCreateInfo);

		
		uint32_t pressentQueue = getPresentQueueFamilyIdnex();
		if (pressentQueue != graphicQueue) {
			const float presentQueueCreatePriority = 1.0;
			VkDeviceQueueCreateInfo pressentqueueCreateInfo = {};
			pressentqueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			pressentqueueCreateInfo.queueFamilyIndex = pressentQueue;
			pressentqueueCreateInfo.queueCount = 1;
			pressentqueueCreateInfo.pQueuePriorities = &presentQueueCreatePriority;//创建重要性
			queueCreateInfos.push_back(pressentqueueCreateInfo);
		}
		

		//VkPhysicalDeviceFeatures
		VkPhysicalDeviceFeatures deviceFeatures = {};
		getLogicDeviceFeatures(deviceFeatures);
		
		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		//extension
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceRequiredExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceRequiredExtensions.data();
		//validationLayer
	
			/*deviceCreateInfo.enabledLayerCount = 0;
			deviceCreateInfo.ppEnabledLayerNames = nullptr;*/
		if (vkCreateDevice(mPhysicsDevice, &deviceCreateInfo, nullptr, &mDevice) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create logical device");
		}
		////----create VkDevice----
		//// 
		////VkDeviceCreateInfo  TODO
		////queueIndex 先填0
		vkGetDeviceQueue(mDevice, getGraphicQueueFamilyIdnex(), 0, &mGraphicQueue);
		vkGetDeviceQueue(mDevice, getPresentQueueFamilyIdnex(), 0, &mPresentQueue);
	}

	uint32_t Device::getGraphicQueueFamilyIdnex() {
		for (const ST_DeviceQueueFamilyProperties &family : queueFamiliesProperties) {
			if (family.graphic) {
				return family.index;
			}
		}
		return -1;
	}

	uint32_t Device::getPresentQueueFamilyIdnex() {
		for (const ST_DeviceQueueFamilyProperties& family : queueFamiliesProperties) {
			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicsDevice, family.index, *(mSurface->getSurface()), &presentSupport);
			if (presentSupport == VK_TRUE) {
				return family.index;
			}
		}
		return -1;
	}

	void Device::getLogicDeviceFeatures(VkPhysicalDeviceFeatures &deviceFeatures) {
		deviceFeatures.samplerAnisotropy = VK_TRUE;
	}
}
#endif // !DEVICE_CPP
