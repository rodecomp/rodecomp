#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/WindowBorder.h"

#include <algorithm>

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
  if (!window || !this->owner) {
    return false;
  }

  return this->owner->m_children.end() != std::find(this->owner->m_children.begin(), this->owner->m_children.end(), window);
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
  this->DrawRectOutline(0, 0, this->geometry.width, this->geometry.height, border);
}

