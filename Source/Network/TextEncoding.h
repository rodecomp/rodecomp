#ifndef RAGNAROK_SOURCE_NETWORK_TEXTENCODING_H
#define RAGNAROK_SOURCE_NETWORK_TEXTENCODING_H

#include <string>
#include <windows.h>

// STATIC: 0x016D3A78
// GLOBAL: gl_24375
extern UINT g_textCodePage;

// Reconstructed names for original Win32/BSTR conversion operations.
std::string ToUtf8(std::string text);
std::string FromUtf8(std::string text);

// Distinct original helper uses string length and reports conversion failures
// as empty strings, unlike the earlier global-codepage helper's odd paths.
std::string FromUtf8(const std::string& text, UINT codePage);
std::string ToUtf8(const std::string& text, UINT codePage);

#endif
