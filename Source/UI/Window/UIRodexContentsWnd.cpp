#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "Render/Text/FontCache.h"
#include "UI/Window/UIEnums.h"
#include "Render/Text/TextContext.h"
#include "UI/Window/UIRodexContentsWnd.h"
#include "Client/Mode/CMode.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RodexMailStore.h"
#include "Contents/SkinMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>

namespace {
#pragma pack(push, 1)
struct RodexMailAttachmentRequestPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int64_t mailId;
  /* 000A */ std::uint8_t mailType;
};

struct RodexMailReportPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int32_t mailId;
};
#pragma pack(pop)
}

// OFFSET: 0x00801CC0
// FUNCTION: fn_10014
void UIRodexContentsWnd::SetContentsText(const char* text) {
  this->m_contents->SetText(text);
}

// OFFSET: 0x00801CE0
// FUNCTION: fn_10015
UIRodexContentsWnd::UIRodexContentsWnd()
    : UIFrameWnd(0), m_contents(nullptr), m_zeny(0), m_itemButton(nullptr), m_zenyButton(nullptr), unknown_0104(nullptr),
      unknown_0108(nullptr) {
  this->m_items.clear();
  this->m_itemIndex = 0;
  this->unknown_010C = false;
}

// OFFSET: 0x00801F80
// FUNCTION: fn_10017
ITEM_INFO UIRodexContentsWnd::GetItemAtPoint(int x, int y) {
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

// OFFSET: 0x00802050
// FUNCTION: fn_10018
void UIRodexContentsWnd::DisableItemButton() {
  this->m_itemButton->Enable(0);
  this->Invalidate();
}

// OFFSET: 0x00802070
// FUNCTION: fn_10019
void UIRodexContentsWnd::DisableZenyButton() {
  this->m_zenyButton->Enable(0);
  this->m_zeny = 0;
  this->Invalidate();
}

// OFFSET: 0x008020B0
// FUNCTION: fn_10020
void UIRodexContentsWnd::OnCreate(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  RodexMailStore* store = g_rodexMailStore;
  switch (store->unknown_0010) {
  case MAIL_INBOX_NORMAL: {
    std::map<std::int64_t, RodexMail>::iterator mail = store->m_mails0.find(store->currentMailId);
    if (mail != store->m_mails0.end()) {
      this->SetMailContents(mail->second.unknown_0004, mail->second.unknown_0024, mail->second.unknown_003C, mail->second.hasItem, mail->second.zeny,
                            mail->second.unknown_0001);

      if ((mail->second.unknown_0001 & 8) == 0 && mail->second.unknown_0194 == 0) {
        UIBasicButton* report = new UIBasicButton();
        report->Configure(70, LookupMessage(3548));
        report->Move(224, 29);
        report->SetId(310);
        report->SetDisabled(0);
        report->Invalidate();
        this->AddChild(report);
      }
    }
    break;
  }
  case MAIL_INBOX_ACCOUNT: {
    std::map<std::int64_t, RodexMail>::iterator mail = store->m_mails1.find(store->currentMailId);
    if (mail != store->m_mails1.end()) {
      this->SetMailContents(mail->second.unknown_0004, mail->second.unknown_0024, mail->second.unknown_003C, mail->second.hasItem, mail->second.zeny,
                            mail->second.unknown_0001);
    }
    break;
  }
  case MAIL_INBOX_RETURNED: {
    std::map<std::int64_t, RodexMail>::iterator mail = store->m_mails2.find(store->currentMailId);
    if (mail != store->m_mails2.end()) {
      this->SetMailContents(mail->second.unknown_0004, mail->second.unknown_0024, mail->second.unknown_003C, mail->second.hasItem, mail->second.zeny,
                            mail->second.unknown_0001);
    }
    break;
  }
  }

  this->unknown_0104 = new UIBasicButton();
  this->unknown_0104->Configure(50, LookupMessage(3542));
  this->unknown_0104->Move(6, 376);
  this->unknown_0104->SetId(211);
  this->AddChild(this->unknown_0104);
}

// OFFSET: 0x008023F0
// FUNCTION: fn_10021
void UIRodexContentsWnd::Paint() {
  this->ClearSurface(0xFF00FF);

  const auto* background = static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_rodex_read.bmp"))
  );
  this->DrawBitmap(0, 0, &background->image, false);
  this->DrawTitleBar(true, LookupMessage(2727), 0);
  this->DrawTextAt(8, 29, this->m_title.c_str(), 0, 0, 12, 0x95330F, 1, 0);
  this->DrawTextAt(9, 52, this->m_sender.c_str(), 0, 0, 12, 0x313131, 0, 0);

  const auto* zenyIcon = static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_zeny.bmp"))
  );
  this->DrawBitmap(19, 342, &zenyIcon->image, true);

  if (this->m_zeny > 0) {
    char amount[260];
    std::memset(amount, 0, sizeof(amount));
    FormatGroupedInteger(this->m_zeny, amount, sizeof(amount));
    const std::string text = FormatOwnedString("%s Zeny", amount);
    this->DrawTextAt(50, 350, text.c_str(), 0, 0, 11, 0x737373, 1, 0);
  }

  for (int x = 26; x < 196; x += 34) {
    const auto* slot = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_off.bmp"))
    );
    this->DrawBitmap(x, 319, &slot->image, true);
  }

  int x = 30;
  for (auto item = this->m_items.begin(); item != this->m_items.end(); ++item) {
    const auto* slot = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))
    );
    this->DrawBitmap(x - 4, 319, &slot->image, true);
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, 309, *item, 0, 0);
    if (item->unknown_0088 > 0) {
      ResourceMgr& resources = ResourceMgr::Instance();
      const std::string name = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item->unknown_0088);
      const auto* grade = static_cast<CBitmapRes*>(resources.Load(Resolve(name.c_str())));
      this->DrawBitmap(x, 321, &grade->image, true);
    }
    x += 34;
  }
}

