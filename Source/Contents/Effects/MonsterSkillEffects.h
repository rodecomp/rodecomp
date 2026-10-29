#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MONSTERSKILLEFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MONSTERSKILLEFFECTS_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectEmitter.h"

#include <cstddef>
#include <list>
#include <string>
#include <vector>

class CCrimsonArrowEffect : public CEffect {
public:
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CRootTwistEffect : public CEffect {
public:
  CRootTwistEffect();
  void Slot03() override;
  bool Slot06() override;
  void Slot12() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CSpriteMableEffect : public CEffect {
public:
  CSpriteMableEffect();
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CStemSpearEffect : public CEffect {
public:
  void Slot03() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CSvgSpiritEffect : public CEffect {
public:
  void Slot03() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CTunaPartyEffect : public CEffect {
public:
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CCurseOfCubeEffect : public CEffect {
public:
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CWarningPlaneEffect : public CEffect {
public:
  void Slot01() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ unsigned char fadingIn;
  /* 00D9 */ unsigned char unknown_00D9[3];
  /* 00DC */ unsigned int expireTime;
};

class CCylinderEffect : public CEffect {
public:
  CCylinderEffect();
  bool Slot06() override;

  /* 00CC */ unsigned int startTime;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CDancingKnifeEffect : public CEffect {
public:
  CDancingKnifeEffect();
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
  /* 00D8 */ std::vector<EZ2STREmitter*> unknown_00D8;
  /* 00E4 */ int unknown_00E4;
};

class CEternalSlashAttackEffect : public CEffect {
public:
  CEternalSlashAttackEffect();
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CEternalSlashCountEffect : public CEffect {
public:
  CEternalSlashCountEffect();
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* emitter;
};

class CFallenAngelEffect : public CEffect {
public:
  CFallenAngelEffect();
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
};

class CFloralFlareRoad : public CEffect {
public:
  CFloralFlareRoad();
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* unknown_00CC;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CFootprintEffect : public CEffect {
public:
  CFootprintEffect();
  void Slot04() override;
  bool Slot06() override;
  bool Slot33() override;

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
  bool Slot06() override;
  bool Slot33() override;
};

class CFromTheAbyssEffect : public CEffect {
public:
  void Slot01() override;
  bool Slot06() override;
};

class CParticle;

class CFullScreenEffect : public CEffect {
public:
  CFullScreenEffect();
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ unsigned char loaded;
  /* 00D9 */ unsigned char unknown_00D9[3];
};

class CPetitioEffect : public CEffect {
public:
  CPetitioEffect();
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ std::vector<EZ2STREmitter*> emitters;
};

class CPneumaticusProcellaEffect : public CEffect {
public:
  CPneumaticusProcellaEffect();
  void Slot04() override;

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ std::vector<EZ2STREmitter*> emitters;
};

class CRandomHitEffect : public CEffect {
public:
  CRandomHitEffect();
  void Slot04() override;

  /* 00CC */ bool active;
  /* 00CD */ unsigned char unknown_00CD[3];
  /* 00D0 */ EZ2STREmitter* emitter;
};

class CRandomOptEffect : public CEffect {
public:
  CRandomOptEffect();
  void Slot08(int red, int green, int blue) override;
  void Slot11(std::string name) override;

  /* 00CC */ vector3d offsets[2];
  /* 00E4 */ std::vector<CParticle*> particles;
};

class CRichsCoinEffect : public CEffect {
public:
  CRichsCoinEffect();
  bool Slot06() override;

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

  /* 00CC */ EZ2STREmitter* emitter;
  /* 00D0 */ vector3d offset;
};

class CServantWeaponEffect : public CEffect {
public:
  CServantWeaponEffect();
  bool Slot06() override;

  /* 00CC */ vector3d offsets[5];
  /* 0108 */ EZ2STREmitter* emitters[3][5];
};

class CServantWeaponNewEffect : public CEffect {
public:
  CServantWeaponNewEffect();
  bool Slot06() override;
  void Slot01() override;

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ std::list<EZ2STREmitter*> trails;
};

class CSevereRainStorm : public CEffect {
public:
  CSevereRainStorm();

  /* 00CC */ std::list<EZ2STREmitter*> emitters;
  /* 00D4 */ int unknown_00D4;
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
static_assert(sizeof(CFloralFlareRoad) == 0xDC);
static_assert(offsetof(CFootprintEffect, stepDistance) == 0x108);
static_assert(offsetof(CFootprintEffect, lastPosition) == 0x114);
static_assert(sizeof(CFootprintEffect) == 0x128);
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
#endif

#endif
