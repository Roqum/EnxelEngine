#include "Vulkan/VulkanRenderer.h"
#include <iostream>

#include "SDL3/SDL.h"
#include <SDL3/SDL_vulkan.h>
/*

#ifdef _WIN32
extern "C" __declspec(dllexport) IRenderer* CreateRenderer() {
    return new VulkanRenderer();
}

extern "C" __declspec(dllexport) void DestroyRenderer(IRenderer* renderer) {
    delete renderer;
}
#elif
extern "C" IRenderer* CreateRenderer() {
    return new VulkanRenderer();
}

extern "C" void DestroyRenderer(IRenderer* renderer) {
    delete renderer;
}
#endif
void VulkanRenderer::Init()
{
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
}

void VulkanRenderer::RenderFrame()
{
}

void VulkanRenderer::Shutdown()
{
}

// SDL TODO: Move out from renderer
void VulkanRenderer::initWindow()
{
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

    window = SDL_CreateWindow(
        "Vulkan Engine",
        800,
        800,
        SDL_WINDOW_VULKAN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return;
    }
}

void VulkanRenderer::createInstance()
{
    //Validation Layer not supported
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Engine Core";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Voxine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInstanceInfo{};
    createInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInstanceInfo.pApplicationInfo = &appInfo;

    // Add validation layer
    if (enableValidationLayers) {
        createInstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInstanceInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInstanceInfo.enabledLayerCount = 0;
    }


    // SDL Extensions
    uint32_t reqSdlExtensionCount;
    const char* const* reqSdlExtensions = SDL_Vulkan_GetInstanceExtensions(&reqSdlExtensionCount);

    int instanceExtensionCount = reqSdlExtensionCount + 1;
    const char** instanceExtensions = (const char**)SDL_malloc(instanceExtensionCount * sizeof(const char*));
    instanceExtensions[0] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
    SDL_memcpy(&instanceExtensions[1], reqSdlExtensions, reqSdlExtensionCount * sizeof(const char*));


    createInstanceInfo.enabledExtensionCount = instanceExtensionCount;
    createInstanceInfo.ppEnabledExtensionNames = instanceExtensions;

    //std::vector<VkExtensionProperties> extensions(extensionCount);
    //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    if (vkCreateInstance(&createInstanceInfo, nullptr, &vkInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void VulkanRenderer::createSurface()
{
    if (SDL_Vulkan_CreateSurface(window, vkInstance, nullptr, &vkSurface) == false)
    {
        throw std::runtime_error("failed to create surface!");
    }
}

void VulkanRenderer::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    if (vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vulkan physical device!");
    }

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            vkPhysicalDevice = device;
            break;
        }
    }

    if (vkPhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

*/