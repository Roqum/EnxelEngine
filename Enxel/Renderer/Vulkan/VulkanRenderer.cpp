#include "Vulkan/VulkanRenderer.h"
#include <iostream>

#include "SDL3/SDL.h"
#include <SDL3/SDL_vulkan.h>

#include <set>

#include <glm/gtc/matrix_transform.hpp>

// filesystem stuff (should not be dealt with in renderer)
#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdParty/stb_image.h"
#include <filesystem>
#include <fstream>
#include <imgui.h>

namespace Enxel
{

    // ------- Shaders Copied from imgui vulkan source -------
    
    // backends/vulkan/glsl_shader.vert, compiled with:
    // # glslangValidator -V -x -o glsl_shader.vert.u32 glsl_shader.vert
    /*
    #version 450 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aUV;
    layout(location = 2) in vec4 aColor;
    layout(push_constant) uniform uPushConstant { vec2 uScale; vec2 uTranslate; } pc;

    out gl_PerVertex { vec4 gl_Position; };
    layout(location = 0) out struct { vec4 Color; vec2 UV; } Out;

    void main()
    {
        Out.Color = aColor;
        Out.UV = aUV;
        gl_Position = vec4(aPos * pc.uScale + pc.uTranslate, 0, 1);
    }
    */
    static uint32_t __glsl_shader_vert_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
        0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
        0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
        0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
        0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
        0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
        0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
        0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
        0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
        0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
        0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
        0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
        0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
        0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
        0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
        0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
        0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
        0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
        0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
        0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
        0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
        0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
        0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
        0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
        0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
        0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
        0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
        0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
        0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
        0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
        0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
        0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
        0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
        0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
        0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
        0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
        0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
        0x0000002d,0x0000002c,0x000100fd,0x00010038
    };

    // backends/vulkan/glsl_shader.frag, compiled with:
    // # glslangValidator -V -x -o glsl_shader.frag.u32 glsl_shader.frag
    /*
    #version 450 core
    layout(location = 0) out vec4 fColor;
    layout(set=0, binding=0) uniform sampler2D sTexture;
    layout(location = 0) in struct { vec4 Color; vec2 UV; } In;
    void main()
    {
        fColor = In.Color * texture(sTexture, In.UV.st);
    }
    */
    static uint32_t __glsl_shader_frag_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
        0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
        0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
        0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
        0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
        0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
        0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
        0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
        0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
        0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
        0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
        0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
        0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
        0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
        0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
        0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
        0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
        0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
        0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
        0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
        0x00010038
    };

    // TODO: I dont want renderer to read files from system. They should be passed in instead
    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        std::cout << "Current working directory: "
            << std::filesystem::current_path() << std::endl;

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;

    }

    void VulkanRenderer::Initialize(SDL_Window* sdlWindow, ImGuiContext* imguiContext) 
    {
        ImGui::SetCurrentContext(imguiContext);
        m_RenderQueue.reserve(1000);

        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

        m_VkWindow = sdlWindow;

        if (!m_VkWindow) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            SDL_Quit();
            return;
        }

        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateImageViews();
        CreateDescriptorSetLayout();
        CreateGraphicsPipeline();
        CreateImGuiGraphicsPipeline();
	    CreateCommandStructure();
        CreateDepthResources();
        CreateTextureImage();
        CreateTextureImageView();
        CreateTextureSampler();
        CreateImGuiBuffers();
        CreateUniformBuffers();
        CreateDescriptorPool();
        CreateDescriptorSets();
        CreateImGuiDescriptorPool();
        CreateImGuiDescriptorSets();
        CreateSyncObjects();

#ifdef PROFILING
        InitializeTimeStampQuery();
