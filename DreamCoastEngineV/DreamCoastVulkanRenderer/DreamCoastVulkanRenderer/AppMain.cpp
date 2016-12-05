#include "DcCommonHeader.h"
#include "SystemSetting.h"
#include "Renderer.h"
#include "Geometry.h"

VulkanRenderer* GRenderer = nullptr;

class DreamCoastVulkan_Triangle
{
public:
	~DreamCoastVulkan_Triangle()
	{
		if (Geom)
		{
			delete Geom;
		}
	}
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
	}
private:
	void initWindow()
	{
#ifdef GLFW_INCLUDE_VULKAN
		glfwInit();// glfw �ʱ�ȭ

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		// ���� �����츦 ����� �Լ�
		// ����, ����, ������ Ÿ��Ʋ, ����� �ж����(����GL����)
		window = glfwCreateWindow(SystemSetting::WIDTH, SystemSetting::HEIGHT, "DreamCoastVulkan", nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetWindowSizeCallback(window, DreamCoastVulkan_Triangle::OnWindowResized);
#endif
	}

	void initVulkan()
	{
		VulkanRenderer* vulkanRenderer = new VulkanRenderer;
		vulkanRenderer->InitRenderer(window);
		renderer = vulkanRenderer;
#ifdef GLFW_INCLUDE_VULKAN
		GRenderer = vulkanRenderer;
#endif
		Geom = new Geometry();
		Geom->SetVertexBuffer();
		renderer->RecreateSwapChain();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{			
			glfwPollEvents();
			// Game Frame
			//
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
	Geometry* Geom = nullptr;
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