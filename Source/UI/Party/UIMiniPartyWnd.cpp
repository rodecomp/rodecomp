#include "UI/Party/UIMiniPartyWnd.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "File/ShortcutFilename.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Buttons/UITextButton.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Party/MiniPartyGrid.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Mode/CMode.h"

#include "Resource/MSGSTRINGID.h"
#include "UI/Party/UIPartySettingWnd.h"
#include <windows.h>
#include <timeapi.h>
#include <new>

#include <cstdio>
#include <cstring>
#include <list>
#include <string>

// OFFSET: 0x007426F0
// FUNCTION: fn_07737
UIMiniPartyWnd::UIMiniPartyWnd()
    : UIFrameWnd(1), m_cell(-1), m_infoWnd(nullptr), m_background(nullptr), m_sizeMode(MINIPARTY_SIZE_WIDE),
      m_imageName("유저인터페이스\\renewalparty\\img_partymini_bg.tga") {
  this->geometry.id = WID_MINIPARTYWND;
}

// OFFSET: 0x00742950
// FUNCTION: fn_45751
UIMiniPartyWnd::~UIMiniPartyWnd() = default;

// OFFSET: 0x00742AA0
// FUNCTION: fn_07739
void UIMiniPartyWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y;
}

// OFFSET: 0x00742AC0
// FUNCTION: fn_07740
void UIMiniPartyWnd::OnCreate(int, int) {
  UIMiniPartyInfoWnd* info = new UIMiniPartyInfoWnd();
  this->m_infoWnd = info;
  info->m_owner = this;
  this->m_infoWnd->SetSize(174, 52);
  this->m_infoWnd->Move(0, 0);
  this->AddChild(this->m_infoWnd);
  this->m_widths.resize(3);
  this->m_widths[0] = 174;
  this->m_widths[1] = 101;
  this->m_widths[2] = 64;
}

// OFFSET: 0x00742BE0
// FUNCTION: fn_07741
void UIMiniPartyWnd::Paint() {
  this->m_primitives.clear();
  const int count = this->m_member.offline != 1 ? 3 : 1;
  for (int index = 0; index < count; ++index) {
    this->DrawNamed(0, 0, this->m_imageName, 0xFFFFFFFF, 0, 0);
  }
}

// OFFSET: 0x00742D80
// FUNCTION: fn_07742
void UIMiniPartyWnd::OnLBtnDown(int x, int y) {
  if (g_windowMgr.lastWindowId != this->geometry.id) {
    g_windowMgr.BringToFront(this->geometry.id);
    g_windowMgr.lastWindowId = this->geometry.id;
  }

  g_windowMgr.SetCapture(this);
  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);
  this->m_dragX = screenX;
  this->m_dragY = screenY;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  this->geometry.alphaStart = timeGetTime();
  g_snapMgr.CaptureOrigins(this);
}

// OFFSET: 0x00740B80
// FUNCTION: fn_07731
void UIMiniPartyInfoWnd::OnLBtnDown(int x, int y) {
  if (!g_session.m_lockMouse) {
    this->m_owner->OnLBtnDown(x, y);
  }

  if (this->m_owner->GetMember().valid && this->m_owner->GetMember().offline == 0) {
    CGameMode* mode = g_modeMgr.GetGameMode();
    if (mode) {
      mode->TargetActorByAid(this->m_owner->GetMember().accountId);
    }
  }
}

// OFFSET: 0x00740D10
// FUNCTION: fn_07732
void UIMiniPartyInfoWnd::OnLBtnUp(int x, int y) {
  if (g_session.m_lockMouse != true && this == g_windowMgr.Capture()) {
    this->m_owner->OnLBtnUp(x, y);
  }
}

