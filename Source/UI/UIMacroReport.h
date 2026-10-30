#ifndef RAGNAROK_SOURCE_UI_UIMACROREPORT_H
#define RAGNAROK_SOURCE_UI_UIMACROREPORT_H

#include "UI/FrameSkin.h"

#include <string>

class CUIEditBox;
class CUIRadioButton;

class CUIMacroReport : public CUIFrameWnd {
public:
  /* 00D8 */ unsigned char unknown_00D8[0x08];
  /* 00E0 */ CUIEditBox* m_messageEdit;
  /* 00E4 */ unsigned char unknown_00E4[0x10];
  /* 00F4 */ CUIRadioButton* m_firstTypeButton;
  /* 00F8 */ CUIRadioButton* m_secondTypeButton;
  /* 00FC */ unsigned char unknown_00FC[0x04];
  /* 0100 */ int unknown_0100;
  /* 0104 */ int m_reportType;
  /* 0108 */ std::string unknown_0108;

  void Slot16() override;
  virtual void Reset();

  void OnButtonClick();
  void OnEditSubmit();
  void SendReport();
  void OnReportTypeSelect(CUIRadioButton* button);
};

#endif
