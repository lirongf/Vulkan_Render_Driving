#ifndef SHADERDATA_CPP
#define SHADERDATA_CPP

#include "ShaderData.h"

namespace mainNameSpace::VulkanDriverFactory {
	
	bool isUniformBuffer(UniformProperty param) {
		if (param.uniformType == ShaderDataType::Texture2D || param.uniformType == ShaderDataType::TextureCube)
			return false;
		else
			return true;
	}

	uint32_t uniformPropertyLength(UniformProperty param) {
		switch (param.uniformType)
		{
		case ShaderDataType::Int:
		case ShaderDataType::Bool:
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Vector2:
			return 8;
		case ShaderDataType::Vector3:
			return 12;
		case ShaderDataType::Vector4:
		case ShaderDataType::Color:
			return 16;
		case ShaderDataType::Matrix4x4:
			return 64;
		default:
			return 0;
		}
	}
	
	ShaderData::ShaderData(uint32_t setIndex){
		mSetIndex = setIndex;
	}

	ShaderData::~ShaderData(){
		mUniformParams.clear();
		mDescriptorSetLayout.reset();
		mDescriptorPool.reset();
		mDescriptorSet.reset();
	}


	void ShaderData::setUniformMap(std::vector<UniformProperty> uboMap, VulkanDriver::Device::Ptr device) {
		mUniformParams.clear();
		largeUBOParamsMap.clear();
		mDevice = device;

		uint32_t largeUBOBufferoffset = 0;
		uint32_t bindPos = 0;
		for (const UniformProperty& param : uboMap) {
			if (isUniformBuffer(param)) {
				OnePropertyAttribute one{};
				one.size = uniformPropertyLength(param);
				one.offset = largeUBOBufferoffset;
				one.type = param.uniformType;
				largeUBOParamsMap.emplace(param.id, one);
				largeUBOBufferoffset += one.size;
			}
			else if(param.uniformType == ShaderDataType::Buffer)
			{
				//TODO
			}
			else {
				VulkanDriver::UniformParameter::Ptr uniformParameter= VulkanDriver::UniformParameter::create();
				
				uniformParameter->mBinding = bindPos++;
				uniformParameter->mCount = 1;
				uniformParameter->mDescriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//TODO
				uniformParameter->mStage = VK_SHADER_STAGE_ALL_GRAPHICS;
				uniformParameter->mSize = 0;
				mUniformParams.push_back(uniformParameter);
				
				DescriptorImageUpdate imageUpdateDes{};
				imageUpdateDes.params = uniformParameter;
				
				ImageDescriptorInfos.emplace(param.id, imageUpdateDes);
			}
		}
		if (largeUBOParamsMap.size() > 0) {
			//create UBO Buffer
			largeBufferParams = VulkanDriver::UniformParameter::create();
			largeBufferParams->mBinding = bindPos++;
			largeBufferParams->mCount = 1;
			largeBufferParams->mDescriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			largeBufferParams->mStage = VK_SHADER_STAGE_ALL_GRAPHICS;
			largeBufferParams->mSize = largeUBOBufferoffset;
			mUniformParams.push_back(largeBufferParams);
			uboBufferData.resize(largeUBOBufferoffset / 4);

			//为每一个交换链创建UBO Buffer
			largeUBOBuffers.resize(mPoolSet);
			largeUBODescriptors.resize(mPoolSet);

			for (uint32_t i = 0; i < mPoolSet; i++) {
				largeUBOBuffers[i] = VulkanDriver::Buffer::createUniformBuffer(mDevice, largeUBOBufferoffset, uboBufferData.data());
				largeUBODescriptors[i].buffer = largeUBOBuffers[i]->getBuffer();
				largeUBODescriptors[i].offset = 0;
				largeUBODescriptors[i].range = largeBufferParams->mSize;
			}
			
			
		}
		createDescriptSetLayoutByMap();
	}

