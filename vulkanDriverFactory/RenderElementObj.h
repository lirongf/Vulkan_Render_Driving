#ifndef RENDERELEMENTOBJ_H
#define RENDERELEMENTOBJ_H

#include "../base.h"

//渲染单元，记录了Geometry，ShaderData组，Pipeline，Shader，录制命令
namespace mainNameSpace::VulkanDriverFactory {
	class RenderElementObj {
	public:
		RenderElementObj();
		~RenderElementObj();
	};
}

#endif // ! BUFFERSTATE_H