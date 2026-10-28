#include "UI/FrameSkin.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/WindowBorder.h"

// OFFSET: 0x009E6C70
// FUNCTION: fn_14470
void CUIFrameWnd::RefreshSkinColors() {
  if (!this->m_resolveSkin) {
    return;
  }

  if (this->m_hasSkinColors) {
    this->m_backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), *this->m_textureNames, this->m_backgroundCell, this->m_resolveSkin);
    this->m_borderColor = SkinPaletteColor(ResourceMgr::Instance(), *this->m_textureNames, this->m_borderCell, this->m_resolveSkin);
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
    this->DrawButtonBar(*this->m_textureNames, this->m_buttonBarStyle - 1, 0, 1);
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

  this->m_backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), *this->m_textureNames, this->m_backgroundCell, this->m_resolveSkin);
  this->m_borderColor = SkinPaletteColor(ResourceMgr::Instance(), *this->m_textureNames, this->m_borderCell, this->m_resolveSkin);
}
