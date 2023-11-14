#include "glfwWindow.h"
#ifndef  GLFWWindow_CPP
#define GLFWWindow_CPP

#include "glfwWindow.h"

namespace wind {

	//重新改变Resize的大小
	static void windowResized(GLFWwindow* window, int width, int height) {
		glfwWindow* pUserData = reinterpret_cast<glfwWindow*>(glfwGetWindowUserPointer(window));
		pUserData->mWindowResized = true;
	}

	//获得Camera的Pos位置
	static void cursorPosCallBack(GLFWwindow* window, double xpos, double ypos) {
		glfwWindow* pUserData = reinterpret_cast<glfwWindow*>(glfwGetWindowUserPointer(window));
		pUserData->onWindowMouseMove(xpos, ypos);
	}

	static void mouseCallBack(GLFWwindow* window, int button, int action, int mods) {
		glfwWindow* pUserData = reinterpret_cast<glfwWindow*>(glfwGetWindowUserPointer(window));
		pUserData->onMouseButton(button,action, mods);
	}

	glfwWindow::glfwWindow(int& width,int& height)
	{
		mWidth = width;
		mHeight = height;
		//初始化glfw
		glfwInit();
		//如果不是OpenGL  就设置No，因为我们这个是Vulkan
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//设置glfwWindow可以改变大小
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		mWindow = glfwCreateWindow(mWidth, mHeight, "vulkan window", nullptr, nullptr);
		
		if (!mWindow) {
			std::cerr << "Error: failed to create window" << std::endl;
		}
		glfwSetWindowUserPointer(mWindow, this);
		//设置改变窗口大小的回调函数
		glfwSetFramebufferSizeCallback(mWindow, windowResized);
		//设置光标位置的回调函数
		glfwSetCursorPosCallback(mWindow, cursorPosCallBack);

		glfwSetMouseButtonCallback(mWindow, mouseCallBack);
		//隐藏光标
		//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	std::vector<const char*> glfwWindow::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;

		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return extensions;
	}

	//release glfwWindow
	glfwWindow::~glfwWindow()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	//判断窗口是否关闭
	bool glfwWindow::shouldClose() {
		return glfwWindowShouldClose(mWindow);
	}

	//接受窗口事件
	void glfwWindow::pollEvents() {
		glfwPollEvents();
	}

	//处理窗口事件
	void glfwWindow::processEvent()
	{
		/*if (mApp.expired()) {
			return;
		}*/

		//auto app = mApp.lock();//锁住不删除

		//如果是esc  退出
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			exit(0);
		}
		//处理Key W
		if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS) {
			onKeyDown(GLFW_KEY_W);
		}

		if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS) {
			onKeyDown(GLFW_KEY_S);
		}

		if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS) {
			onKeyDown(GLFW_KEY_A);
		}

		if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS) {
			onKeyDown(GLFW_KEY_D);
		}

		if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
		{
			onKeyDown(GLFW_KEY_E);
		}

		if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS)
		{
			onKeyDown(GLFW_KEY_Q);
		}
	}

	void glfwWindow::onWindowMouseMove(double xpos, double ypos)
	{
		onMouseMove(xpos, ypos);
	}

	

	

}
#endif // ! GLFWWindow_CPP
