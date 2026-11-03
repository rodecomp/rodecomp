#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererPresentation.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00565B70
// FUNCTION: fn_02223
HRESULT RendererPresentation::Present(DeviceOwnerDX9& owner, int option) {
  ++this->frame;
  ++this->frames;
  const unsigned now = timeGetTime();
  const unsigned elapsed = now - this->lastTick;

  if (elapsed >= 1000) {
    const unsigned count = this->frames;
    this->frames = 0;
    this->lastTick = now;
    this->fps = static_cast<float>(count) / (static_cast<float>(elapsed) / 1000.0f);
  }

  static constexpr char source[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\3dLib\\RendererDX9.cpp";
  g_renderDiagnosticLine = 161;
  g_renderDiagnosticFile = source;
  const HRESULT result = owner.Present(option);
  g_renderDiagnosticLine = 164;
  g_renderDiagnosticFile = source;

  return result;
}
