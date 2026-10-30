#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIRadioBtn.h"

// OFFSET: 0x00851B00
// FUNCTION: fn_10696
UIRadioBtn::~UIRadioBtn() = default;

// OFFSET: 0x0085D470
// FUNCTION: fn_10814
void UIRadioBtn::Paint() {
  UIButtonContext& controls = *this->m_controls;

  if (!this->checkedBitmap.empty() && !this->uncheckedBitmap.empty()) {
    this->ClearSurface(0xFFFF00FF);
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->checkedBitmap.c_str())));
    if (this->geometry.unknown_0030 == 0) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->uncheckedBitmap.c_str())));
    }

    if (bitmap) {
      this->DrawBitmap(0, static_cast<int>((this->geometry.height - bitmap->image.height) * 0.5f), &bitmap->image, true);
    }

    const int textHeight = this->TextHeight(controls.fonts, controls.locale, this->text.c_str(), 0, 0, this->fontHeight, 0, 0);
    const std::uint32_t color = this->enabled == 1 ? this->textColor : 0xB5B5B5;
    this->DrawTextAt(
      controls.fonts, controls.locale, bitmap->image.width + 3, static_cast<int>((this->geometry.height - textHeight) * 0.5f), this->text.c_str(), 0, 0,
      this->fontHeight, color, 0, 0
    );
    return;
  }

  this->ClearSurface(0xFFFF00FF);
  const char* name;
  if (this->geometry.unknown_0030 == 0) {
    name = "유저인터페이스\\radiobtn_off.bmp";
  } else {
    name = this->highlighted != 0 ? "유저인터페이스\\radiobtn_on1.bmp" : "유저인터페이스\\radiobtn_on.bmp";
  }

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  const std::uint32_t color = this->enabled != 0 ? this->textColor : 0xB5B5B5;
  this->DrawTextAt(controls.fonts, controls.locale, 13, 0, this->text.c_str(), 0, 0, this->fontHeight, color, 0, 0);
}

// OFFSET: 0x0085ECB0
// FUNCTION: fn_10844
void UIRadioBtn::Slot25(int, int) {
  if (this->parent && this->enabled != 0) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->value, 0, 0);
  }
}

// OFFSET: 0x0085F400
// FUNCTION: fn_10861
void UIRadioBtn::Slot28(int x, int y) {
  this->Slot27(x, y);
  if (this->tooltip.size()) {
    x = 10;
    y = -20;
    this->ToScreen(x, y);
    reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(this->tooltip.c_str(), x, y, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x00865180
// FUNCTION: fn_10919
int UIRadioBtn::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 13) {
    this->geometry.unknown_0030 = argument1;
    this->Invalidate();
  }

  return 0;
}

// OFFSET: 0x00868180
// FUNCTION: fn_10954
void UIRadioBtn::SetText(const char* value) {
  this->text = value;
  this->Invalidate();
}
