#include "Contents/Sprites/SpriteLayerDrawing.h"
#include "Contents/Actors/AccessoryId.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Jobs/JobId.h"

#include "Client/Session/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Sprites/SpriteLayerOrder.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Contents/Sprites/SpriteRidingPolicy.h"
#include "Render/Renderer.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Resource/Sprites/ImfResource.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"

#include <cstring>
#include <tuple>

// INLINED
static void TransformPoint(vector3d& out, const vector3d& point, const float* matrix) {
  out.x = point.x * matrix[0] + point.y * matrix[3] + point.z * matrix[6] + matrix[9];
  out.y = point.x * matrix[1] + point.y * matrix[4] + point.z * matrix[7] + matrix[10];
  out.z = point.x * matrix[2] + point.y * matrix[5] + point.z * matrix[8] + matrix[11];
}

// OFFSET: 0x0063F500
// FUNCTION: fn_04807
void SpriteLayerRenderer::Render(const float* matrix, int blendMode, char blendVariant) {
  if (!this->pc) {
    return;
  }

  std::memcpy(this->matrix, matrix, sizeof(this->matrix));
  this->blendMode = blendMode;
  this->blendVariant = blendVariant;
  this->PrepareLayers();

  TransformPoint(this->position, this->pc->render.position, matrix);
  g_renderer->ProjectViewport(&this->pc->render.position.x, matrix, &this->x, &this->y, &this->rhw);
  this->distanceScale = g_renderer->ScaleDistance(this->rhw);
  this->pc->render.distanceScale = this->distanceScale / g_heightScale;
  this->pc->ScaleByPulse(this->distanceScale);
  this->distanceScale *= g_session.GetSkillFactor(this->pc->GetEffectiveJob(-1));
  this->scale = g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->pc->render.spriteOffsetX)) * this->pc->render.scale) *
                this->pc->render.distanceScale;

  if (this->pc->HasAppearanceOverride()) {
    const int layer = this->DrawBodyOutline(0);
    this->pc->DrawBody(
      reinterpret_cast<int>(matrix), layer, &this->position.x, this->x, this->y, this->distanceScale, this->scale, this->rhw
    );
    return;
  }

  this->DrawRobe(false);
  this->imf = static_cast<CImfRes*>(ResourceMgr::Instance().Load(this->pc->imfName));
  auto& viewportRef = g_renderer->viewport;
  this->bounds.Set(static_cast<float>(viewportRef.width), static_cast<float>(viewportRef.height), 0.0f, 0.0f, this->rhw);

  bool bodyDrawn = false;
  bool swapped = false;
  if (this->pc->outlineColor) {
    this->DrawLayerOutlines(this->pc->outlineColor);
  }

  for (int index = 8; index >= 0; --index) {
    int layer = GetSpriteLayerOrder(this->imf, index, this->pc->render.animation, this->pc->render.previousAnimation, this->pc->render.curMotion);
    if (!bodyDrawn) {
      bodyDrawn = layer == SPRITE_LAYER_HEAD;
    }

    if (layer == SPRITE_LAYER_BODY) {
      if (bodyDrawn == true) {
        swapped = true;
        layer = SPRITE_LAYER_HEAD_MID;
      }
    } else if (layer == SPRITE_LAYER_HEAD_LOW && swapped == true) {
      layer = SPRITE_LAYER_BODY;
    } else if (layer == SPRITE_LAYER_HEAD_LOW) {
      layer = SPRITE_LAYER_HEAD_MID;
    } else if (layer == SPRITE_LAYER_HEAD_MID) {
      layer = SPRITE_LAYER_HEAD_LOW;
    } else if (static_cast<unsigned int>(layer) > 8) {
      continue;
    }

    if (!this->ShouldDrawLayer(layer)) {
      continue;
    }

    const std::pair<bool, SpriteLayerResources> found = this->FindLayer(layer);
    if (!found.first) {
      continue;
    }

    CActRes* action = found.second.action;
    const int animation = this->pc->render.previousAnimation;
    if (animation >= static_cast<int>(action->animations.size())) {
      continue;
    }

    if (this->pc->render.curMotion >= static_cast<int>(action->FrameCount(animation))) {
      continue;
    }

    int stanceAnimation;
    int frameIndex = 0;
    bool replaced = false;
    const ActionFrame& frame = this->pc->SelectStanceFrame(layer, action, &stanceAnimation, &frameIndex, &replaced);
    std::pair<bool, std::string> sound = this->GetLayerSound(layer, action, frame.sound);
    if (sound.first == true) {
      this->pc->isPlayHitWave = 1;
      std::string name = sound.second;
      StringFormat(this->pc->hitWaveName, 0x80, static_cast<std::size_t>(-1), "%s", name.size() == 0 ? this->pc->hitWaveName : name.c_str());
    }

    const std::pair<bool, const ActionFrame*> synced = this->FindSyncedFrame(layer, action);
    this->DrawLayer(index, layer, synced.first == true ? synced.second : &frame, found.second, stanceAnimation, frameIndex, replaced);
  }

  this->DrawQueuedLayers();

  if (this->alternateLayerRendering == true) {
    this->pc->SetRenderInfo(&this->bounds.left, this->x, this->y);
  } else {
    this->pc->SetTlvert(this->x, this->y);
  }

  this->pc->render.flagsResetPending = 1;
}

