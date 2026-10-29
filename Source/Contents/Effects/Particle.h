#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLE_H

#include <cstddef>
#include <string>

class CTexture;
struct RenderVertex;

class CParticle {
public:
  virtual ~CParticle();
  virtual void Slot01() {}

  void SetQuad(RenderVertex* vertices, float top, float bottom, float left, float right);
  void SetColor(short redValue, short greenValue, short blueValue);
  void SetTexture(const char* name);
  void SetTexture(std::string name);

  /* 0004 */ unsigned char unknown_0004[0x14];
  /* 0018 */ CTexture* texture;
  /* 001C */ unsigned char unknown_001C[0x20];
  /* 003C */ short red;
  /* 003E */ short green;
  /* 0040 */ short blue;
  /* 0042 */ unsigned char unknown_0042[2];
  /* 0044 */ int alpha;
};

void InitParticleQuad(RenderVertex* vertices);
float RandomRange(float minimum, float maximum);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CParticle, texture) == 0x18);
static_assert(offsetof(CParticle, red) == 0x3C);
static_assert(offsetof(CParticle, alpha) == 0x44);
#endif

#endif