// OFFSET: 0x00740D40
// FUNCTION: fn_07733
void UIMiniPartyInfoWnd::OnMouseHover(int, int) {
  CGameMode* mode = g_modeMgr.GetGameMode();
  if (!mode) {
    return;
  }

  if (this->m_owner->GetMember().valid == 0) {
    return;
  }

  char text[256];
  std::memset(text, 0, sizeof(text));
  std::string fitted;
  const std::string mapName = g_session.GetMapDisplayName(this->m_owner->GetMember().map.c_str());
  if (this->m_owner->m_sizeMode == MINIPARTY_SIZE_COMPACT) {
    std::sprintf(text, "%s", this->m_owner->GetMember().name.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.532f), text, 0, 12, 1);
  } else {
    std::sprintf(text, "Lv.%d %s(%s)", this->m_owner->GetMember().level, this->m_owner->GetMember().name.c_str(), mapName.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.6f), text, 0, 12, 1);
  }

  int x;
  int y;
  if (fitted.find(text) == std::string::npos) {
    x = this->geometry.x + 15;
    y = this->geometry.y - 12;
    this->ToScreen(x, y);
    mode->ShowTooltip(text, x, y, 0xFFFFFFFF, 0, 1);
  }
}

// OFFSET: 0x00741280
// FUNCTION: fn_07734
void UIMiniPartyInfoWnd::OnRBtnUp(int, int) {
  if (!g_session.m_lockMouse) {
    this->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
  }
}

