#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "UI/Party/MiniPartyGrid.h"
#include "UI/Party/MiniPartyPlacement.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include <new>

// OFFSET: 0x007435C0
// FUNCTION: fn_07745
CMiniPartyWndAreaMgr* CMiniPartyWndAreaMgr::Instance() {
  if (!g_miniPartyWndAreaMgr) {
    g_miniPartyWndAreaMgr = new (std::nothrow) CMiniPartyWndAreaMgr;
  }
  return g_miniPartyWndAreaMgr;
}

// OFFSET: 0x00AA9ED0
// FUNCTION: fn_17263
void UIWindowMgr::SetScreenSize(int width, int height) {
  this->screenWidth = width;
  this->screenHeight = height;
  this->chatWndX = 0;
  this->chatWndY = height - this->chatWndHeight;
  reinterpret_cast<MiniPartyGrid*>(CMiniPartyWndAreaMgr::Instance())->BuildAvailableCells();
}

// OFFSET: 0x00AA2510
// FUNCTION: fn_17214
void UIWindowMgr::ArrangeMiniPartyWindows() {
  for (UIWindow* window : this->snapExcluded) {
    const MiniPartyGridPoint position = reinterpret_cast<MiniPartyGrid*>(CMiniPartyWndAreaMgr::Instance())
      ->GetCellPosition(*reinterpret_cast<int*>(reinterpret_cast<char*>(window) + 0x104));
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

  RECT bounds;
  bounds.left = 0;
  bounds.top = top;
  bounds.right = this->screenWidth;
  bounds.bottom = height - g_miniPartyCellHeight * 3;
  CMiniPartyWndSortArea area(bounds);
  const std::list<int> cells = area.GetCells();

  auto cell = cells.begin();
  auto entry = this->snapExcluded.begin();
  for (; cell != cells.end() && entry != this->snapExcluded.end(); ++cell, ++entry) {
    const MiniPartyGridPoint position = reinterpret_cast<const MiniPartyGrid*>(&area)->GetWindowPosition(*cell);
    (*entry)->Move(position.x, position.y);
    static_cast<UIMiniPartyWnd*>(*entry)->m_cell = *cell;

    const int index = *cell;
    const MessengerMember member = static_cast<UIMiniPartyWnd*>(*entry)->m_member;
    g_session.SetRecord0734Value(member.accountId, index);
  }
}

// OFFSET: 0x00A8AF20
// FUNCTION: fn_17119
void UIWindowMgr::AddMiniPartyWindow(UIWindow* window) {
  this->rendered.push_back(window);
  this->snapExcluded.emplace_back(window);
  this->ArrangeMiniPartyWindows();
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

  for (const SessionRecord0734& record : records) {
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

    UIWindow* drag = this->menuWnd;
    if (drag != nullptr && *reinterpret_cast<int*>(reinterpret_cast<char*>(drag) + 0xB4) == 301 &&
        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<char*>(drag) + 0xCC) == record.cell) {
      const int x = drag->geometry.x;
      const int y = drag->geometry.y;
      window->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
      this->menuWnd->Move(x, y);
    }
  }

  if (this->Window4F250Ready()) {
    this->Promote(this->transientWnd);
  }

  this->Promote(this->menuWnd);
}
