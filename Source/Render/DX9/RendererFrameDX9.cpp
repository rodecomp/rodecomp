#include "Contents/Effects/Effect.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Primitives/RenderVertexPool.h"
#include "Render/DX9/RendererDX9Operations.h"
#include "Render/DX9/RendererFrameDX9.h"

// OFFSET: 0x0055A360
// FUNCTION: fn_02079
void RendererFrameDX9::Clear() {
  this->retirement.Clear();
  this->queues.Clear();
  this->commands.Rewind();
  this->primitives.Rewind();
  this->lightmapped.Rewind();
  this->vertices.Rewind();
}

// OFFSET: 0x00565970
// FUNCTION: fn_02222
bool RendererFrameDX9::Draw(
  CRenderSystemBase* renderSystem, const ServiceOptions& options, unsigned char multitexture, unsigned char twoPass, RendererFogDX9& fog, const float& aspect,
  const FogConfiguration& configuration
) {
  this->device.Begin(renderSystem);
  this->operations.DrawQueues(this->queues, options, multitexture, twoPass, fog, aspect, configuration);
  this->Clear();
  this->device.End();

  return true;
}
