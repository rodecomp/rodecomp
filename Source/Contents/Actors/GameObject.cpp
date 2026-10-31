#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Camera/Camera.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/World/World.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Render/SpriteQuad.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "Render/Textures/SpritePixels.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <algorithm>
#include <cstdio>
#include <list>

namespace {
  // STATIC: 0x0135EF80
  // GLOBAL: gl_21923
  std::uint64_t nextIdentity = 0;
}

// GLOBAL: gl_24538
float g_cotangent = 0.16f;

// OFFSET: 0x00CA8600
// FUNCTION: fn_20989
CGameObject::CGameObject() : identity(++nextIdentity) {
}

// OFFSET: 0x00CA8EC0
// OFFSET: 0x00CA9230
// FUNCTION: fn_20995
CGameObject::~CGameObject() = default;

// OFFSET: 0x00CA8ED0
// FUNCTION: fn_20996
void __fastcall sub_CA8ED0(void* self) {
  *reinterpret_cast<std::uintptr_t*>(self) = 0x0119FE74;
}

// OFFSET: 0x00CAFD20
// FUNCTION: fn_21087
bool CGameObject::OnProcess() {
  return true;
}

// OFFSET: 0x00CBF850
// FUNCTION: fn_21113
void CGameObject::SendMsg(CGameObject*, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x00CB20A0
// FUNCTION: fn_21101
void CGameObject::Render(int) {
}

// OFFSET: 0x00CABF10
// FUNCTION: fn_21041
int CGameObject::GetDir(float rot) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int longitude = 0;

  if (gameMode) {
    longitude = static_cast<int>(-gameMode->camera->camera.yaw - rot);

    while (longitude < 0) {
      longitude += 360;
    }

    while (longitude >= 360) {
      longitude -= 360;
    }
  }

  return longitude;
}

// OFFSET: 0x00CAB740
// FUNCTION: fn_21029
int CGameObject::Get8Dir2(float rot, int flag) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (!gameMode) {
    return 0;
  }

  float relative = -gameMode->camera->camera.yaw - rot;

  while (relative < 0.0f) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  if (flag != 0) {
    if (relative < 22.5f || relative > 337.5f) {
      return 0;
    }

    return static_cast<int>((static_cast<double>(relative) + 22.5) / 45.0);
  }

  return static_cast<int>(relative / 45.0f);
}

// OFFSET: 0x00CAB7F0
// FUNCTION: fn_21030
int CGameActor::Get8Dir(float angle) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (!gameMode) {
    return 0;
  }

  float relative = -gameMode->camera->camera.yaw - angle;

  while (relative < 0.0f) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  if ((IsPlayerJob(this->GetEffectiveJob(-1)) && (this->render.animation == 0 || this->render.animation == 8 || this->render.animation == 16)) ||
      (this->GetEffectiveJob(-1) > 45 && this->GetEffectiveJob(-1) < 1000 && this->render.animation == 0) ||
      (g_session.IsSkillIdInRange6018(this->GetEffectiveJob(-1)) &&
       (this->render.animation == 0 || this->render.animation == 8 || this->render.animation == 16))) {
    if (relative < 22.5f || relative > 337.5f) {
      return 0;
    }

    return static_cast<int>((static_cast<double>(relative) + 22.5) / 45.0);
  }

  return static_cast<int>(relative / 45.0f);
}

// OFFSET: 0x00CAB920
// FUNCTION: fn_21031
int CGameObject::Get8Dir(float angle) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (!gameMode) {
    return 0;
  }

  float relative = -gameMode->camera->camera.yaw - angle;

  while (relative < 0.0f) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  return static_cast<int>(relative / 45.0f);
}

// OFFSET: 0x00CB20B0
// FUNCTION: fn_21102
void CRenderObject::Render(int argument) {
  if (this->render.visible != 0) {
    this->DrawShadow(argument, this->render.scale);
    this->DrawSprite(argument, false, 0.0f);
  }
}

// OFFSET: 0x00CB20F0

