#pragma once

#include "Hazel/Layer/Layer.h"
#include "Hazel/Util/Graph.h"


#include "Hazel/TUtil.h"

#define GRAPHS_WIDTH 400
#define GRAPHS_HEIGHT 30

namespace Hazel {
	class DebugLayer : public Layer
	{
	public:
		DebugLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual ~DebugLayer() override;
	private:
		void Reset();// Resets the min amd max frametimes
		void Update();

		float lastSecondDelta = 0.0f;
		float lastSecond = 0.0f;
		float min = -1.0, max = -1.0;
		int frames = 0;
		int averageFPS = 0;

#ifndef HZ_DIST
		Graph* m_FrameTime;

		CachedFunction<uint64_t> processVMem, systemVMem, totalVMem;
		CachedFunction<uint64_t> processMem, systemMem, totalMem;
		CachedFunction<float> processCPU, systemCPU;
#endif
	};
}