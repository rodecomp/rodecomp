#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECT_H

#include "Contents/Effects/RagEffectState.h"
#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>

struct EffectActor;

struct CRagEffect : CRenderObject {
  CRagEffect();
  ~CRagEffect() override;

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
  virtual void Slot18();

  /* 0138 */ RagEffectState state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRagEffect) == 0x11CA8);
static_assert(offsetof(CRagEffect, state) == 0x138);
#endif

#endif
