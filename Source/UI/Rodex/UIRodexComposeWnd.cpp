#include "UI/Rodex/UIRodexComposeWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/UIStaticText.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Items/ItemDescription.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeTooltip.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Text/NavigationLink.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace {
#pragma pack(push, 1)
struct RodexCheckRecipientPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char name[24];
  /* 001A */ std::uint8_t known;
};
#pragma pack(pop)

struct RodexSendMailRequest {
  /* 0000 */ int recipientId;
  /* 0004 */ std::string recipient;
  /* 001C */ std::string title;
  /* 0034 */ std::string body;
  /* 004C */ int zeny;
};
}

// OFFSET: 0x007FF520
// FUNCTION: fn_10002
ITEM_INFO UIRodexComposeWnd::GetItemAtPoint(int x, int y) {
  if (x >= 30 && x <= 190 && y >= 309 && y <= 333) {
    this->m_itemIndex = 0;
    int left = 30;
    for (auto item = this->m_items.begin(); item != this->m_items.end(); ++item) {
      if (x > left && x < left + 24) {
        return *item;
      }
      left += 34;
      ++this->m_itemIndex;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x007FF710
// FUNCTION: fn_10003
void UIRodexComposeWnd::sub_7FF710(const ITEM_INFO& item) {
  if (!this->unknown_0100) {
    this->unknown_0100 = new UIItemTagOnMail();
    if (!this->unknown_0100) {
      return;
    }
  }

  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};

  std::string name = naming.DisplayName(item, 0);
  auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->unknown_0100);
  if (!tagCtrl->CanInsertTag(name, this->m_contents->geometry.width)) {
    return;
  }

  tagCtrl->taggedText = this->m_contents->Text();
  std::string appendResult = tagCtrl->AppendTag(naming.DisplayName(item, 0));
  this->m_contents->SetText(appendResult.c_str());
  this->m_contents->sub_855150();
  tagCtrl->sub_89CFE0(item, 0xd630a9);
}

// OFFSET: 0x007FF940
// FUNCTION: fn_10004
void UIRodexComposeWnd::sub_7FF940(int tagId) {
  if (!this->unknown_0100) {
    this->unknown_0100 = new UIItemTagOnMail();
    if (!this->unknown_0100) {
      return;
    }
  }

  auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->unknown_0100);
  if (tagCtrl->tags.size() >= 3) {
    return;
  }

  if (this->m_contents->Length() + this->m_contents->unknown_00F0 > this->m_contents->maximum) {
    return;
  }

  std::string tagText = tagCtrl->sub_874A40(tagId, 0xd630a9);
  tagCtrl->taggedText = this->m_contents->Text();

  std::string fullText = std::string(this->m_contents->Text()) + tagText;
  this->m_contents->SetText(fullText.c_str());
  this->m_contents->sub_855150();
}

// OFFSET: 0x00800480
// FUNCTION: fn_10006
void UIRodexEditCtrl3::Slot15(int width, int height) {
  auto* scrollBar = new UIRodexScrollBar(*this->m_context);
  this->unknown_0130 = scrollBar;
  scrollBar->Configure(width - 5, 0, 1, height);
  this->AddChild(this->unknown_0130);
}

// OFFSET: 0x00800DD0
// FUNCTION: fn_10008
void UIRodexScrollBar::Paint() {
  this->ClearSurface(0xFF00FF);

  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    this->FillRegion(0, 0, 5, this->geometry.height, 0xFFFFFF);
    const auto* upper = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\scrollbar_arrow_upper.bmp"))
    );
    this->DrawBitmap(0, 0, &upper->image, false);

    int offset = this->ThumbOffset() + this->arrowLength;
    if (offset < 5) {
      offset = 5;
    }

    int length = this->ThumbLength();
    if (this->geometry.height - 10 < length) {
      length = 1;
    }

    this->FillRegion(0, offset, 5, length, 0xE9E9E9);
    const auto* lower = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\scrollbar_arrow_down.bmp"))
    );
    this->DrawBitmap(0, this->geometry.height - this->arrowLength, &lower->image, false);
  }

  this->minimumThumb = 0;
}

// OFFSET: 0x00800ED0
// FUNCTION: fn_10009
void UIRodexComposeWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (item.unknown_0000[1] > 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 15, item, item.quantity, -1);
  }
}

