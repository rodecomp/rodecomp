#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H

#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

class CRenderObject : public CGameObject {
public:
  CRenderObject(ResourceMgr& resources, JobNames& names);
  ~CRenderObject() override;
  bool Update() override;
  void SendObjectMessage(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  void Draw(int argument) override;

  virtual void SetScreenBounds(const float* bounds, float x, float y);

  virtual void SetScreenPosition(float x, float y);
  virtual void SetAnimation(unsigned animation, int motion, int repeat);
  static float __stdcall Heading(float x, float z, float targetX, float targetZ);
  void Face(float x, float z);
  float HorizontalDistance(float x, float z) const;
  float Distance(float x, float y, float z) const;
  bool GetTargetPos(std::uint32_t targetAid, vector3d& targetPos) const;
  void SetAnimationSpeed(float speed);
  void SetActionSpeed(float speed);
  void DrawSprite(int argument, bool seasonal, float offset);
  void DrawShadow(int argument, float scale);
  void SendValueMessage(int value, CGameObject* target);
  void SetActionName(ResourceMgr& resources, const char* name);
  void SetSpriteName(ResourceMgr& resources, const char* name);
  void SetColor(int alpha, int red, int green, int blue);
  void SetSequenceDeadline(std::uint32_t duration);
  void SetSequenceInterval(unsigned char interval);
  void SetAnimationSequence(
    unsigned char animation0, unsigned char frame0, unsigned char animation1, unsigned char frame1,
    unsigned char animation2, unsigned char frame2, unsigned char animation3, unsigned char frame3,
    unsigned char animation4, unsigned char frame4, int timed, std::uint32_t duration, int loop
  );
  int HoverHeight();
  void ScaleByPulse(float& scale) const;
  void JitterBounds(float* output, const int* input, float offset, int spread, int base);
  void ApplySwing(int mode, int& x, int& y, int& outX, int& outY) const;
  void SetInstantValue(int key, std::int64_t value);
  std::int64_t GetInstant(int key) const;
  void SetFrameSequence(
    unsigned char animation, unsigned char firstFrame, unsigned char lastFrame, unsigned char finishAnimation,
    unsigned char finishFrame, int timed, std::uint32_t duration, int loop
  );

  /* 0010 */ RenderObjectState render;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRenderObject) == 0x138);
static_assert(offsetof(CRenderObject, render) == 0x0010);
#endif

#endif
