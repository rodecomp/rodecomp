#ifndef RAGNAROK_SOURCE_RENDER_UIRPDATA_H
#define RAGNAROK_SOURCE_RENDER_UIRPDATA_H

#include "Render/RenderPrimitive.h"

#include <list>

class CTexture;
class RenderPrimitivePool;

// Original UIRPData base; commands are borrowed from the renderer pool,
// m_texture is owned. The enclosing window supplies its actual renderer.
class UIRPData {
public:
  UIRPData();
  virtual ~UIRPData();
  void Draw(
    RenderPrimitivePool& pool,
    int x,
    int y,
    CTexture* texture,
    std::uint32_t color,
    int width,
    int height,
    bool stretched
  );

protected:
  /* 0004 */ std::list<RenderPrimitive*> m_primitives;
  /* 000C */ CTexture* m_texture;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRPData) == 16);
#endif

#endif
