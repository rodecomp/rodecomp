#include "Render/Text/FontCache.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
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

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
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
  if (!this->m_tagCtrl) {
    this->m_tagCtrl = new UIItemTagOnMail();
    if (!this->m_tagCtrl) {
      return;
    }
  }

  auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->m_tagCtrl);
  if (!tagCtrl->CanInsertTag(item.DisplayName(0), this->m_contents->geometry.width)) {
    return;
  }

  tagCtrl->taggedText = std::string(this->m_contents->Text());
  this->m_contents->SetText(tagCtrl->AppendTag(item.DisplayName(0)).c_str());
  this->m_contents->MoveCursorToEnd();
  tagCtrl->AddItemTag(item, 0xd630a9);
}

// OFFSET: 0x007FF940
// FUNCTION: fn_10004
void UIRodexComposeWnd::InsertSkillTag(int tagId) {
  if (!this->m_tagCtrl) {
    this->m_tagCtrl = new UIItemTagOnMail();
    if (!this->m_tagCtrl) {
      return;
    }
  }

  auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->m_tagCtrl);
  if (tagCtrl->tags.size() >= 3) {
    return;
  }

  if (this->m_contents->Length() + this->m_contents->tagTextLength > this->m_contents->maximum) {
    return;
  }

  std::string tagText = tagCtrl->AddSkillTag(tagId, 0xd630a9);
  tagCtrl->taggedText = std::string(this->m_contents->Text());

  this->m_contents->SetText((this->m_contents->Text() + tagText).c_str());
  this->m_contents->MoveCursorToEnd();
}

// OFFSET: 0x00800480
// FUNCTION: fn_10006
void UIRodexEditCtrl3::OnCreate(int width, int height) {
  auto* scrollBar = new UIRodexScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(width - 5, 0, 1, height);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00800DD0
// FUNCTION: fn_10008
void UIRodexScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);

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
void UIRodexComposeWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (item.identity[1] > 0) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(g_windowMgr, 15, item, item.quantity, -1);
  }
}

// OFFSET: 0x00800FA0
// FUNCTION: fn_10010
void UIRodexComposeWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    int tooltipX = this->m_itemIndex * 34 + 30;
    int tooltipY = 290;
    this->ToScreen(tooltipX, tooltipY);
    CMode* tooltip = g_modeMgr.gameMode;
    if (item.identified == 1 && item.optionCount != 0) {
      std::uint32_t color = g_session.GetItemNameColor(item);
      tooltip->ShowTooltip(
        FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX, tooltipY, color, 0, 1
      );
    } else {
      std::uint32_t color = g_session.GetItemNameColor(item);
      tooltip->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, color, 0, 1);
    }
  }
}

// OFFSET: 0x008011B0
// FUNCTION: fn_10011
void UIRodexComposeWnd::OnProcess() {
  if (g_windowMgr.GetHoverWindow() == this->m_zeny) {
    this->m_zeny->OnProcess();
  }
}

// OFFSET: 0x008011D0
// FUNCTION: fn_10012
void UIRodexComposeWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x007FEDD0
// FUNCTION: fn_09992
UIRodexComposeWnd::UIRodexComposeWnd()
    : UIFrameWnd(0), m_recipientEdit(nullptr), m_confirmedRecipientEdit(nullptr), m_confirmRecipientButton(nullptr), m_recipientLevelText(nullptr), m_recipientJobText(nullptr),
      m_recipientIdText(nullptr), m_recipientId(0), m_titleEdit(nullptr), m_contents(nullptr), m_sendButton(nullptr), m_zeny(nullptr), m_reservedStateA(0),
      m_reservedStateB(0) {
  this->m_tagCtrl = nullptr;
  this->m_tax = 0;
  this->m_itemIndex = 0;
  this->m_items.clear();
}

