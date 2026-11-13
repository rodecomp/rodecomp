#include "Contents/Actors/GroundScope.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Skills/SkillId.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>
#include <cstdint>
#include <windows.h>
#include <mmsystem.h>

// GLOBAL: gl_24678
int g_updateTick;

// OFFSET: 0x00E4A450
// FUNCTION: fn_24253
CGroundScope::CGroundScope() {
  this->m_lastingTime = 0;
  this->m_size = 1;
  this->m_angle = 0.0f;
  this->m_skillId = -1;
}

// OFFSET: 0x00E4C2F0
// FUNCTION: fn_24284
bool CGroundScope::OnProcess() {
  if (this->m_lastingTime < timeGetTime()) {
    return false;
  }

  return true;
}

// OFFSET: 0x00E4AE10
// FUNCTION: fn_24267
int CGroundScope::SkillScopeType(int skillId) {
  switch (skillId) {
    case MG_NAPALMBEAT:
    case AL_PNEUMA:
    case AC_SHOWER:
    case PR_BENEDICTIO:
    case WZ_FIREPILLAR:
    case WZ_WATERBALL:
    case WZ_FROSTNOVA:
    case BS_HAMMERFALL:
    case HT_BLITZBEAT:
    case HT_DETECTING:
    case AS_GRIMTOOTH:
    case AS_VENOMDUST:
    case SN_FALCONASSAULT:
    case HW_GANBANTEIN:
    case NC_ARMSCANNON:
      return 3;
    case MG_FIREBALL:
    case MG_THUNDERSTORM:
    case PR_SANCTUARY:
    case WZ_HEAVENDRIVE:
    case WZ_QUAGMIRE:
    case HW_GRAVITATION:
    case NPC_GROUNDDRIVE:
    case RK_WINDCUTTER:
    case GC_POISONSMOKE:
    case AB_EPICLESIS:
      return 5;
    case PR_MAGNUS:
    case CR_SLIMPITCHER:
    case NPC_MAGMA_ERUPTION:
    case NPC_CLOUD_KILL:
    case RA_DETONATOR:
    case SO_CLOUD_KILL:
    case SO_WARMER:
    case SO_VACUUM_EXTREME:
    case SO_VARETYR_SPEAR:
    case NC_MAGMA_ERUPTION:
    case LG_KINGS_GRACE:
    case SU_CN_METEOR:
    case SU_CN_METEOR2:
    case AG_TORNADO_STORM:
    case AG_FLORAL_FLARE_ROAD:
    case AG_ASTRAL_STRIKE:
    case CD_PNEUMATICUS_PROCELLA:
      return 7;
    case WZ_METEOR:
    case WZ_STORMGUST:
    case NPC_STORMGUST2:
    case NPC_DRAGONBREATH:
    case NPC_RAINOFMETEOR:
    case RK_DRAGONBREATH:
    case NC_COLDSLOWER:
    case SO_EARTHGRAVE:
    case RK_DRAGONBREATH_WATER:
    case AG_VIOLENT_QUAKE:
    case AG_ALL_BLOOM:
    case ABC_ABYSS_STRIKE:
    case EM_DIAMOND_STORM:
    case EM_LIGHTNING_LAND:
    case EM_VENOM_SWAMP:
    case EM_CONFLAGRATION:
    case EM_TERRA_DRIVE:
      return 9;
    case WZ_VERMILION:
    case AG_MYSTERY_ILLUSION:
      return 11;
    case NPC_COMET:
    case WL_COMET:
      return 18;
    case CD_COMPETENTIA:
      return 19;
    case NPC_PSYCHIC_WAVE:
    case SO_DIAMONDDUST:
    case SO_PSYCHIC_WAVE:
      return -2;
    default:
      return CSkillActionMgr::GetInstance()->GetSkillScopeType(skillId);
  }
}

// OFFSET: 0x00E50000
// FUNCTION: fn_24292
void CGroundScope::Render(int argument) {
  const int cellX = this->m_cellX;
  const int cellY = this->m_cellY;
  g_modeMgr.GetGameMode();
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  GroundGeometry* ground = mode->scene->ground;
  const float center = static_cast<float>(this->m_size) * 0.5f;
  const int half = this->m_size / 2;
  const int left = cellX - half;
  const int top = cellY - half;
  const float sine = ClientSin(this->m_angle);
  const float cosine = ClientCos(this->m_angle);
  const std::uint32_t color = (static_cast<unsigned>(sine * 40.0f) << 24) + 0x80FFFFFF;


  for (int row = 0; row < this->m_size; ++row) {
    for (int column = 0; column < this->m_size; ++column) {
      const float row0 = static_cast<float>(row) - center;
      const float row1 = static_cast<float>(row + 1) - center;
      const float column0 = static_cast<float>(column) - center;
      const float column1 = static_cast<float>(column + 1) - center;
      const float size = static_cast<float>(this->m_size);
      float uv[8];
      uv[0] = (row0 * sine + column0 * cosine + center) / size;
      uv[1] = (row0 * cosine - column0 * sine + center) / size;
      uv[2] = (row0 * sine + column1 * cosine + center) / size;
      uv[3] = (row0 * cosine - column1 * sine + center) / size;
      uv[4] = (row1 * sine + column0 * cosine + center) / size;
      uv[5] = (row1 * cosine - column0 * sine + center) / size;
      uv[6] = (row1 * sine + column1 * cosine + center) / size;
      uv[7] = (row1 * cosine - column1 * sine + center) / size;

      const char* name;
      std::uint32_t tint = color;
      switch (this->m_skillId) {
        case 0x7F0:
          name = g_serviceOptions.Get(0xE5) ? "effect\\mineffect\\new_poisonsmoke\\new_poisonsmoke_cast\\poi_magic_circle_1.tga"
                                             : "effect\\new_poisonsmoke\\new_poisonsmoke_cast\\poi_magic_circle_1.tga";
          break;
        case 0x8A5:
          name = "effect\\new_comet\\new_comet_cast_magicrune\\new_comet_cast_magicrune.tga";
          break;
        case 0x149E:
          name = "effect\\competentia\\competentia_cast_bottom\\com_rune_01.tga";
          tint = static_cast<unsigned char>(static_cast<int>(static_cast<float>(color >> 24) * 0.6f)) << 24 | 0xFFFFFF;
          break;
        default:
          name = "effect\\magic_target.tga";
          break;
      }

      CTexture* texture = g_texMgr.Load(name, 0, false, true, false);
      ground->DrawGridDecal(reinterpret_cast<const float*>(argument), left + column, top + row, tint, *texture, uv);
    }
  }

  if (g_updateTick != 0) {
    this->m_angle += this->m_angleSpeed;
  }
}

// OFFSET: 0x00E50DB0
// FUNCTION: fn_24299
void CGroundScope::SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) {
  switch (message) {
    case AM_SETMASTERGID:
      this->m_masterGid = static_cast<int>(b);
      return;

    case AM_SETPOS:
      this->m_cellX = static_cast<int>(b);
      this->m_cellY = static_cast<int>(c);
      return;

    case AM_SETTIME:
      this->m_lastingTime = static_cast<unsigned long>(b);
      return;

    case AM_SET_SKILL:
      this->m_size = this->SkillScopeType(static_cast<int>(b)) + 2;
      this->m_angleSpeed = this->m_size != 0 ? 0.5f / static_cast<float>(this->m_size) : 0.0f;
      this->m_skillId = static_cast<int>(b);
      return;
  }

  this->EffectActor::SendMsg(sender, message, b, c, d, e, 0, 0);
}