// OFFSET: 0x00CB2830
// FUNCTION: fn_21104
void CGameActor::RenderMercenary(const float* matrix, int job) {
  const float* origin = &this->render.position.x;
  float position[3];
  position[0] = origin[0] * matrix[0] + origin[1] * matrix[3] + origin[2] * matrix[6] + matrix[9];
  position[1] = origin[0] * matrix[1] + origin[1] * matrix[4] + origin[2] * matrix[7] + matrix[10];
  position[2] = origin[0] * matrix[2] + origin[1] * matrix[5] + origin[2] * matrix[8] + matrix[11];

  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(origin, matrix, &x, &y, &reciprocalDepth);
  const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);
  this->render.unknown_005C = distanceScale / g_heightScale;
  const float scale =
    g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.unknown_0078)) * this->render.scale) * this->render.unknown_005C;

  char path[256];
  char accessory[256];
  const ActionFrame* bodyFrame;
  const ActionFrame* headFrame;
  int headOffsetX;
  int headOffsetY;
  int accessoryOffsetX;
  int accessoryOffsetY;

  for (unsigned char layer = 0; layer < 8; ++layer) {
    int offsetX = 0;
    int offsetY = 0;

    if (layer >= 5 && job != 0x178A && job != 0x1794 && job != 0x179E) {
      continue;
    }

    CSprRes* sprite;
    CActRes* action;
    const ActionFrame* frame;

    if (layer == 0) {
      sprite = this->render.sprite;
      action = this->render.action;
      frame = &action->Frame(this->render.previousAnimation, this->render.curMotion);
      if (this->render.previousAnimation < 8) {
        frame = &action->Frame(this->render.previousAnimation, 0);
      }
      bodyFrame = frame;
    } else if (layer == 1) {
      const int head = job % 23 + 1;
      const char* actionName;
      const char* spriteName;
      if (job >= 0x1781 && job <= 0x178A) {
        spriteName = "인간족\\머리통\\여\\%d_여.spr";
        actionName = "인간족\\머리통\\여\\%d_여.act";
      } else {
        spriteName = "인간족\\머리통\\남\\%d_남.spr";
        actionName = "인간족\\머리통\\남\\%d_남.act";
      }

      std::sprintf(path, actionName, head);
      action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
      std::sprintf(path, spriteName, head);
      sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));

      frame = &action->Frame(this->render.previousAnimation, this->render.curMotion);
      if (this->render.previousAnimation < 8) {
        frame = &action->Frame(this->render.previousAnimation, 0);
      }
      headFrame = frame;

      if (headFrame->anchorCount != 0 && bodyFrame->anchorCount != 0 && headFrame->anchors[0].unknown_0008 == bodyFrame->anchors[0].unknown_0008) {
        headOffsetX = bodyFrame->anchors[0].x - headFrame->anchors[0].x;
        headOffsetY = bodyFrame->anchors[0].y - headFrame->anchors[0].y;
      }
      offsetX = headOffsetX;
      offsetY = headOffsetY;
    } else if (layer == 2) {
      const char* actionName;
      const char* spriteName;
      if (job >= 0x1781 && job <= 0x178A) {
        spriteName = "인간족\\용병\\활용병_활.spr";
        actionName = "인간족\\용병\\활용병_활.act";
      } else if (job >= 0x178B && job <= 0x1794) {
        spriteName = "인간족\\용병\\창용병_창.spr";
        actionName = "인간족\\용병\\창용병_창.act";
      } else {
        spriteName = "인간족\\용병\\검용병_검.spr";
        actionName = "인간족\\용병\\검용병_검.act";
      }

      std::sprintf(path, actionName);
      action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
      std::sprintf(path, spriteName);
      sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      frame = &action->Frame(this->render.previousAnimation, this->render.curMotion);
    } else if (layer == 3) {
      if (job >= 0x1781 && job <= 0x178A) {
        continue;
      }

      const char* actionName;
      const char* spriteName;
      if (job >= 0x178B && job <= 0x1794) {
        spriteName = "인간족\\용병\\창용병_창_검광.spr";
        actionName = "인간족\\용병\\창용병_창_검광.act";
      } else {
        spriteName = "인간족\\용병\\검용병_검_검광.spr";
        actionName = "인간족\\용병\\검용병_검_검광.act";
      }

      std::sprintf(path, actionName);
      action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
      std::sprintf(path, spriteName);
      sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      frame = &action->Frame(this->render.previousAnimation, this->render.curMotion);
    } else {
      if (layer == 4) {
        switch (job) {
        case 0x1781:
          std::sprintf(accessory, "여\\여_털모자");
          break;
        case 0x1782:
          std::sprintf(accessory, "여\\여_골든헤드기어");
          break;
        case 0x1783:
          std::sprintf(accessory, "여\\여_대형금방울");
          break;
        case 0x1784:
          std::sprintf(accessory, "여\\여_엘리스모자");
          break;
        case 0x1785:
          std::sprintf(accessory, "여\\여_가부키가면");
          break;
        case 0x1786:
          std::sprintf(accessory, "여\\여_여왕의투구");
          break;
        case 0x1787:
          std::sprintf(accessory, "여\\여_지르타스가면");
          break;
        case 0x1788:
          std::sprintf(accessory, "여\\여_실버티아라");
          break;
        case 0x1789:
          std::sprintf(accessory, "여\\여_고양이모자");
          break;
        case 0x178A:
          std::sprintf(accessory, "여\\여_예쁜리본");
          break;
        case 0x178B:
          std::sprintf(accessory, "남\\남_간호모");
          break;
        case 0x178C:
          std::sprintf(accessory, "남\\남_검은눈가리개");
          break;
        case 0x178D:
          std::sprintf(accessory, "남\\남_고블린가면4호");
          break;
        case 0x178E:
          std::sprintf(accessory, "남\\남_양모자");
          break;
        case 0x178F:
          std::sprintf(accessory, "남\\남_귀찮은너구리");
          break;
        case 0x1790:
          std::sprintf(accessory, "남\\남_돈잃은자의마음");
          break;
        case 0x1791:
          std::sprintf(accessory, "남\\남_마리오넷모자");
          break;
        case 0x1792:
          std::sprintf(accessory, "남\\남_라이온마스크");
          break;
        case 0x1793:
          std::sprintf(accessory, "남\\남_클로스헬멧");
          break;
        case 0x1794:
          std::sprintf(accessory, "남\\남_로드카호의뿔");
          break;
        case 0x1795:
          std::sprintf(accessory, "남\\남_반창고");
          break;
        case 0x1796:
          std::sprintf(accessory, "남\\남_분노입");
          break;
        case 0x1797:
          std::sprintf(accessory, "남\\남_코끼리모자");
          break;
        case 0x1798:
          std::sprintf(accessory, "남\\남_야구모자");
          break;
        case 0x1799:
          std::sprintf(accessory, "남\\남_장례건");
          break;
        case 0x179A:
          std::sprintf(accessory, "남\\남_부용");
          break;
        case 0x179B:
          std::sprintf(accessory, "남\\남_타이거마스크");
          break;
        case 0x179C:
          std::sprintf(accessory, "남\\남_초승달투구");
          break;
        case 0x179D:
          std::sprintf(accessory, "남\\남_엔젤링모자");
          break;
        case 0x179E:
          std::sprintf(accessory, "남\\남_태양신의모자");
          break;
        }
      } else if (layer == 5) {
        if (job == 0x178A) {
          std::sprintf(accessory, "여\\여_꽃잎");
        } else if (job == 0x1794) {
          std::sprintf(accessory, "남\\남_토스트");
        }
      } else if (layer == 6) {
        if (job == 0x178A) {
          std::sprintf(accessory, "여\\여_요정의귀");
        } else if (job == 0x1794 || job == 0x179E) {
          std::sprintf(accessory, "남\\남_요정의귀");
        }
      } else if (layer == 7) {
        if (job == 0x178A) {
          std::sprintf(accessory, "여\\여_미스틱로즈");
        } else if (job == 0x1794) {
          std::sprintf(accessory, "남\\남_타키우스눈가리개");
        } else if (job == 0x179E) {
          std::sprintf(accessory, "남\\남_사이클롭스아이");
        }
      }

      std::sprintf(path, "악세사리\\%s.act", accessory);
      action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
      std::sprintf(path, "악세사리\\%s.spr", accessory);
      sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));

      frame = &action->Frame(this->render.previousAnimation, this->render.curMotion);
      if (this->render.previousAnimation < 8) {
        frame = &action->Frame(this->render.previousAnimation, 0);
      }

      if (frame->anchorCount != 0 && headFrame->anchorCount != 0 && frame->anchors[0].unknown_0008 == headFrame->anchors[0].unknown_0008) {
        accessoryOffsetX = headFrame->anchors[0].x - frame->anchors[0].x;
        accessoryOffsetY = headFrame->anchors[0].y - frame->anchors[0].y;
      }
      offsetX = headOffsetX + accessoryOffsetX;
      offsetY = headOffsetY + accessoryOffsetY;
    }

    bool drawn = false;
    RenderBounds bounds;
    bounds.left = static_cast<float>(g_renderer->viewport.width);
    bounds.top = static_cast<float>(g_renderer->viewport.height);
    bounds.right = 0.0f;
    bounds.bottom = 0.0f;
    bounds.offset = reciprocalDepth;

    for (int index = 0; index < frame->cellCount; ++index) {
      const SpriteFrameCell& cell = frame->Cell(index);
      if (cell.image == -1) {
        continue;
      }

      const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
      float bottomRhw;
      float topRhw;
      SpriteQuad quad;
      int box[4];
      std::uint32_t color;
      if (!this->Slot09(index, index, distanceScale, cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &color, offsetX, offsetY, 0)) {
        continue;
      }

      int blend = (color >> 24) < 0xFF ? 1 : 257;
      drawn = true;
      quad.image = image;

      if (layer >= 4) {
        blend = 1;
        const float bias = static_cast<float>(layer - 4) * 0.00004f + 0.0001f;
        quad.bottomRhw += bias;
        quad.topRhw += bias;
      }

      if (this->render.unknown_00A4Bytes[1] != 0) {
        const float minX = quad.left < quad.right ? quad.left : quad.right;
        const float maxX = quad.left > quad.right ? quad.left : quad.right;
        if (bounds.left > minX) {
          bounds.left = minX;
        }
        if (bounds.top > quad.top) {
          bounds.top = quad.top;
        }
        if (maxX > bounds.right) {
          bounds.right = maxX;
        }
        if (quad.bottom > bounds.bottom) {
          bounds.bottom = quad.bottom;
        }
      }

      const std::uint32_t* palette = sprite->palette;
      if (this->render.unknown_0048 != 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->render.name.c_str()))->palette;
      }

      if (layer <= 1) {
        this->DrawPulseOutline(cell, box, scale, bottomRhw, topRhw, palette, image, color);
      }

      if (IsScreenShakeActive()) {
        this->DrawFlippedQuad(quad, palette, blend, x, y, 0);
      } else {
        this->DrawQuad(quad, palette, blend, 0.0f, 0.0f, 0);
      }
    }

    if (drawn) {
      this->SetRenderInfo(&bounds.left, x, y);
    } else {
      this->SetTlvert(x, y);
    }
  }

  this->render.unknown_00ECBytes[0] = 1;
}

