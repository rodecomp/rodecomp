#ifndef RAGNAROK_SOURCE_CONTENTS_MESSAGETABLE_H
#define RAGNAROK_SOURCE_CONTENTS_MESSAGETABLE_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <windows.h>

// STATIC: 0x016D5780
// GLOBAL: gl_24382
extern std::unordered_map<int, std::string> g_messageStrings;

const char* LookupMessage(int id, bool languagePrefix, int serviceType, WORD language);
void InsertMessage(int id, const char* text);

bool ImportMessageCsv(std::string_view bytes);

#endif
