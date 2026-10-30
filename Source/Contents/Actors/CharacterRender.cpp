#include "Contents/Actors/CharacterRender.h"

// OFFSET: 0x0080D2A0
// FUNCTION: fn_10099
CCharacterRender::CCharacterRender(int unknown0068, int unknown006C)
    : unknown_0004(0), unknown_0008(0), unknown_000C(0), unknown_0014(0), unknown_0028(0), unknown_002C(0), unknown_0030(0), unknown_0034(0),
      unknown_0038(0), unknown_003C(0), unknown_0048(0), unknown_004C(0), unknown_0050(0), unknown_0054(0), unknown_0058(80.0f), unknown_005C(-1),
      unknown_0060(-1), unknown_0064(-1), unknown_0068(unknown0068), unknown_006C(unknown006C), unknown_0070(400.0f), unknown_0074(-45.0f),
      unknown_0078(0.0f), unknown_007C(0.0f), unknown_0080(0.0f), unknown_0084(0.0f), unknown_0088(0.0f), unknown_008C(1.0f), unknown_0090(0),
      unknown_00A8(0), unknown_00AC(0), unknown_00BC(0), unknown_00C0(0) {
  this->unknown_0004 = 0;
  this->unknown_0008 = 0;
  this->unknown_000C = 0;
  this->unknown_0040 = 0;
  this->unknown_0044 = 0;
  this->unknown_00B0 = 1;
}

// OFFSET: 0x0080D730
// FUNCTION: fn_10101
CCharacterRender::~CCharacterRender() = default;
