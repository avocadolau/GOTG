#include <wipch.h>

#include "UiManager.h"

#include "UiButton.h"
#include "UiCheckbox.h"
#include "UiSlider.h"
#include "UiText.h"
#include "UiImage.h"
#include "UiBar.h"
#include "UiAbility.h"
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

	GuiManager::~GuiManager() {
		CleanUp();
	}

	bool GuiManager::Init(Scene* scene)
	{
		m_Scene = scene;
		
		//InitFont("assets/arial.ttf");
		//Test remove once done
		//InitFont("assets/arial.ttf","prueba1");
		return true;
	}

	GuiCanvas* GuiManager::CreateGuiCanvas(unsigned int id,bool active)
	{
		GuiCanvas* canvas_ = new GuiCanvas(m_Scene, id,active);

		canvas.push_back(canvas_);


		return canvas_;
	}
	GuiControl* GuiManager::CreateGuiControl_Simple(GuiControlType type, unsigned int id, Rect2i bounds,const char* path,const char* extraPath,unsigned int canvas_id,int callbackID, Rect2i boundsOriginTex, const char* audioEventName, bool active, bool animated, float framesAnim, std::vector<Rect2i> animRects)
	{
		GuiControl* control = nullptr;
	
			switch (type)
			{
			case GuiControlType::BUTTON:
				control = new GuiButton(m_Scene, id, bounds, path, extraPath,callbackID, boundsOriginTex,audioEventName,active,animated,framesAnim,animRects);
				break;
			case GuiControlType::CHECKBOX:
				control = new GuiCheckbox(m_Scene, id, bounds, path, extraPath, callbackID, boundsOriginTex, audioEventName, active);
				break;
			case GuiControlType::IMAGE:
				control = new GuiImage(m_Scene, id, bounds, path, callbackID, boundsOriginTex, active, animated, framesAnim, animRects);
				break;
			default:
				break;
			}
			if (control != nullptr) canvas.at(canvas_id)->controls.push_back(control);
			if (control->type != GuiControlType::IMAGE) canvas.at(canvas_id)->controlsForSelection.push_back(control);
	
		return control;
	}

	GuiControl* GuiManager::CreateGuiControl(GuiControlType type, unsigned int id, Rect2i bounds, const char* path, const char* slider_path, Rect2i sliderBounds, unsigned int canvas_id, int callbackID, Rect2i boundsOriginTex, Rect2i sliderOriginTex, const char* audioEventName, bool active)
	{
		GuiControl* control = nullptr;
		switch (type)
		{
		case Wiwa::GuiControlType::SLIDER:
			control = new GuiSlider(m_Scene, id, bounds, sliderBounds, path, slider_path, callbackID, boundsOriginTex, sliderOriginTex, audioEventName, active);
			canvas.at(canvas_id)->controls.push_back(control);
			canvas.at(canvas_id)->controlsForSelection.push_back(control);
			break;
		case Wiwa::GuiControlType::BAR:
			control = new GuiBar(m_Scene, id, bounds, sliderBounds, path, slider_path, boundsOriginTex, sliderOriginTex, active);
			canvas.at(canvas_id)->controls.push_back(control);
			break;
		default:
			break;
		}

		

		return control;
	}

	GuiControl* GuiManager::CreateGuiControl_Text(GuiControlType type, unsigned int id, Rect2i bounds, const char* string_text, unsigned int canvas_id, bool active)
	{
		GuiControl* control = nullptr;
		
		
			switch (type)
			{
			case GuiControlType::TEXT:
				control = new GuiText(m_Scene, id, bounds, string_text, active);
				break;
			default:
				break;
			}
			if (control != nullptr) canvas.at(canvas_id)->controls.push_back(control);
		
		return control;
	}

	GuiControl* GuiManager::CreateGuiControl_Ability(GuiControlType type, unsigned int id,unsigned int canvas_id,Rect2i bounds, const char* path, size_t callbackID, Rect2i boundsOriginTex, bool active,bool animated,std::vector<Rect2i> animationRects)
	{
		GuiControl* control = nullptr;

		control = new GuiAbility(m_Scene, id, bounds, path, callbackID, boundsOriginTex, active,animated, animationRects);
		canvas.at(canvas_id)->controls.push_back(control);
		return control;
	}

	
	bool GuiManager::Update()
	{
		
		std::vector<GuiCanvas*> canva = canvas;
		for (int i = 0; i < canva.size(); i++)
		{
			if (canva.at(i)->active)
			{
				canva.at(i)->Update();
				std::vector<GuiControl*> control = canva.at(i)->controls;
				for (int j = 0; j < control.size(); j++)
				{
					if (control.at(j)->active)
					{
						control.at(j)->Update();
					}
				}
			}
			
			
		}
		return true;
	}

	bool GuiManager::Draw()
	{
		size_t Csize = canvasToDestroy.size();
		for (size_t x = 0; x < Csize; x++)
		{
			RemoveCanvas(canvasToDestroy.at(x));
		}
		canvasToDestroy.clear();


		std::vector<GuiCanvas*> canva = canvas;
		for (int i = 0; i < canva.size(); i++)
		{
			size_t rsize = canvas.at(i)->controlsToDestroy.size();

			for (size_t k = 0; k < rsize; k++) {
				RemoveControl(canvas.at(i)->controlsToDestroy[k]);
			}

			canvas.at(i)->controlsToDestroy.clear();
		}

		for (int i = 0; i < canva.size(); i++)
		{
			if (canva.at(i)->active)
			{
				std::vector<GuiControl*> control = canva.at(i)->controls;

				for (int j = 0; j < control.size(); j++)
				{
					if (control.at(j)->active)
					{
						control.at(j)->Draw(&Wiwa::Application::Get().GetRenderer2D());
					}
				}

			}
		}
		
		return false;
	}
	void GuiManager::DestroyCanvas(GuiCanvas* canvas)
	{
		canvasToDestroy.push_back(canvas);
	}
	void GuiManager::DestroyGuiControl(GuiControl* control,GuiCanvas* canvas)
	{
		canvas->controlsToDestroy.push_back(control);
	}
	bool GuiManager::CleanUp()
	{
		for (size_t i = 0; i < canvas.size(); i++)
		{
			canvas.at(i)->controls.clear();
			canvas.at(i)->controlsForSelection.clear();
		}

		canvas.clear();
		return true;
	}

	void GuiManager::RemoveCanvas(GuiCanvas* canvasToDestroy)
	{
		for (int i = 0; i < canvasToDestroy->controls.size(); i++)
		{
			size_t ealive = canvasToDestroy->controls.size();
			Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
			r2d.RemoveInstance(m_Scene, canvasToDestroy->controls.at(i)->id_quad_normal);
			if (canvasToDestroy->controls.at(i)->type == GuiControlType::SLIDER)
			{
				r2d.RemoveInstance(m_Scene, canvasToDestroy->controls.at(i)->id_quad_extra);
			}
			if (canvasToDestroy->controls.at(i)->type == GuiControlType::BAR)
			{
				r2d.RemoveInstance(m_Scene, canvasToDestroy->controls.at(i)->id_quad_extra);
			}
			RemoveControl(canvasToDestroy->controls.at(i));
		}
		for (size_t j = 0; j < canvas.size(); j++)
		{
			if (canvas.at(j) == canvasToDestroy)
			{
				canvas.erase(canvas.begin() + j);
				break;
			}
		}
	}
	void GuiManager::RemoveControl(GuiControl* control)
	{
		std::vector<GuiCanvas*> canva = canvas;
		for (int i = 0; i < canva.size(); i++)
		{
			size_t ealive = canvas.at(i)->controls.size();
			Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
			if(control->type != GuiControlType::BAR)r2d.RemoveInstance(m_Scene, control->id_quad_normal);
			if (control->type == GuiControlType::SLIDER || control->type == GuiControlType::BAR)
			{
				r2d.RemoveInstance(m_Scene, control->id_quad_extra);
			}

			for (size_t j = 0; j < ealive; j++)
			{
				if (canvas.at(i)->controls.at(j) == control)
				{
					canvas.at(i)->controls.erase(canvas.at(i)->controls.begin() + j);
					break;
				}
			}

			size_t selectableAlive = canvas.at(i)->controlsForSelection.size();
			for (size_t k = 0; k < selectableAlive; k++)
			{
				if (canvas.at(i)->controlsForSelection.at(k) == control)
				{
					canvas.at(i)->controlsForSelection.erase(canvas.at(i)->controlsForSelection.begin() + k);
					break;
				}
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
		float scale = stbtt_ScaleForPixelHeight(&info, (float)l_h);

		char* word = _word;

		int x = 0;

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

		ascent = (int)round(ascent * scale);
		descent = (int)round(descent * scale);

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
			int byteOffset =(int)(x + roundf(lsb * scale) + (y * b_w));
			stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

			/* advance x */
			x += (int)roundf(ax * scale);

			/* add kerning */
			int kern;
			kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
			x += (int)roundf(kern * scale);
		}
	
		
		Text* text = new Text();
		text->Init(b_w, b_h, bitmap);

		free(fontBuffer);
		free(bitmap);

		return text;
	}

	Text* GuiManager::InitFontForDialog(const char* path, char* _word)
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

		int b_w = 1024; /* bitmap width */
		int b_h = 1024; /* bitmap height */
		int l_h = 64; /* line height */

		/* create a bitmap for the phrase */
		byte* bitmap = (byte*)calloc(b_w * b_h, sizeof(unsigned char));

		/* calculate font scaling */
		float scale = stbtt_ScaleForPixelHeight(&info, (float)l_h);

		char* word = _word;

		int x = 0;

		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

		ascent = (int)round(ascent * scale);
		descent = (int)round(descent * scale);

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
			int byteOffset = (int)(x + roundf(lsb * scale) + (y * b_w));
			stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

			/* advance x */
			x += (int)roundf(ax * scale);

			/* add kerning */
			int kern;
			kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
			x += (int)roundf(kern * scale);
		}


		Text* text = new Text();
		text->Init(b_w, b_h, bitmap);

		free(fontBuffer);
		free(bitmap);

		return text;
	}

	void GuiManager::SwapSelectedCanvas(GuiCanvas* canvasToSelect)
	{
		for (size_t i = 0; i < canvas.size(); i++)
		{
			canvas.at(i)->selected = false;
		}
		canvasToSelect->selected = true;
	}
	
	void GuiManager::LoadWiUI(const char* path)
	{
		if (!Wiwa::FileSystem::Exists(path)) return;

		File file = Wiwa::FileSystem::Open(path, FileSystem::OM_IN | FileSystem::OM_BINARY);

		if (file.IsOpen()) {
			_loadGUIImpl(file);
		}

		file.Close();

	}
	bool GuiManager::_loadGUIImpl(Wiwa::File& File)
	{
		std::vector<GuiCanvas*>& canvas = ReturnCanvas();
		size_t canvas_count;
		File.Read(&canvas_count, sizeof(size_t));
		for (size_t i = 0; i < canvas_count; i++)
		{
			int id_canvas;
			bool active_canvas;

			File.Read(&id_canvas, sizeof(int));
			File.Read(&active_canvas, 1);

			CreateGuiCanvas(id_canvas, active_canvas);

			std::vector<GuiControl*>& controls = canvas.at(i)->controls;
			size_t controls_count;
			File.Read(&controls_count, sizeof(size_t));
			//controls.resize(controls_count);
			for (size_t j = 0; j < controls_count; j++)
			{
				GuiControl* control = nullptr;
				int id;
				bool active;
				GuiControlType guiType;
				GuiControlState state;
				Rect2i position;
				std::string textureGui;
				std::string extraTextureGui;

				size_t textureGui_len;
				char* textureGui_c;
				size_t extraTextureGui_len;
				char* extraTextureGui_c;

				Rect2i extraPosition;

				Rect2i texturePosition;
				Rect2i extraTexturePosition;

				int callbackID;// = 1;

				std::string text;
				size_t textGuiLen;
				char* textGui_c;
				std::string audioEvent;
				size_t audioEventGuiLen;
				char* audioEventGui_c;

				bool animated;
				float animSpeed;
				size_t animRectsSize;
				std::vector<Rect2i> animRects;

				File.Read(&id, sizeof(int));
				File.Read(&active, 1);
				File.Read(&guiType, sizeof(GuiControlType));
				File.Read(&state, sizeof(GuiControlState));
				File.Read(&position, sizeof(Rect2i));
				File.Read(&callbackID, sizeof(int));
				File.Read(&animated, 1);
				File.Read(&animSpeed, sizeof(float));
				File.Read(&animRectsSize, sizeof(size_t));
				
				for (size_t counterForRects = 0; counterForRects < animRectsSize; counterForRects++)
				{
					Rect2i helperRect;
					File.Read(&helperRect, sizeof(Rect2i));
					animRects.push_back(helperRect);
				}

				File.Read(&extraPosition, sizeof(Rect2i));

				File.Read(&textGuiLen, sizeof(size_t));
				textGui_c = new char[textGuiLen];
				File.Read(textGui_c, textGuiLen);
				text = textGui_c;
				delete[] textGui_c;
				File.Read(&audioEventGuiLen, sizeof(size_t));
				audioEventGui_c = new char[audioEventGuiLen];
				File.Read(audioEventGui_c, audioEventGuiLen);
				audioEvent = audioEventGui_c;
				delete[] audioEventGui_c;

				File.Read(&textureGui_len, sizeof(size_t));
				textureGui_c = new char[textureGui_len];
				File.Read(textureGui_c, textureGui_len);
				textureGui = textureGui_c;
				delete[] textureGui_c;

				File.Read(&extraTextureGui_len, sizeof(size_t));
				extraTextureGui_c = new char[extraTextureGui_len];
				File.Read(extraTextureGui_c, extraTextureGui_len);
				extraTextureGui = extraTextureGui_c;
				delete[] extraTextureGui_c;

				File.Read(&texturePosition, sizeof(Rect2i));
				File.Read(&extraTexturePosition, sizeof(Rect2i));

				switch (guiType)
				{
				case Wiwa::GuiControlType::BUTTON:
					control = CreateGuiControl_Simple(guiType, id, position, textureGui.c_str(), extraTextureGui.c_str(), canvas.at(i)->id, callbackID, texturePosition, audioEvent.c_str(),active,animated,animSpeed,animRects);
					break;
				case Wiwa::GuiControlType::TEXT:
					control = CreateGuiControl_Text(guiType, id, position, text.c_str(), canvas.at(i)->id, active);
					break;
				case Wiwa::GuiControlType::CHECKBOX:
					control = CreateGuiControl_Simple(guiType, id, position, textureGui.c_str(), extraTextureGui.c_str(), canvas.at(i)->id, callbackID, texturePosition, audioEvent.c_str(), active, animated, animSpeed, animRects);
					break;
				case Wiwa::GuiControlType::SLIDER:
					control = CreateGuiControl(guiType, id, position, textureGui.c_str(), extraTextureGui.c_str(), extraPosition, canvas.at(i)->id, callbackID, texturePosition, extraTexturePosition, audioEvent.c_str(), active);
					break;
				case Wiwa::GuiControlType::BAR:
					control = CreateGuiControl(guiType, id, position, textureGui.c_str(), extraTextureGui.c_str(), extraPosition, canvas.at(i)->id, callbackID, texturePosition, extraTexturePosition, audioEvent.c_str(), active);
					break;
				case Wiwa::GuiControlType::IMAGE:
					control = CreateGuiControl_Simple(guiType, id, position, textureGui.c_str(), nullptr, canvas.at(i)->id, callbackID, texturePosition, audioEvent.c_str(), active, animated, animSpeed, animRects);
					break;
				case Wiwa::GuiControlType::ABILITY:
					control = CreateGuiControl_Ability(guiType, id,canvas.at(i)->id, position, textureGui.c_str(), callbackID, texturePosition, active, animated, animRects);
					break;
				default:
					break;
				}
			}
		}
		return true;
	}

	void GuiManager::SaveWiUI(const char* path)
	{
		File file = Wiwa::FileSystem::Open(path, FileSystem::OM_OUT | FileSystem::OM_BINARY);

		if (file.IsOpen()) {
			_saveGUIImpl(file);
		}

		file.Close();
	}
	bool GuiManager::_saveGUIImpl(Wiwa::File& File)
	{
		std::vector<GuiCanvas*>& canvas = ReturnCanvas();
		size_t canvas_count = canvas.size();

		// Save GuiControl count
		File.Write(&canvas_count, sizeof(size_t));

		for (size_t i = 0; i < canvas_count; i++)
		{
			GuiCanvas* canva = canvas.at(i);

			int id_canvas = canva->id;
			bool active_canvas = canva->active;

			File.Write(&id_canvas, sizeof(int));
			File.Write(&active_canvas, 1);

			size_t controls_count = canvas.at(i)->controls.size();
			File.Write(&controls_count, sizeof(size_t));
			for (size_t j = 0; j < controls_count; j++)
			{
				GuiControl* control = canvas.at(i)->controls.at(j);

				int id = control->id;
				bool active = control->GetActive();
				GuiControlType guiType = control->GetType();
				GuiControlState guiState = control->GetState();
				Rect2i position = control->GetPosition();
				int callbackID = control->callbackID;
				Rect2i texturePosition = control->texturePosition;
				Rect2i extraTexturePosition = control->extraTexturePosition;

				const char* textureGui = Wiwa::Resources::getResourcePathById<Wiwa::Image>(control->textId1);
				const char* extraTextureGui = Wiwa::Resources::getResourcePathById<Wiwa::Image>(control->textId2);

				size_t textureGui_len = strlen(textureGui) + 1;
				size_t extraTextureGui_len = strlen(extraTextureGui) + 1;

				const char* text = control->text.c_str();
				const char* audioEvent = control->audioEventForButton.c_str();

				size_t textGuiLen = strlen(text) + 1;
				size_t audioEventGuiLen = strlen(audioEvent) + 1;

				bool animated = control->animatedControl;
				float animSpeed = control->animSpeed;
				size_t animRectsSize = control->positionsForAnimations.size();
				std::vector<Rect2i> animRects = control->positionsForAnimations;

				File.Write(&id, sizeof(int));
				File.Write(&active, 1);
				File.Write(&guiType, sizeof(GuiControlType));
				File.Write(&guiState, sizeof(GuiControlState));
				File.Write(&position, sizeof(Rect2i));

				File.Write(&callbackID, sizeof(int));

				File.Write(&animated, 1);
				File.Write(&animSpeed, sizeof(float));
				File.Write(&animRectsSize, sizeof(size_t));

				for (size_t counterForRects = 0; counterForRects < animRectsSize; counterForRects++)
				{
					File.Write(&animRects.at(counterForRects), sizeof(Rect2i));
				}

				Rect2i extraPosition = control->GetExtraPosition();
				File.Write(&extraPosition, sizeof(Rect2i));

				File.Write(&textGuiLen, sizeof(size_t));
				File.Write(text, textGuiLen);

				File.Write(&audioEventGuiLen, sizeof(size_t));
				File.Write(audioEvent, audioEventGuiLen);

				// Save texture
				File.Write(&textureGui_len, sizeof(size_t));
				File.Write(textureGui, textureGui_len);
				// Save extraTexture
				File.Write(&extraTextureGui_len, sizeof(size_t));
				File.Write(extraTextureGui, extraTextureGui_len);

				File.Write(&texturePosition, sizeof(Rect2i));
				File.Write(&extraTexturePosition, sizeof(Rect2i));

			}
		}
		return true;
	}
	
}