// OFFSET: 0x007412A0
// FUNCTION: fn_07735
int UIMiniPartyInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_REFRESHITEM: {
    if (this->m_owner->GetMember().valid) {
      if (!this->m_owner->GetMember().valid) {
        break;
      }
    }
    CGameMode* mode = g_modeMgr.GetGameMode();
    if (!this->m_owner->GetMember().offline) {
      int current = 0;
      int maximum = 0;
      const bool self = this->m_owner->GetMember().accountId == g_session.m_aid;
      CWorld* world = mode->scene;
      CRenderObject* actor;
      if (self) {
        actor = world->player;
        current = g_session.hp;
        maximum = g_session.maxHp;
      } else {
        actor = world->GetGameActorByAID(this->m_owner->GetMember().accountId);
        if (actor) {
          UIPcGage* gauge = static_cast<UIPcGage*>(static_cast<CPc*>(actor)->gaugeWindow);
          if (gauge) {
            current = gauge->current;
            maximum = gauge->maximum;
          } else {
            current = 0;
            maximum = 0;
          }
        }
      }
      if (actor) {
        this->m_gauge->Move(52, 32);
        this->m_gauge->SetValues(current, maximum);
        break;
      }
      this->m_gauge->Move(-100, -100);
      break;
    }
    this->m_gauge->Move(-100, -100);
    break;
  }
  case UM_MAKEMENU:
    this->m_entries.erase(this->m_entries.begin(), this->m_entries.end());
    if (g_windowMgr.menuWnd) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }
    if (this->m_owner->GetMember().valid) {
      this->m_entries.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_SEND_RODEX)).c_str(), MENUID_WRITE_RODEX));
      if (this->m_owner->GetMember().name == g_session.DecodePlayerName()) {
        this->m_entries.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_LEAVE_PARTY)).c_str(), MENUID_LEAVE));
      } else {
        this->m_entries.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_OPEN_1ON1_WINDOW)).c_str(), MENUID_TALKONLYTWO));
        if (g_session.partyLeader) {
          this->m_entries.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_EXPEL)).c_str(), MENUID_EXPEL));
          this->m_entries.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_YIELD_PARTYMASTER)).c_str(), MENUID_YIELD_PARTYMASTER));
        }
      }
      this->m_entries.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_DELETE_MINI_PARTYWND)).c_str(), MENUID_CLOSE_MINIPARTY));
    }
    if (this->m_entries.size()) {
      UIWindow* menu = g_windowMgr.EnsureWindow(WID_MENUWND);
      menu->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
      for (unsigned index = 0; index < this->m_entries.size(); ++index) {
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->m_entries[index].text.c_str()), 0, 0, 0);
      }
      menu->SendMsg(nullptr, UM_SET_CALLER_WID, WID_MINIPARTYWND, this->m_owner->m_cell, 0, 0);
      menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
      int x = g_mouse.x + 1;
      int y = g_mouse.y + 1;
      if (x < 1) {
        x = 1;
      }
      if (x > g_renderer->viewport.width - menu->geometry.width) {
        x = g_renderer->viewport.width - menu->geometry.width;
      }
      if (y < 1) {
        y = 1;
      }
      if (y > g_renderer->viewport.height - menu->geometry.height) {
        y = g_renderer->viewport.height - menu->geometry.height;
      }
      menu->Move(x, y);
    }
    break;
  case UM_MENU_SELECTED:
    if (!this->m_owner->GetMember().valid) {
      break;
    }
    if (this->m_entries.empty()) {
      g_windowMgr.Confirm(LookupMessage(MSI_GET_OFF_MINI_PARTYWND), 0, false, false, 0, "메시지", 280, 120, 0);
    } else {
      switch (this->m_entries[argument1].value) {
    case MENUID_SEND_MSG:
      this->SendMsg(nullptr, UM_COMMAND, ID_SENDMSG, 0, 0, 0);
      break;
    case MENUID_TALKONLYTWO:
      this->SendMsg(nullptr, UM_COMMAND, ID_TALK, 0, 0, 0);
      break;
    case MENUID_QUERYINFO:
      this->SendMsg(nullptr, UM_COMMAND, ID_FRIEND_INFO, 0, 0, 0);
      break;
    case MENUID_EXPEL:
    case MENUID_DELETE_WHISPER_NAME:
      this->SendMsg(nullptr, UM_COMMAND, ID_DELETE_FRIEND, 0, 0, 0);
      break;
    case MENUID_LEAVE:
      this->SendMsg(nullptr, UM_COMMAND, ID_LEAVE, 0, 0, 0);
      break;
    case MENUID_MAKE_WHISPER_WINDOW:
      this->SendMsg(nullptr, UM_MAKE_WHISPER_WINDOW, 0, 0, 0, 0);
      break;
    case MENUID_INVITE_IN_PARTY:
      if (this->m_owner->m_cell != -1) {
        if (this->m_owner->GetMember().valid) {
          CMode* mode = g_modeMgr.currentMode;
          mode->SendMsg(MM_REQ_JOIN_GROUP, reinterpret_cast<int>(this->m_owner->GetMember().name.c_str()), 0, 0, 0);
        }
      }
      break;
    case MENUID_YIELD_PARTYMASTER:
      if (this->m_owner->m_cell != -1) {
        if (this->m_owner->GetMember().valid && this->m_owner->GetMember().offline == 0) {
          if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_YIELD_PARTYMASTER), 2, false, false, 0, "메시지", 280, 120, 0) == ID_YES) {
            CMode* mode = g_modeMgr.currentMode;
            mode->SendMsg(MM_GROUP_CHANGE_MASTER, this->m_owner->GetMember().accountId, 0, 0, 0);
          }
        } else {
          g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
          const char* message = LookupMessage(MSI_CANNOT_YIELD_PARTYMASTER);
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 255, 3, 0, 0);
        }
      }
      break;
    case MENUID_WRITE_RODEX:
      if (this->m_owner->GetMember().valid) {
        CMode* mode = g_modeMgr.currentMode;
        mode->SendMsg(MM_OPEN_WRITE_RODEX, reinterpret_cast<int>(this->m_owner->GetMember().name.c_str()), 0, 0, 0);
      }
      break;
    case MENUID_CLOSE_MINIPARTY:
      if (this->m_owner->GetMember().valid) {
        g_windowMgr.CloseMiniPartyWindow(this->m_owner->GetMember().accountId);
      }
      break;
      }
    }
    break;
  case UM_COMMAND:
    switch (argument1) {
    case ID_SENDMSG:
      if (!g_session.m_lockMouse) {
        if (!this->m_owner->GetMember().valid) {
          break;
        }
      }
      break;
    case ID_TALK:
      if (!g_session.m_lockMouse) {
        if (this->m_owner->GetMember().valid && !(this->m_owner->GetMember().name == g_session.DecodePlayerName())) {
          const char* group = nullptr;
          if (g_windowMgr.IsFriend(this->m_owner->GetMember().name.c_str())) {
            group = LookupMessage(MSI_FRIEND);
          } else if (g_session.IsGuildMemberCharacter(this->m_owner->GetMember().characterId)) {
            group = "GuildMember";
          }
          g_windowMgr.SendNotice(
            14, reinterpret_cast<std::intptr_t>(this->m_owner->GetMember().name.c_str()), reinterpret_cast<std::intptr_t>(group),
            reinterpret_cast<std::intptr_t>(g_session.ShortcutFilename(this->m_owner->GetMember().characterId).c_str()), 0, 0
          );
        }
      }
      break;
    case ID_FRIEND_INFO:
      if (!g_session.m_lockMouse) {
        if (this->m_owner->GetMember().valid) {
          g_windowMgr.Confirm(LookupMessage(MSI_NOTSUPPORTEDYET), 0, true, false, 0, "메시지", 280, 120, 0);
        }
      }
      break;
    case ID_PARTY_SETTING:
      if (!g_session.m_lockMouse) {
        if (g_windowMgr.partyInvitationToWnd) {
          g_windowMgr.CloseWindow(WID_PARTYINVITATIONTOWND);
        }
        if (!g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT) && g_session.PartyCount()) {
          g_windowMgr.EnsureWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
        }
        g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND);
        UIWindow* window = static_cast<UIWindow*>(g_windowMgr.partySettingWnd);
        if (window) {
          int x = 0;
          int y = 0;
          this->ToScreen(x, y);
          window->Move(x + 2 + this->geometry.width, y);
        }
      }
      break;
    case ID_DELETE_FRIEND:
      if (!g_session.m_lockMouse) {
        if (this->m_owner->GetMember().valid) {
          if (!(this->m_owner->GetMember().name == g_session.DecodePlayerName())) {
            if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_EXPEL), 2, false, false, 0, "메시지", 280, 120, 0) == ID_YES) {
              CMode* mode = g_modeMgr.currentMode;
              mode->SendMsg(MM_REQ_EXPEL_GROUP_MEMBER, reinterpret_cast<int>(this->m_owner->GetMember().name.c_str()), 0, 0, 0);
            }
          }
        }
      }
      break;
    case ID_LEAVE:
      if (!g_session.m_lockMouse) {
        if (!this->m_owner->GetMember().valid || !(this->m_owner->GetMember().name == g_session.DecodePlayerName())) {
          break;
        }
        if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_LEAVE), 2, false, false, 0, "메시지", 280, 120, 0) == ID_YES) {
          g_modeMgr.currentMode->SendMsg(MM_REQ_LEAVE_GROUP, 0, 0, 0, 0);
        }
      }
      break;
    case ID_UPDATE_MEMBER_HP: {
      CWorld* world = g_modeMgr.GetGameMode()->scene;
      CRenderObject* actor;
      if (static_cast<std::uint32_t>(argument2) == g_session.m_aid) {
        actor = world->player;
      } else {
        actor = world->GetGameActorByAID(argument2);
      }
      if (actor) {
        this->m_gauge->Move(52, 32);
        this->m_gauge->SetValues(argument3, argument4);
        this->Invalidate();
        break;
      }
      this->m_gauge->Move(-100, -100);
      this->Invalidate();
      break;
    }
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x00742E10
// FUNCTION: fn_07743
void UIMiniPartyWnd::OnLBtnUp(int x, int y) {
  const int cell = CMiniPartyWndAreaMgr::Instance()->FindCell(this->geometry.x, this->geometry.y);
  if (cell != -1) {
    UIMiniPartyWnd* other = static_cast<UIMiniPartyWnd*>(g_windowMgr.FindSnapExcludedWindow(cell));
    if (other) {
      other->m_cell = this->m_cell;
      g_session.SetRecord0734Value(other->GetMember().accountId, this->m_cell);
      this->m_cell = cell;
      g_session.SetRecord0734Value(this->m_member.accountId, cell);
    } else {
      CMiniPartyWndAreaMgr* area = CMiniPartyWndAreaMgr::Instance();
      if (cell >= 0 && cell < area->rows * area->columns) {
        this->m_cell = cell;
        g_session.SetRecord0734Value(this->m_member.accountId, cell);
      } else {
        g_windowMgr.Confirm(LookupMessage(MSI_CANNOT_MOVE_MINI_PARTYWND), 0, true, false, 0, "메시지", 280, 120, 0);
      }
    }
  }

  UIFrameWnd::OnLBtnUp(x, y);
  g_windowMgr.ArrangeMiniPartyWindows();
}

