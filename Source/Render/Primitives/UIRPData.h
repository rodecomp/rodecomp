#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_UIRPDATA_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_UIRPDATA_H

#include "Render/Primitives/RenderPrimitive.h"

#include <list>
#include <string>

class CTexture;
class RenderPrimitivePool;
class TextureCache;

class UIRPData {
public:
  UIRPData();
  virtual ~UIRPData();
  void Draw(RenderPrimitivePool& pool, int x, int y, CTexture* texture, std::uint32_t color, int width, int height, bool stretched);
  void DrawStretched(RenderPrimitivePool& pool, int x, int y, CTexture* texture, std::uint32_t color, int width, int height);
  void DrawNamed(RenderPrimitivePool& pool, TextureCache& textures, int x, int y, std::string name, std::uint32_t color, int width, int height);
  void SetSolid(RenderPrimitivePool& pool, int width, int height, std::uint32_t color);

protected:
  /* 0004 */ std::list<RenderPrimitive*> m_primitives;
  /* 000C */ CTexture* m_texture;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRPData) == 16);
#endif

#endif
