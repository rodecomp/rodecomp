#include "Client/Mode/ModeEvents.h"
#include "Client/ReplayPacketRecorder.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstring>

// OFFSET: 0x008C8240
UIJoinPartyAcceptWnd::UIJoinPartyAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008CCEE0
// FUNCTION: fn_12031
UIJoinPartyAcceptWnd::~UIJoinPartyAcceptWnd() = default;

// OFFSET: 0x008FE780
// FUNCTION: fn_12353
int UIJoinPartyAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    const int response = static_cast<int>(a);
    this->m_acceptance->recorder.Record(this->geometry.id, response, b, c, d);

    if (response == 184 || response == 185) {
      this->m_frameContext->windows.CloseWindow(35);
      this->m_context->mode->SendModeMessage(60, this->identity, response == 184, 0, 0);
    }

    return 0;
  }

  if (message == 56) {
    this->identity = static_cast<std::uint32_t>(a);
    std::string text = "(";
    text += reinterpret_cast<const char*>(b);
    text += ")";
    text += this->m_acceptance->text.buttons.services.MessageText(94);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);

    return 0;
  }

  if (message == 123) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    std::uint16_t marker;
    std::memcpy(&marker, bytes, sizeof(marker));
    unsigned offset = 6;

    if (marker == 21750) {
      while (static_cast<unsigned>(b) > offset) {
        std::uint16_t kind;
        std::uint32_t size;
        std::memcpy(&kind, bytes + offset, sizeof(kind));
        std::memcpy(&size, bytes + offset + 2, sizeof(size));
        offset += 6;

        if (kind == 21751) {
          break;
        }

        if (kind == 21752) {
          char text[260] = {};
          std::memcpy(text, bytes + offset, size);
          offset += size;
          this->SendMsg(nullptr, 56, 0, reinterpret_cast<std::intptr_t>(text), 0, 0);
        } else {
          offset += size;
        }
      }
    }

    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
