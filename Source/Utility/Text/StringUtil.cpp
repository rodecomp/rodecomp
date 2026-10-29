#include "Utility/Text/StringUtil.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <ranges>
#include <sstream>

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

// OFFSET: 0x0058A8F0
// FUNCTION: fn_02759
std::string GetFileName(const std::string& path) {
  std::size_t position = path.find_last_of("\\/");
  if (position == std::string::npos) {
    return path;
  }
  return path.substr(position + 1);
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

// OFFSET: 0x0058AE30
// FUNCTION: fn_02764
std::string FormatDecimal(double value, int precision, bool trimZeros) {
  double scale = std::pow(10.0, precision);
  double inverseScale = std::pow(0.1, precision);
  double rounded = std::round(value * scale) * inverseScale;

  std::ostringstream stream;
  stream << std::fixed << std::setprecision(precision) << rounded;
  std::string result = stream.str();

  if (trimZeros) {
    result.erase(result.find_last_not_of('0') + 1);
    if (result.back() == '.') {
      result.pop_back();
    }
  }
  return result;
}

// OFFSET: 0x0058B170
// FUNCTION: fn_02765
std::vector<std::string> Split(std::string_view text, std::string_view delimiter) {
  std::vector<std::string> result;
  for (auto part : text | std::views::split(delimiter)) {
    result.emplace_back(std::string_view(part.begin(), part.end()));
  }
  return result;
}

// OFFSET: 0x0058B830
// FUNCTION: fn_02766
void TrimRight(std::string& text, std::string_view characters) {
  if (characters.empty()) {
    auto last = std::find_if(text.rbegin(), text.rend(), [](unsigned char c) { return !std::isspace(c); });
    text.erase(last.base(), text.end());
    return;
  }
  text.erase(text.find_last_not_of(characters) + 1);
}

// OFFSET: 0x0058BA30
// FUNCTION: fn_02767
void TrimLeft(std::string& text, std::string_view characters) {
  if (characters.empty()) {
    auto first = std::find_if(text.begin(), text.end(), [](unsigned char c) { return !std::isspace(c); });
    text.erase(text.begin(), first);
    return;
  }
  text.erase(0, text.find_first_not_of(characters));
}
