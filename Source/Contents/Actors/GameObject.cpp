#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/ActorEnums.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Effects/EffectTypes.h"
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
    if (!static_cast<CGameActor*>(this)->PrepareSprite(
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

    if (layer >= 5 && job != JT_MER_ARCHER10 && job != JT_MER_LANCER10 && job != JT_MER_SWORDMAN10) {
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
      case JT_MER_ARCHER01:
        std::sprintf(accessory, "여\\여_털모자");
        break;
      case JT_MER_ARCHER02:
        std::sprintf(accessory, "여\\여_골든헤드기어");
        break;
      case JT_MER_ARCHER03:
        std::sprintf(accessory, "여\\여_대형금방울");
        break;
      case JT_MER_ARCHER04:
        std::sprintf(accessory, "여\\여_엘리스모자");
        break;
      case JT_MER_ARCHER05:
        std::sprintf(accessory, "여\\여_가부키가면");
        break;
      case JT_MER_ARCHER06:
        std::sprintf(accessory, "여\\여_여왕의투구");
        break;
      case JT_MER_ARCHER07:
        std::sprintf(accessory, "여\\여_지르타스가면");
        break;
      case JT_MER_ARCHER08:
        std::sprintf(accessory, "여\\여_실버티아라");
        break;
      case JT_MER_ARCHER09:
        std::sprintf(accessory, "여\\여_고양이모자");
        break;
      case JT_MER_ARCHER10:
        std::sprintf(accessory, "여\\여_예쁜리본");
        break;
      case JT_MER_LANCER01:
        std::sprintf(accessory, "남\\남_간호모");
        break;
      case JT_MER_LANCER02:
        std::sprintf(accessory, "남\\남_검은눈가리개");
        break;
      case JT_MER_LANCER03:
        std::sprintf(accessory, "남\\남_고블린가면4호");
        break;
      case JT_MER_LANCER04:
        std::sprintf(accessory, "남\\남_양모자");
        break;
      case JT_MER_LANCER05:
        std::sprintf(accessory, "남\\남_귀찮은너구리");
        break;
      case JT_MER_LANCER06:
        std::sprintf(accessory, "남\\남_돈잃은자의마음");
        break;
      case JT_MER_LANCER07:
        std::sprintf(accessory, "남\\남_마리오넷모자");
        break;
      case JT_MER_LANCER08:
        std::sprintf(accessory, "남\\남_라이온마스크");
        break;
      case JT_MER_LANCER09:
        std::sprintf(accessory, "남\\남_클로스헬멧");
        break;
      case JT_MER_LANCER10:
        std::sprintf(accessory, "남\\남_로드카호의뿔");
        break;
      case JT_MER_SWORDMAN01:
        std::sprintf(accessory, "남\\남_반창고");
        break;
      case JT_MER_SWORDMAN02:
        std::sprintf(accessory, "남\\남_분노입");
        break;
      case JT_MER_SWORDMAN03:
        std::sprintf(accessory, "남\\남_코끼리모자");
        break;
      case JT_MER_SWORDMAN04:
        std::sprintf(accessory, "남\\남_야구모자");
        break;
      case JT_MER_SWORDMAN05:
        std::sprintf(accessory, "남\\남_장례건");
        break;
      case JT_MER_SWORDMAN06:
        std::sprintf(accessory, "남\\남_부용");
        break;
      case JT_MER_SWORDMAN07:
        std::sprintf(accessory, "남\\남_타이거마스크");
        break;
      case JT_MER_SWORDMAN08:
        std::sprintf(accessory, "남\\남_초승달투구");
        break;
      case JT_MER_SWORDMAN09:
        std::sprintf(accessory, "남\\남_엔젤링모자");
        break;
      case JT_MER_SWORDMAN10:
        std::sprintf(accessory, "남\\남_태양신의모자");
        break;
      }
    } else if (layer == 5) {
      switch (job) {
      case JT_MER_ARCHER10:
        std::sprintf(accessory, "여\\여_꽃잎");
        break;
      case JT_MER_LANCER10:
        std::sprintf(accessory, "남\\남_토스트");
        break;
      }
    } else if (layer == 6) {
      switch (job) {
      case JT_MER_ARCHER10:
        std::sprintf(accessory, "여\\여_요정의귀");
        break;
      case JT_MER_LANCER10:
      case JT_MER_SWORDMAN10:
        std::sprintf(accessory, "남\\남_요정의귀");
        break;
      }
    } else if (layer == 7) {
      switch (job) {
      case JT_MER_ARCHER10:
        std::sprintf(accessory, "여\\여_미스틱로즈");
        break;
      case JT_MER_LANCER10:
        std::sprintf(accessory, "남\\남_타키우스눈가리개");
        break;
      case JT_MER_SWORDMAN10:
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
      if (!this->PrepareSprite(index, index, distanceScale, cell, x, y, position, scale, &bottomRhw, &topRhw, image, &quad, box, &color, offsetX, offsetY, 0)) {
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
      nullptr, AM_REQ_ATTACK, 0, static_cast<int>(wide), static_cast<int>(wide >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
  this->SetSprJob(value);
  this->SetBodyPaletteName();

  if (this->render.unknown_0070 != 3) {
    this->render.unknown_0070 = 0;
    this->SetState(0, 1);
  }
}

// OFFSET: 0x00CB4100
// FUNCTION: fn_21111
void EffectActor::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int, int, int, int, int, int, int, int) {
  switch (static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) {
    case AM_MASTER_DEAD:
      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        (*it)->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
      }

      this->state.effects.clear();
      break;

    case AM_MAKE_MAGIC_TARGET:
      if (!this->state.second) {
        this->state.second = this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.second) {
          return;
        }
      }

      this->state.second->SendMsg(
        nullptr, AM_SET_DURATION, 0, static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d), 0, 0, 0, 0
      );
      break;

    case AM_DELETE_MAGIC_TARGET:
      if (this->state.second) {
        const auto end = this->state.effects.end();
        const auto it = std::find(this->state.effects.begin(), end, this->state.second);
        if (it != end) {
          (*it)->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
        }

        this->state.second = nullptr;
      }
      break;

    case AM_MAKE_BEGIN_SPELL:
      if (!this->state.first) {
        this->state.first = this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.first) {
          return;
        }
      }

      if ((static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) > 0) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, b, c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->state.first = this->LaunchEffect(b, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.first) {
          return;
        }
      }

      this->state.first->SendMsg(nullptr, AM_SET_DURATION, 0, static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d), 0, 0, 0, 0);
      break;

    case AM_DELETE_BEGIN_SPELL:
      if (this->state.first) {
        const auto end = this->state.effects.end();
        const auto it = std::find(this->state.effects.begin(), end, this->state.first);
        if (it != end) {
          (*it)->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
        }

        this->state.first = nullptr;
      }
      break;

    case AM_EFFECT_DEAD:
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

    case AM_DELETE_EFFECT:
      if (g_effectMgr->HasAllocator(b) && sender) {
        g_effectMgr->Retire(static_cast<CRenderObject*>(sender)->render.accountId, b);
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (effect->state.type == (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b))) {
          effect->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
          return;
        }
      }
      break;

    case AM_DELETE_TAGGED_EFFECT:
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
          effect->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
          return;
        }
      }
      break;

    case AM_EFFECT_LOOP_COUNT:
      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (effect->state.type == (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b))) {
          effect->SendMsg(this, AM_EFFECT_LOOP_COUNT, 0, static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b), d, e, 0, 0);
        }
      }
      break;

    case AM_IS_HAVE_EFFECT:
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
  case AM_DELETE_PROHIBITEFFECT:
    if (this->unknown_037C[10] != 0) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->RemoveObject(reinterpret_cast<CRenderObject*>(this->unknown_037C[10]));
    }
    break;
  case AM_MSGEFFECT_DEAD:
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
  case AM_SET_TARGET_AID:
    this->hitTargetAid = b;
    break;
  case AM_MAKE_NUMBER: {
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
  case AM_MVP: {
    CRagEffect* effect = this->LaunchEffect(0x44, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    }
    break;
  }
  case AM_MAKE_MISS: {
    if (!g_serviceOptions.Get(0x35)) {
      break;
    }
    CMsgEffect* effect = mode->SpawnDamageMsgEffect(&this->render.position, b, this->render.accountId);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    ++this->unknown_02F0;
    break;
  }
  case AM_MAKE_GUARD:
    this->LaunchEffect(0x150, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case AM_MAKE_LUCKY: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0xD, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_CASH_EMOTION: {
    if (mode == nullptr) {
      break;
    }
    if (!g_cashEmotionMgr->IsChatProhibitEmotion(d)) {
      CMsgEffect* effect = mode->SpawnCashEmotion(reinterpret_cast<int>(&this->render.position), b, d, this->render.accountId);
      if (effect != nullptr) {
        this->unknown_03B0.push_back(effect);
        effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
        (*slot)->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        break;
      }
    }
    mode->SendMsg(MM_PLAYER_EMOTION_END, 0, 0, 0, 0);
    break;
  }
  case AM_MAKE_CART: {
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
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[1] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_CART:
  case AM_DELETE_WOLF:
    if (this->unknown_037C[1] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[1])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[1] = 0;
    }
    break;
  case AM_MAKE_BIRD: {
    if (this->unknown_037C[0] != 0) {
      break;
    }
    int effectId;
    switch (this->GetEffectiveJob(-1)) {
    case JT_HUNTER_H:
      effectId = 0x17;
      break;
    case JT_HUNTER_B:
      effectId = 0x18;
      break;
    case JT_GANGSI:
      effectId = 0x20;
      break;
    case JT_DEATHKNIGHT:
      effectId = 0x21;
      break;
    case JT_COLLECTOR:
      effectId = 0x22;
      break;
    case JT_RANGER:
    case JT_RANGER_H:
    case JT_RANGER_B:
      effectId = 0x35;
      break;
    case JT_WINDHAWK:
    case JT_WOLF_WINDHAWK:
      effectId = 0x45;
      break;
    default:
      effectId = 0xE;
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectId, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    effect->SetAnimationSpeed(6.0f);
    this->unknown_037C[0] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_BIRD:
    if (this->unknown_037C[0] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[0])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      reinterpret_cast<CMsgEffect*>(this->unknown_037C[0])->m_visible = false;
      this->unknown_037C[0] = 0;
    }
    break;
  case AM_MAKE_WOLF: {
    if (this->unknown_037C[1] != 0) {
      break;
    }
    int effectId;
    switch (this->GetEffectiveJob(-1)) {
    case JT_RANGER:
    case JT_RANGER_H:
    case JT_WOLF_RANGER:
    case JT_WOLF_RANGER_H:
      effectId = 0x23;
      break;
    case JT_RANGER_B:
    case JT_WOLF_RANGER_B:
      effectId = 0x2A;
      break;
    case JT_WINDHAWK:
    case JT_WOLF_WINDHAWK:
      effectId = 0x46;
      break;
    default:
      return;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectId, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[1] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_MAKE_PKEFFECT: {
    if (this->unknown_037C[2] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(2, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[2] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_PKEFFECT:
    if (this->unknown_037C[2] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[2])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[2] = 0;
    }
    break;
  case AM_MAKE_ICEEFFECT: {
    if (this->unknown_037C[3] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(3, &this->render.position, this->render.accountId, visible);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[3] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_ICEEFFECT:
  case 107:
    if (this->unknown_037C[3] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[3])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[3] = 0;
    }
    break;
  case AM_MAKE_CURSE_EFFECT: {
    if (this->unknown_037C[4] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(6, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[4] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_CURSE_EFFECT:
    if (this->unknown_037C[4] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[4])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[4] = 0;
    }
    break;
  case AM_MAKE_SLEEP_EFFECT: {
    if (this->unknown_037C[5] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(7, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[5] = reinterpret_cast<int>(effect);
    this->SetState(10, 1);
    break;
  }
  case AM_DELETE_SLEEP_EFFECT:
    if (this->unknown_037C[5] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[5])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[5] = 0;
      this->SetState(this->render.unknown_0074, 1);
    }
    break;
  case AM_MAKE_STUN_EFFECT: {
    if (this->unknown_037C[6] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(8, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[6] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_STUN_EFFECT:
    if (this->unknown_037C[6] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[6])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[6] = 0;
    }
    break;
  case AM_MAKE_BURNING_EFFECT:
    this->LaunchEffect(0x316, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case AM_DELETE_BURNING_EFFECT:
    this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x316, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  case AM_MAKE_SILENCE_EFFECT: {
    if (this->unknown_037C[7] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(9, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[7] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_SILENCE_EFFECT:
    if (this->unknown_037C[7] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[7])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[7] = 0;
    }
    break;
  case AM_MAKE_ANGELUS_EFFECT: {
    if (this->unknown_037C[8] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0xA, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[8] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_ANGELUS_EFFECT:
    if (this->unknown_037C[8] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[8])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[8] = 0;
    }
    break;
  case AM_MAKE_CRUCIS_EFFECT: {
    if (this->unknown_037C[9] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0xB, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[9] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_CRUCIS_EFFECT:
    if (this->unknown_037C[9] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[9])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[9] = 0;
    }
    break;
  case AM_MAKE_SMOKE_FFECT: {
    CMsgEffect* effect = mode->SpawnMsgEffect(5, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_MAKE_ICEBREAKE_FFECT: {
    CMsgEffect* effect = mode->SpawnMsgEffect(4, &this->render.position, this->render.accountId, visible);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_MAKE_CRITICAL_EFFECT: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0xC, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case 131: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0x16, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_USEITEM_EFFECT:
    this->LaunchEffect(7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case AM_WILL_BE_DEAD:
    if (g_session.IsHomunAid(this->render.accountId)) {
      if (g_windowMgr.window_03C0 != nullptr) {
        g_windowMgr.CloseWindow(WID_HOMUNINFOWND);
      }
      if (g_windowMgr.window_03C4 != nullptr) {
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_0);
      }
      g_session.ClearHomunStatus();
      g_session.unknown_5614 = 0;
    }
    if (g_session.IsMercenaryAid(this->render.accountId)) {
      if (g_windowMgr.window_03C8 != nullptr) {
        g_windowMgr.CloseWindow(WID_MERINFOWND);
      }
      if (g_windowMgr.window_03CC != nullptr) {
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_1);
      }
      g_session.ResetMercenaryStats();
      g_session.unknown_56B0 = 0;
      break;
    }
    if (this->unknown_02A8.size() != 0 && !IsPlayerJob(this->GetEffectiveJob(-1))) {
      this->unknown_02B0 = 1;
      break;
    }
    this->SetState(3, 1);
    break;
  case 134: {
    if (this->unknown_037C[11] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0x1F, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[11] = reinterpret_cast<int>(effect);
    break;
  }
  case 135:
    if (this->unknown_037C[11] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[11])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[11] = 0;
    }
    break;
  case AM_MAKE_LASSITUDE_EFFECT: {
    if (this->unknown_037C[12] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0x43, &this->render.position, this->render.accountId, true);
    this->unknown_03B0.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_037C[12] = reinterpret_cast<int>(effect);
    break;
  }
  case AM_DELETE_LASSITUDE_EFFECT:
    if (this->unknown_037C[12] != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_037C[12])->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_037C[12] = 0;
    }
    break;
  case AM_DEAD:
    if (this->render.unknown_0070 == 3) {
      break;
    }
    this->SetState(3, 1);
    break;
  case AM_ATTACKED_SKILL:
  case AM_ATTACKEDNOMOTION_SKILL: {
    this->render.overrideColor = 0xFFFF0000;
    const int effectId = reinterpret_cast<const ActorDamageMessage*>(b)->effectId;
    if (effectId == 0xC6) {
      this->render.overrideColor = 0xFF042BA4;
    } else {
      this->render.overrideColor = effectId == 0xB8 ? 0xFF505050 : this->render.overrideColor;
    }
  }
  case AM_ATTACKED:
  case AM_ATTACKEDNOMOTION:
  case AM_ATTACKED_CRITICAL:
  case AM_ATTACKED_LUCKY:
  case AM_ATTACKED_MANYTIMES:
  case AM_ATTACKED_MANYTIMES2:
  case AM_ATTACKED_MANYTIMES2DASH:
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
      weapon = attacker->GetWeapon();
    }

    if (id == 94) {
      this->SendMsg(nullptr, AM_MAKE_LUCKY, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
              this->SendMsg(nullptr, AM_MAKE_GUARD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            } else {
              attacker->SendMsg(nullptr, AM_MAKE_MISS, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            }
          } else {
            attacker->SendMsg(nullptr, AM_MAKE_MISS, 0, static_cast<int>(0xFFFF0000), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
              case JT_ASSASSIN:
              case JT_ASSASSIN_H:
              case JT_ASSASSIN_B:
              case JT_GUILLOTINE_CROSS:
              case JT_GUILLOTINE_CROSS_H:
              case JT_GUILLOTINE_CROSS_B:
              case JT_SHADOW_CROSS:
              case JT_ABYSS_CHASER:
                view = static_cast<int>(weapon);
                if (view > 1000) {
                  view = GetWeaponViewValue(view);
                }
                genericWeapon = false;
                break;
              case JT_DRUID:
              case JT_DRUID_B:
              case JT_KARNOS:
              case JT_KARNOS_B:
              case JT_ALITEA:
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
          effect->SetAngle(static_cast<float>(std::max(rand() % 360, 0)));
        } else if (hitEffect == 2132) {
          const vector3d& playerPosition = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
          g_soundMgr->PlayWave("effect\\ABC_DEFT_STAB2.wav", playerPosition.x - this->render.position.x, 0.0f, playerPosition.z - this->render.position.z, 250, 40, 1.0f, 0);
        } else if (g_effectMgr->Spawn(this->render.accountId, hitEffect, offset.x, offset.y, offset.z, 0, -1, 0, 0) == nullptr) {
          CRagEffect* effect = this->LaunchEffect(hitEffect, offset, 0.0f);
          if (effect != nullptr) {
            effect->SendMsg(nullptr, AM_SETCOUNT, 0, 1, 0, 0, 0, 0);
          }
        }

        if (skillEffect == 122) {
          this->render.angle += 90.0f;
          if (this->render.angle > 360.0f) {
            this->render.angle -= 360.0f;
          }
        } else if (!CSkillActionMgr::GetInstance()->OnSkillDamage(damageMessage->unknown_0030, reinterpret_cast<int>(attacker), this, reinterpret_cast<CGameActor*>(damageMessage->unknown_0034))) {
          if (g_effectMgr->Spawn(this->render.accountId, skillEffect, offset.x, offset.y, offset.z, 0, -1, 0, 0) == nullptr) {
            CRagEffect* effect = this->LaunchEffect(skillEffect, offset, 0.0f);
            if (effect != nullptr) {
              effect->SendMsg(nullptr, AM_SETCOUNT, 0, 1, 0, 0, 0, 0);
              if ((skillEffect == 216 || skillEffect == 217) && attacker != nullptr) {
                effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&attacker->render.position), 0, 0, 0, 0);
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
              this->SendMsg(nullptr, AM_MAKE_NUMBER, 0, damage, damage >> 31, damageMessage->unknown_002C != 0 ? static_cast<int>(0xFF1E90FF) : -1, 0, 21, 0, 0, 0, 0, 0, 0, 0);
              kind = 22;
              color = 0xFFFFFF00;
            } else if (id == 148) {
              this->SendMsg(nullptr, AM_MAKE_CRITICAL_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
              this->SendMsg(nullptr, AM_MAKE_NUMBER, 0, damage, damage >> 31, static_cast<int>(0xFFFFFF00), 0, 16, 0, 0, 0, 0, 0, 0, 0);
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
        this->SendMsg(nullptr, AM_MAKE_NUMBER, 0, value, value >> 31, static_cast<int>(color), 0, static_cast<int>(kind), static_cast<int>(kind >> 32), 0, 0, 0, 0, 0, 0);

        if (!negative) {
          const auto isPlayer = IsPlayerJob(this->GetEffectiveJob(-1));
          if (this->render.unknown_0070 != 3) {
            if (isPlayer) {
              if (this->render.unknown_0070 != 2 && id != 39 && id != 97) {
                this->SetState(4, 1);
                if (motionSpeed > 1.0f) {
                  this->render.unknown_0060 = motionSpeed;
                } else {
                  this->SetAnimationSpeed(motionSpeed * this->render.speed);
                }
              }
            } else if (id != 39 && id != 97) {
              if (!g_session.IsMonsterActor(this) || !(motionSpeed < -0.00001f)) {
                this->SetState(4, 1);
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
      this->SendMsg(nullptr, AM_DEAD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  }
  case 108:
    this->SetState(2, 1);
    this->unknown_0378 = static_cast<float>(this->GetAttackMotion());
    this->RegisterPos();
    this->render.unknown_008C = timeGetTime();
    this->render.curMotion = 4;
    this->SetAnimationSpeed(1.0f);
    this->render.previousAnimation += this->Get8Dir(this->render.angle);
    if (this->unknown_02B0 != 0 && this->unknown_02A8.size() <= 1) {
      this->SendMsg(nullptr, AM_DEAD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case AM_WILL_BE_ATTACKED: {
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
  case AM_WILL_BE_ATTACKED2: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(b);
    if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
      damage->unknown_0024 = d;
      damage->action = 0x81;
    }
    this->unknown_02A8.push_back(*damage);
    break;
  }
  case AM_WILL_BE_ATTACKED2DASH: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(b);
    if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
      damage->unknown_0024 = d;
      damage->action = 0x84;
    }
    this->unknown_02A8.push_back(*damage);
    break;
  }
  case AM_SKILL:
    this->ProcessSkillMessage(sender, reinterpret_cast<const ActorSkillInfo*>(b), d, f);
    break;
  case AM_GROUNDSKILL:
    this->PlayGroundSkillEffect(reinterpret_cast<int>(sender), reinterpret_cast<const int*>(b), d, f);
    break;
  case AM_ACTION:
    this->ProcessHitMessage(sender, reinterpret_cast<const ActorHitInfo*>(b), d, f);
    break;
  case AM_ATTR_STATE_CHANGE: {
    const int* states = reinterpret_cast<const int*>(b);
    this->SetStatusStates(states[0], states[1], states[2], true);
    this->unknown_02C8 = states[3];
    break;
  }
  case AM_STANDENTRY: {
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
    this->SetState(0, 1);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    break;
  }
  case AM_INITSTANDENTRY: {
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
    this->SetSprAct(entry->job, entry->unknown_0008, 0);
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
      case PCSTATE_DEAD:
        this->SetState(3, 1);
        break;
      case PCSTATE_SIT:
        this->SetState(6, 1);
        break;
      default:
        this->SetState(0, 1);
        break;
      }
    } else {
      this->SetState(0, 1);
    }
    this->unknown_02C8 = entry->unknown_0044;
    if (entry->job == 0x2D2 || entry->job == 0x508) {
      this->SetGuildInfo(entry->guildId, entry->guildVersion);
    }
    this->SetPropensityInfo(entry->unknown_0068, entry->unknown_006C);
    if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
      this->render.color = 0x44FFFFFF;
    } else if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 2 && !StateFlag06(entry->stateFlags)) {
      this->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  }
  case AM_INITMOVEENTRY:
  case AM_INITMOVEENTRY2: {
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
    this->SetSprAct(entry->job, entry->unknown_0008, 0);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->unknown_02C8 = entry->unknown_0048;
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->unknown_006C, entry->unknown_0070);
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
      this->SetState(1, 1);
      this->unknown_023C = 0;
    } else {
      this->SetState(0, 1);
    }
    break;
  }
  case AM_MOVEENTRY:
  case AM_MOVEENTRY2: {
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
    this->SetSprAct(entry->job, entry->unknown_0008, 1);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->unknown_02C8 = entry->unknown_0048;
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->unknown_006C, entry->unknown_0070);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->unknown_0158[5] = entry->destX;
      *reinterpret_cast<int*>(this->unknown_0170) = entry->destY;
      this->SetState(1, 1);
      this->unknown_023C = 0;
    }
    break;
  }
  case AM_MOVE: {
    const int* route = reinterpret_cast<const int*>(b);
    this->routeStartTick = route[0];
    if (this->FindRoute(route[1], route[2], route[3], route[4], route[5], route[6], *reinterpret_cast<const int*>(&this->unknown_0170[4]))) {
      this->unknown_0158[5] = route[5];
      *reinterpret_cast<int*>(this->unknown_0170) = route[6];
      this->SetState(1, 1);
      this->unknown_023C = 0;
    } else {
      this->SetState(0, 1);
    }
    break;
  }
  case AM_DELETE_SHOP_TITLE:
    if (this->unknown_0264[2] != 0) {
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[2]));
      this->unknown_0264[2] = 0;
    }
    break;
  case AM_DELETE_CHAT_TITLE:
    if (this->unknown_0264[1] != 0) {
      g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[1]));
      this->unknown_0264[1] = 0;
    }
    break;
  case AM_MAKE_SHOP_TITLE:
    if (this->unknown_0264[2] == 0) {
      UIMerchantShopTitle* title = new UIMerchantShopTitle(0);
      this->unknown_0264[2] = reinterpret_cast<int>(title);
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[2]));
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[2])->SendMsg(nullptr, UM_SETINFO, b, static_cast<std::int16_t>(this->render.unknown_00FC), 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[2])->SendMsg(nullptr, UM_SETNAID, this->render.accountId, 0, 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[2])->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    if (!g_serviceOptions.Get(0xFA)) {
      reinterpret_cast<UIWindow*>(this->unknown_0264[2])->Show(0);
    }
    break;
  case AM_MAKE_CHAT_TITLE:
    if (this->unknown_0264[1] == 0) {
      UIChatRoomTitle* title = new UIChatRoomTitle();
      this->unknown_0264[1] = reinterpret_cast<int>(title);
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[1]));
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[1])->SendMsg(nullptr, UM_SETINFO, b, static_cast<std::int16_t>(this->render.unknown_00FC), 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[1])->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    break;
  case AM_MAKE_SKILL_RECHARGEGAGE:
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
  case AM_DELETE_SKILL_RECHARGEGAGE:
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
  case AM_CHAT: {
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
  case AM_STOPMOVE: {
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
      this->UnRegisterPos();
      if (lying == true) {
        this->unknown_02D0 = this->render.position.x;
        this->unknown_02D4 = this->render.position.z;
      }
      this->SetState(1, lying);
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
      this->SetState(0, lying);
    }
    break;
  }
  case AM_CORRECT_POS:
    this->unknown_0158[5] = b;
    *reinterpret_cast<int*>(this->unknown_0170) = d;
    mode->CellCenter(b, d, this->render.position.x, this->render.position.z);
    break;
  case AM_RESURRECTION:
    this->render.unknown_0070 = 0;
    this->render.isLieOnGround = false;
    this->render.unknown_00A4Bytes[1] = 1;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_02CC = 0;
    this->SetState(0, 1);
    break;
  case AM_OUTOFSIGHT:
    this->render.accountId = 0xFFFFFFFF;
    this->unknown_02B8 = 0;
    this->render.unknown_00A4Bytes[0] = 0;
    this->unknown_02DC = static_cast<int>(timeGetTime() + 510);
    if (this->unknown_02DC == 0) {
      this->unknown_02DC = 1;
    }
    break;
  case AM_MAKE_LEVEL99_EFFECT: {
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
    if (this->GetEffectiveJob(-1) == JT_G_TREASURE_BOX) {
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
  case AM_MAKE_LEVEL160_EFFECT:
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
  case AM_MAKE_LEVEL4TH_EFFECT: {
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
  case AM_STATUS_CHANGE: {
    const bool on = (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0;
    switch (static_cast<std::int64_t>(c) << 32 | static_cast<std::uint32_t>(b)) {
    case EFST_PROVOKE:
    case EFST_ENDURE:
    case EFST_CONCENTRATION:
    case EFST_HIDING:
    case EFST_CLOAKING:
    case EFST_ENCHANTPOISON:
    case EFST_POISONREACT:
    case EFST_QUAGMIRE:
    case EFST_ANGELUS:
    case EFST_BLESSING:
    case EFST_CRUCIS:
    case EFST_INC_AGI:
    case EFST_DEC_AGI:
    case EFST_SLOWPOISON:
    case EFST_IMPOSITIO:
    case EFST_SUFFRAGIUM:
    case EFST_ASPERSIO:
    case EFST_BENEDICTIO:
    case EFST_KYRIE:
    case EFST_MAGNIFICAT:
    case EFST_GLORIA:
    case EFST_LEXAETERNA:
    case EFST_ADRENALINE:
    case EFST_WEAPONPERFECT:
    case EFST_MAXIMIZE:
    case EFST_RIDING:
    case EFST_FALCON:
    case EFST_SHOUT:
    case EFST_BROKENARMOR:
    case EFST_BROKENWEAPON:
    case EFST_WEAPONPROPERTY:
    case EFST_ADRENALINE2:
    case EFST_PLUSATTACKPOWER:
    case EFST_PLUSMAGICPOWER:
    case EFST_CRYSTAL:
    case EFST_ATKER_ASPD:
    case EFST_ATKER_MOVESPEED:
    case EFST_ANGEL_PROTECT:
    case EFST_ELECTRICSHOCKER:
    case EFST_WUGRIDER:
    case EFST_ADORAMUS:
    case EFST_ACARAJE:
    case EFST_WATER_SCREEN:
      break;
    case EFST_TWOHANDQUICKEN:
    case EFST_ONEHANDQUICKEN: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x82, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_SPEARQUICKEN: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xFA, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_NJ_BUNSINJYUTSU:
      while (this->HasId(0x269)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x269, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x269, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DEVIL1: {
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
            this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, id, id >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
          }
        }
        this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xA4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      if (player) {
        player->unknown_0544 = this->LaunchEffect(0xA4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_PROPERTYUNDEAD:
      if (!on) {
        if (this->HasId(0x28F)) {
          vector3d position{0.0f, 0.0f, 0.0f};
          do {
            this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x28F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            this->LaunchEffect(0x290, position, 0.0f);
          } while (this->HasId(0x28F));
        }
      } else if (!this->HasId(0x28F)) {
        this->LaunchEffect(0x28F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SKE:
      if (this->render.accountId == g_session.m_aid) {
        if (on) {
          g_weatherEffect.StartDarken();
        } else {
          g_weatherEffect.StopDarken();
        }
      }
      break;
    case EFST_SMA_READY:
      if (on) {
        this->LaunchEffect(0x222, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_EXPLOSIONSPIRITS: {
      while (this->HasId(0xCB)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0xCB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFE, 0x80, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_STEELBODY:
    case EFST_DBARRIER: {
      while (this->HasId(0xFE)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xFE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x46C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
    case EFST_SG_SUN_WARM:
    case EFST_SG_MOON_WARM:
    case EFST_SG_STAR_WARM:
      while (this->HasId(0x1A2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1A2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      while (this->HasId(0x220)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1A2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DAURA:
      while (this->HasId(0x1A2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1A2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      while (this->HasId(0x220)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1A2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_KAITE:
      while (this->HasId(0x1A3)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1A3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1A3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_AURABLADE:
      while (this->HasId(0x16F)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x16F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      while (this->HasId(0x1DA)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1DA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x16F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x1DA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_STOP:
      while (this->HasId(0x1C5)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1C5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_MOON: {
      while (this->HasId(0x18A)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x18A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x18A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MARIONETTE_MASTER: {
      this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x18B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      while (this->HasId(0x18C)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x18C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MARIONETTE: {
      while (this->HasId(0x18C)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x18C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_LKCONCENTRATION: {
      this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (on) {
        this->LaunchEffect(0x171, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_BERSERK: {
      while (this->HasId(0x170)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x170, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_RG_CCONFINE_M:
    case EFST_RG_CCONFINE_S: {
      while (this->HasId(0x25C)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x25C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x25C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_WZ_SIGHTBLASTER: {
      while (this->HasId(0x259)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x259, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x259, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_POWERUP: {
      while (this->HasId(0x1C7)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1C7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1C7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_RUN: {
      if (this->HasId(0x1BA)) {
        this->LaunchEffect(0x1BB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1BA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case EFST_TING: {
      const vector3d& player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
      g_soundMgr->PlayWave("effect\\T_벽튕김.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0);
      this->LaunchEffect(0x1AA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (this->HasId(0x1BA)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1BA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case EFST_MAGICPOWER: {
      while (this->HasId(0x17E)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x17E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x17E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_OVERTHRUST:
    case EFST_OVERTHRUSTMAX: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x80, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_ENERGYCOAT: {
      while (this->HasId(0xA6)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xA9, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_TRICKDEAD:
      if (on) {
        this->unknown_017C = 1;
        if (this->GetMonsterTransformJob() > 0) {
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
        this->SetState(0, 1);
        this->render.isMotionFreezed = false;
      }
      break;
    case EFST_ILLUSION:
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
    case EFST_RICHMANKIM: {
      if (this->HasId(0x363) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x363, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x363, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_WHISTLE:
    case EFST_HUMMING:
    case EFST_INTOABYSS: {
      if (this->HasId(0x361) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x361, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_RINGNIBELUNGEN:
    case EFST_SIEGFRIED: {
      if (this->HasId(0x36B) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x36B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x36B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_APPLEIDUN: {
      if (this->HasId(0x35D) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x35D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x35D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_ETERNALCHAOS:
    case EFST_ROKISWEIL: {
      if (this->HasId(0x360) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x360, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x360, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_ASSASSINCROSS:
    case EFST_SERVICEFORYOU: {
      if (this->HasId(0x369) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x369, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x369, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_FORTUNEKISS:
    case EFST_DRUMBATTLEFIELD: {
      if (this->HasId(0x367) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x367, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x367, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_DONTFORGETME: {
      if (this->HasId(0x26) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x26, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_POEMBRAGI: {
      if (this->HasId(0x36D) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x36D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x36D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_DA_CONTRACT: {
      while (this->HasId(0x24A)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x24A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x24A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_DUPLELIGHT: {
      while (this->HasId(0x341)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x341, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(0x341, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        vector3d scale{2.0f, 0.0f, 0.0f};
        if (effect) {
          effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&scale), 0, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_ORATIO:
      while (this->HasId(0x2F3)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x2F3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x2F3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BLOOD_SUCKER:
      while (this->HasId(0x170)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x170, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BANDING:
      if (on) {
        if (!this->HasId(0x38D)) {
          this->LaunchEffect(0x38D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      }
      break;
    case EFST_CURSEDCIRCLE_ATKER:
    case EFST_CURSEDCIRCLE_TARGET: {
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
    case EFST_STRIKING:
      if (IsPlayerJob(this->GetEffectiveJob(-1))) {
        while (this->HasId(0x39D)) {
          this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x39D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        if (on) {
          CRagEffect* effect = this->LaunchEffect(0x39D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          if (effect) {
            effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x39D, 0, 0, 0);
          }
        }
      }
      break;
    case EFST_VENOMIMPRESS:
      while (this->HasId(0x314)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x314, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x314, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_FROSTMISTY:
      while (this->HasId(0x315)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x315, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x315, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_COLD:
      while (this->HasId(0x3AC)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x3AC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x3AC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SUMMON1:
      this->SetTetraBall(0, d, f);
      break;
    case EFST_SUMMON2:
      this->SetTetraBall(1, d, f);
      break;
    case EFST_SUMMON3:
      this->SetTetraBall(2, d, f);
      break;
    case EFST_SUMMON4:
      this->SetTetraBall(3, d, f);
      break;
    case EFST_SUMMON5:
      this->SetTetraBall(4, d, f);
      break;
    case EFST_ROLLINGCUTTER:
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
    case EFST_SIREN:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x36F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x36F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x36F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_SONG_OF_MANA:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x361, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DANCE_WITH_WUG:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x363, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x363, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_LERADS_DEW:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x367, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->LaunchEffect(0x367, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DEEP_SLEEP:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_MAKE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_NETHERWORLD:
      while (this->HasId(0x1C5)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1C5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x1C5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_CAMOUFLAGE:
      while (this->HasId(0x2E8)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x2E8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
    case EFST_MANHOLE:
      while (this->HasId(0x337)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x337, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        g_soundMgr->PlayWave("effect\\manhole.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
        if (!g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          this->LaunchEffect(0x337, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      }
      break;
    case EFST_SHADOWFORM:
      while (this->HasId(0x329)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x329, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x329, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_STEALTHFIELD:
      while (this->HasId(0x2E8)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x2E8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x2E8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_WUGDASH:
      while (this->HasId(0x2F0)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x2F0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x2F0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DA_BLACK:
      if (this->render.accountId == g_session.m_aid) {
        g_screenShakeDuration = 1;
        g_screenShakeMode = 1;
      }
      break;
    case EFST_MER_FLEE: {
      const int text = reinterpret_cast<int>("FLEE UP!");
      this->SendMsg(nullptr, AM_CHAT, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_ATK: {
      const int text = reinterpret_cast<int>("ATK UP!");
      this->SendMsg(nullptr, AM_CHAT, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_HIT: {
      const int text = reinterpret_cast<int>("DEX UP!");
      this->SendMsg(nullptr, AM_CHAT, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_HP: {
      const int text = reinterpret_cast<int>("HP UP!");
      this->SendMsg(nullptr, AM_CHAT, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_SP: {
      const int text = reinterpret_cast<int>("SP UP!");
      this->SendMsg(nullptr, AM_CHAT, 0, text, text >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_UNLIMITED_HUMMING_VOICE:
      if (this->HasId(0x361) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x361, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_MELODYOFSINK:
      if (this->HasId(0x57) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x57, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BEYOND_OF_WARCRY:
      if (this->HasId(0x31F) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x31F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x31F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SATURDAY_NIGHT_FEVER:
      if (this->HasId(0x360) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x360, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(0x360, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_PAIN_KILLER:
    case EFST_LIGHT_OF_REGENE:
    case EFST_OVERED_BOOST:
    case EFST_STYLE_CHANGE:
    case EFST_MAGMA_FLOW:
    case EFST_PYROCLASTIC:
    case EFST_VOLCANIC_ASH:
    case EFST_OVERLAPEXPUP:
    case EFST_EQUIPPED_DIVINE_ARMOR:
    case EFST_EQUIPPED_HOLY_ARMOR:
      break;
    case EFST_EQUIPED_FLOOR:
      g_effectMgr->SpawnLevel99(this, on, f);
      break;
    case EFST_GRANITIC_ARMOR:
      while (this->HasId(0x3C3)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x3C3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_ALL_RIDING: {
      if (this->render.unknown_0070 == 25) {
        this->SetState(0, 1);
      }
      int job;
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetRiding(1);
        job = this->GetEffectiveJob(-1);
      } else {
        this->SetRiding(0);
        job = JobNames::BaseJob(this->GetEffectiveJob(-1));
      }
      this->SetActorJob(job);
      this->SetBodyPalette(this->render.unknown_0048);
      this->SetSprRobe(0, 1);
      break;
    }
    case EFST_MONSTER_TRANSFORM:
    case EFST_KYOUGAKU:
    case EFST_ACTIVE_MONSTER_TRANSFORM: {
      int slot;
      switch (b) {
      case EFST_MONSTER_TRANSFORM:
        slot = 0;
        break;
      case EFST_KYOUGAKU:
        slot = 1;
        break;
      case EFST_ACTIVE_MONSTER_TRANSFORM:
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
    case EFST_KO_JYUMONJIKIRI:
      g_effectMgr->Retire(this->render.accountId, 0x3E6);
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0 && g_effectMgr->HasAllocator(0x3E6)) {
        g_effectMgr->Spawn(this->render.accountId, 0x3E6, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_KYOMU:
      while (this->HasId(0x3C3)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x3C3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_KAGEMUSYA:
      while (this->HasId(0x3EC)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x3EC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3EC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_MEIKYOUSISUI:
      while (this->HasId(0x18A)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x18A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
    case EFST_ZANGETSU:
      while (this->HasId(0x3EB)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x3EB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3EB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_GENSOU:
      g_effectMgr->Retire(this->render.accountId, 0x3ED);
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0 && g_effectMgr->HasAllocator(0x3ED)) {
        CEffect* orb = g_effectMgr->Spawn(this->render.accountId, 0x3ED, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (orb) {
          static_cast<CEnergyOrbEffect*>(orb)->SetCount(10);
        }
      }
      while (this->HasId(0x3C3)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x3C3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_AKAITSUKI:
      while (this->HasId(0x220)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_UNKNOWN_NAME:
      this->unknown_02F8 = (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0;
      break;
    case EFST_HAT_EFFECT:
      this->SetStackedEffect(0x3F4, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case EFST_MOONSTAR:
      this->SetStackedEffect(0x40A, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case EFST_DECORATION_OF_MUSIC:
      this->SetStackedEffect(0x437, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case EFST_STRANGELIGHTS:
      this->SetStackedEffect(0x40B, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case EFST_SUPER_STAR:
      this->SetStackedEffect(0x40C, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case EFST_ON_PUSH_CART:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        if (static_cast<std::int64_t>(this->unknown_02FC) == (static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f))) {
          break;
        }
        this->unknown_02FC = f;
        if (StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags)) {
          break;
        }
        this->SendMsg(nullptr, AM_DELETE_CART, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_MAKE_CART, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->unknown_02FC = 0;
        this->SendMsg(nullptr, AM_DELETE_CART, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_FLOWER_LEAF:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        g_effectMgr->Spawn(this->render.accountId, 0x3F5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x3F5);
      }
      break;
    case EFST_RAY_OF_PROTECTION:
      while (this->HasId(0x3FB)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x3FB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x3FB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_CLAN_INFO:
      this->unknown_0305 = (static_cast<std::int64_t>(i) << 32 | static_cast<std::uint32_t>(h)) != 0;
      this->unknown_0308 = h;
      break;
    case EFST_HANDICAPSTATE_ICEEXPLO:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0, 50, 0, 150, 0, 255, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_MAXPAIN:
      while (this->HasId(0x40D)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x40D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x40D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DARKCROW:
      while (this->HasId(0x410)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x410, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x410, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x410, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(0x410, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_TELEKINESIS_INTENSE:
      while (this->HasId(0x418)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x418, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x418, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_ILLUSIONDOPING:
      while (this->HasId(0x419)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x419, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x419, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_WIDEWEB:
      while (this->HasId(0x194)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x194, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BURNT:
      while (this->HasId(0x41E)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x41E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x41E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_CHILL:
      while (this->HasId(0x41F)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x41F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x41F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_OFFERTORIUM:
      while (this->HasId(0x421)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x421, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(0x421, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_C_MARKER:
      this->SendMsg(this, AM_DELETE_EFFECT, 0, 0x433, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (on) {
        if (g_session.unknown_5C28.find(static_cast<int>(this->render.accountId)) == g_session.unknown_5C28.end() && g_effectMgr->HasAllocator(0x425)) {
          g_effectMgr->Spawn(this->render.accountId, 0x425, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
        }
        if (g_effectMgr->HasAllocator(0x433)) {
          g_effectMgr->Spawn(this->render.accountId, 0x433, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
        }
      }
      break;
    case EFST_ANTI_M_BLAST:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x470, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x470);
      }
      break;
    case EFST_E_QD_SHOT_READY:
      g_effectMgr->Spawn(0, 0x434, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      break;
    case EFST_H_MINE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x473, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x473);
      }
      break;
    case EFST_H_MINE_SPLASH:
      if (on) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x425, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(0, 0x432, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_FLOWERSMOKE:
      this->SetStackedEffect(0x43B, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case EFST_HALLUCINATIONWALK:
    case EFST_NPC_HALLUCINATIONWALK: {
      while (this->HasId(0x318)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x318, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 0) {
        break;
      }
      CRagEffect* effect = this->LaunchEffect(0x318, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (!effect) {
        break;
      }
      effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x318, 0, 0, 0);
      break;
    }
    case EFST_SOULLINK:
    case EFST_SOULSHADOW:
    case EFST_SOULFAIRY:
    case EFST_SOULFALCON:
    case EFST_SOULGOLEM:
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
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x1A8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HAPPINESS_STAR:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43F, true);
      } else {
        this->SetStackedEffect(0x43F, false);
      }
      break;
    case EFST_FSTONE:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43C, true);
      } else {
        this->SetStackedEffect(0x43C, false);
      }
      break;
    case EFST_QSCARABA:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43D, true);
      } else {
        this->SetStackedEffect(0x43D, false);
      }
      break;
    case EFST_LJOSALFAR:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x43E, true);
      } else {
        this->SetStackedEffect(0x43E, false);
      }
      break;
    case EFST_MAPLE_FALLS:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x441, true);
      } else {
        this->SetStackedEffect(0x441, false);
      }
      break;
    case EFST_DRESS_UP:
      g_session.unknown_6184 = g_session.m_aid == this->render.accountId ? f : g_session.unknown_6184;
      this->unknown_03DC = f;
      this->SetSprJob(this->GetEffectiveJob(-1));
      this->SetImfFileName();
      this->SetHeadPaletteName();
      this->SetBodyPaletteName();
      break;
    case EFST_MARKING_USE_CHANGEMONSTER:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        g_effectMgr->Spawn(this->render.accountId, 0x442, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x442);
      }
      break;
    case EFST_MAGICAL_FEATHER:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x443, true);
      } else {
        this->SetStackedEffect(0x443, false);
      }
      break;
    case EFST_MERMAID_LONGING:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x444, true);
      } else {
        this->SetStackedEffect(0x444, false);
      }
      break;
    case EFST_TIME_ACCESSORY:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0) {
        this->SetStackedEffect(0x447, true);
      } else {
        this->SetStackedEffect(0x447, false);
      }
      break;
    case EFST_HEAD_EQUIPMENT_EFFECT:
      this->SetStackedEffect(f + 0x9B5, (static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) > 0);
      break;
    case EFST_SPRITEMABLE:
      g_effectMgr->Retire(this->render.accountId, 0x448);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x448, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      }
      break;
    case EFST_SUHIDE:
      if (on == true) {
        this->SetState(0x30, 1);
        this->render.isMotionFreezed = true;
        this->render.unknown_0114 = 1;
      } else {
        this->render.unknown_0070 = 0;
        this->render.isLieOnGround = false;
        this->render.unknown_00A4Bytes[1] = 1;
        this->SetState(0, 1);
        this->render.isMotionFreezed = false;
        this->render.unknown_0114 = 0;
      }
      break;
    case EFST_SU_STOOP:
      if (on == true) {
        this->SetState(0x2F, 1);
      } else if (this->render.unknown_0114 == 0) {
        this->SetState(0, 1);
      }
      break;
    case EFST_TUNAPARTY:
      if (on == true) {
        if (!g_effectMgr->Has(this->render.accountId, 0x449)) {
          g_effectMgr->Spawn(this->render.accountId, 0x449, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x449);
      }
      break;
    case EFST_SV_ROOTTWIST:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x456, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x456);
      }
      break;
    case EFST_BITESCAR:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x45F, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x45F);
      }
      break;
    case EFST_SOULATTACK:
      if (g_session.IsDoramJob(this->GetEffectiveJob(-1))) {
        static_cast<CPc*>(this)->unknown_04D8[0] = on;
      }
      break;
    case EFST_IMMUNE_PROPERTY_NOTHING:
    case EFST_IMMUNE_PROPERTY_WATER:
    case EFST_IMMUNE_PROPERTY_GROUND:
    case EFST_IMMUNE_PROPERTY_FIRE:
    case EFST_IMMUNE_PROPERTY_WIND:
    case EFST_IMMUNE_PROPERTY_POISON:
    case EFST_IMMUNE_PROPERTY_SAINT:
    case EFST_IMMUNE_PROPERTY_DARKNESS:
    case EFST_IMMUNE_PROPERTY_TELEKINESIS:
    case EFST_IMMUNE_PROPERTY_UNDEAD: {
      const int effectId = b + 0x87;
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (g_effectMgr->Has(this->render.accountId, effectId) == true) {
        g_effectMgr->Retire(this->render.accountId, effectId);
      }
      break;
    }
    case EFST_GRAVITYCONTROL: {
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
    case EFST_SUNSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4BE) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4BE);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4BE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_LUNARSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4BF) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4BF);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4BF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_STARSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4C0) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C0);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_UNIVERSESTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4C1) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C1);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_FLASHKICK:
      if (g_effectMgr->Has(this->render.accountId, 0x4C5) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C5);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_SOULUNITY: {
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
    case EFST_ASSUMPTIO:
    case EFST_ASSUMPTIO2:
    case EFST_ASSUMPTIO_BUFF:
      while (this->HasId(0x177)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x177, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        this->LaunchEffect(0x177, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SOULCURSE:
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
    case EFST_SOUND_OF_DESTRUCTION: {
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
    case EFST_EARTHSHAKER: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x576)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x576, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MAGIC_POISON: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x18C)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x18C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MISTY_FROST: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x58C)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x58C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x58C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_PRISON: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x5F5)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x5F5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x5F5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MADOGEAR_TYPE: {
      CGameActor* target = this;
      if (this->render.accountId != g_session.m_aid) {
        target = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(this->render.accountId));
      }
      if (!target) {
        break;
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        if ((static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f)) == 2) {
          target->SetMadogearType(2);
          if (this->render.accountId == g_session.m_aid) {
            g_session.unknown_8074 = 2;
          }
        } else {
          target->SetMadogearType(1);
          if (this->render.accountId == g_session.m_aid) {
            g_session.unknown_8074 = 1;
          }
        }
        target->SendMsg(nullptr, AM_MAKE_CHICKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        target->SetMadogearType(0);
        if (this->render.accountId == g_session.m_aid) {
          g_session.unknown_8074 = 0;
        }
        target->SendMsg(nullptr, AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case EFST_RELIEVE_DAMAGE: {
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
    case EFST_LOCKON_LASER: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (!actor) {
        break;
      }
      if (actor->HasId(0x641)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x641, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x641, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_HELLS_PLANT_ARMOR: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (!actor) {
        break;
      }
      if (actor->HasId(0x64A)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x64A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(0x64A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_HANDICAPSTATE_DEEPBLIND:
      if (this->render.accountId != g_session.m_aid) {
        break;
      }
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        if (!this->HasId(0x14E)) {
          this->LaunchEffect(0x14E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
        g_overrideColor = 1;
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0xA4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        CPlayer* const player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
        if (player) {
          player->unknown_0544 = this->LaunchEffect(0xA4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      } else {
        if (this->HasId(0x14E) == true) {
          this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x14E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        g_overrideColor = 0;
      }
      this->SetInstantValue(0x10, on);
      break;
    case EFST_HANDICAPSTATE_DEEPSILENCE:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_MAKE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_SWOONING:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_MAKE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8C5);
        g_effectMgr->Spawn(this->render.accountId, 0x8C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8C5);
      }
      break;
    case EFST_HANDICAPSTATE_LASSITUDE:
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) == 1) {
        this->SendMsg(nullptr, AM_MAKE_LASSITUDE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x8C4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_LASSITUDE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_FROSTBITE:
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
    case EFST_HANDICAPSTATE_LIGHTNINGSTRIKE:
      g_effectMgr->Retire(this->render.accountId, 0x8DC);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8DC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_CRYSTALLIZATION:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8E1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x8E1);
      }
      break;
    case EFST_HANDICAPSTATE_HOLYFLAME:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x8C6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      this->SetInstantValue(0xC, on);
      break;
    case EFST_HANDICAPSTATE_CONFLAGRATION:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8DD);
        g_effectMgr->Spawn(this->render.accountId, 0x8DD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8DD);
      }
      this->SetInstantValue(0xD, on);
      break;
    case EFST_HANDICAPSTATE_MISFORTUNE:
      if (on == true) {
        this->SendMsg(nullptr, AM_MAKE_MISFORTUNE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_MISFORTUNE_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_DEADLYPOISON:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0, 0xAA, 0, 0, 0, 0xAA, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x8E2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8E2);
      }
      this->SetInstantValue(0xE, on);
      break;
    case EFST_HANDICAPSTATE_DEPRESSION:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0, 0, 0, 0, 0, 0xFF, 0, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      this->SetInstantValue(0xF, on);
      break;
    case EFST_DRAGONIC_AURA: {
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
    case EFST_FIRST_BRAND: {
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x806, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const second = g_effectMgr->Spawn(this->render.accountId, 0x803, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const third = g_effectMgr->Spawn(this->render.accountId, 0x804, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (second) {
          second->SetDelay(1000);
        }
        if (third) {
          third->SetDelay(1000);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x803);
        g_effectMgr->Retire(this->render.accountId, 0x804);
      }
      break;
    }
    case EFST_SECOND_BRAND: {
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x821, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const second = g_effectMgr->Spawn(this->render.accountId, 0x81e, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        CEffect* const third = g_effectMgr->Spawn(this->render.accountId, 0x81f, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (second) {
          second->SetDelay(1000);
        }
        if (third) {
          third->SetDelay(1000);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x821);
        g_effectMgr->Retire(this->render.accountId, 0x81e);
        g_effectMgr->Retire(this->render.accountId, 0x81f);
      }
      break;
    }
    case EFST_HOLY_OIL:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7c7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7c7);
      }
      break;
    case EFST_MEDIALE:
      if (on == true) {
        if (!g_serviceOptions.Get(0xe5) && !g_effectMgr->Find(this->render.accountId, 0x712)) {
          g_effectMgr->Spawn(this->render.accountId, 0x712, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x712);
      }
      break;
    case EFST_DEADLY_DEFEASANCE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x6cd, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x6cd)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_CLIMAX:
      g_effectMgr->Retire(this->render.accountId, 0x708);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x708, this->render.position.x, this->render.position.y, this->render.position.z, 0, f, 0, 0);
      }
      break;
    case EFST_FIRST_FAITH_POWER:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7b7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7b7);
      }
      break;
    case EFST_SECOND_JUDGE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7ba, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x7bd, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7ba);
        g_effectMgr->Retire(this->render.accountId, 0x7bd);
      }
      break;
    case EFST_THIRD_EXOR_FLAME:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x7be, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, 0x7c1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7be);
        g_effectMgr->Retire(this->render.accountId, 0x7c1);
      }
      break;
    case EFST_ABYSS_SLAYER:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x84e, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x84e);
      }
      break;
    case EFST_D_MACHINE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x719, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        this->LaunchEffect(0x8f8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x719)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_A_MACHINE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x72f, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x72f);
      }
      break;
    case EFST_COMPETENTIA:
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
    case EFST_CLIMAX_CRYIMP:
      if (on == true) {
        if (!g_effectMgr->Find(this->render.accountId, 0x743)) {
          g_effectMgr->Spawn(this->render.accountId, 0x743, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x743);
      }
      break;
    case EFST_SOUNDBLEND:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x80c, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x80c)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_WINDSIGN:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x76c, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x76c);
      }
      break;
    case EFST_E_SLASH_COUNT:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x779, this->render.position.x, this->render.position.y, this->render.position.z, 0, f, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x779);
      }
      break;
    case EFST_ELEMENTAL_VEIL:
      if (on == true) {
        this->LaunchEffect(0x2e8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x2e8) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x2e8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_BO_HELL_DUSTY:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8be, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_CLOUD_POISON:
      if (on == true) {
        this->LaunchEffect(0x8e5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x8e5) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x8e5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_SPORE_EXPLOSION_DEBUFF:
      if (on == true) {
        this->LaunchEffect(0x8e6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x8e6) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 0x8e6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_FIRM_FAITH:
    case EFST_MUSICAL_INTERLUDE:
      if (on == true) {
        this->LaunchEffect(0x8f8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_POWERFUL_FAITH:
      if (on == true) {
        this->LaunchEffect(0x8f6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SPELL_ENCHANTING:
      if (on == true) {
        this->LaunchEffect(0x8fa, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_PRE_ACIES:
      if (on == true) {
        this->LaunchEffect(0x8f0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_KILLING_AURA:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x8fe, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x8fe);
        g_effectMgr->Spawn(this->render.accountId, 0x8ff, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    case EFST_CURSE_R_CUBE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x901, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x901)) {
        static_cast<CCurseOfCubeEffect*>(effect)->Restart();
      }
      break;
    case EFST_CURSE_B_CUBE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x902, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x902)) {
        static_cast<CCurseOfCubeEffect*>(effect)->Restart();
      }
      break;
    case EFST_BLOCK:
      if (on == true) {
        g_windowMgr.EnsureWindow(WID_DEBUFFREMOVEWND);
      } else {
        g_windowMgr.CloseWindow(WID_DEBUFFREMOVEWND);
      }
      break;
    default:
      if (on == true) {
        CSkillActionMgr::GetInstance()->OnEfstBegin(b, f, h, l, this, (static_cast<std::int64_t>(k) << 32 | static_cast<std::uint32_t>(j)) != 0);
      } else {
        CSkillActionMgr::GetInstance()->OnEfstEnd(b, f, h, l, this);
      }
      break;
    }
    break;
  }
  case AM_CHANGE_COLOR: {
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
  case AM_RETURN_COLOR:
    this->render.unknown_0090 = 0;
    this->render.unknown_00A3 = 0;
    break;
  case AM_DELETE_BUYING_SHOP_TITLE:
    if (this->unknown_0264[4] == 0) {
      break;
    }
    g_windowMgr.Schedule(reinterpret_cast<UIWindow*>(this->unknown_0264[4]));
    this->unknown_0264[4] = 0;
    break;
  case AM_MAKE_BUYING_SHOP_TITLE: {
    if (this->unknown_0264[4] == 0) {
      UIMerchantShopTitle* const title = new UIMerchantShopTitle(1);
      this->unknown_0264[4] = reinterpret_cast<int>(title);
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(reinterpret_cast<UIWindow*>(this->unknown_0264[4]));
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->SendMsg(nullptr, UM_SETINFO, b, static_cast<std::int16_t>(this->render.unknown_00FC), 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->SendMsg(nullptr, UM_SETNAID, this->render.accountId, 0, 0, 0);
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    if (g_serviceOptions.Get(0xFA)) {
      break;
    }
    reinterpret_cast<UIWindow*>(this->unknown_0264[4])->Show(0);
    break;
  }
  case AM_FALLEN_ANGEL:
    if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1))) {
      if (CRagEffect* effect = this->LaunchEffect(0xA6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xF3, 0, 100, 0);
      }
      if (CEffect* fallenAngel = g_effectMgr->Find(this->render.accountId, 0x474)) {
        vector3d position{0.0f, 0.0f, 0.0f};
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(b, d, position.x, position.z);
        position.y = this->render.position.y;
        static_cast<CFallenAngelEffect*>(fallenAngel)->SetPosition(position);
      }
    }
    this->SendMsg(nullptr, AM_STOPMOVE, 0, b, b >> 31, d, d >> 31, 15, 0, 0, 0, 0, 0, 0, 0);
    this->SetAction(this->render.animation, 6, 1);
    this->render.curMotion = 7;
    this->render.isMotionFreezed = true;
    break;
  case AM_UPDATE_MONSTER_GAGE:
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
  case AM_MAKE_MISFORTUNE_EFFECT:
    if (this->unknown_03D4 == 0) {
      CGameObject* effect = mode->SpawnMsgEffect(68, &this->render.position, this->render.accountId, true);
      this->unknown_03B0.push_back(effect);
      effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D4 = reinterpret_cast<int>(effect);
    }
    break;
  case AM_DELETE_MISFORTUNE_EFFECT:
    if (this->unknown_03D4 != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_03D4)->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D4 = 0;
    }
    break;
  case AM_MAKE_DEEPSILENCE_EFFECT:
    if (this->unknown_03D8 == 0) {
      CGameObject* effect = mode->SpawnMsgEffect(9, &this->render.position, this->render.accountId, true);
      this->unknown_03B0.push_back(effect);
      effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D8 = reinterpret_cast<int>(effect);
    }
    break;
  case AM_DELETE_DEEPSILENCE_EFFECT:
    if (this->unknown_03D8 != 0) {
      reinterpret_cast<CGameObject*>(this->unknown_03D8)->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->unknown_03D8 = 0;
    }
    break;
  default:
    this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    break;
  }
}

