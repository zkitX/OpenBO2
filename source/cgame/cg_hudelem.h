#pragma once

union hudelem_color_t
{
	struct {
		char r;
		char g;
		char b;
		char a;
	};
	int rgba;
};

struct hudelem_s
{
	float x;
	float y;
	float z;
	float fontScale;
	float fromFontScale;
	int fontScaleStartTime;
	hudelem_color_t color;
	hudelem_color_t fromColor;
	int fadeStartTime;
	int scaleStartTime;
	float fromX;
	float fromY;
	int moveStartTime;
	int time;
	int duration;
	float value;
	float sort;
	hudelem_color_t glowColor;
	int fxBirthTime;
	unsigned int flags;
	__int16 targetEntNum;
	__int16 fontScaleTime;
	__int16 fadeTime;
	__int16 label;
	__int16 width;
	__int16 height;
	__int16 fromWidth;
	__int16 fromHeight;
	__int16 scaleTime;
	__int16 moveTime;
	__int16 text;
	unsigned __int16 fxLetterTime;
	unsigned __int16 fxDecayStartTime;
	unsigned __int16 fxDecayDuration;
	unsigned __int16 fxRedactDecayStartTime;
	unsigned __int16 fxRedactDecayDuration;
	char type;
	char font;
	char alignOrg;
	char alignScreen;
	char materialIndex;
	char offscreenMaterialIdx;
	char fromAlignOrg;
	char fromAlignScreen;
	char soundID;
	char ui3dWindow;
};

class cg_hudelem
{
};

