#ifndef PIPELINE_CPP
#define PIPELINE_CPP
#include "pipeline.h"

namespace mainNameSpace::VulkanDriver {
	Pipeline::Pipeline(const Device::Ptr& device,const RenderPass::Ptr &renderpass)
	{
		mDevice = device;
		mRenderPass = renderpass;
	}

	Pipeline::~Pipeline()
	{
		if (mLayout != VK_NULL_HANDLE) {
			vkDestroyPipelineLayout(mDevice->getDevice(), mLayout, nullptr);
		}
		if (mPipeline != VK_NULL_HANDLE) {
			vkDestroyPipeline(mDevice->getDevice(), mPipeline, nullptr);
		}
		mRenderPass.reset();
		mDevice.reset();
	}

	void Pipeline::setViewports(std::vector<VkViewport> viewports) {
		mViewports.resize(viewports.size());
		mViewports = viewports;
	}

	void setpipeLineLayout(std::vector< VkDescriptorSetLayout> &descriptorSetLayouts){
		//TODO
		VkPipelineLayoutCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		//VkPipelineLayoutCreateFlags     flags;TODO
		
		pipelineInfo.setLayoutCount = descriptorSetLayouts.size();
		pipelineInfo.pSetLayouts = descriptorSetLayouts.data();
		//TODO
		//uint32_t                        pushConstantRangeCount;
		//const VkPushConstantRange* pPushConstantRanges;*/
	}

	void Pipeline::setVertexInputState(std::vector<VkVertexInputBindingDescription> &vertexBindDescription,std::vector<VkVertexInputAttributeDescription> &attributeDescriptions) {
		mVertexBindDescription = vertexBindDescription;
		mAttributeDescriptions = attributeDescriptions;
		mVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		mVertexInputState.vertexBindingDescriptionCount =static_cast<uint32_t>(mVertexBindDescription.size());
		mVertexInputState.pVertexBindingDescriptions = mVertexBindDescription.data();
		mVertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(mAttributeDescriptions.size());
		mVertexInputState.pVertexAttributeDescriptions = mAttributeDescriptions.data();
		//mVertexInputState.flags 预留
	}

	void Pipeline::setInputAssemblyState(VkPrimitiveTopology topology ,VkBool32 primitiveRestart) {
		//IA
		mAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		mAssemblyState.primitiveRestartEnable = primitiveRestart;
		mAssemblyState.topology = topology;
		//mAssemblyState.pNext;TODO
		//mAssemblyState.flags;TODO
	}

	void Pipeline::setViewportState() {
		mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		mViewportState.viewportCount = static_cast<uint32_t>(mViewports.size());
		mViewportState.pViewports = mViewports.data();
		mViewportState.scissorCount = static_cast<uint32_t>(mScissors.size());
		mViewportState.pScissors = mScissors.data();
	}

	//光栅化状态
	void Pipeline::setRasterizationStatge(VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable) {
		mRasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		mRasterState.depthClampEnable = depthClampEnable;//是否在光栅化的时候抛弃所有图元
		mRasterState.rasterizerDiscardEnable = rasterizerDiscardEnable;//超出近平面和远平面的片段就会被夹在其中，而不是丢弃。在生成ShadowMap的过程会被用到
		mRasterState.frontFace = frontFace;
		mRasterState.cullMode = cullMode;
		mRasterState.depthBiasEnable = VK_FALSE;
		mRasterState.polygonMode = VK_POLYGON_MODE_FILL;
		mRasterState.lineWidth = 1.0f;
		mRasterState.depthBiasConstantFactor = 0.0f;
		mRasterState.depthBiasClamp = 0.0f;
		mRasterState.depthBiasSlopeFactor = 0.0f;
	}

	void Pipeline::setColorBlendState(VkBool32 logicOpenable, VkLogicOp logic,std::vector<VkPipelineColorBlendAttachmentState> colorBlendndOps) {
		mcolorBlendndOps = colorBlendndOps;
		mBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		mBlendState.logicOpEnable = logicOpenable;
		mBlendState.logicOp = logic; //VK_LOGIC_OP_COPY
		mBlendState.pAttachments = mcolorBlendndOps.data();
		mBlendState.attachmentCount = static_cast<uint32_t>(mcolorBlendndOps.size());
		mBlendState.blendConstants[0] = 0.0;
		mBlendState.blendConstants[1] = 0.0;
		mBlendState.blendConstants[2] = 0.0;
		mBlendState.blendConstants[3] = 0.0;

		//mBlendState.pNext;
		//mBlendState.flags;VkPipelineColorBlendStateCreateFlags指定附加的颜色混合信息TODO
	}

