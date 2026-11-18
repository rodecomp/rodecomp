#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_UIRPDATA_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_UIRPDATA_H

#include "Render/Primitives/RenderPrimitive.h"

#include <list>
#include <string>

class CTexture;
class TextureCache;

class UIRPData {
public:
  UIRPData();
  virtual ~UIRPData();
  void Draw(int x, int y, CTexture* texture, std::uint32_t color, int width, int height, bool stretched);
  void DrawCroppedTexture(int x, int y, CTexture* texture, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, std::uint32_t color);
  void DrawStretched(int x, int y, CTexture* texture, std::uint32_t color, int width, int height);
  void DrawNamed(int x, int y, std::string name, std::uint32_t color, int width, int height);
  void DrawNamedScaledRegion(float x, float y, std::string name, std::uint32_t color, int width, int height, float scalePercent, int sourceX, int sourceY);
  void SetSolid(int width, int height, std::uint32_t color);

  // INLINED
  const std::list<RenderCommand*>& Primitives() const {
    return this->m_primitives;
  }

protected:
  /* 0004 */ std::list<RenderCommand*> m_primitives;
  /* 000C */ CTexture* m_texture;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRPData) == 16);
#endif

#endif
