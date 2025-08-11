#include "Enxel.h"
#include "Renderer/RendererAPI.h"
#include <iostream>
#include "Vulkan/Chunk.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace Enxel
{


void Enxel::StartEngine()
{
    m_Window = std::unique_ptr<IWindow>(IWindow::Create(WindowProperties()));
    m_Renderer = std::unique_ptr<IRenderer>(IRenderer::Create());
    m_Renderer->Initialize(m_Window->GetSDLWindow());


    //m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create());
	//m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create());

	Chunk m_ChunkTest;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

	m_ChunkTest.draw(vertices, indices);
    m_ChunkTest.vertexBuffer = m_Renderer->CreateVertexBuffer(vertices);
    m_ChunkTest.indexBuffer = m_Renderer->CreateIndexBuffer(indices);

    while (true)
    {
		if (!m_Window->Tick())
        {
            m_Window->Cleanup();
			break; 
		}
        m_Renderer->RenderFrame();
    }

    m_Renderer->Shutdown();

    /*
    if (renderer == nullptr)
    {
        RendererModule rendererModule;
        if (!rendererModule.load("VulkanRenderer.dll"))
        {
            std::cout << "Failed to load renderer module\n";
        }

        IRenderer* renderer = rendererModule.instance;
    }
    renderer->Init();


    
    while (!bQuit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // close the window when user alt-f4s or clicks the X button
            if (e.type == SDL_EVENT_QUIT)
            {
                bQuit = true;
            }

            if (e.type == SDL_EVENT_WINDOW_MINIMIZED) {
                stop_rendering = true;
            }
            if (e.type == SDL_EVENT_WINDOW_RESTORED) {
                stop_rendering = false;
            }

        }

        if (renderer->StopRendering()) {
            // throttle the speed 
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        renderer->RenderFrame();
    }
    cleanup();

    */
}
}