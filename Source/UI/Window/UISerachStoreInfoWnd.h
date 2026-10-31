#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISERACHSTOREINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISERACHSTOREINFOWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIBitmapButton;
class UIEditCtrl;
class UIRadioBtn;
class UITextButton;
struct UIButtonContext;
struct UIEditContext;

class UISerachStoreInfoWnd : public UIFrameWnd {
public:
  UISerachStoreInfoWnd();
  void Slot15(int width, int height) override;

protected:
  /* 00B4 */ UIRadioBtn* m_radios[2];
  /* 00BC */ UIEditCtrl* m_edits[4];
  /* 00CC */ UIBitmapButton* m_findButton;
  /* 00D0 */ UITextButton* m_prevButton;
  /* 00D4 */ UITextButton* m_nextButton;
  /* 00D8 */ UIWindow* unknown_00D8;
  /* 00DC */ int unknown_00DC;
  /* 00E0 */ int unknown_00E0;
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ unsigned char unknown_00EC;
  /* 00ED */ unsigned char unknown_00ED;
  /* 00F0 */ std::string unknown_00F0;

  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
};

#endif
