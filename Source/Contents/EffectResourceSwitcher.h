#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTRESOURCESWITCHER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTRESOURCESWITCHER_H

#include "Contents/Effects/Effect.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <map>
#include <string>

struct EZ2STREffectInfo {
  /* 0000 */ int id;
  /* 0004 */ std::string name;
  /* 001C */ int unknown_001C[3];
  /* 0028 */ std::string unknown_0028;
  /* 0040 */ int unknown_0040[2];
};

class CEZ2STREffectMgr : public Singleton<CEZ2STREffectMgr> {
public:
  /* 0004 */ std::map<std::string, EZ2STREffectInfo> effects;
};

class EffectResourceSwitcher : public CEffect {
public:
  /* 00CC */ unsigned char active;
  /* 00CD */ unsigned char unknown_00CD[3];
  /* 00D0 */ CEffect* effect;
  /* 00D4 */ int unknown_00D4[3];
  /* 00E0 */ std::string sourceName;
  /* 00F8 */ std::string targetName;

  void Slot16(int argument) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EZ2STREffectInfo) == 0x48);
static_assert(sizeof(CEZ2STREffectMgr) == 0xC);
static_assert(offsetof(EffectResourceSwitcher, effect) == 0xD0);
static_assert(offsetof(EffectResourceSwitcher, targetName) == 0xF8);
static_assert(sizeof(EffectResourceSwitcher) == 0x110);
#endif

#endif
