#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBITMAPBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBITMAPBUTTON_H

#include "UI/Buttons/UIButton.h"

class UIBitmapButton : public UIButton {
  friend class UIAdvenPartyBoardWnd;
  friend class UIAchViewWnd;

public:
  UIBitmapButton();
  UIBitmapButton(UIWindowContext& windowContext, UIButtonContext& buttonContext);
  ~UIBitmapButton() override;
  void Paint() override;
  bool SetBitmap(const char* name, int state, unsigned char exactSize);
  // INLINED
  int BitmapWidth() const {
    return this->m_bitmapWidth;
  }
  // INLINED
  int BitmapHeight() const {
    return this->m_bitmapHeight;
  }

protected:
  /* 00B4 */ int m_bitmapWidth;
  /* 00B8 */ int m_bitmapHeight;
  /* 00BC */ std::string m_names[4];
  /* 011C */ unsigned char m_invalidateParent;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIBitmapButton) == 296);
#endif

#endif
