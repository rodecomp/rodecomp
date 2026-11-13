#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_EFFECT_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

struct vector3d;

struct EffectEvent {
  /* 0000 */ int kind;
  /* 0004 */ int frame;
  /* 0008 */ float argument;
  /* 000C */ std::string text;
};

class CEffect {
public:
  explicit CEffect(int id);
  virtual void SetRenderFunc(int argument);
  virtual void Open();
  virtual void CheckRange();
  virtual void OnProcess() {}
  virtual void Render();
  virtual void Close();
  virtual bool IsAlive();
  virtual void ResetParticles();
  virtual void SetColor(int argument1, int argument2, int argument3);
  virtual void SetSourceBlend(int argument);
  virtual void SetDestinationBlend(int argument);
  virtual void SetParticleTexture(std::string name);
  virtual void InitEvents();
  virtual void AdvanceFrame(int argument);
  virtual void SetLifeTime(int argument);
  virtual void SetRepeat(int argument);
  virtual void SetDelay(int) {}
  virtual void SetAngle(float argument);
  virtual void SetPitch(float argument);
  virtual void SetYaw(float argument);
  virtual void SetResource(int argument);
  virtual void SetFlags(int argument);
  virtual void SetScale(float argument);
  virtual void SetAlpha(float argument);
  virtual void SetWindowOffset(int argument1, int argument2, int argument3, int argument4, int argument5);
  virtual vector3d GetWindowOffset();
  virtual void SetFlipX(int argument);
  virtual void SetFlipY(int argument);
  virtual int GetFrame();
  virtual void SetId(int id);
  virtual ~CEffect();
  virtual void EnableTimeLimit();
  virtual void EnableGroundRender();
  virtual bool IsVisible();
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
  /* 002C */ int effectLifetime;
  /* 0030 */ int effectArgument;
  /* 0034 */ unsigned char outsideRange;
  /* 0035 */ unsigned char padding_0035[3];
  /* 0038 */ int range;
  /* 003C */ unsigned char areaParameters[12];
  /* 0048 */ float scaleParameter;
  /* 004C */ int baseAngle;
  /* 0050 */ float rotationParameter;
  /* 0054 */ int baseAlpha;
  /* 0058 */ int fadeAlpha;
  /* 005C */ float heights[21];
  /* 00B0 */ int effectLevel;
  /* 00B4 */ std::vector<EffectEvent> events;
  /* 00C0 */ unsigned char retained;
  /* 00C1 */ unsigned char emissionStopped;
  /* 00C2 */ unsigned char forceRender;
  /* 00C3 */ unsigned char padding_00C3;
  /* 00C4 */ int instanceId;
  /* 00C8 */ unsigned char headAttached;
  /* 00C9 */ unsigned char padding_00C9[3];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectEvent) == 36);
static_assert(offsetof(EffectEvent, text) == 0xC);
static_assert(sizeof(CEffect) == 204);
static_assert(offsetof(CEffect, events) == 0xB4);
static_assert(offsetof(CEffect, headAttached) == 0xC8);
#endif

#endif
