#include "UI/Window/UISeekPartyWnd.h"

#include "UI/Window/UIJobListWnd.h"

#include <utility>

// OFFSET: 0x00944B20
// FUNCTION: fn_12833
void UISeekPartyWnd::SetJobs(const std::int16_t* jobs) {
  g_partyBookingJobs.clear();
  for (int index = 0; index < 6; ++index) {
    if (jobs[index] < 0) {
      break;
    }
    g_partyBookingJobs.insert(std::pair<const int, bool>(jobs[index], true));
  }
}
