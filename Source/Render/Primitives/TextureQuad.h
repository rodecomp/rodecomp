#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_TEXTUREQUAD_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_TEXTUREQUAD_H

#include <cstdint>

struct TextureRegion {
  /* 0000 */ float left = 0.0f;
  /* 0004 */ float top = 0.0f;
  /* 0008 */ float right = 0.0f;
  /* 000C */ float bottom = 0.0f;
};

struct UnitTextureRegion : TextureRegion {
  // INLINED
  UnitTextureRegion() {
    this->left = 0.0f;
    this->top = 0.0f;
    this->right = 1.0f;
    this->bottom = 1.0f;
  }
};

extern float g_depthStart;
extern float g_depthEnd;
extern std::uint32_t g_worldOverlayColor;
extern std::uint32_t g_fogVertexMode;
extern std::uint32_t g_fogRange;
extern unsigned char g_multitexture;
extern float g_fogDensity;

float RhwToDepth(float rhw);
float DepthToRhw(float depth);

#endif
