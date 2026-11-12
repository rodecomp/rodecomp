#include "UI/Login/UIWaitDBSaveWnd.h"
#include "UI/Window/WindowId.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Render/Textures/SpritePixels.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>

#include <windows.h>
#include <timeapi.h>

namespace {

// INLINED
__forceinline const char* WaitSpriteName() {
  constexpr auto spriteSourceText = OBFUSCATE_SOURCE("WaitDBSave\\NowLoading.spr", 0x078143534999254DULL);
  const auto& spriteSource = spriteSourceText.data;
  thread_local EncryptedName<sizeof(spriteSource), 0x078143534999254DULL> spriteName(spriteSource);
  return spriteName.Text();
}

// INLINED
__forceinline const char* WaitActionName() {
  constexpr auto actionSourceText = OBFUSCATE_SOURCE("WaitDBSave\\NowLoading.act", 0x17FFC98B398FAF61ULL);
  const auto& actionSource = actionSourceText.data;
  thread_local EncryptedName<sizeof(actionSource), 0x17FFC98B398FAF61ULL> actionName(actionSource);
  return actionName.Text();
}

}

// INLINED
UIWaitDBSaveWnd::UIWaitDBSaveWnd(const char* sprite, const char* action)
    : UIWindow(0), m_animation(0), m_frame(0), m_loopLimit(0), m_sprite(nullptr), m_action(nullptr), m_startTime(0), m_loop(1),
      m_closeTime(0) {
  this->m_closeTime = timeGetTime() + 60000;
  this->m_sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprite));
  this->m_action = static_cast<CActRes*>(ResourceMgr::Instance().Load(action));
  this->m_startTime = timeGetTime();
}

// OFFSET: 0x00AB49D0
// FUNCTION: fn_17446
UIWaitDBSaveWnd::UIWaitDBSaveWnd() : UIWaitDBSaveWnd(WaitSpriteName(), WaitActionName()) {
}

// OFFSET: 0x00AB4CE0
// FUNCTION: fn_17448
void UIWaitDBSaveWnd::OnCreate(int width, int height) {
  UIButton* button = new UIButton();
  button->SetSize(width, height);
  button->Move(0, 0);
  button->SetId(0);
  button->SetDrawLabel(0);
  this->AddChild(button);
}

// OFFSET: 0x00AB4D80
// FUNCTION: fn_17449
void UIWaitDBSaveWnd::OnDestroy() {
}

// OFFSET: 0x00AB4D90
// FUNCTION: fn_17450
void UIWaitDBSaveWnd::Paint() {
  if (!this->geometry.surface) {
    return;
  }

  this->ClearSurface(0xFFFF00FF);
  ResourceMgr& resources = ResourceMgr::Instance();
  CBitmapRes* background = static_cast<CBitmapRes*>(resources.Load(Resolve(OBFUSCATE_TEXT_NAMED("유저인터페이스\\WaitDBSave\\loading.bmp", 0x9B5B959B954D9333ULL, backgroundName))));
  if (background) {
    this->DrawBitmap(
      static_cast<int>((this->geometry.width - background->image.width) * 0.5), static_cast<int>((this->geometry.height - background->image.height) * 0.5),
      background, false
    );
  }

  if (!this->m_sprite || !this->m_action) {
    return;
  }

  const ActionFrame* frame = &this->m_action->Frame(this->m_animation, this->m_frame);
  if (!frame || !this->m_sprite->palette) {
    return;
  }

  for (unsigned index = 0; index < frame->cells.size(); ++index) {
    const SpriteFrameCell& cell = frame->Cell(index);
    if (cell.image == -1 || cell.imageType >= 2) {
      continue;
    }

    const std::vector<SpriteRleImage*>& images = this->m_sprite->images[cell.imageType];
    if (images.size() <= static_cast<unsigned>(cell.image)) {
      continue;
    }

    const SpriteRleImage* image = images[cell.image];
    if (image) {
      this->AddSpriteCellOverlay(
        static_cast<int>(this->geometry.width * 0.5), static_cast<int>(this->geometry.height * 0.5), reinterpret_cast<int>(this->m_action),
        reinterpret_cast<int>(this->m_sprite), image, &cell, 1.0f, 0.0f, 0xFFFFFFFF, this->m_sprite->palette, 1.0f
      );
    }
  }
}

// OFFSET: 0x00AB5060
// FUNCTION: fn_17451
void UIWaitDBSaveWnd::OnMouseMove(int, int) {
}

// OFFSET: 0x00AB5070
// FUNCTION: fn_17452
void UIWaitDBSaveWnd::OnProcess() {
  if (this->m_closeTime != 0 && timeGetTime() >= this->m_closeTime) {
    this->m_closeTime = 0;
    g_windowMgr.CloseWindow(WID_WAITDBSAVEWND);
    return;
  }

  if (!this->m_action || 0.0f == this->m_action->Speed(this->m_animation)) {
    return;
  }

  const float elapsed = static_cast<float>(timeGetTime() - this->m_startTime);
  const unsigned frames = static_cast<unsigned>(elapsed / (this->m_action->Speed(this->m_animation) * 24.0f));
  if (this->m_loopLimit != 0) {
    if (this->m_loop > this->m_loopLimit) {
      return;
    }

    if (frames >= this->m_action->FrameCount(this->m_animation) * this->m_loop) {
      ++this->m_loop;
    }
  }

  this->m_frame = frames % this->m_action->FrameCount(this->m_animation);
  this->Invalidate();
}