// OFFSET: 0x0063FC90
// FUNCTION: fn_04808
void SpriteLayerRenderer::QueueQuad(int layer, const SpriteQuad* quad, const std::uint32_t* palette, int blend) {
  const int direction = this->pc->render.previousAnimation - this->pc->render.animation;
  SpriteLayerQuad entry;
  if (this->layerViews[layer] > 0) {
    int priority = -1;
    g_session.m_hotkeyScript->Call("GetLayerPriority", "ddd>d", direction, layer, this->layerViews[layer], &priority);
    if (priority == -1) {
      return;
    }

    std::map<int, SpriteLayerQuad>& quads = this->quads;
    memcpy_s(&entry.quad, sizeof(entry.quad), quad, sizeof(SpriteQuad));
    entry.palette = palette;
    entry.blend = blend;
    quads.insert(std::make_pair(priority, entry));
  } else {
    std::map<int, SpriteLayerQuad>& quads = this->quads;
    memcpy_s(&entry.quad, sizeof(entry.quad), quad, sizeof(SpriteQuad));
    entry.palette = palette;
    entry.blend = blend;
    quads.emplace(this->quads.size() + 1, entry);
  }
}

// OFFSET: 0x00640050
// FUNCTION: fn_04809
void SpriteLayerRenderer::DrawLayerOutlines(std::uint32_t color) {
  if (this->pc->render.flags & 0x30000) {
    return;
  }

  const float offset = this->rhw * 1000.0f + 2.0f;
  for (int layer = 1; layer >= 0; --layer) {
    const int key = layer;
    const auto found = this->layers.find(key);
    if (found == this->layers.end()) {
      continue;
    }

    CSprRes* sprite = found->second.sprite;
    CActRes* action = found->second.action;
    int animation;
    int frameIndex = 0;
    bool replaced = false;
    const ActionFrame* frame = &this->pc->SelectStanceFrame(layer, action, &animation, &frameIndex, &replaced);
    if (!frame) {
      continue;
    }

    const int cellIndex = GetSlotLayer(!g_session.IsSpecialMonsterJob(this->layerViews[layer]) ? layer : 1);
    const SpriteFrameCell* cell = &frame->Cell(cellIndex);
    if (!cell || !sprite) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell->imageType][cell->image];
    if (!image) {
      continue;
    }

    std::pair<int, int> itemOffset = this->GetItemOffset(layer);
    const SpriteLayerCell resolved = this->ResolveCell(layer, cell, cellIndex, sprite, action, frame, image, animation, frameIndex, replaced);
    itemOffset.first += resolved.offsetX;
    itemOffset.second += resolved.offsetY;
    this->pc->ApplySwing(layer, itemOffset.first, itemOffset.second, this->swingX, this->swingY);

    float topRhw = 0.0f;
    float bottomRhw = 0.0f;
    SpriteQuad quad;
    tagRECT box;
    std::uint32_t quadColor;
    if (!this->pc->PrepareSprite(
          layer, this->layerShift, this->distanceScale, *resolved.cell, this->x, this->y, &this->position.x, this->scale, &bottomRhw,
          &topRhw, resolved.image, &quad, &box, &quadColor, itemOffset.first, itemOffset.second, 0
        )) {
      continue;
    }

    quad.image = resolved.image;
    const std::uint32_t* palette = this->GetPalette(layer, sprite, action);
    const float negative = -offset;
    SpriteQuad outline = quad;
    outline.color = color;

    this->QueueOutline(layer, negative, 0.0f, palette, quad, outline);
    this->QueueOutline(layer, negative, negative, palette, quad, outline);
    this->QueueOutline(layer, offset, negative, palette, quad, outline);
    this->QueueOutline(layer, negative, offset, palette, quad, outline);
    this->QueueOutline(layer, offset, offset, palette, quad, outline);
    this->QueueOutline(layer, offset, 0.0f, palette, quad, outline);
    this->QueueOutline(layer, negative, 0.0f, palette, quad, outline);
  }
}

