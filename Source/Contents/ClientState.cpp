#include "Contents/ClientState.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

// GLOBAL: gl_24492
bool g_homunculusAutoFeed;

// GLOBAL: gl_24493
bool g_petAutoFeed;

// OFFSET: 0x00E488C0
// FUNCTION: fn_24221
void CSession::UpdateBodyStyle(int job, int style, int renderJob) {
  if (style >= 2 || this->IsSecondJob(renderJob)) {
    this->m_bodyStyle = style;
  } else if (static_cast<unsigned>(job - 4001) <= 21) {
    this->m_bodyStyle = job <= 4007;
  } else {
    this->m_bodyStyle = static_cast<unsigned>(job - 4001) <= 6;
  }
}

// OFFSET: 0x00E48930
// FUNCTION: fn_24222
void CSession::SetHomunculusAutoFeed(bool enabled) {
  int message;
  if (enabled == true) {
    this->m_homunculusAutoFeed = true;
    message = 3282;
  } else {
    g_homunculusAutoFeed = false;
    message = 3283;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00E48980
// FUNCTION: fn_24223
void CSession::SetPetAutoFeed(bool enabled) {
  g_petAutoFeed = enabled == true;
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(enabled ? 2579 : 2580)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00E48B00
// FUNCTION: fn_24225
void CSession::CollectCardItems() {
  this->m_cardItems.clear();
  for (const ITEM_INFO& item : this->m_inventoryItems) {
    if (item.unknown_0000[0] == 6) {
      this->m_cardItems.push_back(item);
    }
  }
}

// OFFSET: 0x00E48B70
// FUNCTION: fn_24226
void RequestBasicInfoRefresh() {
  if (g_windowMgr.unknown_01E0 != nullptr) {
    g_windowMgr.unknown_01E0->SendMsg(nullptr, 106, 575, 0, 0, 0);
  }
}
