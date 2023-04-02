#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/render/Text.h>
#include <vector>
#include <string>
#include <Wiwa/core/Input.h>



namespace Wiwa
{
	class WI_API DialogManager
	{
	private:
		Scene* m_Scene;
	public:

		// Constructor
		DialogManager();

		// Destructor
		virtual ~DialogManager();

	
		// Init gui manager
		bool Init(Scene* scene);

		// Update gui manager
		bool Update();

		bool Draw();

		// Called before quitting
		bool CleanUp();


		Text* InitFont(const char* path,char* word);


	public:


	};
}