	void ShaderData::createDescriptSetLayoutByMap() {
		mDescriptorSetLayout = VulkanDriver::DescriptorSetLayout::create(mDevice);
		mDescriptorSetLayout->build(mUniformParams);

		mDescriptorPool = VulkanDriver::DescriptorPool::create(mDevice);
		mDescriptorPool->build(mUniformParams, mPoolSet);

		mDescriptorSet = VulkanDriver::DescriptorSet::create(mDevice, mUniformParams, mDescriptorSetLayout, mDescriptorPool, mPoolSet);
		
		//Update DescriptSet
		if (largeUBOBuffers.size() > 0) {
			std::vector<VkWriteDescriptorSet> writeSet;
			writeSet.resize(mPoolSet);
			for (uint32_t i = 0; i < mPoolSet; i++) {
				writeSet[i] = mDescriptorSet->descriptorSetWritesBuffer(largeBufferParams, mDescriptorSet->getDescriptorSet(i), largeUBODescriptors[i]);
			}
			mDescriptorSet->updateDescriptorSets(writeSet);
		}
	}

	void ShaderData::setVector4(uint32_t index, glm::vec4 data) {
		if (cacheData.find(index) == cacheData.end()) {
			glm::vec4* v4 = new glm::vec4(data);
			cacheData.emplace(index, v4);
		}
		else {
			if (*(glm::vec4*)(cacheData[index]) == data) {
				return;
			}
			else
			{
				*(glm::vec4*)(cacheData[index]) = data;
			}
		}
		if (largeUBOParamsMap.find(index) != largeUBOParamsMap.end()) {
			OnePropertyAttribute attributeInfo = largeUBOParamsMap[index];
			size_t datapos = attributeInfo.offset / 4;
			uboBufferData[datapos] = data.x;
			uboBufferData[datapos + 1] = data.y;
			uboBufferData[datapos + 2] = data.z;
			uboBufferData[datapos + 3] = data.w;
		}
	}

	void ShaderData::setMatrix4x4(uint32_t index, glm::mat4 data) {
		if (cacheData.find(index) == cacheData.end()) {
			glm::mat4* v4 = new glm::mat4(data);
			cacheData.emplace(index, v4);
		}
		else {
			if (*(glm::mat4*)(cacheData[index]) == data) {
				return;
			}
			else
			{
				*(glm::mat4*)(cacheData[index]) = data;
			}
		}
		if (largeUBOParamsMap.find(index) != largeUBOParamsMap.end()) {
			OnePropertyAttribute attributeInfo = largeUBOParamsMap[index];
			size_t datapos = attributeInfo.offset / 4;
			for (int i = 0; i < 16; i++) {
				uboBufferData[datapos + i] = data[i/4][i%4];
			}
		}
	}

	void ShaderData::setTexture2D(uint32_t index, VulkanDriver::Image::Ptr data,VulkanDriver::Sampler::Ptr sampler) {
		if ((ImageInfos.find(index) != ImageInfos.end())) {
			if (ImageInfos[index] == data)
				return;
			else
				ImageInfos.erase(index);
		}
		ImageInfos.emplace(index, data);

		if (ImageDescriptorInfos.find(index) != ImageDescriptorInfos.end()) {
			DescriptorImageUpdate imageInfo = ImageDescriptorInfos[index];
			ImageDescriptorInfos[index].ImageInfo.imageLayout = data->getLayout();
			ImageDescriptorInfos[index].ImageInfo.imageView = data->getImageView();
			ImageDescriptorInfos[index].ImageInfo.sampler = sampler->getSampler();
			std::vector<VkWriteDescriptorSet> writeSet;
			writeSet.resize(mPoolSet);
			for (uint32_t i = 0; i < mPoolSet; i++) {
				writeSet[i] = mDescriptorSet->descriptorSetWritesImage(ImageDescriptorInfos[index].params, mDescriptorSet->getDescriptorSet(i), ImageDescriptorInfos[index].ImageInfo);
			}
			mDescriptorSet->updateDescriptorSets(writeSet);
		}

		
	}

	void ShaderData::uploadData(uint32_t curFram){
		if (largeUBOBuffers.size() > 0) {
			largeUBOBuffers[curFram]->updateBufferByMap(uboBufferData.data(), uboBufferData.size() * sizeof(float));
		}
	}

}
#endif // ! SHADERDATA_CPP