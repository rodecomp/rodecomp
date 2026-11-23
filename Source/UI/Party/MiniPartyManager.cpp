#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Messenger/MessengerWindows.h"
#include "UI/Party/MiniPartyGrid.h"
#include "UI/Party/MiniPartyPlacement.h"
#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Login/UIChangeNameWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Login/UISelCharForUServerWnd.h"
#include "UI/Login/UIWaitDBSaveWnd.h"
#include <new>

namespace {
// INLINED
inline CMiniPartyWndAreaMgr* MiniPartyArea() {
  if (!g_miniPartyWndAreaMgr) {
    g_miniPartyWndAreaMgr = new (std::nothrow) CMiniPartyWndAreaMgr;
  }
  return g_miniPartyWndAreaMgr;
}
}

// OFFSET: 0x00AA9ED0
// FUNCTION: fn_17263
void UIWindowMgr::SetScreenSize(int width, int height) {
  this->screenWidth = width;
  this->screenHeight = height;
  this->chatWndX = 0;
  this->chatWndY = height - this->chatWndHeight;
  MiniPartyArea()->BuildAvailableCells();
}

// OFFSET: 0x00AA2510
// FUNCTION: fn_17214
void UIWindowMgr::ArrangeMiniPartyWindows() {
  for (UIWindow* window : this->snapExcluded) {
    const MiniPartyGridPoint position = MiniPartyArea()
      ->GetCellPosition(static_cast<UIMiniPartyWnd*>(window)->m_cell);
    window->Move(position.x, position.y);
  }
}

// OFFSET: 0x00AAAB20
// FUNCTION: fn_17270
void UIWindowMgr::RearrangeMiniPartyWindows() {
  const int height = this->screenHeight;
  int top = static_cast<int>(static_cast<float>(height) * 0.3f);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
    top = window->geometry.height;
  }

  const int bottom = height - g_miniPartyCellHeight * 3;
  CMiniPartyWndSortArea area(RECT{0, top, this->screenWidth, bottom});
  const std::list<int> cells = area.GetCells();

  auto cell = cells.begin();
  auto entry = this->snapExcluded.begin();
  for (; cell != cells.end() && entry != this->snapExcluded.end(); ++cell, ++entry) {
    const MiniPartyGridPoint position = area.GetWindowPosition(*cell);
    (*entry)->Move(position.x, position.y);
    UIMiniPartyWnd* window = static_cast<UIMiniPartyWnd*>(*entry);
    window->m_cell = *cell;

    const int index = *cell;
    g_session.SetRecord0734Value(static_cast<UIMiniPartyWnd*>(*entry)->GetMember().accountId, index);
  }
}

// OFFSET: 0x00A8AF20
// FUNCTION: fn_17119
void UIWindowMgr::AddMiniPartyWindow(UIWindow* window) {
  UIWindow* added = window;
  this->rendered.push_back(added);
  this->snapExcluded.emplace_back(window);
  this->ArrangeMiniPartyWindows();
}

namespace {
// INLINED
__forceinline void PromoteWindow(UIWindowMgr* manager, UIWindow* window) {
  if (!window || (manager->modal != nullptr && manager->navigationWnd != window)) {
    return;
  }

  if (manager->LookupWindow(WID_NEWMAKECHARWND) || manager->LookupWindow(WID_MAKECHARWND) || manager->LookupWindow(WID_SECONDPASSWORDINPUTWND) || manager->LookupWindow(WID_SECONDPASSWORDEDITWND)) {
    return;
  }

  int companion = -1;

  if (window->geometry.id == 12) {
    companion = 234;
  }
  if (window->geometry.id == 82) {
    companion = 250;
  }

  if (companion != -1) {
    for (auto it = manager->rendered.begin(); it != manager->rendered.end(); ++it) {
      if ((*it)->geometry.id == companion) {
        UIWindow* value = *it;
        manager->rendered.erase(it);
        manager->rendered.push_back(value);
        break;
      }
    }
  }

  const auto entry = std::find(manager->rendered.begin(), manager->rendered.end(), window);

  if (entry != manager->rendered.end()) {
    manager->rendered.erase(entry);
    manager->rendered.push_back(window);
  }
}
// INLINED
inline int TransientWindowReady(UIWindowMgr* manager) {
  UIWindow* window = manager->transientWnd;
  if (window == nullptr) {
    return false;
  }
  if (static_cast<UIDragMiniPartyWnd*>(window)->member.valid == 0) {
    return false;
  }
  if (window->geometry.state != 0) {
    return false;
  }
  return true;
}
}

// OFFSET: 0x00A96090
// FUNCTION: fn_17183
void UIWindowMgr::RebuildMiniPartyWindows() {
  for (auto entry = this->snapExcluded.begin(); entry != this->snapExcluded.end();) {
    UIWindow* window = *entry;
    if (window == this->newSelectCharWnd) {
      this->newSelectCharWnd = nullptr;
    }

    if (window == this->selCharForUServerWnd) {
      this->selCharForUServerWnd = nullptr;
    }

    if (window == this->changeNameWnd) {
      this->changeNameWnd = nullptr;
    }

    if (window == this->previousMouseWnd) {
      this->previousMouseWnd = nullptr;
    }

    if (window == this->waitDBSaveWnd) {
      this->waitDBSaveWnd = nullptr;
    }

    this->retired.push_back(window);
    entry = this->snapExcluded.erase(entry);
  }
  this->snapExcluded.clear();

  const int count = g_session.PartyCount();
  std::list<SessionRecord0734> records;
  for (int index = 0; index < count; ++index) {
    MessengerMember member = g_session.PartyAt(index);
    if (member.valid == 0) {
      continue;
    }

    const SessionRecord0734 record = g_session.FindMiniPartyRecord(member.accountId);
    if (record.active == 0) {
      continue;
    }

    records.push_back(record);
  }

  g_session.ClearMiniPartyRecords();

  for (auto recordIt = records.begin(); recordIt != records.end(); ++recordIt) {
    const SessionRecord0734& record = *recordIt;
    MessengerMember member = g_session.PartyByAccount(record.id);
    if (member.valid == 0) {
      continue;
    }

    UIMiniPartyWnd* window = new UIMiniPartyWnd();
    window->SetSize(174, 52);
    window->m_cell = record.cell;
    this->AddMiniPartyWindow(window);
    window->SendMsg(nullptr, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&member), 0, 0, 0);
    g_session.AddMiniPartyRecord(record.id, record.cell);

    UIMenuWnd* drag = this->menuWnd;
    if (drag != nullptr) {
      if (drag->m_menuId != 301) {
        continue;
      }
      if (drag->m_targetWindowId == record.cell) {
        const int x = drag->geometry.x;
        const int y = drag->geometry.y;
        window->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
        this->menuWnd->Move(x, y);
      }
    }
  }

  if (TransientWindowReady(this)) {
    PromoteWindow(this, this->transientWnd);
  }

  PromoteWindow(this, this->menuWnd);
}
