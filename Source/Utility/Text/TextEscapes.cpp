#include "Utility/Text/TextEscapes.h"

// OFFSET: 0x00AF41D0
// FUNCTION: fn_18244
void DecodeTextEscapes(std::string& text) {
  for (std::size_t position = text.find("\\r"); position != std::string::npos; position = text.find("\\r")) {
    text.replace(position, 2, "\r", 1);
  }

  for (std::size_t position = text.find("\\n"); position != std::string::npos; position = text.find("\\n")) {
    text.replace(position, 2, "\n", 1);
  }

  for (std::size_t position = text.find("\\\""); position != std::string::npos; position = text.find("\\\"")) {
    text.replace(position, 2, "\"", 1);
  }
}
