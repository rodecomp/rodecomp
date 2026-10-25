#include "Render/DX9/DeviceStateDX9.h"
#include "Render/DX9/RendererDX9Operations.h"
#include "Render/TextureStageModes.h"

// OFFSET: 0x00568470
// FUNCTION: fn_02241
void RendererDX9Operations::SetTextureMode(int mode) {
  for (const auto& write : TextureStageModeWrites(mode)) {
    this->state.SetTextureStageState(write.stage, static_cast<D3DTEXTURESTAGESTATETYPE>(write.state), write.value);
  }
}
