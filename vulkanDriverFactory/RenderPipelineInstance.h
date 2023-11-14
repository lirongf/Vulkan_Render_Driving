#ifndef RENDERPIPELINEINSTANCE_H
#define RENDERPIPELINEINSTANCE_H

#include "../base.h"

//根据宏或者各种状态的变量 缓存变量Pipeline
namespace mainNameSpace::VulkanDriverFactory {
	class RenderPipelineInstance {
	public:

		RenderPipelineInstance();
		~RenderPipelineInstance();

		void getBlendState();

		void getDepthStencilState();
		void getPrimitiveState();
		void getVertexAttributeLayout();
		void getGPURenderPipeline();
		void uploadUniforms();

	};
}

#endif // ! RENDERPIPELINEINSTANCE_H