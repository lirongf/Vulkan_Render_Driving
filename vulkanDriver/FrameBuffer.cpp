#ifndef FRAMEBUFFER_CPP
#define FRAMEBUFFER_CPP

#include "FrameBuffer.h"
namespace mainNameSpace::VulkanDriver {
	FrameBuffer::FrameBuffer(uint32_t width,
		uint32_t height,
		std::vector<Image::Ptr> colorImages,
		Image::Ptr depthImage,
		bool mulSampler,
		VkSampleCountFlagBits samplerCout,
		Device::Ptr device) {
		mWidth = width;
		mHeight = height;
		enableMulSampler = mulSampler;
		samplerConut = samplerCout;
		colorAttachs = colorImages;
		mdevice = device;
		for (const Image::Ptr pp : colorImages) {
			attachments.push_back(pp->getImageView());
		}
		if (enableMulSampler) {
			mulSamplerImage = Image::create(device, width, height, colorImages[0]->getImageFormat(), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, samplerCout, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
			//setImageLayout TODO CommandBuffer


			attachments.push_back(mulSamplerImage->getImageView());
		}
		depthStencilImage = depthImage;
		if (depthStencilImage != nullptr) {
			VkImageSubresourceRange region{};
			region.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			region.baseMipLevel = 0;
			region.levelCount = 1;
			region.baseArrayLayer = 0;
			region.layerCount = 1;
			//setImageLayout TODO CommandBuffer

			attachments.push_back(depthStencilImage->getImageView());
		}
	}

	VkFramebuffer FrameBuffer::createVkFrameBuffer(RenderPass::Ptr renderpass) {
		VkFramebuffer frambuffer;
		VkFramebufferCreateInfo frameBufferCreateInfo{};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.renderPass = renderpass->getRenderPass();
		frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		frameBufferCreateInfo.pAttachments = attachments.data();
		frameBufferCreateInfo.width = mWidth;
		frameBufferCreateInfo.height = mHeight;
		frameBufferCreateInfo.layers = 1;
		if (vkCreateFramebuffer(mdevice->getDevice(), &frameBufferCreateInfo, nullptr, &frambuffer) != VK_SUCCESS) {
			throw std::runtime_error("Error:Failed to create frameBuffer");
		};

		return frambuffer;
		//mSwapChainFrameBuffers.resize(mImageCount);
		//for (int i = 0; i < mImageCount; ++i) {
	
		//	std::array<VkImageView, 3> attachments = {
		//		mSwapChainImageViews[i],
		//		mMutiSampleImages[i]->getImageView(),
		//		mDepthImages[i]->getImageView()
		//	};

		//	VkFramebufferCreateInfo frameBufferCreateInfo{};
		//	frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		//	frameBufferCreateInfo.renderPass = renderPass->getRenderPass();
		//	frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		//	frameBufferCreateInfo.pAttachments = attachments.data();
		//	frameBufferCreateInfo.width = mSwapChainExtent.width;
		//	frameBufferCreateInfo.height = mSwapChainExtent.height;
		//	frameBufferCreateInfo.layers = 1;

		//	if (vkCreateFramebuffer(mDevice->getDevice(), &frameBufferCreateInfo, nullptr, &mSwapChainFrameBuffers[i]) != VK_SUCCESS) {
		//		throw std::runtime_error("Error:Failed to create frameBuffer");
		//	}
		//}
	}

	FrameBuffer::~FrameBuffer() {
		std::cout << "remove FrameBuffer" << std::endl;
		for (Image::Ptr pp : colorAttachs) {
			pp.reset();
		}

		if (depthStencilImage != nullptr)
			depthStencilImage.reset();
		if (mulSamplerImage != nullptr)
			mulSamplerImage.reset();
		std::map<uint32_t, VkFramebuffer>::iterator it;
		for (it = framebufferMap.begin(); it != framebufferMap.end(); it++) {
			vkDestroyFramebuffer(mdevice->getDevice(),it->second , nullptr);
		}
		mdevice.reset();
		
	}
}

#endif 