// OFFSET: 0x00CB35F0
// FUNCTION: fn_21105
void CRenderObject::DrawShadow(int argument, float scale) {
  if (this->render.unknown_010C == 565 || this->render.unknown_00A4Bytes[0] == 0 || this->render.unknown_0054 == 0.0f || scale == 0.0f) {
    return;
  }

  const float* matrix = reinterpret_cast<const float*>(argument);
  const float z = this->render.position.z;
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  float position[3];
  position[0] = this->render.position.x;
  position[1] = mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
  position[2] = z;
  const float depth = position[1] * matrix[5] + position[0] * matrix[2] + z * matrix[8] + matrix[11];

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(position, matrix, &screenX, &screenY, &reciprocalDepth);
  const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("shadow.spr"));
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("shadow.act"));
  const SpriteFrameCell& cell = action->Frame(0, 0).Cell(0);
  if (cell.image == -1) {
    return;
  }

  float pulse = 1.0f;
  const SpriteRleImage* image = sprite->images[cell.imageType][0];
  this->ScaleByPulse(pulse);
  const double size = pulse * scale * this->render.unknown_0054;
  const double factor = distanceScale;

  SpriteQuad quad;
  quad.top = static_cast<float>(cell.y * factor * cell.scaleY * size + screenY);
  quad.bottom = static_cast<float>(((image->unknown_0006 + 1) * image->height + cell.y) * factor * cell.scaleY * size + screenY + 1.0);
  float left = static_cast<float>(cell.x * factor * cell.scaleX * size + screenX);
  quad.right = static_cast<float>(((image->unknown_0004 + 1) * image->width + cell.x) * factor * cell.scaleX * size + screenX + 1.0);
  if (quad.top >= static_cast<float>(g_renderer->viewport.height) || quad.bottom <= 0.0f || left >= static_cast<float>(g_renderer->viewport.width) ||
      quad.right <= 0.0f) {
    return;
  }

  quad.left = static_cast<float>(static_cast<int>(this->render.unknown_0078)) + left;
  quad.right = static_cast<float>(static_cast<int>(this->render.unknown_0078)) + quad.right;
  quad.color = static_cast<std::uint32_t>(((static_cast<std::uint32_t>(cell.color >> 24) << 8 | (cell.color & 0xFF)) << 8 | (cell.color >> 8 & 0xFF)) << 8) |
               (cell.color >> 16 & 0xFF);
  quad.bottomRhw = 1.0f / (depth - (quad.bottom - screenY) * g_cotangent) + 0.00001f;
  quad.topRhw = 1.0f / (depth - (quad.top - screenY) * g_cotangent) + 0.00001f;
  quad.angle = static_cast<float>(cell.unknown_001C);
  quad.image = image;

  if (IsScreenShakeActive()) {
    this->DrawFlippedQuad(quad, sprite->palette, 257, screenX, screenY, 0);
    return;
  }

  this->DrawQuad(quad, sprite->palette, 257, 0.0f, 0.0f, 0);
}

