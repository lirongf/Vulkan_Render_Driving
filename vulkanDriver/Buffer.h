#ifndef BUFFER_H
#define BUFFER_H

/*
* 
// ָ�����������ͷ�����ڴ����Device����������Ч�ģ����ҽ���Memorytype���ھ���VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT = 0x00000001,

// ָ��ΪHost����ӳ�������ڴ档���Ա�����ӳ�䣬����ʹ��vkMapMemory���
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT = 0x00000002,
//��λָ���������������vkFlushMappedMemoryRanges and vkInvalidateMappedMemoryRanges����ҪFlush����д�뵽device
//���߱��diviceд����ڴ�������ɼ�
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT = 0x00000004,
//ָ��ʹ�ô����ͷ�����ڴ滺���������ϡ������ڴ����δ�����ڴ���ٶ����ڷ��ʻ����ڴ棬��δ�����ڴ�ʼ��������һ�¡�
    VK_MEMORY_PROPERTY_HOST_CACHED_BIT = 0x00000008,

    VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT = 0x00000010,
  // Provided by VK_VERSION_1_1
  //ָ���洢�����ͽ������豸���ʴ洢�����������ܱ����Ķ��в������ʴ洢�����ڴ����Ͳ��ܾ���HOST��ص�bit
    VK_MEMORY_PROPERTY_PROTECTED_BIT = 0x00000020,
//ָ���������ڴ����͵ķ�����豸�������Զ����úͿɼ��ġ�
  // Provided by VK_AMD_device_coherent_memory
    VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD = 0x00000040,
  // Provided by VK_AMD_device_coherent_memory
  //ָ��ʹ�ô����ͷ�����ڴ治�������豸�ϡ�δ������豸�ڴ�ʼ�����豸һ�¡�
    VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD = 0x00000080,
  // Provided by VK_NV_external_memory_rdma
  //ָ���ⲿ�豸����ֱ�ӷ��ʴ��ڴ档
    VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV = 0x00000100,
*/


/*���ڴ���Host��visible������λ��������Ķ���device����ʹ��VKMapMemory����ȡCPU��ָ��������device Memory����
* ʹ��vkUnmapMemory���ͷţ������������cpuд���������Ҫʹ��vkFlushMappedMemoryRanges��ʹ��cpuд��gpu��
*�����෴�������gpuд������Ҫʹ��vkInvalidateMappedMemoryRanges(3)ʹ�����cpu�ɼ���
*��Ȼ���������λͬʱ����host-coherent����ô�Ͳ���Ҫ������api�ˡ�
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
