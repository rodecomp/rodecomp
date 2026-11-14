#include "Render/Renderer.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Menu/UIMenuIconWnd.h"

#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <shellapi.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include "Contents/Guild/GuildInfo.h"

#include "Resource/MSGSTRINGID.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#pragma pack(push, 1)
struct MenuIconBankingCheckPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
};

struct MenuIconOpenUiPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char type;
};
#pragma pack(pop)


// OFFSET: 0x0084A1E0
// FUNCTION: fn_10606
UIMenuIcon::UIMenuIcon() {}

// OFFSET: 0x0084A240
// FUNCTION: fn_10607
UIMenuIconWnd::UIMenuIconWnd()
    : UIFrameWnd(1), openButton(nullptr), notificationOpenButton(nullptr), closeButton(nullptr) {}

// OFFSET: 0x0084A850
// FUNCTION: fn_10612
bool UIMenuIconWnd::ToggleWindow(int id) {
  const bool closed = g_windowMgr.CloseWindow(id);
  if (!closed) {
    g_windowMgr.EnsureWindow(id);
  }

  return !closed;
}

// OFFSET: 0x0084A880
// FUNCTION: fn_10613
UIWindow* UIMenuIconWnd::HitTest(int x, int y) {
  if (!this->CanHitTest() || this->geometry.visible == 0 || this->geometry.x > x) {
    return nullptr;
  }

  if (!g_session.m_showMenuIcon) {
    if (this->geometry.x + this->geometry.width <= x || this->geometry.y > y || this->geometry.y + this->notificationOpenButton->geometry.height <= y) {
      return nullptr;
    }
    if (this->notifiedCommands.size() == 0) {
      return this->openButton;
    }
    return this->notificationOpenButton;
  }

  if (this->geometry.x + this->geometry.width <= x || this->geometry.y > y || this->geometry.y + this->geometry.height <= y) {
    return nullptr;
  }

  for (auto child = this->m_children.rbegin(); child != this->m_children.rend(); ++child) {
    UIWindow* hit = (*child)->HitTest((x - this->geometry.x), (y - this->geometry.y));
    if (hit) {
      return hit;
    }
  }

  return this;
}

// OFFSET: 0x0084A960
// FUNCTION: fn_10614
bool UIMenuIconWnd::IsTransmitMouseInput() {
  return true;
}

// OFFSET: 0x0084A970
// FUNCTION: fn_10615
void UIMenuIconWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y - 4;
  g_windowMgr.StoreSnapRectangle(this);
}