// OFFSET: 0x00CB39A0
// FUNCTION: fn_21106
void CRenderObject::DrawQuad(const SpriteQuad& quad, const std::uint32_t* palette, int blend, float offsetX, float offsetY, int depthOffset) {
  SpriteAtlasRecord record{};
  const SpriteRleImage* image = quad.image;
  CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
  if (!texture) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    record.u0 = 0.0f;
    record.v0 = 0.0f;
    record.u1 = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
    record.v1 = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());
  }

  const double depthBias = depthOffset * 0.000001;
  const double rhwBias = depthOffset * 0.0002;
  const float bottomRhw = static_cast<float>(rhwBias + quad.bottomRhw);
  const float topRhw = static_cast<float>(quad.topRhw + rhwBias);
  const float bottomZ = RhwToDepth(static_cast<float>(depthBias + quad.bottomRhw));
  const float topZ = RhwToDepth(static_cast<float>(quad.topRhw + depthBias));

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  primitive->texture = texture;

  if (quad.angle != 0.0f) {
    const float cosine = ClientCosine(static_cast<int>(quad.angle));
    const float sine = ClientSine(static_cast<int>(quad.angle));
    const float halfWidth = (quad.right - quad.left) * 0.5f;
    const float halfHeight = (quad.bottom - quad.top) * 0.5f;
    const float centerX = quad.left + halfWidth + offsetX;
    const float centerY = quad.top + halfHeight + offsetY;
    const float minX = -halfWidth;
    const float minY = -halfHeight;

    vertices[0].x = minX * cosine - minY * sine + centerX + 0.5f;
    vertices[0].y = minY * cosine + minX * sine + centerY + 0.5f;
    vertices[1].x = halfWidth * cosine - minY * sine + centerX + 0.5f;
    vertices[1].y = minY * cosine + halfWidth * sine + centerY + 0.5f;
    vertices[2].x = minX * cosine - halfHeight * sine + centerX + 0.5f;
    vertices[2].y = halfHeight * cosine + minX * sine + centerY + 0.5f;
    vertices[3].x = halfWidth * cosine - halfHeight * sine + centerX + 0.5f;
    vertices[3].y = halfHeight * cosine + halfWidth * sine + centerY + 0.5f;
  } else {
    vertices[0].x = quad.left + 0.5f;
    vertices[0].y = quad.top + 0.5f;
    vertices[1].x = quad.right + 0.5f;
    vertices[1].y = quad.top + 0.5f;
    vertices[2].x = quad.left + 0.5f;
    vertices[2].y = quad.bottom + 0.5f;
    vertices[3].x = quad.right + 0.5f;
    vertices[3].y = quad.bottom + 0.5f;
  }

  if (*palette == 0xD77B) {
    record.u1 -= 0.002f;
  }

  vertices[0].rhw = topRhw;
  vertices[0].z = topZ;
  vertices[0].u = record.u0;
  vertices[0].v = record.v0;
  vertices[0].diffuse = quad.color;
  vertices[1].rhw = topRhw;
  vertices[1].z = topZ;
  vertices[1].u = record.u1;
  vertices[1].v = record.v0;
  vertices[1].diffuse = quad.color;
  vertices[2].rhw = bottomRhw;
  vertices[2].z = bottomZ;
  vertices[2].u = record.u0;
  vertices[2].v = record.v1;
  vertices[2].diffuse = quad.color;
  vertices[3].rhw = bottomRhw;
  vertices[3].z = bottomZ;
  vertices[3].u = record.u1;
  vertices[3].v = record.v1;
  vertices[3].diffuse = quad.color;

  primitive->unknown_0018 = 5;
  primitive->unknown_001C = blend != 5 ? 6 : 2;
  primitive->unknown_0014 = g_renderer->twoPass != 1;
  g_renderer->queues.Submit(primitive, static_cast<std::uint16_t>(blend | this->render.unknown_0124), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}

