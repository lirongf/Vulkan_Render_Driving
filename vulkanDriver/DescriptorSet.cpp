#ifndef DESCRIPTORSET_CPP
#define DESCRIPTORSET_CPP
#include "DescriptorSet.h"
namespace mainNameSpace::VulkanDriver {
	DescriptorSet::DescriptorSet(const Device::Ptr& device,
		const std::vector<UniformParameter::Ptr> params,
		const DescriptorSetLayout::Ptr& layout,
		const DescriptorPool::Ptr& pool,
		int frameCount) {
		mDevice = device;
		std::vector<VkDescriptorSetLayout> layouts(frameCount, layout->getLayout());
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool->getPool();
		allocInfo.descriptorSetCount = frameCount;
		allocInfo.pSetLayouts = layouts.data();
		mDescriptorSets.resize(frameCount);
		if (vkAllocateDescriptorSets(mDevice->getDevice(), &allocInfo, &mDescriptorSets[0]) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to allocate descriptor sets");
		}
		//for (int i = 0; i < frameCount; ++i) {
		//	std::vector<VkWriteDescriptorSet> descriptorSetWrites{};
		//	for (const auto& param : params) {
		//		VkWriteDescriptorSet descriptorSetWrite{};
		//		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//		descriptorSetWrite.dstSet = mDescriptorSets[i];
		//		descriptorSetWrite.dstArrayElement = 0;
		//		descriptorSetWrite.descriptorType = param->mDescriptorType;
		//		descriptorSetWrite.descriptorCount = param->mCount;
		//		descriptorSetWrite.dstBinding = param->mBinding;

		//		if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		//			//descriptorSetWrite.pBufferInfo = &param->mBuffers[i]->getBufferInfo();
		//		}

		//		if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
		//			//descriptorSetWrite.pImageInfo = &param->mTexture->getImageInfo();
		//		}

		//		descriptorSetWrites.push_back(descriptorSetWrite);
		//	}

		//	vkUpdateDescriptorSets(mDevice->getDevice(), static_cast<uint32_t>(descriptorSetWrites.size()), descriptorSetWrites.data(), 0, nullptr);
		//}
	}


	void DescriptorSet::updateDescriptorSets(std::vector<VkWriteDescriptorSet> descriptorSetWrites) {
		vkUpdateDescriptorSets(mDevice->getDevice(), static_cast<uint32_t>(descriptorSetWrites.size()), descriptorSetWrites.data(), 0, nullptr);
	}


	VkWriteDescriptorSet DescriptorSet::descriptorSetWritesBuffer(UniformParameter::Ptr param,VkDescriptorSet set,VkDescriptorBufferInfo &bufferInfo ) {
		VkWriteDescriptorSet descriptorSetWrite{};
		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrite.dstSet = set;
		descriptorSetWrite.dstArrayElement = 0;
		descriptorSetWrite.descriptorType = param->mDescriptorType;
		descriptorSetWrite.descriptorCount = param->mCount;
		descriptorSetWrite.dstBinding = param->mBinding;

		if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
			descriptorSetWrite.pBufferInfo = &bufferInfo;
		}

		if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
			//descriptorSetWrite.pImageInfo = &param->mTexture->getImageInfo();
		}
		return descriptorSetWrite;
	}
	VkWriteDescriptorSet DescriptorSet::descriptorSetWritesImage(UniformParameter::Ptr param, VkDescriptorSet set, VkDescriptorImageInfo& imageInfo) {
		VkWriteDescriptorSet descriptorSetWrite{};
		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrite.dstSet = set;
		descriptorSetWrite.dstArrayElement = 0;
		descriptorSetWrite.descriptorType = param->mDescriptorType;
		descriptorSetWrite.descriptorCount = param->mCount;
		descriptorSetWrite.dstBinding = param->mBinding;

		if (param->mDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
			descriptorSetWrite.pImageInfo = &imageInfo;
		}

		return descriptorSetWrite;
	}


	DescriptorSet::~DescriptorSet() {
		//TODO 
		//VkDescriptorSet Destroy
		mDevice.reset();
	}
}
#endif // !DESCRIPTORSET

