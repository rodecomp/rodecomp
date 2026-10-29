#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPARTICLE_H

#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObject.h"

#include <list>

class ResourceMgr;
struct AttachedEffect;

struct EffectParticle : CRenderObject {
  EffectParticle();

  bool UpdateFireWall();
  bool UpdateStoneCurse();
  void UpdateBeginSpell();
  bool UpdateHit6();
  bool UpdateCoin();
  bool UpdateDetoxication();
  bool UpdateExit();
  bool UpdateWarp();
  void PrimBottom2();
  void PrimBottom();
  void PrimBottomOut();
  void PrimBubbleDrop();
  void PrimChemical2();
  void PrimChemical3();
  void PrimChemical();
  void PrimEffectTexture2();
  void PrimEffectTexture_Ani();
  void PrimFirstAid();
  void PrimIntimidate();
  void PrimPoison();
  void PrimGuard();
  bool PrimLineLink();
  void PrimSMA();
  void PrimShieldBoomerang3();
  bool PrimSuperAngel();
  void PrimTeiHit2();
  bool PrimSakura();
  bool PrimKouenka();
  void PrimShieldBoomerang();
  bool PrimGhost();
  void PrimThrowItem();
  bool PrimPeong();
  void PrimGI3();
  void PrimWaterFall();

  bool AdvanceFrame();
  bool FollowMasterAndAdvance();
  void FollowMasterAndTickSlots();
  void FollowMasterAndTickSlots2();
  void UpdateRecords();
  void InitializeRecords();

  /* 0138 */ EffectPrimitiveState state;
};

EffectParticle* CreateEffectParticle(
  AttachedEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z,
  std::list<CRenderObject*>& worldObjects
);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectParticle) == 0xFA18);
static_assert(offsetof(EffectParticle, state) == 0x138);
#endif

#endif
