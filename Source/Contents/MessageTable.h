#ifndef RAGNAROK_SOURCE_CONTENTS_MESSAGETABLE_H
#define RAGNAROK_SOURCE_CONTENTS_MESSAGETABLE_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <windows.h>

enum MessageId : int {
  MSI_FAILED_ADD_TAB = 1578
};

extern std::unordered_map<int, std::string> g_messageStrings;
const char* LookupMessage(int id, bool languagePrefix, int serviceType, WORD language);
void InsertMessage(int id, const char* text);
int FindMessageId(const char* name);
char* DecodeMessageRot(char* str);
void ImportMessageTable();
bool IsObfuscatedMessage(int id);
const char* GetMessageString(int id);

bool ImportMessageCsv(std::string_view bytes);

#endif
