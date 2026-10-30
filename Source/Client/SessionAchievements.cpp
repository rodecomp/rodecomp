#include "Client/Session.h"
#include "Contents/AchievementMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <vector>

// OFFSET: 0x00E064C0
// FUNCTION: fn_23736
int CSession::AddUnknown6160(int id) {
  int value = g_achievementMgr->GetUnknown0050(id);
  if (value == -1) {
    return -1;
  }

  for (std::vector<int>::iterator it = this->unknown_6160.begin(); it != this->unknown_6160.end(); ++it) {
    if (*it == value) {
      return -1;
    }
  }

  this->unknown_6160.push_back(value);
  std::sort(this->unknown_6160.begin(), this->unknown_6160.end());

  if (UIWindow* window = g_windowMgr.LookupWindow(10)) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }

  return value;
}