// OFFSET: 0x00640510
// FUNCTION: fn_04810
void SpriteLayerRenderer::DrawRobe(bool drawOnTop) {
  if (this->pc->robe <= 0 || this->pc->isHelmRobe == true || this->pc->render.state == ST_DEAD ||
      (this->pc->blend == -1 && this->pc->layerCellX == -1 && this->pc->layerCellY == -1)) {
    return;
  }

  if (drawOnTop != this->pc->IsDrawOnTop()) {
    return;
  }

  if (this->pc->sex == 0) {
    switch (this->pc->GetEffectiveJob(-1)) {
      case JT_MADOGEAR:
      case JT_MADOGEAR_H:
      case JT_MADOGEAR_B:
        return;
    }
  }

  std::string sprName = g_session.GetRobeSprName(
    this->pc->GetEffectiveJob(-1), this->pc->appearanceJob, this->pc->sex, this->pc->robe, this->pc->isRiding, this->pc->GetBodyCostume()
  );
  std::string actName = g_session.GetRobeActName(
    this->pc->GetEffectiveJob(-1), this->pc->appearanceJob, this->pc->sex, this->pc->robe, this->pc->isRiding, this->pc->GetBodyCostume()
  );
  if (!ResourceMgr::Instance().Exists(sprName.c_str()) || !ResourceMgr::Instance().Exists(actName.c_str())) {
    return;
  }

  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprName.c_str()));
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actName.c_str()));
  if (!sprite || !action) {
    return;
  }

  int frameIndex = this->pc->render.curMotion;
  int baseFrame = this->pc->render.curMotion;
  const int animation = this->pc->render.previousAnimation;
  if ((animation >= 0 && animation < 8) || (animation >= 16 && animation <= 22)) {
    baseFrame = 0;
  }

  g_session.GetSyncedFrame(
    static_cast<CActRes*>(this->pc->GetEquipAction(1)), action, animation, baseFrame, this->pc->render.motionStartTime, &frameIndex
  );
  const ActionFrame& frame = action->Frame(this->pc->render.previousAnimation, frameIndex);
  for (int index = 0; index < frame.cellCount; ++index) {
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    reinterpret_cast<unsigned char*>(&const_cast<SpriteFrameCell&>(cell).color)[3] =
      static_cast<unsigned char>(g_session.GetRobeAlpha(this->pc->robe));
    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];

    float bottomRhw = 0.0f;
    float topRhw = 0.0f;
    SpriteQuad quad;
    tagRECT box;
    std::uint32_t quadColor;
    this->pc->PrepareSprite(
      this->layerShift, this->layerShift, this->distanceScale, cell, this->x, this->y, &this->position.x, this->scale, &bottomRhw, &topRhw,
      image, &quad, &box, &quadColor, 0, 0, !drawOnTop
    );
    if (this->pc->render.translucentOther[0] == 1) {
      quad.color = static_cast<std::uint32_t>(static_cast<unsigned char>(static_cast<int>(static_cast<float>(quad.color >> 24) * (70.0f / 255.0f)))) << 24 |
                   (quad.color & 0xFFFFFF);
    }

    quad.image = image;
    int blend = this->pc->blend;
    this->pc->ApplyLayerEffects(
      &quad, &cell, 0, &box, this->scale, bottomRhw, bottomRhw, topRhw, this->x, this->y, sprite->palette, image, &blend, 0, 0
    );
    if (drawOnTop && g_session.IsTopLayerRobe(this->pc->robe)) {
      this->quads.insert(std::make_pair(400u, SpriteLayerQuad{quad, sprite->palette, blend}));
    } else {
      this->QueueQuad(0, &quad, sprite->palette, blend);
    }
  }
}

