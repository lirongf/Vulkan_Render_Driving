#ifndef IMAGE_CPP
#define IMAGE_CPP
#include "Image.h"

namespace mainNameSpace::VulkanDriver {

	Image::Ptr Image::createDepthImage(
		const Device::Ptr& device,
		const int& width,
		const int& height,
		VkSampleCountFlagBits samples,
		VkFormat depthFormat
	) {
		if (depthFormat != VK_FORMAT_D32_SFLOAT && depthFormat != VK_FORMAT_D32_SFLOAT_S8_UINT && depthFormat != VK_FORMAT_D24_UNORM_S8_UINT) {
			throw std::runtime_error("Error : depth format error");
		}
		return Image::create(
			device,
			width, height,
			depthFormat,
			VK_IMAGE_TYPE_2D,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			samples,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT| VK_IMAGE_ASPECT_STENCIL_BIT
		);
	}

	Image::Ptr Image::createRenderTargetImage(
		const Device::Ptr& device,
		const int& width,
		const int& height,
		VkSampleCountFlagBits samplerFlag,
		VkFormat format
	) {
		return Image::create(
			device,
			width, height,
			format,
			VK_IMAGE_TYPE_2D,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			samplerFlag,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
	}

	Image::Image(const Device::Ptr& device,
		const int& width,
		const int& height,
		const VkFormat& format,
		const VkImageType& imageType,
		const VkImageTiling& tiling,
		const VkImageUsageFlags& usage,
		const VkSampleCountFlagBits& sample,
		const VkMemoryPropertyFlags properties,//memory
		const VkImageAspectFlags& aspectFlags) {
		mDevice = device;
		mLayout = VK_IMAGE_LAYOUT_UNDEFINED;//initial
		mWidth = width;
		mHeight = height;
		mFormat = format;
		mImageType = imageType;
		mImageAspectFlags = aspectFlags;
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.extent.width = width;
		imageCreateInfo.extent.height = height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.format = format;//rgb rgba
		imageCreateInfo.imageType = imageType;
		imageCreateInfo.tiling = tiling;
		imageCreateInfo.usage = usage;//color depth?
		imageCreateInfo.samples = sample;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.initialLayout = mLayout;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateImage(mDevice->getDevice(), &imageCreateInfo, nullptr, &mImage) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create image");
		}

		VkMemoryRequirements memReq{};
		vkGetImageMemoryRequirements(mDevice->getDevice(), mImage, &memReq);
		byteAllocateSize = memReq.size;
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReq.size;

		allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, properties);

		
		if (vkAllocateMemory(mDevice->getDevice(), &allocInfo, nullptr, &mImageMemory) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to allocate memory");
		}

		vkBindImageMemory(mDevice->getDevice(), mImage, mImageMemory, 0);

		createImagetView();

	}


	Image::Image(const Device::Ptr& device,
		const int& width,
		const int& height,
		const VkFormat& format,
		const VkImageType& imageType,
		const VkImageAspectFlags& aspectFlags,
		const VkImage &image) {
		mDevice = device;
		mLayout = VK_IMAGE_LAYOUT_UNDEFINED;//initial
		mWidth = width;
		mHeight = height;
		mFormat = format;
		mImageType = imageType;
		mImageAspectFlags = aspectFlags;
		mImage = image;
		createImagetView();
		customCreateImage = true;
	}

	void Image::createImagetView() {

		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = mImageType == VK_IMAGE_TYPE_2D ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_3D;
		imageViewCreateInfo.format = mFormat;
		imageViewCreateInfo.image = mImage;
		imageViewCreateInfo.subresourceRange.aspectMask = mImageAspectFlags;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(mDevice->getDevice(), &imageViewCreateInfo, nullptr, &mImageView) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create image view");
		}
	}

	Image::~Image() {
		if (mImageView != VK_NULL_HANDLE) {
			vkDestroyImageView(mDevice->getDevice(), mImageView, nullptr);
		}

		if (mImageMemory != VK_NULL_HANDLE) {
			vkFreeMemory(mDevice->getDevice(), mImageMemory, nullptr);
		}

		if (!customCreateImage&&( mImage != VK_NULL_HANDLE)) {
			vkDestroyImage(mDevice->getDevice(), mImage, nullptr);
		}
		mDevice.reset();
	}

	uint32_t Image::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProps;
		vkGetPhysicalDeviceMemoryProperties(mDevice->getPhysicalDevice(), &memProps);

		for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
			if ((typeFilter & (1 << i)) && ((memProps.memoryTypes[i].propertyFlags & properties) == properties)) {
				return i;
			}
		}

		throw std::runtime_error("Error: cannot find the property memory type!");
	}
}

#endif
