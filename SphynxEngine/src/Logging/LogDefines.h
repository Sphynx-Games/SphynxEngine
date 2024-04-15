#pragma once

#define SPX_CORE_LOG_TRACE(...)		::Sphynx::Log::CoreTrace(__VA_ARGS__)
#define SPX_CORE_LOG_DEBUG(...)		::Sphynx::Log::CoreDebug(__VA_ARGS__)
#define SPX_CORE_LOG_DISPLAY(...)	::Sphynx::Log::CoreDisplay(__VA_ARGS__)
#define SPX_CORE_LOG_WARNING(...)	::Sphynx::Log::CoreWarning(__VA_ARGS__)
#define SPX_CORE_LOG_ERROR(...)		::Sphynx::Log::CoreError(__VA_ARGS__)
#define SPX_CORE_LOG_FATAL(...)		::Sphynx::Log::CoreFatal(__VA_ARGS__)

#define SPX_LOG_TRACE(...)			::Sphynx::Log::ClientTrace(__VA_ARGS__)
#define SPX_LOG_DEBUG(...)			::Sphynx::Log::ClientDebug(__VA_ARGS__)
#define SPX_LOG_DISPLAY(...)		::Sphynx::Log::ClientDisplay(__VA_ARGS__)
#define SPX_LOG_WARNING(...)		::Sphynx::Log::ClientWarning(__VA_ARGS__)
#define SPX_LOG_ERROR(...)			::Sphynx::Log::ClientError(__VA_ARGS__)
#define SPX_LOG_FATAL(...)			::Sphynx::Log::ClientFatal(__VA_ARGS__)
