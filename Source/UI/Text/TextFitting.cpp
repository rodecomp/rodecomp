#include "UI/Window/UIWindow.h"

#include <atlconv.h>
#include <atlexcept.h>
#include <windows.h>

// OFFSET: 0x00A837D0
// FUNCTION: fn_17007
std::string UIWindow::FitTextToWidth(const std::string& source, int width) {
  if (!this->geometry.surface) {
    this->Resize(width, 15);
  }

  std::string text(source);
  int extent = this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);
  this->TextWidth("...", 0, 0, 12, 0, 0);
  ATL::CA2WEX<128> wide(text.c_str(), CP_THREAD_ACP);
  std::wstring remaining(wide);

  while (extent > width) {
    if (remaining.size() <= 1) {
      ATL::CW2AEX<128> converted(remaining.c_str(), CP_THREAD_ACP);
      return std::string(converted) + "...";
    }

    std::size_t length = remaining.size() - 1;

    if ((remaining[remaining.size() - 2] & 0x80) != 0 && (remaining[remaining.size() - 1] & 0x80) != 0) {
      --length;
    }

    remaining.resize(length);
    ATL::CW2AEX<128> converted(remaining.c_str(), CP_THREAD_ACP);
    text = converted;
    text += "...";
    extent = this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);
  }

  return text;
}
