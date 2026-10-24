#include "Client/ErrorDialog.h"
#include "Render/DeviceOwnerDX7.h"
#include "Render/TextureAllocationDX7.h"

// OFFSET: 0x0054A380
// FUNCTION: fn_01896
int DeviceOwnerDX7::RefreshRendererCapabilities(
  int mipmaps,
  TextureAllocationDX7& allocation,
  bool& multitexture,
  CRenderSystemBase* renderSystem
) {
  int format;

  switch (this->redMask) {
  case 0xF800:
    format = 2;
    break;
  case 0x7C00:
    format = 3;
    break;
  case 0x1F:
    ShowErrorDialog(renderSystem, "BGR 565 MODE. NOT SUPPORTED", 0);
    format = 2;
    break;
  default:
    if (this->bitDepth == 32) {
      format = 4;
    } else {
      ShowErrorDialog(renderSystem, "NOT SUPPORTED MODE", 0);
      format = -1;
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
    multitexture = this->caps.wMaxSimultaneousTextures >= 2 &&
                   (this->caps.dwTextureOpCaps & D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR) != 0;
    allocation.Update(mipmaps, this->caps);
  }

  this->largeTextures = this->maxTextureWidth >= 1024 && this->maxTextureHeight >= 1024;
  return format;
}
