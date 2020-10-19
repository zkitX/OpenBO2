#include "q_parse.h"

ParseThreadInfo* Com_GetParseThreadInfo()
{
	return nullptr;
}

void Com_InitParse(void)
{
}

void Com_BeginParseSession(const char* filename)
{
}

void Com_EndParseSession()
{
}

void Com_ResetParseSessions()
{
}

void Com_SetSpaceDelimited(int)
{
}

void Com_SetKeepStringQuotes(int)
{
}

void Com_SetCSV(int)
{
}

void Com_SetParseNegativeNumbers(int)
{
}

int Com_GetCurrentParseLine()
{
	return 0;
}

void Com_SetScriptErrorPrefix(char const*)
{
}

char const* Com_GetScriptErrorPrefix(void)
{
	return nullptr;
}

void Com_SetScriptWarningPrefix(char const*)
{
}

char const* Com_GetScriptWarningPrefix(void)
{
	return nullptr;
}

void Com_ScriptErrorDrop(char const*, ...)
{
}

void Com_ScriptError(char const*, ...)
{
}

void Com_ScriptWarning(char const*, ...)
{
}

void Com_UngetToken(void)
{
}

void Com_ParseSetMark(char const**, com_parse_mark_t*)
{
}

void Com_ParseReturnToMark(char const**, com_parse_mark_t*)
{
}

int Com_Compress(char*)
{
	return 0;
}

char const* Com_GetLastTokenPos(void)
{
	return nullptr;
}

char const* Com_ParseCSV(const char** data_p, int allowLineBreaks)
{
	return nullptr;
}

char const* Com_ParseExt(const char** data_p, int allowLineBreaks)
{
	return nullptr;
}

char const* Com_Parse(const char** data_p)
{
	return nullptr;
}

char const* Com_ParseOnLine(char const**)
{
	return nullptr;
}

enum ParseTokenType Com_GetTokenType(void)
{
	return ParseTokenType();
}

int Com_MatchToken(char const**, char const*, int)
{
	return 0;
}

int Com_SkipBracedSection(char const**, unsigned int, int)
{
	return 0;
}

void Com_SkipRestOfLine(char const**)
{
}

int Com_GetArgCountOnLine(char const**)
{
	return 0;
}

char const* Com_ParseRestOfLine(char const**)
{
	return nullptr;
}

float Com_ParseFloat(char const**)
{
	return 0.0f;
}

float Com_ParseFloatOnLine(char const**)
{
	return 0.0f;
}

int Com_ParseInt(char const**)
{
	return 0;
}

int Com_ParseIntOnLine(char const**)
{
	return 0;
}

unsigned int Com_ParseUIntOnLine(char const**)
{
	return 0;
}

void Com_Parse1DMatrix(char const**, int, float*)
{
}
