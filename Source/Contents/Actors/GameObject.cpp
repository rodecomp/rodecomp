#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/ActorEnums.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Camera/Camera.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/World/World.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "Render/Textures/SpritePixels.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"
#include "Contents/Monster/ChampionMonsterMgr.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/Pc.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Skills/EnergyOrbEffect.h"
#include "Contents/Effects/Skills/MonsterSkillEffects.h"
#include "Contents/Effects/Core/MsgEffect.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/Map/MapSkyMgr.h"
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
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Buttons/UIBalloonOpenBtn.h"
#include "UI/Dialogs/UIMessageBox.h"
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
void CGameObject::SendMsg(CGameObject*, std::int64_t, std::int64_t, std::int64_t, std::int64_t, std::int64_t, std::int64_t, std::int64_t) {
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
      (g_session.IsHirelingJob(this->GetEffectiveJob(-1)) &&
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

  if (g_session.guildLeader == 1 && seasonal == true) {
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
    extra += effect->scaleParameter;
  }
  if (g_championMonsterMgr->IsChampion(static_cast<int>(this->render.spriteJob))) {
    extra += g_championMonsterMgr->GetValue(static_cast<int>(this->render.spriteJob));
  }
  if (extra != 0.0f) {
    distanceScale *= extra;
  }

  this->render.distanceScale = distanceScale / g_heightScale;
  const float scale =
    g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) * this->render.scale) * this->render.distanceScale;

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
    if (this->render.visibilityOptions[1] != 0) {
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
    if (this->render.bodyPalette != 0) {
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->render.name.c_str()))->palette;
    }

    int blend = 257;
    this->DrawSpriteEffects(&quad, &cell, box, scale, bottomRhw, topRhw, palette, image, &blend, color);

    if (this->render.spriteJob == 0x235) {
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
      const short level = this->render.effectPhase;
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

  this->render.effectPhaseBytes[0] = 1;
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
  this->render.distanceScale = distanceScale / g_heightScale;
  const float scale =
    g_renderer->UiScaleX() * (static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) * this->render.scale) * this->render.distanceScale;

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

      if (headFrame->anchorCount != 0 && bodyFrame->anchorCount != 0 && headFrame->anchors[0].attachmentId == bodyFrame->anchors[0].attachmentId) {
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

      if (frame->anchorCount != 0 && headFrame->anchorCount != 0 && frame->anchors[0].attachmentId == headFrame->anchors[0].attachmentId) {
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

      if (this->render.visibilityOptions[1] != 0) {
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
      if (this->render.bodyPalette != 0) {
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

  this->render.effectPhaseBytes[0] = 1;
}

// OFFSET: 0x00CB35F0
// FUNCTION: fn_21105
void CRenderObject::DrawShadow(int argument, float scale) {
  if (this->render.spriteJob == 565 || this->render.visibilityOptions[0] == 0 || this->render.shadowScale == 0.0f || scale == 0.0f) {
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
  const double size = pulse * scale * this->render.shadowScale;
  const double factor = distanceScale;

  SpriteQuad quad;
  quad.top = static_cast<float>(cell.y * factor * cell.scaleY * size + screenY);
  quad.bottom = static_cast<float>(((image->doubleHeight + 1) * image->height + cell.y) * factor * cell.scaleY * size + screenY + 1.0);
  float left = static_cast<float>(cell.x * factor * cell.scaleX * size + screenX);
  quad.right = static_cast<float>(((image->doubleWidth + 1) * image->width + cell.x) * factor * cell.scaleX * size + screenX + 1.0);
  if (quad.top >= static_cast<float>(g_renderer->viewport.height) || quad.bottom <= 0.0f || left >= static_cast<float>(g_renderer->viewport.width) ||
      quad.right <= 0.0f) {
    return;
  }

  quad.left = static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) + left;
  quad.right = static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) + quad.right;
  quad.color = static_cast<std::uint32_t>(((static_cast<std::uint32_t>(cell.color >> 24) << 8 | (cell.color & 0xFF)) << 8 | (cell.color >> 8 & 0xFF)) << 8) |
               (cell.color >> 16 & 0xFF);
  quad.bottomRhw = 1.0f / (depth - (quad.bottom - screenY) * g_cotangent) + 0.00001f;
  quad.topRhw = 1.0f / (depth - (quad.top - screenY) * g_cotangent) + 0.00001f;
  quad.angle = static_cast<float>(cell.rotation);
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
  CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
  if (!texture) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(image, palette, record);
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

  primitive->sourceBlend = 5;
  primitive->destinationBlend = blend != 5 ? 6 : 2;
  primitive->depthFlags = g_renderer->twoPass != 1;
  g_renderer->queues.Submit(primitive, static_cast<std::uint16_t>(blend | this->render.blendFlags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
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
    target->SendMsg(nullptr, AM_REQ_ATTACK, value, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00CB4070
// FUNCTION: fn_21110
void CGameActor::ApplySavedShapeShift() {
  const int pending = this->savedShapeShift;
  this->shapeShift = pending;
  this->savedShapeShift = 0;
  this->render.isLieOnGround = false;
  this->render.visibilityOptions[1] = 1;
  this->render.isMotionFreezed = false;
  this->render.specialMotionLocked = 0;

  int value;
  switch (pending) {
  case 1:
    value = 0x1104;
    break;
  case 2:
    value = 0x1105;
    break;
  default:
    value = this->job;
    break;
  }
  this->SetSprJob(value);
  this->SetBodyPaletteName();

  if (this->render.state != 3) {
    this->render.state = 0;
    this->SetState(0, 1);
  }
}

// OFFSET: 0x00CB4100
// FUNCTION: fn_21111
void EffectActor::SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t, std::int64_t, std::int64_t, std::int64_t) {
  switch (message) {
    case AM_MASTER_DEAD:
      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        (*it)->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
      }

      this->state.effects.clear();
      break;

    case AM_MAKE_MAGIC_TARGET:
      if (!this->state.second) {
        this->state.second = this->LaunchEffect(static_cast<int>(b), vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.second) {
          return;
        }
      }

      this->state.second->SendMsg(nullptr, AM_SET_DURATION, 0, c, 0, 0, 0, 0);
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
        this->state.first = this->LaunchEffect(static_cast<int>(b), vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.first) {
          return;
        }
      }

      if (b > 0) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, b, 0, 0, 0, 0, 0);
        this->state.first = this->LaunchEffect(static_cast<int>(b), vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (!this->state.first) {
          return;
        }
      }

      this->state.first->SendMsg(nullptr, AM_SET_DURATION, 0, c, 0, 0, 0, 0);
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
      if (g_effectMgr->HasAllocator(static_cast<int>(b)) && sender) {
        g_effectMgr->Retire(static_cast<CRenderObject*>(sender)->render.accountId, static_cast<int>(b));
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (effect->state.type == b) {
          effect->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
          this->state.effects.erase(it);
          return;
        }
      }
      break;

    case AM_DELETE_TAGGED_EFFECT:
      if (g_effectMgr->HasAllocator(static_cast<int>(c)) && sender) {
        g_effectMgr->Retire(static_cast<CRenderObject*>(sender)->render.accountId, static_cast<int>(b), static_cast<int>(c));
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (static_cast<int>(effect->state.tag) == b &&
                   effect->state.type == c) {
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
        } else if (effect->state.type == b) {
          effect->SendMsg(this, AM_EFFECT_LOOP_COUNT, 0, b, static_cast<int>(c), static_cast<int>(c >> 32), 0, 0);
        }
      }
      break;

    case AM_IS_HAVE_EFFECT:
      if (g_effectMgr->HasAllocator(static_cast<int>(b)) == true && sender) {
        if (g_effectMgr->Has(static_cast<CRenderObject*>(sender)->render.accountId, static_cast<int>(b)) == true) {
          *reinterpret_cast<int*>(static_cast<std::intptr_t>(b)) = -1;
        }
        return;
      }

      for (auto it = this->state.effects.begin(); it != this->state.effects.end(); ++it) {
        CRagEffect* const effect = *it;
        if (!effect) {
          this->state.effects.erase(it);
        } else if (*reinterpret_cast<int*>(static_cast<std::intptr_t>(b)) == effect->state.type) {
          *reinterpret_cast<int*>(static_cast<std::intptr_t>(b)) = -1;
          return;
        }
      }
      break;
  }
}

namespace {
// INLINED
int TransformSlot(int efst) {
  switch (efst) {
  case EFST_MONSTER_TRANSFORM:
    return 0;
  case EFST_KYOUGAKU:
    return 1;
  case EFST_ACTIVE_MONSTER_TRANSFORM:
    return 2;
  default:
    return 0;
  }
}

struct PcStandEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int sex;
  /* 000C */ int cellX;
  /* 0010 */ int cellY;
  /* 0014 */ int direction;
  /* 0018 */ int speed;
  /* 001C */ int xSize;
  /* 0020 */ int ySize;
  /* 0024 */ int head;
  /* 0028 */ int weapon;
  /* 002C */ int secondaryWeapon;
  /* 0030 */ int accessory;
  /* 0034 */ int state;
  /* 0038 */ int bodyState;
  /* 003C */ int effectState;
  /* 0040 */ std::uint32_t stateFlags;
  /* 0044 */ int healthState;
  /* 0048 */ int shield;
  /* 004C */ int accessory2;
  /* 0050 */ int accessory3;
  /* 0054 */ int headPalette;
  /* 0058 */ int bodyPalette;
  /* 005C */ int headDir;
  /* 0060 */ int guildId;
  /* 0064 */ int guildVersion;
  /* 0068 */ int honor;
  /* 006C */ int virtue;
  /* 0070 */ int robe;
  /* 0074 */ int pkState;
  /* 0078 */ int bodyStyle;
};

struct PcMoveEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int sex;
  /* 000C */ std::uint32_t moveStartTick;
  /* 0010 */ int startX;
  /* 0014 */ int startY;
  /* 0018 */ int startSubX;
  /* 001C */ int startSubY;
  /* 0020 */ int destX;
  /* 0024 */ int destY;
  /* 0028 */ int speed;
  /* 002C */ int head;
  /* 0030 */ int weapon;
  /* 0034 */ int secondaryWeapon;
  /* 0038 */ int accessory;
  /* 003C */ int bodyState;
  /* 0040 */ int effectState;
  /* 0044 */ std::uint32_t stateFlags;
  /* 0048 */ int healthState;
  /* 004C */ int shield;
  /* 0050 */ int accessory2;
  /* 0054 */ int accessory3;
  /* 0058 */ int headPalette;
  /* 005C */ int bodyPalette;
  /* 0060 */ int headDir;
  /* 0064 */ int guildId;
  /* 0068 */ int guildVersion;
  /* 006C */ int honor;
  /* 0070 */ int virtue;
  /* 0074 */ int robe;
  /* 0078 */ int pkState;
  /* 007C */ int bodyStyle;
};
}

