#include <wipch.h>
#include "embed_screen_functions.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/core/Window.h>

void SetFullscreenIntr(bool value)
{
	Wiwa::Application::Get().GetWindow().SetFullScreen(value);
}

bool GetFullscreenIntr()
{
	return Wiwa::Application::Get().GetWindow().GetFullScreen();
}

void SetVsyncIntr(bool value)
{
	Wiwa::Application::Get().GetWindow().SetVSync(value);
}

bool GetVsyncIntr()
{
	return Wiwa::Application::Get().GetWindow().IsVSync();;
}

void SetResizableIntr(bool value)
{
	Wiwa::Application::Get().GetWindow().SetResizable(value);
}

bool GetResizableIntr()
{
	return Wiwa::Application::Get().GetWindow().GetResizable();;
}

void SetResolutionIntr(int width, int height)
{
}

int GetWidthIntr()
{
	return Wiwa::Application::Get().GetWindow().GetWidth();
}

int GetHeightIntr()
{
	return Wiwa::Application::Get().GetWindow().GetHeight();
}
