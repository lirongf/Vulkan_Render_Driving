#ifndef RENDERGEOMETRY_H
#define RENDERGEOMETRY_H

#include "../base.h"

//所有和渲染draw相关的信息，drawindex还是instance 偏移是多少，包含BufferState
namespace mainNameSpace::VulkanDriverFactory {
	class RenderGeometry {
	public:
		RenderGeometry();
		~RenderGeometry();
	};
}

#endif // ! RENDERGEOMETRY_H