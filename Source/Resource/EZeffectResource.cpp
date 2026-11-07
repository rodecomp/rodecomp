#include "Client/Application/ErrorDialog.h"
#include "Render/Textures/Texture.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/EZeffectResource.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"

#include <algorithm>
#include <format>
#include <iterator>
#include <string>

// OFFSET: 0x0074D640
// FUNCTION: fn_07894
EZeffectClip::EZeffectClip() : textureCount(0), unknown_0004(0), frameCount(0), frames(nullptr) {
  const char* const defaultName = "1.bmp";
  std::fill_n(this->names, std::size(this->names), defaultName);
  std::fill_n(this->textures, std::size(this->textures), nullptr);
}

// OFFSET: 0x0074D790
// FUNCTION: fn_07896
EZeffectClip::~EZeffectClip() {
  for (CTexture* texture : this->textures) {
    if (texture) {
      texture->Release();
    }
  }
}

// OFFSET: 0x0074D570
// FUNCTION: fn_07893
CEZeffectRes::CEZeffectRes() {
  this->unknown_D320 = 0;
}

// OFFSET: 0x0074D6F0
// FUNCTION: fn_07895
CEZeffectRes::~CEZeffectRes() = default;

// OFFSET: 0x0074CEE0
// FUNCTION: fn_07889
CEZeffectRes* CEZeffectRes::Create() {
  return new CEZeffectRes;
}

// OFFSET: 0x0074CFF0
// FUNCTION: fn_07890
bool CEZeffectRes::Load(const char* name) {
  auto& file = this->m_file;

  if (!file.Open(name, 0, true, false)) {
    ShowErrorDialog((std::string("Str::Cannot find File : ") + std::string(name)).c_str(), 0);
    this->Unload();

    return false;
  }

  unsigned char magic[4];
  int version;
  file.Read(magic, sizeof(magic));
  file.Read(&version, sizeof(version));
  file.Read(&this->fps, sizeof(this->fps));
  file.Read(&this->maximumFrame, sizeof(this->maximumFrame));
  file.Read(&this->clipCount, sizeof(this->clipCount));
  file.Seek(16, FILE_CURRENT);

  if (version != 148) {
    ShowErrorDialog("LoadAniClip: Version is incorrect", 0);
    this->Unload();
    return false;
  }

  for (int index = 0; index < this->clipCount; ++index) {
    EZeffectClip& clip = this->clips[index];
    file.Read(&clip.textureCount, sizeof(clip.textureCount));

    for (int texture = 0; texture < clip.textureCount; ++texture) {
      clip.names[texture] = static_cast<const char*>(file.ReadPtr(128));
    }

    file.Read(&clip.frameCount, sizeof(clip.frameCount));

    if (clip.frameCount > 0) {
      clip.frames = static_cast<const EZeffectKeyFrame*>(file.ReadPtr(124u * static_cast<unsigned>(clip.frameCount)));
    }
  }

  return true;
}

// OFFSET: 0x0074D320
// FUNCTION: fn_07891
void CEZeffectRes::Unload() {
  this->m_file.Close();
}

// OFFSET: 0x0074D850
// FUNCTION: fn_07897
CTexture* EZeffectClip::Texture(int index) {
  if (index >= this->textureCount) {
    return g_texMgr.Default();
  }

  if (!this->textures[index]) {
    std::string path = std::format("effect\\{}", this->names[index]);

    if (path.find_last_of('.') == std::string::npos) {
      path.append(".bmp", 4);
    }

    this->textures[index] = g_texMgr.Load(path.c_str(), 1, false, true, false);

    if (this->textures[index]) {
      this->textures[index]->Retain();
    }
  }

  return this->textures[index];
}

// OFFSET: 0x0074D9A0
// FUNCTION: fn_07898
CTexture* EZeffectClip::Texture(int index, const char* prefix) {
  if (index >= this->textureCount) {
    return g_texMgr.Default();
  }

  if (!this->textures[index]) {
    std::string path = std::string("effect\\") + prefix + this->names[index];

    if (path.find_last_of('.') == std::string::npos) {
      path.append(".bmp", 4);
    }

    this->textures[index] = g_texMgr.Load(path.c_str(), 1, false, true, false);

    if (this->textures[index]) {
      this->textures[index]->Retain();
    }
  }

  return this->textures[index];
}

// INLINED
CEZeffectRes::CEZeffectRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25555
CEZeffectRes g_strPrototype("str", "texture\\effect\\");
