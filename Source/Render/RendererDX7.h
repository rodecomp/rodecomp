#ifndef RAGNAROK_SOURCE_RENDER_RENDERERDX7_H
#define RAGNAROK_SOURCE_RENDER_RENDERERDX7_H

#include "Render/Renderer.h"

class CRendererDX7 : public CRenderer {
public:
  explicit CRendererDX7(CRenderSystemBase& system);

  /* 025C */ unsigned unknown_025C;
  /* 0260 */ unsigned time;
  /* 0264 */ unsigned unknown_0264;
  /* 0268 */ unsigned unknown_0268;
  /* 026C */ unsigned unknown_026C;
  /* 0270 */ unsigned clearColor;
  /* 0274 */ unsigned char unknown_0274[0x10];

  void SetCamera(const float* eye, const float* target, const float* up) override;
  void SetClearColor(unsigned color) override;

  void SetTextureFormat(unsigned format);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRendererDX7) == 0x284);
static_assert(offsetof(CRendererDX7, clearColor) == 0x0270);
#endif

#endif
