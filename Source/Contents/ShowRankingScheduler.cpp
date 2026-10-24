#include "Contents/ShowRankingScheduler.h"

#include <cerrno>
#include <cstring>
#include <crtdbg.h>

// OFFSET: 0x006380A0
// FUNCTION: fn_04740
const void* CShowRankingScheduler::SetData(const void* data) {
  if (data) {
    std::memcpy(this->rankingData, data, sizeof(this->rankingData));
    return data;
  }

  std::memset(this->rankingData, 0, sizeof(this->rankingData));
  errno = EINVAL;
  _invalid_parameter_noinfo();
  return nullptr;
}