// OFFSET: 0x00640AD0
// FUNCTION: fn_04811
int SpriteLayerRenderer::DrawBodyOutline(int layer) {
  if (!this->pc) {
    return layer;
  }

  std::uint32_t color = this->pc->outlineColor;
  if (!color) {
    return layer;
  }

  if (g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
    color = ((((0x46u << 8) | static_cast<std::uint8_t>(color >> 16)) << 8 | static_cast<std::uint8_t>(color >> 8)) << 8) | static_cast<std::uint8_t>(color);
  }

  const float offset = this->rhw * 1000.0f + 2.0f;
  this->pc->RenderBodyOutline(layer++, &this->position.x, this->x - offset, this->y + 0.0f, this->distanceScale, this->scale, this->rhw, color);
  this->pc->RenderBodyOutline(layer++, &this->position.x, this->x - offset, this->y - offset, this->distanceScale, this->scale, this->rhw, color);
  this->pc->RenderBodyOutline(layer++, &this->position.x, offset + this->x, this->y - offset, this->distanceScale, this->scale, this->rhw, color);
  this->pc->RenderBodyOutline(layer++, &this->position.x, this->x - offset, offset + this->y, this->distanceScale, this->scale, this->rhw, color);
  this->pc->RenderBodyOutline(layer++, &this->position.x, offset + this->x, offset + this->y, this->distanceScale, this->scale, this->rhw, color);
  this->pc->RenderBodyOutline(layer++, &this->position.x, offset + this->x, this->y + 0.0f, this->distanceScale, this->scale, this->rhw, color);
  this->pc->RenderBodyOutline(layer++, &this->position.x, this->x - offset, this->y + 0.0f, this->distanceScale, this->scale, this->rhw, color);
  return layer;
}

