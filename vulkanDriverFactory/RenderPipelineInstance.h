#ifndef RENDERPIPELINEINSTANCE_H
#define RENDERPIPELINEINSTANCE_H

#include "../base.h"

//���ݺ���߸���״̬�ı��� �������Pipeline
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