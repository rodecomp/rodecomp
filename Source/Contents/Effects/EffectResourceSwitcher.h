#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTRESOURCESWITCHER_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTRESOURCESWITCHER_H

#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/EffectEmitter.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct EZ2STREffectInfo {
  /* 0000 */ int id = 0;
  /* 0004 */ std::string name;
  /* 001C */ float offsetX;
  /* 0020 */ float offsetY;
  /* 0024 */ unsigned int flags = 0;
  /* 0028 */ std::string soundPath;
  /* 0040 */ int delays[2] = {-1, 0};
};

struct EZ2STREffectRecord {
  /* 0000 */ std::string name;
  /* 0018 */ double offsetX;
  /* 0020 */ double offsetY;
  /* 0028 */ bool infinite;
  /* 0029 */ bool floor;
  /* 002A */ bool aboveUi;
  /* 002C */ std::string soundPath;
  /* 0044 */ bool soundLoop;
  /* 0048 */ int delays[2];
  /* 0050 */ bool toGround;
};

class CEZ2STREffectMgr : public Singleton<CEZ2STREffectMgr> {
public:
  /* 0004 */ std::map<std::string, EZ2STREffectInfo> effects;

  struct InfoInserter {
    /* 0000 */ CEZ2STREffectMgr* manager;

    void operator()(const std::pair<const std::string, EZ2STREffectRecord>& entry) const;
  };

  __declspec(noinline) EZ2STREffectInfo* FindById(int id);
  int EffectId(const std::string& name);
  std::vector<int> EffectIds();
  bool Load();
};

class EffectResourceSwitcher : public CEffect {
public:
  /* 00CC */ unsigned char active;
  /* 00CD */ unsigned char padding_00CD[3];
  /* 00D0 */ CEZ2STREffectEx* effect;
  /* 00D4 */ vector3d spawnPosition;
  /* 00E0 */ std::string sourceName;
  /* 00F8 */ std::string targetName;

  EffectResourceSwitcher();

  // INLINED
  ~EffectResourceSwitcher() override {
    this->RetireEffects();
  }

  void OnProcess() override;
  bool IsAlive() override;
  void SetDelay(int argument) override;

  void Start();
  void SetNames(const std::string& source, const std::string& target);
  void RetireEffects();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EZ2STREffectInfo) == 0x48);
static_assert(sizeof(CEZ2STREffectMgr) == 0xC);
static_assert(sizeof(EZ2STREffectRecord) == 0x58);
static_assert(offsetof(EZ2STREffectRecord, soundPath) == 0x2C);
static_assert(offsetof(EZ2STREffectRecord, toGround) == 0x50);
static_assert(offsetof(EZ2STREmitter, currentFrame) == 0x3CC4);
static_assert(offsetof(EZ2STREmitter, looping) == 0x3F5C);
static_assert(offsetof(EZ2STREmitter, lifetimeLimit) == 0x3F98);
static_assert(offsetof(EZ2STREmitter, remainingTime) == 0x3FB4);
static_assert(offsetof(EZ2STREmitter, flipY) == 0x3FE9);
static_assert(offsetof(EffectResourceSwitcher, effect) == 0xD0);
static_assert(offsetof(EffectResourceSwitcher, targetName) == 0xF8);
static_assert(sizeof(EffectResourceSwitcher) == 0x110);
#endif

#endif
