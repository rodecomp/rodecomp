#include "Contents/Actors/GraffiSkill.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

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
