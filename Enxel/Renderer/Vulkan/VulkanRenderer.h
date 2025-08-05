#pragma once
/*
#include <vulkan/vulkan.h>
#include "IRenderer.h"
#include "glm/glm.hpp"

// TODO Replace STL with alternative (EASTL)
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <optional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

constexpr unsigned int FRAME_OVERLAP = 2;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

};

static VkVertexInputBindingDescription getBindingDescription() {
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}
static std::array < VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
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

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

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


class VulkanRenderer : public IRenderer
{
public:
	// Inherited via IRenderer
	void Init() override;
	void RenderFrame() override;
	void Shutdown() override;

private:
	void initWindow(); // TODO Move out of renderer

    createInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandStructure();
    createDepthResources();
    createFramebuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createSyncObjects();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);
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

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	int windowHeight;	
	int windowWidth;

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

	VkImage vkDepthImage;
	VkDeviceMemory vkDepthImageMemory;
	VkImageView vkDepthImageView;

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

	const std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	//Selected Validation Layer
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };


	// SDL TODO: Move out of renderer
	struct SDL_Window* window{ nullptr };
	bool isWindowInitialized{ false };
	int frameNumber{ 0 };
	bool stop_rendering{ false };
};

*/