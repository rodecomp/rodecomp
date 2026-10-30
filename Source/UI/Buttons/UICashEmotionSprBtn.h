#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UICASHEMOTIONSPRBTN_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UICASHEMOTIONSPRBTN_H

#include "UI/Buttons/UISpriteButton.h"

class UICashEmotionSprBtn : public UISpriteButton2 {
  friend class UICashEmotionPiece;

public:
  UICashEmotionSprBtn(UIWindowContext& windowContext, UIButtonContext& buttonContext, UIWindow* target);
  void Slot28(int x, int y) override;

  void SetSelected(bool selected);

protected:
  /* 0118 */ bool m_selected;
};

#endif
