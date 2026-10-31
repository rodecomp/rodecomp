#include "UI/Window/UIVideoWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/Movies/BinkMovie.h"
#include "Contents/Sound/SoundMgr.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <string>

// OFFSET: 0x00AB40C0
// FUNCTION: fn_17437
UIVideoWnd::UIVideoWnd(UIWindowContext& windowContext, UIVideoWndContext& videoContext)
    : UIWindow(windowContext, 1), m_movie(nullptr), m_texture(nullptr), m_savedVolume(-1), m_videoContext(&videoContext) {
  this->m_movie = new BinkMovie();
}

// OFFSET: 0x00AB42C0
// FUNCTION: fn_17439
UIVideoWnd::~UIVideoWnd() {
  if (this->m_texture) {
    delete this->m_texture;
    this->m_texture = nullptr;
  }

  delete this->m_movie;

  if (this->m_savedVolume != -1) {
    CSoundMgr::SetVolume(g_soundMgr, this->m_savedVolume);
  }
}

// OFFSET: 0x00AB4060
// FUNCTION: fn_17436
UIVideoSkipButton::UIVideoSkipButton(UIWindowContext& windowContext, UIButtonContext& buttonContext) : UIButton(windowContext, buttonContext) {
}

// OFFSET: 0x00AB4380
// FUNCTION: fn_17440
bool UIVideoWnd::Slot02() {
  return true;
}

// OFFSET: 0x00AB4390
// FUNCTION: fn_17441
void UIVideoWnd::Slot15(int width, int height) {
  UIVideoSkipButton* button = new UIVideoSkipButton(*this->m_context, this->m_videoContext->buttons);
  button->SetSize(127, 43);
  button->Move(width - button->geometry.width - 33, height - button->geometry.height - 110);
  button->SetId(521);
  this->AddChild(button);
}

// OFFSET: 0x00AB4460
// FUNCTION: fn_17442
void UIVideoSkipButton::Paint() {
  this->ClearSurface(0xFF000000);
  std::string text = "SKIP>";
  UIButtonContext& controls = *this->m_buttonContext;
  const int textWidth = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 30, 1, 0);
  const int textHeight = this->TextHeight(controls.fonts, controls.locale, text.c_str(), 0, 0, 30, 1, 0);
  const std::uint32_t color = this->geometry.unknown_0030 == 1 ? 0x4EC7FF : 0xFFFFFF;
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - textWidth) * 0.5f + 5.0f),
    static_cast<int>(static_cast<float>(this->geometry.height - textHeight) * 0.5f), text.c_str(), 0, 0, 30, color, 1, 0
  );
}

// OFFSET: 0x00AB4600
// FUNCTION: fn_17443
void UIVideoWnd::Paint() {
  this->SetSolid(this->m_context->primitives, this->geometry.width, this->geometry.height, 0xFF000000);
  if (this->m_movie->State() != 1) {
    return;
  }

  const BinkHandle* handle = this->m_movie->Handle();
  const int width = this->geometry.width;
  const int height = this->geometry.height;
  const float scaled = (std::min)(static_cast<float>(static_cast<int>(handle->height)) / static_cast<float>(static_cast<int>(handle->width)) * static_cast<float>(width), static_cast<float>(height));
  const int videoHeight = static_cast<int>(scaled);
  this->DrawStretched(this->m_context->primitives, 0, static_cast<int>((height - videoHeight) * 0.5), this->m_texture, 0xFFFFFFFF, width, videoHeight);
}

// OFFSET: 0x00AB4680
// FUNCTION: fn_17444
void UIVideoWnd::Slot19() {
  if (this->m_movie->Update() == true && this->m_movie->CopyTo(this->m_texture) == true) {
    this->Invalidate();
  }

  if (this->m_movie->State() == 2) {
    this->m_videoContext->windows.CloseWindow(304);
  }
}

// OFFSET: 0x00AB46D0
// FUNCTION: fn_17445
int UIVideoWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 0:
    if (argument1 == 27) {
      this->SendMsg(nullptr, 5, 0, 0, 0, 0);
    }
    break;
  case 5:
    if (this->m_videoContext->dialogs.Confirm(LookupMessage(3142), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
      this->m_videoContext->windows.CloseWindow(304);
    }
    break;
  case 6:
    if (argument1 == 521) {
      this->m_videoContext->windows.CloseWindow(304);
    }
    break;
  case 150:
    if (this->m_movie->Open(reinterpret_cast<const char*>(argument1), 0, false) && this->m_movie) {
      if (this->m_texture) {
        g_texMgr.Remove(this->m_texture);
        this->m_texture = nullptr;
      }

      const BinkHandle* handle = this->m_movie->Handle();
      this->m_texture = CreateTexture(handle->width, handle->height, 4, false, false);
    }
    break;
  case 151: {
    BinkHandle* handle = this->m_movie->Handle();
    BinkPause(handle, handle->paused == 1 ? 0 : 1);
    break;
  }
  case 159:
    BinkSetVolume(this->m_movie->Handle(), 0, argument1 << 8);
    if (argument2 == 0) {
      this->m_savedVolume = CSoundMgr::Volume(g_soundMgr);
      CSoundMgr::SetVolume(g_soundMgr, 0);
    }
    break;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
