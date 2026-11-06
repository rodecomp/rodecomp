#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "Client/Mode/CMode.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Edit/UIItemTagCtrl.h"
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
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/GroupedInteger.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Window/InputFocus.h"
#include "Contents/SkinMgr.h"
#include "Contents/MapSkyMgr.h"
#include "Resource/SkinPalette.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>

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

// OFFSET: 0x007FEF20
// FUNCTION: fn_09993
UIRodexEditCtrl3::UIRodexEditCtrl3() : UIEditCtrl3() {
}

// OFFSET: 0x007FEF80
// FUNCTION: fn_09994
UIRodexScrollBar::UIRodexScrollBar() : UIScrollBar() {
  this->arrowLength = 5;
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
void UIRodexComposeWnd::InsertItemTag(const ITEM_INFO& item) {
  if (!this->unknown_0100) {
    this->unknown_0100 = new UIItemTagOnMail();
    if (!this->unknown_0100) {
      return;
    }
  }

  std::string name = item.DisplayName(0);
  auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->unknown_0100);
  if (!tagCtrl->CanInsertTag(name, this->m_contents->geometry.width)) {
    return;
  }

  tagCtrl->taggedText = this->m_contents->Text();
  std::string appendResult = tagCtrl->AppendTag(item.DisplayName(0));
  this->m_contents->SetText(appendResult.c_str());
  this->m_contents->MoveCursorToEnd();
  tagCtrl->AddItemTag(item, 0xd630a9);
}

// OFFSET: 0x007FF940
// FUNCTION: fn_10004
void UIRodexComposeWnd::InsertSkillTag(int tagId) {
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

  std::string tagText = tagCtrl->AddSkillTag(tagId, 0xd630a9);
  tagCtrl->taggedText = std::string(this->m_contents->Text());

  std::string fullText = std::string(this->m_contents->Text()) + tagText;
  this->m_contents->SetText(fullText.c_str());
  this->m_contents->MoveCursorToEnd();
}

