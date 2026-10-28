#include "UI/Text/NavigationLink.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

// OFFSET: 0x00833160
// FUNCTION: fn_10409
int DecodeNavigationNumber(std::string value) {
  constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const std::size_t length = value.size();
  std::uint32_t result = 0;

  for (std::size_t index = 0; index < length; ++index) {
    const char* digit = std::strchr(digits, value[index]);

    if (!digit || *digit == '\0') {
      continue;
    }

    std::uint32_t place = 1;

    for (std::size_t exponent = index + 1; exponent < length; ++exponent) {
      place *= 62;
    }

    result += static_cast<std::uint32_t>(digit - digits) * place;
  }

  return static_cast<int>(result);
}

// OFFSET: 0x00833270
// FUNCTION: fn_10410
std::string EncodeNavigationNumber(int value) {
  const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::vector<char> characters;

  do {
    characters.push_back(digits[value % 62]);
    value /= 62;
  } while (value >= 62);
  characters.push_back(digits[value]);

  std::string result;
  const std::size_t count = characters.size();
  for (std::size_t index = 0; index < count; ++index) {
    result += characters[count - 1 - index];
  }
  return result;
}

// OFFSET: 0x00835890
// FUNCTION: fn_10420
bool ParseNavigationLink(std::string value, std::string& name, int& x, int& y) {
  const auto end = value.find("</NAVIL>");

  if (end == std::string::npos) {
    return true;
  }

  const auto coordinates = end - 4;
  const std::string tail = value.substr(coordinates);
  x = DecodeNavigationNumber(tail.substr(0, 2));
  y = DecodeNavigationNumber(tail.substr(2, 2));
  const std::string head = value.substr(0, coordinates);
  const auto begin = head.find("<NAVIL>");

  if (begin != std::string::npos) {
    name = head.substr(begin + 7);
  }

  return !name.empty();
}

// OFFSET: 0x00835600
// FUNCTION: fn_10419
std::string FormatNavigationLink(std::string value, bool brackets) {
  std::string name;
  int x = 0, y = 0;
  ParseNavigationLink(value, name, x, y);
  char text[128] = {};

  if (x > 999 && y > 999) {
    std::sprintf(text, brackets ? "<%s>" : "%s", name.c_str());
  } else {
    std::sprintf(text, brackets ? "<%s %d,%d>" : "%s %d,%d", name.c_str(), x, y);
  }

  return text;
}

// OFFSET: 0x00833550
// FUNCTION: fn_10411
void DisplayNavigationLink(std::string& output, std::string value) {
  output = value;
  const auto begin = value.find("<NAVIL>");

  if (begin != std::string::npos) {
    output.clear();
    const std::string head = value.substr(0, begin);
    const std::string tail = value.substr(begin);
    const auto end = tail.find("</NAVIL>");

    if (end != std::string::npos) {
      const auto extent = end + 8;
      const std::string rest = tail.substr(extent);
      output = head;
      output += FormatNavigationLink(tail.substr(0, extent), true);
      output += rest;
    }
  }
}

// OFFSET: 0x00834980
// FUNCTION: fn_10417
void ExtractNavigationLink(std::string& output, std::string value) {
  output = value;
  const auto begin = value.find("<NAVIL>");

  if (begin != std::string::npos) {
    output = value.substr(begin);
    const auto end = output.find("</NAVIL>");

    if (end != std::string::npos) {
      output.resize(end + 8);
    }
  }
}