// OFFSET: 0x00640ED0
// FUNCTION: fn_04812
SpriteLayerCell SpriteLayerRenderer::ResolveCell(
  int layer, const SpriteFrameCell* cell, int cellIndex, CSprRes* sprite, CActRes*, const ActionFrame* frame,
  const SpriteRleImage* image, int animation, int frameIndex, bool replaced
) {
  int offsetY = 0;
  int offsetX = 0;
  switch (layer) {
    case SPRITE_LAYER_HEAD: {
      const ActionFrame& body = static_cast<CActRes*>(this->pc->GetEquipAction(0))->Frame(this->pc->render.previousAnimation, this->pc->render.curMotion);
      if (frame->anchorCount > 0 && body.anchorCount > 0) {
        if (frame->anchors[0].attachmentId == body.anchors[0].attachmentId) {
          offsetX = body.anchors[0].x - frame->anchors[0].x;
          offsetY = body.anchors[0].y - frame->anchors[0].y;
          this->swingX = offsetX;
          this->swingY = offsetY;
        }
      }
      break;
    }
    case SPRITE_LAYER_HEAD_LOW:
    case SPRITE_LAYER_HEAD_TOP:
    case SPRITE_LAYER_HEAD_MID:
    case SPRITE_LAYER_ROBE: {
      int syncedFrame = 0;
      const ActionFrame* synced = g_session.GetSyncedFrame(
        static_cast<CActRes*>(this->pc->GetEquipAction(1)), static_cast<CActRes*>(this->pc->GetEquipAction(layer)),
        this->pc->render.previousAnimation, this->pc->render.curMotion, this->pc->render.motionStartTime, &syncedFrame
      );
      if (synced && syncedFrame != this->pc->render.curMotion) {
        frame = synced;
        cell = &synced->Cell(cellIndex);
        if (cell->image != -1) {
          image = sprite->images[cell->imageType][cell->image];
        }
      }

      CActRes* body = static_cast<CActRes*>(this->pc->GetEquipAction(1));
      const ActionFrame* bodyFrame;
      if (replaced == true) {
        bodyFrame = &body->Frame(animation, frameIndex);
      } else {
        bodyFrame = &body->Frame(this->pc->render.previousAnimation, this->pc->render.curMotion);
      }

      if (frame->anchorCount > 0 && bodyFrame->anchorCount > 0) {
        if (frame->anchors[0].attachmentId == bodyFrame->anchors[0].attachmentId) {
          offsetX = bodyFrame->anchors[0].x - frame->anchors[0].x;
          offsetY = bodyFrame->anchors[0].y - frame->anchors[0].y;
        }
      }

      offsetX += this->swingX;
      if (IsIgnoredRidingState(this->layerViews[layer], this->pc->GetEffectiveJob(-1), this->pc->render.animation, this->pc->isRiding) == true) {
        offsetY += SpriteRidingOffset(
          bodyFrame, this->pc->render.previousAnimation, this->pc->render.curMotion, this->pc->GetEffectiveJob(-1), this->pc->sex,
          this->pc->GetBodyCostume()
        );
      } else {
        offsetY += this->swingY;
      }

      const float projectionDepthNumerator = g_projectionDepthNumerator;
      if (projectionDepthNumerator != cell->scaleX) {
        offsetX = static_cast<int>(static_cast<float>(offsetX) / cell->scaleX);
      }
      if (projectionDepthNumerator != cell->scaleY) {
        offsetY = static_cast<int>(static_cast<float>(offsetY) / cell->scaleY);
      }
      break;
    }
  }

  SpriteLayerCell result;
  result.image = image;
  result.offsetY = offsetY;
  result.cell = cell;
  result.offsetX = offsetX;
  return result;
}

// OFFSET: 0x00641130
// FUNCTION: fn_04813
std::pair<int, int> SpriteLayerRenderer::GetItemOffset(int layer) {
  if (g_session.IsDoramJob(this->pc->job) == true) {
    CPc* pc = this->pc;
    int turn = 0;
    if (pc->headDir == 2) {
      turn = -1;
    } else if (pc->headDir == 1) {
      turn = 1;
    }

    int direction = pc->render.previousAnimation - pc->render.animation + turn;
    direction = direction < 0 ? 7 : direction;
    direction = direction > 7 ? 0 : direction;

    int x = 0;
    int y = 0;
    g_session.m_hotkeyScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", this->layerViews[layer], direction, pc->sex, &x, &y);
    return std::make_pair(x, y);
  }

  return std::make_pair(0, 0);
}

// OFFSET: 0x006412B0
// FUNCTION: fn_04814
const std::uint32_t* SpriteLayerRenderer::GetPalette(int layer, CSprRes* sprite, CActRes*) {
  switch (layer) {
    case SPRITE_LAYER_BODY:
      if (static_cast<int>(this->pc->render.bodyPalette) > 0) {
        return static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->pc->render.name))->palette;
      }
      break;
    case SPRITE_LAYER_HEAD:
      if (this->pc->headPalette > 0) {
        return static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->pc->headPaletteName))->palette;
      }
      break;
  }

  return sprite->palette;
}

// OFFSET: 0x00641420
// FUNCTION: fn_04815
std::pair<bool, SpriteLayerResources> SpriteLayerRenderer::FindLayer(int layer) {
  const auto found = this->layers.find(layer);
  if (found == this->layers.end()) {
    return std::make_pair(false, SpriteLayerResources{});
  }

  return std::make_pair(true, found->second);
}

