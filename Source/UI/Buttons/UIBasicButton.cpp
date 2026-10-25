#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Buttons/UIBasicButton.h"

#include <cstring>

// OFFSET: 0x0084EA20
// FUNCTION: fn_10649
UIBasicButton::UIBasicButton(UIWindowContext& windows, UIButtonContext& controls) : UIButton(windows, controls), invalidateParent(0) {
  this->geometry.height = 20;
}

// OFFSET: 0x00852170
UIBasicButton::~UIBasicButton() = default;

// OFFSET: 0x00854240
// FUNCTION: fn_10747
bool UIBasicButton::Configure(int width, const char* value) {
  if (!value) {
    return false;
  }

  this->Resize(width, this->geometry.height);
  this->text = value;

  if (width < this->TextWidth(this->m_buttonContext->fonts, this->m_buttonContext->locale, value, 0, 0, 12, 0, 0)) {
    return false;
  }

  this->Invalidate();
  return true;
}

// OFFSET: 0x0085B230
// FUNCTION: fn_10794
void UIBasicButton::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, 0xFFFF00FFu);
  const unsigned state = this->m_disabled == 1 ? 3 : this->geometry.unknown_0030;

  if (this->m_disabled == 1 || this->geometry.unknown_0030 <= 2) {
    static constexpr const char* skins[] = {"out", "over", "press", "disable"};
    const std::string prefix = std::string("\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\basic_interface\\btn_") + skins[state];
    const auto drawBitmap = [&](const char* suffix, int x) {
      auto* bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load((prefix + suffix).c_str()));
      this->DrawBitmap(x, 0, bitmap ? &bitmap->image : nullptr, false);
    };
    drawBitmap("_left.bmp", 0);
    const int count = (this->geometry.width - 12) / 6 + 1;

    for (int index = 1, x = 6; index <= count; ++index, x = (x + 6)) {
      drawBitmap("_mid.bmp", index == count ? (this->geometry.width - 12) : x);
    }

    drawBitmap("_right.bmp", (this->geometry.width - 6));
    const int width = this->TextWidth(this->m_buttonContext->fonts, this->m_buttonContext->locale, this->text.c_str(), 0, 0, 12, 0, 0);
    int height;

    {
      TextContext context(*this->geometry.surface, this->m_buttonContext->fonts, this->m_buttonContext->locale);
      context.SetFont(0, 12, 0, 0);
      height = context.Measure(this->text.c_str(), static_cast<int>(std::strlen(this->text.c_str()))).cy;
    }

    const int x = (this->geometry.width - width) / 2;
    const int y = (this->geometry.height - height) / 2;
    const bool pressed = state == 2 || state == 3;
    const unsigned shadow = state == 0 ? 0xD7D7D7u : state == 1 ? 0xC7A796u : state == 2 ? 0xFFFFFFu : 0xE2E2E2u;
    this->DrawTextAt(
      this->m_buttonContext->fonts, this->m_buttonContext->locale, (x + (pressed ? 0 : 1)), (y + (pressed ? 0 : 1)), this->text.c_str(), 0, 0, 12, shadow, 0, 0
    );
    this->DrawTextAt(
      this->m_buttonContext->fonts, this->m_buttonContext->locale, (x + (pressed ? 1 : 0)), (y + (pressed ? 1 : 0)), this->text.c_str(), 0, 0, 12,
      state == 3 ? 0x6F6F6Fu : 0, 0, 0
    );
  }

  if (this->invalidateParent == 1 && this->parent) {
    static_cast<UIWindow*>(this->parent)->Invalidate();
  }
}
