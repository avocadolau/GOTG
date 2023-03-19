#include <wipch.h>
#include "embed_application_functions.h"

#include <Wiwa/core/Application.h>

void QuitIntr()
{
	Wiwa::Application::Get().Quit();
}
