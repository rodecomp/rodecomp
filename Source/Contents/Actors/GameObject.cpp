#include "Contents/Actors/GameObject.h"
#include "Utility/Math/ScalarConversion.h"

namespace {
  // STATIC: 0x0135EF80
  // GLOBAL: gl_21923
  std::uint64_t nextIdentity = 0;
}

// OFFSET: 0x00CA8600
// FUNCTION: fn_20989
CGameObject::CGameObject() : identity(++nextIdentity) {
}

// OFFSET: 0x00CA8EC0
// OFFSET: 0x00CA9230
// FUNCTION: fn_20995
CGameObject::~CGameObject() = default;

// OFFSET: 0x00CAFD20
// FUNCTION: fn_21087
bool CGameObject::Update() {
  return true;
}

// OFFSET: 0x00CBF850
// FUNCTION: fn_21113
void CGameObject::SendObjectMessage(CGameObject*, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x00CB20A0
// FUNCTION: fn_21101
void CGameObject::Draw(int) {
}

// OFFSET: 0x00CAB920
// FUNCTION: fn_21031
int CGameObject::Direction(float angle, const CameraDirectionContext& camera) {
  if (camera.modeType != 1) {
    return 0;
  }

  const float negative = -(*camera.cameraAngle);
  float relative = negative - angle;

  while (relative < 0) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  return TruncateToInt(relative / 45.0f);
}

struct GameObjectActorContext : public CGameObject {
  /* 0010 */ unsigned char pad_0010[0x24];
  /* 0034 */ int field_0034;
  /* 0038 */ unsigned char pad_0038[0x18];
  /* 0050 */ float field_0050;
  /* 0054 */ unsigned char pad_0054[4];
  /* 0058 */ float field_0058;
  /* 005C */ unsigned char pad_005C[0x14];
  /* 0070 */ int field_0070;
  /* 0074 */ unsigned char pad_0074[0x28];
  /* 009C */ unsigned char field_009C;
  /* 009D */ unsigned char pad_009D;
  /* 009E */ unsigned char field_009E;
  /* 009F */ unsigned char pad_009F;
  /* 00A0 */ unsigned char field_00A0;
  /* 00A1 */ unsigned char pad_00A1[4];
  /* 00A5 */ unsigned char field_00A5;
  /* 00A6 */ unsigned char pad_00A6[0x6E];
  /* 0114 */ unsigned char field_0114;
  /* 0115 */ unsigned char pad_0115[0x147];
  /* 025C */ int field_025C;
  /* 0260 */ unsigned char pad_0260[0x1A4];
  /* 0404 */ int field_0404;
  /* 0408 */ int field_0408;

  void SubCB20B0(int a);
  void SubCB20F0(int a, int b, int c);
  void SubCB2830();
  void SubCB35F0(int a, float b);
  void SubCB39A0();
  void SubCB3FA0(float speed);
  void SubCB4070();
  void SubCB4100();
  void SubCB4800();
};

// OFFSET: 0x00CB20B0
// FUNCTION: fn_21102
void GameObjectActorContext::SubCB20B0(int a) {
  if (this->field_00A0 != 0) {
    this->SubCB35F0(a, this->field_0050);
    this->SubCB20F0(a, 0, 0);
  }
}

// OFFSET: 0x00CB20F0

// OFFSET: 0x00CB2830

// OFFSET: 0x00CB35F0

// OFFSET: 0x00CB39A0

// OFFSET: 0x00CB3F40

// OFFSET: 0x00CB3FA0

// OFFSET: 0x00CB3FF0

// OFFSET: 0x00CB4070

// OFFSET: 0x00CB4100

// OFFSET: 0x00CB4800
