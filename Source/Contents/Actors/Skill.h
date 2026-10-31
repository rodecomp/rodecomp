#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_SKILL_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_SKILL_H

#include "Contents/Actors/GameActor.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class C3dActor;
struct CRagEffect;

class CSkill : public CGameActor {
public:
  explicit CSkill(std::uint32_t aid);
  ~CSkill() override;
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  bool Slot09(
    int layer, int index, float distanceScale, const SpriteFrameCell& cell, float x, float y, const float* position, float scale,
    float* bottomRhw, float* topRhw, const SpriteRleImage* image, SpriteQuad* quad, int* box, std::uint32_t* color, int offsetX, int offsetY,
    int flags
  ) override;
  void Slot10(
    const SpriteFrameCell* cell, const int* box, float shiftX, float bottomRhw, float topRhw, const std::uint32_t* palette,
    const SpriteRleImage* image, float scale, short hover
  ) override;
  void Slot11(
    SpriteQuad* quad, const SpriteFrameCell* cell, const int* box, float shiftX, float bottomRhw, float topRhw,
    const std::uint32_t* palette, const SpriteRleImage* image, int blend, short hover
  ) override;
  void Slot12(SpriteQuad* quad, const int* box, float shiftX, int first, int second) override;
  void Slot14() override;
  void Slot16() override;
  void Slot29(int value) override;
  void Slot32() override;
  void Slot33(int value) override;
  void Slot36() override;

  const char* GetTrapModelName();
  void ReleaseModel();
  void LaunchGroundEffects(int effectId, int unused, int duration);
  void Render(int argument) override;
  void sub_E55100(const float* box, float x, float y);
  void sub_E55320(const char* modelName);

  /* 0410 */ int unknown_0410;
  /* 0414 */ unsigned m_effectTimer;
  /* 0418 */ std::uint32_t skillAid;
  /* 041C */ C3dActor* m_model;
  /* 0420 */ CRagEffect* m_effect;
  /* 0424 */ int m_effectId;
  /* 0428 */ bool m_hasEffect;
  /* 0429 */ unsigned char unknown_0429[0x03];
  /* 042C */ int range;
  /* 0430 */ std::vector<std::uint64_t> m_targets;
  /* 043C */ int unknown_043C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSkill) == 0x440);
static_assert(offsetof(CSkill, skillAid) == 0x418);
static_assert(offsetof(CSkill, m_hasEffect) == 0x428);
static_assert(offsetof(CSkill, range) == 0x42C);
static_assert(offsetof(CSkill, m_targets) == 0x430);
#endif

#endif
