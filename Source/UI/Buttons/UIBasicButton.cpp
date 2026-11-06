#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"

#include <cstring>

// OFFSET: 0x0084EA20
// FUNCTION: fn_10649
UIBasicButton::UIBasicButton() : UIButton() {
  this->geometry.height = 20;
  this->invalidateParent = 0;
}

UIBasicButton::~UIBasicButton() = default;

// OFFSET: 0x00809910
// FUNCTION: fn_10060
void UIBasicButton::SetText(std::string value) {
  this->text = value;
  this->Invalidate();
}

// OFFSET: 0x00854240
// FUNCTION: fn_10747
bool UIBasicButton::Configure(int width, const char* value) {
  if (!value) {
    return false;
  }

  this->Resize(width, this->geometry.height);
  this->text = value;

  if (width < this->TextWidth(value, 0, 0, 12, 0, 0)) {
    return false;
  }

  this->Invalidate();
  return true;
}

// INLINED
static __forceinline void DrawButtonSkin(UIBasicButton* button, const char* left, const char* middle, const char* right) {
  auto* bitmap = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(left)));
  button->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  const int count = (button->geometry.width - 12) / 6 + 1;

  for (int index = 1, x = 6; index <= count; ++index, x += 6) {
    if (index == count) {
      bitmap = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(middle)));
      button->DrawBitmap(button->geometry.width - 12, 0, bitmap ? &bitmap->image : nullptr, false);
    } else {
      bitmap = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(middle)));
      button->DrawBitmap(x, 0, bitmap ? &bitmap->image : nullptr, false);
    }
  }

  bitmap = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(right)));
  button->DrawBitmap(button->geometry.width - 6, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x0085B230
// FUNCTION: fn_10794
void UIBasicButton::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->m_disabled == 1) {
    DrawButtonSkin(this, "유저인터페이스\\basic_interface\\btn_disable_left.bmp", "유저인터페이스\\basic_interface\\btn_disable_mid.bmp", "유저인터페이스\\basic_interface\\btn_disable_right.bmp");
    const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0xE2E2E2, 0, 0);
    this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0x6F6F6F, 0, 0);
  } else {
    switch (this->geometry.unknown_0030) {
    case 0: {
      DrawButtonSkin(this, "유저인터페이스\\basic_interface\\btn_out_left.bmp", "유저인터페이스\\basic_interface\\btn_out_mid.bmp", "유저인터페이스\\basic_interface\\btn_out_right.bmp");
      const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
      const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
      this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0xD7D7D7, 0, 0);
      this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0, 0, 0);
      break;
    }
    case 1: {
      DrawButtonSkin(this, "유저인터페이스\\basic_interface\\btn_over_left.bmp", "유저인터페이스\\basic_interface\\btn_over_mid.bmp", "유저인터페이스\\basic_interface\\btn_over_right.bmp");
      const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
      const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
      this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0xC7A796, 0, 0);
      this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0, 0, 0);
      break;
    }
    case 2: {
      DrawButtonSkin(this, "유저인터페이스\\basic_interface\\btn_press_left.bmp", "유저인터페이스\\basic_interface\\btn_press_mid.bmp", "유저인터페이스\\basic_interface\\btn_press_right.bmp");
      const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
      const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
      this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
      this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0, 0, 0);
      break;
    }
    }
  }

  if (this->invalidateParent == 1 && this->parent) {
    this->parent->Invalidate();
  }
}
