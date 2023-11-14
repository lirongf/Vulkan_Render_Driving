#ifndef COMMANDPOOL_H
#define COMMANDPOOL_H
#include "../base.h"
#include "Device.h"
namespace mainNameSpace::VulkanDriver {
	class CommandPool
	{
	public:
		using Ptr = std::shared_ptr<CommandPool>;
		static Ptr create(const Device::Ptr& device, VkCommandPoolCreateFlagBits flag = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {
			return std::make_shared<CommandPool>(device, flag);
		}

		CommandPool(const Device::Ptr& device, VkCommandPoolCreateFlagBits flag = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		~CommandPool();

		[[nodiscard]] auto getCommandPool() const { return mCommandPool; }

	private:
		VkCommandPool mCommandPool{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };
	};
}

#endif // !COMMANDPOOL


