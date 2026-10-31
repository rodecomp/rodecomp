#include "Utility/Text/StringUtil.h"
#include "Archive/GPak.h"
#include "Archive/PakCipher.h"
#include "Utility/Threading/NSyncObject.h"
#include "Utility/Compression/Zlib.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <ranges>
#include <sstream>

// OFFSET: 0x00AEB710
// FUNCTION: fn_18127
bool CGPak::Extract(const myGPack::PakEntry* entry, void* buffer) {
  NSyncObject lock(&this->m_criticalSection);
  const std::uint8_t* mapped = this->m_file->Read(entry->dataOffset + 46, entry->alignedSize);
  if (!mapped) {
    return false;
  }
  if (static_cast<signed char>(entry->flags) < 0) {
    return this->m_auxiliary.Extract(
      std::string_view(entry->name.m_name), mapped, entry->alignedSize, this->m_scratch, buffer, entry->originalSize, 0
    );
  }

  int decodedLength = 0;
  const char* name = entry->name.m_name;
  const char* end = name + std::strlen(name);
  const char* basename = name;
  for (const char* current = name; current < end; ++current) {
    if (*current == '\\' || *current == '/') {
      basename = current + 1;
    }
  }
  const int nameLength = static_cast<int>(end - basename);
  std::uint8_t key[8];
  key[0] = 'n';
  key[7] = 'k';
  if (nameLength < 6) {
    std::memcpy(key + 1, basename, nameLength);
    std::memcpy(key + 1 + nameLength, basename, 6 - nameLength);
  } else {
    std::memcpy(key + 1, basename, 6);
  }
  myGPack::PakCipher cipher(key);
  const std::uint8_t* source;
  if (entry->flags & 2) {
    const std::uint32_t size = entry->compressedSize;
    int cycle;
    if (size >= 1000000000u) {
      cycle = 10;
    } else if (size >= 100000000u) {
      cycle = 9;
    } else if (size >= 10000000u) {
      cycle = 8;
    } else if (size >= 1000000u) {
      cycle = 7;
    } else if (size >= 100000u) {
      cycle = 6;
    } else if (size >= 10000u) {
      cycle = 5;
    } else if (size >= 1000u) {
      cycle = 4;
    } else if (size >= 100u) {
      cycle = 3;
    } else {
      cycle = size >= 10u ? 2 : 1;
    }
    cipher.Decode(mapped, static_cast<int>(entry->alignedSize), this->m_scratch.empty() ? nullptr : this->m_scratch.data(), &decodedLength, cycle);
    source = this->m_scratch.empty() ? nullptr : this->m_scratch.data();
  } else if (entry->flags & 4) {
    cipher.DecodePrefix(mapped, static_cast<int>(entry->alignedSize), this->m_scratch.empty() ? nullptr : this->m_scratch.data(), &decodedLength);
    source = this->m_scratch.empty() ? nullptr : this->m_scratch.data();
  } else {
    decodedLength = entry->compressedSize;
    source = mapped;
  }
  if (entry->flags & 1) {
    unsigned long outputSize = entry->originalSize;
    uncompress(static_cast<std::uint8_t*>(buffer), &outputSize, source, decodedLength);
  } else {
    std::memcpy(buffer, source, entry->originalSize);
  }
  return true;
}

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
