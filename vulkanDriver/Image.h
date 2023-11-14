#ifndef IMAGE_H
#define IMAGE_H

#include "../base.h"
#include "Device.h"

namespace mainNameSpace::VulkanDriver {
	/*std::vector<VkFormat> DepthFormats = {
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
	};*/
	class Image
	{
	public:
		using Ptr = std::shared_ptr<Image>;
		static Ptr create(const Device::Ptr& device,
			const int& width,
			const int& height,
			const VkFormat& format,
			const VkImageType& imageType,
			const VkImageTiling& tiling,
			const VkImageUsageFlags& usage,
			const VkSampleCountFlagBits& sample,
			const VkMemoryPropertyFlags properties,//memory
			const VkImageAspectFlags& aspectFlags//view
		) {
			return std::make_shared<Image>(device, width, height, format, imageType, tiling, usage, sample, properties, aspectFlags);
		};

		static Ptr create(const Device::Ptr& device,
			const int& width,
			const int& height,
			const VkFormat& format,
			const VkImageType& imageType,
			const VkImageAspectFlags& aspectFlags,
			const VkImage& image
		) {
			return std::make_shared<Image>(device, width, height, format, imageType, aspectFlags,image);
		};
		static Ptr createRenderTargetImage(
			const Device::Ptr& device,
			const int& width,
			const int& height,
			VkSampleCountFlagBits samplerFlag,
			VkFormat format
		);

		static Ptr createDepthImage(
			const Device::Ptr& device,
			const int& width,
			const int& height,
			VkSampleCountFlagBits samples,
			VkFormat depthFormat
		);

	public:
		Image(const Device::Ptr& device,
			const int& width,
			const int& height,
			const VkFormat& format,
			const VkImageType& imageType,
			const VkImageTiling& tiling,
			const VkImageUsageFlags& usage,
			const VkSampleCountFlagBits& sample,
			const VkMemoryPropertyFlags properties,//memory
			const VkImageAspectFlags& aspectFlags//view;
		);

		//set Image to Class,only swapchain
		Image(const Device::Ptr& device,
			const int& width,
			const int& height,
			const VkFormat& format,
			const VkImageType& imageType,
			const VkImageAspectFlags& aspectFlags,
			const VkImage& image
		);

		~Image();

		//void setImageLayout(
		//	VkImageLayout newLayout,
		//	VkPipelineStageFlags srcStageMask,
		//	VkPipelineStageFlags dstStageMask,
		//	VkImageSubresourceRange subresrouceRange,
		//	//const CommandPool::Ptr& commandPool
		//	);
		//void fillImageData(size_t size, void* pData, const CommandPool::Ptr& commandPool);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		

		[[nodiscard]] VkImage getImage() const { return mImage; }

		[[nodiscard]] VkImageLayout getLayout() const { return mLayout; }
		void setLayout(VkImageLayout newLayout){
			mLayout = newLayout;
			return; 
		}

		

		[[nodiscard]] uint32_t getWidth() const { return mWidth; }

		[[nodiscard]] uint32_t getHeight() const { return mHeight; }

		[[nodiscard]] VkImageView getImageView() const { return mImageView; }

		void setImage(VkImage &image){
			if (image != VK_NULL_HANDLE)
				throw std::runtime_error("Error: failed to set image");
			mImage = image;
		}

		[[nodiscard]] VkFormat getImageFormat() const { return mFormat; }
	private:
		 void createImagetView();
	private:
		Device::Ptr mDevice{ nullptr };
		uint32_t mWidth{ 0 };
		uint32_t mHeight{ 0 };
		VkImage mImage{ VK_NULL_HANDLE };
		VkDeviceMemory mImageMemory{ VK_NULL_HANDLE };
		VkImageView mImageView{ VK_NULL_HANDLE };
		VkFormat mFormat;
		VkImageType mImageType;
		VkImageAspectFlags mImageAspectFlags;
		VkImageLayout mLayout{ VK_IMAGE_LAYOUT_UNDEFINED };
		VkDeviceSize byteAllocateSize;
		bool customCreateImage{false};
	};
}




#endif //!IMAGE_H