// OFFSET: 0x00743020
// FUNCTION: fn_46344
void UIMiniPartyWnd::SetSizeMode(int mode) {
  this->m_sizeMode = mode;
  switch (mode) {
  case MINIPARTY_SIZE_WIDE:
    this->Resize(this->m_widths[MINIPARTY_SIZE_WIDE], this->geometry.height);
    if (this->m_infoWnd) {
      this->m_infoWnd->Resize(this->geometry.width, this->geometry.height);
    }
    this->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg.tga";
    this->m_background->Move(this->m_widths[this->m_sizeMode] - 35, 25);
    break;
  case MINIPARTY_SIZE_COMPACT:
    this->Resize(this->m_widths[MINIPARTY_SIZE_COMPACT], this->geometry.height);
    if (this->m_infoWnd) {
      this->m_infoWnd->Resize(this->geometry.width, this->geometry.height);
    }
    this->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg_mini.tga";
    this->m_background->Move(this->m_widths[this->m_sizeMode] - 23, 25);
    break;
  case MINIPARTY_SIZE_MINIMAL:
    this->Resize(this->m_widths[MINIPARTY_SIZE_MINIMAL], this->geometry.height);
    if (this->m_infoWnd) {
      this->m_infoWnd->Resize(this->geometry.width, this->geometry.height);
    }
    this->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg_maxmini.tga";
    this->m_background->Move(this->m_widths[this->m_sizeMode] - 35, 25);
    break;
  }
  this->Invalidate();
}

