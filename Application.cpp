#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include "Application.h"
namespace mainNameSpace {
	static std::vector<char> readBinary(const std::string& fileName) {
		std::ifstream file(fileName.c_str(), std::ios::ate | std::ios::binary | std::ios::in);

		if (!file) {
			throw std::runtime_error("Error: failed to open shader file");
		}

		const size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}


	static void createWindowSurface(wind::glfwWindow::Ptr window, VulkanDriver::Instance::Ptr instnace, VkSurfaceKHR &surface) {
		if (glfwCreateWindowSurface(instnace->getInstance(), window->getWindow(), nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create surface");
		}
	}

	std::vector<char> Application::readBinary(const std::string& fileName) {
		std::ifstream file(fileName.c_str(), std::ios::ate | std::ios::binary | std::ios::in);

		if (!file) {
			throw std::runtime_error("Error: failed to open shader file");
		}

		const size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void Application::run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanUp();
	}

	void Application::onMouseMove(double xpos, double ypos)
	{
		mCamera.onMouseMove(xpos, ypos);
	}

	void Application::onMouseButton(int button, int action, int mods) {
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			if (action == GLFW_PRESS)
				std::cout <<"按下左键" << std::endl;
			else if(action == GLFW_RELEASE){
				std::cout << "释放左键" << std::endl;
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (action == GLFW_PRESS) {
				mCamera.canMove = true;
				mCamera.m_firstMove = true;
			}
			else if (action == GLFW_RELEASE) {
				mCamera.canMove = false;
			}
			break;
		default:
			break;
		}
	}

	void Application::onKeyDown(int keyIndex)
	{
		switch (keyIndex)
		{
		case GLFW_KEY_W:
			if(mCamera.canMove)
				mCamera.move(CAMERA_MOVE::MOVE_FRONT);
			break;
		case GLFW_KEY_S:
			if (mCamera.canMove)
			mCamera.move(CAMERA_MOVE::MOVE_BACK);
			break;
		case GLFW_KEY_A:
			if (mCamera.canMove)
			mCamera.move(CAMERA_MOVE::MOVE_LEFT);
			break;
		case GLFW_KEY_D:
			if (mCamera.canMove)
			mCamera.move(CAMERA_MOVE::MOVE_RIGHT);
			break;
		default:
			break;
		}
	}

	void Application::initWindow() {
		mWindow = wind::glfwWindow::create(mWidth, mHeight);
		mWindow->setApp(this);
		mWindow->onMouseMove = std::bind(&Application::onMouseMove, this, std::placeholders::_1, std::placeholders::_2);
		mWindow->onKeyDown = std::bind(&Application::onKeyDown, this, std::placeholders::_1);
		mWindow->onMouseButton = std::bind(&Application::onMouseButton, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//CameraTODO
		mCamera.lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mCamera.update();

		mCamera.setPerpective(45.0f, (float)mWidth / (float)mHeight, 0.1f,1000.0f);

		mCamera.setSpeed(0.05f);
	}

	void Application::initVulkan() {
		//vkInstance
		mVkInstance = VulkanDriver::Instance::create(true, mWindow->getRequiredExtensions());
		//vkSurface
		mSurface = VulkanDriver::Surface::create(mVkInstance);
		VkSurfaceKHR* mm = mSurface->getSurface();
		createWindowSurface(mWindow, mVkInstance, *mm);
		//device
		mDevice = VulkanDriver::Device::create(mVkInstance, mSurface);
		//swapChain
		glfwGetFramebufferSize(mWindow->getWindow(), &mWidth, &mHeight);
		mSwapChain = VulkanDriver::SwapChain::create(mDevice, mSurface, mWidth, mHeight);

		//TODO 在哪里创建 这个CommanBuffer是否要搞成转为转换数据而生的mCommandPool
		mCommandPool = VulkanDriver::CommandPool::create(mDevice);

		//创建RenderPass=============临时方案TODO===============
		mRenderPass = VulkanDriverFactory::RenderPassContext::createBlitRenderPass(mDevice, mSwapChain);
		//创建ShaderData
		//1、加载纹理  2、设置UniformBufferMap，设置纹理和数据
		createImageByPath();
		createMaterialShaderData();
		createCameraShaderData();
		// 创建RenderGeometry，创建RenderBuffer
		//1、加载模型，创建BufferState
		loadModel();
		//loadplane();
		//创建PipeLine
		createPipeline();
		//创建CommandBuffer
		createCommandBuffer();
		//异步调控
		createSyncObjects();
	}

	void Application::render() {
		mFences[mCurrentFrame]->waiteFence();
		uint32_t imageIndex{ 0 };

		VkResult result = vkAcquireNextImageKHR(
			mDevice->getDevice(),
			mSwapChain->getSwapChain(),
			1000,
			mImageAvailableSemaphores[mCurrentFrame]->getSemaphore(),
			VK_NULL_HANDLE,
			&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			mWindow->mWindowResized = false;
			mCurrentFrame = 0;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Error: failed to acquire next image");
		}
		
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { mImageAvailableSemaphores[mCurrentFrame]->getSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		auto commandBuffer = mCommandBuffers[imageIndex]->getCommandBuffer();
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphores[mCurrentFrame]->getSemaphore() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		
		mFences[mCurrentFrame]->resetFence();
		if (vkQueueSubmit(mDevice->getGraphicQueue(), 1, &submitInfo, mFences[mCurrentFrame]->getFence()) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to submit renderCommand");
		}
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { mSwapChain->getSwapChain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(mDevice->getPresentQueue(), &presentInfo);

		mCurrentFrame = (mCurrentFrame + 1) % mSwapChain->getImageCount();
	}

	void Application::mainLoop() {
		while (!mWindow->shouldClose())
		{
			mWindow->pollEvents();
			mWindow->processEvent();
			
			cameraShaderData->setMatrix4x4(3, mCamera.getViewMatrix());
			cameraShaderData->setMatrix4x4(4, mCamera.getProjectMatrix());
			cameraShaderData->uploadData(mCurrentFrame);
			render();
		}
		//要在主机上等待完成给定逻辑设备上所有队列的未完成队列操作
		vkDeviceWaitIdle(mDevice->getDevice());
		//vkQueueWaitIdle
	}

	void Application::cleanUp() {
		mSwapChain.reset();
		mDevice.reset();
		mSurface.reset();
		mVkInstance.reset();
		mWindow.reset();
	}

	void Application::cleanupSwapChain() {
		mSwapChain.reset();
		mCommandBuffers.clear();
		mPipeline.reset();
		mRenderPass.reset();
		mImageAvailableSemaphores.clear();
		mRenderFinishedSemaphores.clear();
		mFences.clear();
	}

	void Application::recreateSwapChain(){
		int width = 0, height = 0;
		glfwGetFramebufferSize(mWindow->getWindow(), &mWidth, &mHeight);
		while (width == 0 || height == 0) {
			glfwWaitEvents();
			glfwGetFramebufferSize(mWindow->getWindow(), &width, &height);
		}

		vkDeviceWaitIdle(mDevice->getDevice());

		cleanupSwapChain();
		mSwapChain = VulkanDriver::SwapChain::create(mDevice, mSurface, mWidth, mHeight);
		mRenderPass = VulkanDriverFactory::RenderPassContext::createBlitRenderPass(mDevice, mSwapChain);
		createPipeline();
		

		//mCommandBuffers.resize(mSwapChain->getImageCount());
		//创建CommandBuffer
		createCommandBuffer();
		//异步调控
		createSyncObjects();
	}

	void Application::createSyncObjects() {
		for (uint32_t i = 0; i < mSwapChain->getImageCount(); ++i) {
			VulkanDriver::Semaphore::Ptr imageSemaphore = VulkanDriver::Semaphore::create(mDevice);
			mImageAvailableSemaphores.push_back(imageSemaphore);

			auto renderSemaphore = VulkanDriver::Semaphore::create(mDevice);
			mRenderFinishedSemaphores.push_back(renderSemaphore);

			auto fence = VulkanDriver::Fence::create(mDevice);
			mFences.push_back(fence);
		}
	}
	

	void Application::createImageByPath() {
		int texWidth, texHeight, texSize, texChannles;
		const std::string& imageFilePath = "D:/a_gitPrograms/vulkanTest/Vulkan_Render_Driving/build/Debug/assets/car.jpg";
		stbi_uc* pixels = stbi_load(imageFilePath.c_str(), &texWidth, &texHeight, &texChannles, STBI_rgb_alpha);

		if (!pixels) {
			throw std::runtime_error("Error: failed to read image data");
		}
		texSize = texWidth * texHeight * 4;
		mImage = VulkanDriver::Image::create(mDevice, texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB
			, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_SAMPLE_COUNT_1_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);


		VkImageSubresourceRange region{};
		region.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.baseArrayLayer = 0;
		region.layerCount = 1;

		region.baseMipLevel = 0;
		region.levelCount = 1;
		
		VulkanDriver::MemoryUtil::setImageLayout(mDevice, mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			region,
			mCommandPool);
		VulkanDriver::MemoryUtil::fillImageData(mDevice,mImage,texSize, (void*)pixels, mCommandPool);
		
		VulkanDriver::MemoryUtil::setImageLayout(mDevice, mImage,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			region,
			mCommandPool
		);
		stbi_image_free(pixels);

		mSampler = VulkanDriver::Sampler::create(mDevice);
	}

	void Application::createMaterialShaderData() {
		materialShaderData = VulkanDriverFactory::ShaderData::create(1);
		//给一个2个Color，给一个图片

		std::vector<VulkanDriverFactory::UniformProperty> uboMap;
		VulkanDriverFactory::UniformProperty color1;
		color1.id = 0;
		color1.propertyName = "color1";
		color1.uniformType = VulkanDriverFactory::ShaderDataType::Vector4;
		uboMap.push_back(color1);

		VulkanDriverFactory::UniformProperty color2;
		color2.id = 1;
		color2.propertyName = "color2";
		color2.uniformType = VulkanDriverFactory::ShaderDataType::Vector4;
		uboMap.push_back(color2);

		VulkanDriverFactory::UniformProperty texture00;
		texture00.id = 2;
		texture00.propertyName = "texture00";
		texture00.uniformType = VulkanDriverFactory::ShaderDataType::Texture2D;
		uboMap.push_back(texture00);

		materialShaderData->setUniformMap(uboMap,mDevice);//设置shaderData范围
		
		materialShaderData->setVector4(0, glm::vec4(1.0, 1.0, 0.0, 1.0));
		materialShaderData->setVector4(1, glm::vec4(0.0, 1.0, 1.0, 1.0));
		materialShaderData->setTexture2D(2, mImage, mSampler);
		materialShaderData->uploadData(0);
		
	}
	
	void Application::createCameraShaderData() {
		cameraShaderData = VulkanDriverFactory::ShaderData::create(0);
		std::vector<VulkanDriverFactory::UniformProperty> uboMap;
		VulkanDriverFactory::UniformProperty ViewMat;
		ViewMat.id = 3;
		ViewMat.propertyName = "ViewMat";
		ViewMat.uniformType = VulkanDriverFactory::ShaderDataType::Matrix4x4;
		uboMap.push_back(ViewMat);

		VulkanDriverFactory::UniformProperty ProjectMat;
		ProjectMat.id = 4;
		ProjectMat.propertyName = "ProjectMat";
		ProjectMat.uniformType = VulkanDriverFactory::ShaderDataType::Matrix4x4;
		uboMap.push_back(ProjectMat);
		cameraShaderData->setUniformMap(uboMap,mDevice);
		cameraShaderData->setMatrix4x4(3, mCamera.getViewMatrix());
		cameraShaderData->setMatrix4x4(4, mCamera.getProjectMatrix());

		cameraShaderData->uploadData(0);
	}

	void Application::loadModel() {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		std::string warn;
		std::vector<float> mPositions{};
		std::vector<float> mUVs{};
		std::vector<uint32_t> mIndexDatas{};
		const std::string& path = "D:/a_gitPrograms/vulkanTest/Vulkan_Render_Driving/build/Debug/assets/car.obj";
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
			throw std::runtime_error("Error: failed to load model");
		}

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				mPositions.push_back(attrib.vertices[3 * index.vertex_index + 0]);
				mPositions.push_back(attrib.vertices[3 * index.vertex_index + 1]);
				mPositions.push_back(attrib.vertices[3 * index.vertex_index + 2]);

				mUVs.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
				mUVs.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
				mIndexDatas.push_back((uint32_t)mIndexDatas.size());
			}
		}
		VulkanDriver::Buffer::Ptr mPositionBuffer = VulkanDriver::Buffer::createVertexBuffer(mDevice, mPositions.size() * sizeof(float), mPositions.data());

		VulkanDriver::Buffer::Ptr mUVBuffer = VulkanDriver::Buffer::createVertexBuffer(mDevice, mUVs.size() * sizeof(float), mUVs.data());

		VulkanDriver::Buffer::Ptr mIndexBuffer = VulkanDriver::Buffer::createIndexBuffer(mDevice, mIndexDatas.size() * sizeof(float), mIndexDatas.data());
		
		//create VertexDeclaration
		mBufferState = VulkanDriverFactory::BufferState::create();
		//Pos
		VulkanDriverFactory::VertexAttributeElement pos{};
		pos.format = VulkanDriverFactory::VertexElementFormat::vector3;
		pos.location = 0;
		pos.offset = 0;
		VulkanDriverFactory::VertexDeclaration posDeclaration;
		posDeclaration.isntance = false;
		posDeclaration.vertexStride = 3;
		posDeclaration.vertexElements.push_back(pos);

		//UV
		VulkanDriverFactory::VertexAttributeElement uv{};
		uv.format = VulkanDriverFactory::VertexElementFormat::vector2;
		uv.location = 1;
		uv.offset = 0;
		VulkanDriverFactory::VertexDeclaration uvDeclaration;
		uvDeclaration.isntance = false;
		uvDeclaration.vertexStride = 2;
		uvDeclaration.vertexElements.push_back(uv);

		std::vector<VulkanDriverFactory::VertexDeclaration> declarations;
		declarations.push_back(posDeclaration);
		declarations.push_back(uvDeclaration);
		mBufferState->setVertexDeclarations(declarations);

		std::vector<VulkanDriver::Buffer::Ptr> vertexBuffers;
		vertexBuffers.push_back(mPositionBuffer);
		vertexBuffers.push_back(mUVBuffer);
		mBufferState->setVertexBuffers(vertexBuffers);
		mBufferState->setIndexBuffer(mIndexBuffer);
	}

	//primitive plane
	void Application::loadplane() {
		std::vector<float> mPositions{};
		std::vector<float> mUVs{};
		std::vector<uint32_t> mIndexDatas{};
		int wid = 1;
		mPositions.push_back(-wid); mPositions.push_back(-wid); mPositions.push_back(0);
		mPositions.push_back(wid); mPositions.push_back(-wid); mPositions.push_back(0);
		mPositions.push_back(-wid); mPositions.push_back(wid); mPositions.push_back(0);
		mPositions.push_back(wid); mPositions.push_back(wid); mPositions.push_back(0);

		mUVs.push_back(0); mUVs.push_back(0);
		mUVs.push_back(1); mUVs.push_back(0);
		mUVs.push_back(0); mUVs.push_back(1);
		mUVs.push_back(1); mUVs.push_back(1);

		mIndexDatas.push_back(0);
		mIndexDatas.push_back(1);
		mIndexDatas.push_back(3);
		mIndexDatas.push_back(0);
		mIndexDatas.push_back(3);
		mIndexDatas.push_back(2);
		

		VulkanDriver::Buffer::Ptr mPositionBuffer = VulkanDriver::Buffer::createVertexBuffer(mDevice, mPositions.size() * sizeof(float), mPositions.data());

		VulkanDriver::Buffer::Ptr mUVBuffer = VulkanDriver::Buffer::createVertexBuffer(mDevice, mUVs.size() * sizeof(float), mUVs.data());

		VulkanDriver::Buffer::Ptr mIndexBuffer = VulkanDriver::Buffer::createIndexBuffer(mDevice, mIndexDatas.size() * sizeof(uint32_t), mIndexDatas.data());

		//create VertexDeclaration
		mBufferState = VulkanDriverFactory::BufferState::create();
		//Pos
		VulkanDriverFactory::VertexAttributeElement pos{};
		pos.format = VulkanDriverFactory::VertexElementFormat::vector3;
		pos.location = 0;
		pos.offset = 0;
		VulkanDriverFactory::VertexDeclaration posDeclaration;
		posDeclaration.isntance = false;
		posDeclaration.vertexStride = 3;
		posDeclaration.vertexElements.push_back(pos);

		//UV
		VulkanDriverFactory::VertexAttributeElement uv{};
		uv.format = VulkanDriverFactory::VertexElementFormat::vector2;
		uv.location = 1;
		uv.offset = 0;
		VulkanDriverFactory::VertexDeclaration uvDeclaration;
		uvDeclaration.isntance = false;
		uvDeclaration.vertexStride = 2;
		uvDeclaration.vertexElements.push_back(uv);

		std::vector<VulkanDriverFactory::VertexDeclaration> declarations;
		declarations.push_back(posDeclaration);
		declarations.push_back(uvDeclaration);
		mBufferState->setVertexDeclarations(declarations);

		std::vector<VulkanDriver::Buffer::Ptr> vertexBuffers;
		vertexBuffers.push_back(mPositionBuffer);
		vertexBuffers.push_back(mUVBuffer);
		mBufferState->setVertexBuffers(vertexBuffers);
		mBufferState->setIndexBuffer(mIndexBuffer);
	}

	void Application::createCommandBuffer() {
		mCommandBuffers.resize(mSwapChain->getImageCount());
		for (uint32_t i = 0; i < mSwapChain->getImageCount(); ++i) {
			mCommandBuffers[i] = VulkanDriver::CommandBuffer::create(mDevice, mCommandPool);

			mCommandBuffers[i]->begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, {});

			VkRenderPassBeginInfo renderBeginInfo{};
			renderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderBeginInfo.renderPass = mRenderPass->getRenderPass();
			renderBeginInfo.framebuffer = mSwapChain->getFrameBuffer(i)->getFrameBuffer(mRenderPass);
			renderBeginInfo.renderArea.offset = { 0, 0 };
			renderBeginInfo.renderArea.extent = mSwapChain->getExtent();

			
			std::vector< VkClearValue> clearColors{};
			VkClearValue finalClearColor{};
			finalClearColor.color = { 1.0f, 1.0f, 0.0f, 1.0f };
			clearColors.push_back(finalClearColor);

			VkClearValue mutiClearColor{};
			mutiClearColor.color = { 0.0f, 0.0f,1.0f, 1.0f };
			clearColors.push_back(mutiClearColor);

			VkClearValue depthClearColor{};
			depthClearColor.depthStencil = { 1.0f, 0 };
			clearColors.push_back(depthClearColor);

			renderBeginInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
			renderBeginInfo.pClearValues = clearColors.data();


			mCommandBuffers[i]->beginRenderPass(renderBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			mCommandBuffers[i]->bindGraphicPipeline(mPipeline->getPipeline());

			 VkDescriptorSet set= cameraShaderData->getDescriptorSet()->getDescriptorSet(i);
			mCommandBuffers[i]->bindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,mPipeline->getLayout(),
				0,1,&set);

			set = materialShaderData->getDescriptorSet()->getDescriptorSet(i);
			mCommandBuffers[i]->bindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline->getLayout(),
				1, 1, &set);

			std::vector<VkBuffer> buffers;
			std::vector<VkDeviceSize> bufferOffsets;
			for (const VulkanDriver::Buffer::Ptr buffer : mBufferState->getVertexBuffers()) {
				buffers.push_back(buffer->getBuffer());
				bufferOffsets.push_back(0);
			}
			mCommandBuffers[i]->bindVertexBuffer(buffers,0, bufferOffsets);

			mCommandBuffers[i]->bindIndexBuffer(mBufferState->getIndexBuffer()->getBuffer(),0, VK_INDEX_TYPE_UINT32);

			mCommandBuffers[i]->drawIndex((uint32_t)(mBufferState->getIndexBuffer()->getBufferbyteSize()/4),1,0,0,0);
		

			mCommandBuffers[i]->endRenderPass();

			mCommandBuffers[i]->end();
		}
	}

	void Application::createPipeline() {
		mPipeline = VulkanDriver::Pipeline::create(mDevice, mRenderPass);
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = (float)mHeight;
		viewport.width = (float)mWidth;
		viewport.height = -(float)mHeight;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { (uint32_t)mWidth, (uint32_t)mHeight };
		std::vector<VkViewport> viewports;
		viewports.push_back(viewport);


		mPipeline->setViewports( viewports );
		mPipeline->setScissors({ scissor });

		std::vector<char> vertexShaderCode = Application::readBinary("D:/a_gitPrograms/vulkanTest/Vulkan_Render_Driving/build/Debug/assets/vs.spv");
		std::vector<char> fragmentShaderCode = Application::readBinary("D:/a_gitPrograms/vulkanTest/Vulkan_Render_Driving/build/Debug/assets/fs.spv");
		VulkanDriver::Shader::Ptr vertexShader = VulkanDriver::Shader::create(mDevice
			, vertexShaderCode, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, "main");
		VulkanDriver::Shader::Ptr fragShader = VulkanDriver::Shader::create(mDevice, fragmentShaderCode,
			VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, "main");
		
		
		std::vector<VulkanDriver::Shader::Ptr> shaderGroup{};
		shaderGroup.push_back(vertexShader);
		shaderGroup.push_back(fragShader);
		mPipeline->setShaderGroup(shaderGroup);

		mPipeline->setVertexInputState(mBufferState->getVertexBindDescription(), mBufferState->getAttributeDescription());

		mPipeline->setInputAssemblyState(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
	
		mPipeline->setRasterizationStatge(VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, VK_FALSE);

		mPipeline->setMultisampleState(VK_FALSE, VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT);

		mPipeline->setDepthStencilState(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL, VK_FALSE, nullptr, nullptr);


		VkPipelineColorBlendAttachmentState blendAttachment{};
		blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;

		blendAttachment.blendEnable = VK_FALSE;
		blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

		blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		std::vector<VkPipelineColorBlendAttachmentState> blendAttachments;
		blendAttachments.push_back(blendAttachment);

		mPipeline->setColorBlendState(VK_FALSE, VK_LOGIC_OP_COPY, blendAttachments);

		std::vector<VulkanDriver::DescriptorSetLayout::Ptr> descriptorSetLayout;
		descriptorSetLayout.push_back(cameraShaderData->getDescriptorSetLayout());
		descriptorSetLayout.push_back(materialShaderData->getDescriptorSetLayout());

		mPipeline->setpipeLineLayout(descriptorSetLayout);
		mPipeline->build();
	}


}

#endif // !APPLICATION_CPP


