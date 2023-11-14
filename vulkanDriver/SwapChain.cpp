#ifndef SWAPCHAIN_CPP
#define SWAPCHAIN_CPP

#include "SwapChain.h"
//
//typedef enum VkPresentModeKHR {
//	VK_PRESENT_MODE_IMMEDIATE_KHR = 0,Ӧ�ó����ύ��ͼ��ᱻ�������䵽��Ļ�ϣ����ܻᵼ��˺������
//	VK_PRESENT_MODE_MAILBOX_KHR = 1,��һģʽ�ǵڶ���ģʽ����һ�����֡��������ڽ������Ķ�����ʱ����Ӧ�ó��򣬶����е�ͼ��ᱻֱ���滻ΪӦ�ó������ύ��ͼ����һģʽ��������ʵ���������壬����˺�������ͬʱ��С���ӳ����⡣
//	VK_PRESENT_MODE_FIFO_KHR = 2,���������һ���Ƚ��ȳ��Ķ��У�ÿ�δӶ���ͷ��ȡ��һ��ͼ�������ʾ��Ӧ�ó�����Ⱦ��ͼ���ύ���������󣬻ᱻ���ڶ���β����������Ϊ��ʱ��Ӧ�ó�����Ҫ���еȴ�����һģʽ�ǳ��������ڳ��õĴ�ֱͬ����ˢ����ʾ��ʱ��Ҳ��������ֱ��ɨ��
//	VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3,��һģʽ����һģʽ��Ψһ�����ǣ����Ӧ�ó����ӳ٣����½������Ķ�������һ�δ�ֱ��ɨʱΪ�գ���ô�����Ӧ�ó�������һ�δ�ֱ��ɨǰ�ύͼ��ͼ�����������ʾ����һģʽ���ܻᵼ��˺������
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
		//����֧�ֵ�����
		initSwapchainSurport();
		//֧�ֵ�SurfaceFormat�����ʽ
		getSurfaceFormat();
		//�����ύģʽ
		getPressentMode();
		//����swapchain
		setSwapChaineExtent(width, height);
		createSwapchain();
		//������ͼƬ����
		createSwapchainImages();
		//����swapchain��Image
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
		//��ѡ SRGB�ռ� VK_FORMAT_B8G8R8A8_SRGB  VK_COLOR_SPACE_SRGB_NONLINEAR_KHR 
		//���δָ��,ָ��srgb�ռ�͸�ʽ
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
		//��ѡVK_PRESENT_MODE_MAILBOX_KHR����ѡVK_PRESENT_MODE_FIFO_KHR����û�� ѡ�����ύ
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
		//�����VR���ܻ��ֺö��
		createInfo.imageArrayLayers = 1;
		//ָ������ͼ���Ͻ��������Ĳ�������ͼ����Ϊһ����ɫ������ʹ��VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT�����������Ҫ��ͼ����к��ڴ���֮��Ĳ���������ʹ��VK_IMAGE_USAGE_TRANSFER_DST_BIT
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		uint32_t graphicIndex = mdevice->getGraphicQueueFamily();
		uint32_t presentIndex = mdevice->getPresentQueueFamily();
		if (graphicIndex == presentIndex) {
			//һ��ͼ��ͬһʱ��ֻ�ܱ�һ����������ӵ�У�����һ������ʹ����֮ǰ��������ʽ�ظı�ͼ������Ȩ����һģʽ�����ܱ������
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}
		else {
			std::vector<uint32_t> queueFamilies = { graphicIndex ,  presentIndex };
			//ͼ������ڶ���������ʹ�ã�����Ҫ��ʽ�ظı�ͼ������Ȩ��
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilies.size());
			createInfo.pQueueFamilyIndices = queueFamilies.data();
		}
		//ͼ���Ƿ�ת�� ����Ĭ��ֵ
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


