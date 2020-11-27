#include "r_font.h"

#include <universal/assertive.h>

const Glyph* R_GetCharacterGlyph(Font_s*, unsigned int)
{
	return nullptr;
}

int R_GetKerningAmount(Font_s*, int, int)
{
	return 0;
}

unsigned int R_FontGetRandomLetter(Font_s*, int)
{
	return nullptr;
}

unsigned int R_FontGetRandomNumberCharacter(Font_s*, int)
{
	return nullptr;
}

Font_s* R_RegisterFont_LoadObj(char const*, int)
{
	return nullptr;
}

Font_s* R_RegisterFont_FastFile(char const*, int)
{
	return nullptr;
}

Font_s* R_RegisterFont(char const*, int)
{
	return nullptr;
}

float R_NormalizedTextScale(Font_s*, float)
{
	return 0.0f;
}

void GetButtonProperties(LocalClientNum_t, char const*, Font_s*, int*, int*)
{
}

int R_TextWidth(LocalClientNum_t, char const*, int, Font_s*)
{
	return 0;
}

int R_TextHeight(Font_s* font)
{
	if (!font && !Assert_MyHandler(__FILE__, __LINE__, 0, "(font)", nullptr))
		__debugbreak();
	return font->pixelHeight;
}

char* R_TextLineWrapPosition(char const*, int, int, Font_s*, float)
{
	return nullptr;
}

int R_ConsoleTextWidth(const char*, int, int, int, Font_s*)
{
	return 0;
}