#endif

    }

  

    void VulkanRenderer::BeginScene()
    {
    }

    void VulkanRenderer::EndScene()
    {
    }

    void VulkanRenderer::Submit(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
    {
		m_RenderQueue.emplace_back((VulkanVertexBuffer*)vertexBuffer, (VulkanIndexBuffer*)indexBuffer);
    }


    void VulkanRenderer::RenderFrame()
    {
        vkWaitForFences(m_VkDevice, 1, &m_VkFrames[m_CurrentFrame].vkInFlightFence, VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(m_VkDevice, m_VkSwapChain, UINT64_MAX, m_VkFrames[m_CurrentFrame].vkImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        UpdateUniformBuffer(m_CurrentFrame);

        ImDrawData* drawData = ImGui::GetDrawData();
        UploadImGuiBuffers(drawData, m_CurrentFrame);

        vkResetFences(m_VkDevice, 1, &m_VkFrames[m_CurrentFrame].vkInFlightFence);

        vkResetCommandBuffer(m_VkFrames[m_CurrentFrame].vkMainCommandBuffer, 0);

        RecordCommandBuffer(m_VkFrames[m_CurrentFrame].vkMainCommandBuffer, imageIndex, drawData);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_VkFrames[m_CurrentFrame].vkImageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_VkFrames[m_CurrentFrame].vkMainCommandBuffer;
        VkSemaphore signalSemaphores[] = { m_VkFrames[m_CurrentFrame].vkRenderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_VkGraphicsQueue, 1, &submitInfo, m_VkFrames[m_CurrentFrame].vkInFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_VkSwapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(m_VkPresentQueue, &presentInfo);

        m_CurrentFrame = (m_CurrentFrame + 1) % FRAME_OVERLAP;
    }

    void VulkanRenderer::StopRendering()
    {
    }

    void VulkanRenderer::Shutdown()
    {
        vkDeviceWaitIdle(m_VkDevice);

        vkDestroyImageView(m_VkDevice, m_VkDepthImageView, nullptr);
        vkDestroyImage(m_VkDevice, m_VkDepthImage, nullptr);
        vkFreeMemory(m_VkDevice, m_VkDepthImageMemory, nullptr);

        vkDestroySwapchainKHR(m_VkDevice, m_VkSwapChain, nullptr);

        vkDestroyPipeline(m_VkDevice, m_VkGraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(m_VkDevice, m_VkPipelineLayout, nullptr);

        vkDestroyPipeline(m_VkDevice, m_VkImGuiPipeline, nullptr);
        vkDestroyPipelineLayout(m_VkDevice, m_VkImGuiPipelineLayout, nullptr);

        vkDestroyDescriptorPool(m_VkDevice, m_VkDescriptorPool, nullptr);

        vkDestroySampler(m_VkDevice, m_VkTextureSampler, nullptr);
        vkDestroyImageView(m_VkDevice, m_VkTextureImageView, nullptr);
        vkDestroyImage(m_VkDevice, m_VkTextureImage, nullptr);
        vkFreeMemory(m_VkDevice, m_VkTextureImageMemory, nullptr);

        vkDestroyDescriptorSetLayout(m_VkDevice, m_VkDescriptorSetLayout, nullptr);
        vkDestroyDescriptorSetLayout(m_VkDevice, m_VkImGuiDescriptorSetLayout, nullptr);

        for (size_t i = 0; i < FRAME_OVERLAP; i++) {
            vkDestroyBuffer(m_VkDevice, m_VkFrames[i].vkUniformBuffers, nullptr);
            vkFreeMemory(m_VkDevice, m_VkFrames[i].vkUniformBuffersMemory, nullptr);
        }

        for (size_t i = 0; i < FRAME_OVERLAP; i++)
        {
            vkDestroySemaphore(m_VkDevice, m_VkFrames[i].vkImageAvailableSemaphore, nullptr);
            vkDestroySemaphore(m_VkDevice, m_VkFrames[i].vkRenderFinishedSemaphore, nullptr);
            vkDestroyFence(m_VkDevice, m_VkFrames[i].vkInFlightFence, nullptr);
            vkDestroyCommandPool(m_VkDevice, m_VkFrames[i].vkCommandPool, nullptr);
        }
        vkDestroyCommandPool(m_VkDevice, m_VkTransferCommandPool, nullptr);

        vkDestroySurfaceKHR(m_VkInstance, m_VkSurface, nullptr);
        vkDestroyDevice(m_VkDevice, nullptr);
        vkDestroyInstance(m_VkInstance, nullptr);

        SDL_DestroyWindow(m_VkWindow);
    }

#ifdef PROFILING
    void VulkanRenderer::InitializeTimeStampQuery()
    {
        VkQueryPoolCreateInfo queryPoolInfo{};
        queryPoolInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
        queryPoolInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
        queryPoolInfo.queryCount = 2; // start + end

        vkCreateQueryPool(m_VkDevice, &queryPoolInfo, nullptr, &gpuTimeQueryPool);
    }
    double VulkanRenderer::GetGPUCycleDuration()
    {
        uint64_t timestamps[2];
        vkGetQueryPoolResults(
            m_VkDevice,
            gpuTimeQueryPool,
            0, 2,
            sizeof(timestamps),
            timestamps,
            sizeof(uint64_t),
            VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT
        );

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(m_VkPhysicalDevice, &props);
        return (timestamps[1] - timestamps[0]) * props.limits.timestampPeriod / 1e6;
    }
#endif

    void VulkanRenderer::CreateInstance()
    {
        //Validation Layer not supported
        if (enableValidationLayers && !CheckValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Engine Core";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 2, 0);
        appInfo.pEngineName = "Voxine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 2, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInstanceInfo{};
        createInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInstanceInfo.pApplicationInfo = &appInfo;

        // Add validation layer
        if (enableValidationLayers) {
            createInstanceInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
            createInstanceInfo.ppEnabledLayerNames = m_ValidationLayers.data();
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

        if (vkCreateInstance(&createInstanceInfo, nullptr, &m_VkInstance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void VulkanRenderer::CreateSurface()
    {
        if (SDL_Vulkan_CreateSurface(m_VkWindow, m_VkInstance, nullptr, &m_VkSurface) == false)
        {
            throw std::runtime_error("failed to create surface!");
        }
    }

    void VulkanRenderer::PickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        if (vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, devices.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create vulkan physical device!");
        }

        for (const auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                m_VkPhysicalDevice = device;
                break;
            }
        }

        if (m_VkPhysicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void VulkanRenderer::CreateLogicalDevice()
    {
        m_VkQueueFamilyIndicies = FindQueueFamilies(m_VkPhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfosVec{};
        std::set<uint32_t> uniqueQueueFamilies = { m_VkQueueFamilyIndicies.graphicsFamily.value(), m_VkQueueFamilyIndicies.presentFamily.value(), m_VkQueueFamilyIndicies.transferFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfosVec.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature = {};
        dynamicRenderingFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
        dynamicRenderingFeature.dynamicRendering = VK_TRUE;


        VkPhysicalDeviceFeatures2 deviceFeatures2 = {};
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.pNext = &dynamicRenderingFeature;

        vkGetPhysicalDeviceFeatures2(m_VkPhysicalDevice, &deviceFeatures2);

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = &deviceFeatures2;
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfosVec.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfosVec.data();
        deviceCreateInfo.pEnabledFeatures = nullptr; 
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_RequiredDeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = m_RequiredDeviceExtensions.data();

        if (enableValidationLayers) {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
        }
        else {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(m_VkPhysicalDevice, &deviceCreateInfo, nullptr, &m_VkDevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }
        vkGetDeviceQueue(m_VkDevice, m_VkQueueFamilyIndicies.graphicsFamily.value(), 0, &m_VkGraphicsQueue);
        vkGetDeviceQueue(m_VkDevice, m_VkQueueFamilyIndicies.presentFamily.value(), 0, &m_VkPresentQueue);
        vkGetDeviceQueue(m_VkDevice, m_VkQueueFamilyIndicies.transferFamily.value(), 0, &m_VkTransferQueue);

        vkCmdBeginRenderingKHR =
            (PFN_vkCmdBeginRenderingKHR)vkGetDeviceProcAddr(m_VkDevice, "vkCmdBeginRenderingKHR");

        vkCmdEndRenderingKHR =
            (PFN_vkCmdEndRenderingKHR)vkGetDeviceProcAddr(m_VkDevice, "vkCmdEndRenderingKHR");

        if (!vkCmdBeginRenderingKHR || !vkCmdEndRenderingKHR) {
            throw std::runtime_error("Dynamic rendering extension not available!");
        }
    }

    void VulkanRenderer::CreateSwapChain() {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_VkPhysicalDevice);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        m_VkSwapChainImageFormat = surfaceFormat.format;

        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        m_VkSwapChainExtent = ChooseSwapExtent(swapChainSupport.capabilities);


        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        VkSwapchainCreateInfoKHR swapchainCreateInfo{};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = m_VkSurface;
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = m_VkSwapChainExtent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


        uint32_t queueFamilyIndices[] = { m_VkQueueFamilyIndicies.graphicsFamily.value(), m_VkQueueFamilyIndicies.presentFamily.value() };

        if (m_VkQueueFamilyIndicies.graphicsFamily != m_VkQueueFamilyIndicies.presentFamily) {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
            swapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_VkDevice, &swapchainCreateInfo, nullptr, &m_VkSwapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(m_VkDevice, m_VkSwapChain, &imageCount, nullptr);
        m_VkSwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_VkDevice, m_VkSwapChain, &imageCount, m_VkSwapChainImages.data());
    }

    void VulkanRenderer::CreateImageViews()
    {
        m_VkSwapChainImageViews.resize(m_VkSwapChainImages.size());

        for (size_t i = 0; i < m_VkSwapChainImages.size(); i++) {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = m_VkSwapChainImages[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = m_VkSwapChainImageFormat;

            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_VkDevice, &imageViewCreateInfo, nullptr, &m_VkSwapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    void VulkanRenderer::CreateDescriptorSetLayout()
    {
        // -------------- scene descriptor set layout --------------
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(m_VkDevice, &layoutInfo, nullptr, &m_VkDescriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }

		// -------------- imgui descriptor set layout --------------
        VkDescriptorSetLayoutBinding samplerBindingUI{};
        samplerBindingUI.binding = 0;
        samplerBindingUI.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerBindingUI.descriptorCount = 1;
        samplerBindingUI.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerBindingUI.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfoUI{};
        layoutInfoUI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfoUI.bindingCount = 1;
        layoutInfoUI.pBindings = &samplerBindingUI;

        if (vkCreateDescriptorSetLayout(m_VkDevice, &layoutInfoUI, nullptr, &m_VkImGuiDescriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create ImGui descriptor set layout!");
        }
    }

    void VulkanRenderer::CreateGraphicsPipeline()
    {
        auto vertShaderCode = readFile("shaders/vert.spv");
        auto fragShaderCode = readFile("shaders/frag.spv");

        VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        auto bindingDescription = getBindingDescription();
        auto attributeDescriptions = getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)m_VkSwapChainExtent.width;
        viewport.height = (float)m_VkSwapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_VkSwapChainExtent;

        std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;
        viewportState.pViewports = nullptr;
        viewportState.pScissors = nullptr;


        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_VkDescriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(m_VkDevice, &pipelineLayoutInfo, nullptr, &m_VkPipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional


        VkPipelineRenderingCreateInfoKHR renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR; // Not _KHR suffix
        renderingInfo.pNext = nullptr;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachmentFormats = &m_VkSwapChainImageFormat;
        renderingInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
        renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
        renderingInfo.viewMask = 0; // For multiview rendering, 0 means no multiview

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext = &renderingInfo; // Chain the rendering info
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_VkPipelineLayout;
        pipelineInfo.renderPass = VK_NULL_HANDLE; // Must be VK_NULL_HANDLE for dynamic rendering
        pipelineInfo.subpass = 0; // Ignored when renderPass is VK_NULL_HANDLE
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        

        if (vkCreateGraphicsPipelines(m_VkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_VkGraphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }


        vkDestroyShaderModule(m_VkDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(m_VkDevice, vertShaderModule, nullptr);

        
    }


    void VulkanRenderer::CreateImGuiGraphicsPipeline()
    {
        // -------------------- imgui UI Pipeline --------------------

        VkShaderModuleCreateInfo createVertShaderStageUIInfo{};
        createVertShaderStageUIInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createVertShaderStageUIInfo.codeSize = sizeof(__glsl_shader_vert_spv);
        createVertShaderStageUIInfo.pCode = __glsl_shader_vert_spv;

        VkShaderModule vertShaderModuleUI;
        if (vkCreateShaderModule(m_VkDevice, &createVertShaderStageUIInfo, nullptr, &vertShaderModuleUI) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex shader module!");
        }

        VkShaderModuleCreateInfo createFragShaderStageUIInfo{};
        createFragShaderStageUIInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createFragShaderStageUIInfo.codeSize = sizeof(__glsl_shader_frag_spv);
        createFragShaderStageUIInfo.pCode = __glsl_shader_frag_spv;

        VkShaderModule fragShaderModuleUI;
        if (vkCreateShaderModule(m_VkDevice, &createFragShaderStageUIInfo, nullptr, &fragShaderModuleUI) != VK_SUCCESS) {
            throw std::runtime_error("failed to create fragment shader module!");
        }

        VkPipelineShaderStageCreateInfo vertStageUI{};
        vertStageUI.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertStageUI.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertStageUI.module = vertShaderModuleUI;
        vertStageUI.pName = "main";

        VkPipelineShaderStageCreateInfo fragStageUI{};
        fragStageUI.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragStageUI.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragStageUI.module = fragShaderModuleUI;
        fragStageUI.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStagesUI[] = { vertStageUI, fragStageUI };

        VkVertexInputBindingDescription bindingDescUI{};
        bindingDescUI.binding = 0;
        bindingDescUI.stride = sizeof(ImGuiVertex);
        bindingDescUI.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::array<VkVertexInputAttributeDescription, 3> attrDescsUI{};

        attrDescsUI[0].location = 0; // Position
        attrDescsUI[0].binding = 0;
        attrDescsUI[0].format = VK_FORMAT_R32G32_SFLOAT;
        attrDescsUI[0].offset = offsetof(ImGuiVertex, pos);

        attrDescsUI[1].location = 1; // UV
        attrDescsUI[1].binding = 0;
        attrDescsUI[1].format = VK_FORMAT_R32G32_SFLOAT;
        attrDescsUI[1].offset = offsetof(ImGuiVertex, uv);

        attrDescsUI[2].location = 2; // Color
        attrDescsUI[2].binding = 0;
        attrDescsUI[2].format = VK_FORMAT_R8G8B8A8_UNORM;
        attrDescsUI[2].offset = offsetof(ImGuiVertex, col);

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyUI{};
        inputAssemblyUI.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyUI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyUI.primitiveRestartEnable = VK_FALSE;

        VkPipelineVertexInputStateCreateInfo vertexInputInfoUI{};
        vertexInputInfoUI.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfoUI.vertexBindingDescriptionCount = 1;
        vertexInputInfoUI.pVertexBindingDescriptions = &bindingDescUI;
        vertexInputInfoUI.vertexAttributeDescriptionCount = static_cast<uint32_t>(attrDescsUI.size());
        vertexInputInfoUI.pVertexAttributeDescriptions = attrDescsUI.data();


        VkPipelineRasterizationStateCreateInfo rasterizerUI{};
        rasterizerUI.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerUI.depthClampEnable = VK_FALSE;
        rasterizerUI.rasterizerDiscardEnable = VK_FALSE;
        rasterizerUI.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerUI.lineWidth = 1.0f;
        rasterizerUI.cullMode = VK_CULL_MODE_NONE;
        rasterizerUI.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerUI.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisamplingUI{};
        multisamplingUI.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisamplingUI.sampleShadingEnable = VK_FALSE;
        multisamplingUI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachmentUI{};
        colorBlendAttachmentUI.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentUI.blendEnable = VK_TRUE;
        colorBlendAttachmentUI.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachmentUI.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachmentUI.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentUI.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentUI.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentUI.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlendingUI{};
        colorBlendingUI.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendingUI.logicOpEnable = VK_FALSE;
        colorBlendingUI.attachmentCount = 1;
        colorBlendingUI.pAttachments = &colorBlendAttachmentUI;

        VkPipelineDepthStencilStateCreateInfo depthStencilUI{};
        depthStencilUI.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilUI.depthTestEnable = VK_FALSE;
        depthStencilUI.depthWriteEnable = VK_FALSE;
        depthStencilUI.depthCompareOp = VK_COMPARE_OP_ALWAYS;
        depthStencilUI.stencilTestEnable = VK_FALSE;

        VkDynamicState dynamicStatesUI[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicStateInfoUI{};
        dynamicStateInfoUI.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfoUI.dynamicStateCount = 2;
        dynamicStateInfoUI.pDynamicStates = dynamicStatesUI;

        VkPipelineViewportStateCreateInfo viewportStateUI{};
        viewportStateUI.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateUI.viewportCount = 1;
        viewportStateUI.scissorCount = 1;

        VkPipelineLayoutCreateInfo pipelineLayoutInfoUI{};
        pipelineLayoutInfoUI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        pipelineLayoutInfoUI.setLayoutCount = 1;
        pipelineLayoutInfoUI.pSetLayouts = &m_VkImGuiDescriptorSetLayout;

        VkPushConstantRange pushConstant{};
        pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstant.offset = 0;
        pushConstant.size = sizeof(float) * 4;
        pipelineLayoutInfoUI.pushConstantRangeCount = 1;
        pipelineLayoutInfoUI.pPushConstantRanges = &pushConstant;

        if (vkCreatePipelineLayout(m_VkDevice, &pipelineLayoutInfoUI, nullptr, &m_VkImGuiPipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create ImGui pipeline layout!");
        }

        VkPipelineRenderingCreateInfo renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        renderingInfo.colorAttachmentCount = 1;
        VkFormat colorFormat = m_VkSwapChainImageFormat;
        renderingInfo.pColorAttachmentFormats = &colorFormat;
        // No depth/stencil for ImGui:
        renderingInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
        renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;


        VkGraphicsPipelineCreateInfo pipelineInfoUI{};
        pipelineInfoUI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfoUI.pNext = &renderingInfo; 
        pipelineInfoUI.stageCount = 2;
        pipelineInfoUI.pStages = shaderStagesUI;
        pipelineInfoUI.pVertexInputState = &vertexInputInfoUI;
        pipelineInfoUI.pInputAssemblyState = &inputAssemblyUI;
        pipelineInfoUI.pViewportState = &viewportStateUI;
        pipelineInfoUI.pRasterizationState = &rasterizerUI;
        pipelineInfoUI.pMultisampleState = &multisamplingUI;
        pipelineInfoUI.pDepthStencilState = &depthStencilUI;
        pipelineInfoUI.pColorBlendState = &colorBlendingUI;
        pipelineInfoUI.pDynamicState = &dynamicStateInfoUI;
        pipelineInfoUI.layout = m_VkImGuiPipelineLayout;
        pipelineInfoUI.renderPass = VK_NULL_HANDLE; 


        if (vkCreateGraphicsPipelines(m_VkDevice, VK_NULL_HANDLE, 1, &pipelineInfoUI, nullptr, &m_VkImGuiPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create ImGui graphics pipeline!");
        }
        vkDestroyShaderModule(m_VkDevice, vertShaderModuleUI, nullptr);
        vkDestroyShaderModule(m_VkDevice, fragShaderModuleUI, nullptr);
    }

    void VulkanRenderer::CreateCommandStructure()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.pNext = nullptr;
        poolInfo.queueFamilyIndex = m_VkQueueFamilyIndicies.graphicsFamily.value();

        for (size_t i = 0; i < FRAME_OVERLAP; i++)
        {
            if (vkCreateCommandPool(m_VkDevice, &poolInfo, nullptr, &m_VkFrames[i].vkCommandPool) != VK_SUCCESS) {
                throw std::runtime_error("failed to create command pool!");
            }

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = m_VkFrames[i].vkCommandPool;
            allocInfo.pNext = nullptr;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(m_VkDevice, &allocInfo, &m_VkFrames[i].vkMainCommandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate command buffers!");
            }
        }

        VkCommandPoolCreateInfo transferPoolInfo{};
        transferPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        transferPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        transferPoolInfo.pNext = nullptr;
        transferPoolInfo.queueFamilyIndex = m_VkQueueFamilyIndicies.transferFamily.value();

        if (vkCreateCommandPool(m_VkDevice, &poolInfo, nullptr, &m_VkTransferCommandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void VulkanRenderer::CreateDepthResources()
    {
        VkFormat depthFormat = FindDepthFormat();
        CreateImage(m_VkSwapChainExtent.width, m_VkSwapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VkDepthImage, m_VkDepthImageMemory);
        m_VkDepthImageView = CreateImageView(m_VkDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        TransitionImageLayout(m_VkDepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    }


    void VulkanRenderer::CreateTextureImage()
    {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load("textures/texture.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(m_VkDevice, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(m_VkDevice, stagingBufferMemory);

        stbi_image_free(pixels);

        CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VkTextureImage, m_VkTextureImageMemory);

        TransitionImageLayout(m_VkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        CopyBufferToImage(stagingBuffer, m_VkTextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

        TransitionImageLayout(m_VkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(m_VkDevice, stagingBuffer, nullptr);
        vkFreeMemory(m_VkDevice, stagingBufferMemory, nullptr);
    }

    void VulkanRenderer::CreateTextureImageView()
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_VkTextureImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_VkDevice, &viewInfo, nullptr, &m_VkTextureImageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }
    }

    void VulkanRenderer::CreateTextureSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;

        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(m_VkPhysicalDevice, &properties);

        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(m_VkDevice, &samplerInfo, nullptr, &m_VkTextureSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void VulkanRenderer::CreateImGuiBuffers()
    {

        for (uint32_t i = 0; i < FRAME_OVERLAP; i++) {
            VkDeviceSize initialVertexSize = 1024 * 1024;
            VkDeviceSize initialIndexSize = 1024 * 1024;

            // Create vertex buffer
            CreateBuffer(initialVertexSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_VkImGuiFrames[i].vertexBuffer,
                m_VkImGuiFrames[i].vertexBufferMemory);
            m_VkImGuiFrames[i].vertexBufferSize = initialVertexSize;

            // Create index buffer
            CreateBuffer(initialIndexSize,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_VkImGuiFrames[i].indexBuffer,
                m_VkImGuiFrames[i].indexBufferMemory);
            m_VkImGuiFrames[i].indexBufferSize = initialIndexSize;
        }
    }

    VulkanVertexBuffer* VulkanRenderer::CreateVertexBuffer(std::vector<Vertex>& verticies)
    {
        
        VkDeviceSize bufferSize = sizeof(verticies[0]) * verticies.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        void* data;
        vkMapMemory(m_VkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, verticies.data(), (size_t)bufferSize);
        vkUnmapMemory(m_VkDevice, stagingBufferMemory);

        VkBuffer vkVertexBuffer;
        VkDeviceMemory vkVertexBufferMemory;

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkVertexBuffer, vkVertexBufferMemory);
        
        VulkanVertexBuffer* vertexBuffer = new VulkanVertexBuffer(bufferSize, &m_VkDevice, vkVertexBuffer, vkVertexBufferMemory);

        CopyBuffer(stagingBuffer, vkVertexBuffer, bufferSize);


        vkDestroyBuffer(m_VkDevice, stagingBuffer, nullptr);
        vkFreeMemory(m_VkDevice, stagingBufferMemory, nullptr);

        return vertexBuffer;
    }

    VulkanIndexBuffer* VulkanRenderer::CreateIndexBuffer(std::vector<uint32_t>& indices)
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(m_VkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(m_VkDevice, stagingBufferMemory);

        VkBuffer vkIndexBuffer;
        VkDeviceMemory vkIndexBufferMemory;

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkIndexBuffer, vkIndexBufferMemory);
        CopyBuffer(stagingBuffer, vkIndexBuffer, bufferSize);


        VulkanIndexBuffer* indexBuffer = new VulkanIndexBuffer(bufferSize, &m_VkDevice, vkIndexBuffer, vkIndexBufferMemory);

        vkDestroyBuffer(m_VkDevice, stagingBuffer, nullptr);
        vkFreeMemory(m_VkDevice, stagingBufferMemory, nullptr);

		return indexBuffer;

    }

    void VulkanRenderer::CreateUniformBuffers()
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        for (size_t i = 0; i < FRAME_OVERLAP; i++) {
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_VkFrames[i].vkUniformBuffers, m_VkFrames[i].vkUniformBuffersMemory);

            vkMapMemory(m_VkDevice, m_VkFrames[i].vkUniformBuffersMemory, 0, bufferSize, 0, &m_VkFrames[i].vkUniformBuffersMapped);
        }
    }

    void VulkanRenderer::CreateDescriptorPool()
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(FRAME_OVERLAP);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(FRAME_OVERLAP);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(FRAME_OVERLAP);


        if (vkCreateDescriptorPool(m_VkDevice, &poolInfo, nullptr, &m_VkDescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

    }

    void VulkanRenderer::CreateImGuiDescriptorPool()
    {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = static_cast<uint32_t>(FRAME_OVERLAP);


        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(FRAME_OVERLAP);


        if (vkCreateDescriptorPool(m_VkDevice, &poolInfo, nullptr, &m_VkImGuiDescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

    }
    void VulkanRenderer::CreateDescriptorSets()
    {
        std::vector<VkDescriptorSetLayout> layouts(FRAME_OVERLAP, m_VkDescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_VkDescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(FRAME_OVERLAP);
        allocInfo.pSetLayouts = layouts.data();

        std::vector<VkDescriptorSet> descriptorSets;
        descriptorSets.resize(FRAME_OVERLAP);
        if (vkAllocateDescriptorSets(m_VkDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < FRAME_OVERLAP; i++) {
            m_VkFrames[i].vkDescriptorSet = descriptorSets[i];

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_VkFrames[i].vkUniformBuffers;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = m_VkTextureImageView;
            imageInfo.sampler = m_VkTextureSampler;

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(m_VkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        }
    }

    void VulkanRenderer::CreateImGuiDescriptorSets()
    {
        // --------- Load imgui font ---------
        ImGuiIO& io = ImGui::GetIO();

        // Request RGBA32 font atlas data from ImGui
        unsigned char* fontPixels;
        int texWidth, texHeight;
        io.Fonts->GetTexDataAsRGBA32(&fontPixels, &texWidth, &texHeight);

        VkImage fontImage;
        VkDeviceMemory fontImageMemory;
		CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, fontImage, fontImageMemory);
        
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VkDeviceSize imageSize = texWidth * texHeight * 4; // RGBA32

        CreateBuffer(imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        // Copy font pixels into the buffer
        void* data;
        vkMapMemory(m_VkDevice, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, fontPixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(m_VkDevice, stagingBufferMemory);

        TransitionImageLayout(fontImage,
            VK_FORMAT_R8G8B8A8_UNORM,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        CopyBufferToImage(stagingBuffer,
            fontImage,
            static_cast<uint32_t>(texWidth),
            static_cast<uint32_t>(texHeight));

        TransitionImageLayout(fontImage,
            VK_FORMAT_R8G8B8A8_UNORM,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        
        m_VkImGuiFontImageView = CreateImageView(fontImage,
            VK_FORMAT_R8G8B8A8_UNORM,
            VK_IMAGE_ASPECT_COLOR_BIT);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        
        if (vkCreateSampler(m_VkDevice, &samplerInfo, nullptr, &m_VkImGuiFontSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create font sampler!");
        }
        
        // ------------------------------------


        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_VkImGuiDescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_VkImGuiDescriptorSetLayout;

        if (vkAllocateDescriptorSets(m_VkDevice, &allocInfo, &m_VkImGuiDescriptorSet) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }


        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_VkImGuiFontImageView;
        imageInfo.sampler = m_VkImGuiFontSampler;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_VkImGuiDescriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(m_VkDevice, 1, &descriptorWrite, 0, nullptr);

        io.Fonts->SetTexID((ImTextureID)m_VkImGuiDescriptorSet);

    }

    void VulkanRenderer::CreateSyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < FRAME_OVERLAP; i++)
        {
            if (vkCreateSemaphore(m_VkDevice, &semaphoreInfo, nullptr, &m_VkFrames[i].vkImageAvailableSemaphore) != VK_SUCCESS ||
                vkCreateSemaphore(m_VkDevice, &semaphoreInfo, nullptr, &m_VkFrames[i].vkRenderFinishedSemaphore) != VK_SUCCESS ||
                vkCreateFence(m_VkDevice, &fenceInfo, nullptr, &m_VkFrames[i].vkInFlightFence) != VK_SUCCESS) {
                throw std::runtime_error("failed to create semaphores!");
            }
        }
    }

    void VulkanRenderer::UploadImGuiBuffers(ImDrawData* drawData, uint32_t currentFrame)
    {
        if (!drawData || drawData->TotalVtxCount == 0)
            return;

        ImGuiFrameData& imguiFrameData = m_VkImGuiFrames[currentFrame];

        size_t vertexSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
        size_t indexSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);

        if (vertexSize > imguiFrameData.vertexBufferSize) {
            vkDestroyBuffer(m_VkDevice, imguiFrameData.vertexBuffer, nullptr);
            vkFreeMemory(m_VkDevice, imguiFrameData.vertexBufferMemory, nullptr);
            CreateBuffer(vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                imguiFrameData.vertexBuffer, imguiFrameData.vertexBufferMemory);
            imguiFrameData.vertexBufferSize = vertexSize;
        }

        if (indexSize > imguiFrameData.indexBufferSize) {
            vkDestroyBuffer(m_VkDevice, imguiFrameData.indexBuffer, nullptr);
            vkFreeMemory(m_VkDevice, imguiFrameData.indexBufferMemory, nullptr);
            CreateBuffer(indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                imguiFrameData.indexBuffer, imguiFrameData.indexBufferMemory);
            imguiFrameData.indexBufferSize = indexSize;
        }

        void* vtxDst;
        vkMapMemory(m_VkDevice, imguiFrameData.vertexBufferMemory, 0, vertexSize, 0, &vtxDst);

        void* idxDst;
        vkMapMemory(m_VkDevice, imguiFrameData.indexBufferMemory, 0, indexSize, 0, &idxDst);

        for (int n = 0; n < drawData->CmdListsCount; n++) {
            const ImDrawList* cmdList = drawData->CmdLists[n];
            memcpy(vtxDst, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(idxDst, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
            vtxDst = (char*)vtxDst + cmdList->VtxBuffer.Size * sizeof(ImDrawVert);
            idxDst = (char*)idxDst + cmdList->IdxBuffer.Size * sizeof(ImDrawIdx);
        }

        vkUnmapMemory(m_VkDevice, imguiFrameData.vertexBufferMemory);
        vkUnmapMemory(m_VkDevice, imguiFrameData.indexBufferMemory);

    }

    bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    bool VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(m_RequiredDeviceExtensions.begin(), m_RequiredDeviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    QueueFamilyIndices VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device)
    {

        QueueFamilyIndices queueIndices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());



        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueIndices.graphicsFamily = i;
            }
            if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                queueIndices.transferFamily = i;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_VkSurface, &presentSupport);

            if (presentSupport) {
                queueIndices.presentFamily = i;
            }
            if (queueIndices.isComplete()) {
                break;
            }
            i++;
        }

        return queueIndices;

    }

    SwapChainSupportDetails VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_VkSurface, &details.capabilities);

        uint32_t surfaceFormatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_VkSurface, &surfaceFormatCount, nullptr);

        if (surfaceFormatCount != 0) {
            details.formats.resize(surfaceFormatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_VkSurface, &surfaceFormatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_VkSurface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_VkSurface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    uint32_t VulkanRenderer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_VkPhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");

    }

    bool VulkanRenderer::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_ValidationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    bool VulkanRenderer::HasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    VkExtent2D VulkanRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            SDL_GetWindowSizeInPixels(m_VkWindow, &m_WindowWidth, &m_WindowHeight);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(m_WindowWidth),
                static_cast<uint32_t>(m_WindowHeight)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
        return VkExtent2D();
    }

    void VulkanRenderer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;

        std::vector<uint32_t> sharingIndices;

        if (m_VkQueueFamilyIndicies.graphicsFamily != m_VkQueueFamilyIndicies.transferFamily) {
            sharingIndices = {
                m_VkQueueFamilyIndicies.graphicsFamily.value(),
                m_VkQueueFamilyIndicies.transferFamily.value()
            };
            bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
            bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(sharingIndices.size());
            bufferInfo.pQueueFamilyIndices = sharingIndices.data();
        }
        else {
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            bufferInfo.queueFamilyIndexCount = 0;
            bufferInfo.pQueueFamilyIndices = nullptr;
        }


        if (vkCreateBuffer(m_VkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_VkDevice, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_VkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(m_VkDevice, buffer, bufferMemory, 0);
    }

    void VulkanRenderer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        EndSingleTimeCommands(commandBuffer);

    }

    void VulkanRenderer::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        EndSingleTimeCommands(commandBuffer);
    }

    void VulkanRenderer::UpdateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.model = glm::translate(ubo.model, glm::vec3(4.0f, 0.0f, 0.0f));

        ubo.view = glm::lookAt(glm::vec3(60.0f, 60.0f, 60.0f), glm::vec3(16.0f, 16.0f, 16.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        ubo.proj = glm::perspective(
            glm::radians(45.0f),
            m_VkSwapChainExtent.width / (float)m_VkSwapChainExtent.height,
            0.1f,
            10000.0f // or larger, depending on scene scale
        );
        ubo.proj[1][1] *= -1;

        memcpy(m_VkFrames[currentImage].vkUniformBuffersMapped, &ubo, sizeof(ubo));
    }

    VkShaderModule VulkanRenderer::CreateShaderModule(const std::vector<char>& byteCode)
    {
        VkShaderModuleCreateInfo shaderModulCreateInfo{};
        shaderModulCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModulCreateInfo.codeSize = byteCode.size();
        shaderModulCreateInfo.pCode = reinterpret_cast<const uint32_t*>(byteCode.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_VkDevice, &shaderModulCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        return shaderModule;
    }

    VkCommandBuffer VulkanRenderer::BeginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_VkTransferCommandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_VkDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void VulkanRenderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_VkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_VkGraphicsQueue);

        vkFreeCommandBuffers(m_VkDevice, m_VkTransferCommandPool, 1, &commandBuffer);
    }

    void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, ImDrawData* drawData)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
#ifdef PROFILING
        vkCmdResetQueryPool(commandBuffer, gpuTimeQueryPool, 0, 2);
        vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, gpuTimeQueryPool, 0);
#endif

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_VkSwapChainImages[imageIndex];
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        vkCmdPipelineBarrier(
            commandBuffer,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        VkRenderingAttachmentInfoKHR  colorAttachment{};
        colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        colorAttachment.pNext = nullptr;
        colorAttachment.imageView = m_VkSwapChainImageViews[imageIndex];
        colorAttachment.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        colorAttachment.resolveMode = VK_RESOLVE_MODE_NONE; 
        colorAttachment.resolveImageView = VK_NULL_HANDLE;  
        colorAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED; 
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.clearValue.color = { {0.0f, 0.0f, 0.0f, 1.0f} };


        VkRenderingAttachmentInfoKHR  depthAttachment{};
        VkRenderingAttachmentInfoKHR* pDepthAttachment = nullptr;

        if (m_VkDepthImageView != VK_NULL_HANDLE) {
            depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
            depthAttachment.pNext = nullptr;
            depthAttachment.imageView = m_VkDepthImageView;
            depthAttachment.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            depthAttachment.resolveMode = VK_RESOLVE_MODE_NONE;
            depthAttachment.resolveImageView = VK_NULL_HANDLE;
            depthAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.clearValue.depthStencil = { 1.0f, 0 };
            pDepthAttachment = &depthAttachment;
        }

        // Rendering info
        VkRenderingInfoKHR  renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
        renderingInfo.pNext = nullptr;
        renderingInfo.flags = 0;
        renderingInfo.renderArea.offset = { 0, 0 };
        renderingInfo.renderArea.extent = m_VkSwapChainExtent;
        renderingInfo.layerCount = 1;
        renderingInfo.viewMask = 0;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;
        renderingInfo.pDepthAttachment = pDepthAttachment;
        renderingInfo.pStencilAttachment = nullptr;

        vkCmdBeginRenderingKHR(commandBuffer, &renderingInfo);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkGraphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_VkSwapChainExtent.width);
        viewport.height = static_cast<float>(m_VkSwapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_VkSwapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        while (!m_RenderQueue.empty())
        { 
            RenderCommand& renderCommand = m_RenderQueue.back(); 
            VkBuffer vertexBuffers[] = { renderCommand.vertexBuffer->m_VkBuffer };
            VkDeviceSize offsets[] = { 0 };

            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffer, renderCommand.indexBuffer->m_VkBuffer, 0, VK_INDEX_TYPE_UINT32);

            m_RenderQueue.pop_back();
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkPipelineLayout, 0, 1, &m_VkFrames[m_CurrentFrame].vkDescriptorSet, 0, nullptr);
            uint32_t indexCount = static_cast<uint32_t>(renderCommand.indexBuffer->m_Size / sizeof(uint32_t));

            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        }

        vkCmdEndRenderingKHR(commandBuffer);
      

        // ------------------- UI Rendering -------------------
        VkRenderingAttachmentInfoKHR imguiColorAttachment{};
        imguiColorAttachment.sType = colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        imguiColorAttachment.imageView = m_VkSwapChainImageViews[imageIndex];
        imguiColorAttachment.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        imguiColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;   
        imguiColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        VkRenderingInfoKHR imguiRenderingInfo{};
        imguiRenderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
        imguiRenderingInfo.renderArea.offset = { 0,0 };
        imguiRenderingInfo.renderArea.extent = m_VkSwapChainExtent;
        imguiRenderingInfo.layerCount = 1;
        imguiRenderingInfo.colorAttachmentCount = 1;
        imguiRenderingInfo.pColorAttachments = &imguiColorAttachment;

        vkCmdBeginRenderingKHR(commandBuffer, &imguiRenderingInfo);

        // Viewport/scissor for UI
        VkViewport viewportUI{};
        viewportUI.x = 0.0f;
        viewportUI.y = 0.0f;
        viewportUI.width = static_cast<float>(m_VkSwapChainExtent.width);
        viewportUI.height = static_cast<float>(m_VkSwapChainExtent.height);
        viewportUI.minDepth = 0.0f;
        viewportUI.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewportUI);

        VkRect2D scissorUI{};
        scissorUI.offset = { 0, 0 };
        scissorUI.extent = m_VkSwapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissorUI);

        ImGuiPushConstant imguiPushConstant;
        imguiPushConstant.scale.x = 2.0f / drawData->DisplaySize.x;
        imguiPushConstant.scale.y = 2.0f / drawData->DisplaySize.y;
        imguiPushConstant.translate.x = -1.0f - drawData->DisplayPos.x * imguiPushConstant.scale.x;
        imguiPushConstant.translate.y = -1.0f - drawData->DisplayPos.y * imguiPushConstant.scale.y;

        // Let ImGui draw here
        //ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
        ImGuiFrameData& frame = m_VkImGuiFrames[m_CurrentFrame];

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkImGuiPipeline);

        vkCmdPushConstants(
            commandBuffer,
            m_VkImGuiPipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,                          
            sizeof(ImGuiPushConstant), 
            &imguiPushConstant 
        );

        VkBuffer vertexBuffers[] = { frame.vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, frame.indexBuffer, 0, sizeof(ImDrawIdx) == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkImGuiPipelineLayout, 0, 1, &m_VkImGuiDescriptorSet, 0, nullptr);

        int vtxOffset = 0;
        int idxOffset = 0;

        for (int n = 0; n < drawData->CmdListsCount; n++)
        {
            const ImDrawList* cmdList = drawData->CmdLists[n];

            for (int cmd_i = 0; cmd_i < cmdList->CmdBuffer.Size; cmd_i++)
            {
                const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmd_i];

                VkRect2D scissor;
                scissor.offset.x = static_cast<int32_t>(pcmd->ClipRect.x);
                scissor.offset.y = static_cast<int32_t>(pcmd->ClipRect.y);
                scissor.extent.width = static_cast<uint32_t>(pcmd->ClipRect.z - pcmd->ClipRect.x);
                scissor.extent.height = static_cast<uint32_t>(pcmd->ClipRect.w - pcmd->ClipRect.y);
                vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

                vkCmdDrawIndexed(commandBuffer, pcmd->ElemCount, 1, idxOffset, vtxOffset, 0);

                idxOffset += pcmd->ElemCount;
            }

            vtxOffset += cmdList->VtxBuffer.Size;
        }
        

        vkCmdEndRenderingKHR(commandBuffer);


        VkImageMemoryBarrier presentBarrier{};
        presentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        presentBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
        presentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        presentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        presentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        presentBarrier.image = m_VkSwapChainImages[imageIndex];
        presentBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        presentBarrier.subresourceRange.baseMipLevel = 0;
        presentBarrier.subresourceRange.levelCount = 1;
        presentBarrier.subresourceRange.baseArrayLayer = 0;
        presentBarrier.subresourceRange.layerCount = 1;
        presentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        presentBarrier.dstAccessMask = 0;

        vkCmdPipelineBarrier(
            commandBuffer,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &presentBarrier
        );
        

#ifdef PROFILING
        vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, gpuTimeQueryPool, 1);
#endif

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void VulkanRenderer::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0; // Optional

        if (vkCreateImage(m_VkDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_VkDevice, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_VkDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }
        vkBindImageMemory(m_VkDevice, image, imageMemory, 0);
    }

    VkImageView VulkanRenderer::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(m_VkDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }

    void VulkanRenderer::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {


        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (HasStencilComponent(format)) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndSingleTimeCommands(commandBuffer);
    }

    VkFormat VulkanRenderer::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_VkPhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");
    }

    VkFormat VulkanRenderer::FindDepthFormat() {
        return FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_IMMEDIATE_KHR;
    }

}