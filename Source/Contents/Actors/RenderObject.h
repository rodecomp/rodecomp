#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H

#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

struct SpriteFrameCell;
struct SpriteQuad;
struct SpriteRleImage;

struct RenderBounds {
  /* 0000 */ float left;
  /* 0004 */ float top;
  /* 0008 */ float right;
  /* 000C */ float bottom;
  /* 0010 */ float offset;

  void Set(float left, float top, float right, float bottom, float offset);
  void Include(const float* box);
};

class CRenderObject : public CGameObject {
public:
  CRenderObject();
  ~CRenderObject() override;
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  void Render(int argument) override;

  virtual void SetRenderInfo(const float* bounds, float x, float y);

  virtual void SetTlvert(float x, float y);
  virtual void SetAction(unsigned animation, int motion, int repeat);
  static float __stdcall Heading(float x, float z, float targetX, float targetZ);
  void Face(float x, float z);
  float CalcDist(float x, float z) const;
  float CalcDist(float x, float y, float z) const;
  bool GetTargetPos(std::uint32_t targetAid, vector3d& targetPos) const;
  void UpdateTlvertex(const float* matrix);
  void SetAnimationSpeed(float speed);
  void SetActionSpeed(float speed);
  void DrawSprite(int argument, bool seasonal, float offset);
  void DrawShadow(int argument, float scale);
  void DrawQuad(const SpriteQuad& quad, const std::uint32_t* palette, int blend, float offsetX, float offsetY, int depthOffset);
  void DrawFlippedQuad(SpriteQuad& quad, const std::uint32_t* palette, int blend, float x, float y, int depthOffset);
  void DrawStatusOverlays(SpriteQuad quad, const std::uint32_t* palette);
  void DrawTintedQuad(SpriteQuad& quad, short red, short green, short blue, const std::uint32_t* palette);
  void SendValueMessage(int value, CGameObject* target);
  void SetActionName(ResourceMgr& resources, const char* name);
  void SetSpriteName(ResourceMgr& resources, const char* name);
  void SetColor(int alpha, int red, int green, int blue);
  void ModifyArgb(int alpha, int red, int green, int blue);
  void SetSequenceDeadline(std::uint32_t duration);
  void SetSequenceInterval(unsigned char interval);
  void SetAnimationSequence(
    unsigned char animation0, unsigned char frame0, unsigned char animation1, unsigned char frame1,
    unsigned char animation2, unsigned char frame2, unsigned char animation3, unsigned char frame3,
    unsigned char animation4, unsigned char frame4, int timed, std::uint32_t duration, int loop
  );
  int HoverHeight();
  void ScaleByPulse(float& scale) const;
  void ScaleByFadePulse(float& scale) const;
  void DrawBlurCopies(
    const SpriteFrameCell& cell, const int* box, float offset, float bottomRhw, float topRhw, const std::uint32_t* palette,
    const SpriteRleImage* image, float scale, short shift
  );
  void JitterBounds(float* output, const int* input, float offset, int spread, int base);
  void DrawJitterTrail(
    SpriteQuad& quad, const SpriteFrameCell& cell, const int* box, float offset, float bottomRhw, float topRhw,
    const std::uint32_t* palette, const SpriteRleImage* image, int blend, short shift
  );
  void DrawPulseOutline(
    const SpriteFrameCell& cell, const int* box, float offset, float bottomRhw, float topRhw,
    const std::uint32_t* palette, const SpriteRleImage* image, std::uint32_t color
  );
  void ApplySwing(int mode, int& x, int& y, int& outX, int& outY) const;
  void SetInstantValue(int key, std::int64_t value);
  std::int64_t GetInstant(int key) const;
  CActRes* GetActRes();
  void SetFrameSequence(
    unsigned char animation, unsigned char firstFrame, unsigned char lastFrame, unsigned char finishAnimation,
    unsigned char finishFrame, int timed, std::uint32_t duration, int loop
  );
  void ClearLevelEffects();

  /* 0010 */ RenderObjectState render;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderBounds) == 0x14);
static_assert(sizeof(CRenderObject) == 0x138);
static_assert(offsetof(CRenderObject, render) == 0x0010);
#endif

#endif