// INLINED
UIRodexComposeWnd::~UIRodexComposeWnd() {
  if (this->m_tagCtrl) {
    static_cast<UIItemTagCtrl*>(this->m_tagCtrl)->ClearTags();
    delete this->m_tagCtrl;
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
  this->m_recipientLevelText->SetTextWithinWidth(FormatOwnedString("Lv%d", level).c_str(), 0);
  this->m_recipientLevelText->Show(1);
  this->m_recipientJobText->SetTextWithinWidth(g_session.GetJobName(job, 99), 0);
  this->m_recipientJobText->Show(1);
  this->m_recipientId = id;
  this->m_recipientIdText->SetTextWithinWidth(ShortcutFilename(id).c_str(), 0);
  this->m_recipientIdText->Show(1);
  this->m_recipientEdit->Show(0);
  this->m_confirmRecipientButton->Enable(0);
  this->m_confirmedRecipientEdit->Show(1);
  this->m_confirmedRecipientEdit->SetText(name);
}

// OFFSET: 0x007FF400
// FUNCTION: fn_10000
void UIRodexComposeWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (!g_windowMgr.rodexComposeWnd) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 1) {
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    } else {
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 13, 0, 0, 0);
    }
  } else {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
  }
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
  case UM_COMMAND:
    switch (argument1) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_RODEXCOMPOSEWND);
      break;
    case ID_CHECK:
      if (this->m_recipientEdit->Length() != 0 && this->m_recipientEdit->Length() < 24) {
        if (g_navigationLinkParser->HasLink(this->m_recipientEdit->Text())) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON)), 0x64FFFF, 0, 0, 0);
          return 0;
        }

        RodexCheckRecipientPacket packet;
        packet.packetType = ID_CZ_CHECK_RODEX_RECEIVE;
        std::memset(packet.name, 0, sizeof(packet.name));
        strcpy_s(packet.name, sizeof(packet.name), this->m_recipientEdit->Text());
        if (g_session.HasCharacterName(packet.name)) {
          packet.known = 1;
        } else {
          packet.known = g_session.IsKnownName(packet.name);
        }
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        return 0;
      }

      if (this->m_recipientEdit->Length() >= 24) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_WND_OVERFULL)), 0x64FFFF, 0, 0, 0);
      }
      if (g_navigationLinkParser->HasLink(this->m_recipientEdit->Text())) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON)), 0x64FFFF, 0, 0, 0);
      } else {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_SEND_MAIL_ITEM)), 0x64FFFF, 0, 0, 0);
      }
      break;
    case ID_SEND: {
      if (this->m_recipientEdit->Length() == 0 && this->m_confirmedRecipientEdit->Length() == 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_SEND_MAIL_ITEM)), 0x64FFFF, 0, 0, 0);
        return 0;
      }
      if (this->m_titleEdit->Length() < 4 || this->m_titleEdit->Length() >= 51) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAIELD_SEND_MAIL_TO)), 0x64FFFF, 0, 0, 0);
        return 0;
      }
      if (this->m_contents->Length() >= 1001) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DYNAMICNPC_FAIL_DUPLICATENAME)), 0x64FFFF, 0, 0, 0);
        return 0;
      }

      const std::int64_t zeny = this->GetZeny();
      if (zeny > 0) {
        if (zeny > g_session.m_zeny) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TAIWAN_PERSONALINFO_EXPMSG)), 0x64FFFF, 0, 0, 0);
          return 0;
        }
        if (zeny + this->m_tax > g_session.m_zeny) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ACHIEVEMENT_SYSTEM)), 0x64FFFF, 0, 0, 0);
          return 0;
        }
      }
      if (this->m_tax > 0 && this->m_tax > g_session.m_zeny) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ACHIEVEMENT_SYSTEM)), 0x64FFFF, 0, 0, 0);
        return 0;
      }

      if (this->m_recipientIdText->GetText().length() <= 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SEND_MAIL_COMMISSION)), 0x64FFFF, 0, 0, 0);
        return 0;
      }

      std::string body(this->m_contents->Text());
      if (g_navigationLinkParser->HasLink(this->m_contents->Text())) {
        this->m_contents->SetText("");
        body.clear();
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CLOSE)), 0xFF, 0, 0, 0);
        return 0;
      }

      if (this->m_contents->links.size() != 0) {
        body.clear();
        body = this->m_contents->TakePendingText();
      }

      auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->m_tagCtrl);
      if (tagCtrl && tagCtrl->linkedText.size() != 0) {
        body = tagCtrl->linkedText.c_str();
      }

      RodexSendMailRequest mail;
      mail.recipientId = this->m_recipientId;
      mail.recipient = this->m_confirmedRecipientEdit->Text();
      mail.title = this->m_titleEdit->Text();
      mail.body = body;
      mail.zeny = static_cast<int>(zeny);
      g_modeMgr.GetGameMode()->SendMsg(MM_SEND_MAIL, reinterpret_cast<int>(&mail), 0, 0, 0);
      break;
    }
    }
    return 0;
  case UM_REFRESHITEM:
    this->m_items.clear();
    for (int i = 0; i < 5; ++i) {
      ITEM_INFO item = g_session.GetSelectionItemAt(i);
      if (item.quantity > 0) {
        this->m_items.push_back(item);
      }
    }
    this->Invalidate();
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    g_modeMgr.GetGameMode()->SendMsg(MM_ADD_MAIL_ITEM, mode->drag.id, mode->drag.quantity, 0, 0);
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_TEXT_LBTN_UP:
    if (argument3 > 0) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      if (window) {
        window->SendMsg(nullptr, UM_ITEMEXPLANATION, argument3, 0, 0, 0);
      }
    }
    return 0;
  case UM_DECITEM: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    g_modeMgr.GetGameMode()->SendMsg(MM_REMOVE_RODEX_ITEM, mode->drag.id, mode->drag.quantity, 0, 0);
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
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

  if (this->m_confirmedRecipientEdit->geometry.visible == 1) {
    const auto* balloon = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_id_balloon.bmp")))->image;
    this->DrawBitmap(161, 21, balloon, true);
  }

  std::string countStr = std::format("/ {}", 1000);
  this->DrawTextAt(261, 276, countStr.c_str(), 0, 0, 10, 0xD66E44, 0, 0);

  int length = this->m_contents->Length() + this->m_contents->tagTextLength;
  std::string lengthStr = FormatOwnedString("%d", length);
  this->DrawRightAlignedText(258, 276, lengthStr.c_str(), 0, 0, 10, 0xD66E44, 0);

  const auto* weightBg = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_weight.bmp")))->image;
  this->DrawBitmap(199, 305, weightBg, true);

  const char* weightMsg = LookupMessage(MSI_TWITTER_TAG);
  this->DrawTextAt(245, 316, weightMsg, 0, 0, 11, 0x737373, 0, 0);

  std::string weightStr = FormatOwnedString("%d", static_cast<int>(g_session.achievementResult));
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
  std::int64_t tax = static_cast<std::int64_t>(g_session.CountSelectionItems()) * 2500;
  char inputBuf[260] = {};
  strcpy_s(inputBuf, sizeof(inputBuf), this->m_zeny->Text());
  g_session.IsNumberWithCommas(inputBuf);
  std::int64_t inputZeny = _atoi64(inputBuf);
  if (this->m_zeny->Text() != nullptr) {
    tax += (inputZeny * 2) / 100;
    this->m_tax = tax;
  }
  FormatGroupedInteger(tax, taxBuf, sizeof(taxBuf));
  std::uint32_t taxColor = (inputZeny + this->m_tax > g_session.m_zeny) ? 0x4E71CF : 0;
  this->DrawRightAlignedText(104, 382, taxBuf, 0, 0, 11, taxColor, 0);

  if (this->m_zeny->Length() != 0) {
    std::uint32_t textColor = (inputZeny > g_session.m_zeny || inputZeny + this->m_tax > g_session.m_zeny) ? 0xFF : 0;
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
      if (this->m_tax <= g_session.m_zeny) {
        DrawItemWithQuantity(*this, ResourceMgr::Instance(), itemX, 309, item, 0, 0);
      } else {
        DrawItemWithQuantityAuxiliary(*this, 2, itemX, 309, item, item.identified, 0);
      }
      if (item.grade > 0) {
        std::string gradeStr = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", static_cast<int>(item.grade));
        const auto* gradeBmp = &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(gradeStr.c_str())))->image;
        this->DrawBitmap(itemX, 321, gradeBmp, true);
      }
      itemX += 34;
    }
  }

  if (this->m_tagCtrl) {
    auto* tagCtrl = static_cast<UIItemTagOnMail*>(this->m_tagCtrl);
    tagCtrl->SetLines(this->m_contents->wrappedLines);
    tagCtrl->SetTaggedText(this->m_contents->Text(), this->m_contents->geometry.width);
    tagCtrl->UpdateItemTags();
    auto* editCtrl3 = static_cast<UIEditCtrl3*>(this->m_tagCtrl);
    int diff = editCtrl3->verticalCursorStep - editCtrl3->lineHeight;
    this->m_contents->tagTextLength = std::max(0, diff);
    if (tagCtrl->previousTagCount == 0) {
      delete this->m_tagCtrl;
      this->m_tagCtrl = nullptr;
    }
  }
}

