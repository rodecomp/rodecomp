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
bool CRendererDX9::Draw() {
  // STRING: st_00823
  static constexpr char source[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\3dLib\\RendererDX9.cpp";
  g_renderDiagnosticLine = 116;
  g_renderDiagnosticFile = source;
  if (!this->device->Begin()) {
    this->ClearFrame();
    g_renderDiagnosticLine = 151;
    g_renderDiagnosticFile = source;
    return false;
  }

  if (!CThemidaMgr::IsProtected()) {
    return false;
  }

  g_renderDiagnosticLine = 131;
  g_renderDiagnosticFile = source;
  this->DrawQueues();
  g_renderDiagnosticLine = 134;
  g_renderDiagnosticFile = source;
  this->ClearFrame();
  g_renderDiagnosticLine = 137;
  g_renderDiagnosticFile = source;
  this->device->End();
  g_renderDiagnosticLine = 140;
  g_renderDiagnosticFile = source;
  return true;
}