// OFFSET: 0x00800FA0
// FUNCTION: fn_10010
void UIRodexComposeWnd::Slot28(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    int tooltipX = this->m_itemIndex * 34 + 30;
    int tooltipY = 290;
    this->ToScreen(tooltipX, tooltipY);
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
    if (item.identified == 1 && item.unknown_0098 != 0) {
      std::uint32_t color = g_session.GetItemNameColor(item);
      tooltip->Show(
        FormatOwnedString(LookupMessage(2600), naming.DisplayName(item, 0).c_str(), item.unknown_0098).c_str(), tooltipX, tooltipY, color, 0, 1
      );
    } else {
      std::uint32_t color = g_session.GetItemNameColor(item);
      tooltip->Show(naming.DisplayName(item, 0).c_str(), tooltipX, tooltipY, color, 0, 1);
    }
  }
}

// OFFSET: 0x008011B0
// FUNCTION: fn_10011
void UIRodexComposeWnd::Slot19() {
  if (g_windowMgr.GetHoverWindow() == this->m_zeny) {
    this->m_zeny->Slot19();
  }
}

// OFFSET: 0x008011D0
// FUNCTION: fn_10012
void UIRodexComposeWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x007FEDD0
// FUNCTION: fn_09992
UIRodexComposeWnd::UIRodexComposeWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), unknown_00B4(nullptr), unknown_00B8(nullptr), unknown_00BC(nullptr), unknown_00C0(nullptr), unknown_00C4(nullptr),
      unknown_00C8(nullptr), unknown_00CC(0), unknown_00D0(nullptr), m_contents(nullptr), unknown_00D8(nullptr), m_zeny(nullptr), unknown_00E0(0),
      unknown_00E4(0), m_itemIndex(0), unknown_00F8(0), unknown_0100(nullptr) {
  this->m_items.clear();
}

// INLINED
UIRodexComposeWnd::~UIRodexComposeWnd() {
  if (this->unknown_0100) {
    reinterpret_cast<ItemTagRetirement*>(this->unknown_0100)->Clear();
    delete this->unknown_0100;
  }
}

// OFFSET: 0x007FF260
// FUNCTION: fn_09998
int UIRodexComposeWnd::MeasureText(const std::string& text, const unsigned& bytes) {
  return this->TextWidth(this->m_context->fonts, this->m_context->locale, text.c_str(), bytes, 0, 12, 0, 0);
}

// OFFSET: 0x007FF290
// FUNCTION: fn_09999
void UIRodexComposeWnd::SetRecipient(int id, unsigned short job, unsigned short level, const char* name) {
  this->unknown_00C0->SetTextWithinWidth(FormatOwnedString("Lv%d", level).c_str(), 0);
  this->unknown_00C0->Show(1);
  this->unknown_00C4->SetTextWithinWidth(g_session.GetJobName(job, 99), 0);
  this->unknown_00C4->Show(1);
  this->unknown_00CC = id;
  this->unknown_00C8->SetTextWithinWidth(ShortcutFilename(id).c_str(), 0);
  this->unknown_00C8->Show(1);
  this->unknown_00B4->Show(0);
  this->unknown_00BC->Enable(0);
  this->unknown_00B8->Show(1);
  this->unknown_00B8->SetText(name);
}

// OFFSET: 0x007FF400
// FUNCTION: fn_10000
void UIRodexComposeWnd::Slot10(int x, int y, std::intptr_t value) {
  if (!g_windowMgr.unknown_0458) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 1) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.window_01EC->SendMsg(nullptr, 13, 13, 0, 0, 0);
}

// OFFSET: 0x007FF4A0
// FUNCTION: fn_10001
std::int64_t UIRodexComposeWnd::GetZeny() {
  char text[260];
  std::memset(text, 0, sizeof(text));
  strcpy_s(text, sizeof(text), this->m_zeny->Text());
  g_session.IsNumberWithCommas(text);
  return _atoi64(text);
}

