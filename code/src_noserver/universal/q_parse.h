#pragma once

enum ParseThreadContext
{
	PARSE_THREAD_MAIN = 0x0,
	PARSE_THREAD_BACKEND = 0x1,
	PARSE_THREAD_SERVER = 0x2,
	PARSE_THREAD_FASTFILE = 0x3,
	PARSE_THREAD_COUNT = 0x4,
};

enum ParseTokenType
{
	PARSE_TOKEN_UNKNOWN = 0x0,
	PARSE_TOKEN_NUMBER = 0x1,
	PARSE_TOKEN_STRING = 0x2,
	PARSE_TOKEN_NAME = 0x3,
	PARSE_TOKEN_HASH = 0x4,
	PARSE_TOKEN_PUNCTUATION = 0x5,
};

struct parseInfo_t
{
	char token[1024];
	ParseTokenType tokenType;
	int lines;
	bool ungetToken;
	bool spaceDelimited;
	bool keepStringQuotes;
	bool csv;
	bool negativeNumbers;
	bool numbers;
	const char* extraWordCharacters;
	const char* errorPrefix;
	const char* warningPrefix;
	int backup_lines;
	const char* backup_text;
	const char* parseFile;
};

struct ParseThreadInfo
{
	parseInfo_t parseInfo[16];
	int parseInfoNum;
	const char* tokenPos;
	const char* prevTokenPos;
	char line[1024];
};

ParseThreadInfo g_parse[4];
const char* punctuation[17];

ParseThreadInfo* Com_GetParseThreadInfo();
void Com_InitParse(void);
void Com_BeginParseSession(const char* filename);
void Com_EndParseSession();
void Com_ResetParseSessions();
void Com_SetSpaceDelimited(int);
void Com_SetKeepStringQuotes(int);
void Com_SetCSV(int);
void Com_SetParseNegativeNumbers(int);
int Com_GetCurrentParseLine();
void Com_SetScriptErrorPrefix(char const*);
char const* Com_GetScriptErrorPrefix(void);
void Com_SetScriptWarningPrefix(char const*);
char const* Com_GetScriptWarningPrefix(void);
void Com_ScriptErrorDrop(char const*, ...);
void Com_ScriptError(char const*, ...);
void Com_ScriptWarning(char const*, ...);
void Com_UngetToken(void);
void Com_ParseSetMark(char const**, struct com_parse_mark_t*);
void Com_ParseReturnToMark(char const**, struct com_parse_mark_t*);
int Com_Compress(char*);
char const* Com_GetLastTokenPos(void);
char const* Com_ParseCSV(const char** data_p, int allowLineBreaks);
char const* Com_ParseExt(const char** data_p, int allowLineBreaks);
char const* Com_Parse(const char** data_p);
char const* Com_ParseOnLine(char const**);
enum ParseTokenType Com_GetTokenType(void);
int Com_MatchToken(char const**, char const*, int);
int Com_SkipBracedSection(char const**, unsigned int, int);
void Com_SkipRestOfLine(char const**);
int Com_GetArgCountOnLine(char const**);
char const* Com_ParseRestOfLine(char const**);
float Com_ParseFloat(char const**);
float Com_ParseFloatOnLine(char const**);
int Com_ParseInt(char const**);
int Com_ParseIntOnLine(char const**);
unsigned int Com_ParseUIntOnLine(char const**);
void Com_Parse1DMatrix(char const**, int, float*);