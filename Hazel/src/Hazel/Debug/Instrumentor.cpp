#include "hzpch.h"
#include "Instrumentor.h"

#include "Hazel/Core/Engine.h"

namespace Hazel {

	void Instrumentor::BeginSession(const std::string& name, const char* filepath)
	{
		m_File = fopen(filepath, "w");
		HZ_CORE_ASSERT(m_File, "Failed to open file {}", filepath);
		WriteHeader();
		m_CurrentSession = new InstrumentationSession{ name };
	}

	void Instrumentor::EndSession()
	{
		WriteFooter();
		fclose(m_File);
		m_File = nullptr;
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		if (!Engine::IsStaticInitializationComplete()) return; //We have no way to profile a session in a initalizer
		HZ_CORE_ASSERT(m_CurrentSession, "No session is active!");

		if (m_ProfileCount++ > 0)
			fputc(',', m_File);

		char name[1024];
		TUtil::StringUtils::Copy(name, sizeof(name), result.Name);

		std::replace(TUtil::StringUtils::begin(name), TUtil::StringUtils::end(name), '"', '\'');

		fprintf(m_File, "{"
			"\"cat\":\"function\","
			"\"dur\":%" PRId64 ","
			"\"name\":\"%" "s" "\","
			"\"ph\":\"X\","
			"\"pid\":0,"
			"\"tid\":%" PRId64 ","
			"\"ts\":%" PRId64
			"}", result.End - result.Start, name, result.ThreadID, result.Start);
	}

	void Instrumentor::WriteHeader()
	{
		fprintf(m_File, "{\"otherData\": {},\"traceEvents\":[");
	}

	void Instrumentor::WriteFooter()
	{
		fprintf(m_File, "]}");
	}


	InstrumentationTimer::InstrumentationTimer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	void InstrumentationTimer::Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

		m_Stopped = true;
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!m_Stopped)
			Stop();
	}

}

