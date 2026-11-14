#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H

#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

struct SpriteColor;
struct SpriteFrameCell;
struct SpriteQuad;
struct SpriteRleImage;
struct tagRECT;

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
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;
  void Render(int argument) override;

  virtual void SetRenderInfo(const float* bounds, float x, float y);

  virtual void SetTlvert(float x, float y);
  virtual void SetAction(unsigned animation, int motion, int repeat);
  virtual void ProcessMotion();
  virtual bool PrepareSprite(
    int layer, int index, float distanceScale, const SpriteFrameCell& cell, float x, float y, const float* position, float scale,
    float* bottomRhw, float* topRhw, const SpriteRleImage* image, SpriteQuad* quad, tagRECT* box, std::uint32_t* color, int offsetX, int offsetY,
    int flags
  );
  virtual void RenderBlur(
    const SpriteFrameCell* cell, const tagRECT* box, float shiftX, float bottomRhw, float topRhw, const std::uint32_t* palette,
    const SpriteRleImage* image, float scale, short hover
  );
  virtual void RenderAura(
    SpriteQuad* quad, const SpriteFrameCell* cell, const tagRECT* box, float shiftX, float bottomRhw, float topRhw,
    const std::uint32_t* palette, const SpriteRleImage* image, int blend, short hover
  );
  virtual void ApplySpriteJitter(SpriteQuad* quad, const tagRECT* box, float shiftX, int first, int second);
  // INLINED
  virtual std::uint32_t GetStateFlags() {
    return 0;
  }
  static float __stdcall Heading(float x, float z, float targetX, float targetZ);
  void Face(float x, float z);
  float CalcDist(float x, float z) const;
  float CalcDist(float x, float y, float z) const;
  bool GetTargetPos(std::uint32_t targetAid, vector3d& targetPos) const;
  void UpdateTlvertex(const float* matrix);
  __declspec(noinline) void SetAnimationSpeed(float speed);
  void SetActionSpeed(float speed);
  void DrawSprite(int argument, bool seasonal, float offset);
  void DrawShadow(int argument, float scale);
  void DrawQuad(const SpriteQuad& quad, const std::uint32_t* palette, int blend, float offsetX, float offsetY, int depthOffset);
  void DrawFlippedQuad(SpriteQuad& quad, const std::uint32_t* palette, int blend, float x, float y, int depthOffset);
  void DrawStatusOverlays(SpriteQuad quad, const std::uint32_t* palette);
  void DrawTintedQuad(SpriteQuad& quad, short red, short green, short blue, const std::uint32_t* palette);
  void SendValueMessage(int value, CGameObject* target);
  void SetActionName(const char* name);
  void SetActionName(ResourceMgr& resources, const char* name);
  void SetSpriteName(const char* name);
  void SetSpriteName(ResourceMgr& resources, const char* name);
  void SetColor(int alpha, int red, int green, int blue);
  void ModifyArgb(int alpha, int red, int green, int blue);
  void SetSequenceDeadline(std::uint32_t duration);
  __declspec(noinline) void SetSequenceInterval(unsigned char interval);
  void SetAnimationSequence(
    unsigned char animation0, unsigned char frame0, unsigned char animation1, unsigned char frame1,
    unsigned char animation2, unsigned char frame2, unsigned char animation3, unsigned char frame3,
    unsigned char animation4, unsigned char frame4, int timed, std::uint32_t duration, int loop
  );
  int HoverHeight();
  void ScaleByPulse(float& scale) const;
  void ScaleByFadePulse(float& scale) const;
  __declspec(noinline) void DrawPulseOutline(
    const SpriteFrameCell& cell, const tagRECT* box, float offset, float bottomRhw, float topRhw,
    const std::uint32_t* palette, const SpriteRleImage* image, std::uint32_t color
  );
  void DrawSpriteEffects(
    SpriteQuad* quad, const SpriteFrameCell* cell, const tagRECT* box, float offset, float bottomRhw, float topRhw,
    const std::uint32_t* palette, const SpriteRleImage* image, int* blend, std::uint32_t color
  );
  void ApplySwing(int mode, int& x, int& y, int& outX, int& outY) const;
  void SetInstantValue(int key, std::int64_t value);
  std::int64_t GetInstant(int key) const;
  const char* GetActName();
  __declspec(noinline) CActRes* GetActRes();
  void SetFrameSequence(
    unsigned char animation, unsigned char firstFrame, unsigned char lastFrame, unsigned char finishAnimation,
    unsigned char finishFrame, int timed, std::uint32_t duration, int loop
  );
  void PlayFrameSounds();

  /* 0010 */ RenderObjectState render;
};

extern int g_pickInvert;
extern int g_pickModifierKey;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderBounds) == 0x14);
static_assert(sizeof(CRenderObject) == 0x138);
static_assert(offsetof(CRenderObject, render) == 0x0010);
#endif

#endif
