#ifndef RENDERPASSCONTEXT_H
#define RENDERPASSCONTEXT_H

#include "../base.h"
#include "../vulkanDriver/RenderPass.h"
#include "../vulkanDriver/FrameBuffer.h"
#include "../vulkanDriver/SwapChain.h"
#include "../vulkanDriver/Device.h"
//记录所有渲染信息，调用所有的需求，开启和控制渲染流程
namespace mainNameSpace::VulkanDriverFactory {
	class RenderPassContext {
	public:
		VulkanDriver::RenderPass::Ptr static createBlitRenderPass(VulkanDriver::Device::Ptr mDevice, VulkanDriver::SwapChain::Ptr swapchain);
	public:
		RenderPassContext();
		~RenderPassContext();
	public:
		void setViewPort(float x, float y, float width, float height, float minDepth = 0, float maxDepth = 1.0);
		void setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
		void setClearColor(float r, float g, float b, float a, uint32_t index);
		void setClearDepthStencil(float depthClearValue, uint32_t stencilClearValue);
		void setRenderPass(VulkanDriver::RenderPass::Ptr renderPass);
	private:
		void resetClearValue();
		
		
	private:
		//clear Color
		std::vector<VkClearColorValue> clearColors;
		VkClearDepthStencilValue clearDepthStenceilValue{1.0,0};
		VkClearColorValue mulSamplerClearColors{ 1.0f,1.0f,1.0f,1.0f };
		std::vector< VkClearValue> clearValues{};
		//RenderPass
		VulkanDriver::RenderPass::Ptr mRenderPass;
		
		VkViewport viewport;
		VkRect2D scissor;
		
	};
}

#endif // ! BUFFERSTATE_H