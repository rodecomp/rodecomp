#include "Render/Renderer.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIVideoWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/Movies/BinkMovie.h"
#include "Contents/Sound/SoundMgr.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Window/WindowManagerWindows.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <string>

// OFFSET: 0x00AB40C0
// FUNCTION: fn_17437
UIVideoWnd::UIVideoWnd()
    : UIWindow(1), m_movie(nullptr), m_texture(nullptr), m_savedVolume(-1) {
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
UIVideoSkipButton::UIVideoSkipButton() : UIButton() {
}

// OFFSET: 0x00AB4380
// FUNCTION: fn_17440
bool UIVideoWnd::IsFrameWnd() {
  return true;
}

// OFFSET: 0x00AB4390
// FUNCTION: fn_17441
void UIVideoWnd::OnCreate(int width, int height) {
  UIVideoSkipButton* button = new UIVideoSkipButton();
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
  const int textWidth = this->TextWidth(text.c_str(), 0, 0, 30, 1, 0);
  const int textHeight = this->TextHeight(text.c_str(), 0, 0, 30, 1, 0);
  const std::uint32_t color = this->geometry.unknown_0030 == 1 ? 0x4EC7FF : 0xFFFFFF;
  this->DrawTextAt(static_cast<int>(static_cast<float>(this->geometry.width - textWidth) * 0.5f + 5.0f),
    static_cast<int>(static_cast<float>(this->geometry.height - textHeight) * 0.5f), text.c_str(), 0, 0, 30, color, 1, 0
  );
}

// OFFSET: 0x00AB4600
// FUNCTION: fn_17443
void UIVideoWnd::Paint() {
  this->SetSolid(g_renderer->primitives, this->geometry.width, this->geometry.height, 0xFF000000);
  if (this->m_movie->State() != 1) {
    return;
  }

  const BinkHandle* handle = this->m_movie->Handle();
  const int width = this->geometry.width;
  const int height = this->geometry.height;
  const float scaled = (std::min)(static_cast<float>(static_cast<int>(handle->height)) / static_cast<float>(static_cast<int>(handle->width)) * static_cast<float>(width), static_cast<float>(height));
  const int videoHeight = static_cast<int>(scaled);
  this->DrawStretched(g_renderer->primitives, 0, static_cast<int>((height - videoHeight) * 0.5), this->m_texture, 0xFFFFFFFF, width, videoHeight);
}

// OFFSET: 0x00AB4680
// FUNCTION: fn_17444
void UIVideoWnd::OnProcess() {
  if (this->m_movie->Update() == true && this->m_movie->CopyTo(this->m_texture) == true) {
    this->Invalidate();
  }

  if (this->m_movie->State() == 2) {
    g_windowMgr.CloseWindow(WID_VIDEOWND);
  }
}

// OFFSET: 0x00AB46D0
// FUNCTION: fn_17445
int UIVideoWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_COMMAND:
    if (argument1 == ID_CLOSE_VIDEO) {
      g_windowMgr.CloseWindow(WID_VIDEOWND);
    }
    break;
  case UM_DEFPUSH:
    if (argument1 == 27) {
      this->SendMsg(nullptr, UM_CLOSE, 0, 0, 0, 0);
    }
    break;
  case UM_CLOSE:
    if (g_windowMgr.Confirm(LookupMessage(MSI_CANNOT_MOVE_MINI_PARTYWND), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
      g_windowMgr.CloseWindow(WID_VIDEOWND);
    }
    break;
  case UM_OPEN_VIDEO:
    if (this->m_movie->Open(reinterpret_cast<const char*>(argument1), 0, false) && this->m_movie) {
      if (this->m_texture) {
        g_texMgr.Remove(this->m_texture);
        this->m_texture = nullptr;
      }

      const BinkHandle* handle = this->m_movie->Handle();
      this->m_texture = CreateTexture(handle->width, handle->height, 4, false, false);
    }
    break;
  case UM_SET_VIDEO_VOLUME:
    BinkSetVolume(this->m_movie->Handle(), 0, argument1 << 8);
    if (argument2 == 0) {
      this->m_savedVolume = CSoundMgr::Volume(g_soundMgr);
      CSoundMgr::SetVolume(g_soundMgr, 0);
    }
    break;
  case UM_TOGGLE_VIDEO_PAUSE: {
    BinkHandle* handle = this->m_movie->Handle();
    BinkPause(handle, handle->paused == 1 ? 0 : 1);
    break;
  }
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
