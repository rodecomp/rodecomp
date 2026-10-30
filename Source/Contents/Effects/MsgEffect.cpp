#include "Contents/Effects/MsgEffect.h"

// OFFSET: 0x00E4A9B0
// FUNCTION: fn_24258
CMsgEffect::~CMsgEffect() {
  if (this->m_owner != nullptr) {
    this->m_owner->SendMsg(this, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E4B630
// FUNCTION: fn_24270
bool CMsgEffect::UsesMsgEffect(int type) {
  switch (type) {
    case 3:
    case 18:
    case 27:
    case 31:
      return true;
    default:
      return false;
  }
}
