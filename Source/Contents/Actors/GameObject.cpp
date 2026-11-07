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
#include "Contents/ChampionMonsterMgr.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/Pc.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EnergyOrbEffect.h"
#include "Contents/Effects/MonsterSkillEffects.h"
#include "Contents/Effects/MsgEffect.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMaker.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/UIBalloonOpenBtn.h"
#include "UI/Window/UIMessageBox.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <list>
#include <string>
#include <utility>
#include <windows.h>
#include <timeapi.h>

namespace {
  // VTABLE: vt_01828
  class CGameObjectIDGenerator {
  public:
    virtual ~CGameObjectIDGenerator();
  };

  // GLOBAL: gl_25444
  CGameObjectIDGenerator g_gameObjectIdGenerator;
}

// STATIC: 0x0135EF80
// GLOBAL: gl_21923
std::uint64_t CGameObject::s_nextIdentity = 0;

// GLOBAL: gl_24538
float g_cotangent = 0.16f;

// OFFSET: 0x00CA8600
// FUNCTION: fn_20989
CGameObject::CGameObject() : identity(++s_nextIdentity) {
}

// OFFSET: 0x00CA8EC0
// FUNCTION: fn_20995
CGameObject::~CGameObject() = default;

// OFFSET: 0x00CA8ED0
// FUNCTION: fn_20996
CGameObjectIDGenerator::~CGameObjectIDGenerator() {
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

  if (gameMode) {
    float relative = -gameMode->camera->camera.yaw - angle;

    while (relative < 0.0f) {
      relative += 360.0f;
    }

    while (relative >= 360.0f) {
      relative -= 360.0f;
    }

    return static_cast<int>(relative / 45.0f);
  }

  return 0;
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
// FUNCTION: fn_21103
void CRenderObject::DrawSprite(int argument, bool seasonal, float offset) {
  const float* matrix = reinterpret_cast<const float*>(argument);
  CSprRes* sprite = this->render.sprite;
  CActRes* action = this->render.action;

  if (g_session.unknown_5450 == 1 && seasonal == true) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    int job;
    if (time.wDay % 3 == 0) {
      job = 0x520;
    } else if (time.wDay % 3 == 1) {
      job = 0x56D;
    } else {
      job = 0x4F8;
    }

    char path[256];
    std::sprintf(path, "몬스터\\%s.act", g_session.GetJobName(job, -1));
    action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
    std::sprintf(path, "몬스터\\%s.spr", g_session.GetJobName(job, -1));
    sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
  }

  const ActionFrame& frame = action->Frame(this->render.previousAnimation, this->render.curMotion);

  const float* origin = &this->render.position.x;
  float position[3];
  position[0] = origin[0] * matrix[0] + origin[1] * matrix[3] + origin[2] * matrix[6] + matrix[9];
  position[1] = origin[0] * matrix[1] + origin[1] * matrix[4] + origin[2] * matrix[7] + matrix[10];
  position[2] = origin[0] * matrix[2] + origin[1] * matrix[5] + origin[2] * matrix[8] + matrix[11];

  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(origin, matrix, &x, &y, &reciprocalDepth);
  float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

  float extra = offset;
  if (CEffect* effect = g_effectMgr->Find(static_cast<int>(this->render.accountId), 2374)) {
    extra += *reinterpret_cast<const float*>(&effect->unknown_0048);
  }
  if (g_championMonsterMgr->IsChampion(static_cast<int>(this->render.unknown_010C))) {
    extra += g_championMonsterMgr->GetValue(static_cast<int>(this->render.unknown_010C));
  }
  if (extra != 0.0f) {
    distanceScale *= extra;
  }

  this->render.unknown_005C = distanceScale / g_heightScale;
  const float scale =
    g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.unknown_0078)) * this->render.scale) * this->render.unknown_005C;

  bool drawn = false;
  RenderBounds bounds;
  bounds.left = static_cast<float>(g_renderer->viewport.width);
  bounds.top = static_cast<float>(g_renderer->viewport.height);
  bounds.right = 0.0f;
  bounds.bottom = 0.0f;
  bounds.offset = reciprocalDepth;
  this->ScaleByPulse(distanceScale);

  for (int index = 0; index < frame.cellCount; ++index) {
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    float bottomRhw;
    float topRhw;
    SpriteQuad quad;
    int box[4];
    std::uint32_t color;
    if (!static_cast<CGameActor*>(this)->Slot09(
          index, index, distanceScale, cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &color, 0, 0, 0
        )) {
      continue;
    }

    drawn = true;
    quad.image = image;
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

    int blend = 257;
    this->DrawSpriteEffects(&quad, &cell, box, scale, bottomRhw, topRhw, palette, image, &blend, color);

    if (this->render.unknown_010C == 0x235) {
      quad.bottomRhw = bottomRhw - 0.0000003f;
      quad.topRhw = topRhw - 0.0000003f;
    }

    if (IsScreenShakeActive()) {
      quad.angle = 180.0f;
      this->DrawQuad(quad, palette, blend, (x - quad.right) + (x - quad.left), (y - quad.bottom) + (y - quad.top), 0);
    } else {
      this->DrawQuad(quad, palette, blend, 0.0f, 0.0f, 0);
    }

    SpriteQuad overlay = quad;
    if ((this->render.flags & 0x100) != 0) {
      const short level = this->render.unknown_00F4;
      bool draw = true;
      if (level <= 5) {
        overlay.color = (static_cast<short>(level * 50) << 24) + 0xFFFFFF;
      } else if (level <= 8) {
        overlay.color = 0xFFFFFFFF;
      } else if (level <= 11) {
        overlay.color = (static_cast<short>(895 - level * 80) << 24) + 0xFFFFFF;
      } else {
        draw = false;
      }

      if (draw) {
        this->DrawQuad(overlay, palette, 5, 0.0f, 0.0f, 0);
      }
    }

    if ((this->render.flags & 0x4000) != 0) {
      this->DrawTintedQuad(overlay, 5, 5, 255, palette);
    }

    if ((this->render.flags & 0x40000) != 0) {
      this->DrawTintedQuad(overlay, 255, 5, 5, palette);
    }
  }

  if (drawn) {
    this->SetRenderInfo(&bounds.left, x, y);
  } else {
    this->SetTlvert(x, y);
  }

  this->render.unknown_00ECBytes[0] = 1;
}

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

  for (char layer = 0; layer < 8; ++layer) {
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
      if (job >= 0x1781 && job <= 0x178A) {
        std::sprintf(path, "인간족\\머리통\\여\\%d_여.act", job % 23 + 1);
        action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
        std::sprintf(path, "인간족\\머리통\\여\\%d_여.spr", job % 23 + 1);
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      } else {
        std::sprintf(path, "인간족\\머리통\\남\\%d_남.act", job % 23 + 1);
        action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
        std::sprintf(path, "인간족\\머리통\\남\\%d_남.spr", job % 23 + 1);
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      }

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
      if (job >= 0x1781 && job <= 0x178A) {
        std::sprintf(path, "인간족\\용병\\활용병_활.act");
        action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
        std::sprintf(path, "인간족\\용병\\활용병_활.spr");
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      } else if (job >= 0x178B && job <= 0x1794) {
        std::sprintf(path, "인간족\\용병\\창용병_창.act");
        action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
        std::sprintf(path, "인간족\\용병\\창용병_창.spr");
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      } else {
        std::sprintf(path, "인간족\\용병\\검용병_검.act");
        action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
        std::sprintf(path, "인간족\\용병\\검용병_검.spr");
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      }
      frame = &action->Frame(this->render.previousAnimation, this->render.curMotion);
    } else if (layer == 3) {
      if (job >= 0x1781 && job <= 0x178A) {
        continue;
      }

      if (job >= 0x178B && job <= 0x1794) {
        std::sprintf(path, "인간족\\용병\\창용병_창_검광.act");
        action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
        std::sprintf(path, "인간족\\용병\\창용병_창_검광.spr");
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      } else {
        std::sprintf(path, "인간족\\용병\\검용병_검_검광.act");
        action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path));
        std::sprintf(path, "인간족\\용병\\검용병_검_검광.spr");
        sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path));
      }
      frame = &action->Frame(this->render.previousAnimation, this->render.curMotion);
    } else if (layer == 4) {
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
      switch (job) {
      case 0x178A:
        std::sprintf(accessory, "여\\여_꽃잎");
        break;
      case 0x1794:
        std::sprintf(accessory, "남\\남_토스트");
        break;
      }
    } else if (layer == 6) {
      switch (job) {
      case 0x178A:
        std::sprintf(accessory, "여\\여_요정의귀");
        break;
      case 0x1794:
      case 0x179E:
        std::sprintf(accessory, "남\\남_요정의귀");
        break;
      }
    } else if (layer == 7) {
      switch (job) {
      case 0x178A:
        std::sprintf(accessory, "여\\여_미스틱로즈");
        break;
      case 0x1794:
        std::sprintf(accessory, "남\\남_타키우스눈가리개");
        break;
      case 0x179E:
        std::sprintf(accessory, "남\\남_사이클롭스아이");
        break;
      }
    }

    if (layer >= 4) {
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
    float bounds[5];
    bounds[0] = static_cast<float>(g_renderer->viewport.width);
    bounds[1] = static_cast<float>(g_renderer->viewport.height);
    bounds[2] = 0.0f;
    bounds[3] = 0.0f;
    bounds[4] = reciprocalDepth;

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

      const unsigned char* argb = reinterpret_cast<const unsigned char*>(&color);
      int blend = 0x101;
      if (argb[3] < 0xFF) {
        blend = 1;
      }
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
        if (bounds[0] > minX) {
          bounds[0] = minX;
        }
        if (bounds[1] > quad.top) {
          bounds[1] = quad.top;
        }
        if (maxX > bounds[2]) {
          bounds[2] = maxX;
        }
        if (quad.bottom > bounds[3]) {
          bounds[3] = quad.bottom;
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
      this->SetRenderInfo(bounds, x, y);
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
  position[1] = mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
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
    case 24:
      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        (*it)->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
      }

      this->state.effects.clear();
      break;

    case 86:
      if (!this->state.second) {
        this->state.second = this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.second) {
          return;
        }
      }

      this->state.second->SendMsg(
        nullptr, 80, 0, static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d), 0, 0, 0, 0
      );
      break;

    case 87:
      if (this->state.second) {
        const auto end = this->state.effects.end();
        const auto it = std::find(this->state.effects.begin(), end, this->state.second);
        if (it != end) {
          (*it)->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
        }

        this->state.second = nullptr;
      }
      break;

    case 85:
      if (!this->state.first) {
        this->state.first = this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.first) {
          return;
        }
      }

      if ((static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) > 0) {
        this->SendMsg(nullptr, 109, 0, b, c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->state.first = this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.first) {
          return;
        }
      }

      this->state.first->SendMsg(nullptr, 80, 0, static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d), 0, 0, 0, 0);
      break;

    case 84:
      if (this->state.first) {
        const auto end = this->state.effects.end();
        const auto it = std::find(this->state.effects.begin(), end, this->state.first);
        if (it != end) {
          (*it)->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
        }

        this->state.first = nullptr;
      }
      break;

    case 23:
      if (sender) {
        CRagEffect* const effect = static_cast<CRagEffect*>(sender);
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

    case 109:
      if (g_effectMgr->HasAllocator(b) && sender) {
        g_effectMgr->Retire(static_cast<CRenderObject*>(sender)->render.accountId, b);
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (effect->state.type == (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b))) {
          effect->SendMsg(this, 24, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
          return;
        }
      }
      break;

    case 159:
      if (g_effectMgr->HasAllocator(d) && sender) {
        g_effectMgr->Retire(static_cast<CRenderObject*>(sender)->render.accountId, b, d);
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
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

    case 143:
      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (effect->state.type == (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b))) {
          effect->SendMsg(this, 143, 0, static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b), d, e, 0, 0);
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
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (*reinterpret_cast<int*>(b) == effect->state.type) {
          *reinterpret_cast<int*>(b) = -1;
          return;
        }
      }
      break;
  }
}

