#ifndef RAGNAROK_SOURCE_RENDER_ACTIONSPRITEDRAWING_H
#define RAGNAROK_SOURCE_RENDER_ACTIONSPRITEDRAWING_H

#include <cstdint>

class CActRes;
class CRenderSystemBase;
class CSprRes;

void DrawActionSprite(
  CRenderSystemBase& system, int x, int y, const CActRes* action, const CSprRes* sprite, int animation, int frame, float scale, float angle,
  std::uint32_t color, std::uint16_t flags
);

#endif
