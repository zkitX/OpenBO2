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

parseInfo_t* Com_Parse(const char** data_p);
parseInfo_t* Com_ParseCSV(const char** data_p, int allowLineBreaks);
parseInfo_t* Com_ParseExt(const char** data_p, int allowLineBreaks);
ParseThreadInfo* Com_GetParseThreadInfo();
void Com_BeginParseSession(const char* filename);
void Com_EndParseSession();

class q_parse
{
};

