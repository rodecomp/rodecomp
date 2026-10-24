#include "Render/DeviceContext.h"
#include "UI/UIDropDownBox.h"
#include "UI/WindowBorder.h"
#include "UI/WindowOrdering.h"

#include <algorithm>

// OFFSET: 0x00AA27B0
// FUNCTION: fn_17217
CUIDropDownBox::CUIDropDownBox(UIWindowContext& context) : CUIControl(context), owner(nullptr) {
}

// OFFSET: 0x009E3300
// FUNCTION: fn_45427
// OFFSET: 0x009E3390
// FUNCTION: fn_14415
CUIDropDownBox::~CUIDropDownBox() {
  this->Detach();
}

// OFFSET: 0x009E3430
// FUNCTION: fn_14416
void CUIDropDownBox::Detach() {
  if (!this->m_children.empty()) {
    this->ClearChildren(false);
    this->owner = nullptr;
    this->Invalidate();
  }
}

// OFFSET: 0x009E3460
// FUNCTION: fn_14417
bool CUIDropDownBox::HasChild(const UIWindow* window) const {
  return std::find(this->m_children.begin(), this->m_children.end(), window) != this->m_children.end();
}

// OFFSET: 0x009E3490
// FUNCTION: fn_14418
bool CUIDropDownBox::HasOwnerChild(const UIWindow* window) const {
  return window && this->owner &&
         std::find(this->owner->m_children.begin(), this->owner->m_children.end(), window) !=
           this->owner->m_children.end();
}

// OFFSET: 0x009E3500
// FUNCTION: fn_14420
void CUIDropDownBox::Attach(UIWindow* value, UIWindow* child) {
  if (this->HasChild(child)) {
    return;
  }

  this->Detach();
  this->Resize(child->geometry.width + 2, child->geometry.height + 2);
  this->AddChild(child);
  this->owner = value;
  this->Invalidate();
}

// OFFSET: 0x009E34D0
// FUNCTION: fn_14419
void CUIDropDownBox::Paint() {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, 0xFFFFFFFFu);
  }

  DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0x00C6C6C6u);
}

// OFFSET: 0x00AA27B0
// FUNCTION: fn_17217
void WindowDropDown::Show(UIWindow* owner, UIWindow* child, int x, int y) {
  if (this->popup && this->popup->geometry.visible != 0) {
    this->popup->Detach();
    this->popup->Show(0);
  }

  if (!this->popup) {
    this->popup = new CUIDropDownBox(this->windows);
  }

  this->popup->Attach(owner, child);
  this->popup->Move(x, y);

  auto& rendered = this->ordering.rendered;
  if (std::find(rendered.begin(), rendered.end(), this->popup) == rendered.end()) {
    rendered.push_back(this->popup);
  } else {
    this->ordering.Promote(this->popup);
  }

  this->popup->Show(1);
}

// OFFSET: 0x00A8C750
// FUNCTION: fn_17133
bool WindowDropDown::Close() {
  if (!this->popup || this->popup->geometry.visible == 0) {
    return false;
  }

  this->popup->Detach();
  this->popup->Show(0);
  return true;
}
