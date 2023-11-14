#ifndef SURFACE_CPP
#define SURFACE_CPP

#include "Surface.h"
namespace mainNameSpace::VulkanDriver {
	Surface::Surface(Instance::Ptr instance) {
		mInstance = instance;
	}

	Surface::~Surface() {
		if (vkSurface != VK_NULL_HANDLE) {
			vkDestroySurfaceKHR(mInstance->getInstance(), vkSurface, nullptr);
		}
		mInstance.reset();
	}
}



#endif // !SURFACE_CPP