namespace {
struct PcStandEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int unknown_0008;
  /* 000C */ int cellX;
  /* 0010 */ int cellY;
  /* 0014 */ int direction;
  /* 0018 */ int speed;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ int unknown_0028;
  /* 002C */ int unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int state;
  /* 0038 */ int bodyState;
  /* 003C */ int effectState;
  /* 0040 */ std::uint32_t stateFlags;
  /* 0044 */ int unknown_0044;
  /* 0048 */ int unknown_0048;
  /* 004C */ int unknown_004C;
  /* 0050 */ int unknown_0050;
  /* 0054 */ int unknown_0054;
  /* 0058 */ int unknown_0058;
  /* 005C */ int unknown_005C;
  /* 0060 */ int guildId;
  /* 0064 */ int guildVersion;
  /* 0068 */ int unknown_0068;
  /* 006C */ int unknown_006C;
  /* 0070 */ int robe;
  /* 0074 */ int unknown_0074;
  /* 0078 */ int unknown_0078;
};

struct PcMoveEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int unknown_0008;
  /* 000C */ std::uint32_t moveStartTick;
  /* 0010 */ int startX;
  /* 0014 */ int startY;
  /* 0018 */ int startSubX;
  /* 001C */ int startSubY;
  /* 0020 */ int destX;
  /* 0024 */ int destY;
  /* 0028 */ int speed;
  /* 002C */ int unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int unknown_0038;
  /* 003C */ int bodyState;
  /* 0040 */ int effectState;
  /* 0044 */ std::uint32_t stateFlags;
  /* 0048 */ int unknown_0048;
  /* 004C */ int unknown_004C;
  /* 0050 */ int unknown_0050;
  /* 0054 */ int unknown_0054;
  /* 0058 */ int unknown_0058;
  /* 005C */ int unknown_005C;
  /* 0060 */ int unknown_0060;
  /* 0064 */ int guildId;
  /* 0068 */ int guildVersion;
  /* 006C */ int unknown_006C;
  /* 0070 */ int unknown_0070;
  /* 0074 */ int robe;
  /* 0078 */ int unknown_0078;
  /* 007C */ int unknown_007C;
};
}

