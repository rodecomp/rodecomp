#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MONSTERSKILLEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_MONSTERSKILLEFFECTS_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/EffectEmitter.h"

#include <cstddef>
#include <list>
#include <map>
#include <string>
#include <vector>

class CParticle;
class CEZ2STRParticle;
class CSpriteParticle;

class CCrimsonArrowEffect : public CEffect {
public:
  CCrimsonArrowEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CRootTwistEffect : public CEffect {
public:
  CRootTwistEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void InitEvents() override;
  void Close() override;

  /* 00CC */ std::vector<CSpriteParticle*> emitters;
};

class CSpriteMableEffect : public CEffect {
public:
  CSpriteMableEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ std::vector<CSpriteParticle*> emitters;
};

class CStemSpearEffect : public CEffect {
public:
  CStemSpearEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ std::vector<CSpriteParticle*> emitters;
};

class CSvgSpiritEffect : public CEffect {
public:
  CSvgSpiritEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CTunaPartyEffect : public CEffect {
public:
  CTunaPartyEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ std::vector<CSpriteParticle*> emitters;
};

class CCurseOfCubeEffect : public CEffect {
public:
  CCurseOfCubeEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  virtual void SpawnStart();
  virtual void SpawnLoop();
  virtual void SpawnShock();
  void Restart();

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ int spawnPhase;
  /* 00DC */ float viewOffsetX;
  /* 00E0 */ float viewOffsetY;
  /* 00E4 */ float viewOffsetZ;
};

class CWarningPlaneEffect : public CEffect {
public:
  CWarningPlaneEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void AddPlane(vector3d position);