// OFFSET: 0x00800480
// FUNCTION: fn_10006
void UIRodexEditCtrl3::Slot15(int width, int height) {
  auto* scrollBar = new UIRodexScrollBar();
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
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(g_windowMgr, 15, item, item.quantity, -1);
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
    CMode* tooltip = g_modeMgr.gameMode;
    if (item.identified == 1 && item.unknown_0098 != 0) {
      std::uint32_t color = g_session.GetItemNameColor(item);
      tooltip->ShowTooltip(
        FormatOwnedString(LookupMessage(2600), item.DisplayName(0).c_str(), item.unknown_0098).c_str(), tooltipX, tooltipY, color, 0, 1
      );
    } else {
      std::uint32_t color = g_session.GetItemNameColor(item);
      tooltip->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, color, 0, 1);
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
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x007FEDD0
// FUNCTION: fn_09992
UIRodexComposeWnd::UIRodexComposeWnd()
    : UIFrameWnd(0), unknown_00B4(nullptr), unknown_00B8(nullptr), unknown_00BC(nullptr), unknown_00C0(nullptr), unknown_00C4(nullptr),
      unknown_00C8(nullptr), unknown_00CC(0), unknown_00D0(nullptr), m_contents(nullptr), unknown_00D8(nullptr), m_zeny(nullptr), unknown_00E0(0),
      unknown_00E4(0) {
  this->unknown_0100 = nullptr;
  this->unknown_00F8 = 0;
  this->m_itemIndex = 0;
  this->m_items.clear();
}

// INLINED
UIRodexComposeWnd::~UIRodexComposeWnd() {
  if (this->unknown_0100) {
    static_cast<UIItemTagCtrl*>(this->unknown_0100)->ClearTags();
    delete this->unknown_0100;
  }
}

// OFFSET: 0x007FF260
// FUNCTION: fn_09998
int UIRodexComposeWnd::MeasureText(const std::string& text, const unsigned& bytes) {
  return this->TextWidth(text.c_str(), bytes, 0, 12, 0, 0);
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

  g_windowMgr.ShowWindowById(15, 1);
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

// OFFSET: 0x00800530
// FUNCTION: fn_10007
void UIRodexComposeWnd::Paint() {
  this->ClearSurface(0x00FF00FF);

  const auto* bg = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_rodex_read.bmp")))->image;
  this->DrawBitmap(0, 0, bg, false);

  if (this->unknown_00B8->geometry.visible == 1) {
    const auto* balloon = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_id_balloon.bmp")))->image;
    this->DrawBitmap(161, 21, balloon, true);
  }

  std::string countStr = std::format("/ {}", 1000);
  this->DrawTextAt(261, 276, countStr.c_str(), 0, 0, 10, 0xD66E44, 0, 0);

  int length = this->m_contents->Length() + this->m_contents->unknown_00F0;
  std::string lengthStr = FormatOwnedString("%d", length);
  this->DrawRightAlignedText(258, 276, lengthStr.c_str(), 0, 0, 10, 0xD66E44, 0);

  const auto* weightBg = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_weight.bmp")))->image;
  this->DrawBitmap(199, 305, weightBg, true);

  const char* weightMsg = LookupMessage(3035);
  this->DrawTextAt(245, 316, weightMsg, 0, 0, 11, 0x737373, 0, 0);

  std::string weightStr = FormatOwnedString("%d", static_cast<int>(g_session.unknown_6140));
  this->DrawRightAlignedText(244, 316, weightStr.c_str(), 0, 0, 11, 0x737373, 0);

  this->FillRegion(35, 347, 95, 16, 0xFFFFFF);
  DrawRectangleBorder(*this, 35, 347, 95, 16, 0xA6A6A6);

  const auto* zenyIcon = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_zeny.bmp")))->image;
  this->DrawBitmap(19, 342, zenyIcon, true);

  this->DrawTextAt(135, 350, "Zeny", 0, 0, 11, 0x737373, 0, 0);

  char zenyBuf[260] = {};
  FormatGroupedInteger(static_cast<std::int64_t>(g_session.m_zeny), zenyBuf, sizeof(zenyBuf));
  std::string zenyStr = FormatOwnedString("%s Zeny", zenyBuf);
  this->DrawTextAt(144, 350, zenyStr.c_str(), 0, 0, 11, 0x737373, 1, 0);

  const auto* taxBg = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_tax.bmp")))->image;
  this->DrawBitmap(9, 377, taxBg, true);

  char taxBuf[260] = {};
  std::int64_t tax = static_cast<std::int64_t>(g_session.CountItems5C68()) * 2500;
  char inputBuf[260] = {};
  strcpy_s(inputBuf, sizeof(inputBuf), this->m_zeny->Text());
  g_session.IsNumberWithCommas(inputBuf);
  std::int64_t inputZeny = _atoi64(inputBuf);
  if (this->m_zeny->Text() != nullptr) {
    tax += (inputZeny * 2) / 100;
    this->unknown_00F8 = tax;
  }
  FormatGroupedInteger(tax, taxBuf, sizeof(taxBuf));
  std::uint32_t taxColor = (inputZeny + this->unknown_00F8 > g_session.m_zeny) ? 0x4E71CF : 0;
  this->DrawRightAlignedText(104, 382, taxBuf, 0, 0, 11, taxColor, 0);

  if (this->m_zeny->Length() != 0) {
    std::uint32_t textColor = (inputZeny > g_session.m_zeny || inputZeny + this->unknown_00F8 > g_session.m_zeny) ? 0xFF : 0;
    this->m_zeny->TextColor(textColor, 0xFFFFFFFF);
  }

  for (int x = 26; x < 196; x += 34) {
    const auto* shadowOff = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_off.bmp")))->image;
    this->DrawBitmap(x, 319, shadowOff, true);
  }

  if (!this->m_items.empty()) {
    int itemX = 30;
    for (const auto& item : this->m_items) {
      const auto* shadowOn = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp")))->image;
      this->DrawBitmap(itemX - 4, 319, shadowOn, true);
      if (this->unknown_00F8 <= g_session.m_zeny) {
        DrawItemWithQuantity(*this, ResourceMgr::Instance(), itemX, 309, item, 0, 0);
      } else {
        DrawItemWithQuantityAuxiliary(*this, 2, itemX, 309, item, item.identified, 0);
      }
      if (item.unknown_0088 > 0) {
        std::string gradeStr = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", static_cast<int>(item.unknown_0088));
        const auto* gradeBmp = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(gradeStr.c_str())))->image;
        this->DrawBitmap(itemX, 321, gradeBmp, true);
      }
      itemX += 34;
    }
  }

  if (this->unknown_0100) {
    auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->unknown_0100);
    tagCtrl->SetLines(this->m_contents->unknown_0168);
    tagCtrl->SetTaggedText(this->m_contents->Text(), this->m_contents->geometry.width);
    tagCtrl->UpdateItemTags();
    auto* editCtrl3 = static_cast<UIEditCtrl3*>(this->unknown_0100);
    int diff = editCtrl3->unknown_0150 - editCtrl3->unknown_0138;
    this->m_contents->unknown_00F0 = std::max(0, diff);
    if (tagCtrl->unknown_011C == 0) {
      delete this->unknown_0100;
      this->unknown_0100 = nullptr;
    }
  }
}

