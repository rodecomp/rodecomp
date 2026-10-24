#ifndef RAGNAROK_SOURCE_CONTENTS_RENDEROBJECT_H
#define RAGNAROK_SOURCE_CONTENTS_RENDEROBJECT_H

#include "Contents/GameObject.h"
#include "Contents/RenderObjectState.h"

// Recovered CRenderObject lifecycle. Unrecovered virtual operations are not
// replaced here; explicit recovered operations remain on their real owners.
class CRenderObject : public CGameObject {
public:
  CRenderObject(ResourceMgr& resources, JobNames& names);
  ~CRenderObject() override;
  bool Update() override;
  void SendObjectMessage(
    CGameObject* sender,
    int message,
    int a,
    int b,
    int c,
    int d,
    int e,
    int f,
    int g,
    int h,
    int i,
    int j,
    int k,
    int l,
    int m
  ) override;

  // Vtable slot 5. The base keeps no state; derived actors use the projected
  // bounds and forward the screen point to SetScreenPosition.
  virtual void SetScreenBounds(const float* bounds, float x, float y);

  // Vtable slot 6. Stores the truncated projected screen point.
  virtual void SetScreenPosition(float x, float y);

  RenderObjectState render;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRenderObject) == 0x138);
#endif

#endif
