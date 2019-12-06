#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <thread>

#include <inttypes.h>

namespace Hazel {
	struct ProfileResult
	{
		const char* Name;
		uint64_t Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		FILE* m_File;
		int m_ProfileCount;
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0), m_File(nullptr)
		{
		}

		void BeginSession(const std::string& name, const char* filepath = "results.json");
		void EndSession();
		void WriteProfile(const ProfileResult& result);
		
		void WriteHeader();
		void WriteFooter();

		inline static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:

		InstrumentationTimer(const char* name);
		void Stop();
		~InstrumentationTimer();
	
	
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#define HZ_PROFILE

#ifdef HZ_PROFILE
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::Hazel::Instrumentor::Get().BeginSession(name, filepath)
	#define HZ_PROFILE_END_SESSION() ::Hazel::Instrumentor::Get().EndSession()
	#define HZ_PROFILE_SCOPE(name) ::Hazel::InstrumentationTimer timer##__LINE__(name)
	#define HZ_PROFILE_CALL(x) { HZ_PROFILE_SCOPE(#x); x; }

	#if defined(HZ_COMPILER_GCC)
		#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__PRETTY_FUNCTION__)
	#elif defined(HZ_COMPILER_MSVC)
		#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__)
	#endif
#else
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
	#define HZ_PROFILE_END_SESSION()
	#define HZ_PROFILE_SCOPE(name)
	#define HZ_PROFILE_CALL(x) x;
	#define HZ_PROFILE_FUNCTION()
#endif
