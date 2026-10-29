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
  void GrimToothAtk();
  void Snow();
  void Entry();
  bool CartRevolution();
  void BowlingBash();
  void SonicBlowHit();
  void Exit();
  void Warp();
  __declspec(noinline) void PotionPillar(float heightSpeed, int duration);
  bool Potion_Berserk();
  void AttackEnergy2(const char* name);
  bool LexDivina();
  void RemoveTrap();
  void Sphere();
  bool Aspersio();
  void Blessing();
  void Bottom();
  void BloodDrain();
  void FireBall();
  void FrostDiver(char type);
  void GrimTooth();
  void Hit4(const RenderProjection& projection);
  void SoulStrike(char F1);
  void SpearBmr();
  void SprayPond();
  void WaterBall2();
  void Yufitel(char F1);
  void AirTexture(const char* name);
  void EnergyDrain2();
  void TexturedSlashLevel(const char* name, char kind);
  void TexturedRain(const char* name, char kind);
  void TexturedCrossFade(const char* name);
  void TexturedCrossLevel(const char* name, char level);
  void TexturedCross(const char* name, char kind);
  void TexturedOrbit(const char* name);
  void TexturedSlashCross(const char* name);
  void TexturedHalo(const char* name, char kind);
  void TexturedFlare(const char* name, char size);
  void TexturedSwirl(unsigned char clockwise, const char* name);
  void TexturedBeams(const char* name, char flag);
  void TexturedSlashWave(const char* name, char kind);
  void TexturedSprayKind(const char* name, float spin, char kind);
  void TexturedSparkle(const char* name);
  void TexturedColumn(const char* name, char kind);
  void KyrieGuard(const char* name, char kind);
  void TexturedSlam(const char* name, std::int16_t kind);
  void FeatherRing(const char* name, std::int16_t angle, char large);
  void FeatherBurst(const char* name, std::int16_t angle);
  void SlashFan(std::int16_t reverse);
  void TexturedLevelRing(const char* name, int process, char level);
  void TexturedSpray(const char* name);
  void TexturedMark(const char* name, char kind);
  void TexturedSlashFan(const char* name);
  void TrackCloud(char kind);
  void TrackTargetWide();
  void TrackTarget();
  void TexturedQuadSpin(const char* name, char phase, char kind);
  void TexturedDoubleRing(const char* name, char level);
  void WaterOut();
  void BlackMaximize(const char* name);
  void TexturedPair(const char* name);
  void PurpleSlash(std::int16_t angle);
  void TexturedDrop(const char* name, char flag);
  void TexturedDualSpin(const char* first, const char* second);
  void TexturedScatterLow(const char* name, char flag);
  void TexturedScatter(const char* name, char flag);
  void TexturedSpiralFade(const char* name);
  void TexturedSpiralTall(const char* name);
  void TexturedSpiralSmall(const char* name);
  void TexturedBeam(char flag, const char* name);
  void ColoredFlash(unsigned char flag, unsigned char red, unsigned char green, unsigned char blue);
  void TexturedSpin(const char* name);
  void TexturedSpiral(const char* name);
  void TexturedRise(const char* name, char large);
  void TexturedBurst(const char* name);
  void RingStack(char white);
  void TexturedPulse(const char* name);
  void ShockwaveRing(std::int16_t angle);
  void GROUNDSAMPLE(TextureCache& textures);
  void PotionPillar(TextureCache& textures, float heightSpeed, int duration);
  void CALLZONE(TextureCache& textures, const char* name);
  bool JobLvUp50_2(TextureCache& textures, short angle);
  bool RunStop(TextureCache& textures, short angle);
  void SMA(char flag);
  bool JobLvUp50(TextureCache& textures, int angle);
  void Map_Pika(TextureCache& textures, const char* name);
  void POISON(TextureCache& textures, char flag);
  void Rainbow(TextureCache& textures, const char* name);
  void BLIND(TextureCache& textures, char flag);

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
