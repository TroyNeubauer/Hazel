#include "hzpch.h"
#include "DebugLayer.h"
#include "Hazel/Engine.h"

#include "Platform/PlatformUtils.h"
#include "Platform/System/FileTracker.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

namespace Hazel {
	DebugLayer::DebugLayer()
#ifndef HZ_DIST
	  : processVMem(PlatformUtils::GetProcessVirtualMemoryUsage, 0.5f),
		systemVMem(PlatformUtils::GetSystemVirtualMemoryUsage, 0.5f),
		totalVMem(PlatformUtils::GetTotalMachineVirtualMemory, 0.5f),
		processMem(PlatformUtils::GetProcessPhysicalMemoryUsage, 0.5f),
		systemMem(PlatformUtils::GetSystemPhysicalMemoryUsage, 0.5f),
		totalMem(PlatformUtils::GetTotalMachinePhysicalMemory, 0.5f),
		processCPU(PlatformUtils::GetProcessCPUUsagePercent, 0.5f),
		systemCPU(PlatformUtils::GetSystemCPUUsagePercent, 0.5f)
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


char buffer[4096];
#define DEBUG_TEXT(format, ...)								\
	snprintf(buffer, sizeof(buffer), format, __VA_ARGS__);	\
	ImGui::Text(buffer);									\

	void DebugLayer::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
		const float DISTANCE = 10.0f;
		Update();
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 window_pos = ImVec2(viewport->Pos.x + DISTANCE, viewport->Pos.y + DISTANCE);
		ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(viewport->ID);
		if (ImGui::Begin("Video Performance Overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
		{
			DEBUG_TEXT("Average Frame Time %.2f (FPS: %d) | Frame Time %.2f (FPS: %.1f) ", lastSecondDelta / averageFPS * 1000.0f, averageFPS, Engine::GetDeltaTime() * 1000.0f, 1.0f / Engine::GetDeltaTime());
			DEBUG_TEXT("Min FT: %.2f (FPS: %.1f) | Max FT: %.2f (FPS: %.1f)", min * 1000.0f, 1.0f / min, max * 1000.0f, 1.0f / max);
			
#ifndef HZ_DIST
			DEBUG_TEXT("Current Frame: %d", frames);
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
		if (ImGui::Begin("System Info Overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
		{
			double pVMem = processVMem.Get() / 1024.0 / 1024.0;
			double sVMem = systemVMem.Get() / 1024.0 / 1024.0;
			double tVMem = totalVMem.Get() / 1024.0 / 1024.0 / 1024.0;
			DEBUG_TEXT("VMem Usage %.2f MB / %.1f MB (System) / %.3f GB (Total)", pVMem, sVMem, tVMem);
			
			double pMem = processMem.Get() / 1024.0 / 1024.0;
			double sMem = systemMem.Get() / 1024.0 / 1024.0;
			double tMem = totalMem.Get() / 1024.0 / 1024.0 / 1024.0;
			DEBUG_TEXT("RAM Usage  %.2f MB / %.1f MB (System) / %.3f GB (Total)", pMem, sMem, tMem);

			DEBUG_TEXT("CPU Usage %.1f / %.1f (System)", processCPU.Get(), systemCPU.Get());
			ImGui::Text("");
			DEBUG_TEXT("Files Open %llu, Total Opened %llu, Total Closed %llu", FileTracker::GetCurrentlyOpenFilesCount(), FileTracker::GetOpenedFilesCount(), FileTracker::GetClosedFilesCount());
		
			ImGui::Text("");
			DEBUG_TEXT("Current Allocs %llu, Total Allocs %llu, Total Frees %llu", AllocTracker::GetCurrentAllocCount(), AllocTracker::GetTotalAllocCount(), AllocTracker::GetTotalFreeCount());
			DEBUG_TEXT("Allocs/s %llu, Frees /s %llu", AllocTracker::GetAllocCountSec(), AllocTracker::GetFreeCountSec());
			DEBUG_TEXT("Allocs/frame %llu, Frees /frame %llu", AllocTracker::GetAllocsPerFrame(), AllocTracker::GetFreesPerFrame());

			window_pos = ImVec2(viewport->Pos.x + viewport->Size.x - ImGui::GetWindowWidth() - DISTANCE, viewport->Pos.y + DISTANCE);
			ImGui::SetWindowPos(window_pos);
			ImGui::End();
		}
#endif
	}

	void DebugLayer::OnAttach()
	{
#ifndef HZ_DIST
		Hazel::PlatformUtils::Init();
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
