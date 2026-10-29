#include "Contents/Effects/CaneOfEvilEyeEffect.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x006F86B0

// OFFSET: 0x006F8E70
// FUNCTION: fn_06876
void CCaneOfEvilEyeEffect::RemoveNullEmitters() {
  for (auto it = this->unknown_00D0.begin(); it != this->unknown_00D0.end();) {
    if (*it == nullptr) {
      it = this->unknown_00D0.erase(it);
    } else {
      ++it;
    }
  }

  for (auto it = this->unknown_00DC.begin(); it != this->unknown_00DC.end();) {
    if (*it == nullptr) {
      it = this->unknown_00DC.erase(it);
    } else {
      ++it;
    }
  }
}
