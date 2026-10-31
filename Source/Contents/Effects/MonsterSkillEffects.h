#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MONSTERSKILLEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MONSTERSKILLEFFECTS_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectEmitter.h"

#include <cstddef>
#include <list>
#include <map>
#include <string>
#include <vector>

class CParticle;

class CCrimsonArrowEffect : public CEffect {
public:
  CCrimsonArrowEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CRootTwistEffect : public CEffect {
public:
  CRootTwistEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Slot12() override;
  void Close() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CSpriteMableEffect : public CEffect {
public:
  CSpriteMableEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CStemSpearEffect : public CEffect {
public:
  CStemSpearEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CSvgSpiritEffect : public CEffect {
public:
  CSvgSpiritEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CTunaPartyEffect : public CEffect {
public:
  CTunaPartyEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CCurseOfCubeEffect : public CEffect {
public:
  CCurseOfCubeEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  virtual void SpawnStart();
  virtual void SpawnLoop();
  virtual void SpawnShock();
  void Restart();

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ float unknown_00DC;
  /* 00E0 */ float unknown_00E0;
  /* 00E4 */ float unknown_00E4;
};

class CWarningPlaneEffect : public CEffect {
public:
  CWarningPlaneEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  void AddPlane(vector3d position);

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ unsigned char fadingIn;
  /* 00D9 */ unsigned char unknown_00D9[3];
  /* 00DC */ unsigned int expireTime;
};

class CCylinderEffect : public CEffect {
public:
  CCylinderEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ unsigned int startTime;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CDancingKnifeEffect : public CEffect {
public:
  CDancingKnifeEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  void SpawnFirstStrike();
  void SpawnSecondStrike();

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ std::vector<EZ2STREmitter*> unknown_00D8;
  /* 00E4 */ unsigned char secondStrike;
  /* 00E5 */ unsigned char unknown_00E5[3];
};

class CEternalSlashAttackEffect : public CEffect {
public:
  CEternalSlashAttackEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CEternalSlashCountEffect : public CEffect {
public:
  CEternalSlashCountEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CFallenAngelEffect : public CEffect {
public:
  CFallenAngelEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  void SetPosition(vector3d position);

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ float unknown_00D0;
  /* 00D4 */ float unknown_00D4;
  /* 00D8 */ float unknown_00D8;
};

class CFloralFlareRoad : public CEffect {
public:
  CFloralFlareRoad();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* unknown_00CC;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CFootprintEffect : public CEffect {
public:
  CFootprintEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  bool Slot33() override;
  void Close() override;
  CParticle* SpawnFootprint();

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ std::string unknown_00D4;
  /* 00EC */ std::string unknown_00EC;
  /* 0104 */ int unknown_0104;
  /* 0108 */ double stepDistance;
  /* 0110 */ unsigned char unknown_0110;
  /* 0111 */ unsigned char unknown_0111[3];
  /* 0114 */ vector3d lastPosition;
  /* 0120 */ float unknown_0120;
  /* 0124 */ int unknown_0124;
};

class CFootprintStrEffect : public CEffect {
public:
  CFootprintStrEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  bool Slot33() override;
  void Close() override;
  void SpawnFootprint();

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ std::list<EZ2STREmitter*> unknown_00D4;
  /* 00DC */ std::string unknown_00DC;
  /* 00F4 */ std::string unknown_00F4;
  /* 010C */ std::string unknown_010C;
  /* 0124 */ std::string unknown_0124;
  /* 0140 */ double unknown_0140;
  /* 0148 */ double unknown_0148;
  /* 0150 */ double unknown_0150;
  /* 0158 */ double unknown_0158;
  /* 0160 */ double unknown_0160;
  /* 0168 */ unsigned char unknown_0168;
  /* 0169 */ unsigned char unknown_0169;
  /* 016C */ vector3d unknown_016C;
};

class CFromTheAbyssEffect : public CEffect {
public:
  struct AbyssParticle {
    /* 0000 */ int kind;
    /* 0004 */ EZ2STREmitter* emitter;
  };

  struct AbyssWave {
    /* 0000 */ float scale;
    /* 0004 */ vector3d offset;
  };

  CFromTheAbyssEffect();
  void UpdateParticle(long long key, AbyssParticle particle);
  void SetLevel(long long level);
  void Slot03() override;
  void Slot01() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ unsigned char unknown_00CC[4];
  /* 00D0 */ long long maxLevel;
  /* 00D8 */ long long level;
  /* 00E0 */ std::map<long long, AbyssWave> unknown_00E0;
  /* 00E8 */ std::map<long long, AbyssParticle> unknown_00E8;
};

class CParticle;

class CFullScreenEffect : public CEffect {
public:
  CFullScreenEffect();
  void Slot01() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ unsigned char loaded;
  /* 00D9 */ unsigned char unknown_00D9[3];
};

class CPetitioEffect : public CEffect {
public:
  CPetitioEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CPneumaticusProcellaEffect : public CEffect {
public:
  CPneumaticusProcellaEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CRandomHitEffect : public CEffect {
public:
  CRandomHitEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;

  /* 00CC */ bool active;
  /* 00CD */ unsigned char unknown_00CD[3];
  /* 00D0 */ EZ2STREmitter* emitter;
};

class CRandomOptEffect : public CEffect {
public:
  CRandomOptEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Slot08(int red, int green, int blue) override;
  void Slot11(std::string name) override;
  void Close() override;
  void Init();

  /* 00CC */ vector3d offsets[2];
  /* 00E4 */ std::vector<CParticle*> particles;
};

class CRichsCoinEffect : public CEffect {
public:
  CRichsCoinEffect();
  void Slot01() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ std::vector<CParticle*> coins;
  /* 00E4 */ int unknown_00E4;
};

class CSavageImpactEffect : public CEffect {
public:
  CSavageImpactEffect();
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ vector3d offset;
};

class CServantWeaponEffect : public CEffect {
public:
  CServantWeaponEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  void SetShoot(int column);
  void SetPhantomShot(long long column);
  void SetSignShot(int column);

  /* 00CC */ vector3d offsets[5];
  /* 0108 */ EZ2STREmitter* emitters[3][5];
};

class CServantWeaponNewEffect : public CEffect {
public:
  CServantWeaponNewEffect();
  ~CServantWeaponNewEffect() override;
  bool Slot06() override;
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  void AddWeapon();
  EZ2STREmitter* CreateShot(float angle, int skill);
  void Shoot(int skill);

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ std::list<EZ2STREmitter*> trails;
};

class CSevereRainStorm : public CEffect {
public:
  CSevereRainStorm();
  ~CSevereRainStorm() override;
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void SpawnParticles();
  void SpawnCrossRain();
  void SpawnGrenades();
  void SpawnBombardHits();
  void SpawnCrystalRain();
  void SpawnSevereRain();
  void SpawnTwinklingGalaxy(int index);
  void SpawnStarCannon();
  void SpawnGalaxyStar();

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ int unknown_00D4;
};

class CGaleStorm : public CEffect {
public:
  CGaleStorm();
  void Slot01() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ int unknown_00DC;
};

class CJupitelThunderHit : public CEffect {
public:
  CJupitelThunderHit();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  void SpawnHit();
  void SpawnSpark();

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ std::vector<EZ2STREmitter*> unknown_00D8;
  /* 00E4 */ std::vector<std::string> textures;
};

class CNewCloudKillEffect : public CEffect {
public:
  CNewCloudKillEffect();
  ~CNewCloudKillEffect() override;
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;

  /* 00CC */ std::list<EZ2STREmitter*> unknown_00CC;
  /* 00D4 */ std::list<EZ2STREmitter*> unknown_00D4;
  /* 00DC */ std::list<EZ2STREmitter*> unknown_00DC;
  /* 00E4 */ std::list<EZ2STREmitter*> unknown_00E4;
  /* 00EC */ std::list<EZ2STREmitter*> unknown_00EC;
  /* 00F4 */ int unknown_00F4;
  /* 00F8 */ int unknown_00F8;
};

// VTABLE: vt_01909
class CDropItemEffect : public CEffect {
public:
  CDropItemEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  void Slot12() override;
  void OpenEmitters();

  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int emitterCount;
  /* 00D4 */ std::vector<EZ2STREmitter*> emitters;
  /* 00E0 */ std::string soundPath;
  /* 00F8 */ std::string soundName;
  /* 0110 */ vector3d position;
};

// VTABLE: vt_01908
class CHawkVumerangEffect : public CEffect {
public:
  CHawkVumerangEffect();
  ~CHawkVumerangEffect() override;
  void Slot04() override;
  bool Slot06() override;
  void Close() override;
  void Slot03() override;

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ std::list<EZ2STREmitter*> unknown_00D4;
  /* 00DC */ int unknown_00DC;
  union {
    struct {
      /* 00E0 */ int unknown_00E0;
      /* 00E4 */ int unknown_00E4;
    };
    /* 00E0 */ long long count;
  };
  /* 00E8 */ double unknown_00E8;
  /* 00F0 */ float unknown_00F0;
  /* 00F4 */ float unknown_00F4;
  /* 00F8 */ float unknown_00F8;
  /* 00FC */ unsigned int unknown_00FC;
  /* 0100 */ vector3d unknown_0100;
  /* 010C */ vector3d unknown_010C;
  /* 0118 */ vector3d unknown_0118;
  /* 0124 */ int unknown_0124;
  /* 0128 */ double unknown_0128;
  /* 0130 */ double unknown_0130;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCrimsonArrowEffect, emitter) == 0xCC);
static_assert(sizeof(CRootTwistEffect) == 0xD8);
static_assert(sizeof(CSvgSpiritEffect) == 0xD0);
static_assert(offsetof(CWarningPlaneEffect, expireTime) == 0xDC);
static_assert(offsetof(CCylinderEffect, emitters) == 0xD0);
static_assert(sizeof(CDancingKnifeEffect) == 0xE8);
static_assert(sizeof(CEternalSlashAttackEffect) == 0xD8);
static_assert(sizeof(CEternalSlashCountEffect) == 0xD0);
static_assert(sizeof(CFallenAngelEffect) == 0xDC);
static_assert(sizeof(CFromTheAbyssEffect) == 0xF0);
static_assert(offsetof(CFromTheAbyssEffect, unknown_00E0) == 0xE0);
static_assert(offsetof(CFromTheAbyssEffect, unknown_00E8) == 0xE8);
static_assert(sizeof(CFloralFlareRoad) == 0xDC);
static_assert(offsetof(CFootprintEffect, stepDistance) == 0x108);
static_assert(offsetof(CFootprintEffect, lastPosition) == 0x114);
static_assert(sizeof(CFootprintEffect) == 0x128);
static_assert(sizeof(CFootprintStrEffect) == 0x178);
static_assert(offsetof(CFootprintStrEffect, unknown_00DC) == 0xDC);
static_assert(offsetof(CFootprintStrEffect, unknown_0124) == 0x124);
static_assert(offsetof(CFootprintStrEffect, unknown_0140) == 0x140);
static_assert(offsetof(CFootprintStrEffect, unknown_016C) == 0x16C);
static_assert(offsetof(CFullScreenEffect, loaded) == 0xD8);
static_assert(sizeof(CFullScreenEffect) == 0xDC);
static_assert(sizeof(CPetitioEffect) == 0xD8);
static_assert(sizeof(CPneumaticusProcellaEffect) == 0xDC);
static_assert(sizeof(CRandomHitEffect) == 0xD4);
static_assert(sizeof(CRandomOptEffect) == 0xF0);
static_assert(sizeof(CRichsCoinEffect) == 0xE8);
static_assert(sizeof(CSavageImpactEffect) == 0xDC);
static_assert(sizeof(CServantWeaponEffect) == 0x144);
static_assert(sizeof(CServantWeaponNewEffect) == 0xDC);
static_assert(sizeof(CSevereRainStorm) == 0xD8);
static_assert(sizeof(CGaleStorm) == 0xE0);
static_assert(offsetof(CGaleStorm, unknown_00D8) == 0xD8);
static_assert(sizeof(CJupitelThunderHit) == 0xF0);
static_assert(offsetof(CJupitelThunderHit, textures) == 0xE4);
static_assert(sizeof(CNewCloudKillEffect) == 0xFC);
static_assert(offsetof(CNewCloudKillEffect, unknown_00F4) == 0xF4);
static_assert(offsetof(CDropItemEffect, emitters) == 0xD4);
static_assert(offsetof(CDropItemEffect, position) == 0x110);
static_assert(sizeof(CDropItemEffect) == 0x11C);
static_assert(offsetof(CHawkVumerangEffect, unknown_00D4) == 0xD4);
static_assert(offsetof(CHawkVumerangEffect, unknown_0100) == 0x100);
static_assert(offsetof(CHawkVumerangEffect, unknown_0130) == 0x130);
static_assert(sizeof(CHawkVumerangEffect) == 0x138);
#endif

#endif
