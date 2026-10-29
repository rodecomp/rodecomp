#include "Contents/Actors/SpriteDrawingPolicy.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/Vector3d.h"

// OFFSET: 0x00811C10
// FUNCTION: fn_10109
RenderPrimitive* SpriteDrawingPolicy::DrawShadow(const float* matrix, float scale) const {
  if (scale == 0.0f || !g_modeMgr.GetGameMode() || !static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene) {
    return nullptr;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  float position[3];
  position[0] = this->position.x;
  position[1] = mode->scene->terrain->Grid().Height(this->position.x, this->position.z);
  position[2] = this->position.z;
  const float depth = position[0] * matrix[2] + position[1] * matrix[5] + position[2] * matrix[8] + matrix[11];

  float screenX;
  float screenY;
  float reciprocalDepth;
  if (this->shiftX == 0 && this->shiftY == 0) {
    g_renderer->ProjectViewport(position, matrix, &screenX, &screenY, &reciprocalDepth);
  } else {
    g_renderer->ProjectViewportCentered(
      position, matrix, &screenX, &screenY, &reciprocalDepth, static_cast<float>(this->shiftX) + this->offsetX, static_cast<float>(this->shiftY) + this->offsetY
    );
  }
  const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("shadow.spr"));
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("shadow.act"));
  const SpriteFrameCell& cell = action->Frame(0, 0).Cell(0);
  if (cell.image == -1) {
    return nullptr;
  }

  const SpriteRleImage* image = sprite->images[cell.imageType][0];
  const float size = g_session.GetNpcLocationRadius(this->argument0010, this->riding) * scale;
  const double factor = distanceScale;

  SpriteQuad quad;
  quad.top = static_cast<float>(cell.y * factor * cell.scaleY * size + screenY);
  quad.bottom = static_cast<float>(((image->unknown_0006 + 1) * image->height + cell.y) * factor * cell.scaleY * size + screenY + 1.0);
  quad.left = static_cast<float>(cell.x * factor * cell.scaleX * size + screenX);
  quad.right = static_cast<float>(((image->unknown_0004 + 1) * image->width + cell.x) * factor * cell.scaleX * size + screenX + 1.0);
  if (quad.top >= static_cast<float>(g_renderer->viewport.height) || quad.bottom <= 0.0f || quad.left >= static_cast<float>(g_renderer->viewport.width)
      || quad.right <= 0.0f) {
    return nullptr;
  }

  quad.color = static_cast<std::uint32_t>(((static_cast<std::uint32_t>(cell.color >> 24) << 8 | (cell.color & 0xFF)) << 8 | (cell.color >> 8 & 0xFF)) << 8) |
               (cell.color >> 16 & 0xFF);
  quad.bottomRhw = 1.0f / (depth - (quad.bottom - screenY) * g_cotangent) + 0.00001f;
  quad.topRhw = 1.0f / (depth - (quad.top - screenY) * g_cotangent) + 0.00001f;
  quad.angle = static_cast<float>(cell.unknown_001C);
  quad.image = image;

  return BuildSpriteQuad(*GetRenderSystemHolder()->system, quad, sprite->palette, 257, 0.0f, 0.0f);
}
