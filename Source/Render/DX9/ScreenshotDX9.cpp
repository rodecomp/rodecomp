#include "Render/DX9/D3DX9Runtime.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererDX9Operations.h"
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
bool RendererDX9Operations::SaveScreenshot(
  DeviceOwnerDX9& owner, CRenderSystemBase* renderSystem, const char* filename, int format, CTexture* overlay, const tagRECT* rectangle
) {
  if (overlay) {
    unsigned x = rectangle ? static_cast<unsigned>(rectangle->right) : owner.backBufferDescription.Width;
    unsigned y = rectangle ? static_cast<unsigned>(rectangle->bottom) : owner.backBufferDescription.Height;
    x -= overlay->Width();
    y -= overlay->Height();
    ScreenshotOverlay drawing;
    drawing.Draw(*this->context.primitives, std::bit_cast<std::int32_t>(x), std::bit_cast<std::int32_t>(y), overlay, 0xFFFFFFFFu, 0, 0, false);

    if (owner.Begin(renderSystem)) {
      ScopedDeviceStateDX9 restore(this->state);
      this->state.SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
      this->state.SetRenderState(D3DRS_FOGENABLE, 0);
      this->state.SetRenderState(D3DRS_ALPHAREF, 1);
      this->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
      this->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

      for (RenderPrimitive* command : drawing.Primitives()) {
        this->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
        this->DrawPrimitive(command);
      }

      owner.End();
    }
  }

  return SUCCEEDED(this->context.d3dx->saveSurfaceToFileA(filename, format, owner.backBuffer, nullptr, rectangle));
}
