#include "Contents/Effects/Core/EffectLists.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Primitives/RenderVertexPool.h"
#include "Render/DeviceContext/DCSurface.h"

// OFFSET: 0x0055A360
// FUNCTION: fn_02079
void CRenderer::ClearFrame() {
  g_effectMgr->ClearRetired();
  this->queues.Clear();
  this->commands.Rewind();
  this->primitives.Rewind();
  this->lightmapped.Rewind();
  this->vertices.Rewind();
}

// OFFSET: 0x00565970
// FUNCTION: fn_02222
bool CRendererDX9::Draw() {
  this->device->Begin();
  this->DrawQueues(this->queues, this->twoPass);
  this->ClearFrame();
  this->device->End();

  return true;
}
