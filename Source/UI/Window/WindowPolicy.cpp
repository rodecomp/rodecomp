#include "Client/Replay/Replay.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowPolicy.h"

// OFFSET: 0x00A970E0
// FUNCTION: fn_17200
int UIWindowMgr::WindowPolicy(int action, unsigned int id) const {
  if (action == 3) {
    if (g_replayAnimationScaling == 0) {
      return 1;
    }
  } else {
    if (action >= 8) {
      return 0;
    }
    if (action < 0) {
      return 0;
    }
  }

  if (id <= 10040) {
    return this->policy[id].actions[action];
  }

  return 0;
}

// INLINED
int WindowPolicy(const int* table, bool restricted, unsigned action, unsigned id) {
  return g_windowMgr.WindowPolicy(static_cast<int>(action), id);
}