// OFFSET: 0x00743130
// FUNCTION: fn_07744
int UIMiniPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_REFRESHITEM: {
    const MessengerMember* member = reinterpret_cast<const MessengerMember*>(argument1);
    if (member && member->valid) {
      this->m_member = *member;
      if (this->m_infoWnd) {
        this->m_infoWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
    }
    break;
  }
  case UM_MENU_SELECTED:
    this->m_infoWnd->SendMsg(sender, message, argument1, argument2, argument3, argument4);
    break;
  case UM_COMMAND:
    if (argument1 == 34 && this->m_member.accountId == static_cast<std::uint32_t>(argument2) && this->m_member.offline == 0 && this->m_infoWnd) {
      this->m_infoWnd->SendMsg(sender, message, 34, argument2, argument3, argument4);
      this->InvalidateChildren();
    }
    break;
  case UM_RESIZE: {
    if (argument1 != 7) {
      break;
    }
    const int previous = this->m_sizeMode;
    const int wideWidth = this->m_widths[MINIPARTY_SIZE_WIDE];
    const int normalWidth = this->m_widths[MINIPARTY_SIZE_COMPACT];
    if (argument2 <= this->m_widths[MINIPARTY_SIZE_MINIMAL]) {
      this->m_sizeMode = MINIPARTY_SIZE_MINIMAL;
    } else if (argument2 <= normalWidth) {
      this->m_sizeMode = MINIPARTY_SIZE_COMPACT;
    } else if (argument2 <= wideWidth) {
      this->m_sizeMode = MINIPARTY_SIZE_WIDE;
    } else {
      break;
    }
    if (previous == this->m_sizeMode) {
      break;
    }

    const int count = g_session.PartyCount();
    std::list<SessionRecord0734> records;
    for (int index = 0; index < count; ++index) {
      const MessengerMember member = g_session.PartyAt(index);
      if (!member.valid) {
        continue;
      }
      UIMiniPartyWnd* window = static_cast<UIMiniPartyWnd*>(g_windowMgr.FindItemWindow(member.accountId));
      if (!window) {
        continue;
      }
      window->m_sizeMode = this->m_sizeMode;
      switch (window->m_sizeMode) {
      case MINIPARTY_SIZE_WIDE:
        window->Resize(window->m_widths[MINIPARTY_SIZE_WIDE], window->geometry.height);
        if (window->m_infoWnd) {
          window->m_infoWnd->Resize(window->geometry.width, window->geometry.height);
        }
        window->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg.tga";
        window->m_background->Move(window->m_widths[window->m_sizeMode] - 35, 25);
        break;
      case MINIPARTY_SIZE_COMPACT:
        window->Resize(window->m_widths[MINIPARTY_SIZE_COMPACT], window->geometry.height);
        if (window->m_infoWnd) {
          window->m_infoWnd->Resize(window->geometry.width, window->geometry.height);
        }
        window->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg_mini.tga";
        window->m_background->Move(window->m_widths[window->m_sizeMode] - 23, 25);
        break;
      case MINIPARTY_SIZE_MINIMAL:
        window->Resize(window->m_widths[MINIPARTY_SIZE_MINIMAL], window->geometry.height);
        if (window->m_infoWnd) {
          window->m_infoWnd->Resize(window->geometry.width, window->geometry.height);
        }
        window->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg_maxmini.tga";
        window->m_background->Move(window->m_widths[window->m_sizeMode] - 35, 25);
        break;
      }
      window->Invalidate();
      window->Invalidate();
    }
    return 0;
  }
  case UM_MAKEMENU:
    if (this->m_infoWnd) {
      this->m_infoWnd->SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x007435C0
// FUNCTION: fn_07745
CMiniPartyWndAreaMgr* CMiniPartyWndAreaMgr::Instance() {
  if (!g_miniPartyWndAreaMgr) {
    g_miniPartyWndAreaMgr = new (std::nothrow) CMiniPartyWndAreaMgr;
  }
  return g_miniPartyWndAreaMgr;
}

// OFFSET: 0x00740300
// FUNCTION: fn_07730
void UIMiniPartyInfoWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  std::string directory = "유저인터페이스\\renewalparty\\";
  if (this->m_owner->GetMember().valid == 0) {
    return;
  }

  char path[256];
  const int job = this->m_owner->GetMember().job;
  int dead = 0;
  if (this->m_owner->GetMember().dead == 1) {
    dead = 1;
  } else if (g_session.m_aid == this->m_owner->GetMember().accountId) {
    if (g_modeMgr.GetGameMode()->isPlayerDead == 1) {
      dead = 1;
    }
  }

  const int leader = this->m_owner->GetMember().leader;
  if (dead == 0) {
    std::sprintf(path, "%sicon_jobs_%d.bmp", directory.c_str(), job);
  } else {
    std::sprintf(path, "%sicon_jobs_%d_die.bmp", directory.c_str(), job);
  }
  CRes* icon = ResourceMgr::Instance().Load(Resolve(path));
  int offline = this->m_owner->GetMember().offline;
  const int iconY = leader == 0 ? 8 : 6;
  if (offline == 1) {
    this->DrawAuxiliary(1, 7, iconY, static_cast<CBitmapRes*>(icon), true, false);
  } else {
    this->DrawBitmap(7, iconY, static_cast<CBitmapRes*>(icon), true);
  }

  if (this->m_owner->GetMember().leader == 0) {
    std::sprintf(path, "%sico_partyCrown.bmp", directory.c_str());
    CRes* crown = ResourceMgr::Instance().Load(Resolve(path));
    offline = this->m_owner->GetMember().offline;
    if (offline == 1) {
      this->DrawAuxiliary(1, 9, 0, static_cast<CBitmapRes*>(crown), true, false);
    } else {
      this->DrawBitmap(9, 0, static_cast<CBitmapRes*>(crown), true);
    }
  }

  UIPcGage* gauge = this->m_gauge;
  if (this->m_owner->m_sizeMode == MINIPARTY_SIZE_WIDE) {
    gauge->Move(35, 23);
  } else {
    gauge->Move(-1000, -1000);
  }

  if (this->m_owner->m_sizeMode == MINIPARTY_SIZE_MINIMAL) {
    return;
  }

  char text[256];
  std::memset(text, 0, sizeof(text));
  std::string mapName = g_session.GetMapDisplayName(this->m_owner->GetMember().map.c_str());
  std::string fitted = text;
  if (this->m_owner->m_sizeMode == MINIPARTY_SIZE_COMPACT) {
    std::sprintf(text, "%s", this->m_owner->GetMember().name.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.532f), text, 0, 12, 1);
  } else {
    std::sprintf(text, "Lv.%d %s(%s)", this->m_owner->GetMember().level, this->m_owner->GetMember().name.c_str(), mapName.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.6f), text, 0, 12, 1);
  }

  offline = this->m_owner->GetMember().offline;
  if (offline == 1) {
    this->DrawTextAt(35, 9, fitted.c_str(), 0, 0, 12, 0xAAAAAA, 1, 0);
  } else {
    this->DrawTextAt(35, 9, fitted.c_str(), 0, 0, 12, 0xFFFFFF, 1, 0);
  }
}
