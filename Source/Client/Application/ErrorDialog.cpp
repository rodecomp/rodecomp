#include "Client/Application/ErrorDialog.h"
#include "Render/RenderSystemBase.h"

// OFFSET: 0x00AD6360
// FUNCTION: fn_17828
int ShowErrorDialog(CRenderSystemBase* renderSystem, const char* text, UINT type) {
  if (renderSystem) {
    renderSystem->PrepareErrorDialog();
  }

  return MessageBoxA(nullptr, text, "Error", type);
}
