#include "Render/Renderer.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Render/ActionSpriteDrawing.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"

#include <cstdlib>
#include <cstring>
#include <windows.h>

// OFFSET: 0x0055D150
// FUNCTION: fn_02097
float CRenderer::ScaleDistance(float distance) const {
  return (this->viewport.scaleX * distance) / 7.0f;
}

// OFFSET: 0x0055D170
// FUNCTION: fn_02098
float CRenderer::UiScaleX() const {
  return this->viewport.uiScaleX;
}

// OFFSET: 0x0055D180
// FUNCTION: fn_02099
float CRenderer::UiScaleY() const {
  return this->viewport.uiScaleY;
}

// OFFSET: 0x0055A920
// FUNCTION: fn_02081
void CRenderer::DrawBattleFieldIcon(CRenderSystemBase& system) {
  if (g_session.m_score1 < 0 || g_session.m_score2 < 0) {
    return;
  }

  const TextureRegion region = {0.0f, 0.0f, 1.0f, 1.0f};
  RECT rect;
  rect.left = this->viewport.width - 280;
  rect.top = 26;
  rect.right = this->viewport.width - 232;
  rect.bottom = 74;
  CMode* mode = g_modeMgr.GetGameMode();
  mode->DrawTexture(rect, 0, 0.0999f, 0xCDFFFFFF, g_texMgr.Load("effect\\사자(A).bmp", 0, false, true, false), region);

  rect.left = this->viewport.width - 280;
  rect.top = 84;
  rect.right = this->viewport.width - 232;
  rect.bottom = 132;
  mode = g_modeMgr.GetGameMode();
  mode->DrawTexture(rect, 0, 0.0999f, 0xCDFFFFFF, g_texMgr.Load("effect\\독수리(B).bmp", 0, false, true, false), region);

  const unsigned char width = g_session.m_score1 > 9 || g_session.m_score2 > 9 ? 2 : 1;
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.spr"));

  int digits[2];
  int score = g_session.m_score1 < 0 ? 0 : g_session.m_score1;
  int count = (score > 9) + 1;
  digits[0] = score % 10;
  digits[1] = score / 10 % 10;
  int x = this->viewport.width - (width > 1 ? 175 : 207);
  for (int i = 0; i < count; ++i) {
    g_modeMgr.GetGameMode();
    DrawActionSprite(system, x, 50, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    x -= 32;
  }

  score = g_session.m_score2 < 0 ? 0 : g_session.m_score2;
  count = (score > 9) + 1;
  digits[0] = score % 10;
  digits[1] = score / 10 % 10;
  x = this->viewport.width - (width > 1 ? 175 : 207);
  for (int i = 0; i < count; ++i) {
    g_modeMgr.GetGameMode();
    DrawActionSprite(system, x, 108, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    x -= 32;
  }
}

// OFFSET: 0x0055E840
// FUNCTION: fn_02112
void CRenderer::DrawOverlays(CRenderSystemBase& system) {
  // GLOBAL: gl_24883
  static char s_bannerState;
  // GLOBAL: gl_24885
  static int s_bannerAlpha;
  // GLOBAL: gl_24884
  static short s_shakeStep;

  if (g_serviceType == 0) {
    SYSTEMTIME now;
    GetLocalTime(&now);
    if (now.wMinute == 0 && now.wSecond == 0 && s_bannerState == 0) {
      s_bannerState = 1;
      s_bannerAlpha = 0;
    } else if (now.wMinute == 0 && now.wSecond >= 10 && s_bannerState == 1) {
      s_bannerState = 2;
    } else if (s_bannerState < 1) {
      goto timer;
    }

    {
      CTexture* texture = g_texMgr.Load("유저인터페이스\\", 0, false, true, false);
      if (texture != nullptr) {
        RECT rect;
        rect.bottom = texture->Height() + 45;
        rect.left = this->viewport.width - texture->Width() - 140;
        rect.top = 45;
        rect.right = this->viewport.width - 140;
        if (s_bannerState == 1) {
          if (s_bannerAlpha < 220) {
            ++s_bannerAlpha;
          }
        } else if (s_bannerAlpha > 0) {
          --s_bannerAlpha;
        } else {
          s_bannerState = 0;
        }

        const std::uint32_t color = (s_bannerAlpha << 24) + 0xFFFFFF;
        const TextureRegion region = {0.0f, 0.0f, 1.0f, 1.0f};
        g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, color, texture, region);
      }
    }
  }

timer:
  if (g_session.unknown_5448 != 0) {
    const int scale = this->viewport.height / 700;
    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\TimeFont.act"));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\TimeFont.spr"));
    g_modeMgr.GetGameMode();
    DrawActionSprite(system, 100, 100, action, sprite, 0, 0, static_cast<float>(scale) * 0.5f, 0.0f, 0xFFFFFFFF, 0x201);
    g_modeMgr.GetGameMode();
    DrawActionSprite(system, 100, 200, action, sprite, 1, 0, static_cast<float>(scale), 0.0f, 0xFFFFFFFF, 0x201);
  }

  if (g_session.WorldUnknown0050() == 1) {
    this->DrawBattleFieldIcon(system);
  }

  if (!g_serviceOptions.Get(8) || g_screenShakeDuration <= 0 || g_screenShakeMode == 2 || !this->LockBackBuffer()) {
    return;
  }

  char* line = static_cast<char*>(std::malloc(this->backBuffer.pitch));
  if (s_shakeStep < 2) {
    s_shakeStep = 2;
  }

  for (int y = this->viewport.height - 1; y >= 0; --y) {
    char* row = static_cast<char*>(this->backBuffer.pixels) + this->backBuffer.pitch * y;
    const unsigned char period = s_shakeStep > 10 ? 20 - s_shakeStep : s_shakeStep;
    if (y == this->viewport.height - 1) {
      std::memcpy(line, row, this->backBuffer.pitch);
    }

    if (static_cast<unsigned>(y) % period == 0) {
      std::memcpy(line, row, this->backBuffer.pitch);
    } else {
      const int offset = std::rand() % 8 * 16;
      if (g_screenShakeMode == 0) {
        std::memcpy(row, line, this->backBuffer.pitch);
      } else if (g_screenShakeMode == 1) {
        std::memcpy(row + offset, line, this->backBuffer.pitch - offset);
      }
    }
  }

  s_shakeStep += 2;
  if (s_shakeStep < 2) {
    s_shakeStep = 2;
  } else if (s_shakeStep > 16) {
    g_screenShakeDuration = g_screenShakeDuration - 1 < 0 ? 0 : g_screenShakeDuration - 1;
    s_shakeStep = 2;
  }

  std::free(line);
  this->UnlockBackBuffer();
}

// OFFSET: 0x00560A40
// FUNCTION: fn_02161
CRenderer::CRenderer(CRenderSystemBase& system) : frame(0), unknown_003C(0), unknown_0040(0), unknown_0044(0), twoPass(0), unknown_004C(0) {
  this->atlas.Initialize(system, 50);
  this->fog.enabled = 0;
  this->fog.dirty = 1;
  this->fog.supported = 0;
}

// OFFSET: 0x00562090
// FUNCTION: fn_02175
// OFFSET: 0x00562260
CRenderer::~CRenderer() = default;
