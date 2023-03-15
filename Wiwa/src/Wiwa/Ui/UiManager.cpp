#include <wipch.h>

#include "UiManager.h"

#include "UiButton.h"
#include "UiCheckbox.h"
#include "UiSlider.h"
#include "UiText.h"
#include "UiImage.h"
#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "../vendor/stb/stb_truetype.h"



namespace Wiwa
{
	GuiManager::GuiManager()
	{

	}

	GuiManager::~GuiManager() {}

	bool GuiManager::Init(Scene* scene)
	{
		m_Scene = scene;
		//InitFont("assets/arial.ttf");
		//Test remove once done
		//InitFont("assets/arial.ttf","prueba1");
		return true;
	}

	GuiControl* GuiManager::CreateGuiControl_Simple(GuiControlType type, unsigned int id, Rect2i bounds,const char* path,const char* extraPath)
	{
		GuiControl* control = nullptr;
	
			switch (type)
			{
			case GuiControlType::BUTTON:
				control = new GuiButton(m_Scene, id, bounds, path, extraPath);
				break;
			case GuiControlType::CHECKBOX:
				control = new GuiCheckbox(m_Scene, id, bounds, path, extraPath);
				break;
			case GuiControlType::IMAGE:
				control = new GuiImage(m_Scene, id, bounds, path);
				break;
			default:
				break;
			}
			if (control != nullptr) controls.push_back(control);
	
		return control;
	}

	GuiControl* GuiManager::CreateGuiControl(GuiControlType type, unsigned int id, Rect2i bounds, const char* path, const char* slider_path, Rect2i sliderBounds)
	{
		GuiControl* control = nullptr;
		
		
			switch (type)
			{
			case GuiControlType::BUTTON:
				control = new GuiButton(m_Scene, id, bounds, path, slider_path);
				break;
			case GuiControlType::SLIDER:
				control = new GuiSlider(m_Scene, id, bounds, sliderBounds, path, slider_path);
				break;
			case GuiControlType::CHECKBOX:
				control = new GuiCheckbox(m_Scene, id, bounds, path, slider_path);
				break;
			default:
				break;
			}
			if (control != nullptr) controls.push_back(control);
		
		return control;
	}

	GuiControl* GuiManager::CreateGuiControl_Text(GuiControlType type, unsigned int id, Rect2i bounds, const char* string_text)
	{
		GuiControl* control = nullptr;
		
		
			switch (type)
			{
			case GuiControlType::TEXT:
				control = new GuiText(m_Scene, id, bounds, string_text);
				break;
			default:
				break;
			}
			//Set the observer
			//control->SetObserver(observer);

			// Created GuiControls are added to the list of controls
			if (control != nullptr) controls.push_back(control);
		
		return control;
	}

	bool GuiManager::Update()
	{
		if (Time::IsPlaying())
		{
			std::vector<GuiControl*> control = controls;
			for (int i = 0; i < control.size(); i++)
			{
				if (control.at(i)->active)
				{
					control.at(i)->Update();
				}
			}
		}
		
		
		size_t rsize = controlsToDestroy.size();

		for (size_t i = 0; i < rsize; i++) {
			RemoveControl(controlsToDestroy[i]);
		}

		controlsToDestroy.clear();

		return true;
	
	}

	bool GuiManager::Draw()
	{
		std::vector<GuiControl*> control = controls;
		
		for (int i = 0; i < control.size(); i++)
		{
			control.at(i)->Draw(&Wiwa::Application::Get().GetRenderer2D());
		}
		return true;

	}
	void GuiManager::DestroyGuiControl(GuiControl* control)
	{
		controlsToDestroy.push_back(control);
	}
	bool GuiManager::CleanUp()
	{
		std::vector<GuiControl*> control = controls;

		for (int i = 0; i < control.size(); i++)
		{
			//control.erase(i);
		}

		return true;
	}

	void GuiManager::RemoveControl(GuiControl* control)
	{
		size_t ealive = controls.size();
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		r2d.RemoveInstance(m_Scene, control->id_quad_normal);
		if (control->type == GuiControlType::SLIDER)
		{
			r2d.RemoveInstance(m_Scene, control->id_quad_extra);
		}
				
		for (size_t i = 0; i < ealive; i++)
		{
			if (controls.at(i)->id == control->id)
			{
				controls.erase(controls.begin() + i);
				break;
			}
		}
			
		
	}

	Text* GuiManager::InitFont(const char* path, char* _word)
	{
		
		/* load font file */
		long size;
		byte* fontBuffer;
		
		//Extracted from https://github.com/justinmeiners/stb-truetype-example/blob/master/main.c
		//Read the fontFile lines 19 to 27
		FILE* fontFile = fopen(path, "rb");
		if (!fontFile)
		{
			WI_CORE_INFO("Seems like font at {} doesn't exist", path);
			return false;
		}
		fseek(fontFile, 0, SEEK_END);
		size = ftell(fontFile);
		fseek(fontFile, 0, SEEK_SET);

		fontBuffer = (byte*)malloc(size);

		fread(fontBuffer, size, 1, fontFile);
		fclose(fontFile);


		/* prepare font */
		stbtt_fontinfo info;
		if (!stbtt_InitFont(&info, fontBuffer, 0))
		{
			WI_CORE_ERROR("Failed to load font at {}", path);
			return false;
		}

		int b_w = 512; /* bitmap width */
		int b_h = 512; /* bitmap height */
		int l_h = 64; /* line height */

		/* create a bitmap for the phrase */
		byte* bitmap = (byte*)calloc(b_w * b_h, sizeof(unsigned char));

		/* calculate font scaling */
		float scale = stbtt_ScaleForPixelHeight(&info, l_h);

		
		char* word = _word;

		int x = 0;

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

		ascent = roundf(ascent * scale);
		descent = roundf(descent * scale);

		int i;
		for (i = 0; i < strlen(word); ++i)
		{
			/* how wide is this character */
			int ax;
			int lsb;
			stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);
			/* (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].) */

			/* get bounding box for character (may be offset to account for chars that dip above or below the line) */
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

			/* compute y (different characters have different heights) */
			int y = ascent + c_y1;

			/* render character (stride and offset is important here) */
			int byteOffset = x + roundf(lsb * scale) + (y * b_w);
			stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

			/* advance x */
			x += roundf(ax * scale);

			/* add kerning */
			int kern;
			kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
			x += roundf(kern * scale);
		}
	
		
		Text* text = new Text();
		text->Init(b_w, b_h, bitmap);

		free(fontBuffer);
		free(bitmap);

		return text;
	}

	
}
