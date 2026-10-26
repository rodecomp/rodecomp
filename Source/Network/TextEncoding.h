#ifndef RAGNAROK_SOURCE_NETWORK_TEXTENCODING_H
#define RAGNAROK_SOURCE_NETWORK_TEXTENCODING_H

#include <string>
#include <windows.h>

extern UINT g_textCodePage;

std::string ToUtf8(std::string text);
std::string FromUtf8(std::string text);

std::string FromUtf8(const std::string& text, UINT codePage);
std::string ToUtf8(const std::string& text, UINT codePage);

void LowercaseText(std::string& text);
std::wstring DecodeText(UINT codePage, const std::string& text);
std::string EncodeText(UINT codePage, const std::wstring& text);

#endif
