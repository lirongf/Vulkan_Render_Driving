//VkShaderStageFlagBits
//VK_SHADER_STAGE_VERTEX_BIT = 0x00000001,
//VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT = 0x00000002,
//VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT = 0x00000004,
//VK_SHADER_STAGE_GEOMETRY_BIT = 0x00000008,
//VK_SHADER_STAGE_FRAGMENT_BIT = 0x00000010,
//VK_SHADER_STAGE_COMPUTE_BIT = 0x00000020,
//VK_SHADER_STAGE_ALL_GRAPHICS = 0x0000001F,
//VK_SHADER_STAGE_ALL = 0x7FFFFFFF,
//VkDescriptorType
		//VK_DESCRIPTOR_TYPE_SAMPLER = 0,
		//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER = 1,
		//VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE = 2,
		//VK_DESCRIPTOR_TYPE_STORAGE_IMAGE = 3,
		//VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER = 4,
		//VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER = 5,
		//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 6,
		//VK_DESCRIPTOR_TYPE_STORAGE_BUFFER = 7,
		//VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC = 8,
		//VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC = 9,
		//VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT = 10,
		//// Provided by VK_VERSION_1_3
		//VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK = 1000138000,
		//// Provided by VK_KHR_acceleration_structure
		//VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR = 1000150000,
		//// Provided by VK_NV_ray_tracing
		//VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV = 1000165000,
		//// Provided by VK_QCOM_image_processing
		//VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM = 1000440000,
		//// Provided by VK_QCOM_image_processing
		//VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM = 1000440001,
		//// Provided by VK_EXT_mutable_descriptor_type
		//VK_DESCRIPTOR_TYPE_MUTABLE_EXT = 1000351000,
		//// Provided by VK_EXT_inline_uniform_block
		//VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK,
		//// Provided by VK_VALVE_mutable_descriptor_type
		//VK_DESCRIPTOR_TYPE_MUTABLE_VALVE = VK_DESCRIPTOR_TYPE_MUTABLE_EXT,
#ifndef  DESCRIPTORLAYOUT_H
#define DESCRIPTORLAYOUT_H
#include "Device.h"
#include "../base.h"
#include "Buffer.h"
#include "Image.h"

namespace mainNameSpace::VulkanDriver {
		
	struct UniformParameter
	{
	public:
		using Ptr = std::shared_ptr<UniformParameter>;
		static Ptr create() { return std::make_shared< UniformParameter>(); }
		size_t					mSize{ 0 };
		uint32_t				mBinding{ 0 };
		uint32_t				mCount{ 0 };
		VkDescriptorType		mDescriptorType;
		VkShaderStageFlagBits	mStage;
		
		//ShaderParameterType
		//ShaderParameterName
	};
	
	class DescriptorSetLayout
	{
	public:
		using Ptr = std::shared_ptr<DescriptorSetLayout>;
		static Ptr create(const Device::Ptr& device) { return std::make_shared<DescriptorSetLayout>(device); }
		DescriptorSetLayout(const Device::Ptr& device);

		~DescriptorSetLayout();

		void build(const std::vector<UniformParameter::Ptr>& params);
		[[nodiscard]] auto getLayout() const { return mLayout; }
	private:
		std::vector<UniformParameter::Ptr> mParams{};
		VkDescriptorSetLayout mLayout{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };
	};
}


#endif // ! DESCRIPTORLAYOUT_H