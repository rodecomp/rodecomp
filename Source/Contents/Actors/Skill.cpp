#include "Contents/Actors/Skill.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"

// OFFSET: 0x00E503F0
// FUNCTION: fn_24294
void CSkill::Render(int argument) {
  if (!this->render.enabled) {
    this->UpdateTlvertex(reinterpret_cast<const float*>(argument));
    return;
  }

  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, reinterpret_cast<const float*>(argument), &x, &y, &reciprocalDepth);
  const float scale = g_renderer->ScaleDistance(reciprocalDepth) / g_heightScale;
  this->render.unknown_005C = scale;

  float bounds[5];
  bounds[0] = x - (g_renderer->viewport.uiScaleX + g_renderer->viewport.uiScaleX) * scale;
  bounds[1] = y - (g_renderer->viewport.uiScaleY + g_renderer->viewport.uiScaleY) * scale;
  bounds[2] = (g_renderer->viewport.uiScaleX + g_renderer->viewport.uiScaleX) * scale + x;
  bounds[3] = y;
  bounds[4] = reciprocalDepth;
  this->SetRenderInfo(bounds, x, y);
}
