#ifndef RAGNAROK_SOURCE_CLIENT_MODEFADEOUT_H
#define RAGNAROK_SOURCE_CLIENT_MODEFADEOUT_H

#include <cstdint>

class CRenderer;
class CRenderSystemBase;
class TextureCache;
class ResourceMgr;

std::uint32_t FadeOutMode(CRenderer& renderer, CRenderSystemBase& system, ResourceMgr& resources,
  int showLogo, bool depthAlpha, float depthStart, float depthEnd);

#endif
