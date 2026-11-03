#include "Render/DX7/RenderSystemDX7.h"
#include "Render/BlendColor.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/RendererDX7.h"

#include <bit>

// OFFSET: 0x0055EC10
// FUNCTION: fn_02114
void CRendererDX7::SetLighting(const float* direction, const float* diffuse, const float* ambient) {
  D3DLIGHT7 light{};
  light.dltType = D3DLIGHT_DIRECTIONAL;
  light.dvDirection.x = -direction[0];
  light.dvDirection.y = -direction[1];
  light.dvDirection.z = -direction[2];
  light.dcvDiffuse.r = diffuse[0];
  light.dcvDiffuse.g = diffuse[1];
  light.dcvDiffuse.b = diffuse[2];
  light.dcvDiffuse.a = 255.0f;
  light.dcvAmbient.r = ambient[0];
  light.dcvAmbient.g = ambient[1];
  light.dcvAmbient.b = ambient[2];
  light.dcvAmbient.a = 255.0f;
  this->device->SetLight(0, &light);
  this->device->LightEnable(0, TRUE);
}

// OFFSET: 0x0055A210
// FUNCTION: fn_02077
void CRendererDX7::ClearDepth(int clearColor) {
  if (clearColor) {
    g_deviceOwnerDX7->Clear(0xFF000000);
  } else {
    g_deviceOwnerDX7->ClearDepth();
  }
}

// OFFSET: 0x0055A240
// FUNCTION: fn_02078
void CRendererDX7::ClearWorld(unsigned color, int mode, unsigned char worldFlag, unsigned overlay) {
  if (mode != 0 && worldFlag == 1) {
    color = BlendColor(color, overlay);
  }

  g_deviceOwnerDX7->Clear(color);
}

// OFFSET: 0x0055ACB0
// FUNCTION: fn_02082
void CRendererDX7::DrawPrimitive(RenderCommand* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);

  if (primitive->indexCount) {
    this->device->DrawIndexedPrimitive(
      type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, primitive->vertices, primitive->vertexCount, primitive->indices,
      primitive->indexCount, 0
    );
  } else {
    this->device->DrawPrimitive(type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, primitive->vertices, primitive->vertexCount, 0);
  }
}

// OFFSET: 0x0055AD00
// OFFSET: 0x0055AD50
// FUNCTION: fn_02083
// FUNCTION: fn_02084
void CRendererDX7::DrawLightmappedPrimitive(LightmappedPrimitive* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);

  if (primitive->indexCount) {
    this->device->DrawIndexedPrimitive(
      type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2, primitive->vertices, primitive->vertexCount, primitive->indices,
      primitive->indexCount, 0
    );
  } else {
    this->device->DrawPrimitive(type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2, primitive->vertices, primitive->vertexCount, 0);
  }
}

// OFFSET: 0x0055AFA0
// FUNCTION: fn_02086
std::uint16_t DeviceOwnerDX7Handle::PackColor(int red, int green, int blue) const {
  const DeviceOwnerDX7* owner = this->owner;
  if (!owner) {
    return 0;
  }

  return static_cast<std::uint16_t>(
    (static_cast<std::uint16_t>(blue >> owner->blue.expansion) << owner->blue.shift) +
    (static_cast<std::uint16_t>(green >> owner->green.expansion) << owner->green.shift) +
    (static_cast<std::uint16_t>(red >> owner->red.expansion) << owner->red.shift)
  );
}
