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
#include "Core/Utility/Timer.h"

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
        world->Generate(4,4);
    

	    for (Chunk& chunk : world->chunks)
	    {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            chunk.draw(vertices, indices);
            chunk.setVertexBuffer(m_Renderer->CreateVertexBuffer(vertices));
            chunk.setIndexBuffer(m_Renderer->CreateIndexBuffer(indices));
	    }
    
		bool running = true;
#ifdef PROFILING
		int currentFrame = 0;
        const int UI_FRAME_UPDATE_RATE = 10;
		float totalCycleTime = 0.0f;
        float cpuCycleTime = 0.0f;
        float gpuCycleTime = 0.0f;

        Timer timer;
#endif
        while (running)
        {
#ifdef PROFILING
            timer.Reset();
#endif
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

#ifdef PROFILING
            ImGui::Begin("Profiling");
            ImGui::Text("FPS: %.1f", 1000/totalCycleTime);
            ImGui::Text("Total Cycle Time: %.1f ms", totalCycleTime);
            ImGui::Text("CPU Cycle Time: %.1f ms", cpuCycleTime);
            ImGui::Text("GPU Cycle Time: %.1f ms", gpuCycleTime);
            ImGui::Text("Some more stats will be here soon");
            ImGui::End();

            currentFrame++;
            if (currentFrame > UI_FRAME_UPDATE_RATE) // dirty but works for now
            {
                gpuCycleTime = (float)m_Renderer->GetGPUCycleDuration();
                cpuCycleTime = timer.GetCurTime();
            }
#endif

            ImGui::Render();

            m_Renderer->RenderFrame();
#ifdef PROFILING
            if (currentFrame > UI_FRAME_UPDATE_RATE) // dirty but works for now
            {
                currentFrame = 0;
                totalCycleTime = timer.GetCurTime();
            }
#endif
        }

        m_Window->Cleanup();
        m_Renderer->Shutdown();
		world->Shutdown(); // TODO: Bugfix: Device destroyed before vertex/index buffers. Order should be reversed.
        delete world;
    }
}