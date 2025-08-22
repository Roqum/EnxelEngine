#pragma once
//#include "pchEnxel.h"
#include <vulkan/vulkan.h>
#include "RendererAPI.h"
#include "glm/glm.hpp"

// Just for testing purposes
#include "Vertex.h"


// TODO Replace STL with alternative (EASTL)
#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <array>
#include <optional>

#include "Vulkan/VulkanBuffer.h"
#include "VulkanBuffer.h"
#include <imgui.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

constexpr unsigned int FRAME_OVERLAP = 2;

struct ImGuiVertex {
	glm::vec2 pos;    
	glm::vec2 uv;     
	uint32_t col;     
};

struct ImGuiFrameData {
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
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

namespace Enxel
{
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

struct RenderCommand
{
	VulkanVertexBuffer* vertexBuffer;
	VulkanIndexBuffer* indexBuffer;
};
	class VulkanRenderer : public IRenderer
{
public:
	// Inherited via IRenderer
	void Initialize(SDL_Window* sdlWindow, ImGuiContext* imguiContext);
	virtual void BeginScene() override;
	virtual void EndScene() override;
	void Submit(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) override;

	VulkanVertexBuffer* CreateVertexBuffer(std::vector<Vertex>& verticies) override;
	VulkanIndexBuffer* CreateIndexBuffer(std::vector<uint32_t>& indices) override;
	void RenderFrame() override;
	void StopRendering() override;
	void Shutdown() override;

private:
    void CreateInstance();
    void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateDescriptorSetLayout();
	void CreateGraphicsPipeline();

	//for debugging only
	void DebugPipelineInputs();

	void CreateImGuiGraphicsPipeline();
	void CreateCommandStructure();
	void CreateDepthResources();
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateImGuiDescriptorPool();
	void CreateDescriptorSets();
	void CreateImGuiDescriptorSets();
	void CreateSyncObjects();

	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	bool CheckValidationLayerSupport();
	bool HasStencilComponent(VkFormat format);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void UpdateUniformBuffer(uint32_t currentImage);
	
	VkShaderModule CreateShaderModule(const std::vector<char>& byteCode);
	
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	std::vector<RenderCommand> m_RenderQueue;

	int m_WindowHeight;	
	int m_WindowWidth;

	VkInstance m_VkInstance;
	VkPhysicalDevice m_VkPhysicalDevice = VK_NULL_HANDLE;
	VkDevice m_VkDevice;
	QueueFamilyIndices m_VkQueueFamilyIndicies;
	VkQueue m_VkGraphicsQueue;
	VkQueue m_VkPresentQueue;
	VkQueue m_VkTransferQueue;
	VkSurfaceKHR m_VkSurface;

	VkFormat m_VkSwapChainImageFormat;
	VkExtent2D m_VkSwapChainExtent;
	VkSwapchainKHR m_VkSwapChain;
	std::vector<VkImage> m_VkSwapChainImages;
	std::vector<VkImageView> m_VkSwapChainImageViews;



	VkDescriptorSet m_ImGuiFontDescriptorSet;
	VkDescriptorPool m_VkDescriptorPool;
	VkDescriptorSetLayout m_VkDescriptorSetLayout;
	VkPipeline m_VkGraphicsPipeline;
	VkPipelineLayout m_VkPipelineLayout;

	VkDescriptorSet m_VkImGuiDescriptorSet;
	VkDescriptorPool m_VkImGuiDescriptorPool;
	VkDescriptorSetLayout m_VkImGuiDescriptorSetLayout;
	VkPipelineLayout m_VkImGuiPipelineLayout;
	VkPipeline m_VkImGuiPipeline;

	VkSampler m_VkImGuiFontSampler;
	VkImageView m_VkImGuiFontImageView;


	VkCommandPool m_VkTransferCommandPool;

	VkBuffer m_VkStagingBuffer;
	VkDeviceMemory m_VkStagingBufferMemory;

	VkImage m_VkTextureImage;
	VkDeviceMemory m_VkTextureImageMemory;
	VkImageView m_VkTextureImageView;
	VkSampler m_VkTextureSampler;

	VkImage m_VkDepthImage;
	VkDeviceMemory m_VkDepthImageMemory;
	VkImageView m_VkDepthImageView;

	VkFrameData m_VkFrames[FRAME_OVERLAP];
	ImGuiFrameData m_VkImGuiFrames[FRAME_OVERLAP];
	uint32_t m_CurrentFrame = 0;

	std::vector<VkCommandBuffer> m_VkCommandBuffers;
	std::vector<VkSemaphore> m_VkImageAvailableSemaphores;
	std::vector<VkSemaphore> m_VkRenderFinishedSemaphores;
	std::vector<VkFence> m_VkInFlightFences;

	const std::vector<const char*> m_RequiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	//Selected Validation Layer
	const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

	struct SDL_Window* m_VkWindow{ nullptr };
	bool m_IsWindowInitialized{ false };
	int m_FrameNumber{ 0 };
	bool m_Stop_rendering{ false };

};
}
