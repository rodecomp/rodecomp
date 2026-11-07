#include "Client/Application/ErrorDialog.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/TextureAllocationDX7.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RendererDX7.h"
#include "Render/Textures/Texture.h"
#include "UI/GameSettings/GameSettingsUI.h"

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

// GLOBAL: gl_24508
unsigned char g_multitexture = 1;

// OFFSET: 0x0054A380
// FUNCTION: fn_01896
CRenderer* DeviceOwnerDX7::CreateRenderer() {
  int format;

  if (this->redMask == 0xF800) {
    format = TextureFormat::R5G6B5;
  } else if (this->redMask == 0x7C00) {
    format = TextureFormat::X1R5G5B5;
  } else if (this->redMask == 0x1F) {
    ShowErrorDialog("BGR 565 MODE. NOT SUPPORTED", 0);
    format = TextureFormat::R5G6B5;
  } else if (this->bitDepth == 32) {
    format = TextureFormat::A8R8G8B8;
  } else {
    ShowErrorDialog("NOT SUPPORTED MODE", 0);
    format = TextureFormat::UnknownTextureFormat;
  }

  DDCAPS drawCaps = {sizeof(drawCaps)};

  if (this->draw->GetCaps(&drawCaps, nullptr) == S_OK) {
    this->colorKeySupported = (drawCaps.dwCaps & DDCAPS_BLTSTRETCH) != 0;
  }

  if (this->device->GetCaps(&this->caps) == S_OK) {
    this->minTextureWidth = this->caps.dwMinTextureWidth;
    this->minTextureHeight = this->caps.dwMinTextureHeight;
    this->maxTextureWidth = this->caps.dwMaxTextureWidth;
    this->maxTextureHeight = this->caps.dwMaxTextureHeight;
    this->maxTextureAspectRatio = this->caps.dwMaxTextureAspectRatio;
    g_multitexture = this->caps.wMaxSimultaneousTextures >= 2 && (this->caps.dwTextureOpCaps & D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR) != 0;
    g_textureAllocationDX7.Update(g_textureFiltering, this->caps);
  }

  this->largeTextures = this->maxTextureWidth >= 1024 && this->maxTextureHeight >= 1024;

  CRendererDX7* renderer = new CRendererDX7();
  renderer->SetTextureFormat(format);
  renderer->device = this->device;
  renderer->viewport.SetSize(this->width, this->height);
  this->CalibrateTextureCoordinates();
  return renderer;
}
