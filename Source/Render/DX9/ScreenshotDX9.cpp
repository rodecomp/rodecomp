#include "Render/DX9/D3DX9Runtime.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/UIRPData.h"

#include <cstdint>

// OFFSET: 0x005680A0
// FUNCTION: fn_02236
bool CRendererDX9::SaveScreenshot(const char* filename, int format, CTexture* overlay, const tagRECT* rectangle) {
  if (overlay) {
    DeviceOwnerDX9* device = this->device;
    unsigned y;
    unsigned x;
    if (rectangle) {
      x = rectangle->right;
      y = rectangle->bottom;
    } else {
      x = device->backBufferDescription.Width;
      y = device->backBufferDescription.Height;
    }
    x -= overlay->Width();
    y -= overlay->Height();
    UIRPData drawing;
    drawing.Draw(x, y, overlay, 0xFFFFFFFFu, 0, 0, false);

    if (this->device->Begin()) {
      ScopedDeviceStateDX9 restore = this->device->CaptureState();
      this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
      this->device->SetRenderState(D3DRS_FOGENABLE, 0);
      this->device->SetRenderState(D3DRS_ALPHAREF, 1);
      this->device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
      this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

      for (RenderCommand* const& command : drawing.Primitives()) {
        CTexture* texture = command->texture;
        if (texture) {
          this->device->SetTexture(0, static_cast<CTextureDX9*>(texture)->NativeTexture());
        } else {
          this->device->SetTexture(0, nullptr);
        }
        this->DrawPrimitive(command);
      }

      this->device->End();
    }
  }

  return SUCCEEDED(D3DXSaveSurfaceToFileA(filename, format, this->device->backBuffer, nullptr, rectangle));
}
