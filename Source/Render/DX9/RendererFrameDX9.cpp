#include "Contents/Effects/Core/EffectLists.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Primitives/RenderVertexPool.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Security/ThemidaMgr.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0055A360
// FUNCTION: fn_02079
void CRenderer::ClearFrame() {
  g_effectMgr->ClearRetired();
  this->ClearQueues();
  this->commands.Rewind();
  this->primitives.Rewind();
  this->lightmapped.Rewind();
  this->vertices.Rewind();
}

// OFFSET: 0x00565970
// FUNCTION: fn_02222
// VIRTUALIZED: emulated 0x02772E52
bool CRendererDX9::Draw() {
  // STRING: st_00823
  static constexpr char source[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\3dLib\\RendererDX9.cpp";
  g_renderDiagnosticLine = 116;
  g_renderDiagnosticFile = source;
  CRendererDX9* renderer = this;
  DeviceOwnerDX9* device = renderer->device;
  if (device->Begin()) {
    if (!CThemidaMgr::IsProtected()) {
      return false;
    }

    g_renderDiagnosticLine = 131;
    g_renderDiagnosticFile = source;
    renderer->DrawQueues();
    g_renderDiagnosticLine = 134;
    g_renderDiagnosticFile = source;
    renderer->ClearFrame();
    g_renderDiagnosticLine = 137;
    g_renderDiagnosticFile = source;
    device->End();
    g_renderDiagnosticLine = 140;
    g_renderDiagnosticFile = source;
    return true;
  }

  this->ClearFrame();
  g_renderDiagnosticLine = 151;
  g_renderDiagnosticFile = source;
  return false;
}
