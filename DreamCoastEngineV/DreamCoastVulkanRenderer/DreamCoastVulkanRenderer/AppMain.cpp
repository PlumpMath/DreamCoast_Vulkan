#include "DcCommonHeader.h"
#include "SystemSetting.h"
#include "Renderer.h"

class DreamCoastVulkan_Triangle
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
	}
private:
	void initWindow()
	{
		glfwInit();// glfw �ʱ�ȭ

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		// ���� �����츦 ����� �Լ�
		// ����, ����, ������ Ÿ��Ʋ, ����� �ж����(����GL����)
		window = glfwCreateWindow(SystemSetting::WIDTH, SystemSetting::HEIGHT, "DreamCoastVulkan", nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetWindowSizeCallback(window, DreamCoastVulkan_Triangle::OnWindowResized);
	}

	void initVulkan()
	{
		VulkanRenderer* vulkanRenderer = new VulkanRenderer;
		vulkanRenderer->InitRenderer(window);
		renderer = vulkanRenderer;
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			renderer->DrawFrame();
		}
		renderer->WaitIdle();
	}

	static void OnWindowResized(GLFWwindow* window, int width, int height)
	{
		if (width == 0 || height == 0) return;
		DreamCoastVulkan_Triangle* app = reinterpret_cast<DreamCoastVulkan_Triangle*>(glfwGetWindowUserPointer(window));
		app->RendereResize();
	}

	void RendereResize()
	{
		renderer->RecreateSwapChain();
	}

private:
	GLFWwindow* window = nullptr;
	Renderer* renderer = nullptr;
};


int main()
{
	DreamCoastVulkan_Triangle app;
	try
	{
		app.run();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}