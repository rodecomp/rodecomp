#include "UI/UIRequestJoinPartyMgr.h"
#include "UI/UIWindow.h"
#include "UI/WindowManagerWindows.h"

#include <algorithm>
#include <cstdio>
#include <new>

// STATIC: 0x01450CEC
// GLOBAL: gl_24363
UIRequestJoinPartyMgr* g_requestJoinPartyMgr;

// OFFSET: 0x007BAF90
// FUNCTION: fn_09279
UIRequestJoinPartyMgr::UIRequestJoinPartyMgr(WindowManagerWindows& windows) : m_enabled(true), m_windows(&windows) {
}

// OFFSET: 0x007BB470
// OFFSET: 0x007BB880
UIRequestJoinPartyMgr::~UIRequestJoinPartyMgr() = default;

// OFFSET: 0x007BBD50
// FUNCTION: fn_09291
bool UIRequestJoinPartyMgr::Initialize(WindowManagerWindows& windows) {
  if (g_requestJoinPartyMgr) {
    return false;
  }

  g_requestJoinPartyMgr = new (std::nothrow) UIRequestJoinPartyMgr(windows);

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
void UIRequestJoinPartyMgr::Add(
  std::uint32_t accountId,
  std::uint32_t characterId,
  const char* name,
  std::int16_t job,
  std::int16_t level
) {
  if (std::find(this->m_blocked.begin(), this->m_blocked.end(), accountId) != this->m_blocked.end()) {
    return;
  }

  PartyJoinRequest request;
  request.accountId = accountId;
  request.characterId = characterId;
  _snprintf_s(request.name, sizeof(request.name), _TRUNCATE, "%s", name);
  request.job = job;
  request.level = level;

  if (this->m_windows->LookupWindow(326)) {
    this->m_pending.push_back(request);
  } else {
    UIWindow* window = this->m_windows->EnsureWindow(326);
    window->SendMsg(nullptr, 56, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
  }
}

// OFFSET: 0x007C1770
// FUNCTION: fn_09315
void UIRequestJoinPartyMgr::Block(std::uint32_t accountId) {
  this->m_blocked.push_back(accountId);
}

// OFFSET: 0x007BC110
// FUNCTION: fn_09297
void UIRequestJoinPartyMgr::Advance() {
  if (this->m_pending.empty()) {
    this->m_windows->CloseWindow(326);
  }

  while (!this->m_pending.empty()) {
    UIWindow* window = this->m_windows->LookupWindow(326);

    if (!window) {
      return;
    }

    const PartyJoinRequest request = this->m_pending.front();
    const bool blocked =
      std::find(this->m_blocked.begin(), this->m_blocked.end(), request.accountId) != this->m_blocked.end();

    if (!blocked) {
      // Dispatch precedes removal: reentry observes the original queue head.
      window->SendMsg(nullptr, 56, reinterpret_cast<std::intptr_t>(&request), 0, 0, 0);
      this->m_pending.pop_front();
      return;
    }

    this->m_pending.pop_front();

    if (this->m_pending.empty()) {
      this->m_windows->CloseWindow(326);
    }
  }
}
