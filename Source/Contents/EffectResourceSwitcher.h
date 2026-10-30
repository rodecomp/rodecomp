#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTRESOURCESWITCHER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTRESOURCESWITCHER_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectEmitter.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <map>
#include <string>

struct EZ2STREffectInfo {
  /* 0000 */ int id;
  /* 0004 */ std::string name;
  /* 001C */ float offsetX;
  /* 0020 */ float offsetY;
  /* 0024 */ int unknown_0024;
  /* 0028 */ std::string unknown_0028;
  /* 0040 */ int unknown_0040[2];
};

class CEZ2STREffectMgr : public Singleton<CEZ2STREffectMgr> {
public:
  /* 0004 */ std::map<std::string, EZ2STREffectInfo> effects;

  EZ2STREffectInfo* FindById(int id);
  int EffectId(const std::string& name);
};

class CEZ2STREffect : public CEffect {
public:
  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ unsigned char timed;
  /* 00D1 */ unsigned char unknown_00D1[3];
  /* 00D4 */ int frameStep;
  /* 00D8 */ unsigned char alternateRender;

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Slot14(int argument) override;
  void Slot16(int argument) override;
  void Slot17(float argument) override;
  void Slot18(float argument) override;
  void Slot19(float argument) override;
  void Slot22(float argument) override;
  void Slot23(float argument) override;
  void Slot26(int argument) override;
  void Slot27(int argument) override;
  void Slot32() override;

  bool IsFinished();
  void EndLoop(unsigned char fadeOut);
};

class EffectResourceSwitcher : public CEffect {
public:
  /* 00CC */ unsigned char active;
  /* 00CD */ unsigned char unknown_00CD[3];
  /* 00D0 */ CEZ2STREffect* effect;
  /* 00D4 */ vector3d spawnPosition;
  /* 00E0 */ std::string sourceName;
  /* 00F8 */ std::string targetName;

  EffectResourceSwitcher();

  // INLINED
  ~EffectResourceSwitcher() override {
    this->RetireEffects();
  }

  void Slot03() override;
  bool Slot06() override;
  void Slot16(int argument) override;

  void Start();
  void SetNames(const std::string& source, const std::string& target);
  void RetireEffects();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EZ2STREffectInfo) == 0x48);
static_assert(sizeof(CEZ2STREffectMgr) == 0xC);
static_assert(offsetof(EZ2STREmitter, currentFrame) == 0x3CC4);
static_assert(offsetof(EZ2STREmitter, looping) == 0x3F5C);
static_assert(offsetof(EZ2STREmitter, unknown_3F98) == 0x3F98);
static_assert(offsetof(EZ2STREmitter, remainingTime) == 0x3FB4);
static_assert(offsetof(EZ2STREmitter, unknown_3FE9) == 0x3FE9);
static_assert(offsetof(CEZ2STREffect, alternateRender) == 0xD8);
static_assert(offsetof(EffectResourceSwitcher, effect) == 0xD0);
static_assert(offsetof(EffectResourceSwitcher, targetName) == 0xF8);
static_assert(sizeof(EffectResourceSwitcher) == 0x110);
#endif

#endif
