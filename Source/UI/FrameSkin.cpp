#include "UI/FrameSkin.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/WindowBorder.h"

#include <new>
#include <utility>

// GLOBAL: gl_25106
template <>
CUIRegisterMgr* Singleton<CUIRegisterMgr>::s_instance = nullptr;

__declspec(noinline) CUIRegisterMgr::CUIRegisterMgr() {
  InitializeCriticalSection(&this->lock);
}

// OFFSET: 0x009E69D0
// FUNCTION: fn_14469
CUIFrameWnd::~CUIFrameWnd() {
  if (static_cast<unsigned char>(this->unknown_00B4) != 0) {
    if (CUIRegisterMgr::s_instance == nullptr) {
      CUIRegisterMgr::s_instance = new (std::nothrow) CUIRegisterMgr;
    }
    std::map<int, std::pair<int, int>>& positions = CUIRegisterMgr::s_instance->unknown_000C;
    std::map<int, std::pair<int, int>>::iterator position = positions.lower_bound(this->geometry.id);
    if (position == positions.end() || this->geometry.id < position->first) {
      position = positions.insert(position, std::map<int, std::pair<int, int>>::value_type(this->geometry.id, std::pair<int, int>()));
    }
    position->second.first = this->geometry.x;
    position->second.second = this->geometry.y;
  }

  if (static_cast<unsigned char>(this->unknown_00B4 >> 8) != 0) {
    if (CUIRegisterMgr::s_instance == nullptr) {
      CUIRegisterMgr::s_instance = new (std::nothrow) CUIRegisterMgr;
    }
    std::map<int, std::pair<int, int>>& sizes = CUIRegisterMgr::s_instance->unknown_0014;
    std::map<int, std::pair<int, int>>::iterator size = sizes.lower_bound(this->geometry.id);
    if (size == sizes.end() || this->geometry.id < size->first) {
      size = sizes.insert(size, std::map<int, std::pair<int, int>>::value_type(this->geometry.id, std::pair<int, int>()));
    }
    size->second.first = this->geometry.width;
    size->second.second = this->geometry.height;
  }
}

// OFFSET: 0x009E6C70
// FUNCTION: fn_14470
void CUIFrameWnd::RefreshSkinColors() {
  if (!this->m_resolveSkin) {
    return;
  }

  if (this->m_hasSkinColors) {
    this->m_backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), this->m_backgroundCell, this->m_resolveSkin);
    this->m_borderColor = SkinPaletteColor(ResourceMgr::Instance(), this->m_borderCell, this->m_resolveSkin);
  }

  for (UIWindow* child : this->m_children) {
    child->Slot52(this->m_resolveSkin);
  }
}

// OFFSET: 0x009E6CF0
// FUNCTION: fn_14471
void CUIFrameWnd::Paint() {
  if (this->m_hasSkinColors) {
    this->ClearSurface(0xFFFF00FF);
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, this->m_backgroundColor);
    DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, this->m_borderColor);
  }

  if (this->m_buttonBarStyle > 0) {
    this->DrawButtonBar(this->m_buttonBarStyle - 1, 0, 1);
  }
}

// OFFSET: 0x009E6D60
// FUNCTION: fn_14472
void CUIFrameWnd::SetSkinColors(int backgroundColumn, int backgroundRow, int borderColumn, int borderRow) {
  this->m_backgroundCell.column = backgroundColumn;
  this->m_backgroundCell.row = backgroundRow;
  this->m_borderCell.column = borderColumn;
  this->m_borderCell.row = borderRow;
  this->m_hasSkinColors = true;

  this->m_backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), this->m_backgroundCell, this->m_resolveSkin);
  this->m_borderColor = SkinPaletteColor(ResourceMgr::Instance(), this->m_borderCell, this->m_resolveSkin);
}
