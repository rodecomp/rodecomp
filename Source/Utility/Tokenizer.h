#ifndef RAGNAROK_SOURCE_UTILITY_TOKENIZER_H
#define RAGNAROK_SOURCE_UTILITY_TOKENIZER_H

#include <istream>
#include <string>
#include <vector>

class CTokenizer {
public:
  std::string& operator[](int index);
  void Parse(std::istream& input);
  void Parse(const std::string& text);

  /* 0000 */ std::vector<std::string> tokens;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CTokenizer) == 12);
#endif

#endif
