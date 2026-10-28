#include "Client/Application/ErrorDialog.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"

// OFFSET: 0x00AD6320
// FUNCTION: fn_17827
int ShowErrorDialog(const std::string& text, const UINT& type) {
  GetRenderSystemHolder()->PrepareErrorDialog();
  return MessageBoxA(nullptr, text.c_str(), "Error", type);
}

// OFFSET: 0x00AD6360
// FUNCTION: fn_17828
int ShowErrorDialog(CRenderSystemBase* renderSystem, const char* text, UINT type) {
  if (renderSystem) {
    renderSystem->PrepareErrorDialog();
  }

  return MessageBoxA(nullptr, text, "Error", type);
}

// OFFSET: 0x00AD6390
// FUNCTION: fn_17829
int ShowErrorDialog(int code) {
  char text[80];
  wsprintfA(text, "%d\n", code);
  GetRenderSystemHolder()->PrepareErrorDialog();
  return MessageBoxA(nullptr, text, "Error", MB_OK);
}