// OFFSET: 0x008012A0
// FUNCTION: fn_10013
int UIRodexComposeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    if (argument1 == 201) {
      g_windowMgr.CloseWindow(264);
    } else if (argument1 == 213) {
      if (this->unknown_00B4->Length() != 0 && this->unknown_00B4->Length() < 24) {
        if (g_navigationLinkParser->HasLink(this->unknown_00B4->Text())) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xAFC)), 0x64FFFF, 0, 0, 0);
          return 0;
        }

        RodexCheckRecipientPacket packet;
        packet.packetType = 0xB97;
        std::memset(packet.name, 0, sizeof(packet.name));
        strcpy_s(packet.name, sizeof(packet.name), this->unknown_00B4->Text());
        if (g_session.HasUnknown809C(packet.name)) {
          packet.known = 1;
        } else {
          packet.known = g_session.IsKnownName(packet.name);
        }
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        return 0;
      }

      if (this->unknown_00B4->Length() >= 24) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA27)), 0x64FFFF, 0, 0, 0);
      }
      if (g_navigationLinkParser->HasLink(this->unknown_00B4->Text())) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xAFC)), 0x64FFFF, 0, 0, 0);
      } else {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA23)), 0x64FFFF, 0, 0, 0);
      }
    } else if (argument1 == 345) {
      if (this->unknown_00B4->Length() == 0 && this->unknown_00B8->Length() == 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA23)), 0x64FFFF, 0, 0, 0);
        return 0;
      }
      if (this->unknown_00D0->Length() < 4 || this->unknown_00D0->Length() >= 51) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA24)), 0x64FFFF, 0, 0, 0);
        return 0;
      }
      if (this->m_contents->Length() >= 1001) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA44)), 0x64FFFF, 0, 0, 0);
        return 0;
      }

      const std::int64_t zeny = this->GetZeny();
      if (zeny > 0) {
        if (zeny > g_session.m_zeny) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA35)), 0x64FFFF, 0, 0, 0);
          return 0;
        }
        if (zeny + this->unknown_00F8 > g_session.m_zeny) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA53)), 0x64FFFF, 0, 0, 0);
          return 0;
        }
      }
      if (this->unknown_00F8 > 0 && this->unknown_00F8 > g_session.m_zeny) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA53)), 0x64FFFF, 0, 0, 0);
        return 0;
      }

      if (this->unknown_00C8->GetText().length() <= 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA33)), 0x64FFFF, 0, 0, 0);
        return 0;
      }

      std::string body(this->m_contents->Text());
      if (g_navigationLinkParser->HasLink(this->m_contents->Text())) {
        this->m_contents->SetText("");
        body.clear();
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBE2)), 0xFF, 0, 0, 0);
        return 0;
      }

      if (this->m_contents->links.size() != 0) {
        body.clear();
        body = this->m_contents->TakePendingText();
      }

      auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->unknown_0100);
      if (tagCtrl && tagCtrl->unknown_0140.size() != 0) {
        body = tagCtrl->unknown_0140.c_str();
      }

      RodexSendMailRequest mail;
      mail.recipientId = this->unknown_00CC;
      mail.recipient = this->unknown_00B8->Text();
      mail.title = this->unknown_00D0->Text();
      mail.body = body;
      mail.zeny = static_cast<int>(zeny);
      g_modeMgr.GetGameMode()->SendMsg(0xC0, reinterpret_cast<int>(&mail), 0, 0, 0);
    }
    return 0;
  case 23:
    this->m_items.clear();
    for (int i = 0; i < 5; ++i) {
      ITEM_INFO item = g_session.GetItem5C68(i);
      if (item.quantity > 0) {
        this->m_items.push_back(item);
      }
    }
    this->Invalidate();
    return 0;
  case 38: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    g_modeMgr.GetGameMode()->SendMsg(0xC3, mode->drag.id, mode->drag.quantity, 0, 0);
    g_modeMgr.GetGameMode()->SendMsg(0x12, 0, 0, 0, 0);
    return 0;
  }
  case 98:
    if (argument3 > 0) {
      UIWindow* window = g_windowMgr.EnsureWindow(12);
      if (window) {
        window->SendMsg(nullptr, 24, argument3, 0, 0, 0);
      }
    }
    return 0;
  case 104: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    g_modeMgr.GetGameMode()->SendMsg(0x126, mode->drag.id, mode->drag.quantity, 0, 0);
    g_modeMgr.GetGameMode()->SendMsg(0x12, 0, 0, 0, 0);
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
