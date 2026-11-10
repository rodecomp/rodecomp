#include "Contents/Tables/ForbiddenWords.h"
#include "File/CommonObject.h"
#include "Network/TextEncoding.h"
#include "Utility/StackAlloc.h"

#include <algorithm>
#include <atlconv.h>
#include <atlexcept.h>
#include <cctype>
#include <cstring>
#include <malloc.h>
#include <mbstring.h>
#include <windows.h>

// GLOBAL: gl_24641
ForbiddenWords g_forbiddenWords;

// GLOBAL: gl_24642
static CCommonObject g_forbiddenWordsObject;

// OFFSET: 0x00AE5E50
// FUNCTION: fn_18060
ForbiddenWords::~ForbiddenWords() {
}

namespace {
  // INLINED
  char* LowercaseWithCodePage(char* text, unsigned short codePage) {
    for (char* current = text; current < text + std::strlen(text); current = CharNextExA(codePage, current, 0)) {
      if (*current > 0 && std::isupper(*current)) {
        *current += 32;
      }
    }

    return text;
  }

  // OFFSET: 0x00AE7060
  // FUNCTION: fn_18065
  __declspec(noinline) char* Lowercase(char* text) {
    for (char* current = text; current < text + std::strlen(text); current = CharNextExA(static_cast<unsigned short>(g_textCodePage), current, 0)) {
      if (*current > 0 && std::isupper(*current)) {
        *current += 32;
      }
    }

    return text;
  }
}

// OFFSET: 0x00AE6F30
// FUNCTION: fn_18064
bool ForbiddenWords::Load(const char* name) {
  std::vector<char*> tokens;
  const bool loaded = g_forbiddenWordsObject.LoadTokens(tokens, name, 1);

  for (std::vector<char*>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
    char* token = *it;
    char* first = token;

    while (*first == ' ' || *first == '\t') {
      ++first;
    }

    char* last = first;

    if (*last != '\0') {
      while (*++last != '\0') {
      }
    }

    *last = '\0';

    if (*first != '\0') {
      this->words.push_back(Lowercase(token));
    }
  }

  return loaded;
}

// GLOBAL: gl_24643
static int g_forbiddenWordCloseTagLength = 8;
// GLOBAL: gl_24644
static const std::string g_forbiddenWordOpenTag = "<iteml>";
// GLOBAL: gl_24645
static const std::string g_forbiddenWordCloseTag = "</iteml>";

// OFFSET: 0x00AE6D40
// FUNCTION: fn_18063
bool ForbiddenWords::Allows(const char* text) const {
  const std::size_t size = std::strlen(text) + 1;
  char* storage = static_cast<char*>(STACK_ALLOC(size));
  if (!storage) {
    return false;
  }
  std::strncpy(storage, text, size);
  const char* current = Lowercase(storage);
  const char* end = current + std::strlen(current);

  while (current < end) {
    const auto* open = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>(g_forbiddenWordOpenTag.c_str()));
    const auto* close = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>(g_forbiddenWordCloseTag.c_str()));
    if (open && close && std::strlen(current) != 0 && std::strlen(current) <= std::strlen(reinterpret_cast<const char*>(open))) {
      const auto skip = reinterpret_cast<const char*>(close) + g_forbiddenWordCloseTagLength - reinterpret_cast<const char*>(open);

      for (std::ptrdiff_t index = 0; index < skip; ++index) {
        current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
      }
    }
    for (auto it = this->words.begin(); it != this->words.end(); ++it) {
      if (std::strncmp(*it, current, std::strlen(*it)) == 0) {
        _freea(storage);
        return false;
      }
    }

    current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
  }
  _freea(storage);
  return true;
}

// OFFSET: 0x00AE6D20
// FUNCTION: fn_18062
bool ForbiddenWords::Contains(const char* text) const {
  return !this->Allows(text);
}

// OFFSET: 0x00AE6050
// FUNCTION: fn_18061
void ForbiddenWords::Mask(char* text, std::string* value) const {
  // STATIC: 0x016D4E60
  // GLOBAL: gl_24379
  static char valueBuffer[2048];
  // STATIC: 0x016D4660
  // GLOBAL: gl_24378
  static char scanBuffer[2048];
  // STATIC: 0x016D4558
  // GLOBAL: gl_24377
  static char matchedWord[264];
  const bool fromString = text == nullptr;

  if (fromString) {
    strcpy_s(valueBuffer, sizeof(valueBuffer), value->c_str());
    text = valueBuffer;
  }

  std::string lowercase(text);

  std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), [](unsigned char byte) { return static_cast<char>(std::tolower(byte)); });

  strcpy_s(scanBuffer, sizeof(scanBuffer), text);
  char* current = Lowercase(scanBuffer);

  while (std::strlen(current) != 0) {
    const auto* open = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>(g_forbiddenWordOpenTag.c_str()));
    const auto* close = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>(g_forbiddenWordCloseTag.c_str()));

    if (open && close && std::strlen(current) != 0 && std::strlen(current) <= std::strlen(reinterpret_cast<const char*>(open))) {
      const auto skip = reinterpret_cast<const char*>(close) + g_forbiddenWordCloseTagLength - reinterpret_cast<const char*>(open);

      for (std::ptrdiff_t index = 0; index < skip; ++index) {
        current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
      }
    }

    int advance = 1;

    for (const char* word : this->words) {
      if (std::strncmp(word, current, std::strlen(word)) != 0) {
        continue;
      }

      std::strcpy(matchedWord, word);
      LowercaseWithCodePage(matchedWord, static_cast<unsigned short>(g_textCodePage));
      advance = 0;
      const char* end = matchedWord + std::strlen(matchedWord);

      for (const char* character = matchedWord; character < end; character = CharNextExA(static_cast<WORD>(g_textCodePage), character, 0)) {
        ++advance;
      }

      std::wstring wide(ATL::CA2WEX<128>(lowercase.c_str(), CP_THREAD_ACP));
      const std::wstring wordWide(ATL::CA2WEX<128>(word, CP_THREAD_ACP));
      const std::size_t position = wide.find(wordWide, wide.size() - std::wstring(ATL::CA2WEX<128>(current, CP_THREAD_ACP)).size());

      if (position == std::wstring::npos) {
        advance = 1;
        break;
      }

      wide = std::wstring(ATL::CA2WEX<128>(text, CP_THREAD_ACP));
      const std::wstring prefix = wide.substr(0, position);
      const std::wstring suffix = wide.substr(position + std::wcslen(wordWide.c_str()));
      wide = prefix;
      for (int index = 0; index < advance; ++index) {
        wide += L"*";
      }
      wide += suffix;
      const std::string converted(ATL::CW2AEX<128>(wide.c_str(), CP_THREAD_ACP));
      strcpy_s(text, std::strlen(text) + 1, converted.c_str());
      break;
    }

    for (int index = 0; index < advance; ++index) {
      current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    }
  }

  if (fromString) {
    value->assign(valueBuffer, std::strlen(valueBuffer));
  }
}
