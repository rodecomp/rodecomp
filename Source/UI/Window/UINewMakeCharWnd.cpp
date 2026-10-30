#include "UI/Window/UINewMakeCharWnd.h"
#include <cstdio>
#include <cstring>

// OFFSET: 0x007DB320
// FUNCTION: fn_09617
void UIHairBitmap::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x, y);
  }
}

// OFFSET: 0x007DB300
// FUNCTION: fn_09616
void UIHairBitmap::Slot26(int x, int y) {
  if (this->parent) {
    this->parent->Slot26(x, y);
  }
}

// OFFSET: 0x007DB380
// FUNCTION: fn_09620
void UIHairBitmap::Slot27(int x, int y) {
  if (this->parent) {
    this->parent->Slot27(x, y);
  }
}

// OFFSET: 0x007DB360
// FUNCTION: fn_09619
void UIHairBitmap::Slot28(int x, int y) {
  if (this->parent) {
    this->parent->Slot28(x, y);
  }
}

// OFFSET: 0x007DB3A0
// FUNCTION: fn_09621
void UIHairBitmap::Slot29(int x, int y) {
  if (this->parent) {
    this->parent->Slot29(x, y);
  }
}

// OFFSET: 0x007DB3C0
// FUNCTION: fn_09622
void UIHairBitmap::ResetCursor(int x, int y) {
  if (this->parent) {
    this->parent->ResetCursor(x, y);
  }
}

// OFFSET: 0x007DB340
// FUNCTION: fn_09618
void UIHairBitmap::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x, y);
  }
}

// OFFSET: 0x007DB3E0
// FUNCTION: fn_09623
void UINewMakeCharWnd::Slot19() {
  this->m_raceViews[this->m_race]->Invalidate();
}

// OFFSET: 0x007DB400
// FUNCTION: fn_09624
int UIHairStyleButton::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 13: {
    bool previous = this->m_selected;
    this->m_selected = argument1 != 0;
    if (previous != (argument1 != 0)) {
      if (argument1) {
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 0, 0);
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 1, 0);
      } else {
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_normal.bmp", 0, 0);
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_over.bmp", 1, 0);
      }
      this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 2, 0);
      this->Invalidate();
    }
    break;
  }
  case 40: {
    char name[128];
    memset(name, 0, sizeof(name));
    if (this->m_race == 0) {
      if (this->m_style > 22) {
        if (this->m_preview) {
          this->m_preview->Enable(0);
          this->InvalidateChildren();
        }
        return 0;
      }
      this->m_preview->Enable(1);
      sprintf_s(
        name, this->m_sex == 0 ? "유저인터페이스\\make_character_ver2\\img_hairStyle_girl%02d.bmp" : "유저인터페이스\\make_character_ver2\\img_hairStyle%02d.bmp",
        this->m_style + 1
      );
    } else if (this->m_race == 1) {
      if (this->m_style > 5) {
        if (this->m_preview) {
          this->m_preview->Enable(0);
          this->InvalidateChildren();
        }
        return 0;
      }
      this->m_preview->Enable(1);
      sprintf_s(
        name,
        this->m_sex == 0 ? "유저인터페이스\\make_character_ver2\\img_hairStyle_doramGirl%02d.bmp" : "유저인터페이스\\make_character_ver2\\img_hairStyle_doramBoy%02d.bmp",
        this->m_style + 1
      );
    }
    if (this->m_preview) {
      this->m_preview->SetBitmap(name, 0, 0);
      this->m_preview->SetBitmap(name, 1, 0);
      this->m_preview->SetBitmap(name, 2, 0);
      this->m_preview->SetBitmap("유저인터페이스\\make_character_ver2\\img_hairStyle_none.bmp", 3, 0);
    }
    this->InvalidateChildren();
    break;
  }
  default:
    UIBitmapButton::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    break;
  }
  return 0;
}
