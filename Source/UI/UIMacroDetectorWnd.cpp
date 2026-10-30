#include "UI/UIMacroDetectorWnd.h"
#include "Contents/MessageTable.h"
#include "Utility/Text/FormatString.h"

#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x007EDC00
// FUNCTION: fn_09814
void UIMacroDetectorWnd::Slot25(int, int) {
}

// OFFSET: 0x007EDC10
// FUNCTION: fn_09815
void UIMacroDetectorWnd::Slot31(int, int) {
}

// OFFSET: 0x007EDC20
// FUNCTION: fn_09816
void UIMacroDetectorWnd::Slot27(int, int) {
}

// OFFSET: 0x007EDC30
// FUNCTION: fn_09817
void UIMacroDetectorWnd::Slot19() {
  if (this->messageTick + this->messageDuration < timeGetTime()) {
    this->message = "";
  }
  this->Invalidate();
}

// OFFSET: 0x007EDCB0
// FUNCTION: fn_09819
void UIMacroDetectorWnd::ClearImageData() {
  if (this->m_imageData) {
    std::free(this->m_imageData);
    this->m_imageData = nullptr;
  }
  this->m_imageSize = 0;
  this->m_imageReceived = 0;
  std::memset(this->m_captchaKey, 0, sizeof(this->m_captchaKey));
}

// OFFSET: 0x007EDED0
// FUNCTION: fn_09822
void UIMacroDetectorWnd::SetCountdown(unsigned char count, std::uint32_t duration) {
  DWORD now = timeGetTime();
  if (this->unknown_0110 > 0) {
    std::string text = FormatOwnedString(LookupMessage(2875), count);
    this->message = text;
  }

  this->unknown_00D0 = count;
  this->messageDuration = now;
  this->unknown_0110 = now;
  if (this->totalDuration == 0) {
    this->totalDuration = duration;
  }
  this->startTick = duration;
  this->Invalidate();
}

// OFFSET: 0x007EE010
// FUNCTION: fn_09824
void UIMacroDetectorWnd::InitImageData(unsigned short size) {
  if (this->m_imageData) {
    std::free(this->m_imageData);
    this->m_imageData = nullptr;
  }
  this->m_imageSize = size;
  this->m_imageReceived = 0;
  this->m_imageData = static_cast<unsigned char*>(std::malloc(size));
  std::memset(this->m_imageData, 0, size);
}
