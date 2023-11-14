#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "../base.h"
#include "Device.h"
#include "Surface.h"
#include "FrameBuffer.h"
#include "MemoryUtil.h"

namespace mainNameSpace::VulkanDriver {
	
	struct SwapChainSupportInfo {
		VkSurfaceCapabilitiesKHR mCapabilities;
		std::vector<VkSurfaceFormatKHR> mFormats;
		std::vector<VkPresentModeKHR> mPresentModes;
	};

	struct SwapchainImageInfo
	{
		VkImage mImage{VK_NULL_HANDLE};
		VkImageView mImageViews{ VK_NULL_HANDLE };
		VkFramebuffer mFrameBuffers{ VK_NULL_HANDLE };
	};

	class SwapChain
	{
	public:
		
		using Ptr = std::shared_ptr<SwapChain>;
		static Ptr create(Device::Ptr device, Surface::Ptr surface, uint32_t width, uint32_t height) { return  std::make_shared<SwapChain>(device, surface,height,width); }
	
		SwapChain(Device::Ptr device, Surface::Ptr surface, uint32_t width, uint32_t height);
		~SwapChain();
		[[nodiscard]] VkFormat getFormat() const { return surfaceFormat.format;}
		[[nodiscard]] FrameBuffer::Ptr getFrameBuffer(uint32_t index) {
			return mSwapChainFrameBuffers
				[index];
		}
		[[nodiscard]] uint32_t getImageCount() const { return mImageCount; }
		[[nodiscard]] VkExtent2D getExtent() const { return mSwapChainExtent; }
		[[nodiscard]] VkSwapchainKHR getSwapChain() const { return mswapchain; }
		void createSwapchainImages();

	private:
		void initSwapchainSurport();
		void getSurfaceFormat();
		void getPressentMode();
		void createSwapchain();
		void setSwapChaineExtent(uint32_t width, uint32_t height);
		
		void createSwapChaineImage(const VkImage& image);
		VkImageView createImageView(const VkImage& image);
		
	private:
		Device::Ptr mdevice{ nullptr };
		Surface::Ptr msurface{ nullptr };
		VkSwapchainKHR mswapchain{ VK_NULL_HANDLE };

		SwapChainSupportInfo swapchainSurportInfo{};

		VkSurfaceFormatKHR surfaceFormat{};
		VkPresentModeKHR surfacePresentMode;
		uint32_t mImageCount{ 0 };
		VkExtent2D mSwapChainExtent;
		
		//swapChain Image
		std::vector<Image::Ptr> mSwapChainImages{};
		//std::vector<VkImageView> mSwapChainImageViews{};
		std::vector<FrameBuffer::Ptr> mSwapChainFrameBuffers{};
	};
}
#endif //!SWAPCHAIN_H


