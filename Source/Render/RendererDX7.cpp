#include "Render/RendererDX7.h"

// OFFSET: 0x0055EC00
void CRendererDX7::SetClearColor(unsigned color) {
  this->clearColor = color;
}

// OFFSET: 0x0055ED40
// FUNCTION: fn_02115
void CRendererDX7::SetCamera(const float* eye, const float* target, const float* up) {
}

// OFFSET: 0x0055F690
void CRendererDX7::SetTextureFormat(unsigned format) {
  this->unknown_024C = format;
}
