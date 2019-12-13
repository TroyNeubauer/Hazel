#pragma once

namespace Hazel {



	class Timestep
	{
	public:
		explicit Timestep(float seconds) : m_Seconds(seconds) {}

		inline float Seconds() const { return m_Seconds; }
		inline float MilliSeconds() const { return m_Seconds * 1000.0f; }
		inline float MicroSeconds() const { return m_Seconds * 1000000.0f; }
		inline float NanoSeconds()  const { return m_Seconds * 1000000000.0f; }

		inline bool operator< (const Timestep& other) const { return m_Seconds < other.m_Seconds; }
		inline bool operator> (const Timestep& other) const { return m_Seconds > other.m_Seconds; }
		inline bool operator<=(const Timestep& other) const { return m_Seconds <= other.m_Seconds; }
		inline bool operator>=(const Timestep& other) const { return m_Seconds >= other.m_Seconds; }
		inline bool operator==(const Timestep& other) const { return m_Seconds == other.m_Seconds; }
		inline bool operator!=(const Timestep& other) const { return m_Seconds != other.m_Seconds; }

		inline void operator=(const Timestep& other)  { m_Seconds = other.m_Seconds; }

	private:
		float m_Seconds;
	};

}
