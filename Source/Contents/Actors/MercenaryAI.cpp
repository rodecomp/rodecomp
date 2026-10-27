#include "Contents/Actors/MercenaryAI.h"

extern "C" {
#include <lua.h>
}

// OFFSET: 0x00D9E3F0

// OFFSET: 0x00D9E430

// OFFSET: 0x00D9E570
// FUNCTION: fn_22331
void CMercenaryAI::Shutdown() {
  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }
}
