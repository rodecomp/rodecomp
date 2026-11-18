#include "Contents/Actors/CharacterRender.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/Vector3d.h"

// OFFSET: 0x00811C10
// FUNCTION: fn_10109
RenderPrimitive* CCharacterRender::DrawShadow(const float* matrix, float scale) const {
  if (0.0f == scale || !g_modeMgr.GetGameMode() || !g_modeMgr.GetGameMode()->scene) {
    return nullptr;
  }

  const float height = g_modeMgr.GetGameMode()->scene->terrain->Height(this->position.x, this->position.z);
  vector3d position;
  position.x = this->position.x;
  position.y = height;
  position.z = this->position.z;
  const float depth = position.x * matrix[2] + position.y * matrix[5] + position.z * matrix[8] + matrix[11];

  float screenX;
  float screenY;
  float reciprocalDepth;
  if (this->viewportWidth == 0 && this->viewportHeight == 0) {
    g_renderer->ProjectViewport(&position.x, matrix, &screenX, &screenY, &reciprocalDepth);
  } else {
    g_renderer->ProjectViewportCentered(
      &position.x, matrix, &screenX, &screenY, &reciprocalDepth, static_cast<float>(this->viewportWidth) + this->viewportOffsetX, static_cast<float>(this->viewportHeight) + this->viewportOffsetY
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
  const float size = g_session.GetNpcLocationRadius(this->job, this->riding) * scale;
  const double factor = distanceScale;

  const float top = static_cast<float>(cell.y * factor * cell.scaleY * size + screenY);
  const float bottom = static_cast<float>(((image->doubleHeight + 1) * image->height + cell.y) * factor * cell.scaleY * size + screenY + 1.0);
  const float left = static_cast<float>(cell.x * factor * cell.scaleX * size + screenX);
  const float right = static_cast<float>(((image->doubleWidth + 1) * image->width + cell.x) * factor * cell.scaleX * size + screenX + 1.0);
  if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f || left >= static_cast<float>(g_renderer->viewport.width) || right <= 0.0f) {
    return nullptr;
  }

  SpriteQuad quad;
  quad.top = top;
  quad.left = left;
  quad.bottom = bottom;
  quad.right = right;
  quad.image = image;
  quad.color = (((((cell.colorChannels.alpha << 8) + cell.colorChannels.red) << 8) + cell.colorChannels.green) << 8) + cell.colorChannels.blue;
  quad.bottomRhw = 1.0f / (depth - (bottom - screenY) * g_cotangent) + 0.00001f;
  quad.topRhw = 1.0f / (depth - (top - screenY) * g_cotangent) + 0.00001f;
  quad.angle = static_cast<float>(cell.rotation);

  return this->BuildSpriteQuad(quad, sprite->palette, 257, 0.0f, 0.0f);
}
