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
	glm::vec2 texCoord;


	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0.0f, 0.0f}}, // 0
	{{ 0.5f, -0.5f,  0.5f}, {0, 1, 0}, {1.0f, 0.0f}}, // 1
	{{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1.0f, 1.0f}}, // 2
	{{-0.5f,  0.5f,  0.5f}, {1, 1, 0}, {0.0f, 1.0f}}, // 3

	// Back face
	{{ 0.5f, -0.5f, -0.5f}, {1, 0, 1}, {0.0f, 0.0f}}, // 4
	{{-0.5f, -0.5f, -0.5f}, {0, 1, 1}, {1.0f, 0.0f}}, // 5
	{{-0.5f,  0.5f, -0.5f}, {1, 1, 1}, {1.0f, 1.0f}}, // 6
	{{ 0.5f,  0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}}, // 7

	// Left face
	{{-0.5f, -0.5f, -0.5f}, {1, 0, 1}, {0.0f, 0.0f}}, // 8
	{{-0.5f, -0.5f,  0.5f}, {0, 1, 1}, {1.0f, 0.0f}}, // 9
	{{-0.5f,  0.5f,  0.5f}, {1, 1, 1}, {1.0f, 1.0f}}, //10
	{{-0.5f,  0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}}, //11

	// Right face
	{{ 0.5f, -0.5f,  0.5f}, {1, 0, 1}, {0.0f, 0.0f}}, //12
	{{ 0.5f, -0.5f, -0.5f}, {0, 1, 1}, {1.0f, 0.0f}}, //13
	{{ 0.5f,  0.5f, -0.5f}, {1, 1, 1}, {1.0f, 1.0f}}, //14
	{{ 0.5f,  0.5f,  0.5f}, {0, 0, 0}, {0.0f, 1.0f}}, //15

	// Top face
	{{-0.5f,  0.5f,  0.5f}, {1, 0, 1}, {0.0f, 0.0f}}, //16
	{{ 0.5f,  0.5f,  0.5f}, {0, 1, 1}, {1.0f, 0.0f}}, //17
	{{ 0.5f,  0.5f, -0.5f}, {1, 1, 1}, {1.0f, 1.0f}}, //18
	{{-0.5f,  0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}}, //19

	// Bottom face
	{{-0.5f, -0.5f, -0.5f}, {1, 0, 1}, {0.0f, 0.0f}}, //20
	{{ 0.5f, -0.5f, -0.5f}, {0, 1, 1}, {1.0f, 0.0f}}, //21
	{{ 0.5f, -0.5f,  0.5f}, {1, 1, 1}, {1.0f, 1.0f}}, //22
	{{-0.5f, -0.5f,  0.5f}, {0, 0, 0}, {0.0f, 1.0f}}, //23

};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,       // Front face
	4, 5, 6, 6, 7, 4,       // Back face
	8, 9,10,10,11, 8,       // Left face
   12,13,14,14,15,12,       // Right face
   16,17,18,18,19,16,       // Top face
   20,21,22,22,23,20        // Bottom face
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
	VkBuffer vkStagingBuffer;
	VkDeviceMemory vkStagingBufferMemory;

	VkImage vkTextureImage;
	VkDeviceMemory vkTextureImageMemory;
	VkImageView vkTextureImageView;
	VkSampler vkTextureSampler;



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
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void createFramebuffers();
	void createCommandStructure();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();

	void createSyncObjects();

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

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