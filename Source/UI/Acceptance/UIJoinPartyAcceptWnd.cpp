#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/CMode.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Replay.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstring>

// OFFSET: 0x008C8240
// FUNCTION: fn_11950
UIJoinPartyAcceptWnd::UIJoinPartyAcceptWnd()
    : UIExchangeAcceptWnd() {
}

UIJoinPartyAcceptWnd::~UIJoinPartyAcceptWnd() = default;

// OFFSET: 0x008FE780
// FUNCTION: fn_12353
int UIJoinPartyAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);

    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_JOINPARTYACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_GROUP, this->identity, 1, 0, 0);
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_JOINPARTYACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_GROUP, this->identity, 0, 0, 0);
      break;
    }

    return 0;

  case UM_SET_JOIN_PARTY_MSG: {
    this->identity = static_cast<std::uint32_t>(a);
    std::string text = "(";
    text += reinterpret_cast<const char*>(b);
    text += ")";
    text += LookupMessage(94);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);

    return 0;
  }

  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    std::uint16_t marker;
    std::memcpy(&marker, bytes, sizeof(marker));
    unsigned offset = 6;

    if (marker == 21750) {
      while (static_cast<unsigned>(b) > offset) {
        std::int16_t kind;
        std::uint32_t size;
        std::memcpy(&kind, bytes + offset, sizeof(kind));
        std::memcpy(&size, bytes + offset + 2, sizeof(size));
        offset += 6;

        switch (kind) {
        case 21751:
          return 0;
        case 21752: {
          char text[260] = {};
          std::memcpy(text, bytes + offset, size);
          offset += size;
          this->SendMsg(nullptr, UM_SET_JOIN_PARTY_MSG, 0, reinterpret_cast<std::intptr_t>(text), 0, 0);
          break;
        }
        default:
          offset += size;
          break;
        }
      }
    }

    return 0;
  }

  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}
