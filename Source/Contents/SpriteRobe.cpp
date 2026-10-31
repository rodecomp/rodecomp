#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/CharacterRender.h"
#include "Contents/Actors/SpriteDrawingPolicy.h"
#include "Contents/Camera/Camera.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/SpriteGeometry.h"
#include "Render/SpriteQuad.h"
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
  if (this->unknown_00BC < 1 || this->unknown_00C0 == 1 || (this->unknown_0040 >= 64 && this->unknown_0040 <= 71)
      || (this->unknown_005C == -1 && this->unknown_0060 == -1 && this->unknown_0064 == -1)) {
    return;
  }

  if (reinterpret_cast<const SpriteDrawingPolicy*>(this)->DrawOnTop() == drawOnTop) {
    std::string sprName = g_session.GetRobeSprName(
      this->unknown_0010, this->unknown_0014, this->unknown_0018[0], this->unknown_00BC, this->unknown_004C, this->unknown_0050
    );
    std::string actName = g_session.GetRobeActName(
      this->unknown_0010, this->unknown_0014, this->unknown_0018[0], this->unknown_00BC, this->unknown_004C, this->unknown_0050
    );
    if (!ResourceMgr::Instance().Exists(sprName) || !ResourceMgr::Instance().Exists(actName)) {
      return;
    }

    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprName));
    CActRes* act = static_cast<CActRes*>(ResourceMgr::Instance().Load(actName));
    int frame = this->unknown_0044;
    if (baseAct) {
      int baseFrame = frame;
      if ((this->unknown_0040 >= 0 && this->unknown_0040 < 8) || (this->unknown_0040 >= 16 && this->unknown_0040 <= 22)) {
        baseFrame = 0;
      }
      g_session.GetSyncedFrame(baseAct, act, this->unknown_0040, baseFrame, 0, &frame);
    }

    const ActionFrame& actionFrame = act->Frame(this->unknown_0040, frame);
    for (int index = 0; index < actionFrame.cellCount; ++index) {
      const SpriteFrameCell& cell = actionFrame.Cell(index);
      if (cell.image == -1) {
        continue;
      }

      reinterpret_cast<unsigned char*>(&const_cast<SpriteFrameCell&>(cell).color)[3] =
        static_cast<unsigned char>(g_session.GetRobeAlpha(this->unknown_00BC));
      const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];

      SpriteQuad quad;
      tagRECT bounds;
      SpriteColor color;
      float bottomDepth;
      float topDepth;
      reinterpret_cast<const SpriteDrawingPolicy*>(this)->ProjectCell(
        layer, layer, scale, cell, x, y, position, shift, bottomDepth, topDepth, *image, reinterpret_cast<SpriteCellQuad&>(quad), bounds, color, 0, 0,
        !drawOnTop
      );

      if (!drawOnTop && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch >= -40.0f) {
        quad.bottomRhw = bottomDepth - 0.0000065f;
        quad.topRhw = topDepth - 0.0000065f;
      }
      if (this->unknown_0040 >= 58 && this->unknown_0040 <= 61) {
        quad.bottomRhw = quad.bottomRhw + 0.0000065f;
        quad.topRhw = quad.topRhw + 0.0000065f;
      }

      quad.image = image;
      RenderPrimitive* primitive = BuildSpriteQuad(*GetRenderSystemHolder()->system, quad, sprite->palette, this->unknown_005C, 0.0f, 0.0f);
      primitives.push_back(primitive);
    }
  }

  ++layer;
}
