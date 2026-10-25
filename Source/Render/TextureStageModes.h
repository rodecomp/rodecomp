#ifndef RAGNAROK_SOURCE_RENDER_TEXTURESTAGEMODES_H
#define RAGNAROK_SOURCE_RENDER_TEXTURESTAGEMODES_H

#include <span>
#include <windows.h>

struct TextureStageWrite {
  unsigned stage;
  unsigned state;
  DWORD value;
};

std::span<const TextureStageWrite> TextureStageModeWrites(int mode);

#endif
