//VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT = 0x00000001,
//// Provided by VK_VERSION_1_2
//VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT = 0x00000002,
//// Provided by VK_EXT_mutable_descriptor_type
//VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT = 0x00000004,
//// Provided by VK_NV_descriptor_pool_overallocation
//VK_DESCRIPTOR_POOL_CREATE_ALLOW_OVERALLOCATION_SETS_BIT_NV = 0x00000008,
//// Provided by VK_NV_descriptor_pool_overallocation
//VK_DESCRIPTOR_POOL_CREATE_ALLOW_OVERALLOCATION_POOLS_BIT_NV = 0x00000010,
//// Provided by VK_EXT_descriptor_indexing
//VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
//// Provided by VK_VALVE_mutable_descriptor_type
//VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE = VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT,
#ifndef DESCRIPTORPOOL_H
#define DESCRIPTORPOOL_H
#include "../base.h"
#include "Device.h"
#include "DescriptorSetLayout.h"
namespace mainNameSpace::VulkanDriver {
	class DescriptorPool
	{
	public:
		using Ptr = std::shared_ptr<DescriptorPool>;
		static Ptr create(const Device::Ptr& device) { return std::make_shared<DescriptorPool>(device); }
		DescriptorPool(const Device::Ptr& device);
		~DescriptorPool();
		void build(std::vector<UniformParameter::Ptr>& params, const uint32_t maxSet);
		[[nodiscard]] VkDescriptorPool getPool() const { return mPool; }
	private:
		VkDescriptorPool mPool{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };
		uint32_t poolSize{ 0 };
	};
}
#endif