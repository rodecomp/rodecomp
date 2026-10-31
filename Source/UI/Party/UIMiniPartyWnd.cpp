#include "UI/Party/UIMiniPartyWnd.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Contents/MapInfoMgr.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/MiniPartyGrid.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"

#include <windows.h>
#include <timeapi.h>

#include <cstdio>
#include <cstring>
#include <string>

// OFFSET: 0x007426F0
// FUNCTION: fn_07737
UIMiniPartyWnd::UIMiniPartyWnd(UIWindowContext& windowContext, UIFrameContext& frameContext)
    : UIFrameWnd(windowContext, frameContext, 1), m_cell(-1), m_infoWnd(nullptr), m_background(nullptr), m_sizeMode(0) {
  this->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg.tga";
  this->geometry.id = 301;
}

// OFFSET: 0x00742AA0
// FUNCTION: fn_07739
void UIMiniPartyWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y;
}

// OFFSET: 0x00742AC0
// FUNCTION: fn_07740
void UIMiniPartyWnd::Slot15(int, int) {
  UIMiniPartyInfoWnd* info = new UIMiniPartyInfoWnd(*this->m_context, *this->m_frameContext);
  this->m_infoWnd = info;
  info->m_owner = this;
  info->SetSize(174, 52);
  info->Move(0, 0);
  this->AddChild(info);
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
    this->DrawNamed(this->m_context->primitives, 0, 0, this->m_imageName, 0xFFFFFFFF, 0, 0);
  }
}

// OFFSET: 0x00742D80
// FUNCTION: fn_07742
void UIMiniPartyWnd::Slot25(int x, int y) {
  if (g_dragWindowId != this->geometry.id) {
    g_windowMgr.BringToFront(this->geometry.id);
    g_dragWindowId = this->geometry.id;
  }

  g_windowMgr.SetCapture(this);
  this->ToScreen(x, y);
  this->m_dragX = x;
  this->m_dragY = y;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  this->geometry.alphaStart = timeGetTime();
  this->m_frameContext->snapping.graph.CaptureOrigins(this);
}

// OFFSET: 0x00740B80
// FUNCTION: fn_07731
void UIMiniPartyInfoWnd::Slot25(int x, int y) {
  if (!g_messengerGroupCollapsed) {
    this->m_owner->Slot25(x, y);
  }

  if (this->m_owner->GetMember().valid && this->m_owner->GetMember().offline == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode) {
      mode->TargetActorByAid(this->m_owner->GetMember().accountId);
    }
  }
}

// OFFSET: 0x00740D10
// FUNCTION: fn_07732
void UIMiniPartyInfoWnd::Slot31(int x, int y) {
  if (g_messengerGroupCollapsed != true && this == g_windowMgr.Capture()) {
    this->m_owner->Slot31(x, y);
  }
}

// OFFSET: 0x00740D40
// FUNCTION: fn_07733
void UIMiniPartyInfoWnd::Slot28(int, int) {
  ModeTooltip* mode = reinterpret_cast<ModeTooltip*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  if (this->m_owner->GetMember().valid == 0) {
    return;
  }

  char text[256];
  std::memset(text, 0, sizeof(text));
  std::string fitted;
  const std::string mapName = g_mapInfoMgr->GetDisplayName(std::string(this->m_owner->GetMember().map.c_str()));
  if (this->m_owner->m_sizeMode == 1) {
    std::sprintf(text, "%s", this->m_owner->GetMember().name.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.532f), text, 0, 12, 1);
  } else {
    std::sprintf(text, "Lv.%d %s(%s)", this->m_owner->GetMember().level, this->m_owner->GetMember().name.c_str(), mapName.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.6f), text, 0, 12, 1);
  }

  if (fitted.find(text) == std::string::npos) {
    int x = this->geometry.x + 15;
    int y = this->geometry.y - 12;
    this->ToScreen(x, y);
    mode->Show(text, x, y, 0xFFFFFFFF, 0, 1);
  }
}

// OFFSET: 0x00741280
// FUNCTION: fn_07734
void UIMiniPartyInfoWnd::Slot33(int, int) {
  if (!g_messengerGroupCollapsed) {
    this->SendMsg(nullptr, 0x31, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00742E10
// FUNCTION: fn_07743
void UIMiniPartyWnd::Slot31(int x, int y) {
  const int cell = reinterpret_cast<MiniPartyGrid*>(CMiniPartyWndAreaMgr::Instance())->FindCell(this->geometry.x, this->geometry.y);
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
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(3145), 0, true, false, 0, "Message", 280, 120, 0);
      }
    }
  }

  UIFrameWnd::Slot31(x, y);
  g_windowMgr.ArrangeMiniPartyWindows();
}

