#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>

#include <optional>


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

const std::vector<Vertex> vertices = {
	//Cube top face
	{{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.5f,  0.5f, 0.f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f,  0.5f, 0.f}, {1.0f, 1.0f, 0.0f}},

	//Cube back face
	{{-0.5f, -0.5f, -1.f}, {1.0f, 0.0f, 1.0f}},
	{{-0.5f,  0.5f, -1.f}, {0.0f, 1.0f, 1.0f}},

	//Cube front face
	{{0.5f, -0.5f, -1.f}, {1.0f, 1.0f, 1.0f}},
	{{0.5f,  0.5f, -1.f}, {0.0f, 0.0f, 0.0f}},

	//Cube left face
	//{{0.5f,  0.5f, -1.f}, {0.0f, 0.0f, 1.0f}},
	//{{0.5f, -0.5f, -1.f}, {0.0f, 0.0f, 1.0f}}

};

const std::vector<uint16_t> indices = {
	0, 1, 2, 0, 2, 3, //top
	3, 4, 0, 3, 5, 4, //back
	1, 6, 7, 1, 7, 2, //front
	0, 6, 1, 0, 4, 6, //left
	2, 7, 5, 2, 5, 3, //right
	6, 4, 5, 6, 5, 7  //bottom
};

constexpr unsigned int FRAME_OVERLAP = 2;
struct VkFrameData
{
	VkBuffer vkUniformBuffers;
	VkDeviceMemory vkUniformBuffersMemory;
	void* vkUniformBuffersMapped;

	VkDescriptorSet vkDescriptorSet;

	VkCommandPool vkCommandPool;
	VkCommandBuffer vkMainCommandBuffer;
	VkSemaphore vkImageAvailableSemaphore;
	VkSemaphore vkRenderFinishedSemaphore;
	VkFence vkInFlightFence;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

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
	QueueFamilyIndices vkQueueFamilyIndicies;
	VkQueue vkGraphicsQueue;
	VkQueue vkPresentQueue;
	VkQueue vkTransferQueue;
	VkSurfaceKHR vkSurface;

	VkFormat vkSwapChainImageFormat;
	VkExtent2D vkSwapChainExtent;
	VkSwapchainKHR vkSwapChain;
	std::vector<VkImage> vkSwapChainImages;
	std::vector<VkImageView> vkSwapChainImageViews;
	std::vector<VkFramebuffer> vkSwapChainFramebuffers;
	
	VkDescriptorPool vkDescriptorPool;
	VkDescriptorSetLayout vkDescriptorSetLayout;
	VkPipeline vkGraphicsPipeline;
	VkRenderPass vkRenderPass; 
	VkPipelineLayout vkPipelineLayout;

	VkCommandPool vkTransferCommandPool;

	VkBuffer vkVertexBuffer;
	VkDeviceMemory vkVertexBufferMemory;
	VkBuffer vkIndexBuffer;
	VkDeviceMemory vkIndexBufferMemory;


	//VkCommandBuffer vkCommandBuffer;
	//VkSemaphore vkImageAvailableSemaphore;
	//VkSemaphore vkRenderFinishedSemaphore;
	//VkFence vkInFlightFence;

	VkFrameData vkFrames[FRAME_OVERLAP];
	uint32_t currentFrame = 0;

	std::vector<VkCommandBuffer> vkCommandBuffers;
	std::vector<VkSemaphore> vkImageAvailableSemaphores;
	std::vector<VkSemaphore> vkRenderFinishedSemaphores;
	std::vector<VkFence> vkInFlightFences;

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
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandStructure();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();

	void createSyncObjects();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void updateUniformBuffer(uint32_t currentImage);

	VkShaderModule createShaderModule(const std::vector<char>& byteCode);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	bool checkValidationLayerSupport();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	
};
//< intro