// OFFSET: 0x00CB4800
// FUNCTION: fn_21112
void CGameActor::SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  bool visible = true;
  if (this->render.accountId != g_session.m_aid) {
    if (StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags)) {
      visible = false;
    }
  }

  switch (message) {
  case 21:
    break;
  case AM_DELETE_PROHIBITEFFECT:
    if (this->statusEffects[10] != 0) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->RemoveObject(static_cast<CRenderObject*>(this->statusEffects[10]));
    }
    break;
  case AM_MSGEFFECT_DEAD:
    for (auto it = this->msgEffectList.begin(); it != this->msgEffectList.end(); ++it) {
      if (*it == sender) {
        CGameObject* const effect = *it;
        if (this->statusEffects[1] == effect) {
          this->statusEffects[1] = 0;
        } else if (this->statusEffects[0] == effect) {
          this->statusEffects[0] = 0;
        } else if (this->statusEffects[2] == effect) {
          this->statusEffects[2] = 0;
        } else if (this->statusEffects[3] == effect) {
          this->statusEffects[3] = 0;
        } else if (this->statusEffects[4] == effect) {
          this->statusEffects[4] = 0;
        } else if (this->statusEffects[5] == effect) {
          this->statusEffects[5] = 0;
        } else if (this->statusEffects[6] == effect) {
          this->statusEffects[6] = 0;
        } else if (this->statusEffects[8] == effect) {
          this->statusEffects[8] = 0;
        } else if (this->statusEffects[9] == effect) {
          this->statusEffects[9] = 0;
        } else if (this->statusEffects[7] == effect) {
          this->statusEffects[7] = 0;
        } else if (this->statusEffects[10] == effect) {
          this->statusEffects[10] = 0;
        } else if (this->statusEffects[11] == effect) {
          this->statusEffects[11] = 0;
        } else if (this->statusEffects[12] == effect) {
          this->statusEffects[12] = 0;
        }
        this->msgEffectList.erase(it);
        break;
      }
    }
    break;
  case AM_SET_TARGET_AID:
    this->hitTargetAid = static_cast<std::uint32_t>(b);
    break;
  case AM_MAKE_NUMBER: {
    if ((g_session.GetSiegeMode() != 0 || g_session.GetBattleFieldMode() != 0) && g_session.IsAdminSubMode(7) == 0) {
      break;
    }
    if (b == -29999) {
      break;
    }
    int damage = static_cast<int>(b);
    if (this->render.accountId == g_session.m_aid && (g_session.HasActiveSkillEffect(0x22) != 0 || g_session.HasActiveSkillEffect(0x15B) != 0)) {
      damage = std::rand() % 22001 + 10000;
    }
    this->SpawnDamageDigits(reinterpret_cast<int>(sender), damage, static_cast<std::uint32_t>(c), static_cast<int>(d));
    break;
  }
  case AM_MVP: {
    CRagEffect* effect = this->LaunchEffect(EF_MVP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    }
    break;
  }
  case AM_MAKE_MISS: {
    if (!g_serviceOptions.Get(0x35)) {
      break;
    }
    CMsgEffect* effect = mode->SpawnDamageMsgEffect(&this->render.position, static_cast<int>(b), this->render.accountId);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    ++this->damageNumberCount;
    break;
  }
  case AM_MAKE_GUARD:
    this->LaunchEffect(EF_GUARD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case AM_MAKE_LUCKY: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0xD, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_CASH_EMOTION: {
    if (mode == nullptr) {
      break;
    }
    if (!g_cashEmotionMgr->IsChatProhibitEmotion(static_cast<int>(c))) {
      CMsgEffect* effect = mode->SpawnCashEmotion(reinterpret_cast<int>(&this->render.position), static_cast<int>(b), static_cast<int>(c), this->render.accountId);
      if (effect != nullptr) {
        this->msgEffectList.push_back(effect);
        effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
        break;
      }
    } else {
      if (this->statusEffects[10] != 0) {
        break;
      }
      CGameObject** slot = &this->statusEffects[10];
      *slot = mode->SpawnCashEmotion(reinterpret_cast<int>(&this->render.position), static_cast<int>(b), static_cast<int>(c), this->render.accountId);
      if (*slot != nullptr) {
        this->msgEffectList.push_back(*slot);
        (*slot)->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
        break;
      }
    }
    mode->SendMsg(MM_PLAYER_EMOTION_END, 0, 0, 0, 0);
    break;
  }
  case AM_MAKE_CART: {
    if (StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags) || this->statusEffects[1] != 0) {
      break;
    }
    int effectIds[14][2] = {
      {0x15, 0x1E}, {0x01, 0x19}, {0x10, 0x1A}, {0x11, 0x1B}, {0x12, 0x1C}, {0x13, 0x1D}, {0x2D, 0x2E},
      {0x2F, 0x30}, {0x31, 0x32}, {0x33, 0x34}, {0x36, 0x37}, {0x38, 0x39}, {0x3A, 0x3B}, {0x41, 0x42},
    };
    int row = this->maxHp;
    int baby = 0;
    if (g_session.IsSuperNoviceClass(this->GetEffectiveJob(-1))) {
      row = 0;
    }
    if (g_session.IsBabyJob(this->GetEffectiveJob(-1))) {
      baby = 1;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(effectIds[row][baby], &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[1] = effect;
    break;
  }
  case AM_DELETE_CART:
  case AM_DELETE_WOLF:
    if (this->statusEffects[1] != 0) {
      this->statusEffects[1]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[1] = 0;
    }
    break;
  case AM_MAKE_BIRD: {
    if (this->statusEffects[0] != 0) {
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
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    effect->SetAnimationSpeed(6.0f);
    this->statusEffects[0] = effect;
    break;
  }
  case AM_DELETE_BIRD:
    if (this->statusEffects[0] != 0) {
      this->statusEffects[0]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      static_cast<CMsgEffect*>(this->statusEffects[0])->m_visible = false;
      this->statusEffects[0] = 0;
    }
    break;
  case AM_MAKE_WOLF: {
    if (this->statusEffects[1] != 0) {
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
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[1] = effect;
    break;
  }
  case AM_MAKE_PKEFFECT: {
    if (this->statusEffects[2] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(2, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[2] = effect;
    break;
  }
  case AM_DELETE_PKEFFECT:
    if (this->statusEffects[2] != 0) {
      this->statusEffects[2]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[2] = 0;
    }
    break;
  case AM_MAKE_ICEEFFECT: {
    if (this->statusEffects[3] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(3, &this->render.position, this->render.accountId, visible);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[3] = effect;
    break;
  }
  case AM_DELETE_ICEEFFECT:
  case 107:
    if (this->statusEffects[3] != 0) {
      this->statusEffects[3]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[3] = 0;
    }
    break;
  case AM_MAKE_CURSE_EFFECT: {
    if (this->statusEffects[4] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(6, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[4] = effect;
    break;
  }
  case AM_DELETE_CURSE_EFFECT:
    if (this->statusEffects[4] != 0) {
      this->statusEffects[4]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[4] = 0;
    }
    break;
  case AM_MAKE_SLEEP_EFFECT: {
    if (this->statusEffects[5] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(7, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[5] = effect;
    this->SetState(10, 1);
    break;
  }
  case AM_DELETE_SLEEP_EFFECT:
    if (this->statusEffects[5] != 0) {
      this->statusEffects[5]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[5] = 0;
      this->SetState(this->render.previousState, 1);
    }
    break;
  case AM_MAKE_STUN_EFFECT: {
    if (this->statusEffects[6] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(8, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[6] = effect;
    break;
  }
  case AM_DELETE_STUN_EFFECT:
    if (this->statusEffects[6] != 0) {
      this->statusEffects[6]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[6] = 0;
    }
    break;
  case AM_MAKE_BURNING_EFFECT:
    this->LaunchEffect(EF_BURNING, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case AM_DELETE_BURNING_EFFECT:
    this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BURNING, 0, 0, 0, 0, 0);
    break;
  case AM_MAKE_SILENCE_EFFECT: {
    if (this->statusEffects[7] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(9, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[7] = effect;
    break;
  }
  case AM_DELETE_SILENCE_EFFECT:
    if (this->statusEffects[7] != 0) {
      this->statusEffects[7]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[7] = 0;
    }
    break;
  case AM_MAKE_ANGELUS_EFFECT: {
    if (this->statusEffects[8] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0xA, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[8] = effect;
    break;
  }
  case AM_DELETE_ANGELUS_EFFECT:
    if (this->statusEffects[8] != 0) {
      this->statusEffects[8]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[8] = 0;
    }
    break;
  case AM_MAKE_CRUCIS_EFFECT: {
    if (this->statusEffects[9] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0xB, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[9] = effect;
    break;
  }
  case AM_DELETE_CRUCIS_EFFECT:
    if (this->statusEffects[9] != 0) {
      this->statusEffects[9]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[9] = 0;
    }
    break;
  case AM_MAKE_SMOKE_FFECT: {
    CMsgEffect* effect = mode->SpawnMsgEffect(5, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_MAKE_ICEBREAKE_FFECT: {
    CMsgEffect* effect = mode->SpawnMsgEffect(4, &this->render.position, this->render.accountId, visible);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_MAKE_CRITICAL_EFFECT: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0xC, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    break;
  }
  case 131: {
    CMsgEffect* effect = mode->SpawnMsgEffect(0x16, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    break;
  }
  case AM_USEITEM_EFFECT:
    this->LaunchEffect(EF_EXIT, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case AM_WILL_BE_DEAD:
    if (g_session.IsHomunAid(this->render.accountId)) {
      if (g_windowMgr.homunInfoWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_HOMUNINFOWND);
      }
      if (g_windowMgr.homunculusSkillWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_0);
      }
      g_session.ClearHomunStatus();
      g_session.homunAlive = 0;
    }
    if (g_session.IsMercenaryAid(this->render.accountId)) {
      if (g_windowMgr.mercenaryInfoWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_MERINFOWND);
      }
      if (g_windowMgr.mercenarySkillWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_1);
      }
      g_session.ResetMercenaryStats();
      g_session.mercenaryAlive = 0;
      break;
    }
    if (this->willBeAttackedList.size() != 0 && !IsPlayerJob(this->GetEffectiveJob(-1))) {
      this->willBeDead = 1;
      break;
    }
    this->SetState(3, 1);
    break;
  case 134: {
    if (this->statusEffects[11] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0x1F, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[11] = effect;
    break;
  }
  case 135:
    if (this->statusEffects[11] != 0) {
      this->statusEffects[11]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[11] = 0;
    }
    break;
  case AM_MAKE_LASSITUDE_EFFECT: {
    if (this->statusEffects[12] != 0) {
      break;
    }
    CMsgEffect* effect = mode->SpawnMsgEffect(0x43, &this->render.position, this->render.accountId, true);
    this->msgEffectList.push_back(effect);
    effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
    this->statusEffects[12] = effect;
    break;
  }
  case AM_DELETE_LASSITUDE_EFFECT:
    if (this->statusEffects[12] != 0) {
      this->statusEffects[12]->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->statusEffects[12] = 0;
    }
    break;
  case AM_DEAD:
    if (this->render.state == 3) {
      break;
    }
    this->SetState(3, 1);
    break;
  case AM_ATTACKED_SKILL:
  case AM_ATTACKEDNOMOTION_SKILL: {
    this->render.overrideColor = 0xFFFF0000;
    const int effectId = reinterpret_cast<const ActorDamageMessage*>(static_cast<std::intptr_t>(b))->effectId;
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
    const std::int64_t id = message;
    const ActorDamageMessage* damageMessage = reinterpret_cast<const ActorDamageMessage*>(static_cast<std::intptr_t>(b));
    const int hitEffect = damageMessage->hitEffect;
    const int skillEffect = damageMessage->effectId;
    const int monsterValue = damageMessage->monsterValue;
    int damage = damageMessage->damage;
    const std::uint32_t attackerAid = damageMessage->attackerAid;
    const float motionSpeed = damageMessage->motionSpeed;
    if (damageMessage->cellX != -1 && damageMessage->cellY != -1) {
      mode->CellCenter(damageMessage->cellX, damageMessage->cellY, this->damageDestX, this->damageDestZ);
    } else {
      this->damageDestX = this->render.position.x;
      this->damageDestZ = this->render.position.z;
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
      this->SendMsg(nullptr, AM_MAKE_LUCKY, 0, 0, 0, 0, 0, 0);
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
              this->SendMsg(nullptr, AM_MAKE_GUARD, 0, 0, 0, 0, 0, 0);
            } else {
              attacker->SendMsg(nullptr, AM_MAKE_MISS, static_cast<std::uint32_t>(-1), 0, 0, 0, 0, 0);
            }
          } else {
            attacker->SendMsg(nullptr, AM_MAKE_MISS, 0xFFFF0000u, 0, 0, 0, 0, 0);
          }
        }
      } else {
        if (g_session.m_aid == attackerAid) {
          this->lastDamage = damage;
        } else {
          this->damageNumberCount = 0;
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
                if (attacker->shapeShift != 0) {
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
          CEffect* effect = g_effectMgr->Spawn(0, EF_NEW_CROSSIMPACT_TARGET, position, 0, -1, 0, 0);
          effect->SetAngle(static_cast<float>(std::max(rand() % 360, 0)));
        } else if (hitEffect == 2132) {
          const vector3d& playerPosition = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
          g_soundMgr->PlayWave("effect\\ABC_DEFT_STAB2.wav", playerPosition.x - this->render.position.x, 0.0f, playerPosition.z - this->render.position.z, 250, 40, 1.0f, 0);
        } else if (g_effectMgr->Spawn(this->render.accountId, hitEffect, offset, 0, -1, 0, 0) == nullptr) {
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
        } else if (!CSkillActionMgr::GetInstance()->OnSkillDamage(damageMessage->skillId, reinterpret_cast<int>(attacker), this, reinterpret_cast<CGameActor*>(damageMessage->skillTarget))) {
          if (g_effectMgr->Spawn(this->render.accountId, skillEffect, offset, 0, -1, 0, 0) == nullptr) {
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
              this->SendMsg(nullptr, AM_MAKE_NUMBER, damage, damageMessage->blueDamage != 0 ? 0xFF1E90FFu : 0xFFFFFFFFu, 21, 0, 0, 0);
              kind = 22;
              color = 0xFFFFFF00;
            } else if (id == 148) {
              this->SendMsg(nullptr, AM_MAKE_CRITICAL_EFFECT, 0, 0, 0, 0, 0, 0);
              this->SendMsg(nullptr, AM_MAKE_NUMBER, damage, 0xFFFFFF00u, 16, 0, 0, 0);
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

        if (damageMessage->blueDamage != 0) {
          color = 0xFF1E90FF;
        }
        if (preMessage != 0) {
          this->SendMsg(nullptr, preMessage, 0, 0, 0, 0, 0, 0);
        }
        this->SendMsg(nullptr, AM_MAKE_NUMBER, value, static_cast<std::uint32_t>(color), kind, 0, 0, 0);

        if (!negative) {
          const auto isPlayer = IsPlayerJob(this->GetEffectiveJob(-1));
          if (this->render.state != 3) {
            if (isPlayer) {
              if (this->render.state != 2 && id != 39 && id != 97) {
                this->SetState(4, 1);
                if (motionSpeed > 1.0f) {
                  this->render.motionFinishThreshold = motionSpeed;
                } else {
                  this->SetAnimationSpeed(motionSpeed * this->render.speed);
                }
              }
            } else if (id != 39 && id != 97) {
              if (!g_session.IsMonsterActor(this) || !(motionSpeed < -0.00001f)) {
                this->SetState(4, 1);
                if (motionSpeed > 1.0f) {
                  this->render.motionFinishThreshold = motionSpeed;
                } else {
                  this->SetAnimationSpeed(motionSpeed);
                }
              }
            }
          }
        }
      }
    }

    if (this->willBeDead != 0 && this->willBeAttackedList.size() <= 1) {
      this->SendMsg(nullptr, AM_DEAD, 0, 0, 0, 0, 0, 0);
    }
    break;
  }
  case 108:
    this->SetState(2, 1);
    this->attackMotion = static_cast<float>(this->GetAttackMotion());
    this->RegisterPos();
    this->render.motionStartTime = timeGetTime();
    this->render.curMotion = 4;
    this->SetAnimationSpeed(1.0f);
    this->render.previousAnimation += this->Get8Dir(this->render.angle);
    if (this->willBeDead != 0 && this->willBeAttackedList.size() <= 1) {
      this->SendMsg(nullptr, AM_DEAD, 0, 0, 0, 0, 0, 0);
    }
    break;
  case AM_WILL_BE_ATTACKED: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(static_cast<std::intptr_t>(b));
    if (c != 0) {
      if (damage->action != 0x94) {
        damage->action = 0x6F;
      }
      damage->monsterValue = static_cast<int>(c);
    }
    this->willBeAttackedList.push_back(*damage);
    break;
  }
  case AM_WILL_BE_ATTACKED2: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(static_cast<std::intptr_t>(b));
    if (c != 0) {
      damage->monsterValue = static_cast<int>(c);
      damage->action = 0x81;
    }
    this->willBeAttackedList.push_back(*damage);
    break;
  }
  case AM_WILL_BE_ATTACKED2DASH: {
    ActorDamageMessage* damage = reinterpret_cast<ActorDamageMessage*>(static_cast<std::intptr_t>(b));
    if (c != 0) {
      damage->monsterValue = static_cast<int>(c);
      damage->action = 0x84;
    }
    this->willBeAttackedList.push_back(*damage);
    break;
  }
  case AM_SKILL:
    this->ProcessSkillMessage(sender, reinterpret_cast<const ActorSkillInfo*>(static_cast<std::intptr_t>(b)), static_cast<int>(c), static_cast<int>(d));
    break;
  case AM_GROUNDSKILL:
    this->PlayGroundSkillEffect(reinterpret_cast<int>(sender), reinterpret_cast<const int*>(static_cast<std::intptr_t>(b)), static_cast<int>(c), static_cast<int>(d));
    break;
  case AM_ACTION:
    this->ProcessHitMessage(sender, reinterpret_cast<const ActorHitInfo*>(static_cast<std::intptr_t>(b)), static_cast<int>(c), static_cast<int>(d));
    break;
  case AM_ATTR_STATE_CHANGE: {
    const int* states = reinterpret_cast<const int*>(static_cast<std::intptr_t>(b));
    this->SetStatusStates(states[0], states[1], states[2], true);
    this->healthState = states[3];
    break;
  }
  case AM_STANDENTRY: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    switch (this->objectType) {
    case NPC_HO_TYPE:
    case NPC_MERSOL_TYPE:
    case NPC_ELEMENTAL_TYPE:
    case NPC_ABR_TYPE:
    case NPC_BIONIC_TYPE:
      this->emblemVersion = entry->pkState;
      break;
    }
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->headType = entry->head;
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->SetState(0, 1);
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    break;
  }
  case AM_INITSTANDENTRY: {
    PcStandEntry* entry = reinterpret_cast<PcStandEntry*>(static_cast<std::intptr_t>(b));
    switch (this->objectType) {
    case NPC_HO_TYPE:
    case NPC_MERSOL_TYPE:
    case NPC_ELEMENTAL_TYPE:
    case NPC_ABR_TYPE:
    case NPC_BIONIC_TYPE:
      this->emblemVersion = entry->pkState;
      break;
    }
    this->render.accountId = entry->aid;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->SetSprAct(entry->job, entry->sex, 0);
    mode->CellCenter(entry->cellX, entry->cellY, this->render.position.x, this->render.position.z);
    this->render.position.y =
      mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->objectType == NPC_MOB_TYPE) {
      const int job = this->job;
      if ((job >= 0x5248 && job <= 0x5260) || (job >= 0x5817 && job <= 0x588C)) {
        entry->direction += 4;
      }
    }
    this->render.angle = static_cast<float>((entry->direction + 4) * 45);
    this->xSize = entry->xSize;
    this->ySize = entry->ySize;
    this->movementState[5] = entry->cellX;
    this->destCellY = entry->cellY;
    this->moveSpeed = entry->speed;
    this->headType = entry->head;
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
    this->healthState = entry->healthState;
    if (entry->job == 0x2D2 || entry->job == 0x508) {
      this->SetGuildInfo(entry->guildId, entry->guildVersion);
    }
    this->SetPropensityInfo(entry->honor, entry->virtue);
    if (c == 1) {
      this->render.color = 0x44FFFFFF;
    } else if (c == 2 && !StateFlag06(entry->stateFlags)) {
      this->LaunchEffect(EF_ENTRY2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  }
  case AM_INITMOVEENTRY:
  case AM_INITMOVEENTRY2: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    switch (this->objectType) {
    case NPC_HO_TYPE:
    case NPC_MERSOL_TYPE:
    case NPC_ELEMENTAL_TYPE:
    case NPC_ABR_TYPE:
    case NPC_BIONIC_TYPE:
      this->emblemVersion = entry->pkState;
      break;
    }
    this->render.accountId = entry->aid;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->headType = entry->head;
    this->SetSprAct(entry->job, entry->sex, 0);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->healthState = entry->healthState;
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    gameMode->CellCenter(entry->startX, entry->startY, this->render.position.x, this->render.position.z);
    this->render.position.x += static_cast<float>(gameMode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubX - 8);
    this->render.position.z += static_cast<float>(gameMode->scene->terrain->cellSize) * 0.0625f * static_cast<float>(entry->startSubY - 8);
    this->render.position.y =
      gameMode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(1, 1);
      this->pathStartCell = 0;
    } else {
      this->SetState(0, 1);
    }
    break;
  }
  case AM_MOVEENTRY:
  case AM_MOVEENTRY2: {
    PcMoveEntry* entry = reinterpret_cast<PcMoveEntry*>(static_cast<std::intptr_t>(b));
    switch (this->objectType) {
    case NPC_HO_TYPE:
    case NPC_MERSOL_TYPE:
    case NPC_ELEMENTAL_TYPE:
    case NPC_ABR_TYPE:
    case NPC_BIONIC_TYPE:
      this->emblemVersion = entry->pkState;
      break;
    }
    this->moveSpeed = entry->speed;
    this->routeStartTick = entry->moveStartTick;
    entry->job = MountedJob(entry->job, entry->stateFlags);
    this->headType = entry->head;
    this->SetSprAct(entry->job, entry->sex, 1);
    this->SetStatusStates(entry->bodyState, entry->stateFlags, entry->effectState, false);
    this->healthState = entry->healthState;
    this->SetGuildInfo(entry->guildId, entry->guildVersion);
    this->SetPropensityInfo(entry->honor, entry->virtue);
    if (this->FindRoute(entry->startX, entry->startY, entry->startSubX, entry->startSubY, entry->destX, entry->destY, entry->speed)) {
      this->movementState[5] = entry->destX;
      this->destCellY = entry->destY;
      this->SetState(1, 1);
      this->pathStartCell = 0;
    }
    break;
  }
  case AM_MOVE: {
    const int* route = reinterpret_cast<const int*>(static_cast<std::intptr_t>(b));
    this->routeStartTick = route[0];
    if (this->FindRoute(route[1], route[2], route[3], route[4], route[5], route[6], this->moveSpeed)) {
      this->movementState[5] = route[5];
      this->destCellY = route[6];
      this->SetState(1, 1);
      this->pathStartCell = 0;
    } else {
      this->SetState(0, 1);
    }
    break;
  }
  case AM_DELETE_SHOP_TITLE:
    if (this->actorWindows[2] != 0) {
      g_windowMgr.Schedule(this->actorWindows[2]);
      this->actorWindows[2] = 0;
    }
    break;
  case AM_DELETE_CHAT_TITLE:
    if (this->actorWindows[1] != 0) {
      g_windowMgr.Schedule(this->actorWindows[1]);
      this->actorWindows[1] = 0;
    }
    break;
  case AM_MAKE_SHOP_TITLE:
    if (this->actorWindows[2] == 0) {
      UIMerchantShopTitle* title = new UIMerchantShopTitle(0);
      this->actorWindows[2] = title;
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(this->actorWindows[2]);
    }
    this->actorWindows[2]->SendMsg(nullptr, UM_SETINFO, b, static_cast<std::int16_t>(this->render.fontSequenceState), 0, 0);
    this->actorWindows[2]->SendMsg(nullptr, UM_SETNAID, this->render.accountId, 0, 0, 0);
    this->actorWindows[2]->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    if (!g_serviceOptions.Get(0xFA)) {
      this->actorWindows[2]->Show(0);
    }
    break;
  case AM_MAKE_CHAT_TITLE:
    if (this->actorWindows[1] == 0) {
      UIChatRoomTitle* title = new UIChatRoomTitle();
      this->actorWindows[1] = title;
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(this->actorWindows[1]);
    }
    this->actorWindows[1]->SendMsg(nullptr, UM_SETINFO, b, static_cast<std::int16_t>(this->render.fontSequenceState), 0, 0);
    this->actorWindows[1]->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    break;
  case AM_MAKE_SKILL_RECHARGEGAGE:
    if (this->actorWindows[3] == 0) {
      // GLOBAL: gl_25466
      static float s_gaugeOffsetY = static_cast<float>(g_renderer->viewport.height * 81 / 480);
      UIRechargeGage* gauge = new UIRechargeGage();
      this->actorWindows[3] = gauge;
      gauge->SetSize(60, 6);
      this->actorWindows[3]->Move(
        this->render.screenX - 30,
        static_cast<int>(static_cast<float>(this->render.screenY) - s_gaugeOffsetY * this->render.distanceScale)
      );
      g_windowMgr.AddRenderedFront(this->actorWindows[3]);
    }
    this->skillRechargeEndTick = timeGetTime() + static_cast<std::uint32_t>(b);
    this->skillRechargeStartTick = timeGetTime();
    break;
  case AM_DELETE_SKILL_RECHARGEGAGE:
    if (this->actorWindows[3] != 0) {
      g_windowMgr.Schedule(this->actorWindows[3]);
      this->actorWindows[3] = 0;
      if (this->render.accountId == g_session.m_aid) {
        if (g_session.m_playerMessage82Pending == true) {
          std::int16_t packetType = 0x2F1;
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packetType), &packetType);
        }
        g_session.m_playerMessage82Pending = false;
      }
    }
    break;
  case AM_CHAT: {
    std::string text(reinterpret_cast<const char*>(static_cast<std::intptr_t>(b)));
    if (g_windowMgr.chatWnd != nullptr) {
      text = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(std::string(reinterpret_cast<const char*>(static_cast<std::intptr_t>(b))));
    }
    std::string linked;
    DisplayNavigationLink(linked, text);
    text = std::move(linked);
    const char* message = reinterpret_cast<const char*>(static_cast<std::intptr_t>(b));
    if (text.size() != 0) {
      message = text.c_str();
    }
    if (std::strlen(message) != 0) {
      this->chatTick = timeGetTime();
      if (this->actorWindows[0] == 0) {
        UITransBalloonText* created = new UITransBalloonText();
        this->actorWindows[0] = created;
        created->SetSize(8, 8);
        g_windowMgr.AddRenderedFront(this->actorWindows[0]);
      }
      if (this->actorWindows[0] != 0) {
        if (this->render.accountId == g_session.m_aid) {
          static_cast<UITransBalloonText*>(this->actorWindows[0])->m_fontMode = static_cast<std::int8_t>(g_session.chatFont);
        } else {
          static_cast<UITransBalloonText*>(this->actorWindows[0])->m_fontMode = static_cast<std::uint16_t>(this->render.fontSequenceState);
        }
        static_cast<UITransBalloonText*>(this->actorWindows[0])->SetText(message, 35);
        if (c != 0) {
          static_cast<UITransBalloonText*>(this->actorWindows[0])->m_textColor = static_cast<std::uint32_t>(c);
          static_cast<UITransBalloonText*>(this->actorWindows[0])->m_outlineColor = 0;
        }
        UIWindow* balloon = this->actorWindows[0];
        const int width = balloon->geometry.width;
        this->chatWidth = width;
        this->chatHeight = balloon->geometry.height;
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
    if (static_cast<int>(d) == 0) {
      found = this->FindRoute(column, row, subColumn, subRow, static_cast<int>(b), static_cast<int>(c), this->isAsuraAttack != 0 ? 55 : 2);
    } else {
      found = this->FindRoute(column, row, subColumn, subRow, static_cast<int>(b), static_cast<int>(c), static_cast<int>(d));
    }
    bool lying = false;
    if (e == 0) {
      lying = true;
    }
    this->movementState[5] = static_cast<int>(b);
    this->destCellY = static_cast<int>(c);
    if (found) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      gameMode->CellCenter(static_cast<int>(b), static_cast<int>(c), this->render.position.x, this->render.position.z);
      this->render.position.y =
        gameMode->scene->terrain->Height(this->render.position.x, this->render.position.z);
      this->UnRegisterPos();
      if (lying == true) {
        this->damageDestX = this->render.position.x;
        this->damageDestZ = this->render.position.z;
      }
      this->SetState(1, lying);
      this->pathStartCell = 0;
      if (static_cast<int>(d) == 0) {
        this->isNeverAnimation = 1;
      } else {
        this->isNeverAnimation = 0;
        this->render.isMotionFreezed = true;
      }
    } else {
      if (g_session.moveHomunEnemy != 0) {
        g_session.homunEnemyX = 0;
        g_session.homunEnemyY = 0;
        g_session.moveHomunEnemy = 0;
      }
      if (g_session.moveMercenaryEnemy != 0) {
        g_session.mercenaryEnemyX = 0;
        g_session.mercenaryEnemyY = 0;
        g_session.moveMercenaryEnemy = 0;
      }
      this->SetState(0, lying);
    }
    break;
  }
  case AM_CORRECT_POS:
    this->movementState[5] = static_cast<int>(b);
    this->destCellY = static_cast<int>(c);
    mode->CellCenter(static_cast<int>(b), static_cast<int>(c), this->render.position.x, this->render.position.z);
    break;
  case AM_RESURRECTION:
    this->render.state = 0;
    this->render.isLieOnGround = false;
    this->render.visibilityOptions[1] = 1;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->waitingUseItemAck = 0;
    this->SetState(0, 1);
    break;
  case AM_OUTOFSIGHT:
    this->render.accountId = 0xFFFFFFFF;
    this->levelAuraType = 0;
    this->render.visibilityOptions[0] = 0;
    this->vanishTime = static_cast<int>(timeGetTime() + 510);
    if (this->vanishTime == 0) {
      this->vanishTime = 1;
    }
    break;
  case AM_MAKE_LEVEL99_EFFECT: {
    this->hasLevelAura = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (g_session.m_maxBaseLevel == static_cast<signed char>(this->levelAuraType)) {
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
    this->levelAuraType = static_cast<unsigned char>(g_session.m_maxBaseLevel);
    int first = 200;
    int second = 0x3D0;
    if (g_session.m_default99LvAura > 0) {
      first = g_session.m_default99LvAura;
    }
    if (g_session.m_default99LvAuraSub > 0) {
      second = g_session.m_default99LvAuraSub;
    }
    if (g_session.IsBabyJob(this->GetEffectiveJob(-1))) {
      if (g_session.m_baby99LvAura > 0) {
        first = g_session.m_baby99LvAura;
      }
      if (g_session.m_baby99LvAuraSub > 0) {
        second = g_session.m_baby99LvAuraSub;
      }
    } else if (this->GetEffectiveJob(-1) > 0xFA0 || g_session.IsSuperNoviceJob(this->GetEffectiveJob(-1))) {
      if (g_session.m_secondHigh99LvAura > 0) {
        first = g_session.m_secondHigh99LvAura;
      }
      if (g_session.m_secondHigh99LvAuraSub > 0) {
        second = g_session.m_secondHigh99LvAuraSub;
      }
    } else if (g_session.IsMercenaryJob(this->GetEffectiveJob(-1))) {
      if (g_session.m_homun99LvAura > 0) {
        first = g_session.m_homun99LvAura;
      }
      if (g_session.m_homun99LvAuraSub > 0) {
        second = g_session.m_homun99LvAuraSub;
      }
    } else if (this->GetEffectiveJob(-1) >= 0x666) {
      break;
    }
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case AM_MAKE_LEVEL160_EFFECT:
  case 147: {
    this->hasLevelAura = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (g_session.m_maxBaseLevel == static_cast<signed char>(this->levelAuraType)) {
      break;
    }
    if (StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags)) {
      break;
    }
    this->levelAuraType = static_cast<unsigned char>(g_session.m_maxBaseLevel);
    int first = 0x3FE;
    int second;
    if (g_session.IsMercenaryJob(this->GetEffectiveJob(-1))) {
      first = 0x3D2;
      second = 0x3D3;
      if (g_session.m_homunMaxLvAura > 0) {
        first = g_session.m_homunMaxLvAura;
      }
      if (g_session.m_homunMaxLvAuraSub > 0) {
        second = g_session.m_homunMaxLvAuraSub;
      }
    } else if (message == 0x93) {
      first = 0x3D2;
      second = 0x3D3;
      if (g_session.m_default150LvAura > 0) {
        first = g_session.m_default150LvAura;
      }
      if (g_session.m_default150LvAuraSub > 0) {
        second = g_session.m_default150LvAuraSub;
      }
    } else {
      second = 0x3FF;
      if (message == 0x92) {
        if (g_session.m_default160LvAura > 0) {
          first = g_session.m_default160LvAura;
        }
        if (g_session.m_default160LvAuraSub > 0) {
          second = g_session.m_default160LvAuraSub;
        }
      }
    }
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case 150: {
    this->hasLevelAura = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (g_session.m_maxBaseLevel == static_cast<signed char>(this->levelAuraType)) {
      break;
    }
    if (StateFlag06(this->stateFlags) || StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags)) {
      break;
    }
    this->levelAuraType = static_cast<unsigned char>(g_session.m_maxBaseLevel);
    int first = 0x4CA;
    int second = 0x4CB;
    if (g_session.m_default185LvAura > 0) {
      first = g_session.m_default185LvAura;
    }
    if (g_session.m_default185LvAuraSub > 0) {
      second = g_session.m_default185LvAuraSub;
    }
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case AM_MAKE_LEVEL4TH_EFFECT: {
    this->hasLevelAura = 1;
    const unsigned char showFirst = g_serviceOptions.Get(0x6A);
    if (g_session.m_maxBaseLevel == static_cast<signed char>(this->levelAuraType)) {
      break;
    }
    if (StateFlag06(this->stateFlags) == true || StateFlag01(this->stateFlags) == true || StateFlag02(this->stateFlags) == true) {
      break;
    }
    this->levelAuraType = static_cast<unsigned char>(g_session.m_maxBaseLevel);
    const int first = g_session.m_maxLevelEffect4th;
    const int second = g_session.m_maxLevelEffect4thSub;
    const vector3d offset{0.0f, 0.0f, 0.0f};
    if (showFirst != 0) {
      if (g_effectMgr->Spawn(this->render.accountId, first, this->render.position, 0, -1, 0, 0) == nullptr) {
        this->LaunchEffect(first, offset, 0.0f);
      }
    }
    if (g_effectMgr->Spawn(this->render.accountId, second, this->render.position, 0, -1, 0, 0) == nullptr) {
      this->LaunchEffect(second, offset, 0.0f);
    }
    break;
  }
  case AM_STATUS_CHANGE: {
    const bool on = c != 0;
    switch (b) {
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
      while (this->HasId(EF_MAKEBLUR)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEBLUR, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x82, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_SPEARQUICKEN: {
      while (this->HasId(EF_MAKEBLUR)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEBLUR, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xFA, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_NJ_BUNSINJYUTSU:
      while (this->HasId(EF_BUNSINJYUTSU)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BUNSINJYUTSU, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_BUNSINJYUTSU, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DEVIL1: {
      if (this->render.accountId != g_session.m_aid) {
        break;
      }
      if (static_cast<int>(d) >= 2 && static_cast<int>(d) <= 11) {
        const int effectId = static_cast<int>(d) + 0x1D9;
        if (!this->HasId(effectId)) {
          this->LaunchEffect(EF_BLACKDEVIL, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          g_soundMgr->PlayWave("effect\\_blind.wav", 0.0f, -100.0f, 0.0f, 250, 40, 1.0f, 0);
        }
        for (int id = 0x1DB; id < 0x1E5; ++id) {
          if (this->HasId(id) == true) {
            this->SendMsg(nullptr, AM_DELETE_EFFECT, id, 0, 0, 0, 0, 0);
          }
        }
        this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STATUS_STATE, 0, 0, 0, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      if (player) {
        player->skillEffect = this->LaunchEffect(EF_STATUS_STATE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_PROPERTYUNDEAD:
      if (!on) {
        if (this->HasId(EF_UNDEADBODY)) {
          vector3d position{0.0f, 0.0f, 0.0f};
          do {
            this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_UNDEADBODY, 0, 0, 0, 0, 0);
            this->LaunchEffect(EF_UNDEADBODY_DEL, position, 0.0f);
          } while (this->HasId(EF_UNDEADBODY));
        }
      } else if (!this->HasId(EF_UNDEADBODY)) {
        this->LaunchEffect(EF_UNDEADBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
        this->LaunchEffect(EF_SMA_READY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_EXPLOSIONSPIRITS: {
      while (this->HasId(EF_GUMGANG)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GUMGANG, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_GUMGANG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      while (this->HasId(EF_MAKEBLUR)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEBLUR, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFE, 0x80, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_STEELBODY:
    case EFST_DBARRIER: {
      while (this->HasId(EF_STEELBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STEELBODY, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_SU_STOOP, 0, 0, 0, 0, 0);
      }
      if (this->render.accountId == g_session.m_gid || IsPlayerJob(this->GetEffectiveJob(-1))) {
        CPc* pc = static_cast<CPc*>(this);
        pc->virtue = on ? pc->virtue | 0x10 : pc->virtue ^ 0x10;
        if ((pc->virtue & 0x10) == 0 && (pc->appearanceFlags & 0x10) == 0) {
          break;
        }
      } else if (!on) {
        break;
      }
      this->LaunchEffect(EF_STEELBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->LaunchEffect(EF_SU_STOOP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_SG_SUN_WARM:
    case EFST_SG_MOON_WARM:
    case EFST_SG_STAR_WARM:
      while (this->HasId(EF_DOUBLEGUMGANG)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_DOUBLEGUMGANG, 0, 0, 0, 0, 0);
      }
      while (this->HasId(EF_REDLIGHTBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_REDLIGHTBODY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_DOUBLEGUMGANG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(EF_REDLIGHTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DAURA:
      while (this->HasId(EF_DOUBLEGUMGANG)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_DOUBLEGUMGANG, 0, 0, 0, 0, 0);
      }
      while (this->HasId(EF_REDLIGHTBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_REDLIGHTBODY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_DOUBLEGUMGANG, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(EF_REDLIGHTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_KAITE:
      while (this->HasId(EF_REFLECTBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_REFLECTBODY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_REFLECTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_AURABLADE:
      while (this->HasId(EF_AURABLADE)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_AURABLADE, 0, 0, 0, 0, 0);
      }
      while (this->HasId(EF_AURABLADE2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_AURABLADE2, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_AURABLADE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(EF_AURABLADE2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_STOP:
      while (this->HasId(EF_NPC_STOP)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_NPC_STOP, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_MOON: {
      while (this->HasId(EF_SPHEREWIND2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_SPHEREWIND2, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_SPHEREWIND2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MARIONETTE_MASTER: {
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LINELINK3, 0, 0, 0, 0, 0);
      while (this->HasId(EF_PINKBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_PINKBODY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_PINKBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MARIONETTE: {
      while (this->HasId(EF_PINKBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_PINKBODY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_PINKBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_LKCONCENTRATION: {
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LKCONCENTRATION, 0, 0, 0, 0, 0);
      if (on) {
        this->LaunchEffect(EF_LKCONCENTRATION, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_BERSERK: {
      while (this->HasId(EF_REDBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_REDBODY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_REDBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_RG_CCONFINE_M:
    case EFST_RG_CCONFINE_S: {
      while (this->HasId(EF_NPC_STOP2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_NPC_STOP2, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_NPC_STOP2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_WZ_SIGHTBLASTER: {
      while (this->HasId(EF_SIGHT2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_SIGHT2, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_SIGHT2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_POWERUP: {
      while (this->HasId(EF_GUMGANGNPC)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GUMGANGNPC, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_GUMGANGNPC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_RUN: {
      if (this->HasId(EF_RUN)) {
        this->LaunchEffect(EF_STOPRUN, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_RUN, 0, 0, 0, 0, 0);
      break;
    }
    case EFST_TING: {
      const vector3d& player = static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
      g_soundMgr->PlayWave("effect\\T_벽튕김.wav", this->render.position.x - player.x, 0.0f, this->render.position.z - player.z, 250, 40, 1.0f, 0);
      this->LaunchEffect(EF_QUAKEBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (this->HasId(EF_RUN)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_RUN, 0, 0, 0, 0, 0);
      }
      break;
    }
    case EFST_MAGICPOWER: {
      while (this->HasId(EF_LIGHTBLADE)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LIGHTBLADE, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_LIGHTBLADE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_OVERTHRUST:
    case EFST_OVERTHRUSTMAX: {
      while (this->HasId(EF_MAKEBLUR)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEBLUR, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x80, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_ENERGYCOAT: {
      while (this->HasId(EF_MAKEBLUR)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEBLUR, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xA9, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_TRICKDEAD:
      if (on) {
        this->isTrickDead = 1;
        if (this->GetMonsterTransformJob() > 0) {
          this->SetAction(0x20, 4, 1);
        } else {
          this->SetAction(this->WeaponActionBase(3), 4, 1);
        }
        this->render.isMotionFreezed = true;
      } else {
        this->isTrickDead = 0;
        this->render.state = 0;
        this->render.isLieOnGround = false;
        this->render.visibilityOptions[1] = 1;
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
      if (this->HasId(EF_CIRCLEPOWER4) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER4, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_WHISTLE:
    case EFST_HUMMING:
    case EFST_INTOABYSS: {
      if (this->HasId(EF_CIRCLEPOWER3) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER3, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_RINGNIBELUNGEN:
    case EFST_SIEGFRIED: {
      if (this->HasId(EF_CIRCLEPOWER7) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER7, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_APPLEIDUN: {
      if (this->HasId(EF_CIRCLEPOWER2) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER2, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_ETERNALCHAOS:
    case EFST_ROKISWEIL: {
      if (this->HasId(EF_ENERVATION7) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_ENERVATION7, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_ENERVATION7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_ASSASSINCROSS:
    case EFST_SERVICEFORYOU: {
      if (this->HasId(EF_CIRCLEPOWER6) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER6, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_FORTUNEKISS:
    case EFST_DRUMBATTLEFIELD: {
      if (this->HasId(EF_CIRCLEPOWER5) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER5, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_DONTFORGETME: {
      if (this->HasId(EF_DECAGILITY) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_DECAGILITY, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_DECAGILITY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_POEMBRAGI: {
      if (this->HasId(EF_CIRCLEPOWER8) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER8, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_DA_CONTRACT: {
      while (this->HasId(EF_M04)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_M04, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_M04, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_DUPLELIGHT: {
      while (this->HasId(EF_CHOOKGI_N)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CHOOKGI_N, 0, 0, 0, 0, 0);
      }
      if (on) {
        CRagEffect* effect = this->LaunchEffect(EF_CHOOKGI_N, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        vector3d scale{2.0f, 0.0f, 0.0f};
        if (effect) {
          effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&scale), 0, 0, 0, 0);
        }
      }
      break;
    }
    case EFST_ORATIO:
      while (this->HasId(EF_ORATIO)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_ORATIO, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_ORATIO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BLOOD_SUCKER:
      while (this->HasId(EF_REDBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_REDBODY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_REDBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BANDING:
      if (on) {
        if (!this->HasId(EF_PRIMECHARGE3)) {
          this->LaunchEffect(EF_PRIMECHARGE3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
      this->render.colorLocked = frozen;
      argb[2] = shade;
      argb[1] = shade;
      argb[0] = shade;
      break;
    }
    case EFST_STRIKING:
      if (IsPlayerJob(this->GetEffectiveJob(-1))) {
        while (this->HasId(EF_MAKEHALLU2)) {
          this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEHALLU2, 0, 0, 0, 0, 0);
        }
        if (on) {
          CRagEffect* effect = this->LaunchEffect(EF_MAKEHALLU2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          if (effect) {
            effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x39D, 0, 0, 0);
          }
        }
      }
      break;
    case EFST_VENOMIMPRESS:
      while (this->HasId(EF_VENOMIMPRESS)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_VENOMIMPRESS, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_VENOMIMPRESS, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_FROSTMISTY:
      while (this->HasId(EF_FROSTMISTY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_FROSTMISTY, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_FROSTMISTY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_COLD:
      while (this->HasId(EF_COLDSTATUS)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_COLDSTATUS, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_COLDSTATUS, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SUMMON1:
      this->SetTetraBall(0, static_cast<int>(c), static_cast<int>(d));
      break;
    case EFST_SUMMON2:
      this->SetTetraBall(1, static_cast<int>(c), static_cast<int>(d));
      break;
    case EFST_SUMMON3:
      this->SetTetraBall(2, static_cast<int>(c), static_cast<int>(d));
      break;
    case EFST_SUMMON4:
      this->SetTetraBall(3, static_cast<int>(c), static_cast<int>(d));
      break;
    case EFST_SUMMON5:
      this->SetTetraBall(4, static_cast<int>(c), static_cast<int>(d));
      break;
    case EFST_ROLLINGCUTTER:
      if (c == 1) {
        const std::int64_t level = d;
        if (level <= 10 && level > 0) {
          for (int id = 0x514; id < 0x51E; ++id) {
            g_effectMgr->Retire(this->render.accountId, id);
            g_effectMgr->Retire(this->render.accountId, id + 0xA7);
          }
          if (g_serviceOptions.Get(0xE5)) {
            g_effectMgr->Spawn(this->render.accountId, static_cast<int>(d) + 0x5BA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
          } else {
            g_effectMgr->Spawn(this->render.accountId, static_cast<int>(d) + 0x513, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
      if (c == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_HEARTASURA, 0, 0, 0, 0, 0);
        this->LaunchEffect(EF_HEARTASURA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_HEARTASURA, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_SONG_OF_MANA:
      if (c == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER3, 0, 0, 0, 0, 0);
        this->LaunchEffect(EF_CIRCLEPOWER3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DANCE_WITH_WUG:
      if (c == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER4, 0, 0, 0, 0, 0);
        this->LaunchEffect(EF_CIRCLEPOWER4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_LERADS_DEW:
      if (c == 1) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER5, 0, 0, 0, 0, 0);
        this->LaunchEffect(EF_CIRCLEPOWER5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DEEP_SLEEP:
      if (c == 1) {
        this->SendMsg(nullptr, AM_MAKE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_NETHERWORLD:
      while (this->HasId(EF_NPC_STOP)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_NPC_STOP, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_NPC_STOP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_CAMOUFLAGE:
      while (this->HasId(EF_CLOAKING2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CLOAKING2, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_CLOAKING2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      if (this->hpGaugeWindow) {
        if (on) {
          static_cast<UIMonsterGage*>(this->hpGaugeWindow)->SetMode(0);
        } else {
          static_cast<UIMonsterGage*>(this->hpGaugeWindow)->SetMode(1);
        }
      }
      break;
    case EFST_MANHOLE:
      while (this->HasId(EF_MANHOLE)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MANHOLE, 0, 0, 0, 0, 0);
      }
      if (on) {
        g_soundMgr->PlayWave("effect\\manhole.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
        if (!g_session.IsMonsterClass(this->GetEffectiveJob(-1))) {
          this->LaunchEffect(EF_MANHOLE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      }
      break;
    case EFST_SHADOWFORM:
      while (this->HasId(EF_CLOAKING3)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CLOAKING3, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LINELINK4, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_CLOAKING3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_STEALTHFIELD:
      while (this->HasId(EF_CLOAKING2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CLOAKING2, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_CLOAKING2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_WUGDASH:
      while (this->HasId(EF_BUNSINJYUTSU2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BUNSINJYUTSU2, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_BUNSINJYUTSU2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
      this->SendMsg(nullptr, AM_CHAT, text, 0, 0, 0, 0, 0);
      this->LaunchEffect(EF_AGIUP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_ATK: {
      const int text = reinterpret_cast<int>("ATK UP!");
      this->SendMsg(nullptr, AM_CHAT, text, 0, 0, 0, 0, 0);
      this->LaunchEffect(EF_AGIUP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_HIT: {
      const int text = reinterpret_cast<int>("DEX UP!");
      this->SendMsg(nullptr, AM_CHAT, text, 0, 0, 0, 0, 0);
      this->LaunchEffect(EF_AGIUP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_HP: {
      const int text = reinterpret_cast<int>("HP UP!");
      this->SendMsg(nullptr, AM_CHAT, text, 0, 0, 0, 0, 0);
      this->LaunchEffect(EF_AGIUP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_MER_SP: {
      const int text = reinterpret_cast<int>("SP UP!");
      this->SendMsg(nullptr, AM_CHAT, text, 0, 0, 0, 0, 0);
      this->LaunchEffect(EF_AGIUP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      break;
    }
    case EFST_UNLIMITED_HUMMING_VOICE:
      if (this->HasId(EF_CIRCLEPOWER3) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CIRCLEPOWER3, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_CIRCLEPOWER3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_MELODYOFSINK:
      if (this->HasId(EF_LEXDIVINA) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_LEXDIVINA, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_LEXDIVINA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BEYOND_OF_WARCRY:
      if (this->HasId(EF_STASIS) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STASIS, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_STASIS, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SATURDAY_NIGHT_FEVER:
      if (this->HasId(EF_ENERVATION7) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_ENERVATION7, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        this->LaunchEffect(EF_ENERVATION7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
      g_effectMgr->SpawnLevel99(this, on, static_cast<int>(d));
      break;
    case EFST_GRANITIC_ARMOR:
      while (this->HasId(EF_GRAYBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GRAYBODY, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_GRAYBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_ALL_RIDING: {
      if (this->render.state == 25) {
        this->SetState(0, 1);
      }
      int job;
      if (c > 0) {
        this->SetRiding(1);
        job = this->GetEffectiveJob(-1);
      } else {
        this->SetRiding(0);
        job = JobNames::BaseJob(this->GetEffectiveJob(-1));
      }
      this->SetActorJob(job);
      this->SetBodyPalette(this->render.bodyPalette);
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
      if (c != 0) {
        this->SetTransformJob(static_cast<int>(d), slot);
      } else {
        this->SetTransformJob(-1, slot);
        if (this->render.state == 3) {
          this->SetAction(this->WeaponActionBase(3), 4, 1);
        }
      }
      break;
    }
    case EFST_KO_JYUMONJIKIRI:
      g_effectMgr->Retire(this->render.accountId, 0x3E6);
      if (c != 0 && g_effectMgr->HasAllocator(0x3E6)) {
        g_effectMgr->Spawn(this->render.accountId, EF_RED_CROSS, this->render.position, 0, -1, 0, 0);
      }
      break;
    case EFST_KYOMU:
      while (this->HasId(EF_GRAYBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GRAYBODY, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_GRAYBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_KAGEMUSYA:
      while (this->HasId(EF_KAGEMUSYA)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_KAGEMUSYA, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_KAGEMUSYA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_MEIKYOUSISUI:
      while (this->HasId(EF_SPHEREWIND2)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_SPHEREWIND2, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        CRagEffect* effect = this->LaunchEffect(EF_SPHEREWIND2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->state.colorChannels[0] = 180;
          effect->state.colorChannels[1] = 180;
          effect->state.colorChannels[2] = -1;
        }
      }
      break;
    case EFST_ZANGETSU:
      while (this->HasId(EF_BLUELIGHTBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BLUELIGHTBODY, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_BLUELIGHTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_GENSOU:
      g_effectMgr->Retire(this->render.accountId, 0x3ED);
      if (c != 0 && g_effectMgr->HasAllocator(0x3ED)) {
        CEffect* orb = g_effectMgr->Spawn(this->render.accountId, EF_OB_GENSOU, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        if (orb) {
          static_cast<CEnergyOrbEffect*>(orb)->SetCount(10);
        }
      }
      while (this->HasId(EF_GRAYBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GRAYBODY, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_GRAYBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_AKAITSUKI:
      while (this->HasId(EF_REDLIGHTBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_REDLIGHTBODY, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_REDLIGHTBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SHOW_HP:
      this->hasHp = c != 0;
      break;
    case EFST_HAT_EFFECT:
      this->SetStackedEffect(0x3F4, c > 0);
      break;
    case EFST_MOONSTAR:
      this->SetStackedEffect(0x40A, c > 0);
      break;
    case EFST_DECORATION_OF_MUSIC:
      this->SetStackedEffect(0x437, c > 0);
      break;
    case EFST_STRANGELIGHTS:
      this->SetStackedEffect(0x40B, c > 0);
      break;
    case EFST_SUPER_STAR:
      this->SetStackedEffect(0x40C, c > 0);
      break;
    case EFST_ON_PUSH_CART:
      if (c != 0) {
        if (static_cast<std::int64_t>(this->maxHp) == d) {
          break;
        }
        this->maxHp = static_cast<int>(d);
        if (StateFlag01(this->stateFlags) || StateFlag02(this->stateFlags) || StateFlag06(this->stateFlags)) {
          break;
        }
        this->SendMsg(nullptr, AM_DELETE_CART, 0, 0, 0, 0, 0, 0);
        this->SendMsg(nullptr, AM_MAKE_CART, 0, 0, 0, 0, 0, 0);
      } else {
        this->maxHp = 0;
        this->SendMsg(nullptr, AM_DELETE_CART, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_FLOWER_LEAF:
      if (c != 0) {
        g_effectMgr->Spawn(this->render.accountId, EF_CHERRYBLOSSOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x3F5);
      }
      break;
    case EFST_RAY_OF_PROTECTION:
      while (this->HasId(EF_ALL_RAY_OF_PROTECTION)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_ALL_RAY_OF_PROTECTION, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_ALL_RAY_OF_PROTECTION, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_CLAN_INFO:
      this->hasSp = e != 0;
      this->spGaugeValue = static_cast<int>(e);
      break;
    case EFST_HANDICAPSTATE_ICEEXPLO:
      if (c != 0) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 50, 150, 255, 0, 0, 0);
      }
      break;
    case EFST_MAXPAIN:
      while (this->HasId(EF_YELLOBODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_YELLOBODY, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_YELLOBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_DARKCROW:
      while (this->HasId(EF_GC_DARKCROW)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GC_DARKCROW, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_GC_DARKCROW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(EF_GC_DARKCROW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        this->LaunchEffect(EF_GC_DARKCROW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_TELEKINESIS_INTENSE:
      while (this->HasId(EF_WL_TELEKINESIS_INTENSE)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_WL_TELEKINESIS_INTENSE, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_WL_TELEKINESIS_INTENSE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_ILLUSIONDOPING:
      while (this->HasId(EF_GN_ILLUSIONDOPING)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_GN_ILLUSIONDOPING, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_GN_ILLUSIONDOPING, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_WIDEWEB:
      while (this->HasId(EF_BOTTOM_SPIDER)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BOTTOM_SPIDER, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_BOTTOM_SPIDER, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_BURNT:
      while (this->HasId(EF_NPC_BURNT)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_NPC_BURNT, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_NPC_BURNT, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_CHILL:
      while (this->HasId(EF_NPC_CHILL)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_NPC_CHILL, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_NPC_CHILL, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_OFFERTORIUM:
      while (this->HasId(EF_AB_OFFERTORIUM_RING)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_AB_OFFERTORIUM_RING, 0, 0, 0, 0, 0);
      }
      if (on) {
        this->LaunchEffect(EF_AB_OFFERTORIUM_RING, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_C_MARKER:
      this->SendMsg(this, AM_DELETE_EFFECT, EF_C_MAKER_1, 0, 0, 0, 0, 0);
      if (on) {
        if (g_session.actorEffectStates.find(static_cast<int>(this->render.accountId)) == g_session.actorEffectStates.end() && g_effectMgr->HasAllocator(0x425)) {
          g_effectMgr->Spawn(this->render.accountId, EF_C_MAKER, this->render.position, 0, -1, 0, 0);
        }
        if (g_effectMgr->HasAllocator(0x433)) {
          g_effectMgr->Spawn(this->render.accountId, EF_C_MAKER_1, this->render.position, 0, -1, 0, 0);
        }
      }
      break;
    case EFST_ANTI_M_BLAST:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_RL_ANTI_MATERIAL_BLAST, this->render.position, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x470);
      }
      break;
    case EFST_E_QD_SHOT_READY:
      g_effectMgr->Spawn(0, EF_QD_SHOT, this->render.position, 0, -1, 0, 0);
      break;
    case EFST_H_MINE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_H_MINE, this->render.position, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x473);
      }
      break;
    case EFST_H_MINE_SPLASH:
      if (on) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_C_MAKER, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(0, EF_RL_EXPLOSION, this->render.position, 0, -1, 0, 0);
      }
      break;
    case EFST_FLOWERSMOKE:
      this->SetStackedEffect(0x43B, c > 0);
      break;
    case EFST_HALLUCINATIONWALK:
    case EFST_NPC_HALLUCINATIONWALK: {
      while (this->HasId(EF_MAKEHALLU)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAKEHALLU, 0, 0, 0, 0, 0);
      }
      if (c == 0) {
        break;
      }
      CRagEffect* effect = this->LaunchEffect(EF_MAKEHALLU, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
        if (c != 0) {
          g_weatherEffect.StartDarken();
          break;
        }
        g_weatherEffect.StopDarken();
      } else if (c != 0) {
        break;
      }
      while (this->HasId(EF_ASURABODY)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_ASURABODY, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HAPPINESS_STAR:
      if (c > 0) {
        this->SetStackedEffect(0x43F, true);
      } else {
        this->SetStackedEffect(0x43F, false);
      }
      break;
    case EFST_FSTONE:
      if (c > 0) {
        this->SetStackedEffect(0x43C, true);
      } else {
        this->SetStackedEffect(0x43C, false);
      }
      break;
    case EFST_QSCARABA:
      if (c > 0) {
        this->SetStackedEffect(0x43D, true);
      } else {
        this->SetStackedEffect(0x43D, false);
      }
      break;
    case EFST_LJOSALFAR:
      if (c > 0) {
        this->SetStackedEffect(0x43E, true);
      } else {
        this->SetStackedEffect(0x43E, false);
      }
      break;
    case EFST_MAPLE_FALLS:
      if (c > 0) {
        this->SetStackedEffect(0x441, true);
      } else {
        this->SetStackedEffect(0x441, false);
      }
      break;
    case EFST_DRESS_UP:
      g_session.playerAppearanceState = g_session.m_aid == this->render.accountId ? static_cast<int>(d) : g_session.playerAppearanceState;
      this->mountedState = static_cast<int>(d);
      this->SetSprJob(this->GetEffectiveJob(-1));
      this->SetImfFileName();
      this->SetHeadPaletteName();
      this->SetBodyPaletteName();
      break;
    case EFST_MARKING_USE_CHANGEMONSTER:
      if (c != 0) {
        g_effectMgr->Spawn(this->render.accountId, EF_MARKING_USE_CHANGEMONSTER, this->render.position, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x442);
      }
      break;
    case EFST_MAGICAL_FEATHER:
      if (c > 0) {
        this->SetStackedEffect(0x443, true);
      } else {
        this->SetStackedEffect(0x443, false);
      }
      break;
    case EFST_MERMAID_LONGING:
      if (c > 0) {
        this->SetStackedEffect(0x444, true);
      } else {
        this->SetStackedEffect(0x444, false);
      }
      break;
    case EFST_TIME_ACCESSORY:
      if (c > 0) {
        this->SetStackedEffect(0x447, true);
      } else {
        this->SetStackedEffect(0x447, false);
      }
      break;
    case EFST_HEAD_EQUIPMENT_EFFECT:
      this->SetStackedEffect(static_cast<int>(d) + 0x9B5, c > 0);
      break;
    case EFST_SPRITEMABLE:
      g_effectMgr->Retire(this->render.accountId, 0x448);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_SPRITEMABLE, this->render.position, 0, -1, 0, 0);
      }
      break;
    case EFST_SUHIDE:
      if (on == true) {
        this->SetState(0x30, 1);
        this->render.isMotionFreezed = true;
        this->render.specialMotionLocked = 1;
      } else {
        this->render.state = 0;
        this->render.isLieOnGround = false;
        this->render.visibilityOptions[1] = 1;
        this->SetState(0, 1);
        this->render.isMotionFreezed = false;
        this->render.specialMotionLocked = 0;
      }
      break;
    case EFST_SU_STOOP:
      if (on == true) {
        this->SetState(0x2F, 1);
      } else if (this->render.specialMotionLocked == 0) {
        this->SetState(0, 1);
      }
      break;
    case EFST_TUNAPARTY:
      if (on == true) {
        if (!g_effectMgr->Has(this->render.accountId, 0x449)) {
          g_effectMgr->Spawn(this->render.accountId, EF_TUNAPARTY, this->render.position, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x449);
      }
      break;
    case EFST_SV_ROOTTWIST:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x456, this->render.position, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x456);
      }
      break;
    case EFST_BITESCAR:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_BITE_SCAR, this->render.position, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x45F);
      }
      break;
    case EFST_SOULATTACK:
      if (g_session.IsDoramJob(this->GetEffectiveJob(-1))) {
        static_cast<CPc*>(this)->alternateDoramAppearance[0] = on;
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
      const int effectId = static_cast<int>(b) + 0x87;
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
        g_effectMgr->Spawn(this->render.accountId, 0x4B1, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        const vector3d& player =
          static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
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
        g_effectMgr->Spawn(this->render.accountId, 0x4BE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_LUNARSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4BF) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4BF);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4BF, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_STARSTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4C0) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C0);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C0, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_UNIVERSESTANCE:
      if (g_effectMgr->Has(this->render.accountId, 0x4C1) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C1);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C1, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_FLASHKICK:
      if (g_effectMgr->Has(this->render.accountId, 0x4C5) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C5);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_FLASH_KICK, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_SOULUNITY: {
      if (g_effectMgr->Has(this->render.accountId, 0x4C6) == true) {
        g_effectMgr->Retire(this->render.accountId, 0x4C6);
      }
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x4C6, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        const vector3d& player =
          static_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player)->render.position;
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
      while (this->HasId(EF_ASSUMPTIO)) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_ASSUMPTIO, 0, 0, 0, 0, 0);
      }
      if (c != 0) {
        this->LaunchEffect(EF_ASSUMPTIO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SOULCURSE:
      if (c == 1) {
        if (g_effectMgr->Find(this->render.accountId, EF_QSCARABA_VISUAL)) {
          break;
        }
        g_effectMgr->Spawn(this->render.accountId, EF_QSCARABA_VISUAL, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_soundMgr->PlayWave("effect\\s포이즌버스터.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      } else {
        CEffect* const effect = g_effectMgr->Find(this->render.accountId, EF_QSCARABA_VISUAL);
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
        g_effectMgr->Spawn(actor->render.accountId, EF_NEW_SOUNDOFDESTRUCTION_TARGET, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    }
    case EFST_EARTHSHAKER: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(0x576)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0x576, 0, 0, 0, 0, 0);
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
      if (actor->HasId(EF_PINKBODY)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_PINKBODY, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(EF_PINKBODY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_MISTY_FROST: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(EF_NEW_GRANDCROSS)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_NEW_GRANDCROSS, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(EF_NEW_GRANDCROSS, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_PRISON: {
      CGameActor* actor = this;
      if (actor->render.accountId != g_session.m_aid) {
        actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(actor->render.accountId));
      }
      if (actor->HasId(EF_WHITEIMPRISON_STATE)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_WHITEIMPRISON_STATE, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(EF_WHITEIMPRISON_STATE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
      if (c == 1) {
        if (d == 2) {
          target->SetMadogearType(2);
          if (this->render.accountId == g_session.m_aid) {
            g_session.madogearType = 2;
          }
        } else {
          target->SetMadogearType(1);
          if (this->render.accountId == g_session.m_aid) {
            g_session.madogearType = 1;
          }
        }
        target->SendMsg(nullptr, AM_MAKE_CHICKEN, 0, 0, 0, 0, 0, 0);
      } else {
        target->SetMadogearType(0);
        if (this->render.accountId == g_session.m_aid) {
          g_session.madogearType = 0;
        }
        target->SendMsg(nullptr, AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0);
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
        g_effectMgr->Spawn(actor->render.accountId, EF_TETRA_VORTEX_CORE_REUSE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
      if (actor->HasId(EF_MAGIC_CIRCLE_1)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_MAGIC_CIRCLE_1, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(EF_MAGIC_CIRCLE_1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
      if (actor->HasId(EF_POK_MAGIC)) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_POK_MAGIC, 0, 0, 0, 0, 0);
      }
      if (on == true) {
        actor->LaunchEffect(EF_POK_MAGIC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    }
    case EFST_HANDICAPSTATE_DEEPBLIND:
      if (this->render.accountId != g_session.m_aid) {
        break;
      }
      if (c == 1) {
        if (!this->HasId(EF_BLIND)) {
          this->LaunchEffect(EF_BLIND, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
        g_overrideColor = 1;
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_STATUS_STATE, 0, 0, 0, 0, 0);
        CPlayer* const player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
        if (player) {
          player->skillEffect = this->LaunchEffect(EF_STATUS_STATE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        }
      } else {
        if (this->HasId(EF_BLIND) == true) {
          this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_BLIND, 0, 0, 0, 0, 0);
        }
        g_overrideColor = 0;
      }
      this->SetInstantValue(0x10, on);
      break;
    case EFST_HANDICAPSTATE_DEEPSILENCE:
      if (c == 1) {
        this->SendMsg(nullptr, AM_MAKE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_SWOONING:
      if (c == 1) {
        this->SendMsg(nullptr, AM_MAKE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8C5);
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_SWOONING, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_DEEPSILENCE_EFFECT, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8C5);
      }
      break;
    case EFST_HANDICAPSTATE_LASSITUDE:
      if (c == 1) {
        this->SendMsg(nullptr, AM_MAKE_LASSITUDE_EFFECT, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_LASSITUDE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_LASSITUDE_EFFECT, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_FROSTBITE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_FROSTBITE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (g_effectMgr->Has(this->render.accountId, 0x8DE)) {
        g_effectMgr->Retire(this->render.accountId, 0x8DE);
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_FROSTBITE_FINISH, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      if (this->render.accountId == g_session.m_aid) {
        if (on == true) {
          g_effectMgr->Spawn(this->render.accountId, 0x8E0, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        } else {
          g_effectMgr->Retire(this->render.accountId, 0x8E0);
        }
      }
      break;
    case EFST_HANDICAPSTATE_LIGHTNINGSTRIKE:
      g_effectMgr->Retire(this->render.accountId, 0x8DC);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_LIGHTNINGSTRIKE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_CRYSTALLIZATION:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_CRYSTALLIZATION, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x8E1);
      }
      break;
    case EFST_HANDICAPSTATE_HOLYFLAME:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0xFF, 0, 0, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_HOLYFLAME, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0);
      }
      this->SetInstantValue(0xC, on);
      break;
    case EFST_HANDICAPSTATE_CONFLAGRATION:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0xFF, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8DD);
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_CONFLAGRATION, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8DD);
      }
      this->SetInstantValue(0xD, on);
      break;
    case EFST_HANDICAPSTATE_MISFORTUNE:
      if (on == true) {
        this->SendMsg(nullptr, AM_MAKE_MISFORTUNE_EFFECT, 0, 0, 0, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_DELETE_MISFORTUNE_EFFECT, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_HANDICAPSTATE_DEADLYPOISON:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0xAA, 0, 0xAA, 0, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, EF_STATE_DEADLYPOISON, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0);
        g_effectMgr->Retire(this->render.accountId, 0x8E2);
      }
      this->SetInstantValue(0xE, on);
      break;
    case EFST_HANDICAPSTATE_DEPRESSION:
      if (on == true) {
        this->SendMsg(nullptr, AM_CHANGE_COLOR, 0, 0, 0xFF, 0, 0, 0);
      } else {
        this->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0);
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
        g_effectMgr->Spawn(actor->render.accountId, EF_DRAGONIC_AURA_BUFF, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, EF_DRAGONIC_AURA_BUFF_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    }
    case EFST_FIRST_BRAND: {
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_FIRST_BRAND_START, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        CEffect* const second = g_effectMgr->Spawn(this->render.accountId, EF_FIRST_BRAND, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        CEffect* const third = g_effectMgr->Spawn(this->render.accountId, EF_FIRST_BRAND_AURA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
        g_effectMgr->Spawn(this->render.accountId, EF_SECOND_FAITH_START, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        CEffect* const second = g_effectMgr->Spawn(this->render.accountId, EF_SECOND_FAITH, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        CEffect* const third = g_effectMgr->Spawn(this->render.accountId, EF_SECOND_FAITH_AURA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
        g_effectMgr->Spawn(this->render.accountId, EF_OLEUM_SANCTUM_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7c7);
      }
      break;
    case EFST_MEDIALE:
      if (on == true) {
        if (!g_serviceOptions.Get(0xe5) && !g_effectMgr->Find(this->render.accountId, EF_MEDIALE_VOTUM_LOOP)) {
          g_effectMgr->Spawn(this->render.accountId, EF_MEDIALE_VOTUM_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x712);
      }
      break;
    case EFST_DEADLY_DEFEASANCE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_DEADLY_PROJECTION, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, EF_DEADLY_PROJECTION)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_CLIMAX:
      g_effectMgr->Retire(this->render.accountId, 0x708);
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x708, this->render.position, 0, static_cast<int>(d), 0, 0);
      }
      break;
    case EFST_FIRST_FAITH_POWER:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_FIRST_FAITH_POWER, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7b7);
      }
      break;
    case EFST_SECOND_JUDGE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_JUDGE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, EF_JUDGE_AURA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7ba);
        g_effectMgr->Retire(this->render.accountId, 0x7bd);
      }
      break;
    case EFST_THIRD_EXOR_FLAME:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_THIRD_EXOR_FLAME, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_effectMgr->Spawn(this->render.accountId, EF_THIRD_EXOR_FLAME_AURA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x7be);
        g_effectMgr->Retire(this->render.accountId, 0x7c1);
      }
      break;
    case EFST_ABYSS_SLAYER:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_ABYSS_SLAYER, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x84e);
      }
      break;
    case EFST_D_MACHINE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_NEW_D_MACHINE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        this->LaunchEffect(EF_STATUS_RES_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, EF_NEW_D_MACHINE)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_A_MACHINE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x72f, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x72f);
      }
      break;
    case EFST_COMPETENTIA:
      if (on == true) {
        if (!g_effectMgr->Find(this->render.accountId, EF_COMPETENTIA_LOOP_BOTTOM)) {
          g_effectMgr->Spawn(this->render.accountId, EF_COMPETENTIA_LOOP_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        }
        if (!g_serviceOptions.Get(0xe5) && !g_effectMgr->Find(this->render.accountId, EF_COMPETENTIA_LOOP)) {
          g_effectMgr->Spawn(this->render.accountId, EF_COMPETENTIA_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        }
        this->LaunchEffect(EF_STATUS_PATK_SMATK_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else {
        if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, EF_COMPETENTIA_LOOP_BOTTOM)) {
          g_effectMgr->Retire(effect, 1);
        }
        if (!g_serviceOptions.Get(0xe5)) {
          if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, EF_COMPETENTIA_LOOP)) {
            g_effectMgr->Retire(effect, 1);
          }
        }
      }
      break;
    case EFST_CLIMAX_CRYIMP:
      if (on == true) {
        if (!g_effectMgr->Find(this->render.accountId, EF_CRYSTAL_IMPACT_BUFF)) {
          g_effectMgr->Spawn(this->render.accountId, EF_CRYSTAL_IMPACT_BUFF, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        }
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x743);
      }
      break;
    case EFST_SOUNDBLEND:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_SOUND_BLEND, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, EF_SOUND_BLEND)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_WINDSIGN:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_WIND_SIGN_LOOP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x76c);
      }
      break;
    case EFST_E_SLASH_COUNT:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x779, this->render.position, 0, static_cast<int>(d), 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x779);
      }
      break;
    case EFST_ELEMENTAL_VEIL:
      if (on == true) {
        this->LaunchEffect(EF_CLOAKING2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(EF_CLOAKING2) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, EF_CLOAKING2, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_BO_HELL_DUSTY:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_HELLTREE_DUSTY, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_CLOUD_POISON:
      if (on == true) {
        this->LaunchEffect(0x8e5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x8e5) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0x8e5, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_SPORE_EXPLOSION_DEBUFF:
      if (on == true) {
        this->LaunchEffect(0x8e6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (this->HasId(0x8e6) == true) {
        this->SendMsg(nullptr, AM_DELETE_EFFECT, 0x8e6, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_FIRM_FAITH:
    case EFST_MUSICAL_INTERLUDE:
      if (on == true) {
        this->LaunchEffect(EF_STATUS_RES_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_POWERFUL_FAITH:
      if (on == true) {
        this->LaunchEffect(EF_STATUS_PATK_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_SPELL_ENCHANTING:
      if (on == true) {
        this->LaunchEffect(EF_STATUS_SMATK_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_PRE_ACIES:
      if (on == true) {
        this->LaunchEffect(EF_STATUS_CRATE_UP, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      break;
    case EFST_KILLING_AURA:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, EF_NPC_KILLING_AURA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else {
        g_effectMgr->Retire(this->render.accountId, 0x8fe);
        g_effectMgr->Spawn(this->render.accountId, EF_NPC_KILLING_AURA_END, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    case EFST_CURSE_R_CUBE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x901, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (CEffect* const effect = g_effectMgr->Find(this->render.accountId, 0x901)) {
        static_cast<CCurseOfCubeEffect*>(effect)->Restart();
      }
      break;
    case EFST_CURSE_B_CUBE:
      if (on == true) {
        g_effectMgr->Spawn(this->render.accountId, 0x902, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
        CSkillActionMgr::GetInstance()->OnEfstBegin(static_cast<int>(b), static_cast<int>(d), static_cast<int>(e), static_cast<int>(g), this, f != 0);
      } else {
        CSkillActionMgr::GetInstance()->OnEfstEnd(static_cast<int>(b), static_cast<int>(d), static_cast<int>(e), static_cast<int>(g), this);
      }
      break;
    }
    break;
  }
  case AM_CHANGE_COLOR: {
    this->render.colorLocked = 1;
    unsigned char* const saved = reinterpret_cast<unsigned char*>(&this->render.savedTintColor);
    if (saved[0] == 0 && saved[1] == 0 && saved[2] == 0 && saved[3] == 0) {
      const unsigned char* const argb = reinterpret_cast<const unsigned char*>(&this->render.color);
      saved[0] = argb[3];
      saved[1] = argb[2];
      saved[2] = argb[1];
      saved[3] = argb[0];
    }
    unsigned char* const target = reinterpret_cast<unsigned char*>(&this->render.targetTintColor);
    target[0] = 0xFF;
    target[1] = static_cast<unsigned char>(b);
    target[2] = static_cast<unsigned char>(c);
    target[3] = static_cast<unsigned char>(d);
    const std::int64_t red = b;
    const std::int64_t green = c;
    this->render.color = static_cast<std::uint32_t>(red << 16 | green << 8 | static_cast<std::uint32_t>(d) | 0xFF000000);
    break;
  }
  case AM_RETURN_COLOR:
    this->render.savedTintColor = 0;
    this->render.colorLocked = 0;
    break;
  case AM_DELETE_BUYING_SHOP_TITLE:
    if (this->actorWindows[4] == 0) {
      break;
    }
    g_windowMgr.Schedule(this->actorWindows[4]);
    this->actorWindows[4] = 0;
    break;
  case AM_MAKE_BUYING_SHOP_TITLE: {
    if (this->actorWindows[4] == 0) {
      UIMerchantShopTitle* const title = new UIMerchantShopTitle(1);
      this->actorWindows[4] = title;
      title->SetSize(140, 34);
      g_windowMgr.AddRenderedFront(this->actorWindows[4]);
    }
    this->actorWindows[4]->SendMsg(nullptr, UM_SETINFO, b, static_cast<std::int16_t>(this->render.fontSequenceState), 0, 0);
    this->actorWindows[4]->SendMsg(nullptr, UM_SETNAID, this->render.accountId, 0, 0, 0);
    this->actorWindows[4]->Move(this->render.screenX - 70, this->render.screenY - ScaleByScreenHeight(110));
    if (g_serviceOptions.Get(0xFA)) {
      break;
    }
    this->actorWindows[4]->Show(0);
    break;
  }
  case AM_FASTMOVE:
    if (g_session.IsPlayerJobId(this->GetEffectiveJob(-1))) {
      if (CRagEffect* effect = this->LaunchEffect(EF_MAKEBLUR, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0xF3, 0, 100, 0);
      }
      if (CEffect* fallenAngel = g_effectMgr->Find(this->render.accountId, EF_FALLEN_ANGEL)) {
        vector3d position{0.0f, 0.0f, 0.0f};
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(static_cast<int>(b), static_cast<int>(c), position.x, position.z);
        position.y = this->render.position.y;
        static_cast<CFallenAngelEffect*>(fallenAngel)->SetPosition(position);
      }
    }
    this->SendMsg(nullptr, AM_STOPMOVE, b, c, 15, 0, 0, 0);
    this->SetAction(this->render.animation, 6, 1);
    this->render.curMotion = 7;
    this->render.isMotionFreezed = true;
    break;
  case AM_UPDATE_MONSTER_GAGE:
    if (b == -1 ||
        c == -1) {
      if (this->hpGaugeWindow == nullptr) {
        break;
      }
      g_windowMgr.Schedule(this->hpGaugeWindow);
      this->hpGaugeWindow = nullptr;
      break;
    }
    if (d == 1 && this->hpGaugeWindow != nullptr) {
      g_windowMgr.Schedule(this->hpGaugeWindow);
      this->hpGaugeWindow = nullptr;
    }
    if (this->hpGaugeWindow == nullptr) {
      UIMonsterGage* const gage = new UIMonsterGage();
      this->hpGaugeWindow = gage;
      gage->SetSize(60, 5);
      g_windowMgr.AddRenderedFront(this->hpGaugeWindow);
    }
    this->hpGaugeWindow->SetValues(static_cast<int>(b), static_cast<int>(c));
    this->hpGaugeWindow->Move(
      this->render.screenX - this->hpGaugeWindow->geometry.width / 2,
      static_cast<int>(static_cast<float>(ScaleByScreenHeight(12)) * this->render.distanceScale + static_cast<float>(this->render.screenY))
    );
    break;
  case AM_MAKE_MISFORTUNE_EFFECT:
    if (this->firstFollowerEffect == nullptr) {
      CGameObject* effect = mode->SpawnMsgEffect(68, &this->render.position, this->render.accountId, true);
      this->msgEffectList.push_back(effect);
      effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
      this->firstFollowerEffect = effect;
    }
    break;
  case AM_DELETE_MISFORTUNE_EFFECT:
    if (this->firstFollowerEffect != nullptr) {
      this->firstFollowerEffect->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->firstFollowerEffect = nullptr;
    }
    break;
  case AM_MAKE_DEEPSILENCE_EFFECT:
    if (this->secondFollowerEffect == nullptr) {
      CGameObject* effect = mode->SpawnMsgEffect(9, &this->render.position, this->render.accountId, true);
      this->msgEffectList.push_back(effect);
      effect->SendMsg(this, AM_SET_MASTER_POINTER, 0, 0, 0, 0, 0, 0);
      this->secondFollowerEffect = effect;
    }
    break;
  case AM_DELETE_DEEPSILENCE_EFFECT:
    if (this->secondFollowerEffect != nullptr) {
      this->secondFollowerEffect->SendMsg(nullptr, AM_DISAPPEAR, 0, 0, 0, 0, 0, 0);
      this->secondFollowerEffect = nullptr;
    }
    break;
  default:
    this->EffectActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    break;
  }
}

