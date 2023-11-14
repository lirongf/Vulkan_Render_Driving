#ifndef SWAPCHAIN_CPP
#define SWAPCHAIN_CPP

#include "SwapChain.h"
//
//typedef enum VkPresentModeKHR {
//	VK_PRESENT_MODE_IMMEDIATE_KHR = 0,应用程序提交的图像会被立即传输到屏幕上，可能会导致撕裂现象。
//	VK_PRESENT_MODE_MAILBOX_KHR = 1,这一模式是第二种模式的另一个变种。它不会在交换链的队列满时阻塞应用程序，队列中的图像会被直接替换为应用程序新提交的图像。这一模式可以用来实现三倍缓冲，避免撕裂现象的同时减小了延迟问题。
//	VK_PRESENT_MODE_FIFO_KHR = 2,交换链变成一个先进先出的队列，每次从队列头部取出一张图像进行显示，应用程序渲染的图像提交给交换链后，会被放在队列尾部。当队列为满时，应用程序需要进行等待。这一模式非常类似现在常用的垂直同步。刷新显示的时刻也被叫做垂直回扫。
//	VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3,这一模式和上一模式的唯一区别是，如果应用程序延迟，导致交换链的队列在上一次垂直回扫时为空，那么，如果应用程序在下一次垂直回扫前提交图像，图像会立即被显示。这一模式可能会导致撕裂现象。
//	VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR = 1000111000,
//	VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR = 1000111001,
//} VkPresentModeKHR;

namespace mainNameSpace::VulkanDriver {
	static VkImageView createImageView(Device::Ptr mDevice,VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;

		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		
		VkImageView imageView{ VK_NULL_HANDLE };
		if (vkCreateImageView(mDevice->getDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create image view in swapchain");
		}
		
		return imageView;
	}

	SwapChain::SwapChain(Device::Ptr device, Surface::Ptr surface,uint32_t width,uint32_t height) {
		mdevice = device;
		msurface = surface;
		//查找支持的特性
		initSwapchainSurport();
		//支持的SurfaceFormat纹理格式
		getSurfaceFormat();
		//设置提交模式
		getPressentMode();
		//创建swapchain
		setSwapChaineExtent(width, height);
		createSwapchain();
		//交换链图片数量
		createSwapchainImages();
		//创建swapchain的Image
	}

	void SwapChain::initSwapchainSurport() {
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mdevice->getPhysicalDevice(), *(msurface->getSurface()), &swapchainSurportInfo.mCapabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(mdevice->getPhysicalDevice(), *(msurface->getSurface()), &formatCount, nullptr);

		if (formatCount != 0) {
			swapchainSurportInfo.mFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(mdevice->getPhysicalDevice(), *(msurface->getSurface()), &formatCount, swapchainSurportInfo.mFormats.data());
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(mdevice->getPhysicalDevice(), *(msurface->getSurface()), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			swapchainSurportInfo.mPresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(mdevice->getPhysicalDevice(), *(msurface->getSurface()), &presentModeCount, swapchainSurportInfo.mPresentModes.data());
		}
	}

	void SwapChain::getSurfaceFormat() {
		//首选 SRGB空间 VK_FORMAT_B8G8R8A8_SRGB  VK_COLOR_SPACE_SRGB_NONLINEAR_KHR 
		//如果未指定,指定srgb空间和格式
		if (swapchainSurportInfo.mFormats.size() == 1 && swapchainSurportInfo.mFormats[0].format == VK_FORMAT_UNDEFINED) {
			surfaceFormat.format = VK_FORMAT_R8G8B8A8_SRGB;
			surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			return;
		}

		for (const VkSurfaceFormatKHR& surfaceFormatOne : swapchainSurportInfo.mFormats) {
			if (surfaceFormatOne.format == VK_FORMAT_B8G8R8A8_SRGB &&
				surfaceFormatOne.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				surfaceFormat = surfaceFormatOne;
				return;
			}
		}
		surfaceFormat = swapchainSurportInfo.mFormats[0];
		 return;
	}

	void SwapChain::getPressentMode() {
		//首选VK_PRESENT_MODE_MAILBOX_KHR，次选VK_PRESENT_MODE_FIFO_KHR，都没有 选立即提交
		surfacePresentMode = VK_PRESENT_MODE_FIFO_KHR;
		for (const VkPresentModeKHR& presentMode : swapchainSurportInfo.mPresentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				surfacePresentMode = presentMode;
				return;
			}
		}

		for (const VkPresentModeKHR& presentMode : swapchainSurportInfo.mPresentModes) {
			if (presentMode == VK_PRESENT_MODE_FIFO_KHR) {
				surfacePresentMode = presentMode;
				return;
			}
		}
		surfacePresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		return;
	}

	void SwapChain::createSwapchain() {
		mImageCount = swapchainSurportInfo.mCapabilities.minImageCount + 1;
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = *msurface->getSurface();
		createInfo.minImageCount = mImageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = mSwapChainExtent;
		//如果是VR可能会又好多层
		createInfo.imageArrayLayers = 1;
		//指定将在图像上进行怎样的操作。将图像作为一个颜色附着来使用VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT。如果读者需要对图像进行后期处理之类的操作，可以使用VK_IMAGE_USAGE_TRANSFER_DST_BIT
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		uint32_t graphicIndex = mdevice->getGraphicQueueFamily();
		uint32_t presentIndex = mdevice->getPresentQueueFamily();
		if (graphicIndex == presentIndex) {
			//一张图像同一时间只能被一个队列族所拥有，在另一队列族使用它之前，必须显式地改变图像所有权。这一模式下性能表现最佳
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}
		else {
			std::vector<uint32_t> queueFamilies = { graphicIndex ,  presentIndex };
			//图像可以在多个队列族间使用，不需要显式地改变图像所有权。
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilies.size());
			createInfo.pQueueFamilyIndices = queueFamilies.data();
		}
		//图像是否转换 采用默认值
		createInfo.preTransform = swapchainSurportInfo.mCapabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = surfacePresentMode;
		createInfo.clipped = VK_TRUE;
		if (vkCreateSwapchainKHR(mdevice->getDevice(), &createInfo, nullptr, &mswapchain)) {
			throw std::runtime_error("Error: failed to create swapChain");
		}
	}

