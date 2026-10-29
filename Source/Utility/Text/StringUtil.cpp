#include "Utility/Text/StringUtil.h"

#include <cstring>

// OFFSET: 0x0058A4D0
// FUNCTION: fn_02757
int CountCharacters(WORD codePage, const char* text, const char* end) {
  if (text == nullptr) {
    return 0;
  }

  int count = 0;
  while (*text != '\0' && (end == nullptr || text < end)) {
    text = CharNextExA(codePage, text, 0);
    if (end != nullptr && text > end) {
      break;
    }
    ++count;
  }
  return count;
}

// OFFSET: 0x0058A620
// FUNCTION: fn_02758
bool ContainsText(const std::string& text, std::string_view pattern) {
  return text.find(pattern.data(), 0, pattern.size()) != std::string::npos;
}

// OFFSET: 0x0058AAF0
// FUNCTION: fn_02761
bool IsNullOrEmpty(const char* text) {
  return text == nullptr || std::strlen(text) == 0;
}

// OFFSET: 0x0058AC30
// FUNCTION: fn_02762
int RemoveAll(std::string& text, std::string_view pattern) {
  return ReplaceAll(text, pattern, "");
}

// OFFSET: 0x0058AC60
// FUNCTION: fn_02763
int ReplaceAll(std::string& text, std::string_view pattern, std::string_view replacement) {
  int count = 0;
  std::size_t position = 0;
  while ((position = text.find(pattern.data(), position, pattern.size())) != std::string::npos) {
    text.replace(position, pattern.size(), replacement.data(), replacement.size());
    position += replacement.size();
    ++count;
  }
  return count;
}
