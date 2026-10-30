#include "Contents/Actors/GraffiSkill.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundRendering.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

#include <cmath>
#include <cstdint>
#include <cstring>

// INLINED
CGraffiSkill::~CGraffiSkill() {
  delete this->m_canvas;
}

// OFFSET: 0x00E4B930
// FUNCTION: fn_24275
void CGraffiSkill::ReleaseTexture() {
  delete this->m_canvas;
}

// OFFSET: 0x00E4BAD0
// FUNCTION: fn_24279
void CGraffiSkill::BuildTexture() {
  this->m_penX = 0;
  this->m_penY = 0;
  this->m_font = g_texMgr.Load("effect\\alpabet.bmp", 0, false, true, false);
  this->m_canvas = CreateTexture(0x180, 0x180, 0, false, false);

  int length = static_cast<int>(std::strlen(this->m_text));
  this->m_penY += (0x90 - length) / 24 * 36;

  int width;
  for (int i = 0; i < length; ++i) {
    char letter = this->m_text[i];
    unsigned char glyph;
    int x = this->m_penX;
    if (letter >= 'a' && letter <= 'z') {
      glyph = letter - 'a' + 26;
      width = 32;
    } else if (letter >= 'A' && letter <= 'Z') {
      glyph = letter - 'A';
      width = 32;
    } else if (letter >= '0' && letter <= '9') {
      glyph = letter - '0' + 54;
      width = 32;
    } else if (letter == ' ') {
      for (const char* next = &this->m_text[i + 1]; *next != ' ' && *next != '\0'; ++next) {
        x += 32;
      }

      if (x >= 0x160) {
        this->m_penY += 36;
        x = 0;
        this->m_penX = 0;
      } else {
        x = this->m_penX;
      }

      glyph = 99;
    } else {
      if (letter == '?') {
        glyph = 53;
      } else if (letter == '!') {
        glyph = 54;
      } else {
        glyph = 99;
      }

      width = 16;
    }

    if (width + x >= 0x160) {
      this->m_penY += 36;
      x = 0;
      this->m_penX = 0;
    }

    if (glyph != 99) {
      this->m_canvas->CopyTexture(this->m_font, (glyph & 7) * 32, (glyph >> 3) * 32, 32, 32, x, this->m_penY, width, 32);
      x = this->m_penX;
    }

    x += width;
    this->m_penX = x;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  this->m_angle = mode->scene->player->render.angle;
}

// OFFSET: 0x00E4C2E0
// FUNCTION: fn_24283
bool CGraffiSkill::OnProcess() {
  return true;
}

// OFFSET: 0x00E4FD70
// FUNCTION: fn_24291
void CGraffiSkill::Render(int argument) {
  const int cellX = this->m_cellX;
  const int cellY = this->m_cellY;
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  GroundGeometry* ground = mode->scene->ground;
  const float sine = static_cast<float>(std::sin(static_cast<double>(this->m_angle / 180.0f * 3.141592f)));
  const float cosine = static_cast<float>(std::cos(static_cast<double>(this->m_angle / 180.0f * 3.141592f)));

  const RenderProjection projection = g_renderer->viewport.Projection();
  const bool depthAlpha = g_renderer->fog.supported != 0;
  GroundRenderContext context{
    projection, g_renderer->primitives, g_renderer->queues, g_renderer->viewport.width, g_renderer->viewport.height, g_renderer->frame, depthAlpha,
    g_depthStart, g_depthEnd
  };

  for (int row = 0; row < 18; ++row) {
    for (int column = 0; column < 18; ++column) {
      const float row0 = static_cast<float>(row) - 9.0f;
      const float row1 = static_cast<float>(row + 1) - 9.0f;
      const float column0 = static_cast<float>(column) - 9.0f;
      const float column1 = static_cast<float>(column + 1) - 9.0f;
      float uv[8];
      uv[0] = 1.0f - (row0 * sine + column0 * cosine + 9.0f) / 18.0f;
      uv[1] = (row0 * cosine - column0 * sine + 9.0f) / 18.0f;
      uv[2] = 1.0f - (row0 * sine + column1 * cosine + 9.0f) / 18.0f;
      uv[3] = (row0 * cosine - column1 * sine + 9.0f) / 18.0f;
      uv[4] = 1.0f - (row1 * sine + column0 * cosine + 9.0f) / 18.0f;
      uv[5] = (row1 * cosine - column0 * sine + 9.0f) / 18.0f;
      uv[6] = 1.0f - (row1 * sine + column1 * cosine + 9.0f) / 18.0f;
      uv[7] = (row1 * cosine - column1 * sine + 9.0f) / 18.0f;
      ground->DrawGridDecal(
        *mode->scene->terrain, reinterpret_cast<const float*>(argument), cellX - 9 + column, cellY - 9 + row, 0xFFFFFFFF, *this->m_canvas, uv, context
      );
    }
  }
}

// OFFSET: 0x00E50D10
// FUNCTION: fn_24298
void CGraffiSkill::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int, int, int, int, int, int) {
  const std::int64_t id = static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message);
  if (id == 22) {
    this->skillAid = b;
    return;
  }

  if (id == 117) {
    std::strcpy(this->m_text, reinterpret_cast<const char*>(b));
    this->m_cellX = d;
    this->m_cellY = f;
    this->BuildTexture();
    return;
  }

  this->CGameActor::SendMsg(sender, message, a, b, c, d, e, f, g, 0, 0, 0, 0, 0, 0);
}
