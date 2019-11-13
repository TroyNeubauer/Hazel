#include "hzpch.h"
#include "DebugLayer.h"
#include "Hazel/Engine.h"

#include "Hazel/TUtil.h"
#include "Platform/System/FileTracker.h"
#include "Platform/System/AllocTracker.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

namespace Hazel {
	DebugLayer::DebugLayer()
#ifndef HZ_DIST
	  : processVMem(System::GetProcessVirtualMemoryUsage, 0.5f),
		systemVMem(System::GetSystemVirtualMemoryUsage, 0.5f),
		totalVMem(System::GetTotalMachineVirtualMemory, 0.5f),
		processMem(System::GetProcessPhysicalMemoryUsage, 0.5f),
		systemMem(System::GetSystemPhysicalMemoryUsage, 0.5f),
		totalMem(System::GetTotalMachinePhysicalMemory, 0.5f),
		processCPU(System::GetProcessCPUUsagePercent, 0.5f),
		systemCPU(System::GetSystemCPUUsagePercent, 0.5f)
#endif
	{}

	void DebugLayer::Update()
	{
		frames++;

		float secondDelta = Engine::GetTime() - lastSecond;
		if (secondDelta > 1.0f) {
			averageFPS = frames;
			lastSecondDelta = secondDelta;
			lastSecond += 1.0f;

			frames = 0;
		}

		if (min == -1.0f || Engine::GetDeltaTime() < min)
			min = Engine::GetDeltaTime();
		if (max == -1.0f || Engine::GetDeltaTime() > max)
			max = Engine::GetDeltaTime();
#ifndef HZ_DIST
		m_FrameTime->Update();
#endif
	}
	char buffer[1024];
	bool showFileWindow = false;

	void DebugLayer::OnImGuiRender()
	{
		const float DISTANCE = 10.0f;
		Update();
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 window_pos = ImVec2(viewport->Pos.x + DISTANCE, viewport->Pos.y + DISTANCE);
		ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(viewport->ID);
		if (ImGui::Begin("Video Performance Overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
		{
			ImGui::Text("Average Frame Time %.2f (FPS: %d) | Frame Time %.2f (FPS: %.1f) ", lastSecondDelta / averageFPS * 1000.0f, averageFPS, Engine::GetDeltaTime() * 1000.0f, 1.0f / Engine::GetDeltaTime());
			ImGui::Text("Min FT: %.2f (FPS: %.1f) | Max FT: %.2f (FPS: %.1f)", min * 1000.0f, 1.0f / min, max * 1000.0f, 1.0f / max);
			
#ifndef HZ_DIST
			ImGui::Text("Current Frame: %d", frames);
#endif
			ImGui::End();
		}

#ifndef HZ_DIST
		viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowViewport(viewport->ID);
		if (ImGui::Begin("Graphs Performance Overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
		{
			m_FrameTime->Render(GRAPHS_WIDTH, GRAPHS_HEIGHT);

			window_pos = ImVec2(viewport->Pos.x + viewport->Size.x - ImGui::GetWindowWidth() - DISTANCE, viewport->Pos.y + viewport->Size.y - ImGui::GetWindowHeight() - DISTANCE);
			ImGui::SetWindowPos(window_pos);
			ImGui::End();
		}

		viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowViewport(viewport->ID);
		if (ImGui::Begin("System Info Overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings))
		{
			double pVMem = processVMem.Get() / 1024.0 / 1024.0;
			double sVMem = systemVMem.Get() / 1024.0 / 1024.0;
			double tVMem = totalVMem.Get() / 1024.0 / 1024.0 / 1024.0;
			ImGui::Text("VMem Usage %.2f MB / %.1f MB (System) / %.3f GB (Total)", pVMem, sVMem, tVMem);
			
			double pMem = processMem.Get() / 1024.0 / 1024.0;
			double sMem = systemMem.Get() / 1024.0 / 1024.0;
			double tMem = totalMem.Get() / 1024.0 / 1024.0 / 1024.0;
			ImGui::Text("RAM Usage  %.2f MB / %.1f MB (System) / %.3f GB (Total)", pMem, sMem, tMem);

			ImGui::Text("CPU Usage %.1f / %.1f (System)", processCPU.Get(), systemCPU.Get());
			
			snprintf(buffer, sizeof(buffer), "Files Open %llu, Total Opened %llu, Total Closed %llu", FileTracker::GetCurrentlyOpenFilesCount(), FileTracker::GetOpenedFilesCount(), FileTracker::GetClosedFilesCount());
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			if (ImGui::Button(buffer))
				showFileWindow = !showFileWindow;
			ImGui::PopStyleColor();
			if (showFileWindow) {
				ImGui::Begin("Files");
				ImGui::Text("Open Files:");
				for (auto& it : FileTracker::GetOpenFilePaths()) {
					const std::string& path = it.second;
					ImGui::Text("\t%s", path.c_str());
				}
				ImGui::Text("Closed Files:");
				for (auto& path : FileTracker::GetClosedFilePaths()) {
					ImGui::Text("\t%s", path.c_str());
				}
				ImGui::End();
			}

			ImGui::Text("");
			ImGui::Text("Current Allocs %lu, Total Allocs %lu, Total Frees %lu", AllocTracker::GetCurrentAllocCount(), AllocTracker::GetTotalAllocCount(), AllocTracker::GetTotalFreeCount());
			ImGui::Text("Allocs/s %lu, Frees /s %lu", AllocTracker::GetAllocCountSec(), AllocTracker::GetFreeCountSec());
			ImGui::Text("Allocs/frame %lu, Frees /frame %lu", AllocTracker::GetAllocsPerFrame(), AllocTracker::GetFreesPerFrame());

			window_pos = ImVec2(viewport->Pos.x + viewport->Size.x - ImGui::GetWindowWidth() - DISTANCE, viewport->Pos.y + DISTANCE);
			ImGui::SetWindowPos(window_pos);
			ImGui::End();
		}
#endif
	}

	void DebugLayer::OnAttach()
	{
#ifndef HZ_DIST
		m_FrameTime = new Graph("Frame Time", []() { return Engine::GetDeltaTime(); }, 5, 60);
		m_FrameTime->SetDataMode(GraphDataMode::CurrentValue);
		m_FrameTime->SetDisplayMode(GraphDisplayMode::Linear);
#endif
	}

	void DebugLayer::OnDetach()
	{
#ifndef HZ_DIST
		delete m_FrameTime;
#endif
	}

	void DebugLayer::Reset()
	{
		min = Engine::GetDeltaTime();
		max = Engine::GetDeltaTime();
	}

	DebugLayer::~DebugLayer()
	{
	}

}
