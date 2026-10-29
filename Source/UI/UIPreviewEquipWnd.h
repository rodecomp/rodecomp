#ifndef RAGNAROK_SOURCE_UI_UIPREVIEWEQUIPWND_H
#define RAGNAROK_SOURCE_UI_UIPREVIEWEQUIPWND_H

#include "UI/Window/UIFrameWnd.h"

class CCharacterRender;
struct PreviewEquipWndInfo;

class UIPreviewEquipWnd : public UIFrameWnd {
public:
  UIPreviewEquipWnd(UIWindowContext& windows, UIFrameContext& frames);
  ~UIPreviewEquipWnd() override;
  void Slot11() override;
  void Slot19() override;

  /* 00B4 */ PreviewEquipWndInfo* saved;
  /* 00B8 */ CCharacterRender* m_render;
  /* 00BC */ UIWindow* m_leftButton;
  /* 00C0 */ UIWindow* m_rightButton;
  /* 00C4 */ int unknown_00C4;
};

#endif
