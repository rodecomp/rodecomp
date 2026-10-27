#include "Utility/Tokenizer.h"
#include "Client/Application/ErrorDialog.h"
#include "Render/DeviceContext/DCSurface.h"

#include <cwctype>

// OFFSET: 0x00746A70
// FUNCTION: fn_07779
std::string& CTokenizer::operator[](int index) {
  if (index < 0 || static_cast<unsigned int>(index) >= this->tokens.size()) {
    ShowErrorDialog(GetRenderSystemHolder()->system, "Error in CTokenizer :: operator []", 0);
    for (unsigned int token = 0; token < this->tokens.size(); ++token) {
      ShowErrorDialog(GetRenderSystemHolder()->system, this->tokens[token].c_str(), 0);
    }
  }

  return this->tokens[index];
}

// OFFSET: 0x00AF5970
// FUNCTION: fn_18248
void CTokenizer::Parse(std::istream& input) {
  char text[1024];
  input.getline(text, sizeof(text));
  this->Parse(std::string(text));
}

// OFFSET: 0x00AF7680
// FUNCTION: fn_18261
void CTokenizer::Parse(const std::string& text) {
  enum class State { Token = 1, Whitespace = 2 };
  char token[1024];
  unsigned int length = 0;
  State state = State::Token;
  const int size = static_cast<int>(text.size());
  this->tokens.clear();

  int index = 0;
  for (; index < size; ++index) {
    const char value = text[index];
    if (state == State::Token) {
      if (value == '/' && text[index + 1] == '/') {
        return;
      }

      if (std::iswspace(static_cast<wchar_t>(value)) && value >= 0) {
        if (index != 0) {
          token[length] = '\0';
          this->tokens.push_back(std::string(token));
          length = 0;
        }
        state = State::Whitespace;
      } else {
        token[length++] = value;
      }
    } else if (!std::iswspace(static_cast<wchar_t>(value)) || value < 0) {
      --index;
      state = State::Token;
    }
  }

  if (state != State::Whitespace && index != 0) {
    token[length] = '\0';
    this->tokens.push_back(std::string(token));
  }
}
