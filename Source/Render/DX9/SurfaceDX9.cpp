#include "Render/DX9/SurfaceDX9.h"

#include <algorithm>
#include <cstring>

namespace {
  // OFFSET: 0x00570970
  // FUNCTION: fn_02430
  RECT Clip(const RECT& rectangle, const D3DSURFACE_DESC& description) {
    return {
      std::clamp<LONG>(rectangle.left, 0, description.Width), std::clamp<LONG>(rectangle.top, 0, description.Height),
      std::clamp<LONG>(rectangle.right, 0, description.Width), std::clamp<LONG>(rectangle.bottom, 0, description.Height)
    };
  }

  // OFFSET: 0x00570C70
  // FUNCTION: fn_02437
  int BitsPerPixel(D3DFORMAT format) {
    switch (format) {
    case D3DFMT_A8R8G8B8:
    case D3DFMT_X8R8G8B8:
      return 32;
    case D3DFMT_R5G6B5:
    case D3DFMT_X1R5G5B5:
    case D3DFMT_A1R5G5B5:
    case D3DFMT_A4R4G4B4:
      return 16;
    default:
      return 0;
    }
  }

  // OFFSET: 0x00570D70
  // FUNCTION: fn_02441
  std::uint16_t Color1555(std::uint32_t color) {
    return ((color >> 16) & 0x8000) | ((color & 255) >> 3) | ((color >> 6) & 0x3E0) | ((color >> 9) & 0x7C00);
  }

  // INLINED
  UINT AlignDimension(int value) {
    const std::uint32_t dimension = value;
    return dimension + ((0u - dimension) & 3);
  }
}

// OFFSET: 0x00570720
// FUNCTION: fn_02427
CSurfaceDX9::CSurfaceDX9(SurfaceDX9Context& context, int width, int height)
    : m_context(&context), m_width(AlignDimension(width)), m_height(AlignDimension(height)), m_surface(nullptr) {
  context.device->CreateOffscreenPlainSurface(this->m_width, this->m_height, context.backBufferDescription.Format, D3DPOOL_DEFAULT, &this->m_surface, nullptr);
}

// OFFSET: 0x005707D0
// FUNCTION: fn_02428
CSurfaceDX9::CSurfaceDX9(SurfaceDX9Context& context, IDirect3DSurface9* surface) : m_context(&context) {
  D3DSURFACE_DESC description;
  if (FAILED(surface->GetDesc(&description))) {
    this->m_width = 0;
    this->m_height = 0;
    this->m_surface = nullptr;
  } else {
    this->m_width = description.Width;
    this->m_height = description.Height;
    this->m_surface = surface;
    surface->AddRef();
  }
}

CSurfaceDX9::~CSurfaceDX9() {
  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }
}

// OFFSET: 0x00570CB0
// FUNCTION: fn_02438
std::uint32_t CSurfaceDX9::Width() {
  return this->m_width;
}

// OFFSET: 0x00570C60
// FUNCTION: fn_02436
std::uint32_t CSurfaceDX9::Height() {
  return this->m_height;
}

// OFFSET: 0x00570A20
// FUNCTION: fn_02432
bool CSurfaceDX9::Resize(int width, int height) {
  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }

  this->m_width = AlignDimension(width);
  this->m_height = AlignDimension(height);
  return SUCCEEDED(this->m_context->device->CreateOffscreenPlainSurface(
           this->m_width, this->m_height, this->m_context->backBufferDescription.Format, D3DPOOL_DEFAULT, &this->m_surface, nullptr
         )) &&
         this->m_surface;
}

// OFFSET: 0x00570CC0
// FUNCTION: fn_02439
SurfaceLock CSurfaceDX9::Lock() {
  SurfaceLock output{};
  D3DSURFACE_DESC description;
  D3DLOCKED_RECT locked;

  if (this->m_surface && SUCCEEDED(this->m_surface->GetDesc(&description)) && SUCCEEDED(this->m_surface->LockRect(&locked, nullptr, 0))) {
    output.success = true;
    output.pixels = locked.pBits;
    output.pitch = locked.Pitch;
    output.width = description.Width;
    output.height = description.Height;
    output.bitsPerPixel = BitsPerPixel(description.Format);
  }

  return output;
}

