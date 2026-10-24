#include "Contents/TipBoxTitles.h"

#include <windows.h>

namespace {
  // OFFSET: 0x009B45D0
  // FUNCTION: fn_13832
  void SplitTitle(const char* text, std::vector<std::string>& lines, int width, unsigned short codePage) {
    const char* begin = text;
    const char* current = text;
    const char* end = text + std::char_traits<char>::length(text);
    const char* limit = text + width;

    while (current < end) {
      if (current >= limit || *current == '\n') {
        lines.emplace_back(begin, current);
        begin = current;
        limit = current + width;
      }

      current = CharNextExA(codePage, current, 0);
    }

    // Original intentionally does not append the final partial segment.
  }
}

// OFFSET: 0x007ABEC0
// FUNCTION: fn_09118
TipBoxEntry::TipBoxEntry() : unknown_0018(0), unknown_0044(0), unknown_0048(0) {
  this->unknown_004C[0] = 0;
  this->unknown_004C[1] = 0;
  this->unknown_004C[2] = 0;
}

// OFFSET: 0x007ACC70
// FUNCTION: fn_09127
std::string TipBoxTitles::Title(int id, std::string* abbreviated, int width) const {
  const auto entry = this->entries.find(id);
  const auto& title = entry == this->entries.end() ? this->missing.title : entry->second.title;

  if (title.empty()) {
    return title;
  }

  std::string result = title.substr(0, title.find('#'));

  if (abbreviated) {
    if (width <= 10 || result.size() <= static_cast<unsigned>(width) + 4u) {
      *abbreviated = result;
    } else {
      std::vector<std::string> lines;
      SplitTitle(result.c_str(), lines, width, this->codePage);
      abbreviated->clear();

      for (const auto& line : lines) {
        *abbreviated += line;
      }

      *abbreviated += "...";
    }
  }

  return result;
}
