#pragma once
#include "hzpch.h"
#include "Hazel/Core.h"
#include "Hazel/Engine.h"

namespace Hazel {
	template<typename T>
	class CachedFunction
	{
	public:
		CachedFunction(std::function<T()> func, float secondsPerUpdates) : m_Func(func), m_SecondsBetweenUpdates(secondsPerUpdates) {}

		T Get() {
			if (m_LastUpdate == -1.0f || (Engine::GetTime() - m_LastUpdate > m_SecondsBetweenUpdates)) {
				m_LastUpdate = Engine::GetTime();
				m_CachedValue = m_Func();
			}
			return m_CachedValue;
		}

	private:
		float m_SecondsBetweenUpdates;
		T m_CachedValue;

		float m_LastUpdate = -1.0f;
		std::function<T()> m_Func;
	};
}