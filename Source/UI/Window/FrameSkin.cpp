#include "UI/Window/FrameSkin.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/WindowBorder.h"

#include <new>
#include <utility>

// GLOBAL: gl_25106
template <>
CUIRegisterMgr* Singleton<CUIRegisterMgr>::s_instance = nullptr;

// OFFSET: 0x009E69D0
// FUNCTION: fn_14469
CUIFrameWnd::~CUIFrameWnd() {
  if (this->m_ownsBackgroundColor) {
    if (CUIRegisterMgr::s_instance == nullptr) {
      CUIRegisterMgr::s_instance = new (std::nothrow) CUIRegisterMgr;
    }
    CUIRegisterMgr::s_instance->windowPositions[static_cast<int>(this->geometry.id)] = POINT{this->geometry.x, this->geometry.y};
  }

  if (this->m_ownsBorderColor) {
    if (CUIRegisterMgr::s_instance == nullptr) {
      CUIRegisterMgr::s_instance = new (std::nothrow) CUIRegisterMgr;
    }
    CUIRegisterMgr::s_instance->windowSizes[static_cast<int>(this->geometry.id)] = SIZE{this->geometry.width, this->geometry.height};
  }
}

// OFFSET: 0x009E6C70
// FUNCTION: fn_14470
void CUIFrameWnd::RefreshSkinColors() {
  if (!this->m_resolveSkin) {
    return;
  }

  if (this->m_hasSkinColors) {
    this->m_backgroundColor = g_skinInfo.PaletteColor(this->m_backgroundCell, this->m_resolveSkin);
    this->m_borderColor = g_skinInfo.PaletteColor(this->m_borderCell, this->m_resolveSkin);
  }

  for (UIWindow* child : this->m_children) {
    child->ApplySkin(this->m_resolveSkin);
  }
}

// OFFSET: 0x009E6CF0
// FUNCTION: fn_14471
void CUIFrameWnd::Paint() {
  if (this->m_hasSkinColors) {
    this->ClearSurface(0xFFFF00FF);
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, this->m_backgroundColor);
    this->DrawRoundedOutline(0, 0, this->geometry.width, this->geometry.height, this->m_borderColor);
  }

  if (this->m_buttonBarStyle > 0) {
    this->DrawButtonBar(this->m_buttonBarStyle - 1, 0, 1);
  }
}

// OFFSET: 0x009E6D60
// FUNCTION: fn_14472
void CUIFrameWnd::SetSkinColors(int backgroundColumn, int backgroundRow, int borderColumn, int borderRow) {
  this->m_hasSkinColors = true;
  this->m_backgroundCell = SkinPaletteCell{backgroundColumn, backgroundRow};
  this->m_borderCell = SkinPaletteCell{borderColumn, borderRow};

  this->m_backgroundColor = g_skinInfo.PaletteColor(this->m_backgroundCell, this->m_resolveSkin);
  this->m_borderColor = g_skinInfo.PaletteColor(this->m_borderCell, this->m_resolveSkin);
}
