#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Input/InputCharset.h"
#include "UI/UIMacroDetectorWnd.h"
#include "Client/Input/Language.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/UICaptchaPreviewWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

namespace {
struct CaptchaAnswerPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char text[16];
};
static_assert(sizeof(CaptchaAnswerPacket) == 18);
}

// OFFSET: 0x007EDC00
// FUNCTION: fn_09814
void UIMacroDetectorWnd::OnLBtnDown(int, int) {
}

// OFFSET: 0x007EDC10
// FUNCTION: fn_09815
void UIMacroDetectorWnd::OnLBtnUp(int, int) {
}

// OFFSET: 0x007EDC20
// FUNCTION: fn_09816
void UIMacroDetectorWnd::OnMouseMove(int, int) {
}

// OFFSET: 0x007EDC30
// FUNCTION: fn_09817
void UIMacroDetectorWnd::OnProcess() {
  if (this->messageTick + this->messageDuration < timeGetTime()) {
    this->message = "";
  }
  this->Invalidate();
}

// OFFSET: 0x007EDCB0
// FUNCTION: fn_09819
void UIMacroDetectorWnd::ClearImageData() {
  if (this->m_imageData) {
    delete[] this->m_imageData;
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
  if (this->lastCountdownTick > 0) {
    std::string text = FormatOwnedString(LookupMessage(MSI_MACRO_REGISTER_DATA), count);
    this->message = text;
  }

  this->remainingAttempts = count;
  this->messageDuration = now;
  this->lastCountdownTick = now;
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
    delete[] this->m_imageData;
    this->m_imageData = nullptr;
  }
  this->m_imageSize = size;
  this->m_imageReceived = 0;
  this->m_imageData = new unsigned char[size];
  std::memset(this->m_imageData, 0, size);
}

// OFFSET: 0x007EDCF0
// FUNCTION: fn_09820
int UICaptchaPreviewWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_COMMAND && a == ID_CLOSE) {
    g_windowMgr.CloseWindow(WID_CAPTCHAPREVIEWWND);
  }
  return UIWindow::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x007EDD40
// FUNCTION: fn_09821
int UIMacroDetectorWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_DEFPUSH) {
    this->SendMsg(this, UM_COMMAND, ID_SEND, 0, 0, 0);
  } else if (message == UM_COMMAND) {
    if (a == 201) {
      g_windowMgr.CloseWindow(WID_CAPTCHAREGISTERWND);
    } else if (a == 345 && this->m_editCtrl->Length() != 0 && this->m_lastAnswer != this->m_editCtrl->Text()) {
      CaptchaAnswerPacket packet;
      packet.id = 0xA5C;
      memcpy_s(packet.text, sizeof(packet.text), this->m_editCtrl->Text(), sizeof(packet.text));
      GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      this->m_lastAnswer = this->m_editCtrl->Text();
      g_serverLanguage->ResetInput();
    }
  }
  return UIWindow::SendMsg(sender, message, a, b, c, d);
}
