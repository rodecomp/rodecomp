#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECT_H

#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/RenderObjectState.h"

class CRenderObject : public CGameObject {
public:
  CRenderObject(ResourceMgr& resources, JobNames& names);
  ~CRenderObject() override;
  bool Update() override;
  void SendObjectMessage(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;

  virtual void SetScreenBounds(const float* bounds, float x, float y);

  virtual void SetScreenPosition(float x, float y);

  RenderObjectState render;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRenderObject) == 0x138);
#endif

#endif
