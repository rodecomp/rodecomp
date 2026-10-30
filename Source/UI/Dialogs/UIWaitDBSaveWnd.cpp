#include "UI/Dialogs/UIWaitDBSaveWnd.h"

#include "Render/Textures/SpritePixels.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>

#include <windows.h>
#include <timeapi.h>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

// OFFSET: 0x00AB49D0
// FUNCTION: fn_17446
UIWaitDBSaveWnd::UIWaitDBSaveWnd(UIWindowContext& windowContext, UIWaitDBSaveWndContext& waitContext)
    : UIWindow(windowContext, 0), m_animation(0), m_frame(0), m_loopLimit(0), m_sprite(nullptr), m_action(nullptr), m_startTime(0), m_loop(1),
      m_closeTime(0), m_waitContext(&waitContext) {
  const std::uint8_t actionSource[] = {0x36, 0xCE, 0xE6, 0x4D, 0xCF, 0x8B, 0xAC, 0x76, 0x17, 0xCA, 0xD3, 0x77, 0xE4,
                                       0xBE, 0xB3, 0x78, 0x00, 0xCB, 0xE6, 0x57, 0xEC, 0xE7, 0x9E, 0x74, 0x15, 0xAF};
  thread_local EncryptedName<sizeof(actionSource), 0x17FFC98B398FAF61ULL> actionName(actionSource);
  const char* action = actionName.Text();

  const std::uint8_t spriteSource[] = {0x1A, 0x44, 0xF0, 0x3D, 0x17, 0x01, 0xD2, 0x66, 0x3B, 0x40, 0xC5, 0x07, 0x3C,
                                       0x34, 0xCD, 0x68, 0x2C, 0x41, 0xF0, 0x27, 0x34, 0x6D, 0xF2, 0x77, 0x3F, 0x25};
  thread_local EncryptedName<sizeof(spriteSource), 0x078143534999254DULL> spriteName(spriteSource);
  const char* sprite = spriteName.Text();

  this->m_closeTime = timeGetTime() + 60000;
  this->m_sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprite));
  this->m_action = static_cast<CActRes*>(ResourceMgr::Instance().Load(action));
  this->m_startTime = timeGetTime();
}

// OFFSET: 0x00AB4CE0
// FUNCTION: fn_17448
void UIWaitDBSaveWnd::Slot15(int width, int height) {
  UIButton* button = new UIButton(*this->m_context, this->m_waitContext->buttons);
  button->SetSize(width, height);
  button->Move(0, 0);
  button->SetId(0);
  button->SetDrawLabel(0);
  this->AddChild(button);
}

// OFFSET: 0x00AB4D80
// FUNCTION: fn_17449
void UIWaitDBSaveWnd::Slot18() {
}

// OFFSET: 0x00AB4D90
// FUNCTION: fn_17450
void UIWaitDBSaveWnd::Paint() {
  if (!this->geometry.surface) {
    return;
  }

  this->ClearSurface(0xFFFF00FF);
  ResourceMgr& resources = ResourceMgr::Instance();
  const std::uint8_t backgroundSource[] = {0xF3, 0x3C, 0x8D, 0x6F, 0x5B, 0x5B, 0x9E, 0x56, 0xF5, 0x77, 0x8D, 0x59, 0x26, 0x2F, 0x07, 0xCC, 0x52, 0xFA, 0x39,
                                           0xD1, 0xD9, 0xC6, 0x3A, 0xED, 0x56, 0xCF, 0x21, 0xFA, 0xFA, 0xF1, 0x32, 0xF5, 0x54, 0xBD, 0x2F, 0xF8, 0xEB, 0x95};
  thread_local EncryptedName<sizeof(backgroundSource), 0x9B5B959B954D9333ULL> backgroundName(backgroundSource);
  CBitmapRes* background = static_cast<CBitmapRes*>(resources.Load(Resolve(backgroundName.Text())));
  if (background) {
    this->DrawBitmap(
      static_cast<int>((this->geometry.width - background->image.width) * 0.5), static_cast<int>((this->geometry.height - background->image.height) * 0.5),
      &background->image, false
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
void UIWaitDBSaveWnd::Slot27(int, int) {
}

// OFFSET: 0x00AB5070
// FUNCTION: fn_17452
void UIWaitDBSaveWnd::Slot19() {
  if (this->m_closeTime != 0 && timeGetTime() >= this->m_closeTime) {
    this->m_closeTime = 0;
    this->m_waitContext->windows.CloseWindow(10024);
    return;
  }

  if (!this->m_action || this->m_action->Speed(this->m_animation) == 0.0f) {
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
