#include "Contents/Effects/RectUpEffect.h"

// OFFSET: 0x00709F50
CRectUpEffect::CRectUpEffect() : CEffect(1088), unknown_00CC{} {
}

// OFFSET: 0x0070A410
void CRectUpEffect::Slot12() {
  if (this->id == 1088) {
    this->AddEvent(0, "effect\\EF_FireWall.wav", 0, 0);
  }
}