// OFFSET: 0x007FFB90
// FUNCTION: fn_10005
void UIRodexComposeWnd::Slot15(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->unknown_00B4 = new UIEditCtrl_Custom();
  this->unknown_00B4->SetSize(0x96, 0x12);
  this->unknown_00B4->Move(8, 0x1A);
  this->unknown_00B4->maximum = 50;
  this->unknown_00B4->Background(0xFF, 0xFF, 0xFF);
  this->unknown_00B4->flat = 0;
  this->unknown_00B4->Border(0xA6, 0xA6, 0xA6);
  this->unknown_00B4->TextColor(0x313131, 0xFFFFFFFF);
  this->unknown_00B4->unknown_0120 = 1;
  this->unknown_00B4->unknown_0124 = 1;
  this->AddChild(this->unknown_00B4);

  this->unknown_00B8 = new UIEditCtrl_Custom();
  this->unknown_00B8->SetSize(0x96, 0x12);
  this->unknown_00B8->Move(8, 0x1A);
  this->unknown_00B8->maximum = 50;
  this->unknown_00B8->Background(0x110, 0x110, 0x110);
  this->unknown_00B8->flat = 0;
  this->unknown_00B8->Border(0xA6, 0xA6, 0xA6);
  this->unknown_00B8->TextColor(0x95330F, 0xFFFFFFFF);
  this->unknown_00B8->unknown_011C = 1;
  this->unknown_00B8->unknown_0120 = 1;
  this->unknown_00B8->unknown_0124 = 1;
  this->unknown_00B8->disabled = 1;
  this->unknown_00B8->Invalidate();
  this->unknown_00B8->Show(0);
  this->AddChild(this->unknown_00B8);

  this->unknown_00BC = new UIBmpTextButton();
  this->unknown_00BC->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_out.bmp", 0, 0);
  this->unknown_00BC->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_over.bmp", 1, 0);
  this->unknown_00BC->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_press.bmp", 2, 0);
  this->unknown_00BC->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_press.bmp", 3, 0);
  this->unknown_00BC->SetSize(this->unknown_00BC->BitmapWidth(), this->unknown_00BC->BitmapHeight());
  {
    const int textX = static_cast<int>(
      static_cast<float>(this->unknown_00BC->geometry.width - this->TextWidth(LookupMessage(0xDD9), 0, 0, 12, 0, 0)) *
      g_depthAverageHalf
    );
    UIBmpTextButton* button = this->unknown_00BC;
    const int textY = static_cast<int>(
      static_cast<float>(button->geometry.height - this->TextHeight(LookupMessage(0xDD9), 0, 0, 12, 0, 0)) *
      g_depthAverageHalf
    );
    button->AddTextStyle(0, 12, 0x313131, 0, 0, textX, textY);
    this->unknown_00BC->AddTextStyle(1, 12, 0x313131, 0, 0, textX, textY);
    this->unknown_00BC->AddTextStyle(2, 12, 0x313131, 0, 1, textX, textY);
    this->unknown_00BC->AddTextStyle(3, 12, 0xC2C2C2, 0, 0, textX, textY);
  }
  this->unknown_00BC->SetText(LookupMessage(0xDD9));
  this->unknown_00BC->Move(0xA2, 0x1A);
  this->unknown_00BC->SetId(0xD5);
  this->AddChild(this->unknown_00BC);

  this->unknown_00C0 = new UIStaticText();
  this->unknown_00C0->SetSize(0x100, 0x10);
  this->unknown_00C0->Move(0xAC, 0x19);
  this->unknown_00C0->SetColor(0xC6, 0x28, 0x28);
  this->unknown_00C0->SetFont(0, 11, 0, 0);
  this->unknown_00C0->Show(0);
  this->AddChild(this->unknown_00C0);

  this->unknown_00C4 = new UIStaticText();
  this->unknown_00C4->SetSize(0x100, 0x10);
  this->unknown_00C4->Move(0xAC, 0x25);
  this->unknown_00C4->SetColor(0xC6, 0x28, 0x28);
  this->unknown_00C4->SetFont(0, 11, 0, 0);
  this->unknown_00C4->Show(0);
  this->AddChild(this->unknown_00C4);

  this->unknown_00C8 = new UIStaticText();
  this->unknown_00C8->SetSize(0xFD, 0x10);
  this->unknown_00C8->Move(0xAC, 0x31);
  this->unknown_00C8->SetColor(0xC6, 0x28, 0x28);
  this->unknown_00C8->SetFont(0, 11, 0, 0);
  this->unknown_00C8->Show(0);
  this->AddChild(this->unknown_00C8);

  this->unknown_00D0 = new UIEditCtrl_Custom();
  this->unknown_00D0->SetSize(0x9B, 0xE);
  this->unknown_00D0->Move(6, 0x32);
  this->unknown_00D0->maximum = 50;
  this->unknown_00D0->SetText("TITLE");
  this->AddChild(this->unknown_00D0);

  this->m_contents = new UIRodexEditCtrl3();
  this->m_contents->SetSize(0x10B, 0xC0);
  this->m_contents->invalidatesParent = 1;
  this->m_contents->Move(0xF, 0x51);
  this->m_contents->maximum = 1000;
  const bool selected = g_skinInfo.Selected();
  const SkinPaletteCell cell = {1, 2};
  const std::uint32_t color = SkinPaletteColor(ResourceMgr::Instance(), cell, selected);
  this->m_contents->Background(color & 0xFF, (color & 0xFFFF) >> 8, (color >> 16) & 0xFF);
  this->m_contents->TextColor(0x313131, 0xFFFFFFFF);
  this->AddChild(this->m_contents);
  g_windowMgr.SetFocus(this->m_contents);
  this->m_contents->SetId(0x1EB);

  this->m_zeny = new UIEditCtrl_Currency();
  this->m_zeny->SetSize(0x54, 0xE);
  this->m_zeny->invalidatesParent = 0;
  this->m_zeny->Move(0x2D, 0x15C);
  this->m_zeny->maximum = 13;
  this->m_zeny->SetText("0");
  this->AddChild(this->m_zeny);

  this->unknown_00D8 = new UIBasicButton();
  this->unknown_00D8->Configure(0x46, LookupMessage(0xDD8));
  this->unknown_00D8->Move(0xE2, 0x178);
  this->unknown_00D8->SetId(0x159);
  this->AddChild(this->unknown_00D8);
}
