#pragma once

#include "../qcommon/common.h"

static struct {
	char* language;
	char* strings;
} localization;

char language_buffer[4096];

_iobuf* Win_InitLocalization(bool isZombies);
void Win_ShutdownLocalization();
char* Win_LocalizeRef(const char* ref);
char const* Win_GetLanguage();