#include "Enxel.h"
#include "Renderer/RendererAPI.h"
#include <iostream>
#include "World/World.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <World/Chunk.h>

namespace Enxel
{


    void Enxel::StartEngine()
    {
        m_Window = std::unique_ptr<IWindow>(IWindow::Create(WindowProperties()));
        m_Renderer = std::unique_ptr<IRenderer>(IRenderer::Create());
        m_Renderer->Initialize(m_Window->GetSDLWindow());

	    World* world = new World();
        world->Generate(6,6);
    

	    for (Chunk& chunk : world->chunks)
	    {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            chunk.draw(vertices, indices);
            chunk.setVertexBuffer(m_Renderer->CreateVertexBuffer(vertices));
            chunk.setIndexBuffer(m_Renderer->CreateIndexBuffer(indices));
	    }
    

        while (true)
        {
		    if (!m_Window->Tick())
            {
                m_Window->Cleanup();
			    break; 
		    }

            for (Chunk& chunk : world->chunks)
            {
                m_Renderer->Submit(chunk.getVertexBuffer(), chunk.getIndexBuffer());
            }
            m_Renderer->RenderFrame();
        }

        m_Renderer->Shutdown();
        delete world;
    }
}