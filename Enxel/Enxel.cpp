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
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL_events.h>

namespace Enxel
{


    void Enxel::StartEngine()
    {
        m_Window = std::unique_ptr<IWindow>(IWindow::Create(WindowProperties()));
        m_Renderer = std::unique_ptr<IRenderer>(IRenderer::Create());

        IMGUI_CHECKVERSION();
        ImGuiContext* imguiContext = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        // Enable Docking if you want
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Style
        ImGui::StyleColorsDark();


        // Init SDL3 input binding
        ImGui_ImplSDL3_InitForVulkan(m_Window->GetSDLWindow());

        
        m_Renderer->Initialize(m_Window->GetSDLWindow(), imguiContext);

        


	    World* world = new World();
        world->Generate(3,3);
    

	    for (Chunk& chunk : world->chunks)
	    {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            chunk.draw(vertices, indices);
            chunk.setVertexBuffer(m_Renderer->CreateVertexBuffer(vertices));
            chunk.setIndexBuffer(m_Renderer->CreateIndexBuffer(indices));
	    }
    
		bool running = true;
        while (running)
        {

            SDL_Event event;
            while (SDL_PollEvent(&event)) {

                ImGui_ImplSDL3_ProcessEvent(&event);

                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
            }

            for (Chunk& chunk : world->chunks)
            {
                m_Renderer->Submit(chunk.getVertexBuffer(), chunk.getIndexBuffer());
            }

            ImGui_ImplSDL3_NewFrame();    
            ImGui::NewFrame();

            ImGui::Begin("Stats");
            ImGui::Text("FPS: %.1f");
            ImGui::End();

            ImGui::Render();


            m_Renderer->RenderFrame();
        }

        m_Window->Cleanup();
        m_Renderer->Shutdown();
		world->Shutdown(); // TODO: Bugfix: Device destroyed before vertex/index buffers. Order should be reversed.
        delete world;
    }
}