#include "UI/UICaptchaPreviewWnd.h"
#include "UI/UIMacroDetectorWnd.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/MessageTable.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x007ED950
// FUNCTION: fn_09812
void UICaptchaPreviewWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x007ED960
// FUNCTION: fn_09813
void UIMacroDetectorWnd::Paint() {
  this->sub_8D2CE0(false, "", 0);
  this->DrawButtonBar(0, 0, 1);
  this->FillRegion(0, 16, this->geometry.width, this->geometry.height - 36, 0xFFFFFFFF);
  DrawRectangleBorder(*this, 0, 16, this->geometry.width, this->geometry.height - 36, 0xC6C6C6);

  this->sub_A84360(25, 28, LookupMessage(2883), 0, 0, 12, 0, 1, 0);
  this->sub_A84360(25, 45, LookupMessage(2884), 0, 0, 12, 0, 1, 0);
  this->sub_A84360(25, 62, LookupMessage(2885), 0, 0, 12, 0, 1, 0);

  this->sub_A84360(25, 82, this->message.c_str(), 0, 0, 12, 0x2323FF, 1, 0);

  this->FillRegion(59, 158, 222, 92, 0xC6C6C6);
  this->FillRegion(60, 97, 220, 30, 0xC6C6C6);

  DWORD now = timeGetTime();
  std::uint32_t remaining = (this->messageDuration + this->startTick) - now;
  if (remaining != 0) {
    float width = (static_cast<float>(remaining) / static_cast<float>(this->totalDuration)) * 220.0f;
    this->FillRegion(60, 97, static_cast<int>(width), 30, 0xFF);

    unsigned int seconds = remaining / 1000;
    {
      std::string str = FormatOwnedString("%2d : %2d", seconds / 60, seconds);
      this->sub_A86440(280, 138, str.c_str(), 0, 0, 13, 0xFF2323, 1);
    }
    {
      std::string str = FormatOwnedString(LookupMessage(2886), this->unknown_00D0);
      this->sub_A84360(60, 138, str.c_str(), 0, 0, 13, 0x2323FF, 1, 0);
    }
  }
}
