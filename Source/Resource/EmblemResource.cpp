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

// OFFSET: 0x0074CA90

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
