#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECT_H

#include <cstdint>
#include <list>
#include <string>
#include <vector>

struct EffectEvent {
  int kind;
  int frame;
  int argument;
  std::string text;
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
  unsigned char unknown_0035[3];
  /* 0038 */ int range;
  unsigned char unknown_003C[116];
  /* 00B0 */ int unknown_00B0;
  /* 00B4 */ std::vector<EffectEvent> events;
  /* 00C0 */ unsigned char retained;
  unsigned char unknown_00C1;
  /* 00C2 */ unsigned char forceRender;
  unsigned char unknown_00C3;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ unsigned char unknown_00C8;
  unsigned char unknown_00C9[3];
};

struct EffectRetirement {
  std::list<CEffect*>& effects;
  void Clear();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectEvent) == 36);
static_assert(sizeof(CEffect) == 204);
#endif

#endif
