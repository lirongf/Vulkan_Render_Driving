#ifndef APPLICATION_H
#define APPLICATION_H

#include "base.h"
#include"Window/glfwWindow.h"
#include "vulkanDriver/Instance.h"
#include "vulkanDriver/Surface.h"
#include "vulkanDriver/Device.h"
#include "vulkanDriver/SwapChain.h"

#include "vulkanDriverFactory/RenderPassContext.h"
#include "vulkanDriver/MemoryUtil.h"
//Test
#include "vulkanDriver/Image.h"
#include "vulkanDriver/Sampler.h"

#include "stb_image.h"
#include "tiny_obj_loader.h"

#include "vulkanDriverFactory/ShaderData.h"
#include "vulkanDriverFactory/BufferState.h"
#include "vulkanDriver/Pipeline.h"
#include "vulkanDriver/Shader.h"

#include "vulkanDriver/Semaphore.h"
#include "vulkanDriver/Fence.h"

#include "Camera.h"

namespace mainNameSpace {

	class Application: public std::enable_shared_from_this<Application>
	{
	public:
		static std::vector<char> readBinary(const std::string& fileName);
	public:
		Application() = default;

		~Application() = default;
		//运行程序
		void run();
		//鼠标事件
		void onMouseMove(double xpos, double ypos);
		//键盘事件
		void onKeyDown(int keyIndex);
		//鼠标按下事件
		void onMouseButton(int button, int action, int mods);

	private:
		void initWindow();

		void initVulkan();

		void mainLoop();

		void render();

		void cleanUp();

	private:
		//Window width
		int mWidth{ 800 };
		//Window Height
		int mHeight{ 600 };

	private:
		wind::glfwWindow::Ptr mWindow{ nullptr };
		VulkanDriver::Instance::Ptr mVkInstance{ nullptr };
		VulkanDriver::Surface::Ptr mSurface{ nullptr };
		VulkanDriver::Device::Ptr mDevice{ nullptr };
		VulkanDriver::SwapChain::Ptr mSwapChain{ nullptr };
		
		//TODO 临时测试
		VulkanDriver::RenderPass::Ptr mRenderPass{ nullptr };
		/*
		Wrapper::Pipeline::Ptr mPipeline{ nullptr };
		
		Wrapper::CommandPool::Ptr mCommandPool{ nullptr };
		std::vector<Wrapper::CommandBuffer::Ptr> mCommandBuffers{};

		std::vector<Wrapper::Semaphore::Ptr> mImageAvailableSemaphores{};
		std::vector<Wrapper::Semaphore::Ptr> mRenderFinishedSemaphores{};
		std::vector<Wrapper::Fence::Ptr> mFences{};

		UniformManager::Ptr mUniformManager{ nullptr };

		Model::Ptr	mModel{ nullptr };
		VPMatrices	mVPMatrices;
		Camera      mCamera;*/
		Camera      mCamera; 
		VulkanDriver::CommandPool::Ptr mCommandPool;
		std::vector<VulkanDriver::CommandBuffer::Ptr> mCommandBuffers{};
		//创建ShaderData
		VulkanDriver::Image::Ptr mImage;
		VulkanDriver::Sampler::Ptr mSampler;
		void createImageByPath();
		VulkanDriverFactory::ShaderData::Ptr materialShaderData;//1
		void createMaterialShaderData();
		//创建Camera ShaderData
		VulkanDriverFactory::ShaderData::Ptr cameraShaderData;//0
		void createCameraShaderData();
		//TODO
		//void createModuleShaderData();
		// 创建RenderGeometry，创建RenderBuffer
		VulkanDriverFactory::BufferState::Ptr mBufferState;
		void loadModel();
		void loadplane();
		VulkanDriver::Pipeline::Ptr mPipeline;
		void createPipeline();
		void createCommandBuffer();
		void createSyncObjects();
		void recreateSwapChain();
		void cleanupSwapChain();
		//sync

		int mCurrentFrame{ 0 };
		std::vector<VulkanDriver::Semaphore::Ptr> mImageAvailableSemaphores;
		std::vector<VulkanDriver::Semaphore::Ptr> mRenderFinishedSemaphores;
		std::vector<VulkanDriver::Fence::Ptr> mFences;
	};
}

#endif // !APPLICATION_H


