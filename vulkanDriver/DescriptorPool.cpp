#ifndef DESCRIPTORPOOL_CPP
#define DESCRIPTORPOOL_CPP
#include "DescriptorPool.h"
namespace mainNameSpace::VulkanDriver {
	DescriptorPool::DescriptorPool(const Device::Ptr& device) {
		mDevice = device;
	}

	DescriptorPool::~DescriptorPool() {
		if (mPool != VK_NULL_HANDLE) {
			vkDestroyDescriptorPool(mDevice->getDevice(), mPool, nullptr);
		}
	}

	void DescriptorPool::build(std::vector<UniformParameter::Ptr>& params,const uint32_t maxSet) {
		//TODO
		poolSize = maxSet;
		//decriptor
		int uniformBufferCount = 0;
		int textureCount = 0;

		for (const auto& param : params) {
			if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) { uniformBufferCount++; }
			if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) { textureCount++; }
		}

		std::vector<VkDescriptorPoolSize> poolSizes{};
		
		if (uniformBufferCount > 0) {
			VkDescriptorPoolSize uniformBufferSize{};
			uniformBufferSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uniformBufferSize.descriptorCount = uniformBufferCount * maxSet;//3暂时是交换链的数量

			poolSizes.push_back(uniformBufferSize);
		}
		
		if (textureCount > 0) {
			VkDescriptorPoolSize textureSize{};
			textureSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			textureSize.descriptorCount = textureCount * maxSet;//3暂时是交换链的数量
			poolSizes.push_back(textureSize);
		}
		
		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		createInfo.pPoolSizes = poolSizes.data();
		createInfo.maxSets = static_cast<uint32_t>(maxSet);

		if (vkCreateDescriptorPool(mDevice->getDevice(), &createInfo, nullptr, &mPool) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create Descriptor pool");
		}
	}
}
#endif