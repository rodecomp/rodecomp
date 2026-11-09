#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMessage.h"
#include "Client/Mode/CMode.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Replay.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/WindowManagerWindows.h"

#include "Resource/MSGSTRINGID.h"
#include <cstring>
#include <format>

namespace {
#pragma pack(push, 1)
  struct ExchangeRecordHeader {
    /* 0000 */ std::int16_t type;
    /* 0002 */ std::uint32_t length;
  };
#pragma pack(pop)

  // INLINED
  void SetExchangeName(CGameMode* mode, std::string name) {
    mode->exchangeName = name;
  }
}

// OFFSET: 0x008CAAE0
// FUNCTION: fn_11996
UIExchangeAcceptWnd::~UIExchangeAcceptWnd() = default;

// OFFSET: 0x008E0380
// FUNCTION: fn_12172
void UIExchangeAcceptWnd::OnCreate(int width, int height) {
  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{189, 96}, {234, 96}};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton();
    for (int state = 0; state < 3; ++state) {
      button->SetBitmap((prefix + names[i][state] + ".bmp").c_str(), state, 0);
    }
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
  this->viewer = new UITextViewer();
  this->viewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->viewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
  this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>("deal"), 38, 0, 0);
}

// OFFSET: 0x008FA8C0
// FUNCTION: fn_12343
int UIExchangeAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_EXCHANGEACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_ACK_EXCHANGE_ITEM, 3, 0, 0, 0);
      return 0;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_EXCHANGEACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_ACK_EXCHANGE_ITEM, 4, 0, 0, 0);
      return 0;
    }
    return 0;
  case UM_SETINFO:
    this->saved = reinterpret_cast<NormalItemSlotWindow*>(a);
    if (!reinterpret_cast<ExchangeAcceptWndInfo*>(this->saved)->IsValid()) {
      reinterpret_cast<ExchangeAcceptWndInfo*>(this->saved)->Reset();
    }
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_SET_EXCHANGE_CHAR_NAME: {
    std::string text = "(";
    text += reinterpret_cast<const char*>(a);
    text += ")";
    text += LookupMessage(MSI_SUGGEST_DEAL);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 138, 0, 0);
    const std::string code = std::format("C-Code : {}     {}{}", ShortcutFilename(static_cast<std::uint32_t>(b)), g_serviceType == 12 ? "Nv." : "Lv.", c);
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(code.c_str()), 138, 0, 0);
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ExchangeRecordHeader header = {};
    std::memcpy(&header, bytes, sizeof(header));
    unsigned offset = sizeof(header);
    if (header.type == 21650) {
      while (offset < static_cast<unsigned>(b)) {
        std::memcpy(&header, bytes + offset, sizeof(header));
        offset += sizeof(header);
        if (header.type == 21651) {
          break;
        }
        if (header.type == 21652) {
          char text[260] = {};
          std::memcpy(text, bytes + offset, header.length);
          offset += header.length;
          this->SendMsg(nullptr, UM_SET_EXCHANGE_CHAR_NAME, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
          const std::string name = text;
          CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
          SetExchangeName(mode, name);
        } else {
          offset += header.length;
        }
      }
    }
    return 0;
  }
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00909EC0
// FUNCTION: fn_12405
void UIExchangeAcceptWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}
