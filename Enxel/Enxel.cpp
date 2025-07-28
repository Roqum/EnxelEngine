#include "Enxel.h"
#include "IRenderer.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using CreateRendererFunc = IRenderer * (*)();
using DestroyRendererFunc = void (*)(IRenderer*);

struct RendererModule {
    void* handle = nullptr;
    CreateRendererFunc create = nullptr;
    DestroyRendererFunc destroy = nullptr;
    IRenderer* instance = nullptr;

    bool load(const char* path) {
#ifdef _WIN32
        handle = LoadLibraryA(path);
        if (!handle) return false;
        create = (CreateRendererFunc)GetProcAddress((HMODULE)handle, "CreateRenderer");
        destroy = (DestroyRendererFunc)GetProcAddress((HMODULE)handle, "DestroyRenderer");
#else
        handle = dlopen(path, RTLD_NOW);
        if (!handle) return false;
        create = (CreateRendererFunc)dlsym(handle, "CreateRenderer");
        destroy = (DestroyRendererFunc)dlsym(handle, "DestroyRenderer");
#endif
        if (!create || !destroy) return false;
        instance = create();
        return instance != nullptr;
    }

    void unload() {
        if (instance && destroy) {
            destroy(instance);
            instance = nullptr;
        }
#ifdef _WIN32
        if (handle) FreeLibrary((HMODULE)handle);
#else
        if (handle) dlclose(handle);
#endif
        handle = nullptr;
        create = nullptr;
        destroy = nullptr;
    }
};

void Enxel::StartRenderer()
{
    RendererModule rendererModule;
    if (!rendererModule.load("VulkanRenderer.dll"))
    {
        std::cout << "Failed to load renderer module\n";
    }

    IRenderer* renderer = rendererModule.instance;
    renderer->Init();
}