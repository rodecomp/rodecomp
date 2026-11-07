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
  // INLINED
  template <typename Byte>
  explicit EncryptedName(const Byte (&source)[Length]) : m_encrypted(true) {
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

// INLINED
__forceinline const char* WaitSpriteName() {
  const char spriteSource[] = {'\x1A', '\x44', '\xF0', '\x3D', '\x17', '\x01', '\xD2', '\x66', '\x3B', '\x40', '\xC5', '\x07', '\x3C',
                               '\x34', '\xCD', '\x68', '\x2C', '\x41', '\xF0', '\x27', '\x34', '\x6D', '\xF2', '\x77', '\x3F', '\x25'};
  thread_local EncryptedName<sizeof(spriteSource), 0x078143534999254DULL> spriteName(spriteSource);
  return spriteName.Text();
}

// INLINED
__forceinline const char* WaitActionName() {
  const char actionSource[] = {'\x36', '\xCE', '\xE6', '\x4D', '\xCF', '\x8B', '\xAC', '\x76', '\x17', '\xCA', '\xD3', '\x77', '\xE4',
                               '\xBE', '\xB3', '\x78', '\x00', '\xCB', '\xE6', '\x57', '\xEC', '\xE7', '\x9E', '\x74', '\x15', '\xAF'};
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
void UIWaitDBSaveWnd::Slot15(int width, int height) {
  UIButton* button = new UIButton();
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
  const char backgroundSource[] = {
      '\xF3', '\x3C', '\x8D', '\x6F', '\x5B', '\x5B', '\x9E', '\x56', '\xF5', '\x77', '\x8D', '\x59', '\x26', '\x2F', '\x07', '\xCC',
      '\x52', '\xFA', '\x39', '\xD1', '\xD9', '\xC6', '\x3A', '\xED', '\x56', '\xCF', '\x21', '\xFA', '\xFA', '\xF1', '\x32', '\xF5',
      '\x54', '\xBD', '\x2F', '\xF8', '\xEB', '\x95'};
  thread_local EncryptedName<sizeof(backgroundSource), 0x9B5B959B954D9333ULL> backgroundName(backgroundSource);
  CBitmapRes* background = static_cast<CBitmapRes*>(resources.Load(Resolve(backgroundName.Text())));
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
void UIWaitDBSaveWnd::Slot27(int, int) {
}

// OFFSET: 0x00AB5070
// FUNCTION: fn_17452
void UIWaitDBSaveWnd::Slot19() {
  if (this->m_closeTime != 0 && timeGetTime() >= this->m_closeTime) {
    this->m_closeTime = 0;
    g_windowMgr.CloseWindow(10024);
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
