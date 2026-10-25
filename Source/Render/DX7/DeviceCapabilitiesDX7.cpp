#include "Client/Application/ErrorDialog.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/TextureAllocationDX7.h"
#include "Render/Textures/Texture.h"

// INLINED
inline void TextureAllocationDX7::Update(int enabled, const D3DDEVICEDESC7& caps) {
  if (enabled != 0 && (caps.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR)) {
    this->descriptorFlags = DDSD_MIPMAPCOUNT;
    this->surfaceCaps = DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
  } else {
    this->descriptorFlags = 0;
    this->surfaceCaps = 0;
  }
}

// OFFSET: 0x0054A380
// FUNCTION: fn_01896
int DeviceOwnerDX7::RefreshRendererCapabilities(int mipmaps, TextureAllocationDX7& allocation, bool& multitexture, CRenderSystemBase* renderSystem) {
  int format;

  switch (this->redMask) {
  case 0xF800:
    format = TextureFormat::R5G6B5;
    break;
  case 0x7C00:
    format = TextureFormat::X1R5G5B5;
    break;
  case 0x1F:
    ShowErrorDialog(renderSystem, "BGR 565 MODE. NOT SUPPORTED", 0);
    format = TextureFormat::R5G6B5;
    break;
  default:
    if (this->bitDepth == 32) {
      format = TextureFormat::A8R8G8B8;
    } else {
      ShowErrorDialog(renderSystem, "NOT SUPPORTED MODE", 0);
      format = TextureFormat::UnknownTextureFormat;
    }
    break;
  }

  DDCAPS drawCaps{};
  drawCaps.dwSize = sizeof(drawCaps);

  if (this->draw->GetCaps(&drawCaps, nullptr) == S_OK) {
    this->colorKeySupported = (drawCaps.dwCaps & DDCAPS_BLTSTRETCH) != 0;
  }

  if (this->device->GetCaps(&this->caps) == S_OK) {
    this->minTextureWidth = this->caps.dwMinTextureWidth;
    this->minTextureHeight = this->caps.dwMinTextureHeight;
    this->maxTextureWidth = this->caps.dwMaxTextureWidth;
    this->maxTextureHeight = this->caps.dwMaxTextureHeight;
    this->maxTextureAspectRatio = this->caps.dwMaxTextureAspectRatio;
    multitexture = this->caps.wMaxSimultaneousTextures >= 2 && (this->caps.dwTextureOpCaps & D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR) != 0;
    allocation.Update(mipmaps, this->caps);
  }

  this->largeTextures = this->maxTextureWidth >= 1024 && this->maxTextureHeight >= 1024;
  return format;
}
