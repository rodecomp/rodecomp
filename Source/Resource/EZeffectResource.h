#ifndef RAGNAROK_SOURCE_RESOURCE_EZEFFECTRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_EZEFFECTRESOURCE_H

#include "File/File.h"
#include "Resource/Resource.h"

#include <cstddef>
#include <cstdint>

class CTexture;

struct EZeffectTransform {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float u;
  /* 000C */ float v;
  /* 0010 */ float us;
  /* 0014 */ float vs;
  /* 0018 */ float u2;
  /* 001C */ float v2;
  /* 0020 */ float us2;
  /* 0024 */ float vs2;
  /* 0028 */ float ax[4];
  /* 0038 */ float ay[4];
  /* 0048 */ float aniframe;
  /* 004C */ std::uint32_t anitype;
  /* 0050 */ float anidelta;
  /* 0054 */ float rz;
  /* 0058 */ float crR;
  /* 005C */ float crG;
  /* 0060 */ float crB;
  /* 0064 */ float crA;
  /* 0068 */ std::uint32_t srcalpha;
  /* 006C */ std::uint32_t destalpha;
  /* 0070 */ std::uint32_t mtpreset;
};

struct EZeffectKeyFrame {
  /* 0000 */ int frame;
  /* 0004 */ std::uint32_t type;
  /* 0008 */ EZeffectTransform transform;
};

struct EZeffectClip {
  EZeffectClip();
  ~EZeffectClip();
  CTexture* Texture(int index);
  CTexture* Texture(int index, const char* prefix);

  /* 0000 */ int textureCount;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int frameCount;
  /* 000C */ const EZeffectKeyFrame* frames;
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
static_assert(sizeof(EZeffectTransform) == 116);
static_assert(sizeof(EZeffectKeyFrame) == 124);
static_assert(sizeof(EZeffectClip) == 896);
static_assert(offsetof(EZeffectClip, names) == 0x10);
static_assert(offsetof(EZeffectClip, textures) == 0x1C8);
static_assert(sizeof(CEZeffectRes) == 0xD350);
#endif

#endif
