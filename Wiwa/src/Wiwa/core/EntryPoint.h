#pragma once

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Application.h>



#ifdef WI_PLATFORM_WINDOWS

// Make the application always use the GPU

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) DWORD NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif

extern Wiwa::Application* Wiwa::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	Wiwa::Log::Init();
	WI_CORE_INFO("Initialized Log!");

	auto app = Wiwa::CreateApplication(argc, argv);
	app->Run();

	delete app;
	
	quick_exit(EXIT_SUCCESS);
	return 0;
}
#endif // WI_PLATFORM_WINDOWS