// OFFSET: 0x00CB3F40
// FUNCTION: fn_21107
void CRenderObject::DrawFlippedQuad(SpriteQuad& quad, const std::uint32_t* palette, int blend, float x, float y, int depthOffset) {
  quad.angle = 180.0f;
  this->DrawQuad(quad, palette, blend, (x - quad.right) + (x - quad.left), (y - quad.bottom) + (y - quad.top), depthOffset);
}


// OFFSET: 0x00CB3FA0
// FUNCTION: fn_21108
void CRenderObject::SetActionSpeed(float speed) {
  speed *= this->render.action->Speed(this->render.animation);

  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }

  if (speed > 0.0f) {
    this->render.speed = speed;
  }
}

// OFFSET: 0x00CB3FF0
// FUNCTION: fn_21109
void CRenderObject::SendValueMessage(int value, CGameObject* target) {
  if (target) {
    const std::int64_t wide = value;
    target->SendMsg(
      nullptr, 29, 0, static_cast<int>(wide), static_cast<int>(wide >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    );
  }
}

// OFFSET: 0x00CB4070
// FUNCTION: fn_21110
void CGameActor::ApplyPendingUnknown0408() {
  const int pending = this->unknown_0408;
  this->unknown_0404 = pending;
  this->unknown_0408 = 0;
  this->render.isLieOnGround = false;
  this->render.unknown_00A4Bytes[1] = 1;
  this->render.isMotionFreezed = false;
  this->render.unknown_0114 = 0;

  int value;
  if (pending == 1) {
    value = 0x1104;
  } else if (pending == 2) {
    value = 0x1105;
  } else {
    value = this->unknown_025C;
  }
  this->Slot21(value);
  this->Slot32();

  if (this->render.unknown_0070 != 3) {
    this->render.unknown_0070 = 0;
    this->Slot15(0, 1);
  }
}

// OFFSET: 0x00CB4100
// FUNCTION: fn_21111
void EffectActor::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int, int, int, int, int, int, int, int) {
  switch (static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) {
    case 23:
      if (sender) {
        RagEffectState* const effect = reinterpret_cast<RagEffectState*>(sender);
        for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
          if (*it == effect) {
            this->state.effects.erase(it);

            if (this->state.first == effect) {
              this->state.first = nullptr;
            } else if (this->state.second == effect) {
              this->state.second = nullptr;
            }

            return;
          }
        }
      }
      break;

    case 24:
      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        reinterpret_cast<CRagEffect*>(*it)->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
      }

      this->state.effects.clear();
      break;

    case 84:
      if (this->state.first) {
        const auto end = this->state.effects.end();
        const auto it = std::find(this->state.effects.begin(), end, this->state.first);
        if (it != end) {
          reinterpret_cast<CRagEffect*>(*it)->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
        }

        this->state.first = nullptr;
      }
      break;

    case 85:
      if (!this->state.first) {
        this->state.first = reinterpret_cast<RagEffectState*>(this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f));
        if (!this->state.first) {
          return;
        }
      }

      if ((static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) > 0) {
        this->SendMsg(nullptr, 109, 0, b, c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->state.first = reinterpret_cast<RagEffectState*>(this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f));
        if (!this->state.first) {
          return;
        }
      }

      reinterpret_cast<CRagEffect*>(this->state.first)->SendMsg(nullptr, 80, 0, static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d), 0, 0, 0, 0);
      break;

    case 86:
      if (!this->state.second) {
        this->state.second = reinterpret_cast<RagEffectState*>(this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f));
        if (!this->state.second) {
          return;
        }
      }

      reinterpret_cast<CRagEffect*>(this->state.second)->SendMsg(
        nullptr, 80, 0, static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d), 0, 0, 0, 0
      );
      break;

    case 87:
      if (this->state.second) {
        const auto end = this->state.effects.end();
        const auto it = std::find(this->state.effects.begin(), end, this->state.second);
        if (it != end) {
          reinterpret_cast<CRagEffect*>(*it)->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
        }

        this->state.second = nullptr;
      }
      break;

    case 109:
      if (g_effectMgr->HasAllocator(b) && sender) {
        g_effectMgr->Retire(static_cast<CRenderObject*>(sender)->render.accountId, b);
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = reinterpret_cast<CRagEffect*>(*it);
        if (!effect) {
          this->state.effects.erase(it);
        } else if (effect->state.type == (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b))) {
          effect->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
          return;
        }
      }
      break;

    case 120:
      if (g_effectMgr->HasAllocator(b) == true && sender) {
        if (g_effectMgr->Has(static_cast<CRenderObject*>(sender)->render.accountId, b) == true) {
          *reinterpret_cast<int*>(b) = -1;
        }
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = reinterpret_cast<CRagEffect*>(*it);
        if (!effect) {
          this->state.effects.erase(it);
        } else if (*reinterpret_cast<int*>(b) == effect->state.type) {
          *reinterpret_cast<int*>(b) = -1;
          return;
        }
      }
      break;

    case 143:
      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = reinterpret_cast<CRagEffect*>(*it);
        if (!effect) {
          this->state.effects.erase(it);
        } else if (effect->state.type == (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b))) {
          effect->SendMsg(this, 143, 0, static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b), d, e, 0, 0);
        }
      }
      break;

    case 159:
      if (g_effectMgr->HasAllocator(d) && sender) {
        g_effectMgr->Retire(static_cast<CRenderObject*>(sender)->render.accountId, b, d);
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = reinterpret_cast<CRagEffect*>(*it);
        if (!effect) {
          this->state.effects.erase(it);
        } else if (static_cast<int>(effect->state.tag) == (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) &&
                   effect->state.type == (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d))) {
          effect->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
          return;
        }
      }
      break;
  }
}

// OFFSET: 0x00CB4800
