#ifndef DESCRIPTORSET_H
#define DESCRIPTORSET_H
#include "Image.h"
#include "Buffer.h"
#include "../base.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
namespace mainNameSpace::VulkanDriver {
	class DescriptorSet
	{
	public:
		using Ptr = std::shared_ptr<DescriptorSet>;
		static Ptr create(
			const Device::Ptr& device,
			const std::vector<UniformParameter::Ptr> params,
			const DescriptorSetLayout::Ptr& layout,
			const DescriptorPool::Ptr& pool,
			int frameCount
		) {
			return std::make_shared<DescriptorSet>(
				device,
				params,
				layout,
				pool,
				frameCount
				);
		}
	public:
		DescriptorSet(const Device::Ptr& device,
			const std::vector<UniformParameter::Ptr> params,
			const DescriptorSetLayout::Ptr& layout,
			const DescriptorPool::Ptr& pool,
			int frameCount);
		~DescriptorSet();
		VkWriteDescriptorSet DescriptorSet::descriptorSetWritesBuffer(UniformParameter::Ptr param, VkDescriptorSet set, VkDescriptorBufferInfo& bufferInfo);
		VkWriteDescriptorSet DescriptorSet::descriptorSetWritesImage(UniformParameter::Ptr param, VkDescriptorSet set, VkDescriptorImageInfo& imageInfo);
		VkDescriptorSet getDescriptorSet(uint32_t index) { return mDescriptorSets[index]; }
		void updateDescriptorSets(std::vector<VkWriteDescriptorSet> descriptorSetWrites);
	private:
		std::vector<VkDescriptorSet> mDescriptorSets{};
		Device::Ptr mDevice{ nullptr };
	};
}
#endif // !DESCRIPTORSET



