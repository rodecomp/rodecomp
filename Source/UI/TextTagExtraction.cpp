#include "UI/TextTags.h"

// OFFSET: 0x009BA1D0
// FUNCTION: fn_13882
std::vector<STagString> ExtractTextTags(std::string text) {
  std::vector<STagString> tags;
  int index = 0;

  for (const CTagPrototype& prototype : g_textTagRegistry) {
    std::string remaining = text;

    for (std::size_t offset = 0; offset < text.size();) {
      const std::size_t begin = remaining.find(prototype.opening);

      if (begin == std::string::npos) {
        break;
      }

      std::size_t end = remaining.find(prototype.closing);
      std::size_t advance;

      if (end == std::string::npos) {
        advance = prototype.opening.size();
        remaining.erase(0, advance);
      } else {
        end += prototype.closing.size();
        STagString tag(index, offset + begin, remaining.substr(begin, end - begin));
        tags.push_back(tag);
        remaining.erase(0, end);
        advance = end;
      }

      offset += advance;
    }

    ++index;
  }

  return tags;
}
