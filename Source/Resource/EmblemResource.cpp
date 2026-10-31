#include "Resource/EmblemResource.h"

#include "Contents/Emblems/EmblemCache.h"
#include "Contents/GuildMgr.h"
#include "File/File.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <zlib.h>

namespace {
  // INLINED
  EmblemCache& GuildEmblemCache() {
    return *reinterpret_cast<EmblemCache*>(g_guildMgr);
  }
}

// INLINED
CEmblemRes::CEmblemRes(BitmapResourceContext& context) : gif(context) {
}

// INLINED
CEmblemRes::~CEmblemRes() = default;

// OFFSET: 0x0074C9F0
// FUNCTION: fn_07880
CEmblemRes* CEmblemRes::Create() {
  return new CEmblemRes(this->gif.Context());
}

// OFFSET: 0x0074CA90
// FUNCTION: fn_07881
bool CEmblemRes::Load(const char* name) {
  if (GuildEmblemCache().IsInvalid(name) == true) {
    this->gif.CreateBlank(24, 24);
    return true;
  }

  CFile file;
  if (!file.Open(name, 0, true, false)) {
    return false;
  }

  uLongf size = static_cast<uLongf>(static_cast<std::uint32_t>(g_guildMgr->m_timeout) * 1.2);
  Bytef* buffer = static_cast<Bytef*>(std::malloc(size));
  if (uncompress(buffer, &size, static_cast<const Bytef*>(file.GetData()), file.GetSize()) != Z_OK
      || this->gif.Decode("_emblem.bmp", buffer, size, true) != true) {
    GuildEmblemCache().MarkInvalid(name);
    this->gif.CreateBlank(24, 24);
  }

  file.Close();
  std::free(buffer);
  return true;
}

// OFFSET: 0x0074CD00
// FUNCTION: fn_07882
void CEmblemRes::Unload() {
  this->gif.Unload();
}

// OFFSET: 0x0074CD10
// FUNCTION: fn_07883
void CEmblemRes::Slot02() {
  this->gif.Slot02();
}

// OFFSET: 0x0074CD20
// FUNCTION: fn_07884
bool CEmblemRes::IsTransient() {
  return this->gif.IsTransient();
}

// OFFSET: 0x0074CD40
// FUNCTION: fn_07885
void CEmblemRes::MissingFile(CRenderSystemBase*, const char*) {
}

// OFFSET: 0x0074CDC0
// FUNCTION: fn_07887
CEmblemRes::CEmblemRes() : CRes(), gif() {
}