// OFFSET: 0x008026E0
// FUNCTION: fn_10022
void UIRodexContentsWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x00802700
// FUNCTION: fn_10023
void UIRodexContentsWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    int tooltipX = this->m_itemIndex * 34 + 30;
    int tooltipY = 290;
    this->ToScreen(tooltipX, tooltipY);
    CMode* tooltip = g_modeMgr.gameMode;
    if (item.identified == 1 && item.unknown_0098 != 0) {
      tooltip->ShowTooltip(FormatOwnedString(LookupMessage(2600), item.DisplayName(0).c_str(), item.unknown_0098).c_str(), tooltipX, tooltipY,
                    g_session.GetItemNameColor(item), 0, 1);
    } else {
      tooltip->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00802910
// FUNCTION: fn_10024
void UIRodexContentsWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x008029E0
// FUNCTION: fn_10025
int UIRodexContentsWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == UM_COMMAND) {
    switch (argument1) {
    case ID_BUTTON1:
      if (this->m_items.size() != 0) {
        if (g_modeMgr.GetGameMode() && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsMenuWindowOpen() == 1) {
          break;
        }
        RodexMailAttachmentRequestPacket packet;
        packet.packetType = 0x9F3;
        packet.mailId = g_rodexMailStore->currentMailId;
        packet.mailType = g_rodexMailStore->unknown_0010;
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        UIBitmapButton* button = this->m_itemButton;
        button->SetDisabled(1);
        button->Invalidate();
        this->Invalidate();
      }
      break;
    case ID_BUTTON2:
      if (this->m_zeny > 0) {
        if (g_modeMgr.GetGameMode() && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsMenuWindowOpen() == 1) {
          break;
        }
        RodexMailAttachmentRequestPacket packet;
        packet.packetType = 0x9F1;
        packet.mailId = g_rodexMailStore->currentMailId;
        packet.mailType = g_rodexMailStore->unknown_0010;
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        UIBitmapButton* button = this->m_zenyButton;
        button->SetDisabled(1);
        button->Invalidate();
        this->Invalidate();
      }
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_RODEXCONTENTSWND);
      g_rodexMailStore->currentMailId = 0;
      if (g_windowMgr.unknown_0454) {
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0454)->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      }
      break;
    case ID_DELETE: {
      RodexMailStore* store = g_rodexMailStore;
      std::map<std::int64_t, RodexMail>* mails;
      if (store->unknown_0010 == 0) {
        mails = &store->m_mails0;
      } else if (store->unknown_0010 == 1) {
        mails = &store->m_mails1;
      } else {
        mails = &store->m_mails2;
      }

      std::map<std::int64_t, RodexMail>::iterator mail = mails->find(store->currentMailId);
      if (mail == mails->end()) {
        break;
      }

      if ((mail->second.unknown_0001 & 6) != 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(0xA34)), 0x64FFFF, 0, 0, 0);
        break;
      }

      if (g_windowMgr.Confirm(LookupMessage(0x164), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
        g_modeMgr.GetGameMode()->SendMsg(MM_DELETE_MAIL, static_cast<int>(g_rodexMailStore->currentMailId), g_rodexMailStore->unknown_0010, 0, 0);
        if (g_windowMgr.unknown_045C != 0) {
          g_windowMgr.CloseWindow(WID_RODEXCONTENTSWND);
        }
      }
      break;
    }
    case ID_SEND_MAIL:
      if (this->unknown_010C != 1) {
        g_modeMgr.GetGameMode()->SendMsg(MM_OPEN_WRITE_RODEX, reinterpret_cast<int>(this->m_title.c_str()), 0, 0, 0);
      }
      break;
    case ID_REPORT_RODEX: {
      RodexMailReportPacket packet;
      packet.packetType = 0xB98;
      packet.mailId = static_cast<std::int32_t>(g_rodexMailStore->currentMailId);
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      break;
    }
    }
  } else if (message == UM_REFRESHITEM) {
    this->m_items.clear();
    for (int i = 0; i < 5; ++i) {
      ITEM_INFO item = g_session.GetItem5C68(i);
      if (item.quantity > 0) {
        this->m_items.push_back(item);
      }
    }
    this->Invalidate();
  } else if (message == UM_SET_DISABLE) {
    if (this->unknown_010C && this->unknown_0108) {
      UIBasicButton* button = this->unknown_0108;
      button->SetDisabled(1);
      button->Invalidate();
    }
  } else {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x008030A0
// FUNCTION: fn_10026
void UIRodexContentsWnd::SetMailContents(std::string title, std::string sender, std::string body, unsigned char hasItem, std::int64_t zeny, unsigned char flags) {
  this->m_title = title;
  this->m_sender = sender;
  std::string text = body;

  std::pair<bool, std::string> lineBreak = RemoveLineBreakTag(this->m_sender);
  this->unknown_010C = lineBreak.first;
  this->m_sender = lineBreak.second;

  this->m_title = g_navigationLinkParser->ExpandMessages(this->m_title);
  this->m_sender = g_navigationLinkParser->ExpandMessages(this->m_sender);
  text = g_navigationLinkParser->ExpandMessages(text);

  this->m_contents = new UIRodexEditCtrl3();
  this->m_contents->SetSize(267, 192);
  this->m_contents->invalidatesParent = 1;
  this->m_contents->Move(15, 81);
  this->m_contents->maximum = 1000;

  const bool selected = g_skinInfo.Selected();
  const SkinPaletteCell cell = {1, 2};
  const std::uint32_t color = SkinPaletteColor(ResourceMgr::Instance(), cell, selected);
  this->m_contents->Background(color & 0xFF, (color & 0xFFFF) >> 8, (color >> 16) & 0xFF);
  this->m_contents->TextColor(0x313131, 0xFFFFFFFF);
  this->m_contents->disabled = 1;
  this->m_contents->Invalidate();
  reinterpret_cast<unsigned char*>(&this->m_contents->unknown_0184)[1] = 1;
  this->m_contents->SetId(504);

  if (text.find("<NAVIL>") == std::string::npos && text.find("</NAVIL>") == std::string::npos) {
    this->m_contents->SetText(text.c_str());
  } else {
    this->m_contents->SendMsg(nullptr, UM_INSERT_ITEM_LINK, reinterpret_cast<std::intptr_t>(text.c_str()), 0, 0, 0);
  }

  this->m_contents->SetTagColor(0xD630A9);
  this->AddChild(this->m_contents);

  if (hasItem) {
    this->m_itemButton = new UIBitmapButton();
    this->m_itemButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_receive_out.bmp", 0, 0);
    this->m_itemButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_receive_over.bmp", 1, 0);
    this->m_itemButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_receive_press.bmp", 2, 0);
    this->m_itemButton->SetSize(this->m_itemButton->BitmapWidth(), this->m_itemButton->BitmapHeight());
    this->m_itemButton->Move(252, 312);
    this->m_itemButton->SetId(189);
    this->AddChild(this->m_itemButton);
  }

  if (zeny > 0) {
    this->m_zeny = zeny;
    this->m_zenyButton = new UIBitmapButton();
    this->m_zenyButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_receive_out.bmp", 0, 0);
    this->m_zenyButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_receive_over.bmp", 1, 0);
    this->m_zenyButton->SetBitmap("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\btn_receive_press.bmp", 2, 0);
    this->m_zenyButton->SetSize(this->m_zenyButton->BitmapWidth(), this->m_zenyButton->BitmapHeight());
    this->m_zenyButton->Move(252, 345);
    this->m_zenyButton->SetId(190);
    this->AddChild(this->m_zenyButton);
  }

  if (g_rodexMailStore->unknown_0010 == 0 && (flags & 8) == 0) {
    this->unknown_0108 = new UIBasicButton();
    this->unknown_0108->Configure(70, LookupMessage(3543));
    this->unknown_0108->Move(224, 376);
    this->unknown_0108->SetId(305);
    this->unknown_0108->SetDisabled(0);
    this->unknown_0108->Invalidate();
    this->AddChild(this->unknown_0108);
  }

  if (this->unknown_010C == 1) {
    this->SendMsg(nullptr, UM_SET_DISABLE, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00803760
// FUNCTION: fn_10027
void UIRodexContentsWnd::SetItemButtonDisabled(unsigned char disabled) {
  UIBitmapButton* button = this->m_itemButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x00803790
// FUNCTION: fn_10028
void UIRodexContentsWnd::SetZenyButtonDisabled(unsigned char disabled) {
  UIBitmapButton* button = this->m_zenyButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->Invalidate();
}