// OFFSET: 0x0084A9A0
// FUNCTION: fn_10616
void UIMenuIconWnd::OnCreate(int width, int height) {
  const int ids[24] = {ID_STATUS, ID_EQUIP, ID_ITEMS, ID_SKILL, ID_SELECT_PARTY_LISTING, ID_FRIEND, MENUICON_GUILD, MENUICON_BATTLE, ID_QUEST, ID_MAP, MENUICON_NAVIGATION, ID_OPTION, MENUICON_BANK, MENUICON_REC, MENUICON_MAIL, MENUICON_ACHIEVEMENT, MENUICON_TIP, MENUICON_SHOP, ID_OPEN_HOTKEY, MENUICON_ATTENDANCE, ID_TOGGLE_PARTY_BOARD, MENUICON_REPUTE, MENUICON_ADVENTURE_GUIDE, MENUICON_PROBABILITY};
  std::string names[24] = {
    "status", "equip", "item", "skill", "booking", "party", "guild", "battle", "quest", "map", "navigation", "option",
    "bank", "rec", "mail", "achievement", "tip", "shop", "keyboard", "attendance", "adventurerAgency", "repute", "adventureguide", "probability",
  };

  unsigned int position = 0;
  for (int index = 0; index < 24; ++index) {
    if (ids[index] == ID_STATUS && g_session.IsDoramJob(g_session.m_job) == true) {
      names[index] = "status_doram";
    }

    if (ids[index] == ID_SELECT_PARTY_LISTING || ids[index] == 600) {
      continue;
    }

    UIMenuIcon* icon = new UIMenuIcon();
    icon->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s.bmp", names[index].c_str()).c_str(), 0, 0);
    icon->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s.bmp", names[index].c_str()).c_str(), 1, 0);
    icon->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_press.bmp", names[index].c_str()).c_str(), 2, 0);
    icon->SetSize(icon->BitmapWidth(), icon->BitmapHeight());
    const int row = position / 5 * 43;
    const int x = position % 5 * 42 + 9;
    icon->Move(x, row + 23);
    icon->SetId(ids[index]);
    icon->Enable(0);
    this->AddChild(icon);
    this->menuButtons[ids[index]] = icon;

    switch (ids[index]) {
    case ID_STATUS:
    case ID_ITEMS:
    case ID_SKILL:
    case MENUICON_ACHIEVEMENT:
    case MENUICON_MAIL: {
      UIMenuIcon* badge = new UIMenuIcon();
      badge->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_new.bmp", names[index].c_str()).c_str(), 0, 0);
      badge->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_new.bmp", names[index].c_str()).c_str(), 1, 0);
      badge->SetBitmap(FormatOwnedString("유저인터페이스\\menu_icon\\bt_%s_new_press.bmp", names[index].c_str()).c_str(), 2, 0);
      badge->SetSize(badge->BitmapWidth(), badge->BitmapHeight());
      badge->Move(x, row + 17);
      badge->SetId(ids[index]);
      badge->Enable(0);
      this->AddChild(badge);
      this->notificationButtons[ids[index]] = badge;
      break;
    }
    }

    ++position;
  }

  this->openButton = new UIBitmapButton();
  this->openButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_normal.bmp", 0, 0);
  this->openButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_over.bmp", 1, 0);
  this->openButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_press.bmp", 2, 0);
  this->openButton->SetSize(this->openButton->BitmapWidth(), this->openButton->BitmapHeight());
  this->openButton->Move(0, 4);
  this->openButton->SetId(MENUICON_OPEN);
  this->openButton->Enable(0);
  this->AddChild(this->openButton);
  this->notificationOpenButton = new UIBitmapButton();
  this->notificationOpenButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_new_normal.bmp", 0, 0);
  this->notificationOpenButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_new_over.bmp", 1, 0);
  this->notificationOpenButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_new_press.bmp", 2, 0);
  this->notificationOpenButton->SetSize(this->notificationOpenButton->BitmapWidth(), this->notificationOpenButton->BitmapHeight());
  this->notificationOpenButton->Move(0, 0);
  this->notificationOpenButton->SetId(MENUICON_OPEN);
  this->notificationOpenButton->Enable(0);
  this->AddChild(this->notificationOpenButton);
  this->closeButton = new UIBitmapButton();
  this->closeButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_close_normal.bmp", 0, 0);
  this->closeButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_close_over.bmp", 1, 0);
  this->closeButton->SetBitmap("유저인터페이스\\menu_icon\\bt_menu_close_press.bmp", 2, 0);
  this->closeButton->SetSize(this->closeButton->BitmapWidth(), this->closeButton->BitmapHeight());
  this->closeButton->Move(0, 4);
  this->closeButton->SetId(ID_CLOSE);
  this->closeButton->Enable(0);
  this->AddChild(this->closeButton);
  this->SendMsg(this, UM_COMMAND, g_session.m_showMenuIcon == true ? MENUICON_OPEN : ID_CLOSE, 0, 0, 0);
  if (this->menuButtons.size() > 20) {
    this->Resize(220, 238);
  }
}

