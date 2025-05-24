#pragma once

#include "Core/Core.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)


namespace Sphynx
{
	class SPHYNX_API Log
	{
	public:
		static void Init();
		static void Shutdown();

		/* Engine logging API */
		template<typename... Args>
		static void CoreTrace(Args&&... args);

		template<typename... Args>
		static void CoreDebug(Args&&... args);

		template<typename... Args>
		static void CoreDisplay(Args&&... args);

		template<typename... Args>
		static void CoreWarning(Args&&... args);

		template<typename... Args>
		static void CoreError(Args&&... args);

		template<typename... Args>
		static void CoreFatal(Args&&... args);

		/* Client logging API */
		template<typename... Args>
		static void ClientTrace(Args&&... args);

		template<typename... Args>
		static void ClientDebug(Args&&... args);

		template<typename... Args>
		static void ClientDisplay(Args&&... args);

		template<typename... Args>
		static void ClientWarning(Args&&... args);

		template<typename... Args>
		static void ClientError(Args&&... args);

		template<typename... Args>
		static void ClientFatal(Args&&... args);

	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;

	};

	template<typename ...Args>
	inline void Log::CoreTrace(Args&&... args)
	{
		m_CoreLogger->trace(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::CoreDebug(Args&&... args)
	{
		m_CoreLogger->debug(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::CoreDisplay(Args&&... args)
	{
		m_CoreLogger->info(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::CoreWarning(Args&&... args)
	{
		m_CoreLogger->warn(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::CoreError(Args&&... args)
	{
		m_CoreLogger->error(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::CoreFatal(Args&&... args)
	{
		m_CoreLogger->critical(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::ClientTrace(Args&&... args)
	{
		m_ClientLogger->trace(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::ClientDebug(Args&&... args)
	{
		m_ClientLogger->debug(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::ClientDisplay(Args&&... args)
	{
		m_ClientLogger->info(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::ClientWarning(Args&&... args)
	{
		m_ClientLogger->warn(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::ClientError(Args&&... args)
	{
		m_ClientLogger->error(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	inline void Log::ClientFatal(Args&&... args)
	{
		m_ClientLogger->critical(std::forward<Args>(args)...);
	}
}

#include "LogDefines.h"
