#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_UIRPDATA_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_UIRPDATA_H

#include "Render/Primitives/RenderPrimitive.h"

#include <list>

class CTexture;
class RenderPrimitivePool;

class UIRPData {
public:
  UIRPData();
  virtual ~UIRPData();
  void Draw(RenderPrimitivePool& pool, int x, int y, CTexture* texture, std::uint32_t color, int width, int height, bool stretched);

protected:
  /* 0004 */ std::list<RenderPrimitive*> m_primitives;
  /* 000C */ CTexture* m_texture;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRPData) == 16);
#endif

#endif
