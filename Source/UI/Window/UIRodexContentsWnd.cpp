#include "UI/Window/UIRodexContentsWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RodexMailStore.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
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
UIRodexContentsWnd::UIRodexContentsWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_contents(nullptr), m_zeny(0), m_itemButton(nullptr), m_zenyButton(nullptr), unknown_0104(nullptr),
      unknown_0108(nullptr), m_controls(&controls) {
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

// OFFSET: 0x008023F0
// FUNCTION: fn_10021
void UIRodexContentsWnd::Paint() {
  auto& controls = *this->m_controls;
  this->ClearSurface(0xFF00FF);

  const auto* background = static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\bg_rodex_read.bmp"))
  );
  this->DrawBitmap(0, 0, &background->image, false);
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2727), 0);
  this->DrawTextAt(controls.fonts, controls.locale, 8, 29, this->m_title.c_str(), 0, 0, 12, 0x95330F, 1, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 9, 52, this->m_sender.c_str(), 0, 0, 12, 0x313131, 0, 0);

  const auto* zenyIcon = static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\icon_zeny.bmp"))
  );
  this->DrawBitmap(19, 342, &zenyIcon->image, true);

  if (this->m_zeny > 0) {
    char amount[260];
    std::memset(amount, 0, sizeof(amount));
    FormatGroupedInteger(this->m_zeny, amount, sizeof(amount));
    const std::string text = FormatOwnedString("%s Zeny", amount);
    this->DrawTextAt(controls.fonts, controls.locale, 50, 350, text.c_str(), 0, 0, 11, 0x737373, 1, 0);
  }

  for (int x = 26; x < 196; x += 34) {
    const auto* slot = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_off.bmp"))
    );
    this->DrawBitmap(x, 319, &slot->image, true);
  }

  ItemDescriptions descriptions{g_itemDescriptions};
  int x = 30;
  for (auto item = this->m_items.begin(); item != this->m_items.end(); ++item) {
    const auto* slot = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\shadow_item_on.bmp"))
    );
    this->DrawBitmap(x - 4, 319, &slot->image, true);
    DrawItemWithQuantity(*this, controls.resources, descriptions, controls.fonts, controls.locale, x, 309, *item, 0, 0);
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
void UIRodexContentsWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x00802700
// FUNCTION: fn_10023
void UIRodexContentsWnd::Slot28(int x, int y) {
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
      tooltip->Show(FormatOwnedString(LookupMessage(2600), naming.DisplayName(item, 0).c_str(), item.unknown_0098).c_str(), tooltipX, tooltipY,
                    g_session.GetItemNameColor(item), 0, 1);
    } else {
      tooltip->Show(naming.DisplayName(item, 0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00802910
// FUNCTION: fn_10024
void UIRodexContentsWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x008029E0
// FUNCTION: fn_10025
int UIRodexContentsWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    switch (argument1) {
    case 0xBD:
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
    case 0xBE:
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
    case 0xC9:
      g_windowMgr.CloseWindow(265);
      g_rodexMailStore->currentMailId = 0;
      if (g_windowMgr.unknown_0454) {
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0454)->SendMsg(nullptr, 60, 0, 0, 0, 0);
      }
      break;
    case 0xD3: {
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
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA34)), 0x64FFFF, 0, 0, 0);
        break;
      }

      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x164), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
        g_modeMgr.GetGameMode()->SendMsg(0xC4, static_cast<int>(g_rodexMailStore->currentMailId), g_rodexMailStore->unknown_0010, 0, 0);
        if (g_windowMgr.unknown_045C != 0) {
          g_windowMgr.CloseWindow(265);
        }
      }
      break;
    }
    case 0x131:
      if (this->unknown_010C != 1) {
        g_modeMgr.GetGameMode()->SendMsg(0x10C, reinterpret_cast<int>(this->m_title.c_str()), 0, 0, 0);
      }
      break;
    case 0x136: {
      RodexMailReportPacket packet;
      packet.packetType = 0xB98;
      packet.mailId = static_cast<std::int32_t>(g_rodexMailStore->currentMailId);
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      break;
    }
    }
  } else if (message == 23) {
    this->m_items.clear();
    for (int i = 0; i < 5; ++i) {
      ITEM_INFO item = g_session.GetItem5C68(i);
      if (item.quantity > 0) {
        this->m_items.push_back(item);
      }
    }
    this->Invalidate();
  } else if (message == 106) {
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
