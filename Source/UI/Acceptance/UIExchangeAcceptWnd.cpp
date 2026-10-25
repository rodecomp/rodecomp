#include "Client/Mode/ModeEvents.h"
#include "Client/ReplayPacketRecorder.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstring>

// OFFSET: 0x008C7AA0
UIExchangeAcceptWnd::UIExchangeAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIChooseSellBuyWnd(windows, frames, acceptance), saved(nullptr) {
}

// OFFSET: 0x008CC9C0
// FUNCTION: fn_12021
UIExchangeAcceptWnd::~UIExchangeAcceptWnd() = default;

// OFFSET: 0x008E0380
// FUNCTION: fn_12172
void UIExchangeAcceptWnd::Slot15(int width, int height) {
  this->AddChoice("btn_ok", 184, 189, 96);
  this->AddChoice("btn_cancel", 185, 234, 96);
  this->m_message0 = 184;
  this->CreateViewer(width, height);
  char text[] = "deal";
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text), 38, 0, 0);
}

// OFFSET: 0x008FA8C0
// FUNCTION: fn_12343
int UIExchangeAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  auto& owner = *this->m_acceptance;

  if (message == 6) {
    const int response = static_cast<int>(a);
    owner.recorder.Record(this->geometry.id, response, b, c, d);

    if (response == 184 || response == 185) {
      this->m_frameContext->windows.CloseWindow(32);
      this->m_context->mode->SendModeMessage(50, response == 184 ? 3 : 4, 0, 0, 0);
    }

    return 0;
  }

  if (message == 34) {
    this->saved = reinterpret_cast<NormalItemSlotWindow*>(a);

    if (this->saved->width != 280 || this->saved->height != 120 || this->saved->x < 0 || this->saved->x > owner.screenWidth - 280 || this->saved->y < 0 ||
        this->saved->y > owner.screenHeight - 120) {
      this->saved->x = 200;
      this->saved->y = 200;
      this->saved->width = 280;
      this->saved->height = 120;
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  if (message == 50) {
    std::string text = "(";
    text += reinterpret_cast<const char*>(a);
    text += ")";
    text += owner.text.buttons.services.MessageText(93);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 138, 0, 0);
    text = "C-Code : ";
    text += ShortcutFilename(static_cast<std::uint32_t>(b));
    text += "     ";
    text += owner.service == 12 ? "Nv." : "Lv.";
    text += std::to_string(static_cast<int>(c));
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 138, 0, 0);

    return 0;
  }

  if (message == 123) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    std::uint16_t marker;
    std::memcpy(&marker, bytes, sizeof(marker));
    unsigned offset = 6;

    if (marker == 21650) {
      while (static_cast<unsigned>(b) > offset) {
        std::uint16_t kind;
        std::uint32_t size;
        std::memcpy(&kind, bytes + offset, sizeof(kind));
        std::memcpy(&size, bytes + offset + 2, sizeof(size));
        offset += 6;

        if (kind == 21651) {
          break;
        }

        if (kind == 21652) {
          char text[260] = {};
          std::memcpy(text, bytes + offset, size);
          offset += size;
          this->SendMsg(nullptr, 50, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
          owner.replayExchangeName = text;
        } else {
          offset += size;
        }
      }
    }

    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
