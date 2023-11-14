#ifndef BUFFER_CPP
#define BUFFER_CPP
#include "Buffer.h"

namespace mainNameSpace::VulkanDriver {
	Buffer::Ptr Buffer::createVertexBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData) {
		VkBufferUsageFlags usageflag = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		VkMemoryPropertyFlags memoryflag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		Buffer::Ptr buffer = Buffer::create(device, size, usageflag, memoryflag);
		buffer->updateBufferByStage(pData, size);
		return buffer;
	}

	Buffer::Ptr Buffer::createIndexBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData) {
		VkBufferUsageFlags usageflag = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		VkMemoryPropertyFlags memoryflag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		Buffer::Ptr buffer = Buffer::create(device, size, usageflag, memoryflag);
		buffer->updateBufferByStage(pData, size);
		return buffer;
	}

	Buffer::Ptr Buffer::createUniformBuffer(Device::Ptr& device, VkDeviceSize size, void* pData){
		VkBufferUsageFlags usageflag = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		VkMemoryPropertyFlags memoryflag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		Buffer::Ptr buffer = Buffer::create(
			device, size,
			usageflag,
			memoryflag
		);
		if (pData != nullptr) {
			buffer->updateBufferByMap(pData, size);
		}
		return buffer;
	}

	Buffer::Ptr Buffer::createStageBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData) {
		VkBufferUsageFlags usageflag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkMemoryPropertyFlags memoryflag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		Buffer::Ptr buffer = Buffer::create(
			device, size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		if (pData != nullptr) {
			buffer->updateBufferByMap(pData, size);
		}
		return buffer;
	}

	Buffer::Buffer(const Device::Ptr& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
		mdevice = device;
		//create VKBuffer
		createVKBuffer(size, usage);
		//��ȡVKBuffer��memory��Ҫ��
		VkMemoryRequirements memRequire;
		vkGetBufferMemoryRequirements(mdevice->getDevice(), mBuffer, &memRequire);
		byteAllocateSize = memRequire.size;

		CreateBufferMemory(memRequire, properties);


	}

	Buffer::~Buffer() {
		if (mBuffer != VK_NULL_HANDLE) {
			vkDestroyBuffer(mdevice->getDevice(), mBuffer, nullptr);
		}

		if (mBufferMemory != VK_NULL_HANDLE) {
			vkFreeMemory(mdevice->getDevice(), mBufferMemory, nullptr);
		}
		mdevice.reset();
	}

	void Buffer::createVKBuffer(VkDeviceSize size, VkBufferUsageFlags usage) {
		//����Buffer��CreateInfo
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		//https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkBufferUsageFlagBits.html
		createInfo.usage = usage;
		//https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkBufferCreateFlagBits.html
		//ָ�����������Ӳ�����λ����
		//createInfo.flags
		//����ģʽ
		//VK_SHARING_MODE_EXCLUSIVEָ���Զ�����κη�Χ��ͼ����Դ�ķ���һ��ֻ�ܷ��ʵ��������塣
		//VK_SHARING_MODE_CONCURRENTָ��֧�ִӶ�������岢�з��ʶ�����κη�Χ��ͼ����Դ��
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(mdevice->getDevice(), &createInfo, nullptr, &mBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Error : failed to create buffer");
		}
	}

	void Buffer::CreateBufferMemory(VkMemoryRequirements& memRequire, VkMemoryPropertyFlags properties) {
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequire.size;

		allocInfo.memoryTypeIndex = getMemoryTypeInex(memRequire.memoryTypeBits, properties);

		if (vkAllocateMemory(mdevice->getDevice(), &allocInfo, nullptr, &mBufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to allocate memory");
		}

		VkResult re = vkBindBufferMemory(mdevice->getDevice(), mBuffer, mBufferMemory, 0);
		VkDescriptorBufferInfo info;
		info.buffer = mBuffer;
		info.offset = 0;
		info.range = byteAllocateSize;
	}

	uint32_t Buffer::getMemoryTypeInex(uint32_t memoryTypeBits,VkMemoryPropertyFlags properties) {
		//��ʱ����  ����ỻ��VMA��
		
		VkPhysicalDeviceMemoryProperties memProps;
		vkGetPhysicalDeviceMemoryProperties(mdevice->getPhysicalDevice(), &memProps);
		for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
			if ((memoryTypeBits & (1 << i)) && ((memProps.memoryTypes[i].propertyFlags & properties) == properties)) {
				return i;
			}
		}
		//TODO
		return -1;
	}

	void Buffer::updateBufferByMap(void* data, size_t size) {
		void* memPtr = nullptr;
		vkMapMemory(mdevice->getDevice(), mBufferMemory, 0, size, 0, &memPtr);
		memcpy(memPtr, data, size);
		vkUnmapMemory(mdevice->getDevice(), mBufferMemory);
	}

	void Buffer::updateBufferByStage(void* data, size_t size) {
		Buffer::Ptr stageBuffer = Buffer::create(mdevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		stageBuffer->updateBufferByMap(data, size);

		MemoryUtil::copyBuffer(mdevice,stageBuffer->getBuffer(), mBuffer, static_cast<VkDeviceSize>(size));
	}

}

#endif
