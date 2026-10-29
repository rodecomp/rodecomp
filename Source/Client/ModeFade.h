#ifndef RAGNAROK_SOURCE_CLIENT_MODEFADE_H
#define RAGNAROK_SOURCE_CLIENT_MODEFADE_H

#include <cstdint>

class RenderPrimitivePool;
class TextureCache;
struct RenderQueues;

extern std::uint32_t g_modeFadeTick;

struct ModeFade {
  int& requested;
  RenderPrimitivePool& primitives;
  RenderQueues& queues;
  const int& screenWidth;
  const int& screenHeight;
  const bool& depthAlpha;
  const float& depthStart;
  const float& depthEnd;

  void Draw();
};

#endif
