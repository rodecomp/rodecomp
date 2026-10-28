#include "Client/ModeScreenshot.h"

// OFFSET: 0x00AD63F0
// FUNCTION: fn_17830
void SanitizeScreenshotName(std::string& name) {
  for (std::size_t i = 0; i < name.size(); ++i) {
    const char c = name[i];
    if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
      name[i] = '_';
    }
  }
}
