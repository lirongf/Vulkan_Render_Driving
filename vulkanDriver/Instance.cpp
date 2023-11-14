#ifndef  INSTANCE_CPP
#define INSTANCE_CPP
/*
Vulkan提供的层
VK_LAYER_NV_optimus
VK_LAYER_RENDERDOC_Capture
VK_LAYER_LUNARG_api_dump
VK_LAYER_LUNARG_gfxreconstruct
VK_LAYER_KHRONOS_synchronization2
VK_LAYER_KHRONOS_validation  为了最小化驱动程序的开销，vulkan默认情况下几乎不检测API的错误调用，可以手动增加验证层（validation layers）添加错误检测步骤
VK_LAYER_LUNARG_monitor
VK_LAYER_LUNARG_screenshot
VK_LAYER_KHRONOS_profiles
*/

/*
* VK_LAYER_KHRONOS_validation
* 你可以在debug构建中开启验证层，并在release构建中关闭验证层以达到更好的性能。Vulkan没有内置的验证层，但是Vulkan SDK提供了一些用于检查一般错误的验证层，它们被集成到名为VK_LAYER_KHRONOS_validation的验证层中。
* 由于所有的验证层都需要额外安装，如果没有vulkan SDK，就无法开启VK_LAYER_KHRONOS_validation，因此，我们最好检查一下请求的验证层是否能被支持。
*/

#include "Instance.h"

#define DebugValidationLayer "VK_LAYER_KHRONOS_validation"

namespace mainNameSpace::VulkanDriver {
	//global
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pMessageData,
		void* pUserData) {
		std::cout << "ValidationLayer: " << pMessageData->pMessage << std::endl;

		return VK_FALSE;
	}

	//加载创建 DebugUtilMessage 方法
	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* debugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, debugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	//加载删除 DebugUtilMessage 方法
	static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT  debugMessenger,
		const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

		if (func != nullptr) {
			return func(instance, debugMessenger, pAllocator);
		}
	}

	Instance::Instance(bool enableValidationLayer, std::vector<const char*>& instanceExtensions) {
		mEnableValidationLayer = enableValidationLayer;
		if (enableValidationLayer) {
			if (!checkOneValidationLayerSupport(DebugValidationLayer)) {
				throw std::runtime_error("Error: validation layer is not supported");//不支持验证层
				mEnableValidationLayer = false;
			}
			else
			{
				validationLayers.push_back(DebugValidationLayer);
			}
		}

		//create instance
		setRequiredInstanceExtensions(instanceExtensions);
		VkApplicationInfo appInfo = getApplicationInfo();
		createInstanceCreateInfo(appInfo);

		//设置验证层的调试CallBack
		setupDebugger();
	}

	Instance::~Instance() {
		if (mEnableValidationLayer) {
			DestroyDebugUtilsMessengerEXT(vkInstance, mDebugger, nullptr);
		}

		vkDestroyInstance(vkInstance, nullptr);
	}

	//private
	VkApplicationInfo Instance::getApplicationInfo() {
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "vulkanApp";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "LayaNative";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;//VK_API_VERSION_1_1
		return appInfo;
	}
	//private
	void Instance::setRequiredInstanceExtensions(std::vector<const char*> &instanceExtensions) {
		extensions = instanceExtensions;
		if (mEnableValidationLayer) {
			/*在开启验证层时，必须同时开启VK_EXT_DEBUG_UTILS_EXTENSION_NAME扩展
			或者VK_EXT_DEBUG_REPORT_EXTENSION_NAME扩展，
			DEBUG_UTILS扩展比DEBUG_REPORT扩展功能更强大,优先使用。
			*/
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		
	}
	//private
	void Instance::createInstanceCreateInfo(VkApplicationInfo &applicationInfo) {
		VkInstanceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pApplicationInfo = &applicationInfo;
		
		//ValidationLayer
		if (mEnableValidationLayer) {
			info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			info.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			info.enabledLayerCount = 0;
			info.ppEnabledLayerNames = nullptr;
		}

		info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		info.ppEnabledExtensionNames = extensions.data();

		if (vkCreateInstance(&info, nullptr, &vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create instance");
		}
		else
			std::cout << "create VKInstance Success" << std::endl;
		return;
	}

	//找到所有支持的层
	void Instance::printValidationLayerSupport() {
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> vavailableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, vavailableLayers.data());
		for (const auto& layerName : vavailableLayers) {
			std::cout << layerName.layerName << std::endl;
		}
	}
	
	//检测某个扩展是否支持
	bool Instance::checkOneValidationLayerSupport(const char* ValidationLayerName) {
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> vavailableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, vavailableLayers.data());
		bool supportFlag = false;
		for (const auto& layerName : vavailableLayers) {
			if (std::strcmp(layerName.layerName, ValidationLayerName) == 0) {
				supportFlag = true;
				break;
			}
		}
		return supportFlag;
	}
	
	//找到所有支持的扩展
	void Instance::printAvailableExtensions() {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());
		std::cout << "Available extensions:" << std::endl;
		for (const auto& extension : extensionProperties) {
			std::cout << extension.extensionName << std::endl;
		}
	}

	//检测某个扩展是否支持
	bool Instance::checkOneExtensionAvailable(const char* extensionName) {
		uint32_t extensionCount = 0;
		std::vector<VkExtensionProperties> extensionProperties(1);
		vkEnumerateInstanceExtensionProperties(extensionName, &extensionCount, extensionProperties.data());
		return extensionProperties[0].specVersion!=0;
	}

	void Instance::setupDebugger() {
		if (!mEnableValidationLayer) { return; }

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = debugCallBack;
		createInfo.pUserData = nullptr;

		if (CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &mDebugger) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create debugger");
		}
	}
}
#endif // ! INSTANCE_CPP


