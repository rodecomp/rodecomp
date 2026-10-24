#include "Contents/Effect.h"
#include "Render/DeviceOwnerDX9.h"
#include "Render/LightmappedPrimitive.h"
#include "Render/RenderCommandPool.h"
#include "Render/RenderPrimitivePool.h"
#include "Render/RenderQueues.h"
#include "Render/RenderVertexPool.h"
#include "Render/RendererDX9Operations.h"
#include "Render/RendererFrameDX9.h"

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
  CRenderSystemBase* renderSystem,
  const ServiceOptions& options,
  unsigned char multitexture,
  unsigned char twoPass,
  RendererFogDX9& fog,
  const float& aspect,
  const FogConfiguration& configuration
) {
  // The original deliberately ignores Begin's result and always draws/clears.
  this->device.Begin(renderSystem);
  this->operations.DrawQueues(this->queues, options, multitexture, twoPass, fog, aspect, configuration);
  this->Clear();
  this->device.End();
  return true;
}
