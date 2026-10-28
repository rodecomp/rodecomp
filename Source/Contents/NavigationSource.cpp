#include "Contents/NavigationManager.h"

// OFFSET: 0x00B96980

// OFFSET: 0x00B96AE0

// OFFSET: 0x00B971D0
// FUNCTION: fn_19597
int NavigationManager::GetCurrentTargetType() const {
  return this->currentTarget.type;
}

// OFFSET: 0x00BA0AE0
// FUNCTION: fn_19635
int NavigationManager::SaveCurrentTarget() {
  const int type = this->currentTarget.type;
  if (type == 6) {
    return -92;
  }
  if ((type != 1 && type != 2 && type != 3 && type != 0) || !this->currentTarget.object) {
    return -97;
  }
  this->savedTarget = this->currentTarget;
  return 0;
}