	void SwapChain::createSwapchainImages() {
		std::vector<VkImage> images{};
		vkGetSwapchainImagesKHR(mdevice->getDevice(), mswapchain, &mImageCount, nullptr);
		images.resize(mImageCount);
		vkGetSwapchainImagesKHR(mdevice->getDevice(), mswapchain, &mImageCount, images.data());

		//mSwapChainImages.resize(mImageCount);
		//mSwapChainFrameBuffers.resize(mImageCount);

		size_t index = 0;
		VkImageSubresourceRange region{};
		region.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT| VK_IMAGE_ASPECT_STENCIL_BIT;
		region.baseMipLevel = 0;
		region.levelCount = 1;
		region.baseArrayLayer = 0;
		region.layerCount = 1;

		for (const VkImage& ima : images) {
			Image::Ptr imag = Image::create(mdevice, mSwapChainExtent.width, mSwapChainExtent.height, getFormat(), VkImageType::VK_IMAGE_TYPE_2D, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, ima);
		
			mSwapChainImages.push_back(imag);
			std::vector<Image::Ptr> colorImages;
			colorImages.push_back(imag);
			//TODO depth
			Image::Ptr depthImage = Image::createDepthImage(
				mdevice,
				mSwapChainExtent.width,
				mSwapChainExtent.height,
				VK_SAMPLE_COUNT_4_BIT,
				VkFormat::VK_FORMAT_D24_UNORM_S8_UINT
			);

			MemoryUtil::setImageLayout(mdevice, depthImage, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, region, nullptr);
		
			FrameBuffer::Ptr framBuffer = FrameBuffer::create(mSwapChainExtent.width, mSwapChainExtent.height, colorImages, depthImage, true, VK_SAMPLE_COUNT_4_BIT, mdevice);
			mSwapChainFrameBuffers.push_back(framBuffer);
			index++;
		}
	}

	void SwapChain::setSwapChaineExtent(uint32_t width, uint32_t height) {
		mSwapChainExtent.width = std::max(swapchainSurportInfo.mCapabilities.minImageExtent.width,
			std::min(swapchainSurportInfo.mCapabilities.maxImageExtent.width, width));
		mSwapChainExtent.height = std::max(swapchainSurportInfo.mCapabilities.minImageExtent.height,
			std::min(swapchainSurportInfo.mCapabilities.maxImageExtent.height, height));
	}

	SwapChain::~SwapChain() {
		mSwapChainFrameBuffers.clear();
		

		if (mswapchain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(mdevice->getDevice(), mswapchain, nullptr);
		}
		mdevice.reset();
		msurface.reset();
	}
}
#endif //!SWAPCHAIN_CPP