// OFFSET: 0x00743130
// FUNCTION: fn_07744
int UIMiniPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    if (argument1 == 34 && this->m_member.accountId == static_cast<std::uint32_t>(argument2) && this->m_member.offline == 0 && this->m_infoWnd) {
      this->m_infoWnd->SendMsg(sender, message, 34, argument2, argument3, argument4);
      this->InvalidateChildren();
    }
    return 0;
  case 14: {
    if (argument1 != 7) {
      return 0;
    }
    const int previous = this->m_sizeMode;
    if (argument2 <= this->m_widths[2]) {
      this->m_sizeMode = 2;
    } else if (argument2 <= this->m_widths[1]) {
      this->m_sizeMode = 1;
    } else if (argument2 <= this->m_widths[0]) {
      this->m_sizeMode = 0;
    } else {
      return 0;
    }
    if (previous == this->m_sizeMode) {
      return 0;
    }

    ClientMessenger messenger{g_session.m_partyMembers, g_session.m_friends};
    const int count = messenger.PartyCount();
    for (int index = 0; index < count; ++index) {
      const MessengerMember member = messenger.PartyAt(index);
      if (!member.valid) {
        continue;
      }
      UIMiniPartyWnd* window = static_cast<UIMiniPartyWnd*>(g_windowMgr.FindItemWindow(member.accountId));
      if (!window) {
        continue;
      }
      window->m_sizeMode = this->m_sizeMode;
      switch (window->m_sizeMode) {
      case 0:
        window->Resize(window->m_widths[0], window->geometry.height);
        if (window->m_infoWnd) {
          window->m_infoWnd->Resize(window->geometry.width, window->geometry.height);
        }
        window->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg.tga";
        window->m_background->Move(window->m_widths[window->m_sizeMode] - 35, 25);
        break;
      case 1:
        window->Resize(window->m_widths[1], window->geometry.height);
        if (window->m_infoWnd) {
          window->m_infoWnd->Resize(window->geometry.width, window->geometry.height);
        }
        window->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg_mini.tga";
        window->m_background->Move(window->m_widths[window->m_sizeMode] - 23, 25);
        break;
      case 2:
        window->Resize(window->m_widths[2], window->geometry.height);
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
  case 23: {
    const MessengerMember* member = reinterpret_cast<const MessengerMember*>(argument1);
    if (member && member->valid) {
      this->m_member = *member;
      if (this->m_infoWnd) {
        this->m_infoWnd->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
    }
    return 0;
  }
  case 48:
    this->m_infoWnd->SendMsg(sender, message, argument1, argument2, argument3, argument4);
    return 0;
  case 49:
    if (this->m_infoWnd) {
      this->m_infoWnd->SendMsg(sender, message, argument1, argument2, argument3, argument4);
    }
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
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
    dead = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0250 == 1;
  }

  const int leader = this->m_owner->GetMember().leader;
  std::sprintf(path, dead ? "%sicon_jobs_%d_die.bmp" : "%sicon_jobs_%d.bmp", directory.c_str(), job);
  CRes* icon = ResourceMgr::Instance().Load(Resolve(path));
  if (this->m_owner->GetMember().offline == 1) {
    this->DrawAuxiliary(1, 7, leader == 0 ? 8 : 6, reinterpret_cast<BitmapImage*>(icon), true, false);
  } else {
    this->DrawBitmap(7, leader == 0 ? 8 : 6, reinterpret_cast<const BitmapImage*>(icon), true);
  }

  if (this->m_owner->GetMember().leader == 0) {
    std::sprintf(path, "%sico_partyCrown.bmp", directory.c_str());
    CRes* crown = ResourceMgr::Instance().Load(Resolve(path));
    if (this->m_owner->GetMember().offline == 1) {
      this->DrawAuxiliary(1, 9, 0, reinterpret_cast<BitmapImage*>(crown), true, false);
    } else {
      this->DrawBitmap(9, 0, reinterpret_cast<const BitmapImage*>(crown), true);
    }
  }

  if (this->m_owner->m_sizeMode == 0) {
    this->m_gauge->Move(35, 23);
  } else {
    this->m_gauge->Move(-1000, -1000);
  }

  if (this->m_owner->m_sizeMode == 2) {
    return;
  }

  char text[256];
  std::memset(text, 0, sizeof(text));
  std::string mapName = g_session.GetMapDisplayName(this->m_owner->GetMember().map.c_str());
  std::string fitted = text;
  if (this->m_owner->m_sizeMode == 1) {
    std::sprintf(text, "%s", this->m_owner->GetMember().name.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.532f), text, 0, 12, 1);
  } else {
    std::sprintf(text, "Lv.%d %s(%s)", this->m_owner->GetMember().level, this->m_owner->GetMember().name.c_str(), mapName.c_str());
    fitted = this->FitDisplayText(static_cast<int>(static_cast<float>(this->geometry.width) * 0.6f), text, 0, 12, 1);
  }

  this->DrawTextAt(
    this->m_context->fonts, this->m_context->locale, 35, 9, fitted.c_str(), 0, 0, 12, this->m_owner->GetMember().offline == 1 ? 0xAAAAAA : 0xFFFFFF, 1, 0
  );
}
