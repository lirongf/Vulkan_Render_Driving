
#ifndef RENDERPASSCONTEXT_CPP
#define RENDERPASSCONTEXT_CPP

#include "RenderPassContext.h"

namespace mainNameSpace::VulkanDriverFactory {
	//创建一个根据交换链规则的RenderPass 默认打开MULS
	VulkanDriver::RenderPass::Ptr RenderPassContext::createBlitRenderPass(VulkanDriver::Device::Ptr mDevice,VulkanDriver::SwapChain::Ptr swapchain) {
		VulkanDriver::RenderPass::Ptr mRenderPass = VulkanDriver::RenderPass::create(mDevice);
		//color Attach
		VkAttachmentDescription finalAttachmentDes{};
		finalAttachmentDes.format = swapchain->getFormat();
		finalAttachmentDes.samples = VK_SAMPLE_COUNT_1_BIT;
		finalAttachmentDes.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		finalAttachmentDes.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		finalAttachmentDes.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		finalAttachmentDes.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		finalAttachmentDes.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		finalAttachmentDes.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		mRenderPass->addAttachment(finalAttachmentDes);
		//如果MulSampler
		VkAttachmentDescription MutiAttachmentDes{};
		MutiAttachmentDes.format = swapchain->getFormat();
		MutiAttachmentDes.samples = swapchain->getFrameBuffer(0)->getSamples();
		MutiAttachmentDes.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		MutiAttachmentDes.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		MutiAttachmentDes.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		MutiAttachmentDes.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		MutiAttachmentDes.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		MutiAttachmentDes.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		mRenderPass->addAttachment(MutiAttachmentDes);
		VkAttachmentDescription depthAttachmentDes{};
		depthAttachmentDes.format = swapchain->getFrameBuffer(0)->getDepthFormat();
		depthAttachmentDes.samples = swapchain->getFrameBuffer(0)->getSamples();//必须和Color一样
		depthAttachmentDes.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachmentDes.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentDes.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachmentDes.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentDes.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachmentDes.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		mRenderPass->addAttachment(depthAttachmentDes);
		//SubPass
		std::vector<VulkanDriver::SubPass> subPasss{};
		VulkanDriver::SubPass subPass{};
		subPass.addColorAttachmentReference(1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		subPass.setDepthStencilAttachmentReference(2);
		subPass.setResolveAttachmentReference(0);
		subPass.buildSubPassDescription();
		subPasss.push_back(subPass);
		mRenderPass->setSubPass(subPasss);
		//subPassDependency
		std::vector<VkSubpassDependency> dependecies{};
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependecies.push_back(dependency);
		mRenderPass->setDependency(dependecies);
		mRenderPass->buildRenderPass();
		return mRenderPass;
	}

	RenderPassContext::RenderPassContext(){

	}
	RenderPassContext::~RenderPassContext(){
		
	}
	void RenderPassContext::setViewPort(float x, float y, float width, float height, float minDepth, float maxDepth)
	{
		viewport.x = x;
		viewport.y = y;
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = minDepth;
		viewport.maxDepth = maxDepth;
	}
	void RenderPassContext::setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height)
	{
		scissor.offset.x = x;
		scissor.offset.y = y;
		scissor.extent.width = width;
		scissor.extent.height = height;
	}
	void RenderPassContext::setClearColor(float r, float g, float b, float a,uint32_t index)
	{
		if (clearColors.size()>index) {
			VkClearColorValue color;
			color.float32[0] = r;
			color.float32[1] = r;
			color.float32[2] = r;
			color.float32[3] = r;
			clearColors[index] = color;
		}

	}
	void RenderPassContext::setClearDepthStencil(float depthClearValue, uint32_t stencilClearValue)
	{
		clearDepthStenceilValue.depth = depthClearValue;
		clearDepthStenceilValue.stencil = stencilClearValue;
	}
	void RenderPassContext::setRenderPass(VulkanDriver::RenderPass::Ptr renderPass)
	{
		mRenderPass = renderPass;
	}
	void RenderPassContext::resetClearValue()
	{

	}
}
#endif // ! RENDERPASSCONTEXT_CPP
