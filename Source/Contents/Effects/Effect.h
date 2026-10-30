#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECT_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

struct vector3d;

struct EffectEvent {
  /* 0000 */ int kind;
  /* 0004 */ int frame;
  /* 0008 */ int argument;
  /* 000C */ std::string text;
};

class CEffect {
public:
  explicit CEffect(int id);
  virtual void Slot00(int argument);
  virtual void Slot01();
  virtual void Slot02();
  virtual void Slot03() {}
  virtual void Slot04();
  virtual void Close();
  virtual bool Slot06();
  virtual void Slot07();
  virtual void Slot08(int argument1, int argument2, int argument3);
  virtual void Slot09(int argument);
  virtual void Slot10(int argument);
  virtual void Slot11(std::string name);
  virtual void Slot12();
  virtual void Slot13(int argument);
  virtual void Slot14(int argument);
  virtual void Slot15(int argument);
  virtual void Slot16(int) {}
  virtual void Slot17(float argument);
  virtual void Slot18(float argument);
  virtual void Slot19(float argument);
  virtual void Slot20(int argument);
  virtual void Slot21(int argument);
  virtual void Slot22(float argument);
  virtual void Slot23(float argument);
  virtual void Slot24(int argument1, int argument2, int argument3, int argument4, int argument5);
  virtual vector3d Slot25();
  virtual void Slot26(int argument);
  virtual void Slot27(int argument);
  virtual int Slot28();
  virtual void Slot29(int id);
  virtual ~CEffect();
  virtual void Slot31();
  virtual void Slot32();
  virtual bool Slot33();
  void AddEvent(int kind, const char* text, int frame, int argument);
  void SetActor(std::uint32_t aid);
  void SetSourceActor(std::uint32_t aid);
  void SetEventFrame(int kind, int frame);
  bool RetireIfInactive();
  bool ActorExists() const;
  bool IsFinished() const;
  void UpdateVisibility();
  void Update(std::uint32_t aid);
  bool InRange();

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
  /* 003C */ unsigned char unknown_003C[12];
  /* 0048 */ int unknown_0048;
  /* 004C */ unsigned char unknown_004C[4];
  /* 0050 */ int unknown_0050;
  /* 0054 */ unsigned char unknown_0054[92];
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