// OFFSET: 0x0084B940
// FUNCTION: fn_10617
void UIMenuIconWnd::Paint() {
  this->m_primitives.clear();
  this->openButton->Enable(0);
  this->notificationOpenButton->Enable(0);
  this->closeButton->Enable(0);

  UIButton* toggle;
  if (!g_session.m_showMenuIcon) {
    if (this->notifiedCommands.size() != 0) {
      toggle = this->notificationOpenButton;
    } else {
      toggle = this->openButton;
    }
  } else {
    toggle = this->closeButton;
  }
  toggle->Enable(1);

  for (auto it = this->menuButtons.begin(); it != this->menuButtons.end(); ++it) {
    if (!g_session.m_showMenuIcon) {
      it->second->Enable(0);
      if (this->notificationButtons.end() != this->notificationButtons.find(it->first)) {
        this->notificationButtons[it->first]->Enable(0);
      }
    } else {
      this->DrawNamed(g_renderer->primitives, 0, 13, "유저인터페이스\\menu_icon\\bg_menu.tga", 0xFFFFFFFF, 0, 0);
      const bool pinned = std::find(this->notifiedCommands.begin(), this->notifiedCommands.end(), it->first) != this->notifiedCommands.end();
      it->second->Enable(!pinned);
      if (this->notificationButtons.end() != this->notificationButtons.find(it->first)) {
        this->notificationButtons[it->first]->Enable(pinned);
      }
    }
  }
}

// OFFSET: 0x0084BD20
// FUNCTION: fn_10618
void UIMenuIconWnd::OnLBtnDown(int x, int y) {}

// OFFSET: 0x0084BD30
// FUNCTION: fn_10619
void UIMenuIconWnd::OnLBtnUp(int x, int y) {}

// OFFSET: 0x0084BD40
// FUNCTION: fn_10620
void UIMenuIcon::OnMouseMove(int x, int y) {
  int behaviorId = 10041;
  int message = 4698;
  bool known = true;
  switch (this->geometry.id) {
  case ID_STATUS:
    behaviorId = WID_STATUSWND;
    message = MSI_STATUS;
    break;
  case ID_EQUIP:
    behaviorId = WID_EQUIPWND;
    message = MSI_EQUIPED_ITEM;
    break;
  case ID_ITEMS:
    behaviorId = WID_ITEMWND;
    message = MSI_ITEM;
    break;
  case ID_SKILL:
    behaviorId = WID_SKILLLISTWND;
    message = MSI_SKILLLIST;
    break;
  case ID_FRIEND:
    behaviorId = WID_MESSENGERWND;
    message = MSI_PARTY;
    break;
  case MENUICON_GUILD:
    behaviorId = WID_GUILDWND;
    message = MSI_CASHSHOP;
    break;
  case MENUICON_BATTLE:
    behaviorId = WID_JOINTO_BATTLEFIELDWND;
    message = MSI_GRAPHIC_MSG_BATTLE;
    break;
  case ID_QUEST:
    behaviorId = 134;
    message = MSI_QUESTWIN;
    break;
  case ID_MAP:
    behaviorId = WID_ROMAPWND;
    message = MSI_MINIMAP_BUTTON_ON;
    break;
  case MENUICON_NAVIGATION:
    behaviorId = WID_NAVIGATIONV4WND;
    message = MSI_CLAN_LEVEL;
    break;
  case ID_OPTION:
    message = MSI_OPTIONWND;
    break;
  case MENUICON_REC:
    behaviorId = WID_REPLAYRECCONTROLWND;
    message = MSI_MENU_NAVIGATIONWND;
    break;
  case ID_OPEN_HOTKEY:
    message = MSI_IMAGE_CAPTURE;
    break;
  case MENUICON_BANK:
    behaviorId = 251;
    message = MSI_SIT;
    break;
  case MENUICON_MAIL:
    behaviorId = WID_RODEXINBOXWND;
    message = MSI_MOVE;
    break;
  case MENUICON_ACHIEVEMENT:
    behaviorId = WID_ACHIEVEMENTWND;
    message = MSI_ACHIEVEMENT_SCORE_REQUIRED_TO_REWARD;
    break;
  case MENUICON_TIP:
    behaviorId = WID_TIPBOXWND;
    message = MSI_REFINING_AGAIN_FAILED_INGREDIENT_CHANGED;
    break;
  case MENUICON_SHOP:
    behaviorId = WID_CASHSHOP_WND;
    message = MSI_ROTATE;
    break;
  case MENUICON_ATTENDANCE:
    behaviorId = WID_CHECKATTENDANCEWND;
    message = MSI_CAN_NOT_OFFER_EXP_TO_GUILD_MAX_EXP;
    break;
  case ID_TOGGLE_PARTY_BOARD:
    behaviorId = WID_ADVENPARTYBOARDWND;
    message = MSI_CANNOT_MORE_SELECT_JOB;
    break;
  case MENUICON_REPUTE:
    behaviorId = WID_REPUTEWND;
    message = MSI_REPUTE_NOW_LOADING;
    break;
  case MENUICON_ADVENTURE_GUIDE:
    behaviorId = WID_ADVENTUREGUIDE;
    message = MSI_RUNESYSTEM_CLOSE_RODEX;
    break;
  case MENUICON_PROBABILITY:
    message = MSI_REFINING_PROBABILITY_URL;
    break;
  default:
    known = false;
    break;
  }

  if (known) {
    const char* behavior = nullptr;
    g_session.m_hotkeyScript->Call("GetBeHaviorOfWindow", "d>s", behaviorId, &behavior);
    if (std::strlen(behavior) != 0) {
      const std::string text = FormatOwnedString("%s (%s)", LookupMessage(message), behavior);
      this->m_tooltip = text;
    } else {
      this->m_tooltip = LookupMessage(message);
    }
  }

  if (this->geometry.id == ID_OPTION) {
    const std::string text = FormatOwnedString("%s (%s)", LookupMessage(message), "Esc");
    this->m_tooltip = text;
  }
}

