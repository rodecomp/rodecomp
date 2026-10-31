#ifndef RAGNAROK_SOURCE_UI_FRAMESKIN_H
#define RAGNAROK_SOURCE_UI_FRAMESKIN_H

#include "Resource/SkinPalette.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class CUIFrameWnd : public UIFrameWnd {
public:
  // INLINED
  CUIFrameWnd()
      : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0), unknown_00B4(0), m_hasSkinColors(false),
        m_backgroundColor(0xFFFFFF), m_borderColor(0xCECECE), m_backgroundCell{0, 0}, m_borderCell{3, 1}, m_buttonBarStyle(0), m_resolveSkin(true) {
  }

  ~CUIFrameWnd() override;
  void Paint() override;
  void RefreshSkinColors();
  void SetSkinColors(int backgroundColumn, int backgroundRow, int borderColumn, int borderRow);

protected:
  /* 00B4 */ std::uint16_t unknown_00B4;
  /* 00B6 */ bool m_hasSkinColors;
  /* 00B8 */ std::uint32_t m_backgroundColor;
  /* 00BC */ std::uint32_t m_borderColor;
  /* 00C0 */ SkinPaletteCell m_backgroundCell;
  /* 00C8 */ SkinPaletteCell m_borderCell;
  /* 00D0 */ int m_buttonBarStyle;
  /* 00D4 */ bool m_resolveSkin;
};

#endif
