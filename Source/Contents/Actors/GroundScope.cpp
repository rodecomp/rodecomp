#include "Contents/Actors/GroundScope.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/TextureCache.h"

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
    case 0x0B:
    case 0x19:
    case 0x2F:
    case 0x45:
    case 0x50:
    case 0x56:
    case 0x58:
    case 0x6E:
    case 0x81:
    case 0x82:
    case 0x89:
    case 0x8C:
    case 0x17D:
    case 0x1E3:
    case 0x8D5:
      return 3;
    case 0x11:
    case 0x15:
    case 0x46:
    case 0x5B:
    case 0x5C:
    case 0x1E4:
    case 0x302:
    case 0x7D5:
    case 0x7F0:
    case 0x7FC:
      return 5;
    case 0x4F:
    case 0x1DE:
    case 0x2DD:
    case 0x2E3:
    case 0x8BD:
    case 0x992:
    case 0x994:
    case 0x995:
    case 0x996:
    case 0x138E:
    case 0x1395:
    case 0x13A4:
    case 0x13B2:
    case 0x146B:
    case 0x146D:
    case 0x146E:
    case 0x149F:
      return 7;
    case 0x53:
    case 0x59:
    case 0x2D3:
    case 0x2DB:
    case 0x301:
    case 0x7D8:
    case 0x8D4:
    case 0x98E:
    case 0x138C:
    case 0x1462:
    case 0x1466:
    case 0x14C7:
    case 0x14F9:
    case 0x14FA:
    case 0x14FB:
    case 0x14FC:
    case 0x14FD:
      return 9;
    case 0x55:
    case 0x1461:
      return 11;
    case 0x2C4:
    case 0x8A5:
      return 18;
    case 0x149E:
      return 19;
    case 0x2E0:
    case 0x98F:
    case 0x991:
      return -2;
    default:
      return CSkillActionMgr::GetInstance()->ActionSlot14(skillId);
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
  const float sine = static_cast<float>(std::sin(static_cast<double>(this->m_angle)));
  const float cosine = static_cast<float>(std::cos(static_cast<double>(this->m_angle)));
  const std::uint32_t color = (static_cast<unsigned>(sine * 40.0f) << 24) + 0x80FFFFFF;

  const RenderProjection projection = g_renderer->viewport.Projection();
  const bool depthAlpha = g_renderer->fog.supported != 0;
  GroundRenderContext context{
    projection, g_renderer->primitives, g_renderer->queues, g_renderer->viewport.width, g_renderer->viewport.height, g_renderer->frame, depthAlpha,
    g_depthStart, g_depthEnd
  };

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
      if (this->m_skillId == 0x7F0) {
        name = g_serviceOptions.Get(0xE5) ? "effect\\mineffect\\new_poisonsmoke\\new_poisonsmoke_cast\\poi_magic_circle_1.tga"
                                           : "effect\\new_poisonsmoke\\new_poisonsmoke_cast\\poi_magic_circle_1.tga";
      } else if (this->m_skillId == 0x8A5) {
        name = "effect\\new_comet\\new_comet_cast_magicrune\\new_comet_cast_magicrune.tga";
      } else if (this->m_skillId == 0x149E) {
        name = "effect\\competentia\\competentia_cast_bottom\\com_rune_01.tga";
        tint = static_cast<unsigned char>(static_cast<int>(static_cast<float>(color >> 24) * 0.6f)) << 24 | 0xFFFFFF;
      } else {
        name = "effect\\magic_target.tga";
      }

      CTexture* texture = g_texMgr.Load(name, 0, false, true, false);
      ground->DrawGridDecal(*mode->scene->terrain, reinterpret_cast<const float*>(argument), left + column, top + row, tint, *texture, uv, context);
    }
  }

  if (g_updateTick != 0) {
    this->m_angle += this->m_angleSpeed;
  }
}

// OFFSET: 0x00E50DB0
// FUNCTION: fn_24299
void CGroundScope::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  switch (static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) {
    case 14:
      this->m_cellX = b;
      this->m_cellY = d;
      return;

    case 22:
      this->unknown_0158 = b;
      return;

    case 115:
      this->m_lastingTime = b;
      return;

    case 117:
      this->m_size = this->SkillScopeType(b) + 2;
      this->m_skillId = b;
      this->m_angleSpeed = this->m_size != 0 ? 0.5f / static_cast<float>(this->m_size) : 0.0f;
      return;
  }

  this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}
