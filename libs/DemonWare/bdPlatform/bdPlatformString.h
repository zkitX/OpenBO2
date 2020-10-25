#pragma once

unsigned int bdStrGetToken(const char* const str, const char* const delimeters, char* const tokenBuffer, const unsigned int tokenBufferSize, const char** end);
bool bdDelimSubstr(const char* str, const char* substr, const char* const delimiters);