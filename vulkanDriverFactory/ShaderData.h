#ifndef SHADERDATA_H
#define SHADERDATA_H

#include "../base.h"
#include "../vulkanDriver/Device.h"
#include "../vulkanDriver/DescriptorSetLayout.h"
#include "../vulkanDriver/DescriptorSet.h"
#include "../vulkanDriver/DescriptorPool.h"
#include "../vulkanDriver/Buffer.h"
#include "../vulkanDriver/Image.h"
#include "../vulkanDriver/Sampler.h"
//�洢������Դ������DescriptorSetLayout
	// �����Ѿ��õ���UBO Struct������UBOBuffer������������Ҫ������
	//��֮������Դ�Ĳ��ֺͼ���
//����ShaderData�ĺ�  ������Pipeline��Layout

namespace mainNameSpace::VulkanDriverFactory {
	
	enum ShaderDataType {
		Int,
		Bool,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Color,
		Matrix4x4,
		Texture2D,
		TextureCube,
		Buffer,
	};
	
	struct UniformProperty
	{
		uint32_t id{ 0 };
		std::string propertyName;
		ShaderDataType uniformType;
	};

	struct OnePropertyAttribute {
		uint32_t size;
		uint32_t offset;
		ShaderDataType type;
	};

	//������DescriptorSet��Buffer����
	struct DescriptorImageUpdate
	{
		VulkanDriver::UniformParameter::Ptr params;
		VkDescriptorImageInfo ImageInfo;
		//VkDescriptorImageInfo Buffer;
	};

	class ShaderData {
	public:
		using Ptr = std::shared_ptr<ShaderData>;
		static Ptr create(uint32_t setIndex) { return std::make_shared<ShaderData>(setIndex); }
	public:
		ShaderData(uint32_t setIndex);
		~ShaderData();

		void setUniformMap(std::vector<UniformProperty> uboMap,VulkanDriver::Device::Ptr device);

		void uploadData(uint32_t curFram);

		void setVector4(uint32_t index, glm::vec4 data);

		void setMatrix4x4(uint32_t index, glm::mat4 data);

		void setTexture2D(uint32_t index, VulkanDriver::Image::Ptr data, VulkanDriver::Sampler::Ptr sampler);

		[[nodiscard]] VulkanDriver::DescriptorSetLayout::Ptr getDescriptorSetLayout() const { return mDescriptorSetLayout; }
		[[nodiscard]] VulkanDriver::DescriptorSet::Ptr getDescriptorSet() const { return mDescriptorSet; }
	private:
		void createDescriptSetLayoutByMap();
		

	private:
		VulkanDriver::Device::Ptr mDevice{ nullptr };
		std::vector<VulkanDriver::UniformParameter::Ptr> mUniformParams;
		
		VulkanDriver::DescriptorSetLayout::Ptr mDescriptorSetLayout{ nullptr };
		VulkanDriver::DescriptorPool::Ptr	mDescriptorPool{ nullptr };
		VulkanDriver::DescriptorSet::Ptr mDescriptorSet{ nullptr };
		uint32_t mSetIndex;
		uint32_t mPoolSet{3};

		std::vector< VulkanDriver::Buffer::Ptr> largeUBOBuffers;
		std::vector< VkDescriptorBufferInfo> largeUBODescriptors;
		VulkanDriver::UniformParameter::Ptr largeBufferParams;

		std::vector<float> uboBufferData;
		std::unordered_map<uint32_t, OnePropertyAttribute> largeUBOParamsMap; 


		std::unordered_map<uint32_t, void*> cacheData;//TODO ɾ��

		std::unordered_map<uint32_t, VkDescriptorBufferInfo> bufferData;
		std::unordered_map<uint32_t, VulkanDriver::Buffer::Ptr> BufferData;

		std::unordered_map<uint32_t, DescriptorImageUpdate> ImageDescriptorInfos;
		std::unordered_map<uint32_t, VulkanDriver::Image::Ptr> ImageInfos;
	};
}

#endif // ! SHADERDATA_H