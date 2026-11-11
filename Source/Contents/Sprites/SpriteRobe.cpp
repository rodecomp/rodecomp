#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/CharacterRender.h"
#include "Contents/Camera/Camera.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"

#include <list>
#include <string>
#include <windows.h>

// OFFSET: 0x0080DBA0
// FUNCTION: fn_10104
void CCharacterRender::RenderRobe(
  std::list<RenderPrimitive*>& primitives, bool drawOnTop, const void*, int& layer, const float* position, float x, float y, float scale,
  float shift, float, CActRes* baseAct
) {
  if (this->robe < 1 || this->isHelmRobe == 1 || (this->actionIndex >= 64 && this->actionIndex <= 71)
      || (this->bodyBlend == -1 && this->bodyCellX == -1 && this->bodyCellY == -1)) {
    return;
  }

  if (this->DrawOnTop() == drawOnTop) {
    std::string sprName = g_session.GetRobeSprName(
      this->job, this->headJob, this->appearance[0], this->robe, this->riding, this->bodyCostume
    );
    std::string actName = g_session.GetRobeActName(
      this->job, this->headJob, this->appearance[0], this->robe, this->riding, this->bodyCostume
    );
    if (!ResourceMgr::Instance().Exists(sprName) || !ResourceMgr::Instance().Exists(actName)) {
      return;
    }

    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprName));
    CActRes* act = static_cast<CActRes*>(ResourceMgr::Instance().Load(actName));
    int frame = this->frameIndex;
    if (baseAct) {
      int action = this->actionIndex;
      g_session.GetSyncedFrame(
        baseAct, act, action, ((action >= 0 && action < 8) || (action >= 16 && action <= 22)) ? 0 : frame, 0, &frame
      );
    }

    const ActionFrame& actionFrame = act->Frame(this->actionIndex, frame);
    for (int index = 0; index < actionFrame.cellCount; ++index) {
      const SpriteFrameCell& cell = actionFrame.Cell(index);
      if (cell.image == -1) {
        continue;
      }

      reinterpret_cast<unsigned char*>(&const_cast<SpriteFrameCell&>(cell).color)[3] =
        static_cast<unsigned char>(g_session.GetRobeAlpha(this->robe));
      const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];

      SpriteQuad quad;
      tagRECT bounds;
      SpriteColor color;
      float bottomDepth;
      float topDepth;
      this->ProjectCell(
        layer, layer, scale, cell, x, y, position, shift, bottomDepth, topDepth, *image, reinterpret_cast<SpriteCellQuad&>(quad), bounds, color, 0, 0,
        !drawOnTop
      );

      if (!drawOnTop && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch >= -40.0f) {
        quad.bottomRhw = bottomDepth - 0.0000065f;
        quad.topRhw = topDepth - 0.0000065f;
      }
      if (this->actionIndex >= 58 && this->actionIndex <= 61) {
        quad.bottomRhw = quad.bottomRhw + 0.0000065f;
        quad.topRhw = quad.topRhw + 0.0000065f;
      }

      quad.image = image;
      RenderPrimitive* primitive = BuildSpriteQuad(*GetRenderSystemHolder()->system, quad, sprite->palette, this->bodyBlend, 0.0f, 0.0f);
      primitives.push_back(primitive);
    }
  }

  ++layer;
}
