#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"

#include <algorithm>

// INLINED
CUIDropDownBox::CUIDropDownBox() : CUIControl(), owner(nullptr) {
}

// OFFSET: 0x009E3300
// FUNCTION: fn_45427
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
  return this->m_children.end() != std::find(this->m_children.begin(), this->m_children.end(), window);
}

// OFFSET: 0x009E3490
// FUNCTION: fn_14418
bool CUIDropDownBox::HasOwnerChild(const UIWindow* window) const {
  return window && this->owner && std::find(this->owner->m_children.begin(), this->owner->m_children.end(), window) != this->owner->m_children.end();
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
  const std::uint32_t background = 0xFFFFFFFFu;
  this->ClearSurface(background);

  const std::uint32_t border = 0x00C6C6C6u;
  DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, border);
}

// OFFSET: 0x00AA27B0
// FUNCTION: fn_17217
void UIWindowMgr::ShowDropDown(UIWindow* owner, UIWindow* child, int x, int y) {
  CUIDropDownBox*& popup = reinterpret_cast<CUIDropDownBox*&>(this->window_0A30);
  if (popup && popup->geometry.visible != 0) {
    popup->Detach();
    popup->Show(0);
  }

  if (!popup) {
    popup = new CUIDropDownBox();
  }

  popup->Attach(owner, child);
  popup->Move(x, y);

  auto& rendered = this->rendered;
  if (std::find(rendered.begin(), rendered.end(), popup) == rendered.end()) {
    rendered.push_back(popup);
  } else {
    this->Promote(popup);
  }

  popup->Show(1);
}

// OFFSET: 0x00A8C750
// FUNCTION: fn_17133
bool UIWindowMgr::CloseDropDown() {
  CUIDropDownBox* popup = static_cast<CUIDropDownBox*>(this->window_0A30);
  if (!popup || popup->geometry.visible == 0) {
    return false;
  }

  popup->Detach();
  popup->Show(0);
  return true;
}
