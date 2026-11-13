#include "Render/DX9/D3DX9Runtime.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/UIRPData.h"

#include <bit>
#include <cstdint>

namespace {
  class ScreenshotOverlay : public UIRPData {
  public:
    // INLINED
    const std::list<RenderPrimitive*>& Primitives() const {
      return this->m_primitives;
    }
  };
}

// OFFSET: 0x005680A0
// FUNCTION: fn_02236
bool CRendererDX9::SaveScreenshot(const char* filename, int format, CTexture* overlay, const tagRECT* rectangle) {
  DeviceOwnerDX9& owner = *this->device;
  if (overlay) {
    unsigned x = rectangle ? static_cast<unsigned>(rectangle->right) : owner.backBufferDescription.Width;
    unsigned y = rectangle ? static_cast<unsigned>(rectangle->bottom) : owner.backBufferDescription.Height;
    x -= overlay->Width();
    y -= overlay->Height();
    ScreenshotOverlay drawing;
    drawing.Draw(std::bit_cast<std::int32_t>(x), std::bit_cast<std::int32_t>(y), overlay, 0xFFFFFFFFu, 0, 0, false);

    if (owner.Begin()) {
      ScopedDeviceStateDX9 restore(this->device->state);
      this->device->state.SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
      this->device->state.SetRenderState(D3DRS_FOGENABLE, 0);
      this->device->state.SetRenderState(D3DRS_ALPHAREF, 1);
      this->device->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
      this->device->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

      for (RenderPrimitive* command : drawing.Primitives()) {
        this->device->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
        this->DrawPrimitive(command);
      }

      owner.End();
    }
  }

  return SUCCEEDED(D3DXSaveSurfaceToFileA(filename, format, owner.backBuffer, nullptr, rectangle));
}
