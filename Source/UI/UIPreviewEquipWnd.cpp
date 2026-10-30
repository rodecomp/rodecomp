#include "UI/UIPreviewEquipWnd.h"

#include "Contents/Actors/CharacterRender.h"
#include "Contents/UserData/NormalItemSlots.h"

// OFFSET: 0x007F2F90
// FUNCTION: fn_09879
UIPreviewEquipWnd::UIPreviewEquipWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 1), saved(nullptr), m_render(nullptr), m_leftButton(nullptr), m_rightButton(nullptr), unknown_00C4(0) {
}

// OFFSET: 0x007F3090
// FUNCTION: fn_09880
UIPreviewEquipWnd::~UIPreviewEquipWnd() {
  delete this->m_render;
}

// OFFSET: 0x007F4830
// FUNCTION: fn_09888
void UIPreviewEquipWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007F3DB0
// FUNCTION: fn_09884
void UIPreviewEquipWnd::Slot19() {
  this->Invalidate();
}
