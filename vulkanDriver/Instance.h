#ifndef INSTANCE_H
#define INSTANCE_H

#include "../base.h"

namespace mainNameSpace::VulkanDriver {
	class Instance
	{
	public:
		using Ptr = std::shared_ptr<Instance>;

		static Ptr create(bool enableValidationLayer, std::vector<const char*>& instanceExtensions) {
			return std::make_shared<Instance>(enableValidationLayer, instanceExtensions);
		}

		Instance(bool enableValidationLayer, std::vector<const char*>& instanceExtensions);
		
		~Instance();
		
		bool checkOneExtensionAvailable(const char* extensionName);
		
		bool checkOneValidationLayerSupport(const char* ValidationLayerName);

		[[nodiscard]] VkInstance getInstance() const { return vkInstance; }

		[[nodiscard]] std::vector<const char*> getValidationlayers() const {return validationLayers;}

		
	public:
		//是否开启验证层
		bool mEnableValidationLayer{ false };

		VkInstance vkInstance{ VK_NULL_HANDLE };

		VkDebugUtilsMessengerEXT mDebugger{ VK_NULL_HANDLE };
	private:
		
		VkApplicationInfo getApplicationInfo();
		
		void createInstanceCreateInfo(VkApplicationInfo& applicationInfo);

		void setRequiredInstanceExtensions(std::vector<const char*>& instanceExtensions);
	private:
		std::vector<const char*> validationLayers;

		std::vector<const char*> extensions;

		void setupDebugger();
	public://help Function
		//打印所有的扩展
		void printAvailableExtensions();

		void printValidationLayerSupport();
	
	};

}
#endif //!INSTANCE_H

