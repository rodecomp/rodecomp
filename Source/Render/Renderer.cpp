#include "Render/Renderer.h"
#include "UI/GameSettings/TalkType.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"

#include <algorithm>
#include <bit>
#include <cstdlib>
#include <cstring>
#include <windows.h>

// GLOBAL: gl_24883
char s_hourlyBannerState;
// GLOBAL: gl_24884
short s_screenShakeStep;
// GLOBAL: gl_24885
int s_hourlyBannerAlpha;

// OFFSET: 0x00544A60
// FUNCTION: fn_01820
std::uint32_t __fastcall sub_544A60(const CRenderer* renderer) {
  return std::bit_cast<std::uint32_t>(renderer->viewDirectionZ);
}

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
void CRenderer::DrawBattleFieldIcon() {
  if (g_session.m_score1 < 0 || g_session.m_score2 < 0) {
    return;
  }

  const TextureRegion region = {0.0f, 0.0f, 1.0f, 1.0f};
  RECT rect;
  rect.left = this->viewport.width - 280;
  rect.top = 26;
  rect.right = this->viewport.width - 232;
  rect.bottom = 74;
  CGameMode* mode = g_modeMgr.GetGameMode();
  CTexture* texture = g_texMgr.Load("effect\\사자(A).bmp", 0, false, true, false);
  mode->DrawTexture(rect, 0, 0.0999f, 0xCDFFFFFF, texture, region);

  rect.left = this->viewport.width - 280;
  rect.top = 84;
  rect.right = this->viewport.width - 232;
  rect.bottom = 132;
  mode = g_modeMgr.GetGameMode();
  texture = g_texMgr.Load("effect\\독수리(B).bmp", 0, false, true, false);
  mode->DrawTexture(rect, 0, 0.0999f, 0xCDFFFFFF, texture, region);

  const unsigned char width = g_session.m_score1 > 9 || g_session.m_score2 > 9 ? 2 : 1;
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.spr"));

  int score = g_session.m_score1;
  if (score < 0) {
    score = 0;
  }

  int count = 1;
  if (score > 9) {
    count = 2;
  }

  int digits[2];
  digits[0] = score % 10;
  score /= 10;
  digits[1] = score % 10;

  int x = this->viewport.width - 150;
  for (int i = 0; i < count; ++i) {
    if (width > 1) {
      g_modeMgr.GetGameMode()->DrawActionSprite(x - i * 32 - 25, 50, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    } else {
      g_modeMgr.GetGameMode()->DrawActionSprite(x - i * 32 - 25 - 32, 50, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    }
  }

  score = g_session.m_score2;
  if (score < 0) {
    score = 0;
  }

  count = 1;
  if (score > 9) {
    count = 2;
  }

  digits[0] = score % 10;
  score /= 10;
  digits[1] = score % 10;

  x = this->viewport.width - 150;
  for (int i = 0; i < count; ++i) {
    if (width > 1) {
      g_modeMgr.GetGameMode()->DrawActionSprite(x - i * 32 - 25, 108, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    } else {
      g_modeMgr.GetGameMode()->DrawActionSprite(x - i * 32 - 25 - 32, 108, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    }
  }
}

// OFFSET: 0x0055A7E0
// FUNCTION: fn_46353
void CRenderer::DrawHourlyBanner() {
  SYSTEMTIME now;
  GetLocalTime(&now);
  if (now.wMinute == 0 && now.wSecond == 0 && s_hourlyBannerState == 0) {
    s_hourlyBannerState = 1;
    s_hourlyBannerAlpha = 0;
  } else if (now.wMinute == 0 && now.wSecond >= 10 && s_hourlyBannerState == 1) {
    s_hourlyBannerState = 2;
  } else if (s_hourlyBannerState < 1) {
    return;
  }

  CTexture* texture = g_texMgr.Load("유저인터페이스\\", 0, false, true, false);
  if (texture != nullptr) {
    RECT rect;
    rect.bottom = texture->Height() + 45;
    const int width = this->viewport.width;
    rect.left = width - texture->Width() - 140;
    rect.top = 45;
    rect.right = width - 140;
    int alpha = s_hourlyBannerAlpha;
    if (s_hourlyBannerState == 1) {
      if (alpha < 220) {
        s_hourlyBannerAlpha = ++alpha;
      }
    } else if (alpha > 0) {
      s_hourlyBannerAlpha = --alpha;
    } else {
      s_hourlyBannerState = 0;
    }

    const std::uint32_t color = (alpha << 24) + 0xFFFFFF;
    const TextureRegion region = {0.0f, 0.0f, 1.0f, 1.0f};
    g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, color, texture, region);
  }
}

// OFFSET: 0x0055E840
// FUNCTION: fn_02112
void CRenderer::DrawOverlays() {
  if (g_serviceType == 0) {
    this->DrawHourlyBanner();
  }

  if (g_session.showTimer != 0) {
    const int scale = this->viewport.height / 700;
    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\TimeFont.act"));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\TimeFont.spr"));
    g_modeMgr.GetGameMode()->DrawActionSprite(100, 100, action, sprite, 0, 0, static_cast<float>(scale) * 0.5f, 0.0f, 0xFFFFFFFF, 0x201);
    g_modeMgr.GetGameMode()->DrawActionSprite(100, 200, action, sprite, 1, 0, static_cast<float>(scale), 0.0f, 0xFFFFFFFF, 0x201);
  }

  if (g_session.GetBattleFieldMode() == 1) {
    this->DrawBattleFieldIcon();
  }

  if (!g_serviceOptions.Get(TT_EFFECT_ON_OFF) || g_screenShakeDuration <= 0 || g_screenShakeMode == 2 || !this->LockBackBuffer()) {
    return;
  }

  char* line = static_cast<char*>(std::malloc(this->backBuffer.pitch));
  if (s_screenShakeStep < 2) {
    s_screenShakeStep = 2;
  }

  for (int y = this->viewport.height - 1; y >= 0; --y) {
    char* row = static_cast<char*>(this->backBuffer.pixels) + this->backBuffer.pitch * y;
    const unsigned char period = s_screenShakeStep > 10 ? 20 - s_screenShakeStep : s_screenShakeStep;
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

  s_screenShakeStep += 2;
  if (s_screenShakeStep < 2) {
    s_screenShakeStep = 2;
  } else if (s_screenShakeStep > 16) {
    if (--g_screenShakeDuration < 0) {
      g_screenShakeDuration = 0;
    }
    s_screenShakeStep = 2;
  }

  std::free(line);
  this->UnlockBackBuffer();
}

// OFFSET: 0x00560A40
// FUNCTION: fn_02161
CRenderer::CRenderer() : viewDirectionX(0), viewDirectionY(0), viewDirectionZ(0) {
  this->frame = 0;
  this->twoPass = 0;
  this->unknown_004C = 0;
  this->commands.Init(1024);
  this->primitives.Init(512);
  this->lightmapped.Init(8192);
  this->vertices.Init(10240);
  this->atlas.Initialize(50);
  this->fog.enabled = 0;
  this->fog.dirty = 1;
  this->fog.supported = 0;
  this->ReserveQueues(1000);
}

// OFFSET: 0x00562090
// FUNCTION: fn_02175
CRenderer::~CRenderer() = default;
