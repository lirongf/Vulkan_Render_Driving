#include "glfwWindow.h"
#ifndef  GLFWWindow_CPP
#define GLFWWindow_CPP

#include "glfwWindow.h"

namespace wind {

	//���¸ı�Resize�Ĵ�С
	static void windowResized(GLFWwindow* window, int width, int height) {
		glfwWindow* pUserData = reinterpret_cast<glfwWindow*>(glfwGetWindowUserPointer(window));
		pUserData->mWindowResized = true;
	}

	//���Camera��Posλ��
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
		//��ʼ��glfw
		glfwInit();
		//�������OpenGL  ������No����Ϊ���������Vulkan
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//����glfwWindow���Ըı��С
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		mWindow = glfwCreateWindow(mWidth, mHeight, "vulkan window", nullptr, nullptr);
		
		if (!mWindow) {
			std::cerr << "Error: failed to create window" << std::endl;
		}
		glfwSetWindowUserPointer(mWindow, this);
		//���øı䴰�ڴ�С�Ļص�����
		glfwSetFramebufferSizeCallback(mWindow, windowResized);
		//���ù��λ�õĻص�����
		glfwSetCursorPosCallback(mWindow, cursorPosCallBack);

		glfwSetMouseButtonCallback(mWindow, mouseCallBack);
		//���ع��
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

	//�жϴ����Ƿ�ر�
	bool glfwWindow::shouldClose() {
		return glfwWindowShouldClose(mWindow);
	}

	//���ܴ����¼�
	void glfwWindow::pollEvents() {
		glfwPollEvents();
	}

	//�������¼�
	void glfwWindow::processEvent()
	{
		/*if (mApp.expired()) {
			return;
		}*/

		//auto app = mApp.lock();//��ס��ɾ��

		//�����esc  �˳�
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			exit(0);
		}
		//����Key W
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
