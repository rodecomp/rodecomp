#ifndef RAGNAROK_SOURCE_RESOURCE_EZEFFECTRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_EZEFFECTRESOURCE_H

#include "File/File.h"
#include "Resource/Resource.h"

#include <cstddef>
#include <cstdint>

class CTexture;

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

class CEZeffectRes : public CRes {
public:
  CEZeffectRes();
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
  /* D320 */ std::uint32_t unknown_D320;
  /* D324 */ CFile m_file;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EZeffectClip) == 896);
static_assert(offsetof(EZeffectClip, names) == 0x10);
static_assert(offsetof(EZeffectClip, textures) == 0x1C8);
static_assert(sizeof(CEZeffectRes) == 0xD350);
#endif

#endif
