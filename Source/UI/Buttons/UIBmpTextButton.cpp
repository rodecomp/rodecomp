#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Buttons/UIBmpTextButton.h"

#include <cstring>

// OFFSET: 0x0084ED30
// FUNCTION: fn_10652
UIBmpTextButton::UIBmpTextButton(UIWindowContext& windows, UIButtonContext& controls) : UIBitmapButton(windows, controls) {
}

// OFFSET: 0x00852360
UIBmpTextButton::~UIBmpTextButton() = default;

// OFFSET: 0x007C2340
// FUNCTION: fn_09323
void UIBmpTextButton::SetText(std::string value) {
  this->text = value;
  this->Invalidate();
}

// OFFSET: 0x0085BDE0
// FUNCTION: fn_10799
void UIBmpTextButton::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, 0xFFFF00FFu);
  const BitmapButtonTextStyle* style = nullptr;

  for (const auto& candidate : this->styles) {
    if ((this->m_disabled == 0 && candidate.state == this->geometry.unknown_0030) || (this->m_disabled == 1 && candidate.state == 3)) {
      style = &candidate;
      break;
    }
  }

  const unsigned state = this->geometry.unknown_0030;
  const bool disabled = this->m_disabled == 1;

  if (disabled || state <= 2) {
    const char* path = this->m_names[disabled ? 3 : state].c_str();
    auto* resource = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(path));
    this->DrawBitmap(0, 0, resource ? &resource->image : nullptr, false);

    if (!this->text.empty()) {
      const auto draw = [&](int x, int y, int height, unsigned color, unsigned char bold) {
        this->DrawTextAt(this->m_buttonContext->fonts, this->m_buttonContext->locale, x, y, this->text.c_str(), 0, 0, height, color, bold, 0);
      };

      if (style) {
        const unsigned shadowColor = disabled ? 0xE2E2E2u : state == 0 ? 0xD7D7D7u : state == 1 ? 0xC7A796u : 0xFFFFFFu;
        const int offset = !disabled && (state == 0 || state == 1) ? 1 : 0;

        if (style->shadow == 1) {
          draw((style->x + offset), (style->y + offset), style->height, shadowColor, disabled ? 0 : style->bold);
        }

        const int pressedOffset = disabled || state == 2 ? 1 : 0;
        draw((style->x + pressedOffset), (style->y + pressedOffset), style->height, style->color, style->bold);
      } else {
        const int width = this->TextWidth(this->m_buttonContext->fonts, this->m_buttonContext->locale, this->text.c_str(), 0, 0, 12, 0, 0);
        int height;

        {
          TextContext context(*this->geometry.surface, this->m_buttonContext->fonts, this->m_buttonContext->locale);
          context.SetFont(0, 12, 0, 0);
          height = context.Measure(this->text.c_str(), static_cast<int>(std::strlen(this->text.c_str()))).cy;
        }

        const int x = (this->geometry.width - width) / 2;
        const int y = (this->geometry.height - height) / 2;
        const bool pressed = disabled || state == 2;
        const unsigned shadowColor = disabled ? 0xE2E2E2u : state == 0 ? 0xD7D7D7u : state == 1 ? 0xC7A796u : 0xFFFFFFu;
        draw((x + (pressed ? 0 : 1)), (y + (pressed ? 0 : 1)), 12, shadowColor, 0);
        draw((x + (pressed ? 1 : 0)), (y + (pressed ? 1 : 0)), 12, disabled ? 0x6F6F6Fu : 0, 0);
      }
    }
  }

  if (this->m_invalidateParent == 1 && this->parent) {
    this->parent->Invalidate();
  }
}
