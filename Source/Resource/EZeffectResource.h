#ifndef RAGNAROK_SOURCE_RESOURCE_EZEFFECTRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_EZEFFECTRESOURCE_H

#include "Resource/Resource.h"

#include <cstdint>
#include <vector>

class ResourceBytes;
class CTexture;

// Renderer texture owner. Loading keeps its real cache,
// fallback and retain behavior; this resource never fabricates a texture.
class EffectTextures {
public:
  virtual CTexture* Load(const char* name, int format, bool option1, bool option2, bool option3) = 0;
  virtual CTexture* Default() = 0;

protected:
  ~EffectTextures() = default;
};

struct EZeffectClip {
  EZeffectClip();
  ~EZeffectClip();
  CTexture* Texture(int index, EffectTextures& textures);
  CTexture* Texture(int index, const char* prefix, EffectTextures& textures);

  /* 0000 */ int textureCount;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int frameCount;
  /* 000C */ const unsigned char* frames;
  /* 0010 */ const char* names[110];
  /* 01C8 */ CTexture* textures[110];
};

struct EZeffectResourceContext {
  ResourceBytes& file;
  CRenderSystemBase*& renderSystem;
};

// RTTI CEZeffectRes. STR version 148 retains its backing
// allocation; names and 124-byte frame records are borrowed slices, not copies.
class CEZeffectRes : public CRes {
public:
  explicit CEZeffectRes(EZeffectResourceContext& context);
  ~CEZeffectRes() override;
  CEZeffectRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ int maximumFrame;
  /* 0114 */ int fps;
  /* 0118 */ int clipCount;
  /* 011C */ int unknown_011C;
  /* 0120 */ EZeffectClip clips[60];

private:
  // Source replacement for the original retained CFile allocation. Required
  // snapshot: ResourceBytes exposes borrowed bytes but cannot transfer storage.
  std::vector<unsigned char> m_bytes;
  EZeffectResourceContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EZeffectClip) == 896);
#endif

#endif
