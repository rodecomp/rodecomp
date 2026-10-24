#ifndef RAGNAROK_SOURCE_CONTENTS_MESSAGETABLE_H
#define RAGNAROK_SOURCE_CONTENTS_MESSAGETABLE_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <windows.h>

// STATIC: 0x016D5780
// GLOBAL: gl_24382
extern std::unordered_map<int, std::string> g_messageStrings;

// Original lookup reads locale globals and font owner's LANGID at +0xB4.
// Explicit inputs retain that dependency without inventing a font owner.
const char* LookupMessage(int id, bool languagePrefix, int serviceType, WORD language);
void InsertMessage(int id, const char* text);

// Original file acquisition remains CFile/archive work; this consumes the exact
// acquired CSV bytes and retains row/field/import behavior.
bool ImportMessageCsv(std::string_view bytes);
#endif