// OFFSET: 0x00641620
// FUNCTION: fn_04816
std::pair<bool, std::string> SpriteLayerRenderer::GetLayerSound(int layer, CActRes* action, int sound) {
  if (layer != SPRITE_LAYER_WEAPON_EFFECT) {
    return std::make_pair(false, "");
  }

  if (sound == -1) {
    return std::make_pair(false, "");
  }

  std::string name = action->Sound(sound);
  if (name.find(".wav") != std::string::npos) {
    name = "";
  }

  return std::make_pair(true, name);
}

// OFFSET: 0x00641850
// FUNCTION: fn_04817
void SpriteLayerRenderer::DrawLayer(
  int index, int layer, const ActionFrame* frame, SpriteLayerResources resources, int animation, int frameIndex, bool replaced
) {
  int cellIndex;
  if (g_session.IsSpecialMonsterJob(this->layerViews[layer])) {
    cellIndex = GetSlotLayer(1);
  } else {
    cellIndex = GetSlotLayer(layer);
  }
  for (int i = 0; i < frame->cellCount; ++i) {
    if (layer == SPRITE_LAYER_SHIELD) {
      cellIndex = i;
    } else if (cellIndex != i) {
      continue;
    }

    const SpriteFrameCell* cell = &frame->Cell(cellIndex);
    if (cell->image == -1) {
      continue;
    }

    const SpriteRleImage* image = resources.sprite->images[cell->imageType][cell->image];
    int offsetX = 0;
    int offsetY = 0;
    const std::pair<int, int> itemOffset = this->GetItemOffset(layer);
    offsetX += itemOffset.first;
    offsetY += itemOffset.second;
    const SpriteLayerCell resolved = this->ResolveCell(layer, cell, cellIndex, resources.sprite, resources.action, frame, image, animation, frameIndex, replaced);
    offsetX += resolved.offsetX;
    offsetY += resolved.offsetY;
    auto& pcRef = this->pc;
    pcRef->ApplySwing(layer, offsetX, offsetY, this->swingX, this->swingY);

    float bottomRhw = 0.0f;
    float topRhw = 0.0f;
    SpriteQuad quad;
    tagRECT box;
    std::uint32_t quadColor;
    if (!pcRef->PrepareSprite(
          index, this->layerShift, this->distanceScale, *resolved.cell, this->x, this->y, &this->position.x, this->scale, &bottomRhw, &topRhw,
          resolved.image, &quad, &box, &quadColor, offsetX, offsetY, 0
        )) {
      continue;
    }

    if (layer == SPRITE_LAYER_BODY) {
      const float depthScale = (quad.bottomRhw - quad.topRhw) / (quad.bottom - quad.top);
      this->depthScale = depthScale;
      this->depthBias = ((quad.topRhw + quad.bottomRhw) - (quad.top + quad.bottom) * depthScale) * g_depthAverageHalf;
    }

    if (pcRef->render.translucentOther[0] == 1) {
      quad.color = static_cast<std::uint32_t>(static_cast<unsigned char>(static_cast<int>(static_cast<float>(quad.color >> 24) * (70.0f / 255.0f)))) << 24 |
                   (quad.color & 0xFFFFFF);
    }

    if (layer == SPRITE_LAYER_HEAD) {
      CPc* pc = pcRef;
      const float right = static_cast<float>(resolved.cell->x + this->swingX);
      const float bottom = static_cast<float>(resolved.cell->y + this->swingY);
      pc->extentBottom = bottom;
      pc->extentRight = right;
      if (this->hideHeadLayer == true) {
        continue;
      }
    }

    quad.image = resolved.image;
    const std::uint32_t* palette = this->GetPalette(layer, resources.sprite, resources.action);
    int effect = 0xA6;
    pcRef->SendMsg(nullptr, AM_IS_HAVE_EFFECT, reinterpret_cast<int>(&effect), 0, 0, 0, 0, 0);

    int blend = this->GetLayerBlend(layer, quad.color);

    this->IncludeLayerBounds(layer, &quad.top);

    this->alternateLayerRendering = true;
    pcRef->ApplyLayerEffects(
      &quad, resolved.cell, layer, &box, this->scale, this->rhw, bottomRhw, topRhw, this->x, this->y, palette, resolved.image, &blend, offsetX,
      offsetY
    );
    this->QueueQuad(layer, &quad, palette, blend);
    pcRef->DrawStatusOverlays(quad, palette);
    if (layer == SPRITE_LAYER_BODY) {
      pcRef->blend = blend;
      pcRef->layerCellX = resolved.cell->x;
      pcRef->layerCellY = resolved.cell->y;
      this->DrawRobe(true);
    }
  }
}