	void Pipeline::setDepthStencilState(VkBool32 depthTest,VkBool32 depthWrite, VkCompareOp depthCompare, VkBool32 stencilTest, VkStencilOpState* stencilFront, VkStencilOpState* stencilBake) {
		mDepthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		//VkStructureType                           sType;
		// const void* pNext;
		//VkPipelineDepthStencilStateCreateFlags    flags;TODO
		mDepthStencilState.depthBoundsTestEnable = false;//TODO
		//float                                     minDepthBounds;
		//float                                     maxDepthBounds;
		mDepthStencilState.depthTestEnable = depthTest;
		mDepthStencilState.depthWriteEnable = depthWrite;
		mDepthStencilState.depthCompareOp = depthCompare;
		mDepthStencilState.stencilTestEnable = stencilTest;
		//VkBool32                                  stencilTestEnable;
		if(stencilFront!= NULL)
			mDepthStencilState.front = *stencilFront;
		if (stencilBake != NULL)
			mDepthStencilState.back = *stencilBake;
	}

	void Pipeline::setMultisampleState(VkBool32 enable,VkSampleCountFlagBits mulSamples) {
		//const void* pNext;
		//VkPipelineMultisampleStateCreateFlags    flags;
		mmultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		mmultisampleState.rasterizationSamples = mulSamples;
		mmultisampleState.sampleShadingEnable = enable;
		mmultisampleState.minSampleShading = 1.0f;
		//TODO
		mmultisampleState.pSampleMask = nullptr;
		mmultisampleState.alphaToCoverageEnable = VK_FALSE;
		mmultisampleState.alphaToOneEnable = VK_FALSE;
	}

	void Pipeline::setpipeLineLayout(std::vector<DescriptorSetLayout::Ptr> layouts) {
		mdescriptorSetLayout.resize(layouts.size());
		mdescriptorSetLayout = layouts;

		
	}

	void Pipeline::build() {
		std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos{};
		for (const auto& shader : mShaders) {
			VkPipelineShaderStageCreateInfo shaderCreateInfo{};
			shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderCreateInfo.stage = shader->getShaderStage();
			shaderCreateInfo.pName = shader->getShaderEntryPoint().c_str();
			shaderCreateInfo.module = shader->getShaderModule();
			shaderCreateInfos.push_back(shaderCreateInfo);
		}

		setViewportState();
		if (mLayout != VK_NULL_HANDLE) {
			vkDestroyPipelineLayout(mDevice->getDevice(), mLayout, nullptr);
		}

		mLayoutState.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		mLayoutState.setLayoutCount = mdescriptorSetLayout.size();
		//VkDescriptorl
		std::vector< VkDescriptorSetLayout> vklayouts;
		for (const DescriptorSetLayout::Ptr lay : mdescriptorSetLayout) {
			vklayouts.push_back(lay->getLayout());
		}

		mLayoutState.pSetLayouts = vklayouts.data();;
		mLayoutState.pushConstantRangeCount = 0;
		mLayoutState.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(mDevice->getDevice(), &mLayoutState, nullptr, &mLayout) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create pipeline layout");
		}

		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderCreateInfos.size());
		pipelineCreateInfo.pStages = shaderCreateInfos.data();

		pipelineCreateInfo.pVertexInputState = &mVertexInputState;
		pipelineCreateInfo.pInputAssemblyState = &mAssemblyState;
		pipelineCreateInfo.pViewportState = &mViewportState;
		pipelineCreateInfo.pRasterizationState = &mRasterState;
		pipelineCreateInfo.pMultisampleState = &mmultisampleState;
		pipelineCreateInfo.pDepthStencilState = &mDepthStencilState;
		pipelineCreateInfo.pColorBlendState = &mBlendState;
		pipelineCreateInfo.layout = mLayout;
		pipelineCreateInfo.renderPass = mRenderPass->getRenderPass();
		pipelineCreateInfo.subpass = 0;

		

		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;

		if (mPipeline != VK_NULL_HANDLE) {
			vkDestroyPipeline(mDevice->getDevice(), mPipeline, nullptr);
		}

		if (vkCreateGraphicsPipelines(mDevice->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mPipeline) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create pipeline");
		}

	}
}

#endif // !PIPELINE_CPP


