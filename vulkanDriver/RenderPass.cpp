#ifndef RENDERPASS_CPP
#define RENDERPASS_CPP
#include "RenderPass.h"
namespace mainNameSpace::VulkanDriver {
	
	RenderPass::RenderPass(const Device::Ptr& device) {
		mDevice = device;
	}

	RenderPass::~RenderPass() {
		if (mRenderPass != VK_NULL_HANDLE) {
			vkDestroyRenderPass(mDevice->getDevice(), mRenderPass, nullptr);
		}
		mDevice.reset();
	}

	void RenderPass::setSubPass(std::vector<SubPass> &subpasses) {
		mSubPasses = subpasses;
	}
	void RenderPass::setDependency(std::vector<VkSubpassDependency> &subpassDependencies) {
		mDependencies = subpassDependencies;
	}

	void RenderPass::addAttachment(const VkAttachmentDescription attachmentDes) {
		mAttachmentDescriptions.push_back(attachmentDes);
	}

	void RenderPass::buildRenderPass() {
		std::vector<VkSubpassDescription> subpassDescriptions{};
		for (const SubPass& subpass : mSubPasses) {
			subpassDescriptions.push_back( subpass.getSubPassDescription());
		}
		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(mAttachmentDescriptions.size());
		createInfo.pAttachments = mAttachmentDescriptions.data();
		createInfo.subpassCount = static_cast<uint32_t>( mSubPasses.size());
		createInfo.pSubpasses = subpassDescriptions.data();
		createInfo.dependencyCount = static_cast<uint32_t>(mDependencies.size());
		createInfo.pDependencies = mDependencies.data();

		if (vkCreateRenderPass(mDevice->getDevice(), &createInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create renderPass");
		}
		//const void* pNext TODO
		//VkRenderPassCreateFlags  flags TODO指定渲染过程的附加属性的位掩码
	}

}
#endif
