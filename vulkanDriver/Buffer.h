#ifndef BUFFER_H
#define BUFFER_H

/*
* 
// 指定用这种类型分配的内存对于Device访问是最有效的，当且仅当Memorytype属于具有VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT = 0x00000001,

// 指定为Host访问映射分配的内存。可以被主机映射，并且使用vkMapMemory获得
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT = 0x00000002,
//此位指主机缓存管理命令vkFlushMappedMemoryRanges and vkInvalidateMappedMemoryRanges不需要Flush主机写入到device
//或者标记divice写入的内存对主机可见
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT = 0x00000004,
//指定使用此类型分配的内存缓存在主机上。主机内存访问未缓存内存的速度慢于访问缓存内存，但未缓存内存始终与主机一致。
    VK_MEMORY_PROPERTY_HOST_CACHED_BIT = 0x00000008,

    VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT = 0x00000010,
  // Provided by VK_VERSION_1_1
  //指定存储器类型仅允许设备访问存储器，并允许受保护的队列操作访问存储器。内存类型不能具有HOST相关的bit
    VK_MEMORY_PROPERTY_PROTECTED_BIT = 0x00000020,
//指定对这种内存类型的分配的设备访问是自动可用和可见的。
  // Provided by VK_AMD_device_coherent_memory
    VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD = 0x00000040,
  // Provided by VK_AMD_device_coherent_memory
  //指定使用此类型分配的内存不缓存在设备上。未缓存的设备内存始终与设备一致。
    VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD = 0x00000080,
  // Provided by VK_NV_external_memory_rdma
  //指定外部设备可以直接访问此内存。
    VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV = 0x00000100,
*/


/*对于带有Host—visible的掩码位申请出来的对象，device可以使用VKMapMemory来获取CPU的指针来访问device Memory数据
* 使用vkUnmapMemory来释放，如果过程中有cpu写，则可能需要使用vkFlushMappedMemoryRanges来使得cpu写入gpu，
*或者相反，如果有gpu写，则需要使用vkInvalidateMappedMemoryRanges(3)使得其对cpu可见。
*当然，如果掩码位同时还有host-coherent，那么就不需要这两个api了。
*/

#include "../base.h"
#include "Device.h"
#include "MemoryUtil.h"
namespace mainNameSpace::VulkanDriver {
	class Buffer
	{
	public:
		using Ptr = std::shared_ptr<Buffer>;
		static Ptr create(const Device::Ptr& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
			return std::make_shared<Buffer>(device, size, usage, properties);
		}
		
		static Ptr createVertexBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData);
		static Ptr createIndexBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData);
		static Ptr createUniformBuffer( Device::Ptr& device, VkDeviceSize size, void* pData);
		static Ptr createStageBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData);

		
	public:
		Buffer(const Device::Ptr& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		
		~Buffer();

		//set buffer Data
		void updateBufferByMap(void* data, size_t size);

		void updateBufferByStage(void* data, size_t size);

		[[nodiscard]] VkBuffer& getBuffer() { return mBuffer; };
		[[nodiscard]] VkDeviceSize getBufferbyteSize(){ return byteAllocateSize; };
	private:
		void createVKBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
		void CreateBufferMemory(VkMemoryRequirements& memRequire, VkMemoryPropertyFlags properties);
		uint32_t Buffer::getMemoryTypeInex(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
	private:
		Device::Ptr mdevice{ VK_NULL_HANDLE };
		VkDeviceMemory mBufferMemory{ VK_NULL_HANDLE };
		
		VkDeviceSize byteAllocateSize;
		VkBuffer mBuffer{VK_NULL_HANDLE};

	};
}

#endif
