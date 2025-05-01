#pragma once
#include <vulkan/vulkan.h>
#include <vector>


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class VulkanEngine {
public:
	//run main loop
	void run();

private:
	static VulkanEngine& Get();

	//Vulkan
	VkInstance vkInstance;
	VkPhysicalDevice vkPhysicalDevice;
	VkDevice vkDevice;

	//Selected Validation Layer
	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};



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
	void createPhysDevice();
	void createDevice();

	bool checkValidationLayerSupport();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	
};
//< intro