#ifndef PIPELINE_H
#define PIPELINE_H

#include "../base.h"
#include "Device.h"
#include "Shader.h"
#include "RenderPass.h"
#include "DescriptorSetLayout.h"
namespace mainNameSpace::VulkanDriver {
	class Pipeline
	{
	public:
		using Ptr = std::shared_ptr<Pipeline>;
		static Ptr create(const Device::Ptr& device, const RenderPass::Ptr& renderpass) {
			return std::make_shared<Pipeline>(device, renderpass);
		}
	public:
		Pipeline(const Device::Ptr& device, const RenderPass::Ptr& renderpass);
		~Pipeline();

		void setShaderGroup(const std::vector<Shader::Ptr>& shaderGroup) { mShaders = shaderGroup; }

		void setViewports(std::vector<VkViewport> viewports);

		void setScissors(const std::vector<VkRect2D>& scissors) { mScissors = scissors; }


		//TODO
		void setpipeLineLayout(std::vector<DescriptorSetLayout::Ptr> layouts);

		void setVertexInputState(std::vector<VkVertexInputBindingDescription>& vertexBindDescription, std::vector<VkVertexInputAttributeDescription>& attributeDescriptions);

		void setInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestart);

		void setRasterizationStatge(VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable);

		void setColorBlendState(VkBool32 logicOpenable, VkLogicOp logic, std::vector<VkPipelineColorBlendAttachmentState> colorBlendndOps);

		void setDepthStencilState(VkBool32 depthTest, VkBool32 depthWrite, VkCompareOp depthCompare, VkBool32 stencilTest, VkStencilOpState* stencilFront, VkStencilOpState* stencilBake);

		void setMultisampleState(VkBool32 enable, VkSampleCountFlagBits mulSamples);

		[[nodiscard]] auto getPipeline() const { return mPipeline; }
		[[nodiscard]] auto getLayout() const { return mLayout; }

		void build();
	private:
		void setViewportState();
		VkPipelineVertexInputStateCreateInfo mVertexInputState{};
		VkPipelineInputAssemblyStateCreateInfo mAssemblyState{};
		VkPipelineViewportStateCreateInfo mViewportState{};
		VkPipelineRasterizationStateCreateInfo mRasterState{};
		VkPipelineMultisampleStateCreateInfo mmultisampleState{};
		VkPipelineColorBlendStateCreateInfo mBlendState{};
		VkPipelineDepthStencilStateCreateInfo mDepthStencilState{};
		VkPipelineLayoutCreateInfo mLayoutState{};

	private:
		Device::Ptr mDevice{ nullptr };
		RenderPass::Ptr mRenderPass{ nullptr };
		VkPipeline mPipeline{ VK_NULL_HANDLE };
		VkPipelineLayout mLayout{ VK_NULL_HANDLE };
		
		std::vector<Shader::Ptr> mShaders{};

		
		
		std::vector<VkViewport> mViewports{};
		std::vector<VkRect2D> mScissors{};

		std::vector<VkPipelineColorBlendAttachmentState> mBlendAttachmentStates{};
		std::vector<DescriptorSetLayout::Ptr> mdescriptorSetLayout;
		
		std::vector<VkVertexInputBindingDescription> mVertexBindDescription;
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;
		//FrameBuffer TODO
		std::vector<VkPipelineColorBlendAttachmentState> mcolorBlendndOps;
	};


}
#endif

