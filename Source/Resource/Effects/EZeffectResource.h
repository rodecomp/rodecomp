#ifndef RAGNAROK_SOURCE_RESOURCE_EFFECTS_EZEFFECTRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_EFFECTS_EZEFFECTRESOURCE_H

#include "File/File.h"
#include "Resource/Resource.h"

#include <cstddef>
#include <cstdint>
#include <string>

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
  CTexture* Texture(int index, const std::string& prefix);

  /* 0000 */ int textureCount;
  /* 0004 */ int textureState;
  /* 0008 */ int frameCount;
  /* 000C */ const EZeffectKeyFrame* frames;
  /* 0010 */ const char* names[110];
  /* 01C8 */ CTexture* textures[110];
};

struct EZeffectData {
  /* 0000 */ int maximumFrame;
  /* 0004 */ int fps;
  /* 0008 */ int clipCount;
  /* 000C */ int endLayerCount;
  /* 0010 */ EZeffectClip clips[60];
};

class CEZeffectRes : public CRes {
public:
  CEZeffectRes();
  CEZeffectRes(const char* extension, const char* prefix);
  ~CEZeffectRes() override;
  CEZeffectRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ EZeffectData data;

private:
  /* D320 */ std::uint32_t loadState;
  /* D324 */ CFile m_file;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EZeffectTransform) == 116);
static_assert(sizeof(EZeffectKeyFrame) == 124);
static_assert(sizeof(EZeffectClip) == 896);
static_assert(offsetof(EZeffectClip, names) == 0x10);
static_assert(offsetof(EZeffectClip, textures) == 0x1C8);
static_assert(sizeof(EZeffectData) == 0xD210);
static_assert(offsetof(EZeffectData, clips) == 0x10);
static_assert(offsetof(CEZeffectRes, data) == 0x110);
static_assert(sizeof(CEZeffectRes) == 0xD350);
#endif

#endif
