#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECT_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

struct EffectEvent {
  /* 0000 */ int kind;
  /* 0004 */ int frame;
  /* 0008 */ int argument;
  /* 000C */ std::string text;
};

class CEffect {
public:
  explicit CEffect(int id);
  virtual ~CEffect();
  virtual void Close();
  void AddEvent(int kind, const char* text, int frame, int argument);
  void SetEventFrame(int kind, int frame);

  /* 0004 */ int id;
  /* 0008 */ float position[3];
  /* 0014 */ float sourcePosition[3];
  /* 0020 */ std::uint32_t actor;
  /* 0024 */ std::uint32_t sourceActor;
  /* 0028 */ int frame;
  /* 002C */ int unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ unsigned char outsideRange;
  /* 0035 */ unsigned char unknown_0035[3];
  /* 0038 */ int range;
  /* 003C */ unsigned char unknown_003C[116];
  /* 00B0 */ int unknown_00B0;
  /* 00B4 */ std::vector<EffectEvent> events;
  /* 00C0 */ unsigned char retained;
  /* 00C1 */ unsigned char unknown_00C1;
  /* 00C2 */ unsigned char forceRender;
  /* 00C3 */ unsigned char unknown_00C3;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ unsigned char unknown_00C8;
  /* 00C9 */ unsigned char unknown_00C9[3];
};

struct EffectRetirement {
  std::list<CEffect*>& effects;
  void Clear();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectEvent) == 36);
static_assert(offsetof(EffectEvent, text) == 0xC);
static_assert(sizeof(CEffect) == 204);
static_assert(offsetof(CEffect, events) == 0xB4);
static_assert(offsetof(CEffect, unknown_00C8) == 0xC8);
#endif

#endif
