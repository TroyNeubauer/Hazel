#include "hzpch.h"
#include "Graph.h"

#include "../Core/Engine.h"
#include "imgui.h"

namespace Hazel {
	thread_local unsigned int Graph::currentSample, Graph::currentTotalSamples;

	Graph::Graph(const char* name, std::function<float()> Sample, unsigned int seconds, unsigned int samplesPerSecond) :
		m_Name(name), m_GetSample(Sample), m_Seconds(seconds), m_SamplesPerSecond(samplesPerSecond), m_Temp(1)
	{
		this->m_Data = new float[TotalSamples()];
		for (int i = 0; i < TotalSamples(); i++) {
			m_Data[i] = FLT_MIN;
		}
	}
	Graph::~Graph()
	{
		delete[] m_Data;
	}

	inline int PositiveModulo(int i, int n) {
		return (i % n + n) % n;
	}

	void Graph::Update()
	{
		m_Index = static_cast<int>(Engine::GetTime() * m_SamplesPerSecond) % TotalSamples();
		float current;
		switch (m_DataMode) {
			case CurrentValue:
			{
				break;//Nop - we only get the value when we need it
			}
			case Average:
			{
				m_Temp.push_back(GetSample());//Add the next sample
				break;
			}
			case Min:
			{
				current = GetSample();
				if (current < m_Temp[0])//Store this one if it is better then the old one
					m_Temp[0] = current;
				break; 
			}
			case Max:
			{
				current = GetSample();
				if (current > m_Temp[0])
					m_Temp[0] = current;
				break;
			}
			default:
			{
				HZ_ASSERT(false, "Unhandled case! " + std::to_string((int) m_DataMode));
				break;
			}
		}

		if (m_Index != m_LastIndex) {
			float value;
			switch (m_DataMode) {//Determine the avtual value to put in the array based on what data mode we are using
				case CurrentValue:
				{
					value = GetSample();
					ResetForCurrentValue();
					break;
				}
				case Average:
				{
					double total = 0.0;
					for (int i = 0; i < m_Temp.size(); i++)
						total += (double) m_Temp[i];
					value = (float) (total / (double) m_Temp.size());
					ResetForAverage();// Clears the vector
					break;
				}
				case Min:
				{
					value = m_Temp[0];
					ResetForMin();//Clears the current min value for next time
					break;
				}
				case Max:
				{
					value = m_Temp[0];
					ResetForMax();//Clears the current max value for next time
					break;
				}
				default:
				{
					HZ_ASSERT(false, "Unhandled case! " + std::to_string((int) m_DataMode));
					break;
				}
			}

			for (int i = m_Index; true; i = PositiveModulo(i - 1, TotalSamples())) {//Start at the current index and fill in any data points we missed from now to the last index
				if (i == m_LastIndex) break;//Stop once we hit the last index
				m_Data[i] = value;
			}

			m_LastIndex = m_Index;
		}
	}

	void Graph::Render(float width, float height)
	{
		currentSample = m_Index;
		currentTotalSamples = TotalSamples();
		float(*valuesGetter)(void*, int) = (m_DisplayMode == GraphDisplayMode::Linear) ? GetLinearData : GetLogData;
		ImGui::PlotLines("", valuesGetter, m_Data, Graph::currentTotalSamples, 0, m_Name, FLT_MAX, FLT_MAX, {width, height});
	}

	float Graph::GetLogData(void* data, int i)
	{
		int index = PositiveModulo(currentSample - (currentTotalSamples - i - 1), currentTotalSamples);//Make it look like the graph is scrolling forward
		return log(static_cast<float*>(data)[index]);
	}

	float Graph::GetLinearData(void* data, int i)
	{
		int index = PositiveModulo(currentSample - (currentTotalSamples - i - 1), currentTotalSamples);//Make it look like the graph is scrolling forward
		return static_cast<float*>(data)[index];
	}

	void Graph::SetDataMode(GraphDataMode mode)
	{
		m_DataMode = mode;
		switch (m_DataMode) {//Ensure that we can use the new mode properly
		case CurrentValue: 
			ResetForCurrentValue();
			break;
		case Average:
			ResetForAverage();
			break;
		case Min:
			ResetForMin();
			break;
		case Max:
			ResetForMax();
			break;
		default:
			HZ_ASSERT(false, "Unhandled case! " + std::to_string((int) m_DataMode));
		}
	}
}
