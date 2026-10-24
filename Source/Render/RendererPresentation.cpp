#include "Render/DeviceOwnerDX9.h"
#include "Render/RendererPresentation.h"

#include <mmsystem.h>

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
  this->diagnostic.line = 161;
  this->diagnostic.file = source;
  const HRESULT result = owner.Present(option);
  this->diagnostic.line = 164;
  this->diagnostic.file = source;
  return result;
}
