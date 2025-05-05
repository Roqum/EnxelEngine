#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct VkFrameData {

	VkCommandBuffer vkMainCommandBuffer;
};

constexpr unsigned int FRAME_OVERLAP = 2;

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanEngine {
public:
	//run main loop
	void run();

private:
	static VulkanEngine& Get();

	int windowHeight;
	int windowWidth;

	//Vulkan
	VkInstance vkInstance;
	VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
	VkDevice vkDevice;
	VkQueue vkGraphicsQueue;
	VkQueue vkPresentQueue;
	VkSurfaceKHR vkSurface;

	VkFormat vkSwapChainImageFormat;
	VkExtent2D vkSwapChainExtent;
	VkSwapchainKHR vkSwapChain;
	std::vector<VkImage> vkSwapChainImages;
	std::vector<VkImageView> vkSwapChainImageViews;
	std::vector<VkFramebuffer> vkSwapChainFramebuffers;
	VkCommandPool vkCommandPool;
	VkCommandBuffer vkCommandBuffer;


	
	VkPipeline vkGraphicsPipeline;
	VkRenderPass vkRenderPass;
	VkPipelineLayout vkPipelineLayout;

	//VkFrameData frames[FRAME_OVERLAP];

	//VkFrameData& getCurrentFrame() { return frames[frameNumber % FRAME_OVERLAP]; };

	const std::vector<const char*> requiredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	
	//Selected Validation Layer
	const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

	// SDL
	struct SDL_Window* window{ nullptr };
	bool isWindowInitialized{ false };
	int frameNumber {0};
	bool stop_rendering{ false };


	//initializes everything in the engine
	void init();
	void initWindow();
	

	//Vulkan
	void initVulkan();
	
	void createInstance();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffer();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	VkShaderModule createShaderModule(const std::vector<char>& byteCode);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	bool checkValidationLayerSupport();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	
};
//< intro