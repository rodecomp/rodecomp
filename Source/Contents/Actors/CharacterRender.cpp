#include "Contents/Actors/CharacterRender.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Actors/SpriteDrawingPolicy.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/SpriteLayerDrawing.h"
#include "Contents/SpriteLayerOrder.h"
#include "Contents/SpriteRidingOffset.h"
#include "Contents/SpriteRidingPolicy.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Render/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ImfResource.h"
#include "Resource/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"

#include <algorithm>
#include <cstring>
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
CCharacterRender::CCharacterRender(int unknown0068, int unknown006C)
    : unknown_0004(0), unknown_0008(0), unknown_000C(0), unknown_0014(0), unknown_0028(0), unknown_002C(0), unknown_0030(0), unknown_0034(0),
      unknown_0038(0), unknown_003C(0), unknown_0048(0), unknown_004C(0), unknown_0050(0), unknown_0054(0), unknown_0058(80.0f), unknown_005C(-1),
      unknown_0060(-1), unknown_0064(-1), unknown_0068(unknown0068), unknown_006C(unknown006C), unknown_0070(400.0f), unknown_0074(-45.0f),
      unknown_0078(0.0f), unknown_007C(0.0f), unknown_0080(0.0f), unknown_0084(0.0f), unknown_0088(0.0f), unknown_008C(1.0f), unknown_0090(0),
      unknown_00A8(0), unknown_00AC(0), unknown_00BC(0), unknown_00C0(0) {
  this->unknown_0004 = 0;
  this->unknown_0008 = 0;
  this->unknown_000C = 0;
  this->unknown_0040 = 0;
  this->unknown_0044 = 0;
  this->unknown_00B0 = 1;
}

// OFFSET: 0x0080D730
// FUNCTION: fn_10101
CCharacterRender::~CCharacterRender() = default;

