#include "Contents/Actors/CharacterRender.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/AccessoryId.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Sprites/SpriteLayerDrawing.h"
#include "Contents/Sprites/SpriteLayerOrder.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Contents/Sprites/SpriteRidingPolicy.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/Sprites/ImfResource.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"

#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <windows.h>

// GLOBAL: gl_25256
static int g_headAnchorOffsetX;

// GLOBAL: gl_25257
static int g_headAnchorOffsetY;

struct CharacterRenderDrawItem {
  int flags;
  RenderCommand* command;
};

// OFFSET: 0x0080D2A0
// FUNCTION: fn_10099
CCharacterRender::CCharacterRender(int width, int renderHeight)
    : position(), headJob(0), lowerHeadgear(0), upperHeadgear(0), middleHeadgear(0), lowerHeadgearLocation(0),
      upperHeadgearLocation(0), middleHeadgearLocation(0), madogearType(0), riding(0), bodyCostume(0), viewportOffsetX(0), viewportOffsetY(80.0f), bodyBlend(-1),
      bodyCellX(-1), bodyCellY(-1), viewportWidth(width), viewportHeight(renderHeight), cameraDistance(400.0f), cameraPitch(-45.0f),
      cameraYaw(0.0f), unknown_007C(0.0f), unknown_0080(0.0f), unknown_0084(0.0f), cameraUp{0.0f, 1.0f, 0.0f},
      screenX(0.0f), screenY(0.0f), robe(0), isHelmRobe(0) {
  this->position.x = 0.0f;
  this->position.y = 0.0f;
  this->position.z = 0.0f;
  this->actionIndex = 0;
  this->frameIndex = 0;
  this->drawShadow = 1;
}

// OFFSET: 0x0080D730
// FUNCTION: fn_10101
CCharacterRender::~CCharacterRender() = default;

