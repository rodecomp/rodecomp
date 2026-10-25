#include "Contents/ForbiddenWords.h"
#include "File/CommonObject.h"

#include <atlconv.h>
#include <atlexcept.h>
#include <cctype>
#include <cstring>
#include <malloc.h>
#include <mbstring.h>
#include <windows.h>

namespace {
  // OFFSET: 0x00AE7060
  // FUNCTION: fn_18065
  char* Lowercase(char* text, unsigned short codePage) {
    for (char* current = text; current < text + std::strlen(text); current = CharNextExA(codePage, current, 0)) {
      if (*current > 0 && std::isupper(*current)) {
        *current += 32;
      }
    }

    return text;
  }
}

// OFFSET: 0x00AE6F30
// FUNCTION: fn_18064
bool ForbiddenWords::Load(const char* name, CCommonObject& storage, ResourceBytes& file) {
  std::vector<char*> tokens;
  const bool loaded = storage.LoadTokens(tokens, name, 1, file);

  for (char* token : tokens) {
    char* first = token;

    while (*first == ' ' || *first == '\t') {
      ++first;
    }

    if (*first != '\0') {
      this->words.push_back(Lowercase(token, this->codePage));
    }
  }

  return loaded;
}

// OFFSET: 0x00AE6D40
// FUNCTION: fn_18063
bool ForbiddenWords::Allows(const char* text) const {
  const std::size_t size = std::strlen(text) + 1;
  char* storage = static_cast<char*>(_malloca(size));

  if (!storage) {
    return false;
  }

  std::strncpy(storage, text, size);
  const char* current = Lowercase(storage, this->codePage);
  const char* end = current + std::strlen(current);

  while (current < end) {
    const auto* open = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>("<iteml>"));
    const auto* close = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>("</iteml>"));

    if (open && close && std::strlen(current) != 0 && std::strlen(current) <= std::strlen(reinterpret_cast<const char*>(open))) {
      const auto skip = reinterpret_cast<const char*>(close) + 8 - reinterpret_cast<const char*>(open);

      for (std::ptrdiff_t index = 0; index < skip; ++index) {
        current = CharNextExA(this->codePage, current, 0);
      }
    }

    for (const char* word : this->words) {
      if (std::strncmp(word, current, std::strlen(word)) == 0) {
        _freea(storage);
        return false;
      }
    }

    current = CharNextExA(this->codePage, current, 0);
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

  for (char& byte : lowercase) {
    byte = static_cast<char>(std::tolower(static_cast<unsigned char>(byte)));
  }

  strcpy_s(scanBuffer, sizeof(scanBuffer), text);
  char* current = Lowercase(scanBuffer, this->codePage);

  while (std::strlen(current) != 0) {
    const auto* open = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>("<iteml>"));
    const auto* close = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>("</iteml>"));

    if (open && close && std::strlen(current) != 0 && std::strlen(current) <= std::strlen(reinterpret_cast<const char*>(open))) {
      const auto skip = reinterpret_cast<const char*>(close) + 8 - reinterpret_cast<const char*>(open);

      char* next = current;

      for (std::ptrdiff_t index = 0; index < skip; ++index) {
        next = CharNextExA(this->codePage, current, 0);
      }

      current = next;
    }

    int advance = 1;

    for (const char* word : this->words) {
      if (std::strncmp(word, current, std::strlen(word)) != 0) {
        continue;
      }

      std::strcpy(matchedWord, word);
      Lowercase(matchedWord, this->codePage);
      advance = 0;
      const char* end = matchedWord + std::strlen(matchedWord);

      for (const char* character = matchedWord; character < end; character = CharNextExA(this->codePage, character, 0)) {
        ++advance;
      }

      const std::wstring lowerWide(ATL::CA2WEX<128>(lowercase.c_str(), CP_THREAD_ACP));
      const std::wstring wordWide(ATL::CA2WEX<128>(word, CP_THREAD_ACP));
      const std::wstring suffixWide(ATL::CA2WEX<128>(current, CP_THREAD_ACP));
      const std::size_t position = lowerWide.find(wordWide, lowerWide.size() - suffixWide.size());

      if (position == std::wstring::npos) {
        advance = 1;
        break;
      }

      const std::wstring original(ATL::CA2WEX<128>(text, CP_THREAD_ACP));
      std::wstring masked = original.substr(0, position);
      const std::wstring suffix = original.substr(position + wordWide.size());
      masked.append(static_cast<std::size_t>(advance), L'*');
      masked.append(suffix);
      const std::string converted(ATL::CW2AEX<128>(masked.c_str(), CP_THREAD_ACP));
      strcpy_s(text, std::strlen(text) + 1, converted.c_str());
      break;
    }

    for (int index = 0; index < advance; ++index) {
      current = CharNextExA(this->codePage, current, 0);
    }
  }

  if (fromString) {
    value->assign(valueBuffer, std::strlen(valueBuffer));
  }
}
