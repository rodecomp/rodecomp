#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <string>

// OFFSET: 0x00CCF480
// FUNCTION: fn_21242
void CGameMode::PushUnknown01A4(int value) {
  this->unknown_01A4.push_back(value);
}

// OFFSET: 0x00CCF530
// FUNCTION: fn_21243
bool CGameMode::IsOverWeight(bool notify) {
  int maxWeight = g_session.m_maxWeight;
  if (maxWeight < 1) {
    maxWeight = 1;
  }

  if (g_session.m_weight * 100 / maxWeight < 90) {
    return false;
  }

  if (notify) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(307)), 255, 0, 0, 0);
  }

  return true;
}

// OFFSET: 0x00CCFCF0
// FUNCTION: fn_21247
void CGameMode::CompleteUnknown0194(std::uint32_t id, const std::string& text) {
  auto it = this->unknown_0194.find(id);
  if (it != this->unknown_0194.end()) {
    this->SendMsg(140, reinterpret_cast<int>(text.c_str()), it->second, 0, 0);
    this->unknown_0194.erase(it);
  }
}

// OFFSET: 0x00CED190
// FUNCTION: fn_21363
void CGameMode::UpdateUnknown0628(int id, bool add) {
  if (add == true) {
    this->unknown_0628.insert(id);
  } else {
    this->unknown_0628.erase(id);
  }
}
