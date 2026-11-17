#ifndef RAGNAROK_SOURCE_UTILITY_TEXT_TOKENIZER_H
#define RAGNAROK_SOURCE_UTILITY_TEXT_TOKENIZER_H

#include "Client/Application/ErrorDialog.h"

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

// OFFSET: 0x00746A70
// FUNCTION: fn_07779
inline std::string& CTokenizer::operator[](int index) {
  if (index < 0 || static_cast<unsigned int>(index) >= this->tokens.size()) {
    ShowErrorDialog("Error in CTokenizer :: operator []", 0);
    for (unsigned int token = 0; token < this->tokens.size(); ++token) {
      ShowErrorDialog(this->tokens[token].c_str(), 0);
    }
  }

  return this->tokens[index];
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CTokenizer) == 12);
#endif

#endif