// OFFSET: 0x00CB4800
// FUNCTION: fn_21112
void CGameActor::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  bool visible = true;
  if (this->render.accountId != g_session.m_aid) {
    if (StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags)) {
      visible = false;
    }
  }

  switch (static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) {
  case 21:
    break;
  case 118:
    if (this->unknown_037C[10] != 0) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->RemoveObject(reinterpret_cast<CRenderObject*>(this->unknown_037C[10]));
    }
    break;
  case 48:
    for (auto it = this->unknown_03B0.begin(); it != this->unknown_03B0.end(); ++it) {
      if (*it == sender) {
        const int effect = reinterpret_cast<int>(*it);
        if (this->unknown_037C[1] == effect) {
          this->unknown_037C[1] = 0;
        } else if (this->unknown_037C[0] == effect) {
          this->unknown_037C[0] = 0;
        } else if (this->unknown_037C[2] == effect) {
          this->unknown_037C[2] = 0;
        } else if (this->unknown_037C[3] == effect) {
          this->unknown_037C[3] = 0;
        } else if (this->unknown_037C[4] == effect) {
          this->unknown_037C[4] = 0;
        } else if (this->unknown_037C[5] == effect) {
          this->unknown_037C[5] = 0;
        } else if (this->unknown_037C[6] == effect) {
          this->unknown_037C[6] = 0;
        } else if (this->unknown_037C[8] == effect) {
          this->unknown_037C[8] = 0;
        } else if (this->unknown_037C[9] == effect) {
          this->unknown_037C[9] = 0;
        } else if (this->unknown_037C[7] == effect) {
          this->unknown_037C[7] = 0;
        } else if (this->unknown_037C[10] == effect) {
          this->unknown_037C[10] = 0;
        } else if (this->unknown_037C[11] == effect) {
          this->unknown_037C[11] = 0;
        } else if (this->unknown_037C[12] == effect) {
          this->unknown_037C[12] = 0;
        }
        this->unknown_03B0.erase(it);
        break;
      }
    }
    break;
  case 81:
    this->hitTargetAid = b;
    break;
  case 88: {
    if ((g_session.GetSceneUnknown004C() != 0 || g_session.WorldUnknown0050() != 0) && g_session.IsAdminSubMode(7) == 0) {
      break;
    }
    if ((static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) == -29999) {
      break;
    }
    int damage = b;
    if (this->render.accountId == g_session.m_aid && (g_session.HasActiveSkillEffect(0x22) != 0 || g_session.HasActiveSkillEffect(0x15B) != 0)) {
      damage = std::rand() % 22001 + 10000;
    }
    this->SpawnDamageDigits(reinterpret_cast<int>(sender), damage, d, f);
    break;
  }
  case 36: {
    CRagEffect* effect = this->LaunchEffect(0x44, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0);
    }
    break;
  }
  case 89: {
    if (!g_serviceOptions.Get(0x35)) {
      break;
    }
    CMsgEffect* effect = mode->SpawnDamageMsgEffect(&this->render.position, b, this->render.accountId);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    ++this->unknown_02F0;
    break;
  }
  case 126:
    this->LaunchEffect(0x150, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 95: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0xD, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case 160: {
    if (mode == nullptr) {
      break;
    }
    if (!g_cashEmotionMgr->IsChatProhibitEmotion(d)) {
      CMsgEffect* effect = mode->SpawnCashEmotion(reinterpret_cast<int>(&this->render.position), b, d, this->render.accountId);
      if (effect != nullptr) {
        this->unknown_03B0.push_back(effect);
        effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        break;
      }
    } else {
      if (this->unknown_037C[10] != 0) {
        break;
      }
      CGameObject** slot = reinterpret_cast<CGameObject**>(&this->unknown_037C[10]);
      *slot = mode->SpawnCashEmotion(reinterpret_cast<int>(&this->render.position), b, d, this->render.accountId);
      if (*slot != nullptr) {
        this->unknown_03B0.push_back(*slot);
        (*slot)->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        break;
      }
    }
    mode->SendMsg(0x21, 0, 0, 0, 0);
    break;
  }
  case 51: {
    if (StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags) || this->unknown_037C[1] != 0) {
      break;
    }
    int effectIds[14][2] = {
      {0x15, 0x1E}, {0x01, 0x19}, {0x10, 0x1A}, {0x11, 0x1B}, {0x12, 0x1C}, {0x13, 0x1D}, {0x2D, 0x2E},
      {0x2F, 0x30}, {0x31, 0x32}, {0x33, 0x34}, {0x36, 0x37}, {0x38, 0x39}, {0x3A, 0x3B}, {0x41, 0x42},
    };
    int row = this->unknown_02FC;
    int baby = 0;
    if (g_session.IsSuperNoviceClass(this->GetEffectiveJob(-1))) {
      row = 0;
    }
    if (g_session.IsBabyJob(this->GetEffectiveJob(-1))) {
      baby = 1;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectIds[row][baby], &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[1] = reinterpret_cast<int>(effect);
    break;
  }
  case 52:
  case 138:
    if (this->unknown_037C[1] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[1])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[1] = 0;
    }
    break;
  case 101: {
    if (this->unknown_037C[0] != 0) {
      break;
    }
    int effectId;
    switch (this->GetEffectiveJob(-1)) {
    case 0xFAC:
      effectId = 0x17;
      break;
    case 0xFC2:
      effectId = 0x18;
      break;
    case 0xFD2:
      effectId = 0x20;
      break;
    case 0xFD3:
      effectId = 0x21;
      break;
    case 0xFD4:
      effectId = 0x22;
      break;
    case 0xFD8:
    case 0xFDE:
    case 0x1002:
      effectId = 0x35;
      break;
    case 0x10A1:
    case 0x10B6:
      effectId = 0x45;
      break;
    default:
      effectId = 0xE;
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectId, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->SetAnimationSpeed(6.0f);
    this->unknown_037C[0] = reinterpret_cast<int>(effect);
    break;
  }
  case 102:
    if (this->unknown_037C[0] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[0])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      reinterpret_cast<CMsgEffect*>(this->unknown_037C[0])->m_visible = false;
      this->unknown_037C[0] = 0;
    }
    break;
  case 137: {
    if (this->unknown_037C[1] != 0) {
      break;
    }
    int effectId;
    switch (this->GetEffectiveJob(-1)) {
    case 0xFD8:
    case 0xFDE:
    case 0xFF4:
    case 0xFF5:
      effectId = 0x23;
      break;
    case 0x1002:
    case 0x100F:
      effectId = 0x2A;
      break;
    case 0x10A1:
    case 0x10B6:
      effectId = 0x46;
      break;
    default:
      return;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectId, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[1] = reinterpret_cast<int>(effect);
    break;
  }
  case 56: {
    if (this->unknown_037C[2] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(2, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[2] = reinterpret_cast<int>(effect);
    break;
  }
  case 57:
    if (this->unknown_037C[2] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[2])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[2] = 0;
    }
    break;
  case 58: {
    if (this->unknown_037C[3] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(3, &this->render.position, this->render.accountId, visible);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[3] = reinterpret_cast<int>(effect);
    break;
  }
  case 59:
  case 107:
    if (this->unknown_037C[3] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[3])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[3] = 0;
    }
    break;
  case 67: {
    if (this->unknown_037C[4] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(6, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[4] = reinterpret_cast<int>(effect);
    break;
  }
  case 68:
    if (this->unknown_037C[4] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[4])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[4] = 0;
    }
    break;
  case 69: {
    if (this->unknown_037C[5] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(7, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[5] = reinterpret_cast<int>(effect);
    this->Slot15(10, 1);
    break;
  }
  case 70:
    if (this->unknown_037C[5] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[5])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[5] = 0;
      this->Slot15(this->render.unknown_0074, 1);
    }
    break;
  case 71: {
    if (this->unknown_037C[6] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(8, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[6] = reinterpret_cast<int>(effect);
    break;
  }
  case 72:
    if (this->unknown_037C[6] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[6])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[6] = 0;
    }
    break;
  case 139:
    this->LaunchEffect(0x316, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 140:
    this->SendMsg(nullptr, 0x6D, 0, 0x316, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 73: {
    if (this->unknown_037C[7] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(9, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[7] = reinterpret_cast<int>(effect);
    break;
  }
  case 74:
    if (this->unknown_037C[7] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[7])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[7] = 0;
    }
    break;
  case 75: {
    if (this->unknown_037C[8] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0xA, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[8] = reinterpret_cast<int>(effect);
    break;
  }
  case 76:
    if (this->unknown_037C[8] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[8])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[8] = 0;
    }
    break;
  case 78: {
    if (this->unknown_037C[9] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0xB, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[9] = reinterpret_cast<int>(effect);
    break;
  }
  case 79:
    if (this->unknown_037C[9] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[9])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[9] = 0;
    }
    break;
  case 63: {
    CMsgEffect* effect = mode->SpawnMsgEffect(5, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case 62: {
    CMsgEffect* effect = mode->SpawnMsgEffect(4, &this->render.position, this->render.accountId, visible);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case 92: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0xC, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case 131: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0x16, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case 20:
    this->LaunchEffect(7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 28:
    if (g_session.IsHomunAid(this->render.accountId)) {
      if (g_windowMgr.window_03C0 != nullptr) {
        g_windowMgr.CloseWindow(0x71);
      }
      if (g_windowMgr.window_03C4 != nullptr) {
        g_windowMgr.CloseWindow(0x72);
      }
      g_session.ClearHomunStatus();
      g_session.unknown_5614 = 0;
    }
    if (g_session.IsMercenaryAid(this->render.accountId)) {
      if (g_windowMgr.window_03C8 != nullptr) {
        g_windowMgr.CloseWindow(0x7D);
      }
      if (g_windowMgr.window_03CC != nullptr) {
        g_windowMgr.CloseWindow(0x7E);
      }
      g_session.ResetMercenaryStats();
      g_session.unknown_56B0 = 0;
      break;
    }
    if (this->unknown_02A8.size() != 0 && !IsPlayerJob(this->GetEffectiveJob(-1))) {
      this->unknown_02B0 = 1;
      break;
    }
    this->Slot15(3, 1);
    break;
  case 134: {
    if (this->unknown_037C[11] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0x1F, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[11] = reinterpret_cast<int>(effect);
    break;
  }
  case 135:
    if (this->unknown_037C[11] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[11])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[11] = 0;
    }
    break;
  case 152: {
    if (this->unknown_037C[12] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0x43, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, 0x32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[12] = reinterpret_cast<int>(effect);
    break;
  }
  case 153:
    if (this->unknown_037C[12] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[12])->SendMsg(nullptr, 0x35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[12] = 0;
    }
    break;
  case 12:
    if (this->render.unknown_0070 == 3) {
      break;
    }
    this->Slot15(3, 1);
    break;
  case 96:
  case 97: {
    this->render.overrideColor = 0xFFFF0000;
    const int effectId = reinterpret_cast<const ActorDamageMessage*>(b)->effectId;
    if (effectId == 0xC6) {
      this->render.overrideColor = 0xFF042BA4;
    } else {
      this->render.overrideColor = effectId == 0xB8 ? 0xFF505050 : this->render.overrideColor;
    }
  }
  case 26:
  case 39:
  case 93:
  case 94:
  case 111:
  case 129:
  case 132:
  case 148: {
    const std::int64_t id = static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message);
    const ActorDamageMessage* damageMessage = reinterpret_cast<const ActorDamageMessage*>(b);
    const int hitEffect = damageMessage->unknown_001C;
    const int skillEffect = damageMessage->effectId;
    const int monsterValue = damageMessage->unknown_0024;
    int damage = damageMessage->damage;
    const std::uint32_t attackerAid = damageMessage->attackerAid;
    const float motionSpeed = damageMessage->motionSpeed;
    if (damageMessage->unknown_0014 != -1 && damageMessage->unknown_0018 != -1) {
      mode->CellCenter(damageMessage->unknown_0014, damageMessage->unknown_0018, this->unknown_02D0, this->unknown_02D4);
    } else {
      this->unknown_02D0 = this->render.position.x;
      this->unknown_02D4 = this->render.position.z;
    }

    int job = this->GetEffectiveJob(-1);
    std::int64_t weapon = -1;
    CGameActor* attacker;
    if (g_session.m_aid == attackerAid) {
      attacker = mode->scene->player;
    } else {
      attacker = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(attackerAid));
    }
    if (attacker != nullptr) {
      job = attacker->GetEffectiveJob(-1);
      weapon = attacker->Slot35();
    }

    if (id == 94) {
      this->SendMsg(nullptr, 95, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      bool negative = false;
      if (damage < 0 && damage != -29999) {
        damage = -damage;
        negative = true;
      }

      if (damage == 0) {
        if (attacker != nullptr) {
          if ((job >= 45 && job < 1000) || g_session.IsMonsterClass(job)) {
            if ((this->render.accountId == g_session.m_aid && g_session.HasActiveSkillEffect(19)) ||
                (this->render.accountId == g_session.m_aid && g_session.HasActiveSkillEffect(104))) {
              this->SendMsg(nullptr, 126, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            } else {
              attacker->SendMsg(nullptr, 89, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            }
          } else {
            attacker->SendMsg(nullptr, 89, 0, static_cast<int>(0xFFFF0000), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
          }
        }
      } else {
        if (g_session.m_aid == attackerAid) {
          this->unknown_02F4 = damage;
        } else {
          this->unknown_02F0 = 0;
        }

        this->render.started = timeGetTime();
        vector3d offset{0.0f, 0.0f, 0.0f};
        if (damageMessage->effectId == -2) {
          this->render.isSingleColor = false;
        } else {
          this->render.isSingleColor = true;
          const vector3d* playerPosition = &mode->scene->player->render.position;
          const char* wave;
          if (id == 96 || id == 97) {
            wave = g_session.GetWeaponHitWaveName(-1);
          } else if (IsPlayerJob(this->GetEffectiveJob(-1))) {
            wave = g_session.GetHitWaveName(this->GetEffectiveJob(-1));
          } else {
            bool genericWeapon = true;
            int view = 0;
            if (attacker != nullptr) {
              switch (attacker->GetEffectiveJob(-1)) {
              case 12:
              case 4013:
              case 4035:
              case 4059:
              case 4065:
              case 4101:
              case 4254:
              case 4260:
                view = static_cast<int>(weapon);
                if (view > 1000) {
                  view = GetWeaponViewValue(view);
                }
                genericWeapon = false;
                break;
              case 4351:
              case 4352:
              case 4353:
              case 4354:
              case 4355:
                if (attacker->unknown_0404 != 0) {
                  view = -1;
                  genericWeapon = false;
                }
                break;
              }
            }
            if (genericWeapon) {
              std::int64_t weaponView = weapon;
              if (weaponView > 1000) {
                weaponView = GetWeaponViewValue(static_cast<int>(weaponView));
              }
              view = static_cast<int>(weaponView);
            }
            wave = g_session.GetWeaponHitWaveName(view);
          }
          g_soundMgr->PlayWave(wave, this->render.position.x - playerPosition->x, 0.0f, this->render.position.z - playerPosition->z, 250, 40, 1.0f, 0);
        }

        if (hitEffect == 1567) {
          vector3d position = this->render.position;
          const int offsetX = std::max(rand() % 10, 0);
          const int offsetY = std::max(rand() % 10, 0);
          const int direction = std::max(rand() % 4, 0);
          if (direction == 0) {
            position.x += static_cast<float>(offsetX);
            position.y += static_cast<float>(offsetY);
          } else if (direction == 1) {
            position.x += static_cast<float>(offsetX);
            position.y -= static_cast<float>(offsetY);
          } else if (direction == 2) {
            position.x -= static_cast<float>(offsetX);
            position.y += static_cast<float>(offsetY);
          } else if (direction == 3) {
            position.x -= static_cast<float>(offsetX);
            position.y -= static_cast<float>(offsetY);
          }
          CEffect* effect = g_effectMgr->Spawn(0, 1567, position.x, position.y, position.z, 0, -1, 0, 0);
          effect->Slot17(static_cast<float>(std::max(rand() % 360, 0)));
        } else if (hitEffect == 2132) {
          const vector3d& playerPosition = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
          g_soundMgr->PlayWave("effect\\ABC_DEFT_STAB2.wav", playerPosition.x - this->render.position.x, 0.0f, playerPosition.z - this->render.position.z, 250, 40, 1.0f, 0);
        } else if (g_effectMgr->Spawn(this->render.accountId, hitEffect, offset.x, offset.y, offset.z, 0, -1, 0, 0) == nullptr) {
          CRagEffect* effect = this->LaunchEffect(hitEffect, offset, 0.0f);
          if (effect != nullptr) {
            effect->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
          }
        }

        if (skillEffect == 122) {
          this->render.angle += 90.0f;
          if (this->render.angle > 360.0f) {
            this->render.angle -= 360.0f;
          }
        } else if (!CSkillActionMgr::GetInstance()->ActionSlot16(damageMessage->unknown_0030, reinterpret_cast<int>(attacker), this, reinterpret_cast<CGameActor*>(damageMessage->unknown_0034))) {
          if (g_effectMgr->Spawn(this->render.accountId, skillEffect, offset.x, offset.y, offset.z, 0, -1, 0, 0) == nullptr) {
            CRagEffect* effect = this->LaunchEffect(skillEffect, offset, 0.0f);
            if (effect != nullptr) {
              effect->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
              if ((skillEffect == 216 || skillEffect == 217) && attacker != nullptr) {
                effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&attacker->render.position), 0, 0, 0, 0);
              }
            }
          }
        }

        int value = damage;
        std::uint32_t color;
        std::int64_t kind;
        std::int64_t preMessage;
        if (negative) {
          kind = 0;
          preMessage = 0;
          color = 0x72;
        } else if (id == 93) {
          preMessage = 92;
          kind = 16;
          color = 0xFFFFFF00;
        } else if ((this->GetEffectiveJob(-1) >= 45 && this->GetEffectiveJob(-1) < 1000) || g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          value = monsterValue;
          if (id == 129) {
            preMessage = 131;
            kind = 22;
            color = 0xFFFFFFFF;
          } else {
            color = 0xFFFFFFFF;
            kind = 22;
            preMessage = 0;
            if (id == 132) {
            } else if (id == 111) {
              this->SendMsg(nullptr, 88, 0, damage, damage >> 31, damageMessage->unknown_002C != 0 ? static_cast<int>(0xFF1E90FF) : -1, 0, 21, 0, 0, 0, 0, 0, 0, 0);
              kind = 22;
              color = 0xFFFFFF00;
            } else if (id == 148) {
              this->SendMsg(nullptr, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
              this->SendMsg(nullptr, 88, 0, damage, damage >> 31, static_cast<int>(0xFFFFFF00), 0, 16, 0, 0, 0, 0, 0, 0, 0);
              kind = 22;
              color = 0xFFFFFF00;
            } else {
              value = damage;
              color = 0xFFFFFFFF;
              kind = 14;
            }
          }
        } else {
          kind = 14;
          preMessage = 0;
          color = 0xFFFF0000;
        }

        if (damageMessage->unknown_002C != 0) {
          color = 0xFF1E90FF;
        }
        if (preMessage != 0) {
          this->SendMsg(nullptr, static_cast<int>(preMessage), static_cast<int>(preMessage >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        this->SendMsg(nullptr, 88, 0, value, value >> 31, static_cast<int>(color), 0, static_cast<int>(kind), static_cast<int>(kind >> 32), 0, 0, 0, 0, 0, 0);

        if (!negative) {
          const auto isPlayer = IsPlayerJob(this->GetEffectiveJob(-1));
          if (this->render.unknown_0070 != 3) {
            if (isPlayer) {
              if (this->render.unknown_0070 != 2 && id != 39 && id != 97) {
                this->Slot15(4, 1);
                if (motionSpeed > 1.0f) {
                  this->render.unknown_0060 = motionSpeed;
                } else {
                  this->SetAnimationSpeed(motionSpeed * this->render.speed);
                }
              }
            } else if (id != 39 && id != 97) {
              if (!g_session.IsMonsterActor(this) || !(motionSpeed < -0.00001f)) {
                this->Slot15(4, 1);
                if (motionSpeed > 1.0f) {
                  this->render.unknown_0060 = motionSpeed;
                } else {
                  this->SetAnimationSpeed(motionSpeed);
                }
              }
            }
          }
        }
      }
    }

    if (this->unknown_02B0 != 0 && this->unknown_02A8.size() <= 1) {
      this->SendMsg(nullptr, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  }
  case 108:
    this->Slot15(2, 1);
    this->unknown_0378 = static_cast<float>(this->Slot37());
    this->Slot16();
    this->render.unknown_008C = timeGetTime();
    this->render.curMotion = 4;
    this->SetAnimationSpeed(1.0f);
    this->render.previousAnimation += this->Get8Dir(this->render.angle);
    if (this->unknown_02B0 != 0 && this->unknown_02A8.size() <= 1) {
      this->SendMsg(nullptr, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 27: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(b);
    if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
      if (damage->action != 0x94) {
        damage->action = 0x6F;
      }
      damage->unknown_0024 = d;
    }
    this->unknown_02A8.push_back(*damage);
    break;
  }
  case 130: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(b);
    if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
      damage->unknown_0024 = d;
      damage->action = 0x81;
    }
    this->unknown_02A8.push_back(*damage);
    break;
  }
  case 133: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(b);
    if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
      damage->unknown_0024 = d;
      damage->action = 0x84;
    }
    this->unknown_02A8.push_back(*damage);
    break;
  }
  case 37:
    this->ProcessSkillMessage(sender, reinterpret_cast<const ActorSkillInfo*>(b), d, f);
    break;
  case 38:
    this->PlayGroundSkillEffect(reinterpret_cast<int>(sender), reinterpret_cast<const int*>(b), d, f);
    break;
  case 11:
    this->ProcessHitMessage(sender, reinterpret_cast<const ActorHitInfo*>(b), d, f);
    break;
  case 43: {
    const int* states = reinterpret_cast<const int*>(b);
    this->SetStatusStates(states[0], states[1], states[2], true);
    this->unknown_02C8 = states[3];
    break;
  }
  case 3: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(b);
    switch (this->unknown_0314) {
    case 8:
    case 9:
    case 10:
    case 13:
    case 14:
      this->unknown_02EC = entry->unknown_0074;
      break;
    }
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->unknown_0294 = entry->unknown_001C;
    this->unknown_0298 = entry->unknown_0020;
    this->unknown_0158[5] = entry->cellX;
    *reinterpret_cast<int*>(this->unknown_0170) = entry->cellY;
    *reinterpret_cast<int*>(this->unknown_029C) = entry->unknown_0024;
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->Slot15(0, 1);
    this->Slot49(entry->guildId, entry->guildVersion);
    break;
  }
  case 4: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(b);
    switch (this->unknown_0314) {
    case 8:
    case 9:
    case 10:
    case 13:
    case 14:
      this->unknown_02EC = entry->unknown_0074;
      break;
    }
    this->render.accountId = entry->aid;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->Slot18(entry->job, entry->unknown_0008, 0);
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->unknown_0314 == 5) {
      const int job = this->unknown_025C;
      if ((job >= 0x5248 && job <= 0x5260) || (job >= 0x5817 && job <= 0x588C)) {
        entry->direction += 4;
      }
    }
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->unknown_0294 = entry->unknown_001C;
    this->unknown_0298 = entry->unknown_0020;
    this->unknown_0158[5] = entry->cellX;
    *reinterpret_cast<int*>(this->unknown_0170) = entry->cellY;
    *reinterpret_cast<int*>(&this->unknown_0170[4]) = entry->speed;
    *reinterpret_cast<int*>(this->unknown_029C) = entry->unknown_0024;
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    if (Replay::Instance().state == 6) {
      switch (entry->state) {
      case 1:
        this->Slot15(3, 1);
        break;
      case 2:
        this->Slot15(6, 1);
        break;
      default:
        this->Slot15(0, 1);
        break;
      }
    } else {
      this->Slot15(0, 1);
    }
    this->unknown_02C8 = entry->unknown_0044;
    if (entry->job == 0x2D2 || entry->job == 0x508) {
      this->Slot49(entry->guildId, entry->guildVersion);
    }
    this->Slot41(entry->unknown_0068, entry->unknown_006C);
    if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
      this->render.color = 0x44FFFFFF;
    } else if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 2 && !StateFlag06(entry->stateFlags)) {
      this->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  }
  case 6:
  case 122: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(b);
    switch (this->unknown_0314) {
    case 8:
    case 9:
    case 10:
    case 13:
    case 14:
      this->unknown_02EC = entry->unknown_0078;
      break;
    }
    this->render.accountId = entry->aid;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    *reinterpret_cast<int*>(this->unknown_029C) = entry->unknown_002C;
    this->Slot18(entry->job, entry->unknown_0008, 0);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->unknown_02C8 = entry->unknown_0048;
    this->Slot49(entry->guildId, entry->guildVersion);
    this->Slot41(entry->unknown_006C, entry->unknown_0070);
    *reinterpret_cast<int*>(&this->unknown_0170[4]) = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    gameMode->CellCenter(entry->startX, entry->startY, this->render.position.x, this->render.position.z);
    this->render.position.x += static_cast<float>(gameMode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubX - 8);
    this->render.position.z += static_cast<float>(gameMode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubY - 8);
    this->render.position.y =
      gameMode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->unknown_0158[5] = entry->destX;
      *reinterpret_cast<int*>(this->unknown_0170) = entry->destY;
      this->Slot15(1, 1);
      this->unknown_023C = 0;
    } else {
      this->Slot15(0, 1);
    }
    break;
  }
  case 5:
  case 123: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(b);
    switch (this->unknown_0314) {
    case 8:
    case 9:
    case 10:
    case 13:
    case 14:
      this->unknown_02EC = entry->unknown_0078;
      break;
    }
    *reinterpret_cast<int*>(&this->unknown_0170[4]) = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    *reinterpret_cast<int*>(this->unknown_029C) = entry->unknown_002C;
    this->Slot18(entry->job, entry->unknown_0008, 1);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->unknown_02C8 = entry->unknown_0048;
    this->Slot49(entry->guildId, entry->guildVersion);
    this->Slot41(entry->unknown_006C, entry->unknown_0070);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->unknown_0158[5] = entry->destX;
      *reinterpret_cast<int*>(this->unknown_0170) = entry->destY;
      this->Slot15(1, 1);
      this->unknown_023C = 0;
    }
    break;
  }
  case 0: {
    const int* route = reinterpret_cast<const int*>(b);
    this->routeStartTick = route[0];
    if (this->FindRoute(route[1], route[2], route[3], route[4], route[5], route[6], *reinterpret_cast<const int*>(&this->unknown_0170[4]))) {
      this->unknown_0158[5] = route[5];
      *reinterpret_cast<int*>(this->unknown_0170) = route[6];
      this->Slot15(1, 1);
      this->unknown_023C = 0;
    } else {
      this->Slot15(0, 1);
    }
    break;
  }
  case 54:
    if (this->unknown_0264[2] != 0) {
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[2]));
      this->unknown_0264[2] = 0;
    }
    break;
  case 33:
    if (this->unknown_0264[1] != 0) {
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[1]));
      this->unknown_0264[1] = 0;
    }
    break;
  case 55:
    if (this->unknown_0264[2] == 0) {
      UIMerchantShopTitle* title = new UIMerchantShopTitle(0);
      this->unknown_0264[2] = reinterpret_cast<int>(title);
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[2]));
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[2])->SendMsg(nullptr, 0x22, b, static_cast<std::int16_t>(this->render.unknown_00FC), 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[2])->SendMsg(nullptr, 0x1C, this->render.accountId, 0, 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[2])->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    if (!g_serviceOptions.Get(0xFA)) {
      reinterpret_cast<UIWindow*>(this->unknown_0264[2])->Show(0);
    }
    break;
  case 32:
    if (this->unknown_0264[1] == 0) {
      UIChatRoomTitle* title = new UIChatRoomTitle();
      this->unknown_0264[1] = reinterpret_cast<int>(title);
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[1]));
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[1])->SendMsg(nullptr, 0x22, b, static_cast<std::int16_t>(this->render.unknown_00FC), 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[1])->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    break;
  case 82:
    if (this->unknown_0264[3] == 0) {
      // GLOBAL: gl_25466
      static float s_gaugeOffsetY = static_cast<float>(g_renderer->viewport.height * 81 / 480);
      UIRechargeGage* gauge = new UIRechargeGage();
      this->unknown_0264[3] = reinterpret_cast<int>(gauge);
      gauge->SetSize(60, 6);
      reinterpret_cast<UIWindow*>(this->unknown_0264[3])->Move(
        this->render.screenX - 30,
        static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.unknown_005C)
      );
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[3]));
    }
    this->unknown_0280 = timeGetTime() + b;
    this->unknown_0284 = timeGetTime();
    break;
  case 83:
    if (this->unknown_0264[3] != 0) {
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[3]));
      this->unknown_0264[3] = 0;
      if (this->render.accountId == g_session.m_aid) {
        if (g_session.unknown_555C[0x0A] == true) {
          std::int16_t packetType = 0x2F1;
          GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packetType), &packetType);
        }
        g_session.unknown_555C[0x0A] = false;
      }
    }
    break;
  case 7: {
    std::string text(reinterpret_cast<const char*>(b));
    if (g_windowMgr.window_01C8 != nullptr) {
      text = static_cast<UINewChatWnd*>(g_windowMgr.window_01C8)->ConvertChat(std::string(reinterpret_cast<const char*>(b)));
    }
    std::string linked;
    DisplayNavigationLink(linked, text);
    text = std::move(linked);
    const char* message = reinterpret_cast<const char*>(b);
    if (text.size() != 0) {
      message = text.c_str();
    }
    if (std::strlen(message) != 0) {
      this->unknown_0248 = timeGetTime();
      if (this->unknown_0264[0] == 0) {
        UITransBalloonText* created = new UITransBalloonText();
        this->unknown_0264[0] = reinterpret_cast<int>(created);
        created->SetSize(8, 8);
        g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[0]));
      }
      if (this->unknown_0264[0] != 0) {
        if (this->render.accountId == g_session.m_aid) {
          reinterpret_cast<UITransBalloonText*>(this->unknown_0264[0])->m_fontMode = static_cast<std::int8_t>(g_session.unknown_0F30[0]);
        } else {
          reinterpret_cast<UITransBalloonText*>(this->unknown_0264[0])->m_fontMode = static_cast<std::uint16_t>(this->render.unknown_00FC);
        }
        reinterpret_cast<UITransBalloonText*>(this->unknown_0264[0])->SetText(message, 35);
        if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
          reinterpret_cast<UITransBalloonText*>(this->unknown_0264[0])->m_textColor = d;
          reinterpret_cast<UITransBalloonText*>(this->unknown_0264[0])->m_outlineColor = 0;
        }
        UIWindow* balloon = reinterpret_cast<UIWindow*>(this->unknown_0264[0]);
        const int width = balloon->geometry.width;
        this->unknown_0288 = width;
        this->unknown_028C = balloon->geometry.height;
        balloon->Move(this->render.screenX - width / 2, this->render.screenY - ScaleByScreenHeight(110));
      }
    }
    break;
  }
  case 19: {
    this->routeStartTick = g_session.GetServerTime();
    int column;
    int row;
    int subColumn;
    int subRow;
    mode->WorldSubcell(this->render.position.x, this->render.position.z, column, row, subColumn, subRow);
    bool found;
    if (f == 0) {
      found = this->FindRoute(column, row, subColumn, subRow, b, d, this->unknown_0184 != 0 ? 55 : 2);
    } else {
      found = this->FindRoute(column, row, subColumn, subRow, b, d, f);
    }
    bool lying = false;
    if ((static_cast<std::int64_t>(i) << 32 | static_cast<std::uint32_t>(h)) == 0) {
      lying = true;
    }
    this->unknown_0158[5] = b;
    *reinterpret_cast<int*>(this->unknown_0170) = d;
    if (found) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      gameMode->CellCenter(b, d, this->render.position.x, this->render.position.z);
      this->render.position.y =
        gameMode->scene->terrain->Height(this->render.position.x, this->render.position.z);
      this->Slot17();
      if (lying == true) {
        this->unknown_02D0 = this->render.position.x;
        this->unknown_02D4 = this->render.position.z;
      }
      this->Slot15(1, lying);
      this->unknown_023C = 0;
      if (f == 0) {
        this->unknown_0238 = 1;
      } else {
        this->unknown_0238 = 0;
        this->render.isMotionFreezed = true;
      }
    } else {
      if (g_session.unknown_5608 != 0) {
        g_session.unknown_5600 = 0;
        g_session.unknown_5604 = 0;
        g_session.unknown_5608 = 0;
      }
      if (g_session.unknown_56A4 != 0) {
        g_session.unknown_569C = 0;
        g_session.unknown_56A0 = 0;
        g_session.unknown_56A4 = 0;
      }
      this->Slot15(0, lying);
    }
    break;
  }
  case 61:
    this->unknown_0158[5] = b;
    *reinterpret_cast<int*>(this->unknown_0170) = d;
    mode->CellCenter(b, d, this->render.position.x, this->render.position.z);
    break;
  case 98:
    this->render.unknown_0070 = 0;
    this->render.isLieOnGround = false;
    this->render.unknown_00A4Bytes[1] = 1;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_02CC = 0;
    this->Slot15(0, 1);
    break;
  case 99:
    this->render.accountId = 0xFFFFFFFF;
    this->unknown_02B8 = 0;
    this->render.unknown_00A4Bytes[0] = 0;
    this->unknown_02DC = static_cast<int>(timeGetTime() + 510);
    if (this->unknown_02DC == 0) {
      this->unknown_02DC = 1;
    }
    break;
  case 125: {
    this->unknown_02B4 = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (*reinterpret_cast<int*>(g_session.unknown_7EE8) == static_cast<signed char>(this->unknown_02B8)) {
      break;
    }
    if (StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags)) {
      break;
    }
    if (g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
      break;
    }
    if (this->GetEffectiveJob(-1) >= 0x52C && this->GetEffectiveJob(-1) <= 0x553) {
      break;
    }
    if (this->GetEffectiveJob(-1) >= 0x792 && this->GetEffectiveJob(-1) <= 0x7A3) {
      break;
    }
    if (this->GetEffectiveJob(-1) == 0x6C4) {
      break;
    }
    this->unknown_02B8 = static_cast<unsigned char>(*reinterpret_cast<int*>(g_session.unknown_7EE8));
    int first = 200;
    int second = 0x3D0;
    if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x30]) > 0) {
      first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x30]);
    }
    if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x34]) > 0) {
      second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x34]);
    }
    if (g_session.IsBabyJob(this->GetEffectiveJob(-1))) {
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x38]) > 0) {
        first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x38]);
      }
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x3C]) > 0) {
        second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x3C]);
      }
    } else if (this->GetEffectiveJob(-1) > 0xFA0 || g_session.IsSuperNoviceJob(this->GetEffectiveJob(-1))) {
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x40]) > 0) {
        first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x40]);
      }
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x44]) > 0) {
        second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x44]);
      }
    } else if (g_session.IsMercenaryJob(this->GetEffectiveJob(-1))) {
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x48]) > 0) {
        first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x48]);
      }
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x4C]) > 0) {
        second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x4C]);
      }
    } else if (this->GetEffectiveJob(-1) >= 0x666) {
      break;
    }
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case 146:
  case 147: {
    this->unknown_02B4 = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (*reinterpret_cast<int*>(g_session.unknown_7EE8) == static_cast<signed char>(this->unknown_02B8)) {
      break;
    }
    if (StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags)) {
      break;
    }
    this->unknown_02B8 = static_cast<unsigned char>(*reinterpret_cast<int*>(g_session.unknown_7EE8));
    int first = 0x3FE;
    int second;
    if (g_session.IsMercenaryJob(this->GetEffectiveJob(-1))) {
      first = 0x3D2;
      second = 0x3D3;
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x64]) > 0) {
        first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x64]);
      }
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x68]) > 0) {
        second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x68]);
      }
    } else if ((static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) == 0x93) {
      first = 0x3D2;
      second = 0x3D3;
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x54]) > 0) {
        first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x54]);
      }
      if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x58]) > 0) {
        second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x58]);
      }
    } else {
      second = 0x3FF;
      if ((static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) == 0x92) {
        if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x5C]) > 0) {
          first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x5C]);
        }
        if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x60]) > 0) {
          second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x60]);
        }
      }
    }
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case 150: {
    this->unknown_02B4 = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (*reinterpret_cast<int*>(g_session.unknown_7EE8) == static_cast<signed char>(this->unknown_02B8)) {
      break;
    }
    if (StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags)) {
      break;
    }
    this->unknown_02B8 = static_cast<unsigned char>(*reinterpret_cast<int*>(g_session.unknown_7EE8));
    int first = 0x4CA;
    int second = 0x4CB;
    if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x6C]) > 0) {
      first = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x6C]);
    }
    if (*reinterpret_cast<int*>(&g_session.unknown_7EF0[0x70]) > 0) {
      second = *reinterpret_cast<int*>(&g_session.unknown_7EF0[0x70]);
    }
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case 151: {
    this->unknown_02B4 = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (*reinterpret_cast<int*>(g_session.unknown_7EE8) == static_cast<signed char>(this->unknown_02B8)) {
      break;
    }
    if (StateFlag06(this->stateFlags) == true || StateFlag01(this->stateFlags) == true || StateFlag02(this->stateFlags) == true) {
      break;
    }
    this->unknown_02B8 = static_cast<unsigned char>(*reinterpret_cast<int*>(g_session.unknown_7EE8));
    const int first = g_session.m_maxLevelEffect4th;
    const int second = g_session.m_maxLevelEffect4thSub;
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case 110: {
    const bool on = (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0;
    switch (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) {
    case 0x0:
    case 0x1:
    case 0x3:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1E:
    case 0x20:
    case 0x21:
    case 0x40:
    case 0x93:
    case 0x96:
    case 0x97:
    case 0x10A:
    case 0x129:
    case 0x12B:
    case 0x13A:
    case 0x161:
    case 0x165:
    case 0x191:
    case 0x19E:
    case 0x207:
      break;
    case 0x2:
    case 0xA1: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, 109, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 0x82, 0, 0, 0);
        }
      }
      break;
    }
    case 0x44: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, 109, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 0xFA, 0, 0, 0);
        }
      }
      break;
    }
    case 0xCF:
      while (this->HasId(0x269)) {
        this->SendMsg(nullptr, 109, 0, 0x269, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x269, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x98: {
      if (this->render.accountId != g_session.m_aid) {
        break;
      }
      if (f >= 2 && f <= 11) {
        const int effectId = f + 0x1D9;
        if (!this->HasId(effectId)) {
          this->LaunchEffect(0x1E7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          g_soundMgr->PlayWave("effect\\_blind.wav", 0.0f, -100.0f, 0.0f, 250, 40, 1.0f, 0);
        }
        for (int id = 0x1DB; id < 0x1E5; ++id) {
          if (this->HasId(id) == true) {
            this->SendMsg(nullptr, 109, 0, id, id >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
          }
        }
        this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      this->SendMsg(nullptr, 109, 0, 0xA4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      if (player) {
        player->unknown_0544 = this->LaunchEffect(0xA4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x61:
      if (!on) {
        if (this->HasId(0x28F)) {
          vector3d position{0.0f, 0.0f, 0.0f};
          do {
            this->SendMsg(nullptr, 109, 0, 0x28F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            this->LaunchEffect(0x290, position, 0.0f);
          } while (this->HasId(0x28F));
        }
      } else if (!this->HasId(0x28F)) {
        this->LaunchEffect(0x28F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0xA0:
      if (this->render.accountId == g_session.m_aid) {
        if (on) {
          g_weatherEffect.StartDarken();
        } else {
          g_weatherEffect.StopDarken();
        }
      }
      break;
    case 0x9F:
      if (on) {
        this->LaunchEffect(0x222, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x56: {
      while (this->HasId(0xCB)) {
        this->SendMsg(nullptr, 109, 0, 0xCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0xCB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, 109, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFE, 0x80, 0, 0, 0);
        }
      }
      break;
    }
    case 0x57:
    case 0xEB: {
      while (this->HasId(0xFE)) {
        this->SendMsg(nullptr, 109, 0, 0xFE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, 109, 0, 0x46C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (this->render.accountId == g_session.m_gid || IsPlayerJob(this->GetEffectiveJob(-1))) {
        CPc* pc = static_cast<CPc*>(this);
        pc->unknown_0434 = on ? pc->unknown_0434 | 0x10 : pc->unknown_0434 ^ 0x10;
        if ((pc->unknown_0434 & 0x10) == 0 && (pc->unknown_0438 & 0x10) == 0) {
          break;
        }
      } else if (!on) {
        break;
      }
      this->LaunchEffect(0xFE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(0x46C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case 0xA5:
    case 0xA6:
    case 0xA7:
      while (this->HasId(0x1A2)) {
        this->SendMsg(nullptr, 109, 0, 0x1A2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      while (this->HasId(0x220)) {
        this->SendMsg(nullptr, 109, 0, 0x220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1A2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0xE8:
      while (this->HasId(0x1A2)) {
        this->SendMsg(nullptr, 109, 0, 0x1A2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      while (this->HasId(0x220)) {
        this->SendMsg(nullptr, 109, 0, 0x220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1A2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x99:
      while (this->HasId(0x1A3)) {
        this->SendMsg(nullptr, 109, 0, 0x1A3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1A3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x67:
      while (this->HasId(0x16F)) {
        this->SendMsg(nullptr, 109, 0, 0x16F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      while (this->HasId(0x1DA)) {
        this->SendMsg(nullptr, 109, 0, 0x1DA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x16F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x1DA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x5F:
      while (this->HasId(0x1C5)) {
        this->SendMsg(nullptr, 109, 0, 0x1C5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x7B: {
      while (this->HasId(0x18A)) {
        this->SendMsg(nullptr, 109, 0, 0x18A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x18A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x79: {
      this->SendMsg(nullptr, 109, 0, 0x18B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      while (this->HasId(0x18C)) {
        this->SendMsg(nullptr, 109, 0, 0x18C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x7A: {
      while (this->HasId(0x18C)) {
        this->SendMsg(nullptr, 109, 0, 0x18C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x69: {
      this->SendMsg(nullptr, 109, 0, 0x171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (on) {
        this->LaunchEffect(0x171, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x6B: {
      while (this->HasId(0x170)) {
        this->SendMsg(nullptr, 109, 0, 0x170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x170, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0xC8:
    case 0xC9: {
      while (this->HasId(0x25C)) {
        this->SendMsg(nullptr, 109, 0, 0x25C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x25C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0xC6: {
      while (this->HasId(0x259)) {
        this->SendMsg(nullptr, 109, 0, 0x259, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x259, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x62: {
      while (this->HasId(0x1C7)) {
        this->SendMsg(nullptr, 109, 0, 0x1C7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1C7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x85: {
      if (this->HasId(0x1BA)) {
        this->LaunchEffect(0x1BB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      this->SendMsg(nullptr, 109, 0, 0x1BA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0x86: {
      const vector3d& player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
      g_soundMgr->PlayWave("effect\\T_벽튕김.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0);
      this->LaunchEffect(0x1AA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (this->HasId(0x1BA)) {
        this->SendMsg(nullptr, 109, 0, 0x1BA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case 0x71: {
      while (this->HasId(0x17E)) {
        this->SendMsg(nullptr, 109, 0, 0x17E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x17E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x19:
    case 0xBC: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, 109, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 0x80, 0, 0, 0);
        }
      }
      break;
    }
    case 0x1F: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, 109, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 0xA9, 0, 0, 0);
        }
      }
      break;
    }
    case 0x1D:
      if (on) {
        this->unknown_017C = 1;
        if (this->Slot55() > 0) {
          this->SetAction(0x20, 4, 1);
        } else {
          this->SetAction(this->WeaponActionBase(3), 4, 1);
        }
        this->render.isMotionFreezed = true;
      } else {
        this->unknown_017C = 0;
        this->render.unknown_0070 = 0;
        this->render.isLieOnGround = false;
        this->render.unknown_00A4Bytes[1] = 1;
        this->Slot15(0, 1);
        this->render.isMotionFreezed = false;
      }
      break;
    case 0x22:
      if (this->render.accountId == g_session.m_aid) {
        if (on) {
          g_screenShakeDuration = 10000;
          g_screenShakeMode = 2;
          CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
          if (gameMode) {
            gameMode->camera->frame.up[0] = 0.0f;
            gameMode->camera->frame.up[1] = -1.0f;
            gameMode->camera->frame.up[2] = 0.0f;
          }
        } else {
          g_screenShakeDuration = 0;
          CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
          if (gameMode) {
            gameMode->camera->frame.up[0] = 0.0f;
            gameMode->camera->frame.up[1] = 1.0f;
            gameMode->camera->frame.up[2] = 0.0f;
          }
        }
      }
      break;
    case 0x4E: {
      if (this->HasId(0x363) == true) {
        this->SendMsg(nullptr, 109, 0, 0x363, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x363, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x46:
    case 0x4A:
    case 0x53: {
      if (this->HasId(0x361) == true) {
        this->SendMsg(nullptr, 109, 0, 0x361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x361, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x51:
    case 0x54: {
      if (this->HasId(0x36B) == true) {
        this->SendMsg(nullptr, 109, 0, 0x36B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x36B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x49: {
      if (this->HasId(0x35D) == true) {
        this->SendMsg(nullptr, 109, 0, 0x35D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x35D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x4F:
    case 0x52: {
      if (this->HasId(0x360) == true) {
        this->SendMsg(nullptr, 109, 0, 0x360, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x360, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x47:
    case 0x4D: {
      if (this->HasId(0x369) == true) {
        this->SendMsg(nullptr, 109, 0, 0x369, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x369, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x4C:
    case 0x50: {
      if (this->HasId(0x367) == true) {
        this->SendMsg(nullptr, 109, 0, 0x367, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x367, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x4B: {
      if (this->HasId(0x26) == true) {
        this->SendMsg(nullptr, 109, 0, 0x26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x26, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x48: {
      if (this->HasId(0x36D) == true) {
        this->SendMsg(nullptr, 109, 0, 0x36D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x36D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x107: {
      while (this->HasId(0x24A)) {
        this->SendMsg(nullptr, 109, 0, 0x24A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x24A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x15E: {
      while (this->HasId(0x341)) {
        this->SendMsg(nullptr, 109, 0, 0x341, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0x341, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        vector3d scale{2.0f, 0.0f, 0.0f};
        if (effect) {
          effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&scale), 0, 0, 0, 0);
        }
      }
      break;
    }
    case 0x14A:
      while (this->HasId(0x2F3)) {
        this->SendMsg(nullptr, 109, 0, 0x2F3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x2F3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1D0:
      while (this->HasId(0x170)) {
        this->SendMsg(nullptr, 109, 0, 0x170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x170, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x195:
      if (on) {
        if (!this->HasId(0x38D)) {
          this->LaunchEffect(0x38D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      }
      break;
    case 0x1A0:
    case 0x1A1: {
      unsigned char shade;
      bool frozen;
      if (on) {
        shade = 0x40;
        frozen = true;
      } else {
        shade = 0xFF;
        frozen = false;
      }
      unsigned char* const argb = reinterpret_cast<unsigned char*>(&this->render.color);
      this->render.isMotionFreezed = frozen;
      this->render.unknown_00A3 = frozen;
      argb[2] = shade;
      argb[1] = shade;
      argb[0] = shade;
      break;
    }
    case 0x1BD:
      if (IsPlayerJob(this->GetEffectiveJob(-1))) {
        while (this->HasId(0x39D)) {
          this->SendMsg(nullptr, 109, 0, 0x39D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        if (on) {
          CRagEffect* effect = this->LaunchEffect(0x39D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          if (effect) {
            effect->SendMsg(nullptr, 44, 0, 0xFFFF, 0x39D, 0, 0, 0);
          }
        }
      }
      break;
    case 0x148:
      while (this->HasId(0x314)) {
        this->SendMsg(nullptr, 109, 0, 0x314, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x314, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x15F:
      while (this->HasId(0x315)) {
        this->SendMsg(nullptr, 109, 0, 0x315, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x315, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1B5:
      while (this->HasId(0x3AC)) {
        this->SendMsg(nullptr, 109, 0, 0x3AC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x3AC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x16B:
      this->SetTetraBall(0, d, f);
      break;
    case 0x16C:
      this->SetTetraBall(1, d, f);
      break;
    case 0x16D:
      this->SetTetraBall(2, d, f);
      break;
    case 0x16E:
      this->SetTetraBall(3, d, f);
      break;
    case 0x16F:
      this->SetTetraBall(4, d, f);
      break;
    case 0x153:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        const std::int64_t level = static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f);
        if (level <= 10 && level > 0) {
          for (int id = 0x514; id < 0x51E; ++id) {
            g_effectMgr->Retire(this->render.accountId, id);
            g_effectMgr->Retire(this->render.accountId, id + 0xA7);
          }
          if (g_serviceOptions.Get(0xE5)) {
            g_effectMgr->Spawn(this->render.accountId, f + 0x5BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
          } else {
            g_effectMgr->Spawn(this->render.accountId, f + 0x513, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
          }
        }
      } else {
        for (int id = 0x514; id < 0x51E; ++id) {
          g_effectMgr->Retire(this->render.accountId, id);
          g_effectMgr->Retire(this->render.accountId, id + 0xA7);
        }
      }
      break;
    case 0x1B2:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 109, 0, 0x36F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x36F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else {
        this->SendMsg(nullptr, 109, 0, 0x36F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x1B7:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 109, 0, 0x361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x361, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1B9:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 109, 0, 0x363, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x363, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1C3:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 109, 0, 0x367, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x367, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1B3:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x1B1:
      while (this->HasId(0x1C5)) {
        this->SendMsg(nullptr, 109, 0, 0x1C5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1C5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x168:
      while (this->HasId(0x2E8)) {
        this->SendMsg(nullptr, 109, 0, 0x2E8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x2E8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      if (this->unknown_0300) {
        if (on) {
          reinterpret_cast<UIMonsterGage*>(this->unknown_0300)->SetMode(0);
        } else {
          reinterpret_cast<UIMonsterGage*>(this->unknown_0300)->SetMode(1);
        }
      }
      break;
    case 0x1A6:
      while (this->HasId(0x337)) {
        this->SendMsg(nullptr, 109, 0, 0x337, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        g_soundMgr->PlayWave("effect\\manhole.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
        if (!g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          this->LaunchEffect(0x337, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      }
      break;
    case 0x18A:
      while (this->HasId(0x329)) {
        this->SendMsg(nullptr, 109, 0, 0x329, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, 109, 0, 0x333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x329, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x17B:
      while (this->HasId(0x2E8)) {
        this->SendMsg(nullptr, 109, 0, 0x2E8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x2E8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x166:
      while (this->HasId(0x2F0)) {
        this->SendMsg(nullptr, 109, 0, 0x2F0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x2F0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x108:
      if (this->render.accountId == g_session.m_aid) {
        g_screenShakeDuration = 1;
        g_screenShakeMode = 1;
      }
      break;
    case 0x115: {
      const int text = reinterpret_cast<int>("FLEE UP!");
      this->SendMsg(nullptr, 7, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case 0x116: {
      const int text = reinterpret_cast<int>("ATK UP!");
      this->SendMsg(nullptr, 7, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case 0x119: {
      const int text = reinterpret_cast<int>("DEX UP!");
      this->SendMsg(nullptr, 7, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case 0x117: {
      const int text = reinterpret_cast<int>("HP UP!");
      this->SendMsg(nullptr, 7, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case 0x118: {
      const int text = reinterpret_cast<int>("SP UP!");
      this->SendMsg(nullptr, 7, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case 0x1C6:
      if (this->HasId(0x361) == true) {
        this->SendMsg(nullptr, 109, 0, 0x361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x361, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1C4:
      if (this->HasId(0x57) == true) {
        this->SendMsg(nullptr, 109, 0, 0x57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x57, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1C5:
      if (this->HasId(0x31F) == true) {
        this->SendMsg(nullptr, 109, 0, 0x31F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x31F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x1C0:
      if (this->HasId(0x360) == true) {
        this->SendMsg(nullptr, 109, 0, 0x360, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x360, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x241:
    case 0x244:
    case 0x245:
    case 0x248:
    case 0x25D:
    case 0x25F:
    case 0x260:
    case 0x26A:
    case 0x288:
    case 0x289:
      break;
    case 0x23B:
      g_effectMgr->SpawnLevel99(this, on, f);
      break;
    case 0x25E:
      while (this->HasId(0x3C3)) {
        this->SendMsg(nullptr, 109, 0, 0x3C3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x265: {
      if (this->render.unknown_0070 == 25) {
        this->Slot15(0, 1);
      }
      int job;
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->Slot47(1);
        job = this->GetEffectiveJob(-1);
      } else {
        this->Slot47(0);
        job = JobNames::BaseJob(this->GetEffectiveJob(-1));
      }
      this->SetActorJob(job);
      this->Slot33(this->render.unknown_0048);
      this->Slot43(0, 1);
      break;
    }
    case 0x26D:
    case 0x28B:
    case 0x299: {
      int slot;
      switch (b) {
      case 0x26D:
        slot = 0;
        break;
      case 0x28B:
        slot = 1;
        break;
      case 0x299:
        slot = 2;
        break;
      default:
        slot = 0;
        break;
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->SetUnknown03B8(f, slot);
      } else {
        this->SetUnknown03B8(-1, slot);
        if (this->render.unknown_0070 == 3) {
          this->SetAction(this->WeaponActionBase(3), 4, 1);
        }
      }
      break;
    }
    case 0x285:
      g_effectMgr->Retire(this->render.accountId, 0x3E6);
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0 && g_effectMgr->HasAllocator(0x3E6)) {
        g_effectMgr->Spawn(this->render.accountId, 0x3E6, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case 0x28F:
      while (this->HasId(0x3C3)) {
        this->SendMsg(nullptr, 109, 0, 0x3C3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x290:
      while (this->HasId(0x3EC)) {
        this->SendMsg(nullptr, 109, 0, 0x3EC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3EC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x286:
      while (this->HasId(0x18A)) {
        this->SendMsg(nullptr, 109, 0, 0x18A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        CRagEffect* effect = this->LaunchEffect(0x18A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->state.unknown_11C88[0] = 180;
          effect->state.unknown_11C88[1] = 180;
          effect->state.unknown_11C88[2] = -1;
        }
      }
      break;
    case 0x291:
      while (this->HasId(0x3EB)) {
        this->SendMsg(nullptr, 109, 0, 0x3EB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3EB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x293:
      g_effectMgr->Retire(this->render.accountId, 0x3ED);
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0 && g_effectMgr->HasAllocator(0x3ED)) {
        CEffect* orb = g_effectMgr->Spawn(this->render.accountId, 0x3ED, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (orb) {
          static_cast<CEnergyOrbEffect*>(orb)->SetCount(10);
        }
      }
      while (this->HasId(0x3C3)) {
        this->SendMsg(nullptr, 109, 0, 0x3C3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x294:
      while (this->HasId(0x220)) {
        this->SendMsg(nullptr, 109, 0, 0x220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2A0:
      this->unknown_02F8 = (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0;
      break;
    case 0x2A2:
      this->SetStackedEffect(0x3F4, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case 0x2CE:
      this->SetStackedEffect(0x40A, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case 0x30B:
      this->SetStackedEffect(0x437, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case 0x2CF:
      this->SetStackedEffect(0x40B, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case 0x2D8:
      this->SetStackedEffect(0x40C, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case 0x2A1:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        if (static_cast<std::int64_t>(this->unknown_02FC) == (static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f))) {
          break;
        }
        this->unknown_02FC = f;
        if (StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags)) {
          break;
        }
        this->SendMsg(nullptr, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->unknown_02FC = 0;
        this->SendMsg(nullptr, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x2A3:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        g_effectMgr->Spawn(this->render.accountId, 0x3F5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x3F5);
      }
      break;
    case 0x2A4:
      while (this->HasId(0x3FB)) {
        this->SendMsg(nullptr, 109, 0, 0x3FB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3FB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2B8:
      this->unknown_0305 = (static_cast<std::int64_t>(i) << 32 | static_cast<std::uint32_t>(h)) != 0;
      this->unknown_0308 = h;
      break;
    case 0x2C6:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->SendMsg(nullptr, 127, 0, 50, 0, 150, 0, 255, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x2C9:
      while (this->HasId(0x40D)) {
        this->SendMsg(nullptr, 109, 0, 0x40D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x40D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2DA:
      while (this->HasId(0x410)) {
        this->SendMsg(nullptr, 109, 0, 0x410, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x410, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x410, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x410, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2CD:
      while (this->HasId(0x418)) {
        this->SendMsg(nullptr, 109, 0, 0x418, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x418, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2DE:
      while (this->HasId(0x419)) {
        this->SendMsg(nullptr, 109, 0, 0x419, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x419, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2DF:
      while (this->HasId(0x194)) {
        this->SendMsg(nullptr, 109, 0, 0x194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x194, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2E1:
      while (this->HasId(0x41E)) {
        this->SendMsg(nullptr, 109, 0, 0x41E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x41E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2E0:
      while (this->HasId(0x41F)) {
        this->SendMsg(nullptr, 109, 0, 0x41F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x41F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2CC:
      while (this->HasId(0x421)) {
        this->SendMsg(nullptr, 109, 0, 0x421, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x421, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x2F3:
      this->SendMsg(this, 109, 0, 0x433, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (on) {
        if (g_session.unknown_5C28.find(static_cast<int>(this->render.accountId)) == g_session.unknown_5C28.end() && g_effectMgr->HasAllocator(0x425)) {
          g_effectMgr->Spawn(this->render.accountId, 0x425, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
        }
        if (g_effectMgr->HasAllocator(0x433)) {
          g_effectMgr->Spawn(this->render.accountId, 0x433, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
        }
      }
      break;
    case 0x2F8:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x470, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x470);
      }
      break;
    case 0x2F2:
      g_effectMgr->Spawn(0, 0x434, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      break;
    case 0x2F4:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x473, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x473);
      }
      break;
    case 0x2F5:
      if (on) {
        this->SendMsg(nullptr, 109, 0, 0x425, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(0, 0x432, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case 0x336:
      this->SetStackedEffect(0x43B, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case 0x14E:
    case 0x365: {
      while (this->HasId(0x318)) {
        this->SendMsg(nullptr, 109, 0, 0x318, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 0) {
        break;
      }
      CRagEffect* effect = this->LaunchEffect(0x318, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (!effect) {
        break;
      }
      effect->SendMsg(nullptr, 44, 0, 0xFFFF, 0x318, 0, 0, 0);
      break;
    }
    case 0x95:
    case 0x420:
    case 0x421:
    case 0x422:
    case 0x423:
      if (this->render.accountId == g_session.m_aid) {
        if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
          g_weatherEffect.StartDarken();
          break;
        }
        g_weatherEffect.StopDarken();
      } else if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        break;
      }
      while (this->HasId(0x1A8)) {
        this->SendMsg(nullptr, 109, 0, 0x1A8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x351:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43F, true);
      } else {
        this->SetStackedEffect(0x43F, false);
      }
      break;
    case 0x337:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43C, true);
      } else {
        this->SetStackedEffect(0x43C, false);
      }
      break;
    case 0x339:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43D, true);
      } else {
        this->SetStackedEffect(0x43D, false);
      }
      break;
    case 0x33A:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43E, true);
      } else {
        this->SetStackedEffect(0x43E, false);
      }
      break;
    case 0x359:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x441, true);
      } else {
        this->SetStackedEffect(0x441, false);
      }
      break;
    case 0x358:
      g_session.unknown_6184 = g_session.m_aid == this->render.accountId ? f : g_session.unknown_6184;
      this->unknown_03DC = f;
      this->Slot21(this->GetEffectiveJob(-1));
      this->Slot30();
      this->Slot31();
      this->Slot32();
      break;
    case 0x35B:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        g_effectMgr->Spawn(this->render.accountId, 0x442, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x442);
      }
      break;
    case 0x360:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x443, true);
      } else {
        this->SetStackedEffect(0x443, false);
      }
      break;
    case 0x35F:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x444, true);
      } else {
        this->SetStackedEffect(0x444, false);
      }
      break;
    case 0x368:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x447, true);
      } else {
        this->SetStackedEffect(0x447, false);
      }
      break;
    case 0x37F:
      this->SetStackedEffect(f + 0x9B5, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case 0x3A9:
      g_effectMgr->Retire(this->render.accountId, 0x448);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x448, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case 0x3A5:
      if (on == true) {
        this->Slot15(0x30, 1);
        this->render.isMotionFreezed = true;
        this->render.unknown_0114 = 1;
      } else {
        this->render.unknown_0070 = 0;
        this->render.isLieOnGround = false;
        this->render.unknown_00A4Bytes[1] = 1;
        this->Slot15(0, 1);
        this->render.isMotionFreezed = false;
        this->render.unknown_0114 = 0;
      }
      break;
    case 0x37D:
      if (on == true) {
        this->Slot15(0x2F, 1);
      } else if (this->render.unknown_0114 == 0) {
        this->Slot15(0, 1);
      }
      break;
    case 0x397:
      if (on == true) {
        if (!g_effectMgr->Has(this->render.accountId, 0x449)) {
          g_effectMgr->Spawn(this->render.accountId, 0x449, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x449);
      }
      break;
    case 0x380:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x456, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x456);
      }
      break;
    case 0x395:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x45F, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x45F);
      }
      break;
    case 0x3D6:
      if (g_session.IsDoramJob(this->GetEffectiveJob(-1))) {
        static_cast<CPc*>(this)->unknown_04D8[0] = on;
      }
      break;
    case 0x3F8:
    case 0x3F9:
    case 0x3FA:
    case 0x3FB:
    case 0x3FC:
    case 0x3FD:
    case 0x3FE:
    case 0x3FF:
    case 0x400:
    case 0x401: {
      const int effectId = b + 0x87;
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (g_effectMgr->Has(this->render.accountId, effectId) == true) {
        g_effectMgr->Retire(this->render.accountId, effectId);
      }
      break;
    }
    case 0x41A: {
      if (g_effectMgr->Has(this->render.accountId, 0x4B1) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4B1);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4B1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        const vector3d& player =
          reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
        g_soundMgr->PlayWave(
          "effect\\t_캐스팅.wav", this->render.position.x - player.x, 0.0f,
          this->render.position.z - player.z, 250, 40, 1.0f, 0
        );
      }
      break;
    }
    case 0x410:
      if (g_effectMgr->Has(this->render.accountId, 0x4BE) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4BE);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4BE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x40E:
      if (g_effectMgr->Has(this->render.accountId, 0x4BF) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4BF);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4BF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x413:
      if (g_effectMgr->Has(this->render.accountId, 0x4C0) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C0);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x40F:
      if (g_effectMgr->Has(this->render.accountId, 0x4C1) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C1);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x411:
      if (g_effectMgr->Has(this->render.accountId, 0x4C5) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C5);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x41F: {
      if (g_effectMgr->Has(this->render.accountId, 0x4C6) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C6);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        const vector3d& player =
          reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
        g_soundMgr->PlayWave(
          "effect\\t_바람방출.wav", this->render.position.x - player.x, 0.0f,
          this->render.position.z - player.z, 250, 40, 1.0f, 0
        );
      }
      break;
    }
    case 0x6E:
    case 0x1D9:
    case 0x461:
      while (this->HasId(0x177)) {
        this->SendMsg(nullptr, 109, 0, 0x177, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x177, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x465:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        if (g_effectMgr->Find(this->render.accountId, 0x552)) {
          break;
        }
        g_effectMgr->Spawn(this->render.accountId, 0x552, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_soundMgr->PlayWave("effect\\s포이즌버스터.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      } else {
        CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x552);
        if (effect) {
          g_effectMgr->Retire(effect, 1);
        }
      }
      break;
    case 0x466: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (!actor) {
        break;
      }
      g_effectMgr->Retire(actor->render.accountId, 0x66C);
      if (on == true) {
        g_effectMgr->Spawn(actor->render.accountId, 0x66C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    }
    case 0x473: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x576)) {
        actor->SendMsg(nullptr, 0x6D, 0, 0x576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x576, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x476: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x18C)) {
        actor->SendMsg(nullptr, 0x6D, 0, 0x18C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x475: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x58C)) {
        actor->SendMsg(nullptr, 0x6D, 0, 0x58C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x58C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x47C: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x5F5)) {
        actor->SendMsg(nullptr, 0x6D, 0, 0x5F5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x5F5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x47D: {
      CGameActor* target = this;
      if (this->render.accountId != g_session.m_aid) {
        target = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(this->render.accountId));
      }
      if (!target) {
        break;
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        if ((static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f)) == 2) {
          target->Slot57(2);
          if (this->render.accountId == g_session.m_aid) {
            g_session.unknown_8074 = 2;
          }
        } else {
          target->Slot57(1);
          if (this->render.accountId == g_session.m_aid) {
            g_session.unknown_8074 = 1;
          }
        }
        target->SendMsg(nullptr, 0x69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        target->Slot57(0);
        if (this->render.accountId == g_session.m_aid) {
          g_session.unknown_8074 = 0;
        }
        target->SendMsg(nullptr, 0x6A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case 0x48E: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (!actor) {
        break;
      }
      g_effectMgr->Retire(actor->render.accountId, 0x63F);
      if (on == true) {
        g_effectMgr->Spawn(actor->render.accountId, 0x63F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    }
    case 0x48F: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (!actor) {
        break;
      }
      if (actor->HasId(0x641)) {
        actor->SendMsg(nullptr, 0x6D, 0, 0x641, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x641, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x48D: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (!actor) {
        break;
      }
      if (actor->HasId(0x64A)) {
        actor->SendMsg(nullptr, 0x6D, 0, 0x64A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x64A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case 0x4B5:
      if (this->render.accountId != g_session.m_aid) {
        break;
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        if (!this->HasId(0x14E)) {
          this->LaunchEffect(0x14E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
        g_overrideColor = 1;
        this->SendMsg(nullptr, 0x6D, 0, 0xA4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        CPlayer* const player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
        if (player) {
          player->unknown_0544 = this->LaunchEffect(0xA4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      } else {
        if (this->HasId(0x14E) == true) {
          this->SendMsg(nullptr, 0x6D, 0, 0x14E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        g_overrideColor = 0;
      }
      this->SetInstantValue(0x10, on);
      break;
    case 0x4B6:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 0x9C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x9D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x4B9:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 0x9C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8C5);
        g_effectMgr->Spawn(this->render.accountId, 0x8C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x9D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8C5);
      }
      break;
    case 0x4B7:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, 0x98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x8C4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x4B8:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8DE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (g_effectMgr->Has(this->render.accountId, 0x8DE)) {
        g_effectMgr->Retire(this->render.accountId, 0x8DE);
        g_effectMgr->Spawn(this->render.accountId, 0x8DF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      if (this->render.accountId == g_session.m_aid) {
        if (on == true) {
          g_effectMgr->Spawn(this->render.accountId, 0x8E0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        } else {
          g_effectMgr->Retire(this->render.accountId, 0x8E0);
        }
      }
      break;
    case 0x4BA:
      g_effectMgr->Retire(this->render.accountId, 0x8DC);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8DC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x4BB:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8E1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x8E1);
      }
      break;
    case 0x4C0:
      if (on == true) {
        this->SendMsg(nullptr, 0x7F, 0, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x8C6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      this->SetInstantValue(0xC, on);
      break;
    case 0x4BC:
      if (on == true) {
        this->SendMsg(nullptr, 0x7F, 0, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8DD);
        g_effectMgr->Spawn(this->render.accountId, 0x8DD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8DD);
      }
      this->SetInstantValue(0xD, on);
      break;
    case 0x4BD:
      if (on == true) {
        this->SendMsg(nullptr, 0x9A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x9B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x4BE:
      if (on == true) {
        this->SendMsg(nullptr, 0x7F, 0, 0xAA, 0, 0, 0, 0xAA, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x8E2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8E2);
      }
      this->SetInstantValue(0xE, on);
      break;
    case 0x4BF:
      if (on == true) {
        this->SendMsg(nullptr, 0x7F, 0, 0, 0, 0, 0, 0xFF, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      this->SetInstantValue(0xF, on);
      break;
    case 0x498: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (!actor) {
        break;
      }
      g_effectMgr->Retire(actor->render.accountId, 0x6BE);
      g_effectMgr->Retire(actor->render.accountId, 0x6BF);
      if (on == true) {
        g_effectMgr->Spawn(actor->render.accountId, 0x6BE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 0x6BF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    }
    case 0x4ce: {
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x806, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const second = g_effectMgr->Spawn(this->render.accountId, 0x803, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const third = g_effectMgr->Spawn(this->render.accountId, 0x804, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (second) {
          second->Slot16(1000);
        }
        if (third) {
          third->Slot16(1000);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x803);
        g_effectMgr->Retire(this->render.accountId, 0x804);
      }
      break;
    }
    case 0x4cf: {
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x821, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const second = g_effectMgr->Spawn(this->render.accountId, 0x81e, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const third = g_effectMgr->Spawn(this->render.accountId, 0x81f, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (second) {
          second->Slot16(1000);
        }
        if (third) {
          third->Slot16(1000);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x821);
        g_effectMgr->Retire(this->render.accountId, 0x81e);
        g_effectMgr->Retire(this->render.accountId, 0x81f);
      }
      break;
    }
    case 0x4a6:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7c7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7c7);
      }
      break;
    case 0x4ad:
      if (on == true) {
        if (!g_serviceOptions.Get(0xe5) && !g_effectMgr->Find(this->render.accountId, 0x712)) {
          g_effectMgr->Spawn(this->render.accountId, 0x712, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x712);
      }
      break;
    case 0x47e:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x6cd, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x6cd)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case 0x480:
      g_effectMgr->Retire(this->render.accountId, 0x708);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x708, this->render.position.x, this->render.position.y, this->render.position.z, 0, f, 0, 0);
      }
      break;
    case 0x4d2:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7b7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7b7);
      }
      break;
    case 0x4d0:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7ba, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x7bd, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7ba);
        g_effectMgr->Retire(this->render.accountId, 0x7bd);
      }
      break;
    case 0x4d1:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7be, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x7c1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7be);
        g_effectMgr->Retire(this->render.accountId, 0x7c1);
      }
      break;
    case 0x4dd:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x84e, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x84e);
      }
      break;
    case 0x4d5:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x719, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        this->LaunchEffect(0x8f8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x719)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case 0x4d4:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x72f, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x72f);
      }
      break;
    case 0x4b1:
      if (on == true) {
        if (!g_effectMgr->Find(this->render.accountId, 0x73d)) {
          g_effectMgr->Spawn(this->render.accountId, 0x73d, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        }
        if (!g_serviceOptions.Get(0xe5) && !g_effectMgr->Find(this->render.accountId, 0x73c)) {
          g_effectMgr->Spawn(this->render.accountId, 0x73c, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        }
        this->LaunchEffect(0x8fc, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else {
        if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x73d)) {
          g_effectMgr->Retire(effect, 1);
        }
        if (!g_serviceOptions.Get(0xe5)) {
          if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x73c)) {
            g_effectMgr->Retire(effect, 1);
          }
        }
      }
      break;
    case 0x4a0:
      if (on == true) {
        if (!g_effectMgr->Find(this->render.accountId, 0x743)) {
          g_effectMgr->Spawn(this->render.accountId, 0x743, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x743);
      }
      break;
    case 0x4ea:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x80c, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x80c)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case 0x4e2:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x76c, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x76c);
      }
      break;
    case 0x4ac:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x779, this->render.position.x, this->render.position.y, this->render.position.z, 0, f, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x779);
      }
      break;
    case 0x515:
      if (on == true) {
        this->LaunchEffect(0x2e8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x2e8) == true) {
        this->SendMsg(nullptr, 109, 0, 0x2e8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x4e1:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8be, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x526:
      if (on == true) {
        this->LaunchEffect(0x8e5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x8e5) == true) {
        this->SendMsg(nullptr, 109, 0, 0x8e5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x527:
      if (on == true) {
        this->LaunchEffect(0x8e6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x8e6) == true) {
        this->SendMsg(nullptr, 109, 0, 0x8e6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x48a:
    case 0x4ed:
      if (on == true) {
        this->LaunchEffect(0x8f8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x488:
      if (on == true) {
        this->LaunchEffect(0x8f6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x4f7:
      if (on == true) {
        this->LaunchEffect(0x8fa, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x4b0:
      if (on == true) {
        this->LaunchEffect(0x8f0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case 0x53b:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8fe, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x8fe);
        g_effectMgr->Spawn(this->render.accountId, 0x8ff, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case 0x539:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x901, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x901)) {
        static_cast<CCurseOfCubeEffect*>(effect)->Restart();
      }
      break;
    case 0x53a:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x902, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x902)) {
        static_cast<CCurseOfCubeEffect*>(effect)->Restart();
      }
      break;
    case 0x698:
      if (on == true) {
        g_windowMgr.EnsureWindow(0x2730);
      } else {
        g_windowMgr.CloseWindow(0x2730);
      }
      break;
    default:
      if (on == true) {
        CSkillActionMgr::GetInstance()->EfstSlot00(b, f, h, l, this, (static_cast<std::int64_t>(k) << 32 | static_cast<std::uint32_t>(j)) != 0);
      } else {
        CSkillActionMgr::GetInstance()->EfstSlot01(b, f, h, l, this);
      }
      break;
    }
    break;
  }
  case 127: {
    this->render.unknown_00A3 = 1;
    unsigned char* const saved = reinterpret_cast<unsigned char*>(&this->render.unknown_0090);
    if (saved[0] == 0 && saved[1] == 0 && saved[2] == 0 && saved[3] == 0) {
      const unsigned char* const argb = reinterpret_cast<const unsigned char*>(&this->render.color);
      saved[0] = argb[3];
      saved[1] = argb[2];
      saved[2] = argb[1];
      saved[3] = argb[0];
    }
    unsigned char* const target = reinterpret_cast<unsigned char*>(&this->render.unknown_0094);
    target[0] = 0xFF;
    target[1] = static_cast<unsigned char>(b);
    target[2] = static_cast<unsigned char>(d);
    target[3] = static_cast<unsigned char>(f);
    const std::int64_t red = static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b);
    const std::int64_t green = static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d);
    this->render.color = static_cast<std::uint32_t>(red << 16 | green << 8 | f | 0xFF000000);
    break;
  }
  case 128:
    this->render.unknown_0090 = 0;
    this->render.unknown_00A3 = 0;
    break;
  case 142:
    if (this->unknown_0264[4] == 0) {
      break;
    }
    g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[4]));
    this->unknown_0264[4] = 0;
    break;
  case 141: {
    if (this->unknown_0264[4] == 0) {
      UIMerchantShopTitle* const title = new UIMerchantShopTitle(1);
      this->unknown_0264[4] = reinterpret_cast<int>(title);
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[4]));
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->SendMsg(nullptr, 34, b, static_cast<std::int16_t>(this->render.unknown_00FC), 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->SendMsg(nullptr, 28, this->render.accountId, 0, 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    if (g_serviceOptions.Get(0xFA)) {
      break;
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->Show(0);
    break;
  }
  case 144:
    if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1))) {
      if (CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 44, 0, 0xFFFF, 0xF3, 0, 100, 0);
      }
      if (CEffect* fallenAngel = g_effectMgr->Find(this->render.accountId, 0x474)) {
        vector3d position{0.0f, 0.0f, 0.0f};
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(b, d, position.x, position.z);
        position.y = this->render.position.y;
        static_cast<CFallenAngelEffect*>(fallenAngel)->SetPosition(position);
      }
    }
    this->SendMsg(nullptr, 19, 0, b, b >> 31, d, d >> 31, 15, 0, 0, 0, 0, 0, 0, 0);
    this->SetAction(this->render.animation, 6, 1);
    this->render.curMotion = 7;
    this->render.isMotionFreezed = true;
    break;
  case 145:
    if ((static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) == -1 ||
        (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == -1) {
      if (this->unknown_0300 == 0) {
        break;
      }
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0300));
      this->unknown_0300 = 0;
      break;
    }
    if ((static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f)) == 1 && this->unknown_0300 != 0) {
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0300));
      this->unknown_0300 = 0;
    }
    if (this->unknown_0300 == 0) {
      UIMonsterGage* const gage = new UIMonsterGage();
      this->unknown_0300 = reinterpret_cast<int>(gage);
      gage->SetSize(60, 5);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0300));
    }
    reinterpret_cast<UIPcGage*>(this->unknown_0300)->SetValues(b, d);
    reinterpret_cast<UIPcGage*>(this->unknown_0300)->Move(
      this->render.screenX - reinterpret_cast<UIPcGage*>(this->unknown_0300)->geometry.width / 2,
      static_cast<int>(static_cast<float>(ScaleByScreenHeight(12)) * this->render.unknown_005C + static_cast<float>(this->render.screenY))
    );
    break;
  case 154:
    if (this->unknown_03D4 == 0) {
      CGameObject* effect = mode->SpawnMsgEffect(68, &this->render.position, this->render.accountId, true);
      this->unknown_03B0.push_back(effect);
      effect->SendMsg(this, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D4 = reinterpret_cast<int>(effect);
    }
    break;
  case 155:
    if (this->unknown_03D4 != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_03D4)->SendMsg(nullptr, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D4 = 0;
    }
    break;
  case 156:
    if (this->unknown_03D8 == 0) {
      CGameObject* effect = mode->SpawnMsgEffect(9, &this->render.position, this->render.accountId, true);
      this->unknown_03B0.push_back(effect);
      effect->SendMsg(this, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D8 = reinterpret_cast<int>(effect);
    }
    break;
  case 157:
    if (this->unknown_03D8 != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_03D8)->SendMsg(nullptr, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D8 = 0;
    }
    break;
  default:
    this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    break;
  }
}

