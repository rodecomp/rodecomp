#include "Client/Session.h"
#include "Contents/Messenger/MessengerMember.h"
#include "UI/MiniPartyGrid.h"
#include "UI/MiniPartyPlacement.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
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
