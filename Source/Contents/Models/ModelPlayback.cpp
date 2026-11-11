#include "Contents/Models/ModelActor.h"
#include "Utility/Math/ScalarConversion.h"

#include <windows.h>

namespace {
  // INLINED
  unsigned UnsignedFrame(double value) {
    if (!(value < 0x1p32) || value <= -1.0) {
      return 0xFFFFFFFFu;
    }

    return value < 0.0 ? 0u : static_cast<unsigned>(value);
  }
}

// OFFSET: 0x00AB7930
// FUNCTION: fn_17493
void C3dActorV1::Update() {
  switch (this->playback) {
    case 1: {
      const unsigned tick = GetTickCount();
      if (tick - this->lastTick == 0) {
        return;
      }

      this->frame = static_cast<int>(this->speed * 100.0f + static_cast<float>(this->frame));
      if (this->frame >= this->animationLength) {
        this->frame = this->animationLength - 1;
      }

      this->Animate(static_cast<double>(this->frame));
      this->lastTick = tick;
      break;
    }
    case 2: {
      const unsigned tick = GetTickCount();
      if (tick - this->lastTick == 0) {
        return;
      }

      this->frame = static_cast<int>(this->speed * 100.0f + static_cast<float>(this->frame));
      if (this->frame >= this->animationLength) {
        this->frame = 0;
      }

      this->Animate(static_cast<double>(this->frame));
      this->lastTick = tick;
      break;
    }
  }
}

// OFFSET: 0x00AB7A10
// FUNCTION: fn_17494
void C3dActorV2::Update() {
  switch (this->playback) {
    case 1: {
      const unsigned tick = GetTickCount();
      unsigned frame = static_cast<unsigned>(static_cast<float>(tick - this->startTick) * this->speed);
      if (static_cast<double>(frame) >= this->duration) {
        frame = static_cast<unsigned>(this->duration - 1.0);
      }
      this->Animate(static_cast<double>(frame));
      break;
    }
    case 2: {
      const unsigned tick = GetTickCount();
      unsigned frame = static_cast<unsigned>(static_cast<float>(tick - this->startTick) * this->speed);
      if (static_cast<double>(frame) >= this->duration) {
        this->startTick = tick;
        frame = 0;
      }
      this->Animate(static_cast<double>(frame));
      break;
    }
  }
}
