#include "Client/Session.h"

#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00DE3C10
// FUNCTION: fn_23486
int CSession::GetRobeAlpha(int robeId) {
  const RobeAlpha& alpha = this->m_robeAlpha.find(robeId)->second;
  const int to = alpha.to;
  const int period = alpha.period;
  const int range = std::abs(to - alpha.from);
  if (range == 0) {
    return to;
  }

  const int base = to < alpha.from ? to : alpha.from;
  const int half = static_cast<int>(static_cast<float>(period) * 0.5f);
  int phase = timeGetTime() % period;
  if (phase > half) {
    phase = period - phase;
  }

  const int value = static_cast<int>(static_cast<float>(phase) / static_cast<float>(half) * static_cast<float>(range) + static_cast<float>(base));
  int result = 0;
  if (value >= 0) {
    result = value;
  }
  if (result > 255) {
    result = 255;
  }
  return result;
}
