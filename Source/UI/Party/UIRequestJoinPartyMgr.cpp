#include "UI/Party/UIRequestJoinPartyMgr.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdio>
#include <new>

// STATIC: 0x01450CEC
// GLOBAL: gl_24363
UIRequestJoinPartyMgr* g_requestJoinPartyMgr;

// OFFSET: 0x007BAF90
// FUNCTION: fn_09279
UIRequestJoinPartyMgr::UIRequestJoinPartyMgr() {
  this->m_enabled = true;
}

// OFFSET: 0x007BB470
// FUNCTION: fn_09281
UIRequestJoinPartyMgr::~UIRequestJoinPartyMgr() = default;

// OFFSET: 0x007BBD50
// FUNCTION: fn_09291
bool UIRequestJoinPartyMgr::Initialize() {
  if (g_requestJoinPartyMgr) {
    return false;
  }

  g_requestJoinPartyMgr = new (std::nothrow) UIRequestJoinPartyMgr();

  if (!g_requestJoinPartyMgr) {
    return false;
  }

  if (!g_requestJoinPartyMgr->m_enabled) {
    delete g_requestJoinPartyMgr;
    g_requestJoinPartyMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x007BBE50
// FUNCTION: fn_09292
void UIRequestJoinPartyMgr::Shutdown() {
  if (g_requestJoinPartyMgr) {
    delete g_requestJoinPartyMgr;
    g_requestJoinPartyMgr = nullptr;
  }
}

// OFFSET: 0x007C00F0
// FUNCTION: fn_09311
void UIRequestJoinPartyMgr::Add(std::uint32_t accountId, std::uint32_t characterId, const char* name, std::int16_t job, std::int16_t level) {
  if (std::find(this->m_blocked.begin(), this->m_blocked.end(), accountId) != this->m_blocked.end()) {
    return;
  }

  PartyJoinRequest request;
  request.accountId = accountId;
  request.characterId = characterId;
  StringFormat(request.name, sizeof(request.name), _TRUNCATE, "%s", name);
  request.job = job;
  request.level = level;

  if (!g_windowMgr.LookupWindow(WID_REQUESTJOINPARTYWND)) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_REQUESTJOINPARTYWND);
    window->SendMsg(nullptr, UM_SET_JOIN_PARTY_MSG, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
  } else {
    this->m_pending.push_back(request);
  }
}

// OFFSET: 0x007BC110
// FUNCTION: fn_09297
void UIRequestJoinPartyMgr::Advance() {
  if (this->m_pending.empty()) {
    g_windowMgr.CloseWindow(WID_REQUESTJOINPARTYWND);
  }

  const std::list<PartyJoinRequest>::iterator first = this->m_pending.begin();
  if (first == this->m_pending.end()) {
    return;
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_REQUESTJOINPARTYWND);
  if (!window) {
    return;
  }

  PartyJoinRequest request = *first;
  if (std::find(this->m_blocked.begin(), this->m_blocked.end(), request.accountId) != this->m_blocked.end()) {
    this->m_pending.pop_front();
    this->Advance();
    return;
  }

  window->SendMsg(nullptr, UM_SET_JOIN_PARTY_MSG, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
  this->m_pending.pop_front();
}