// OFFSET: 0x00641E40
// FUNCTION: fn_04818
void SpriteLayerRenderer::LoadLayers() {
  for (int layer = 0; layer < 9; ++layer) {
    CRes* sprite = this->pc->GetEquipSprite(layer);
    CRes* action = this->pc->GetEquipAction(layer);
    if (!sprite || !action) {
      continue;
    }

    if (layer == SPRITE_LAYER_HEAD) {
      const ActionFrame& frame = static_cast<CActRes*>(action)->Frame(this->pc->render.previousAnimation, this->pc->render.curMotion);
      if (frame.Cell(GetSlotLayer(1)).image == -1) {
        this->hideAccessories = true;
      }

      if (IsOrcish(this->pc->stateFlags) == true) {
        if (g_session.IsDoramJob(this->pc->job) == true) {
          sprite = ResourceMgr::Instance().Load("이팩트\\orcface_doram.spr");
          action = ResourceMgr::Instance().Load("이팩트\\orcface_doram.act");
        } else {
          sprite = ResourceMgr::Instance().Load("이팩트\\orcface.spr");
          action = ResourceMgr::Instance().Load("이팩트\\orcface.act");
        }
      }
    }

    this->layers[layer] = {static_cast<CSprRes*>(sprite), static_cast<CActRes*>(action)};
  }

  for (int layer = 2; layer <= 4; ++layer) {
    if (g_session.IsSpecialMonsterJob(this->layerViews[layer]) == true) {
      this->hideHeadLayer = true;
      return;
    }
  }
}

// OFFSET: 0x00640DB0
// FUNCTION: fn_45587
std::pair<bool, const ActionFrame*> SpriteLayerRenderer::FindSyncedFrame(int layer, CActRes* action) const {
  switch (layer) {
    case SPRITE_LAYER_HEAD_LOW:
    case SPRITE_LAYER_HEAD_TOP:
    case SPRITE_LAYER_HEAD_MID:
    case SPRITE_LAYER_ROBE: {
      const int bodyCount = static_cast<CActRes*>(this->pc->GetEquipAction(1))->FrameCount(this->pc->render.previousAnimation);
      const int layerCount = static_cast<CActRes*>(this->pc->GetEquipAction(layer))->FrameCount(this->pc->render.previousAnimation);
      if (bodyCount > 0 && layerCount > 0 && layerCount > bodyCount && layerCount % bodyCount == 0) {
        return std::make_pair(true, &action->Frame(this->pc->render.previousAnimation, this->pc->render.curMotion * (layerCount / bodyCount)));
      }
      break;
    }
  }
  return std::make_pair(false, nullptr);
}

// OFFSET: 0x00641390
// FUNCTION: fn_45588
int SpriteLayerRenderer::GetLayerBlend(int layer, std::uint32_t color) const {
  switch (layer) {
    case SPRITE_LAYER_HEAD_LOW:
    case SPRITE_LAYER_HEAD_TOP:
    case SPRITE_LAYER_ROBE:
      return 1;
    case SPRITE_LAYER_HEAD_MID:
      if (this->pc->accessory3 == ACCESSORY_BATTLEONLINE) {
        return 0x11;
      }
      return 1;
  }
  if (color >= 0xFF000000 && this->blendMode == 1) {
    switch (this->blendVariant) {
      case SPRITE_BLEND_CUSTOM_WEAPON:
        if (layer == SPRITE_LAYER_WEAPON) {
          return 5;
        }
        break;
      case SPRITE_BLEND_BODY_HEAD_HIGH_ALPHA_REF:
        if (layer == SPRITE_LAYER_BODY || layer == SPRITE_LAYER_HEAD) {
          return 0x11;
        }
        break;
    }
  }
  return 0x101;
}