// OFFSET: 0x008105A0
// FUNCTION: fn_10108
std::list<CharacterRenderDrawItem> CCharacterRender::BuildDrawList(unsigned char trackBounds, char* boundsUpdated) {
  CGameMode* mode = g_modeMgr.GetGameMode();
  const float* view = mode->camera->frame.view.values;
  vector3d position;
  position.x = this->position.x * view[0] + this->position.y * view[3] + this->position.z * view[6] + view[9];
  position.y = this->position.x * view[1] + this->position.y * view[4] + this->position.z * view[7] + view[10];
  position.z = this->position.x * view[2] + this->position.y * view[5] + this->position.z * view[8] + view[11];
  float depthBias = 0.0f;

  this->screenX = 0.0f;
  this->screenY = 0.0f;
  float rhw = 0.0f;
  g_renderer->ProjectViewport(&this->position.x, view, &this->screenX, &this->screenY, &rhw);
  float distanceScale = g_renderer->ScaleDistance(rhw);
  distanceScale *= g_session.GetSkillFactor(this->job);

  int swapCount = 0;
  bool bodyDrawn = false;
  *boundsUpdated = 0;
  this->renderBounds.Set(static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.height), 0.0f, 0.0f, rhw);

  std::list<CharacterRenderDrawItem> commands;
  if (this->drawShadow == 1) {
    if (RenderPrimitive* shadow = this->DrawShadow(view, 1.0f)) {
      CharacterRenderDrawItem item;
      item.flags = RSF_ALPHA | RSF_DEPTH_SORTED;
      item.command = shadow;
      commands.push_back(item);
    }
  }

  int layer = 0;
  CImfRes* imf = static_cast<CImfRes*>(
    ResourceMgr::Instance().Load(g_session.GetBodyImfName(this->job, this->appearance[0], this->appearance[1], this->bodyCostume).c_str())
  );
  this->RenderRobeCommands(commands, false, view, layer, &position.x, this->screenX, this->screenY, distanceScale, 0.0f, rhw);

  std::map<int, SpriteLayerQuad> quads;
  this->layerViews[2] = this->lowerHeadgear;
  this->layerViews[3] = this->upperHeadgear;
  this->layerViews[4] = this->middleHeadgear;
  this->layerViews[8] = this->robe;

  std::vector<CActRes*> actions(9, nullptr);
  std::vector<CSprRes*> sprites(9, nullptr);
  actions[0] = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetBodyActionResourceName(
    this->riding == 1 ? g_session.GetRidingJob(this->job) : this->job, this->appearance[0], this->bodyCostume, 0
  )));
  sprites[0] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetBodySprName(
    this->riding == 1 ? g_session.GetRidingJob(this->job) : this->job, this->appearance[0], this->bodyCostume, 0
  )));
  actions[1] = static_cast<CActRes*>(
    ResourceMgr::Instance().Load(g_session.GetHeadActName(this->job, this->appearance[0], &this->appearance[1]))
  );
  sprites[1] = static_cast<CSprRes*>(
    ResourceMgr::Instance().Load(g_session.GetHeadSprName(this->job, this->appearance[0], &this->appearance[1]))
  );
  actions[2] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], this->layerViews[2])
  ));
  sprites[2] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], this->layerViews[2])
  ));
  actions[3] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], this->layerViews[3])
  ));
  sprites[3] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], this->layerViews[3])
  ));
  actions[4] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], this->layerViews[4])
  ));
  sprites[4] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], this->layerViews[4])
  ));
  if (this->isHelmRobe == 1) {
    actions[8] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
      g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], this->layerViews[8])
    ));
    sprites[8] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
      g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], this->layerViews[8])
    ));
  }

  bool hidesHead = false;
  for (int slot = 2; slot <= 3; ++slot) {
    if (g_session.IsSpecialMonsterJob(this->layerViews[slot]) == true) {
      hidesHead = true;
      break;
    }
  }

  for (int index = 8; index >= 0; --index) {
    const int action = this->actionIndex;
    const int order = GetSpriteLayerOrder(imf, index, action - action % 8, action, this->frameIndex);
    int type = order;
    if (!bodyDrawn) {
      bodyDrawn = order == SPRITE_LAYER_HEAD;
    }
    if (bodyDrawn == true && type == SPRITE_LAYER_BODY) {
      ++swapCount;
      type = SPRITE_LAYER_HEAD_LOW;
    }
    if (swapCount == 1 && order == SPRITE_LAYER_HEAD_LOW) {
      type = SPRITE_LAYER_BODY;
      swapCount = 2;
    } else if (type == SPRITE_LAYER_HEAD_LOW) {
      type = SPRITE_LAYER_HEAD_MID;
    } else if (type == SPRITE_LAYER_HEAD_MID) {
      type = SPRITE_LAYER_HEAD_LOW;
    }

    if (actions[type] == nullptr || sprites[type] == nullptr || type == SPRITE_LAYER_WEAPON || type == SPRITE_LAYER_WEAPON_EFFECT || type == SPRITE_LAYER_SHIELD) {
      continue;
    }

    int cellIndex;
    if (g_session.IsSpecialMonsterJob(this->layerViews[type])) {
      cellIndex = GetSlotLayer(1);
    } else {
      cellIndex = GetSlotLayer(type);
    }
    if (this->frameIndex >= static_cast<int>(actions[type]->FrameCount(this->actionIndex))) {
      continue;
    }

    const ActionFrame* frame = &actions[type]->Frame(this->actionIndex, this->frameIndex);
    switch (type) {
      case SPRITE_LAYER_HEAD_LOW:
      case SPRITE_LAYER_HEAD_TOP:
      case SPRITE_LAYER_HEAD_MID:
      case SPRITE_LAYER_ROBE: {
        const int bodyCount = actions[1]->FrameCount(this->actionIndex);
        const int layerCount = actions[type]->FrameCount(this->actionIndex);
        if (bodyCount > 0 && layerCount > 0 && bodyCount < layerCount && layerCount % bodyCount == 0) {
          frame = &actions[type]->Frame(this->actionIndex, this->frameIndex * (layerCount / bodyCount));
        }
        break;
      }
    }

    const SpriteFrameCell* cell = &frame->Cell(cellIndex);
    const int imageIndex = cell->image;
    if (imageIndex == -1) {
      continue;
    }

    int offsetX = 0;
    int offsetY = 0;
    SpriteQuad quad{};
    quad.image = sprites[type]->images[cell->imageType][imageIndex];
    if (g_session.IsDoramJob(this->job) == true) {
      int doramX = 0;
      int doramY = 0;
      int direction = this->actionIndex % 8;
      if (direction < 0) {
        direction = 7;
      }
      if (direction > 7) {
        direction = 0;
      }
      g_session.m_hotkeyScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", this->layerViews[type], direction, this->appearance[0], &doramX, &doramY);
      offsetX = doramX;
      offsetY = doramY;
    }

    switch (type) {
      case SPRITE_LAYER_HEAD: {
        CActRes* bodyAction = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetBodyActionResourceName(
          this->riding == 1 ? g_session.GetRidingJob(this->job) : this->job, this->appearance[0], this->bodyCostume, 0
        )));
        const ActionFrame& bodyFrame = bodyAction->Frame(this->actionIndex, this->frameIndex);
        if (frame->anchorCount > 0 && bodyFrame.anchorCount > 0) {
          const ActionAnchor& anchor = frame->anchors[0];
          const ActionAnchor& bodyAnchor = bodyFrame.anchors[0];
          if (anchor.attachmentId == bodyAnchor.attachmentId) {
            offsetX += bodyAnchor.x - anchor.x;
            offsetY += bodyAnchor.y - anchor.y;
            g_headAnchorOffsetX = offsetX;
            g_headAnchorOffsetY = offsetY;
          }
        }
        if (hidesHead == true) {
          continue;
        }
        break;
      }
      case SPRITE_LAYER_HEAD_LOW:
      case SPRITE_LAYER_HEAD_TOP:
      case SPRITE_LAYER_HEAD_MID:
      case SPRITE_LAYER_ROBE: {
        int syncedIndex = 0;
        const ActionFrame* layerFrame = g_session.GetSyncedFrame(actions[1], actions[type], this->actionIndex, this->frameIndex, 0, &syncedIndex);
        if (layerFrame != nullptr && syncedIndex != this->frameIndex) {
          cell = &layerFrame->Cell(cellIndex);
          if (cell->image != -1) {
            quad.image = sprites[type]->images[cell->imageType][cell->image];
          }
        } else {
          layerFrame = frame;
        }

        const ActionFrame& bodyFrame = actions[1]->Frame(this->actionIndex, this->frameIndex);
        if (layerFrame->anchorCount > 0 && bodyFrame.anchorCount > 0) {
          const ActionAnchor& anchor = layerFrame->anchors[0];
          const ActionAnchor& bodyAnchor = bodyFrame.anchors[0];
          if (anchor.attachmentId == bodyAnchor.attachmentId) {
            offsetX += bodyAnchor.x - anchor.x;
            offsetY += bodyAnchor.y - anchor.y;
          }
        }

        offsetX += g_headAnchorOffsetX;
        if (IsIgnoredRidingState(this->layerViews[type], this->job, this->actionIndex - this->actionIndex % 8, this->riding) == true) {
          offsetY += SpriteRidingOffset(&bodyFrame, this->actionIndex, this->frameIndex, this->job, this->appearance[0], this->bodyCostume);
        } else {
          offsetY += g_headAnchorOffsetY;
        }

        if (cell->scaleX != 1.0f) {
          offsetX = static_cast<int>(static_cast<float>(offsetX) / cell->scaleX);
        }
        if (cell->scaleY != 1.0f) {
          offsetY = static_cast<int>(static_cast<float>(offsetY) / cell->scaleY);
        }
        break;
      }
    }

    float bottomDepth = 0.0f;
    float topDepth = 0.0f;
    SpriteColor color = {};
    SpriteCellBounds rect;
    if (!this->ProjectCell(
          index, layer, distanceScale, *cell, this->screenX, this->screenY, &position.x, 0.0f, bottomDepth, topDepth, *quad.image,
          quad, rect, color, offsetX, offsetY, 0
        )) {
      continue;
    }

    *boundsUpdated = 1;
    ++layer;
    switch (type) {
      case SPRITE_LAYER_HEAD_LOW:
      case SPRITE_LAYER_HEAD_TOP:
      case SPRITE_LAYER_HEAD_MID:
      case SPRITE_LAYER_ROBE:
        this->OffsetLayerDepth(type, quad, bottomDepth, topDepth);
        break;
      default:
        this->OffsetAccessoryLayerDepth(type, quad, bottomDepth, topDepth);
        break;
    }

    if (trackBounds == 1 && (type == SPRITE_LAYER_BODY || type == SPRITE_LAYER_HEAD)) {
      this->renderBounds.Include(&quad.top);
    }

    const std::uint32_t* palette = sprites[type]->palette;
    switch (type) {
      case SPRITE_LAYER_BODY:
        if (this->appearance[2] > 0) {
          palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(
            g_session.GetBodyPalName(this->job, this->appearance[0], this->appearance[2], this->bodyCostume, 0, 0).c_str()
          ))->palette;
        } else {
          palette = sprites[0]->palette;
        }
        break;
      case SPRITE_LAYER_HEAD:
        if (this->appearance[3] > 0) {
          palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(
            g_session.GetHeadPalName(this->appearance[1], this->job, this->appearance[0], this->appearance[3]).c_str()
          ))->palette;
        } else {
          palette = sprites[1]->palette;
        }
        break;
    }

    int blend;
    switch (type) {
      case SPRITE_LAYER_HEAD_LOW:
      case SPRITE_LAYER_HEAD_TOP:
      case SPRITE_LAYER_ROBE:
        blend = 1;
        break;
      case SPRITE_LAYER_HEAD_MID:
        blend = this->middleHeadgear != ACCESSORY_BATTLEONLINE;
        break;
      default:
        blend = RSF_ALPHA | RSF_DEPTH_SORTED;
        break;
    }

    switch (type) {
      case SPRITE_LAYER_HEAD_LOW:
      case SPRITE_LAYER_HEAD_TOP:
      case SPRITE_LAYER_HEAD_MID:
      case SPRITE_LAYER_ROBE: {
        const int direction = this->actionIndex % 8;
        if (this->layerViews[type] > 0) {
          int priority = -1;
          g_session.m_hotkeyScript->Call("GetLayerPriority", "ddd>d", direction, type, this->layerViews[type], &priority);
          if (priority != -1) {
            SpriteLayerQuad entry;
            entry.quad = quad;
            entry.palette = palette;
            entry.blend = blend;
            quads.insert(std::make_pair(priority, entry));
          }
        }
        break;
      }
      default: {
        CharacterRenderDrawItem item;
        item.flags = blend;
        item.command = this->BuildSpriteQuad(quad, palette, blend, 0.0f, 0.0f);
        commands.push_back(item);
        break;
      }
    }

    if (type == SPRITE_LAYER_BODY) {
      this->bodyBlend = blend;
      this->bodyCellX = cell->x;
      this->bodyCellY = cell->y;
      this->RenderRobeCommands(commands, true, view, layer, &position.x, this->screenX, this->screenY, distanceScale, 0.0f, rhw);
    }
  }

  float maxBottom = depthBias;
  float maxTop = depthBias;
  for (std::map<int, SpriteLayerQuad>::iterator it = quads.begin(); it != quads.end(); ++it) {
    float bottom = it->second.quad.bottomRhw;
    float top = it->second.quad.topRhw;
    if (it != quads.begin()) {
      bottom = bottom > maxBottom ? bottom : maxBottom;
      top = top > maxTop ? top : maxTop;
    }
    maxBottom = bottom;
    maxTop = top;
  }

  for (std::map<int, SpriteLayerQuad>::iterator it = quads.begin(); it != quads.end(); ++it) {
    SpriteLayerQuad& entry = it->second;
    entry.quad.bottomRhw = depthBias + maxBottom;
    entry.quad.topRhw = depthBias + maxTop;
    commands.push_back({entry.blend, this->BuildSpriteQuad(entry.quad, entry.palette, entry.blend, 0.0f, 0.0f)});
    depthBias += 0.000001f;
  }

  return commands;
}
