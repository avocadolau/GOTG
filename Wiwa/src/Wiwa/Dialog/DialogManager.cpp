#include <wipch.h>

#include "DialogManager.h"

#include <Wiwa/core/Application.h>
//#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

//#define STB_TRUETYPE_IMPLEMENTATION 
#include "../vendor/stb/stb_truetype.h"



namespace Wiwa
{
	DialogManager::DialogManager()
	{

	}

	DialogManager::~DialogManager() {}

	bool DialogManager::Init(Scene* scene)
	{
		m_Scene = scene;
		
		//InitFont("assets/arial.ttf");
		//Test remove once done
		//InitFont("assets/arial.ttf","prueba1");
		return true;
	}


	bool DialogManager::Update()
	{
		
		
		return true;
	}

	bool DialogManager::Draw()
	{
		
		
		return false;
	}
	
	bool DialogManager::CleanUp()
	{
		

		return true;
	}

	Text* DialogManager::InitFont(const char* path, char* _word)
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
	
}
