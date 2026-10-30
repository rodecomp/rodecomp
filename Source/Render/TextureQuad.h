#ifndef RAGNAROK_SOURCE_RENDER_TEXTUREQUAD_H
#define RAGNAROK_SOURCE_RENDER_TEXTUREQUAD_H

#include <cstdint>

struct TextureRegion {
  /* 0000 */ float left;
  /* 0004 */ float top;
  /* 0008 */ float right;
  /* 000C */ float bottom;
};

extern float g_depthStart;
extern float g_depthEnd;
extern std::uint32_t g_worldOverlayColor;
extern float g_fogDensity;

float RhwToDepth(float rhw);

#endif
