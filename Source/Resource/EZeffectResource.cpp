#include "Client/Application/ErrorDialog.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/EZeffectResource.h"
#include "Resource/ResourceByteReader.h"

#include <string>

// OFFSET: 0x0074D640
// FUNCTION: fn_07894
EZeffectClip::EZeffectClip() : textureCount(0), unknown_0004(0), frameCount(0), frames(nullptr), textures{} {
  for (const char*& name : this->names) {
    name = "1.bmp";
  }
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

// OFFSET: 0x0074CEE0
// FUNCTION: fn_07889
// OFFSET: 0x0074D570
// FUNCTION: fn_07893
CEZeffectRes::CEZeffectRes(EZeffectResourceContext& context) : m_context(&context) {
}

// OFFSET: 0x0074D6F0
// FUNCTION: fn_07895
// OFFSET: 0x0074D330
// FUNCTION: fn_07892
CEZeffectRes::~CEZeffectRes() = default;

// OFFSET: 0x0074CEE0
// FUNCTION: fn_07889
CEZeffectRes* CEZeffectRes::Create() {
  return new CEZeffectRes(*this->m_context);
}

// OFFSET: 0x0074CFF0
// FUNCTION: fn_07890
bool CEZeffectRes::Load(const char* name) {
  auto& file = this->m_context->file;

  if (!file.Open(name)) {
    const std::string message = std::string("Str::Cannot find File : ") + name;
    ShowErrorDialog(this->m_context->renderSystem, message.c_str(), 0);
    this->Unload();

    return false;
  }

  const auto* data = static_cast<const unsigned char*>(file.Data());
  this->m_bytes.assign(data, data + file.Size());
  file.Close();
  ResourceByteReader input{this->m_bytes.data(), static_cast<unsigned>(this->m_bytes.size())};
  unsigned char magic[4];
  int version;
  input.Read(magic, sizeof(magic));
  input.Read(&version, sizeof(version));
  input.Read(&this->fps, sizeof(this->fps));
  input.Read(&this->maximumFrame, sizeof(this->maximumFrame));
  input.Read(&this->clipCount, sizeof(this->clipCount));
  input.position += 16;

  if (version != 148) {
    ShowErrorDialog(this->m_context->renderSystem, "LoadAniClip: Version is incorrect", 0);
    this->Unload();
    return false;
  }

  for (int index = 0; index < this->clipCount; ++index) {
    EZeffectClip& clip = this->clips[index];
    input.Read(&clip.textureCount, sizeof(clip.textureCount));

    for (int texture = 0; texture < clip.textureCount; ++texture) {
      clip.names[texture] = reinterpret_cast<const char*>(input.data + input.position);
      input.position += 128;
    }

    input.Read(&clip.frameCount, sizeof(clip.frameCount));

    if (clip.frameCount > 0) {
      clip.frames = input.data + input.position;
      input.position += 124u * static_cast<unsigned>(clip.frameCount);
    }
  }

  return true;
}

// OFFSET: 0x0074D320
// FUNCTION: fn_07891
void CEZeffectRes::Unload() {
  std::vector<unsigned char>().swap(this->m_bytes);
}

// OFFSET: 0x0074D850
// FUNCTION: fn_07897
CTexture* EZeffectClip::Texture(int index, EffectTextures& owner) {
  if (index >= this->textureCount) {
    return owner.Default();
  }

  if (!this->textures[index]) {
    std::string path = std::string("effect\\") + this->names[index];

    if (path.find_last_of('.') == std::string::npos) {
      path.append(".bmp", 4);
    }

    this->textures[index] = owner.Load(path.c_str(), 1, false, true, false);

    if (this->textures[index]) {
      this->textures[index]->Retain();
    }
  }

  return this->textures[index];
}

// OFFSET: 0x0074D9A0
// FUNCTION: fn_07898
CTexture* EZeffectClip::Texture(int index, const char* prefix, EffectTextures& owner) {
  if (index >= this->textureCount) {
    return owner.Default();
  }

  if (!this->textures[index]) {
    std::string path = std::string("effect\\") + prefix + this->names[index];

    if (path.find_last_of('.') == std::string::npos) {
      path.append(".bmp", 4);
    }

    this->textures[index] = owner.Load(path.c_str(), 1, false, true, false);

    if (this->textures[index]) {
      this->textures[index]->Retain();
    }
  }

  return this->textures[index];
}
