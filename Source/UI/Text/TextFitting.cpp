#include "Render/Text/TextContext.h"
#include "UI/Text/TextFitting.h"

#include <atlconv.h>
#include <atlexcept.h>
#include <windows.h>

// OFFSET: 0x00A837D0
// FUNCTION: fn_17007
std::string FitText(CDC& surface, FontCache& fonts, const TextLocale& locale, std::string text, int width) {
  TextContext context(surface, fonts, locale);
  context.SetFont(0, 12, 0, 0);
  int extent = text.empty() ? 0 : context.Measure(text.c_str(), static_cast<int>(text.size())).cx;
  context.Measure("...", 3);
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
    extent = context.Measure(text.c_str(), static_cast<int>(text.size())).cx;
  }

  return text;
}