// OFFSET: 0x007FFB90
// FUNCTION: fn_10005
void UIRodexComposeWnd::OnCreate(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->m_recipientEdit = new UIEditCtrl_Custom();
  this->m_recipientEdit->SetSize(0x96, 0x12);
  this->m_recipientEdit->Move(8, 0x1A);
  this->m_recipientEdit->maximum = 50;
  this->m_recipientEdit->Background(0xFF, 0xFF, 0xFF);
  this->m_recipientEdit->flat = 0;
  this->m_recipientEdit->Border(0xA6, 0xA6, 0xA6);
  this->m_recipientEdit->TextColor(0x31, 0x31, 0x31, 0xFFFFFFFF);
  this->m_recipientEdit->labelOffsetX = 1;
  this->m_recipientEdit->labelOffsetY = 1;
  this->AddChild(this->m_recipientEdit);

  this->m_confirmedRecipientEdit = new UIEditCtrl_Custom();
  this->m_confirmedRecipientEdit->SetSize(0x96, 0x12);
  this->m_confirmedRecipientEdit->Move(8, 0x1A);
  this->m_confirmedRecipientEdit->maximum = 50;
  this->m_confirmedRecipientEdit->Background(0x110, 0x110, 0x110);
  this->m_confirmedRecipientEdit->flat = 0;
  this->m_confirmedRecipientEdit->Border(0xA6, 0xA6, 0xA6);
  this->m_confirmedRecipientEdit->TextColor(0x0F, 0x33, 0x95, 0xFFFFFFFF);
  this->m_confirmedRecipientEdit->labelBold = 1;
  this->m_confirmedRecipientEdit->labelOffsetX = 1;
  this->m_confirmedRecipientEdit->labelOffsetY = 1;
  this->m_confirmedRecipientEdit->disabled = 1;
  this->m_confirmedRecipientEdit->Invalidate();
  this->m_confirmedRecipientEdit->Show(0);
  this->AddChild(this->m_confirmedRecipientEdit);

  this->m_confirmRecipientButton = new UIBmpTextButton();
  this->m_confirmRecipientButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_out.bmp", 0, 0);
  this->m_confirmRecipientButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_over.bmp", 1, 0);
  this->m_confirmRecipientButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_press.bmp", 2, 0);
  this->m_confirmRecipientButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_confirm_id_empty_press.bmp", 3, 0);
  this->m_confirmRecipientButton->SetSize(this->m_confirmRecipientButton->BitmapWidth(), this->m_confirmRecipientButton->BitmapHeight());
  {
    const int textX = static_cast<int>(
      static_cast<float>(this->m_confirmRecipientButton->geometry.width - this->TextWidth(LookupMessage(MSI_RODEX_TAB_RETURN), 0, 0, 12, 0, 0)) *
      g_depthAverageHalf
    );
    UIBmpTextButton* button = this->m_confirmRecipientButton;
    const int textY = static_cast<int>(
      static_cast<float>(button->geometry.height - this->TextHeight(LookupMessage(MSI_RODEX_TAB_RETURN), 0, 0, 12, 0, 0)) *
      g_depthAverageHalf
    );
    button->AddTextStyle(0, 12, 0x313131, 0, 0, textX, textY);
    this->m_confirmRecipientButton->AddTextStyle(1, 12, 0x313131, 0, 0, textX, textY);
    this->m_confirmRecipientButton->AddTextStyle(2, 12, 0x313131, 0, 1, textX, textY);
    this->m_confirmRecipientButton->AddTextStyle(3, 12, 0xC2C2C2, 0, 0, textX, textY);
  }
  this->m_confirmRecipientButton->SetText(LookupMessage(MSI_RODEX_TAB_RETURN));
  this->m_confirmRecipientButton->Move(0xA2, 0x1A);
  this->m_confirmRecipientButton->SetId(0xD5);
  this->AddChild(this->m_confirmRecipientButton);

  this->m_recipientLevelText = new UIStaticText();
  this->m_recipientLevelText->SetSize(0x100, 0x10);
  this->m_recipientLevelText->Move(0xAC, 0x19);
  this->m_recipientLevelText->SetColor(0xC6, 0x28, 0x28);
  this->m_recipientLevelText->SetFont(0, 11, 0, 0);
  this->m_recipientLevelText->Show(0);
  this->AddChild(this->m_recipientLevelText);

  this->m_recipientJobText = new UIStaticText();
  this->m_recipientJobText->SetSize(0x100, 0x10);
  this->m_recipientJobText->Move(0xAC, 0x25);
  this->m_recipientJobText->SetColor(0xC6, 0x28, 0x28);
  this->m_recipientJobText->SetFont(0, 11, 0, 0);
  this->m_recipientJobText->Show(0);
  this->AddChild(this->m_recipientJobText);

  this->m_recipientIdText = new UIStaticText();
  this->m_recipientIdText->SetSize(0xFD, 0x10);
  this->m_recipientIdText->Move(0xAC, 0x31);
  this->m_recipientIdText->SetColor(0xC6, 0x28, 0x28);
  this->m_recipientIdText->SetFont(0, 11, 0, 0);
  this->m_recipientIdText->Show(0);
  this->AddChild(this->m_recipientIdText);

  this->m_titleEdit = new UIEditCtrl_Custom();
  this->m_titleEdit->SetSize(0x9B, 0xE);
  this->m_titleEdit->Move(6, 0x32);
  this->m_titleEdit->maximum = 50;
  this->m_titleEdit->SetText("TITLE");
  this->AddChild(this->m_titleEdit);

  this->m_contents = new UIRodexEditCtrl3();
  this->m_contents->SetSize(0x10B, 0xC0);
  this->m_contents->invalidatesParent = 1;
  this->m_contents->Move(0xF, 0x51);
  this->m_contents->maximum = 1000;
  const bool selected = g_skinInfo.Selected();
  const SkinPaletteCell cell = {1, 2};
  const std::uint32_t color = SkinPaletteColor(ResourceMgr::Instance(), cell, selected);
  this->m_contents->Background(color & 0xFF, (color & 0xFFFF) >> 8, (color >> 16) & 0xFF);
  this->m_contents->TextColor(0x31, 0x31, 0x31, 0xFFFFFFFF);
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

  this->m_sendButton = new UIBasicButton();
  this->m_sendButton->Configure(0x46, LookupMessage(MSI_RODEX_TAB_DEFAULT));
  this->m_sendButton->Move(0xE2, 0x178);
  this->m_sendButton->SetId(0x159);
  this->AddChild(this->m_sendButton);
}
