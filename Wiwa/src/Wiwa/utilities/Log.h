#pragma once
#pragma warning(disable : 4251)
#pragma warning(disable : 4390)
#include <Wiwa/core/Core.h>

#include "imgui.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <Wiwa/imgui/ImGuiLog.h>

namespace Wiwa {
	class WI_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		static const char* GetCoreLastLog();
		static const char* GetClientLastLog();

		static void ImGuiLogTrace(const char* log);
		static void ImGuiLogInfo(const char* log);
		static void ImGuiLogWarn(const char* log);
		static void ImGuiLogError(const char* log);
		static void ImGuiLogCritical(const char* log);

		static void ImGuiConsoleTrace(const char* log);
		static void ImGuiConsoleInfo(const char* log);
		static void ImGuiConsoleWarn(const char* log);
		static void ImGuiConsoleError(const char* log);
		static void ImGuiConsoleCritical(const char* log);
	private:
		Log() = default;

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
#ifndef WI_DIST
//Core log macros
#define WI_CORE_TRACE(...)      ::Wiwa::Log::GetCoreLogger()->trace(__VA_ARGS__); ::Wiwa::Log::ImGuiLogTrace(::Wiwa::Log::GetCoreLastLog())
#define WI_CORE_INFO(...)       ::Wiwa::Log::GetCoreLogger()->info(__VA_ARGS__);  ::Wiwa::Log::ImGuiLogInfo(::Wiwa::Log::GetCoreLastLog())
#define WI_CORE_WARN(...)       ::Wiwa::Log::GetCoreLogger()->warn(__VA_ARGS__); ::Wiwa::Log::ImGuiLogWarn(::Wiwa::Log::GetCoreLastLog())
#define WI_CORE_ERROR(...)      ::Wiwa::Log::GetCoreLogger()->error(__VA_ARGS__);  ::Wiwa::Log::ImGuiLogError(::Wiwa::Log::GetCoreLastLog())
#define WI_CORE_CRITICAL(...)      ::Wiwa::Log::GetCoreLogger()->critical(__VA_ARGS__); ::Wiwa::Log::ImGuiLogCritical(::Wiwa::Log::GetCoreLastLog())
#else
#define WI_CORE_TRACE(...)   
#define WI_CORE_INFO(...)    
#define WI_CORE_WARN(...)    
#define WI_CORE_ERROR(...)   
#define WI_CORE_CRITICAL(...)
#endif

#define GLERR \
		{ \
        GLuint glerr;\
        while((glerr = glGetError()) != GL_NO_ERROR){\
            fprintf(stderr, "%s:%d glGetError() = 0x%04x\n", __FILE__, __LINE__, glerr); \
			assert(glerr == GL_NO_ERROR);	\
		}}

#ifdef WI_DEBUG
#define GL(_f)                     \
    gl##_f;                        \
    { \
	GLenum error = glGetError(); \
	if(error != GL_NO_ERROR) fprintf(stderr, "%s:%d glGetError() = 0x%04x\n", __FILE__, __LINE__, error); \
    assert(error == GL_NO_ERROR); }
#else  // NOT WI_DEBUG
#define GL(_f) gl##_f
#endif  // WI_DEBUG


#define GL_PTR_OFFSET(i) reinterpret_cast<void*>(static_cast<intptr_t>(i))

//Client log macros
#ifndef WI_DIST
#define WI_TRACE(...)           ::Wiwa::Log::GetClientLogger()->trace(__VA_ARGS__); ::Wiwa::Log::ImGuiConsoleTrace(::Wiwa::Log::GetClientLastLog())
#define WI_INFO(...)            ::Wiwa::Log::GetClientLogger()->info(__VA_ARGS__); ::Wiwa::Log::ImGuiConsoleInfo(::Wiwa::Log::GetClientLastLog())
#define WI_WARN(...)            ::Wiwa::Log::GetClientLogger()->warn(__VA_ARGS__); ::Wiwa::Log::ImGuiConsoleWarn(::Wiwa::Log::GetClientLastLog())
#define WI_ERROR(...)           ::Wiwa::Log::GetClientLogger()->error(__VA_ARGS__); ::Wiwa::Log::ImGuiConsoleError(::Wiwa::Log::GetClientLastLog())
#define WI_CRITICAL(...)           ::Wiwa::Log::GetClientLogger()->critical(__VA_ARGS__); ::Wiwa::Log::ImGuiConsoleCritical(::Wiwa::Log::GetClientLastLog())
#else
#define WI_TRACE(...)   
#define WI_INFO(...)    
#define WI_WARN(...)    
#define WI_ERROR(...)   
#define WI_CRITICAL(...)
#endif