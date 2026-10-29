#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_CHARACTERRENDER_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_CHARACTERRENDER_H

#include <cstddef>
#include <cstdint>
#include <map>

class CCharacterRender {
public:
  CCharacterRender(int unknown0068, int unknown006C);
  virtual ~CCharacterRender();

  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018[4];
  /* 0028 */ int unknown_0028;
  /* 002C */ int unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int unknown_0038;
  /* 003C */ int unknown_003C;
  /* 0040 */ int unknown_0040;
  /* 0044 */ int unknown_0044;
  /* 0048 */ int unknown_0048;
  /* 004C */ unsigned char unknown_004C;
  /* 0050 */ int unknown_0050;
  /* 0054 */ int unknown_0054;
  /* 0058 */ float unknown_0058;
  /* 005C */ int unknown_005C;
  /* 0060 */ int unknown_0060;
  /* 0064 */ int unknown_0064;
  /* 0068 */ int unknown_0068;
  /* 006C */ int unknown_006C;
  /* 0070 */ float unknown_0070;
  /* 0074 */ float unknown_0074;
  /* 0078 */ float unknown_0078;
  /* 007C */ float unknown_007C;
  /* 0080 */ float unknown_0080;
  /* 0084 */ float unknown_0084;
  /* 0088 */ float unknown_0088;
  /* 008C */ float unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ int unknown_0094[5];
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ int unknown_00AC;
  /* 00B0 */ unsigned char unknown_00B0;
  /* 00B4 */ std::map<int, int> unknown_00B4;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ unsigned char unknown_00C0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCharacterRender, unknown_0068) == 0x68);
static_assert(offsetof(CCharacterRender, unknown_008C) == 0x8C);
static_assert(offsetof(CCharacterRender, unknown_00B4) == 0xB4);
static_assert(sizeof(CCharacterRender) == 0xC4);
#endif

#endif
