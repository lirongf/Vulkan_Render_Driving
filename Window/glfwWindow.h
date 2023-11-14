#ifndef  GLFWWindow_H
#define GLFWWindow_H

#include "../base.h"


namespace mainNameSpace {
	class Application;
}

namespace wind {

	class glfwWindow
	{
	public:
		using Ptr = std::shared_ptr<glfwWindow>;
		static glfwWindow::Ptr create(int& width,int& height) {
			return std::make_shared<glfwWindow>(width, height);
		}

	public:
		glfwWindow(int& width,int& height);
		
		~glfwWindow();

		bool shouldClose();

		void pollEvents();

		void processEvent();

		[[nodiscard]] GLFWwindow* getWindow() const { return mWindow; };

		void setApp(mainNameSpace::Application* app) { mApp = app; };

		void onWindowMouseMove(double xpos, double ypos);

		std::vector<const char*> getRequiredExtensions();
	public:
		bool mWindowResized{ false };
		mainNameSpace::Application* mApp;
		std::function<void(double xpos, double ypos)> onMouseMove;
		std::function<void(int keyIndex)> onKeyDown;
		std::function<void(int button, int action, int mods)> onMouseButton;
	private:
		int mWidth{ 0 };
		int mHeight{ 0 };
		GLFWwindow* mWindow{ NULL };
	};
}

#endif // ! GLFWWindow_H