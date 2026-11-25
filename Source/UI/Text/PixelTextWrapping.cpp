#include "Client/Input/InputCharset.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Window/UIWindow.h"
#include "Network/TextEncoding.h"

#include <cstring>
#include <mbstring.h>

// OFFSET: 0x008932D0
// FUNCTION: fn_11504
void UIWindow::WrapLinkText(int kind, std::vector<std::string>* lines, const std::string& text) {
  const int width = this->geometry.width;
  if (*reinterpret_cast<int*>(kind) != 3) {
    const int inner = width - 16;
    this->WrapPixelText(text.c_str(), *lines, inner);
    return;
  }

  const int inner = width - 24;
  std::vector<bool> wrapped;
  this->WrapTextWithBreaks(text.c_str(), lines, &wrapped, inner);
}
