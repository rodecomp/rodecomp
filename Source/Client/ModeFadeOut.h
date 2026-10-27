#ifndef RAGNAROK_SOURCE_CLIENT_MODEFADEOUT_H
#define RAGNAROK_SOURCE_CLIENT_MODEFADEOUT_H

#include <cstdint>

class CRenderer;
class CRenderSystemBase;
class TextureCache;
class ResourceMgr;
struct TextureNames;

std::uint32_t FadeOutMode(CRenderer& renderer, CRenderSystemBase& system, TextureCache& textures, ResourceMgr& resources, TextureNames& names,
  int showLogo, bool depthAlpha, float depthStart, float depthEnd);

#endif
