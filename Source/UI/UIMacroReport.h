#ifndef RAGNAROK_SOURCE_UI_UIMACROREPORT_H
#define RAGNAROK_SOURCE_UI_UIMACROREPORT_H

#include "UI/FrameSkin.h"

#include <string>

class CUIControl;
class CUIEditBox;
class CUIImageButton;
class CUIRadioButton;
class CUIStaticImage;
class CUIStaticText;
class CUITitleBar;

class CUIMacroReport : public CUIFrameWnd {
public:
  /* 00D8 */ CUIStaticImage* m_background;
  /* 00DC */ CUITitleBar* m_titleBar;
  /* 00E0 */ CUIEditBox* m_messageEdit;
  /* 00E4 */ CUIStaticText* unknown_00E4;
  /* 00E8 */ CUIStaticText* unknown_00E8;
  /* 00EC */ CUIImageButton* unknown_00EC;
  /* 00F0 */ CUIImageButton* unknown_00F0;
  /* 00F4 */ CUIRadioButton* m_firstTypeButton;
  /* 00F8 */ CUIRadioButton* m_secondTypeButton;
  /* 00FC */ CUIStaticText* unknown_00FC;
  /* 0100 */ int unknown_0100;
  /* 0104 */ int m_reportType;
  /* 0108 */ std::string unknown_0108;

  __declspec(noinline) CUIMacroReport();

  void Slot15(int width, int height) override;
  void Slot16() override;
  virtual void Reset();

  void OnButtonClick();
  void OnEditSubmit();
  void SendReport();
  void OnReportTypeSelect(CUIRadioButton* button);
};

#endif
