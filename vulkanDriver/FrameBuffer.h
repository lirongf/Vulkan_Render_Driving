#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "../base.h"
#include "RenderPass.h"
#include "Image.h"
#include "Device.h"
namespace mainNameSpace::VulkanDriver {

	class FrameBuffer
	{
	public:
		using Ptr = std::shared_ptr<FrameBuffer>;
		static Ptr create(uint32_t width,
			uint32_t height,
			std::vector<Image::Ptr> colorImages,
			Image::Ptr depthImage,
			bool mulSampler,
			VkSampleCountFlagBits samplerCout,
			Device::Ptr device) {
			return std::make_shared<FrameBuffer>(width, height, colorImages,depthImage, mulSampler, samplerCout,device);
		}
		FrameBuffer(uint32_t width,
			uint32_t height, 
			std::vector<Image::Ptr> colorImages, 
			Image::Ptr depthImage, 
			bool mulSampler,
			VkSampleCountFlagBits samplerCout,
			Device::Ptr device);
		~FrameBuffer();

		[[nodiscard]] VkSampleCountFlagBits getSamples() const {
			if (enableMulSampler)
				return  samplerConut;
			else
				return VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		}
		[[nodiscard]] VkFormat getDepthFormat() {
			if (depthStencilImage != nullptr)
				return depthStencilImage->getImageFormat();
			else
				return VkFormat::VK_FORMAT_UNDEFINED;
		};
		[[nodiscard]] VkFormat getColorAttachFormat(size_t index) {
			if (colorAttachs.size() <= index) {
				return VkFormat::VK_FORMAT_UNDEFINED;
			}
			else
				return colorAttachs[index]->getImageFormat();

		}

		VkFramebuffer getFrameBuffer(RenderPass::Ptr renderpass) {
			std::map<uint32_t, VkFramebuffer>::iterator it;
			uint32_t index = renderpass->getID();
			it = framebufferMap.find(index);
			
			if (it == framebufferMap.end()) {
				
				framebufferMap.insert(std::pair<uint32_t, VkFramebuffer>(index, createVkFrameBuffer(renderpass)));
				it = framebufferMap.find(index);
			}
			return it->second;
		}
		
	private:
		std::vector<Image::Ptr> colorAttachs{};
		
		std::vector<VkImageView> attachments;

		Image::Ptr depthStencilImage{nullptr};

		VkSampleCountFlagBits samplerConut{ VK_SAMPLE_COUNT_1_BIT};

		bool enableMulSampler = false;

		Image::Ptr mulSamplerImage{nullptr};

		uint32_t mWidth{0};

		uint32_t mHeight{0};

		std::map<uint32_t, VkFramebuffer> framebufferMap;

		VkFramebuffer createVkFrameBuffer(RenderPass::Ptr renderpass);

		Device::Ptr mdevice;
	};
}
#endif // !FRAMEBUFFER_H



