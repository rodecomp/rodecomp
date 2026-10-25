#include "Render/DX7/RendererDX7Operations.h"
#include "Render/TextureStageModes.h"

// OFFSET: 0x0055ED50
// FUNCTION: fn_02116
void RendererDX7Operations::SetTextureMode(int mode) {
  for (const auto& write : TextureStageModeWrites(mode)) {
    this->device->SetTextureStageState(write.stage, static_cast<D3DTEXTURESTAGESTATETYPE>(write.state), write.value);
  }
}