// OFFSET: 0x008105A0
// FUNCTION: fn_10108
std::list<CharacterRenderDrawItem> CCharacterRender::BuildDrawList(unsigned char trackBounds, char* boundsUpdated) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float* origin = reinterpret_cast<const float*>(&this->unknown_0004);
  const float* view = mode->camera->frame.view.values;
  float position[3];
  position[0] = origin[0] * view[0] + origin[1] * view[3] + origin[2] * view[6] + view[9];
  position[1] = origin[0] * view[1] + origin[1] * view[4] + origin[2] * view[7] + view[10];
  position[2] = origin[0] * view[2] + origin[1] * view[5] + origin[2] * view[8] + view[11];
  float depthBias = 0.0f;

  float& screenX = *reinterpret_cast<float*>(&this->unknown_00A8);
  float& screenY = *reinterpret_cast<float*>(&this->unknown_00AC);
  screenX = 0.0f;
  screenY = 0.0f;
  float rhw = 0.0f;
  g_renderer->ProjectViewport(origin, view, &screenX, &screenY, &rhw);
  float distanceScale = g_renderer->ScaleDistance(rhw);
  distanceScale *= GetSkillFactor(this->unknown_0010);

  int swapCount = 0;
  bool bodyDrawn = false;
  *boundsUpdated = 0;
  RenderBounds& bounds = *reinterpret_cast<RenderBounds*>(this->unknown_0094);
  bounds.Set(static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.height), 0.0f, 0.0f, rhw);

  std::list<CharacterRenderDrawItem> commands;
  if (this->unknown_00B0 == 1) {
    if (RenderPrimitive* shadow = reinterpret_cast<const SpriteDrawingPolicy*>(this)->DrawShadow(view, 1.0f)) {
      CharacterRenderDrawItem item;
      item.flags = 0x101;
      item.command = shadow;
      commands.push_back(item);
    }
  }

  int layer = 0;
  CImfRes* imf = static_cast<CImfRes*>(
    ResourceMgr::Instance().Load(GetBodyImfName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], this->unknown_0050).c_str())
  );
  this->RenderRobeCommands(commands, false, view, layer, position, screenX, screenY, distanceScale, 0.0f, rhw);

  std::map<int, SpriteLayerQuad> quads;
  std::map<int, int>& views = this->unknown_00B4;
  views[2] = this->unknown_0028;
  views[3] = this->unknown_002C;
  views[4] = this->unknown_0030;
  views[8] = this->unknown_00BC;

  std::vector<CActRes*> actions(9, nullptr);
  std::vector<CSprRes*> sprites(9, nullptr);
  const int job = this->unknown_004C == 1 ? g_session.GetRidingJob(this->unknown_0010) : this->unknown_0010;
  actions[0] = static_cast<CActRes*>(
    ResourceMgr::Instance().Load(GetBodyActionResourceName(job, this->unknown_0018[0], this->unknown_0050, 0))
  );
  const int spriteJob = this->unknown_004C == 1 ? g_session.GetRidingJob(this->unknown_0010) : this->unknown_0010;
  sprites[0] = static_cast<CSprRes*>(
    ResourceMgr::Instance().Load(g_session.GetBodySprName(spriteJob, this->unknown_0018[0], this->unknown_0050, 0))
  );
  actions[1] = static_cast<CActRes*>(
    ResourceMgr::Instance().Load(g_session.GetHeadActName(this->unknown_0010, this->unknown_0018[0], &this->unknown_0018[1]))
  );
  sprites[1] = static_cast<CSprRes*>(
    ResourceMgr::Instance().Load(g_session.GetHeadSprName(this->unknown_0010, this->unknown_0018[0], &this->unknown_0018[1]))
  );
  actions[2] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[2])
  ));
  sprites[2] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[2])
  ));
  actions[3] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[3])
  ));
  sprites[3] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[3])
  ));
  actions[4] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[4])
  ));
  sprites[4] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[4])
  ));
  if (this->unknown_00C0 == 1) {
    actions[8] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
      g_session.GetAccessoryActName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[8])
    ));
    sprites[8] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
      g_session.GetAccessorySprName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[1], views[8])
    ));
  }

  bool hidesHead = false;
  for (int slot = 2; slot <= 3; ++slot) {
    if (g_session.IsSpecialMonsterJob(views[slot]) == true) {
      hidesHead = true;
      break;
    }
  }

  for (int index = 8; index >= 0; --index) {
    const int action = this->unknown_0040;
    const int order = GetSpriteLayerOrder(imf, index, action - action % 8, action, this->unknown_0044);
    int type = order;
    if (!bodyDrawn) {
      bodyDrawn = order == 1;
    }
    if (bodyDrawn == true && type == 0) {
      ++swapCount;
      type = 2;
    }
    if (swapCount == 1 && order == 2) {
      type = 0;
      swapCount = 2;
    } else if (type == 2) {
      type = 4;
    } else if (type == 4) {
      type = 2;
    }

    if (actions[type] == nullptr || sprites[type] == nullptr || type == 5 || type == 6 || type == 7) {
      continue;
    }

    const int cellIndex = GetSlotLayer(g_session.IsSpecialMonsterJob(views[type]) ? 1 : type);
    if (this->unknown_0044 >= static_cast<int>(actions[type]->FrameCount(this->unknown_0040))) {
      continue;
    }

    const ActionFrame* frame = &actions[type]->Frame(this->unknown_0040, this->unknown_0044);
    switch (type) {
      case 2:
      case 3:
      case 4:
      case 8: {
        const int bodyCount = actions[1]->FrameCount(this->unknown_0040);
        const int layerCount = actions[type]->FrameCount(this->unknown_0040);
        if (bodyCount > 0 && layerCount > 0 && bodyCount < layerCount && layerCount % bodyCount == 0) {
          frame = &actions[type]->Frame(this->unknown_0040, this->unknown_0044 * (layerCount / bodyCount));
        }
        break;
      }
    }

    const SpriteFrameCell* cell = &frame->Cell(cellIndex);
    if (cell->image == -1) {
      continue;
    }

    int offsetX = 0;
    int offsetY = 0;
    const SpriteRleImage* image = sprites[type]->images[cell->imageType][cell->image];
    if (g_session.IsDoramJob(this->unknown_0010) == true) {
      int doramX = 0;
      int doramY = 0;
      int direction = this->unknown_0040 % 8;
      if (direction < 0) {
        direction = 7;
      }
      if (direction > 7) {
        direction = 0;
      }
      g_luaScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", views[type], direction, this->unknown_0018[0], &doramX, &doramY);
      offsetX = doramX;
      offsetY = doramY;
    }

    switch (type) {
      case 1: {
        const int bodyJob = this->unknown_004C == 1 ? g_session.GetRidingJob(this->unknown_0010) : this->unknown_0010;
        CActRes* bodyAction = static_cast<CActRes*>(
          ResourceMgr::Instance().Load(GetBodyActionResourceName(bodyJob, this->unknown_0018[0], this->unknown_0050, 0))
        );
        const ActionFrame& bodyFrame = bodyAction->Frame(this->unknown_0040, this->unknown_0044);
        if (frame->anchorCount > 0 && bodyFrame.anchorCount > 0) {
          const ActionAnchor& anchor = frame->anchors[0];
          const ActionAnchor& bodyAnchor = bodyFrame.anchors[0];
          if (anchor.unknown_0008 == bodyAnchor.unknown_0008) {
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
      case 2:
      case 3:
      case 4:
      case 8: {
        int syncedIndex = 0;
        const ActionFrame* layerFrame = g_session.GetSyncedFrame(actions[1], actions[type], this->unknown_0040, this->unknown_0044, 0, &syncedIndex);
        if (layerFrame != nullptr && syncedIndex != this->unknown_0044) {
          cell = &layerFrame->Cell(cellIndex);
          if (cell->image != -1) {
            image = sprites[type]->images[cell->imageType][cell->image];
          }
        } else {
          layerFrame = frame;
        }

        const ActionFrame& bodyFrame = actions[1]->Frame(this->unknown_0040, this->unknown_0044);
        if (layerFrame->anchorCount > 0 && bodyFrame.anchorCount > 0) {
          const ActionAnchor& anchor = layerFrame->anchors[0];
          const ActionAnchor& bodyAnchor = bodyFrame.anchors[0];
          if (anchor.unknown_0008 == bodyAnchor.unknown_0008) {
            offsetX += bodyAnchor.x - anchor.x;
            offsetY += bodyAnchor.y - anchor.y;
          }
        }

        offsetX += g_headAnchorOffsetX;
        const int baseAction = this->unknown_0040 - this->unknown_0040 % 8;
        if (IsIgnoredRidingState(views[type], this->unknown_0010, baseAction, this->unknown_004C) == true) {
          offsetY += SpriteRidingOffset(&bodyFrame, this->unknown_0040, this->unknown_0044, this->unknown_0010, this->unknown_0018[0], this->unknown_0050);
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
    SpriteQuad quad;
    std::memset(&quad, 0, sizeof(quad));
    SpriteColor color = {};
    tagRECT rect;
    if (!reinterpret_cast<const SpriteDrawingPolicy*>(this)->ProjectCell(
          index, layer, distanceScale, *cell, screenX, screenY, position, 0.0f, bottomDepth, topDepth, *image,
          reinterpret_cast<SpriteCellQuad&>(quad), rect, color, offsetX, offsetY, 0
        )) {
      continue;
    }

    *boundsUpdated = 1;
    ++layer;
    switch (type) {
      case 2:
      case 3:
      case 4:
      case 8:
        reinterpret_cast<const SpriteDrawingPolicy*>(this)->OffsetLayerDepth(type, reinterpret_cast<SpriteCellQuad&>(quad), bottomDepth, topDepth);
        break;
      default:
        reinterpret_cast<const AccessoryLayerDepth*>(this)->OffsetLayerDepth(type, reinterpret_cast<SpriteCellQuad&>(quad), bottomDepth, topDepth);
        break;
    }

    if (trackBounds == 1 && (type == 0 || type == 1)) {
      bounds.Include(&quad.top);
    }

    const std::uint32_t* palette = sprites[type]->palette;
    if (type == 0) {
      if (this->unknown_0018[2] > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(
          g_session.GetBodyPalName(this->unknown_0010, this->unknown_0018[0], this->unknown_0018[2], this->unknown_0050, 0, 0).c_str()
        ))->palette;
      } else {
        palette = sprites[0]->palette;
      }
    } else if (type == 1) {
      if (this->unknown_0018[3] > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(
          g_session.GetHeadPalName(this->unknown_0018[1], this->unknown_0010, this->unknown_0018[0], this->unknown_0018[3]).c_str()
        ))->palette;
      } else {
        palette = sprites[1]->palette;
      }
    }

    int blend;
    switch (type) {
      case 2:
      case 3:
      case 8:
        blend = 1;
        break;
      case 4:
        blend = this->unknown_0030 != 0xB9;
        break;
      default:
        blend = 0x101;
        break;
    }

    switch (type) {
      case 2:
      case 3:
      case 4:
      case 8: {
        const int direction = this->unknown_0040 % 8;
        if (views[type] > 0) {
          int priority = -1;
          g_luaScript->Call("GetLayerPriority", "ddd>d", direction, type, views[type], &priority);
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
        item.command = BuildSpriteQuad(*GetRenderSystemHolder()->system, quad, palette, blend, 0.0f, 0.0f);
        commands.push_back(item);
        break;
      }
    }

    if (type == 0) {
      this->unknown_005C = blend;
      this->unknown_0060 = cell->x;
      this->unknown_0064 = cell->y;
      this->RenderRobeCommands(commands, true, view, layer, position, screenX, screenY, distanceScale, 0.0f, rhw);
    }
  }

  float maxBottom = depthBias;
  float maxTop = depthBias;
  for (std::map<int, SpriteLayerQuad>::iterator it = quads.begin(); it != quads.end(); ++it) {
    float bottom = it->second.quad.bottomRhw;
    float top = it->second.quad.topRhw;
    if (it != quads.begin()) {
      bottom = (std::max)(maxBottom, bottom);
      top = (std::max)(maxTop, top);
    }
    maxBottom = bottom;
    maxTop = top;
  }

  for (std::map<int, SpriteLayerQuad>::iterator it = quads.begin(); it != quads.end(); ++it) {
    SpriteLayerQuad& entry = it->second;
    entry.quad.bottomRhw = depthBias + maxBottom;
    entry.quad.topRhw = depthBias + maxTop;
    CharacterRenderDrawItem item;
    item.command = BuildSpriteQuad(*GetRenderSystemHolder()->system, entry.quad, entry.palette, entry.blend, 0.0f, 0.0f);
    item.flags = entry.blend;
    commands.push_back(item);
    depthBias += 0.000001f;
  }

  return commands;
}