  /* 00CC */ std::vector<CParticle*> emitters;
  /* 00D8 */ unsigned char fadingIn;
  /* 00D9 */ unsigned char padding_00D9[3];
  /* 00DC */ unsigned int expireTime;
};

class CCylinderEffect : public CEffect {
public:
  CCylinderEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ unsigned int startTime;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CDancingKnifeEffect : public CEffect {
public:
  CDancingKnifeEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void SpawnFirstStrike();
  void SpawnSecondStrike();

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ std::vector<EZ2STREmitter*> secondStrikeEmitters;
  /* 00E4 */ unsigned char secondStrike;
  /* 00E5 */ unsigned char padding_00E5[3];
};

class CEternalSlashAttackEffect : public CEffect {
public:
  CEternalSlashAttackEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CEternalSlashCountEffect : public CEffect {
public:
  CEternalSlashCountEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CFallenAngelEffect : public CEffect {
public:
  CFallenAngelEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void SetPosition(vector3d position);

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ float startX;
  /* 00D4 */ float startY;
  /* 00D8 */ float startZ;
};

class CFloralFlareRoad : public CEffect {
public:
  CFloralFlareRoad();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CFootprintEffect : public CEffect {
public:
  CFootprintEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  bool IsVisible() override;
  void Close() override;
  CParticle* SpawnFootprint();

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ std::string leftTexture;
  /* 00EC */ std::string rightTexture;
  /* 0104 */ int padding_0104;
  /* 0108 */ double stepDistance;
  /* 0110 */ unsigned char leftFoot;
  /* 0111 */ unsigned char padding_0111[3];
  /* 0114 */ vector3d lastPosition;
  /* 0120 */ float lastGroundHeight;
  /* 0124 */ int padding_0124;
};

class CFootprintStrEffect : public CEffect {
public:
  CFootprintStrEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  bool IsVisible() override;
  void Close() override;
  void SpawnFootprint();

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ std::list<EZ2STREmitter*> topEmitters;
  /* 00DC */ std::string leftBottomFile;
  /* 00F4 */ std::string rightBottomFile;
  /* 010C */ std::string leftTopFile;
  /* 0124 */ std::string rightTopFile;
  /* 0140 */ double bottomScale;
  /* 0148 */ double topScale;
  /* 0150 */ double topHeight;
  /* 0158 */ double stepDistance;
  /* 0160 */ double footGap;
  /* 0168 */ unsigned char adjustAngle;
  /* 0169 */ unsigned char leftFoot;
  /* 016C */ vector3d lastPosition;
};

class CFromTheAbyssEffect : public CEffect {
public:
  struct AbyssParticle {
    /* 0000 */ int kind;
    /* 0004 */ EZ2STREmitter* emitter;
  };

  struct AbyssWave {
    AbyssWave() : scale(0.0f), offset() {}
    AbyssWave(float scale, const vector3d& offset) : scale(scale), offset(offset) {}

    /* 0000 */ float scale;
    /* 0004 */ vector3d offset;
  };

  CFromTheAbyssEffect();
  void UpdateParticle(long long key, AbyssParticle particle);
  void SetLevel(long long level);
  void OnProcess() override;
  void Open() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ unsigned char padding_00CC[4];
  /* 00D0 */ long long maxLevel;
  /* 00D8 */ long long level;
  /* 00E0 */ std::map<long long, AbyssWave> waves;
  /* 00E8 */ std::map<long long, AbyssParticle> particles;
};

class CParticle;

class CFullScreenEffect : public CEffect {
public:
  CFullScreenEffect();
  void Open() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ unsigned char loaded;
  /* 00D9 */ unsigned char padding_00D9[3];
};

class CPetitioEffect : public CEffect {
public:
  CPetitioEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CPneumaticusProcellaEffect : public CEffect {
public:
  CPneumaticusProcellaEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CRandomHitEffect : public CEffect {
public:
  CRandomHitEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  // INLINED
  bool IsAlive() override {
    return this->active;
  }

  /* 00CC */ bool active;
  /* 00CD */ unsigned char padding_00CD[3];
  /* 00D0 */ EZ2STREmitter* emitter;
};

class CRandomOptEffect : public CEffect {
public:
  CRandomOptEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void SetColor(int red, int green, int blue) override;
  void SetParticleTexture(std::string name) override;
  void Close() override;
  void Init();

  /* 00CC */ vector3d offsets[2];
  /* 00E4 */ std::vector<CParticle*> particles;
};

class CRichsCoinEffect : public CEffect {
public:
  CRichsCoinEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ std::vector<CParticle*> coins;
  /* 00E4 */ int coinCount;
};

class CSavageImpactEffect : public CEffect {
public:
  CSavageImpactEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ vector3d offset;
};

class CServantWeaponEffect : public CEffect {
public:
  CServantWeaponEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
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
  bool IsAlive() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
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
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

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
  /* 00D4 */ int elapsedFrames;
};

class CGaleStorm : public CEffect {
public:
  CGaleStorm();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void SpawnArrow();
  void SpawnStorm();

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  union {
    struct {
      /* 00D8 */ int countLow;
      /* 00DC */ int countHigh;
    };
    /* 00D8 */ long long count;
  };
};

class CJupitelThunderHit : public CEffect {
public:
  CJupitelThunderHit();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void SpawnHit();
  void SpawnSpark();

  /* 00CC */ std::vector<CParticle*> emitters;
  /* 00D8 */ std::vector<CParticle*> hitEmitters;
  /* 00E4 */ std::vector<std::string> textures;
};

class CNewCloudKillEffect : public CEffect {
public:
  CNewCloudKillEffect();
  ~CNewCloudKillEffect() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void ResetMistParticle(CParticle* particle);
  void ResetCloudParticle(CParticle* particle);
  void ResetLightning(CParticle* particle);
  void ResetSmokeParticle(CParticle* particle);

  /* 00CC */ std::list<EZ2STREmitter*> circleEmitters;
  /* 00D4 */ std::list<EZ2STREmitter*> cloudEmitters;
  /* 00DC */ std::list<EZ2STREmitter*> smokeEmitters;
  /* 00E4 */ std::list<EZ2STREmitter*> lightningEmitters;
  /* 00EC */ std::list<EZ2STREmitter*> mistEmitters;
  /* 00F4 */ int circleResetTime;
  /* 00F8 */ int cloudResetTime;
};

// VTABLE: vt_01909
class CDropItemEffect : public CEffect {
public:
  CDropItemEffect();
  void Open() override;
  void OnProcess() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void InitEvents() override;
  void OpenEmitters();

  /* 00CC */ int padding_00CC;
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
  void Open() override;
  void Render() override;
  bool IsAlive() override;
  void Close() override;
  void OnProcess() override;

  /* 00CC */ std::list<CEZ2STRParticle*> emitters;
  /* 00D4 */ std::list<CEZ2STRParticle*> finishedEmitters;
  /* 00DC */ int padding_00DC;
  union {
    struct {
      /* 00E0 */ int countLow;
      /* 00E4 */ int countHigh;
    };
    /* 00E0 */ long long count;
  };
  /* 00E8 */ double angularRange;
  /* 00F0 */ float orbitOffsetX;
  /* 00F4 */ float orbitOffsetY;
  /* 00F8 */ float orbitOffsetZ;
  /* 00FC */ unsigned int startTime;
  /* 0100 */ vector3d targetPosition;
  /* 010C */ vector3d originPosition;
  /* 0118 */ vector3d orbitCenter;
  /* 0124 */ int padding_0124;
  /* 0128 */ double orbitRadius;
  /* 0130 */ double startAngle;
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
static_assert(offsetof(CFromTheAbyssEffect, waves) == 0xE0);
static_assert(offsetof(CFromTheAbyssEffect, particles) == 0xE8);
static_assert(sizeof(CFloralFlareRoad) == 0xDC);
static_assert(offsetof(CFootprintEffect, stepDistance) == 0x108);
static_assert(offsetof(CFootprintEffect, lastPosition) == 0x114);
static_assert(sizeof(CFootprintEffect) == 0x128);
static_assert(sizeof(CFootprintStrEffect) == 0x178);
static_assert(offsetof(CFootprintStrEffect, leftBottomFile) == 0xDC);
static_assert(offsetof(CFootprintStrEffect, rightTopFile) == 0x124);
static_assert(offsetof(CFootprintStrEffect, bottomScale) == 0x140);
static_assert(offsetof(CFootprintStrEffect, lastPosition) == 0x16C);
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
static_assert(offsetof(CGaleStorm, countLow) == 0xD8);
static_assert(sizeof(CJupitelThunderHit) == 0xF0);
static_assert(offsetof(CJupitelThunderHit, textures) == 0xE4);
static_assert(sizeof(CNewCloudKillEffect) == 0xFC);
static_assert(offsetof(CNewCloudKillEffect, circleResetTime) == 0xF4);
static_assert(offsetof(CDropItemEffect, emitters) == 0xD4);
static_assert(offsetof(CDropItemEffect, position) == 0x110);
static_assert(sizeof(CDropItemEffect) == 0x11C);
static_assert(offsetof(CHawkVumerangEffect, finishedEmitters) == 0xD4);
static_assert(offsetof(CHawkVumerangEffect, targetPosition) == 0x100);
static_assert(offsetof(CHawkVumerangEffect, startAngle) == 0x130);
static_assert(sizeof(CHawkVumerangEffect) == 0x138);
#endif

#endif
