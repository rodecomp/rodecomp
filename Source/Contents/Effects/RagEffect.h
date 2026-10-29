#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECT_H

#include "Contents/Effects/RagEffectState.h"
#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>

struct EffectActor;
class TextureCache;
struct RenderProjection;

struct CRagEffect : CRenderObject {
  CRagEffect();
  ~CRagEffect() override;

  void InitEZ2STRFrame();
  bool ProcessEZ2STR(float x, float y, float z);
  void Fireworks();
  bool ProcessEZ2STRC10830();
  bool ProcessEZ2STRC153A0();
  bool ProcessEZ2STRC3CC40();
  bool ProcessEZ2STRC3FE00();
  bool ProcessEZ2STRC65300();
  bool ProcessEZ2STRC15990();
  bool ProcessEZ2STRC159D0();
  bool ShieldCharge();
  void GrimToothAtk(TextureCache& textures);
  void Snow();
  void Entry(TextureCache& textures);
  void AttackEnergy2(TextureCache& textures, const char* name);
  bool LexDivina();
  void RemoveTrap();
  void Sphere();
  bool Aspersio();
  void Blessing(TextureCache& textures);
  void Bottom(TextureCache& textures);
  void BloodDrain();
  void FireBall();
  void FrostDiver(TextureCache& textures, char type);
  void GrimTooth(TextureCache& textures);
  void Hit4(const RenderProjection& projection, TextureCache& textures);
  void SoulStrike(char F1);
  void SpearBmr();
  void SprayPond(TextureCache& textures);
  void WaterBall2();
  void Yufitel(TextureCache& textures, char F1);
  void AirTexture(TextureCache& textures, const char* name);
  void EnergyDrain2();

  virtual void Slot08();
  virtual void Slot09(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth, int eleventh, int twelfth, int thirteenth, int fourteenth, int fifteenth, int sixteenth, int seventeenth);
  virtual void Slot10(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth);
  virtual void Slot11(int first, int second, int third, int fourth, int fifth, int sixth, int seventh, int eighth, int ninth, int tenth);
  virtual void Slot12(int first, int second, int third, int fourth, int fifth);
  virtual void Slot13();
  virtual void Slot14();
  virtual void Slot15();
  virtual void SendMsg(CGameObject* sender, int message, int first, std::int64_t second, int third, int fourth, int fifth, int sixth);
  virtual void Init(EffectActor* parent, int id, vector3d deltaPos);
  virtual int GetDuration();

  /* 0138 */ RagEffectState state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRagEffect) == 0x11CA8);
static_assert(offsetof(CRagEffect, state) == 0x138);
#endif

#endif
