#ifndef SURFACE_H
#define SURFACE_H
#include "../base.h"
#include "Instance.h"
namespace mainNameSpace::VulkanDriver {

	class Surface
	{
	public:
		
		using Ptr = std::shared_ptr<Surface>;
		
		static Ptr create(Instance::Ptr instance) {
			return std::make_shared<Surface>(instance);
		}

		Surface(Instance::Ptr instance);

		~Surface();

		VkSurfaceKHR* getSurface() { return &vkSurface; };

	private:
		VkSurfaceKHR vkSurface{ VK_NULL_HANDLE };
		Instance::Ptr mInstance{ nullptr };
	};
	
}
#endif // !SURFACE_H




