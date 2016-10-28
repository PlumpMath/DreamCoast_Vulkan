#pragma once

// ����׿� Ȯ�� �ݹ��� ����� ���� �Լ�

extern VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

extern void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

// ������ ������, ���� ������ ���� �����Ѵ�.
template <typename T>
class VDeleter {
public:
	VDeleter() : VDeleter([](T, VkAllocationCallbacks*) {}) {}

	VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef)
	{
		this->deleter = [=](T obj) { deletef(obj, nullptr); };
	}

	VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef)
	{
		this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
	}

	VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
	{
		this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
	}

	~VDeleter()
	{
		cleanup();
	}

	const T* operator &() const
	{
		return &object;
	}

	T* replace()
	{
		cleanup();
		return &object;
	}

	operator T() const
	{
		return object;
	}

	void operator=(T rhs)
	{
		if (rhs != object) {
			cleanup();
			object = rhs;
		}
	}

	template<typename V>
	bool operator==(V rhs)
	{
		return object == T(rhs);
	}

private:
	T object{ VK_NULL_HANDLE };
	std::function<void(T)> deleter;

	void cleanup() {
		if (object != VK_NULL_HANDLE) {
			deleter(object);
		}
		object = VK_NULL_HANDLE;
	}
};

// ��ĭ������ ��� ����� ť�� �̷����� �̷� �ٸ� Ÿ�Ե��� ť�� ������ ť�йи��鿡 ����ֽ��ϴ�.
struct QueueFamilyIndices
{
	int graphicFamily = -1;
	int presentFamily = -1;

	bool isComplete()
	{
		return graphicFamily >= 0 && presentFamily >= 0;
	}
};

// ����ü�� ������ ������ ���ִ��� Ȯ���ϱ� ���� ����ü
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Renderer
{
public:
	virtual void InitRenderer(GLFWwindow* InWindow) {};
	virtual void DrawFrame() {};
	virtual void WaitIdle() {};
	virtual void RecreateSwapChain() {};
};

class VulkanRenderer : public Renderer
{
public:
	VulkanRenderer();
	virtual ~VulkanRenderer();
	virtual void InitRenderer(GLFWwindow* InWindow) override;
	virtual void DrawFrame();
	virtual void WaitIdle();
	virtual void RecreateSwapChain();

private:
	//creators
	bool CheckValidationLayerSupport();
	void CheckAvailableExtension();
	std::vector<const char*> GetRequiredExtensions();
	void CreateInstance();
	
	void SetupDebugCallback();

	void CreateSurface(GLFWwindow* InWindow);

	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeLine();
	void CreateFrameBuffer();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateSemaphore();

	// helpers
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
		uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	int RateDeviceSuitability(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);

private:
	VDeleter<VkInstance> instance{ vkDestroyInstance }; // ��ĭ �ν�ź��
	VDeleter<VkDebugReportCallbackEXT> callback{ instance, DestroyDebugReportCallbackEXT }; // ��ĭ �ݹ� Ȯ���Լ�
	VDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR }; // �����̽�, ��µ� �̹���

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // ���� ����̽� VkInstance�� �μ����� ���� �μ�����
	VDeleter<VkDevice> device{ vkDestroyDevice };  //��ĭ ������ ����̽�
	
	VkQueue graphicQueue; // �׷��� ť ���� ��ĭ �ν�ź���� �ı��ɶ� ���� �ı��ȴ�.
	VkQueue presentQueue; // ������Ʈ ť

	VDeleter<VkSwapchainKHR> swapChain{ device, vkDestroySwapchainKHR };
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat; 
	VkExtent2D swapChainExtent;

	std::vector<VDeleter<VkImageView>> swapChainImageViews; // VkImage�ʹ� �ٸ��� ���� ������� �Ѵ�.

	VDeleter<VkShaderModule> vertShaderModule{ device, vkDestroyShaderModule };
	VDeleter<VkShaderModule> fragShaderModule{ device, vkDestroyShaderModule };

	VDeleter<VkPipelineLayout> pipelineLayout{ device, vkDestroyPipelineLayout };
	VDeleter<VkRenderPass> renderPass{device, vkDestroyRenderPass};

	VDeleter<VkPipeline> graphicsPipeline{ device, vkDestroyPipeline };

	std::vector<VDeleter<VkFramebuffer>> swapChainFrameBuffer;

	VDeleter<VkCommandPool> commandPool{ device, vkDestroyCommandPool };
	std::vector<VkCommandBuffer> commandBuffers;

	VDeleter<VkSemaphore> imageAvailableSemaphore{ device, vkDestroySemaphore };
	VDeleter<VkSemaphore> renderFinishedSemaphore{ device, vkDestroySemaphore };
};