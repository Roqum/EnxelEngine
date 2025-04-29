//> includes
#include "vk_engine.h"
#include "iostream"
#include <chrono>
#include <thread>
//< includes

//> init
constexpr bool bUseValidationLayers = false;

VulkanEngine* loadedEngine = nullptr;

VulkanEngine& VulkanEngine::Get() { return *loadedEngine; }
void VulkanEngine::init()
{
   
}

void VulkanEngine::cleanup()
{
    
}

void VulkanEngine::draw()
{
}

void VulkanEngine::run()
{
    std::cout << "Hello, World!" << std::endl; 
    while (true)
    {

    }
    
}
//< drawloop