// OFFSET: 0x0084C250
// FUNCTION: fn_10621
void UIMenuIconWnd::OnMouseMove(int x, int y) {}

// OFFSET: 0x0084C260
// FUNCTION: fn_10622
int UIMenuIconWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == UM_COMMAND) {
    if (!Replay::Instance().AllowsEvent(argument1)) {
      switch (argument1) {
      case ID_CLOSE:
        g_session.m_showMenuIcon = false;
        this->Invalidate();
        break;
      case ID_NOTICE: {
        const int id = argument2;
        auto found = std::find(this->notifiedCommands.begin(), this->notifiedCommands.end(), id);
        if (argument3 > 0) {
          if (found == this->notifiedCommands.end()) {
            this->notifiedCommands.push_back(id);
          }
        } else if (found != this->notifiedCommands.end()) {
          this->notifiedCommands.erase(found);
        }
        this->Invalidate();
        break;
      }
      case ID_STATUS:
        this->ToggleWindow(WID_STATUSWND);
        break;
      case ID_EQUIP:
        this->ToggleWindow(WID_EQUIPWND);
        break;
      case ID_ITEMS:
        this->ToggleWindow(WID_ITEMWND);
        break;
      case ID_SKILL:
        this->ToggleWindow(WID_SKILLLISTWND);
        break;
      case ID_FRIEND:
        this->ToggleWindow(WID_MESSENGERWND);
        break;
      case MENUICON_GUILD:
        this->ToggleWindow(static_cast<int>(g_guildInfo.m_guildId) < 1 ? WID_GUILDHELPERWND : WID_GUILDWND);
        break;
      case ID_QUEST:
        this->ToggleWindow(WID_RENEWQUESTUI);
        break;
      case ID_MAP:
        this->ToggleWindow(WID_ROMAPWND);
        break;
      case MENUICON_NAVIGATION:
        if (g_windowMgr.navigationWnd) {
          g_windowMgr.navigationWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
        } else {
          g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
        }
        break;
      case ID_OPTION:
        this->ToggleWindow(WID_ESCOPTIONWND);
        break;
      case MENUICON_REC:
        if (Replay::Instance().state != 1) {
          g_windowMgr.Confirm("REC End please", 0, true, false, 0, "Message", 280, 120, 0);
        } else {
          this->ToggleWindow(WID_REPLAYRECCONTROLWND);
        }
        break;
      case ID_OPEN_HOTKEY:
        this->ToggleWindow(WID_HOTKEYGUIDEWND);
        break;
      case MENUICON_BANK: {
        MenuIconBankingCheckPacket packet;
        packet.id = ID_CZ_REQ_BANKING_CHECK;
        packet.aid = g_session.m_aid;
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
        break;
      }
      case MENUICON_OPEN:
        g_session.m_showMenuIcon = true;
        this->Invalidate();
        break;
      case MENUICON_MAIL:
        if (g_windowMgr.LookupWindow(WID_REFININGWND)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_AURA_EFFECT_OFF)), 255, 0, 0, 0);
        } else if (g_windowMgr.LookupWindow(WID_GRADEENCHANTWND)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BROADCASTING_GRADE_ENCHANT_SUCCESS)), 255, 0, 0, 0);
        } else if (g_windowMgr.LookupWindow(WID_ENCHANTUI)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ENCHANT_REQUEST)), 255, 0, 0, 0);
        } else {
          this->ToggleWindow(WID_RODEXINBOXWND);
        }
        break;
      case MENUICON_ACHIEVEMENT:
        this->ToggleWindow(WID_ACHIEVEMENTWND);
        break;
      case MENUICON_TIP:
        this->ToggleWindow(WID_TIPBOXWND);
        break;
      case MENUICON_SHOP:
        g_modeMgr.GetGameMode()->SendMsg(MM_REQUEST_CASH_SHOP, 0, 0, 0, 0);
        break;
      case MENUICON_ATTENDANCE:
        if (g_windowMgr.LookupWindow(WID_CHECKATTENDANCEWND)) {
          g_windowMgr.CloseWindow(WID_CHECKATTENDANCEWND);
        } else {
          MenuIconOpenUiPacket packet;
          packet.id = ID_CZ_UI_OPEN;
          packet.type = 5;
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
        }
        break;
      case ID_TOGGLE_PARTY_BOARD:
        this->ToggleWindow(WID_ADVENPARTYBOARDWND);
        break;
      case MENUICON_REPUTE:
        this->ToggleWindow(WID_REPUTEWND);
        break;
      case MENUICON_ADVENTURE_GUIDE:
        this->ToggleWindow(WID_ADVENTUREGUIDE);
        break;
      case MENUICON_PROBABILITY:
        if (g_windowMgr.Confirm(LookupMessage(MSI_GENDER_CHANGE_FAILED_CAUSE_MONSTER_TRANSFORM), 2, true, false, 0, "Message", 280, 120, 0) == ID_YES) {
          ShellExecuteA(hWnd, "open", LookupMessage(MSI_GENDER_CHANGE_FAILED_CAUSE_MARRIED), nullptr, nullptr, SW_SHOWNORMAL);
        }
        break;
      }
    }
  } else if (message == UM_REFRESH) {
    this->SendMsg(nullptr, UM_COMMAND, g_session.m_showMenuIcon == true ? ID_CLOSE : MENUICON_OPEN, 0, 0, 0);
  }

  if (message == UM_COMMAND && argument1 == MENUICON_BATTLE && !Replay::Instance().AllowsEvent(argument1) && g_entryQueueMgr) {
    if (reinterpret_cast<const unsigned char*>(g_entryQueueMgr)[0x3C] == 1) {
      g_entryQueueMgr->SendRankingRequest();
    }
    this->ToggleWindow(WID_JOINTO_BATTLEFIELDWND);
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
