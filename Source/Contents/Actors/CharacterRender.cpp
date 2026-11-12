#include "Contents/Actors/CharacterRender.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
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

template __declspec(noinline) std::vector<CActRes*>::vector(const size_type, CActRes* const&, const std::allocator<CActRes*>&);
template __declspec(noinline) std::vector<CSprRes*>::vector(const size_type, CSprRes* const&, const std::allocator<CSprRes*>&);
template __declspec(noinline) std::vector<CActRes*>::~vector() noexcept;
template __declspec(noinline) std::vector<CSprRes*>::~vector() noexcept;
template __declspec(noinline) void std::list<CharacterRenderDrawItem>::push_back(const CharacterRenderDrawItem&);
template __declspec(noinline) std::pair<std::map<int, SpriteLayerQuad>::iterator, bool> std::map<int, SpriteLayerQuad>::_Mybase::emplace<std::pair<int, SpriteLayerQuad>>(std::pair<int, SpriteLayerQuad>&&);
template __declspec(noinline) std::map<int, SpriteLayerQuad>::_Mybase::~_Tree() noexcept;

// OFFSET: 0x0080D2A0
// FUNCTION: fn_10099
CCharacterRender::CCharacterRender(int width, int renderHeight)
    : headJob(0), lowerHeadgear(0), upperHeadgear(0), middleHeadgear(0), lowerHeadgearLocation(0),
      upperHeadgearLocation(0), middleHeadgearLocation(0), madogearType(0), riding(0), bodyCostume(0), viewportOffsetX(0), viewportOffsetY(80.0f), bodyBlend(-1),
      bodyCellX(-1), bodyCellY(-1), viewportWidth(width), viewportHeight(renderHeight), cameraDistance(400.0f), cameraPitch(-45.0f),
      cameraYaw(0.0f), padding_007C(0.0f), padding_0080(0.0f), padding_0084(0.0f), cameraUpX(0.0f), cameraUpY(1.0f), cameraUpZ(0),
      screenX(0), screenY(0), robe(0), isHelmRobe(0) {
  this->positionX = 0;
  this->positionY = 0;
  this->positionZ = 0;
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
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const float* origin = reinterpret_cast<const float*>(&this->positionX);
  const float* view = mode->camera->frame.view.values;
  float position[3];
  position[0] = origin[0] * view[0] + origin[1] * view[3] + origin[2] * view[6] + view[9];
  position[1] = origin[0] * view[1] + origin[1] * view[4] + origin[2] * view[7] + view[10];
  position[2] = origin[0] * view[2] + origin[1] * view[5] + origin[2] * view[8] + view[11];
  float depthBias = 0.0f;

  float& screenX = *reinterpret_cast<float*>(&this->screenX);
  float& screenY = *reinterpret_cast<float*>(&this->screenY);
  screenX = 0.0f;
  screenY = 0.0f;
  float rhw = 0.0f;
  g_renderer->ProjectViewport(origin, view, &screenX, &screenY, &rhw);
  float distanceScale = g_renderer->ScaleDistance(rhw);
  distanceScale *= GetSkillFactor(this->job);

  int swapCount = 0;
  bool bodyDrawn = false;
  *boundsUpdated = 0;
  RenderBounds& bounds = *reinterpret_cast<RenderBounds*>(this->renderBounds);
  bounds.Set(static_cast<float>(g_renderer->viewport.width), static_cast<float>(g_renderer->viewport.height), 0.0f, 0.0f, rhw);

  std::list<CharacterRenderDrawItem> commands;
  if (this->drawShadow == 1) {
    if (RenderPrimitive* shadow = this->DrawShadow(view, 1.0f)) {
      CharacterRenderDrawItem item;
      item.flags = 0x101;
      item.command = shadow;
      commands.push_back(item);
    }
  }

  int layer = 0;
  CImfRes* imf = static_cast<CImfRes*>(
    ResourceMgr::Instance().Load(g_session.GetBodyImfName(this->job, this->appearance[0], this->appearance[1], this->bodyCostume).c_str())
  );
  this->RenderRobeCommands(commands, false, view, layer, position, screenX, screenY, distanceScale, 0.0f, rhw);

  std::map<int, SpriteLayerQuad> quads;
  std::map<int, int>& views = this->layerViews;
  views[2] = this->lowerHeadgear;
  views[3] = this->upperHeadgear;
  views[4] = this->middleHeadgear;
  views[8] = this->robe;

  std::vector<CActRes*> actions(9, nullptr);
  std::vector<CSprRes*> sprites(9, nullptr);
  const int job = this->riding == 1 ? g_session.GetRidingJob(this->job) : this->job;
  actions[0] = static_cast<CActRes*>(
    ResourceMgr::Instance().Load(g_session.GetBodyActionResourceName(job, this->appearance[0], this->bodyCostume, 0))
  );
  const int spriteJob = this->riding == 1 ? g_session.GetRidingJob(this->job) : this->job;
  sprites[0] = static_cast<CSprRes*>(
    ResourceMgr::Instance().Load(g_session.GetBodySprName(spriteJob, this->appearance[0], this->bodyCostume, 0))
  );
  actions[1] = static_cast<CActRes*>(
    ResourceMgr::Instance().Load(g_session.GetHeadActName(this->job, this->appearance[0], &this->appearance[1]))
  );
  sprites[1] = static_cast<CSprRes*>(
    ResourceMgr::Instance().Load(g_session.GetHeadSprName(this->job, this->appearance[0], &this->appearance[1]))
  );
  actions[2] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], views[2])
  ));
  sprites[2] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], views[2])
  ));
  actions[3] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], views[3])
  ));
  sprites[3] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], views[3])
  ));
  actions[4] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], views[4])
  ));
  sprites[4] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
    g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], views[4])
  ));
  if (this->isHelmRobe == 1) {
    actions[8] = static_cast<CActRes*>(ResourceMgr::Instance().Load(
      g_session.GetAccessoryActName(this->job, this->appearance[0], this->appearance[1], views[8])
    ));
    sprites[8] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(
      g_session.GetAccessorySprName(this->job, this->appearance[0], this->appearance[1], views[8])
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
    const int action = this->actionIndex;
    const int order = GetSpriteLayerOrder(imf, index, action - action % 8, action, this->frameIndex);
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
    if (this->frameIndex >= static_cast<int>(actions[type]->FrameCount(this->actionIndex))) {
      continue;
    }

    const ActionFrame* frame = &actions[type]->Frame(this->actionIndex, this->frameIndex);
    switch (type) {
      case 2:
      case 3:
      case 4:
      case 8: {
        const int bodyCount = actions[1]->FrameCount(this->actionIndex);
        const int layerCount = actions[type]->FrameCount(this->actionIndex);
        if (bodyCount > 0 && layerCount > 0 && bodyCount < layerCount && layerCount % bodyCount == 0) {
          frame = &actions[type]->Frame(this->actionIndex, this->frameIndex * (layerCount / bodyCount));
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
      g_session.m_hotkeyScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", views[type], direction, this->appearance[0], &doramX, &doramY);
      offsetX = doramX;
      offsetY = doramY;
    }

    switch (type) {
      case 1: {
        const int bodyJob = this->riding == 1 ? g_session.GetRidingJob(this->job) : this->job;
        CActRes* bodyAction = static_cast<CActRes*>(
          ResourceMgr::Instance().Load(g_session.GetBodyActionResourceName(bodyJob, this->appearance[0], this->bodyCostume, 0))
        );
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
      case 2:
      case 3:
      case 4:
      case 8: {
        int syncedIndex = 0;
        const ActionFrame* layerFrame = g_session.GetSyncedFrame(actions[1], actions[type], this->actionIndex, this->frameIndex, 0, &syncedIndex);
        if (layerFrame != nullptr && syncedIndex != this->frameIndex) {
          cell = &layerFrame->Cell(cellIndex);
          if (cell->image != -1) {
            image = sprites[type]->images[cell->imageType][cell->image];
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
        const int baseAction = this->actionIndex - this->actionIndex % 8;
        if (IsIgnoredRidingState(views[type], this->job, baseAction, this->riding) == true) {
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
    SpriteQuad quad;
    std::memset(&quad, 0, sizeof(quad));
    SpriteColor color = {};
    tagRECT rect;
    if (!this->ProjectCell(
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
        this->OffsetLayerDepth(type, reinterpret_cast<SpriteCellQuad&>(quad), bottomDepth, topDepth);
        break;
      default:
        this->OffsetAccessoryLayerDepth(type, reinterpret_cast<SpriteCellQuad&>(quad), bottomDepth, topDepth);
        break;
    }

    if (trackBounds == 1 && (type == 0 || type == 1)) {
      bounds.Include(&quad.top);
    }

    const std::uint32_t* palette = sprites[type]->palette;
    if (type == 0) {
      if (this->appearance[2] > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(
          g_session.GetBodyPalName(this->job, this->appearance[0], this->appearance[2], this->bodyCostume, 0, 0).c_str()
        ))->palette;
      } else {
        palette = sprites[0]->palette;
      }
    } else if (type == 1) {
      if (this->appearance[3] > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(
          g_session.GetHeadPalName(this->appearance[1], this->job, this->appearance[0], this->appearance[3]).c_str()
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
        blend = this->middleHeadgear != 0xB9;
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
        const int direction = this->actionIndex % 8;
        if (views[type] > 0) {
          int priority = -1;
          g_session.m_hotkeyScript->Call("GetLayerPriority", "ddd>d", direction, type, views[type], &priority);
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
      this->bodyBlend = blend;
      this->bodyCellX = cell->x;
      this->bodyCellY = cell->y;
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
    commands.push_back({entry.blend, BuildSpriteQuad(*GetRenderSystemHolder()->system, entry.quad, entry.palette, entry.blend, 0.0f, 0.0f)});
    depthBias += 0.000001f;
  }

  return commands;
}
