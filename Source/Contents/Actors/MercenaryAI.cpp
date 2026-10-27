#include "Contents/Actors/MercenaryAI.h"

extern "C" {
#include <lua.h>
}

// OFFSET: 0x00D9E3F0
// FUNCTION: fn_22327
CMercenaryAI::CMercenaryAI(int kind) : state(nullptr), actionStarted(0), actionDuration(0), kind(kind), lastTick(0) {
}

// OFFSET: 0x00D9E430
// FUNCTION: fn_22328
CMercenaryAI::~CMercenaryAI() {
  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }
}

// OFFSET: 0x00D9E570
// FUNCTION: fn_22331
void CMercenaryAI::Shutdown() {
  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }
}
