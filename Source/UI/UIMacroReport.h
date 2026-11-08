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
  /* 00E4 */ CUIStaticText* firstTypeText;
  /* 00E8 */ CUIStaticText* secondTypeText;
  /* 00EC */ CUIImageButton* cancelButton;
  /* 00F0 */ CUIImageButton* reportButton;
  /* 00F4 */ CUIRadioButton* m_firstTypeButton;
  /* 00F8 */ CUIRadioButton* m_secondTypeButton;
  /* 00FC */ CUIStaticText* warningText;
  /* 0100 */ int targetId;
  /* 0104 */ int m_reportType;
  /* 0108 */ std::string targetName;

  __declspec(noinline) CUIMacroReport();

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Initialize() override;

  void OnButtonClick();
  void OnEditSubmit();
  void SendReport();
  void OnReportTypeSelect(CUIRadioButton* button);
};

#endif
