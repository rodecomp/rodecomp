#include "UI/UIHotkeyGuideWnd.h"

#include "Contents/MapSkyMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"

// OFFSET: 0x0096C140
// FUNCTION: fn_13166
void UIHotkeyGuideWnd::Slot19() {
  auto tab = this->m_tabs.begin();
  for (; tab != this->m_tabs.end(); ++tab) {
    if ((*tab)->geometry.unknown_0030 == 2) {
      int id = (*tab)->geometry.id;
      if (id != this->m_current) {
        this->m_keys.clear();
        for (auto entry = this->m_entries.begin(); entry != this->m_entries.end(); ++entry) {
          if (id == entry->id) {
            if (entry->firstKey != 0) {
              this->m_keys.push_back(entry->firstKey);
              if (entry->secondKey != 0) {
                this->m_keys.push_back(entry->secondKey);
              }
            }
            break;
          }
        }
        this->m_current = id;
        this->Invalidate();
      }
      break;
    }
  }
  if (tab == this->m_tabs.end() && this->m_current != 4698) {
    this->m_keys.clear();
    this->m_current = 4698;
    this->Invalidate();
  }
}

// OFFSET: 0x0096C5A0
// FUNCTION: fn_13173
void UIHotkeyGuideWnd::DrawKey(const HotkeyGuideKey* key) {
  CBitmapRes* bitmap;
  int x;
  if (key->width == 36) {
    const char* name = key->height != 75 ? "유저인터페이스\\hotkey_guide\\bt_keyboard.bmp" : "유저인터페이스\\hotkey_guide\\bt_enter2.bmp";
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
    x = key->x;
  } else {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_left.bmp")));
    this->DrawBitmap(key->x, key->y, &bitmap->image, true);
    int offset = 6;
    for (int i = 1; i <= (key->width - 12) / 6; ++i) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_mid.bmp")));
      this->DrawBitmap(key->x + offset, key->y, &bitmap->image, true);
      offset += 6;
    }
    if ((key->width - 12) % 6 != 0) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_mid.bmp")));
      this->DrawBitmap(key->x + (key->width - 12), key->y, &bitmap->image, true);
    }
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_right.bmp")));
    x = key->x + (key->width - 6);
  }
  this->DrawBitmap(x, key->y, &bitmap->image, true);
}

// OFFSET: 0x0096C730
// FUNCTION: fn_13174
void UIHotkeyGuideWnd::DrawKeyLabel(std::string text, const HotkeyGuideKey* key) {
  UIButtonContext& controls = *this->m_controls;
  int x = key->x;
  if (key->align == 6) {
    int textWidth = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 12, 0, 0);
    x = static_cast<int>(static_cast<float>(key->width - textWidth) * g_depthAverageHalf + static_cast<float>(x));
  } else if (key->align == 0) {
    x += key->padding;
  } else if (key->align == 2) {
    int textWidth = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 12, 0, 0);
    x += key->width - key->padding - textWidth;
  }
  int y = static_cast<int>(static_cast<double>(key->height - 12) * 0.5 + static_cast<double>(key->y));
  this->DrawTextAt(controls.fonts, controls.locale, x, y, text.c_str(), 0, 0, 12, 0x2B2625, 0, 0);
}
