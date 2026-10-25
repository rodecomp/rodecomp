#include "Render/DX7/TextureAllocationDX7.h"

// ORIGINAL_SITE: 0x0054A930 in fn_02323 (fragment)
void TextureAllocationDX7::Update(int enabled, const D3DDEVICEDESC7& caps) {
  if (enabled != 0 && (caps.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR)) {
    this->descriptorFlags = DDSD_MIPMAPCOUNT;
    this->surfaceCaps = DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
  } else {
    this->descriptorFlags = 0;
    this->surfaceCaps = 0;
  }
}
