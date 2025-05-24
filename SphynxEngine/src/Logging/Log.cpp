#include "spxpch.h"
#include "Log.h"

#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)


namespace Sphynx
{
	SPHYNX_API std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	SPHYNX_API std::shared_ptr<spdlog::logger> Log::m_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_CoreLogger = spdlog::stdout_color_mt("SPHYNX");
		m_CoreLogger->set_level(spdlog::level::trace);

		m_ClientLogger = spdlog::stdout_color_mt("APP");
		m_ClientLogger->set_level(spdlog::level::trace);
	}

	void Log::Shutdown()
	{
		m_CoreLogger.reset();
		m_ClientLogger.reset();
	}
}
