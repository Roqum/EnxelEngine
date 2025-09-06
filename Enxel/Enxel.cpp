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
#include "Renderer/Camera.h"
#include "Core/Event/EventSystem.h"
#include "Core/Input.h"

namespace Enxel
{
	static Enxel* s_EnxelInstance = nullptr;

    Enxel::Enxel()
    {
		s_EnxelInstance = this;
    }

    Enxel::~Enxel()
    {
		s_EnxelInstance = nullptr;
    }

    Enxel& Enxel::Get()
    {
        return *s_EnxelInstance;
    }

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

        Timer timer;
	    World* world = new World();

#ifdef PROFILING
		float chunkGenStartTime = timer.GetCurTimeMs();
#endif
        world->Generate(4,4);
    
#ifdef PROFILING
        float chunkGenInitTime = timer.GetCurTimeMs() - chunkGenStartTime;
        float verticesAndIndicesStartTime = timer.GetCurTimeMs();
        int verticesCount = 0;
#endif
	    for (Chunk& chunk : world->chunks)
	    {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            chunk.draw(vertices, indices);
            chunk.setVertexBuffer(m_Renderer->CreateVertexBuffer(vertices));
            chunk.setIndexBuffer(m_Renderer->CreateIndexBuffer(indices));
#ifdef PROFILING
			verticesCount += vertices.size();
#endif
	    }	
#ifdef PROFILING
        float verticesAndIndicesInitTime = timer.GetCurTimeMs() - verticesAndIndicesStartTime;

		int currentFrame = 0;
        const int UI_FRAME_UPDATE_RATE = 10;
		float totalCycleTime = 0.0f;
        float cpuCycleTime = 0.0f;
        float gpuCycleTime = 0.0f;

#endif

        m_Camera = Camera();
		m_CameraController = CameraController(&m_Camera);

        bool running = true;
        double previousTime = timer.GetCurTimeMs();
        while (running) // Game loop. Need to be cleaned up and refactored
        {
            float currentTime = timer.GetCurTimeMs();

			// TODO: delta time is changing based on hardware perfromance. Need to be fixed
            float elapsedTime = currentTime - previousTime;
            Update(elapsedTime);
            timer.Reset();

            SDL_Event sdlEvent;
            while (SDL_PollEvent(&sdlEvent)) {

                ImGui_ImplSDL3_ProcessEvent(&sdlEvent);

                if (sdlEvent.type == SDL_EVENT_QUIT)
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
            ImGui::Text("Initialization:");
            ImGui::Text("Vertices: %d:", verticesCount);
            ImGui::Text("World Generation: %.2f ms", chunkGenInitTime);
            ImGui::Text("Vertex and Index Initialization: %.2f ms", verticesAndIndicesInitTime);
            ImGui::Text("");

            ImGui::Text("Game Loop:");
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
                cpuCycleTime = timer.GetCurTimeMs();
            }
#endif

            ImGui::Render();
            m_Renderer->BeginScene(&m_Camera);
            m_Renderer->RenderFrame();
#ifdef PROFILING
            if (currentFrame > UI_FRAME_UPDATE_RATE) // dirty but works for now
            {
                currentFrame = 0;
                totalCycleTime = timer.GetCurTimeMs();
            }
#endif
        }

        m_Window->Cleanup();
        m_Renderer->Shutdown();
		world->Shutdown(); // TODO: Bugfix: Device destroyed before vertex/index buffers. Order should be reversed.
        delete world;
    }


    void Enxel::Update(float deltaTime)
    {
		m_CameraController.OnUpdate(deltaTime);
    }
}