// OFFSET: 0x00570DB0
// FUNCTION: fn_02442
void CSurfaceDX9::Unlock() {
  if (this->m_surface) {
    this->m_surface->UnlockRect();
  }
}

// OFFSET: 0x00570C30
// FUNCTION: fn_02435
bool CSurfaceDX9::GetDC(HDC* output) {
  return this->m_surface && SUCCEEDED(this->m_surface->GetDC(output));
}

// OFFSET: 0x00570D50
// FUNCTION: fn_02440
void CSurfaceDX9::ReleaseDC(HDC dc) {
  if (this->m_surface) {
    this->m_surface->ReleaseDC(dc);
  }
}

// OFFSET: 0x00570DC0
// FUNCTION: fn_02443
void CSurfaceDX9::Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int, int sourceWidth) {
  if (!this->m_surface || !width || !height) {
    return;
  }

  D3DSURFACE_DESC description;
  if (FAILED(this->m_surface->GetDesc(&description))) {
    return;
  }

  RECT rectangle{x, y, x + width, y + height};
  D3DLOCKED_RECT locked;
  if (FAILED(this->m_surface->LockRect(&locked, &rectangle, 0))) {
    return;
  }

  const int stride = sourceWidth ? sourceWidth : width;
  if (BitsPerPixel(description.Format) == 16) {
    for (int row = 0; row < height; ++row) {
      auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;

      for (int column = 0; column < width; ++column) {
        const std::uint32_t color = pixels[row * stride + column];
        std::uint16_t value = 0;
        if ((color & 0xFFFFFF) != 0xFF00FF) {
          switch (description.Format) {
          case D3DFMT_R5G6B5:
            value = ((color & 255) >> 3) | ((color >> 5) & 0x7E0) | ((color >> 8) & 0xF800);
            break;
          case D3DFMT_X1R5G5B5:
          case D3DFMT_A1R5G5B5:
            value = Color1555(color);
            break;
          case D3DFMT_A4R4G4B4:
            value = ((color >> 16) & 0xF000) | ((color & 255) >> 4) | ((color >> 8) & 0xF0) | ((color >> 12) & 0xF00);
            break;
          default:
            break;
          }
        }

        std::memcpy(output + column * sizeof(value), &value, sizeof(value));
      }
    }
  } else if (description.Format == D3DFMT_A8R8G8B8 || description.Format == D3DFMT_X8R8G8B8) {
    for (int row = 0; row < height; ++row) {
      auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;

      for (int column = 0; column < width; ++column) {
        const std::uint32_t color = pixels[row * stride + column];
        const std::uint32_t value = (color & 0xFFFFFF) == 0xFF00FF ? 0 : color;
        std::memcpy(output + column * sizeof(value), &value, sizeof(value));
      }
    }
  }

  this->m_surface->UnlockRect();
}

// OFFSET: 0x005709F0
// FUNCTION: fn_02431
void CSurfaceDX9::Fill(const RECT* rectangle, std::uint32_t color) {
  if (this->m_surface) {
    this->m_context->device->ColorFill(this->m_surface, rectangle, color);
  }
}

// OFFSET: 0x00570AA0
// FUNCTION: fn_02433
void CSurfaceDX9::Draw(int x, int y, int width, int height, std::uint32_t) {
  if (!this->m_surface) {
    return;
  }

  D3DSURFACE_DESC description;
  if (FAILED(this->m_surface->GetDesc(&description))) {
    return;
  }

  const RECT target{x, y, x + width, y + height};
  const RECT destination = Clip(target, this->m_context->backBufferDescription);
  const RECT original{0, 0, destination.right - destination.left, destination.bottom - destination.top};
  const RECT source = Clip(original, description);
  this->m_context->device->StretchRect(this->m_surface, &source, this->m_context->backBuffer, &destination, D3DTEXF_NONE);
}

// OFFSET: 0x00570BA0
// FUNCTION: fn_02434
void CSurfaceDX9::Stretch(int x, int y, int width, int height) {
  if (!this->m_surface) {
    return;
  }

  const RECT target{x, y, x + width, y + height};
  const RECT destination = Clip(target, this->m_context->backBufferDescription);
  this->m_context->device->StretchRect(this->m_surface, nullptr, this->m_context->backBuffer, &destination, D3DTEXF_LINEAR);
}
