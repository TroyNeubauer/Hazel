#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"

#include <functional>
#include <algorithm>

#include <math.h>
#include <float.h>

namespace Hazel {

	enum GraphDisplayMode
	{
		Linear, Log
	};

	enum GraphDataMode
	{
		CurrentValue,//Grab whatever value is currently avilable when its time for the next value
		Average,//Average the values we get between stores
		Min,//Store the min value collected
		Max//Store the max value collected
	};

	class Graph
	{
	public:
		Graph(const char* name, std::function<float()> Sample, unsigned int seconds, unsigned int samplesPerSecond);
		~Graph();

		void Render(float width, float height);
		void Update();//Must be called from OnImGuiRender();

		inline void SetDisplayMode(GraphDisplayMode mode) { m_DisplayMode = mode; }
		inline GraphDisplayMode GetDisplayMode() { return m_DisplayMode; }

		void SetDataMode(GraphDataMode mode);
		inline GraphDataMode GetDataMode() { return m_DataMode; }

		inline float GetLatestSample() { return m_LatestSample; }

	private:
		static thread_local unsigned int currentSample, currentTotalSamples;

		static float GetLogData(void* data, int i);
		static float GetLinearData(void* data, int i);

	private:
		inline int TotalSamples() { return m_Seconds * m_SamplesPerSecond; }
		inline void ResetForCurrentValue() {}//Nop - we dont use m_Temp at all
		inline void ResetForAverage() { m_Temp.clear(); }//Clear it to be ready to compue the next average
		inline void ResetForMin() { m_Temp.reserve(1); m_Temp[0] = FLT_MAX; }
		inline void ResetForMax() { m_Temp.reserve(1); m_Temp[0] = FLT_MIN; }

		inline float GetSample() { m_LatestSample = m_GetSample(); return m_LatestSample; }

	private:
		const char* m_Name;
		unsigned int m_Seconds, m_SamplesPerSecond;
		float* m_Data;

		std::vector<float> m_Temp;//General purpose vector for computing averages, min, maxes etc (usage depends on m_DataMode)
		std::function<float()> m_GetSample;//The function that gets the nes sample

		unsigned int m_Index, m_LastIndex = 0;
		float m_LatestSample = -1.0;
		
		GraphDisplayMode m_DisplayMode = GraphDisplayMode::Linear;
		GraphDataMode m_DataMode = GraphDataMode::CurrentValue;

	};

}
