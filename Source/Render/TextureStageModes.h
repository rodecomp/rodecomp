#ifndef RAGNAROK_SOURCE_RENDER_TEXTURESTAGEMODES_H
#define RAGNAROK_SOURCE_RENDER_TEXTURESTAGEMODES_H

#include <span>
#include <windows.h>

// Both original backends issue the same ordered partial stage transitions.
// State numbers are shared D3D7/D3D9 values; callers retain their native API type.
struct TextureStageWrite {
  unsigned stage;
  unsigned state;
  DWORD value;
};

std::span<const TextureStageWrite> TextureStageModeWrites(int mode);

#endif