// OFFSET: 0x00641490
// FUNCTION: fn_45589
void SpriteLayerRenderer::PrepareLayers() {
  this->layerShift = 0;
  this->swingX = 0;
  this->swingY = 0;
  this->hideHeadLayer = false;
  this->hideAccessories = false;
  this->alternateLayerRendering = false;

  this->layerViews[2] = this->pc->accessory;
  this->layerViews[3] = this->pc->accessory2;
  this->layerViews[4] = this->pc->accessory3;
  this->layerViews[8] = this->pc->robe;
  this->LoadLayers();
}

// OFFSET: 0x00641570
// FUNCTION: fn_45590
bool SpriteLayerRenderer::ShouldDrawLayer(int layer) const {
  if (layer == SPRITE_LAYER_WEAPON || layer == SPRITE_LAYER_WEAPON_EFFECT || layer == SPRITE_LAYER_SHIELD) {
    if (this->pc->IsCostumeOutfit() == true || !IsReadyOrAttackAction(this->pc->render.previousAnimation)) {
      return false;
    }

    if (layer == SPRITE_LAYER_SHIELD) {
      const int job = this->pc->GetEffectiveJob(-1);
      return job != JT_MADOGEAR && job != JT_MADOGEAR_H && job != JT_MADOGEAR_B;
    }
    if ((layer == SPRITE_LAYER_WEAPON || layer == SPRITE_LAYER_WEAPON_EFFECT) && this->pc->isRiding == 1) {
      return false;
    }
  }

  if ((layer == SPRITE_LAYER_HEAD_LOW || layer == SPRITE_LAYER_HEAD_TOP || layer == SPRITE_LAYER_HEAD_MID) && this->hideAccessories == true) {
    return false;
  }

  if (layer == SPRITE_LAYER_ROBE && this->pc->isHelmRobe == false) {
    return false;
  }
  return true;
}

// OFFSET: 0x00641CD0
// FUNCTION: fn_45591
void SpriteLayerRenderer::DrawQueuedLayers() {
  int depth = 0;
  for (std::map<int, SpriteLayerQuad>::iterator it = this->quads.begin(); it != this->quads.end(); ++it, ++depth) {
    SpriteLayerQuad& entry = it->second;
    entry.quad.bottomRhw = entry.quad.bottom * this->depthScale + this->depthBias;
    entry.quad.topRhw = this->depthScale * entry.quad.top + this->depthBias;
    if (g_session.IsScreenShakeActive() == true) {
      this->pc->DrawFlippedQuad(entry.quad, entry.palette, entry.blend, this->x, this->y, depth);
    } else {
      this->pc->DrawQuad(entry.quad, entry.palette, entry.blend, 0.0f, 0.0f, depth);
    }
  }
}

// OFFSET: 0x00641DE0
// FUNCTION: fn_45592
void SpriteLayerRenderer::QueueOutline(int layer, float yOffset, float xOffset, const std::uint32_t* palette, const SpriteQuad& quad, SpriteQuad& outline) {
  outline.top = yOffset + quad.top;
  outline.bottom = yOffset + quad.bottom;
  outline.left = xOffset + quad.left;
  outline.right = xOffset + quad.right;
  this->QueueQuad(layer, &outline, palette, 5);
}

// OFFSET: 0x00642130
// FUNCTION: fn_45593
void SpriteLayerRenderer::IncludeLayerBounds(int layer, const float* box) {
  if ((layer == SPRITE_LAYER_BODY || layer == SPRITE_LAYER_HEAD) && this->pc->render.trackBounds == 1) {
    this->bounds.Include(box);
  }
}
