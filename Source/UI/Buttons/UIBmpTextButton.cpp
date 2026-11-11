#include "Render/Text/FontCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBmpTextButton.h"

#include <cstring>

// OFFSET: 0x0084ED30
// FUNCTION: fn_10652
UIBmpTextButton::UIBmpTextButton() : UIBitmapButton() {
}

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
  this->ClearSurface(0xFFFF00FF);

  bool found = false;
  std::vector<BitmapButtonTextStyle>::iterator style = this->styles.begin();

  for (; style != this->styles.end(); ++style) {
    if (this->m_disabled == 0) {
      if (this->geometry.state == style->state) {
        found = true;
        break;
      }
    } else if (this->m_disabled == 1 && style->state == 3) {
      found = true;
      break;
    }
  }

  if (this->m_disabled == 1) {
    this->DrawBitmap(0, 0, dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[3].c_str()))), false);

    if (this->text.size() > 0) {
      if (found) {
        if (style->shadow == 1) {
          this->DrawTextAt(style->x, style->y, this->text.c_str(), 0, 0, style->height, 0xE2E2E2, 0, 0);
        }

        this->DrawTextAt(style->x + 1, style->y + 1, this->text.c_str(), 0, 0, style->height, style->color, style->bold, 0);
      } else {
        const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
        const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
        this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0xE2E2E2, 0, 0);
        this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0x6F6F6F, 0, 0);
      }
    }
  } else {
    switch (this->geometry.state) {
    case 0: {
      this->DrawBitmap(0, 0, dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[0].c_str()))), false);

      if (this->text.size() > 0) {
        if (found) {
          if (style->shadow == 1) {
            this->DrawTextAt(style->x + 1, style->y + 1, this->text.c_str(), 0, 0, style->height, 0xD7D7D7, style->bold, 0);
          }

          this->DrawTextAt(style->x, style->y, this->text.c_str(), 0, 0, style->height, style->color, style->bold, 0);
        } else {
          const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
          const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
          this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0xD7D7D7, 0, 0);
          this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0, 0, 0);
        }
      }
      break;
    }
    case 1: {
      this->DrawBitmap(0, 0, dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[1].c_str()))), false);

      if (this->text.size() > 0) {
        if (found) {
          if (style->shadow == 1) {
            this->DrawTextAt(style->x + 1, style->y + 1, this->text.c_str(), 0, 0, style->height, 0xC7A796, style->bold, 0);
          }

          this->DrawTextAt(style->x, style->y, this->text.c_str(), 0, 0, style->height, style->color, style->bold, 0);
        } else {
          const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
          const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
          this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0xC7A796, 0, 0);
          this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0, 0, 0);
        }
      }
      break;
    }
    case 2: {
      this->DrawBitmap(0, 0, dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[2].c_str()))), false);

      if (this->text.size() > 0) {
        if (found) {
          if (style->shadow == 1) {
            this->DrawTextAt(style->x, style->y, this->text.c_str(), 0, 0, style->height, 0xFFFFFF, style->bold, 0);
          }

          this->DrawTextAt(style->x + 1, style->y + 1, this->text.c_str(), 0, 0, style->height, style->color, style->bold, 0);
        } else {
          const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
          const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->text.c_str(), 0, 0, 12, 0, 0)) * 0.5);
          this->DrawTextAt(x, y, this->text.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
          this->DrawTextAt(x + 1, y + 1, this->text.c_str(), 0, 0, 12, 0, 0, 0);
        }
      }
      break;
    }
    }
  }

  if (this->m_invalidateParent == 1 && this->parent) {
    this->parent->Invalidate();
  }
}
