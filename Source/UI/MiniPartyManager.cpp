#include "Client/Session.h"
#include "Contents/ClientItemLists.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "UI/MiniPartyGrid.h"
#include "UI/MiniPartyPlacement.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"
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
  this->unknown_047C = width;
  this->unknown_0480 = height;
  this->unknown_0030 = 0;
  this->unknown_0034 = height - this->unknown_0038;
  reinterpret_cast<MiniPartyGrid*>(CMiniPartyWndAreaMgr::Instance())->BuildAvailableCells();
}

// OFFSET: 0x00AA2510
// FUNCTION: fn_17214
void UIWindowMgr::ArrangeMiniPartyWindows() {
  for (UIWindow* window : this->snapExcluded) {
    MiniPartyGridPoint position;
    reinterpret_cast<MiniPartyGrid*>(CMiniPartyWndAreaMgr::Instance())
      ->GetCellPosition(&position, *reinterpret_cast<int*>(reinterpret_cast<char*>(window) + 0x104));
    window->Move(position.x, position.y);
  }
}

// OFFSET: 0x00AAAB20
// FUNCTION: fn_17270
void UIWindowMgr::RearrangeMiniPartyWindows() {
  const int height = this->unknown_0480;
  int top = static_cast<int>(static_cast<float>(height) * 0.3f);
  if (UIWindow* window = g_windowMgr.LookupWindow(0x133)) {
    top = window->geometry.height;
  }

  RECT bounds;
  bounds.left = 0;
  bounds.top = top;
  bounds.right = this->unknown_047C;
  bounds.bottom = height - g_miniPartyCellHeight * 3;
  CMiniPartyWndSortArea area(bounds);
  const std::list<int> cells = area.GetCells();

  auto cell = cells.begin();
  auto entry = this->snapExcluded.begin();
  for (; cell != cells.end() && entry != this->snapExcluded.end(); ++cell, ++entry) {
    MiniPartyGridPoint position;
    reinterpret_cast<const MiniPartyGrid*>(&area)->GetWindowPosition(&position, *cell);
    UIMiniPartyWnd* window = static_cast<UIMiniPartyWnd*>(*entry);
    window->Move(position.x, position.y);
    window->m_cell = *cell;
    const int index = *cell;
    const MessengerMember member = window->m_member;
    g_session.SetRecord0734Value(member.accountId, index);
  }
}

// OFFSET: 0x00A8AF20
// FUNCTION: fn_17119
void UIWindowMgr::AddMiniPartyWindow(UIWindow* window) {
  this->rendered.push_back(window);
  this->snapExcluded.push_back(window);
  this->ArrangeMiniPartyWindows();
}

// OFFSET: 0x00A96090
// FUNCTION: fn_17183
void UIWindowMgr::RebuildMiniPartyWindows() {
  for (auto entry = this->snapExcluded.begin(); entry != this->snapExcluded.end();) {
    UIWindow* window = *entry;
    if (window == this->window_03D4) {
      this->window_03D4 = nullptr;
    }

    if (window == this->window_03D8) {
      this->window_03D8 = nullptr;
    }

    if (window == this->window_03DC) {
      this->window_03DC = nullptr;
    }

    if (window == this->window_01B8) {
      this->window_01B8 = nullptr;
    }

    if (window == this->window_4F2AC) {
      this->window_4F2AC = nullptr;
    }

    this->retired.push_back(window);
    entry = this->snapExcluded.erase(entry);
  }
  this->snapExcluded.clear();

  const int count = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount();
  std::list<SessionRecord0734> records;
  for (int index = 0; index < count; ++index) {
    MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyAt(index);
    if (member.valid == 0) {
      continue;
    }

    const SessionRecord0734 record = g_session.FindRecord0734(member.accountId);
    if (record.unknown_0000 == 0) {
      continue;
    }

    records.push_back(record);
  }

  reinterpret_cast<ClientItemLists&>(g_session).ClearRecords0734();

  WindowDialogs& dialogs = *reinterpret_cast<WindowDialogs*>(this);
  for (const SessionRecord0734& record : records) {
    MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyByAccount(record.id);
    if (member.valid == 0) {
      continue;
    }

    UIMiniPartyWnd* window = new UIMiniPartyWnd(dialogs.windows, dialogs.frames);
    window->SetSize(174, 52);
    window->m_cell = record.unknown_0008;
    this->AddMiniPartyWindow(window);
    window->SendMsg(nullptr, 23, reinterpret_cast<std::intptr_t>(&member), 0, 0, 0);
    g_session.AddRecord0734(record.id, record.unknown_0008);

    UIWindow* drag = this->unknown_0210;
    if (drag != nullptr && *reinterpret_cast<int*>(reinterpret_cast<char*>(drag) + 0xB4) == 301 &&
        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<char*>(drag) + 0xCC) == record.unknown_0008) {
      const int x = drag->geometry.x;
      const int y = drag->geometry.y;
      window->SendMsg(nullptr, 49, 0, 0, 0, 0);
      this->unknown_0210->Move(x, y);
    }
  }

  WindowOrdering ordering{this->rendered, reinterpret_cast<WindowManagerWindows&>(*this), reinterpret_cast<const int&>(this->modal), this->window_4F174};
  if (this->Window4F250Ready()) {
    ordering.Promote(this->window_4F250);
  }

  ordering.Promote(this->unknown_0210);
}
