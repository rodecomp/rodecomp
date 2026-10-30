#ifndef RAGNAROK_SOURCE_UI_UICAPTCHAREGISTERWND_H
#define RAGNAROK_SOURCE_UI_UICAPTCHAREGISTERWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIStaticText;
class UIComboBoxDefWnd;
class UIStaticBitmap;
class UIEditCtrl;
class UIBitmapButton;
struct UIButtonContext;

class UICaptchaRegisterWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) override;
  void SetRegisterDisabled(unsigned char disabled);

  void sub_8CEF10();
  void sub_8CF0C0(const char* tooltip, int width);

  /* 00B4 */ UIStaticText* m_pathText;
  /* 00B8 */ UIComboBoxDefWnd* m_fileComboBox;
  /* 00BC */ UIStaticBitmap* m_captchaBitmap;
  /* 00C0 */ UIEditCtrl* m_editCtrl;
  /* 00C4 */ UIBitmapButton* registerButton;
  /* 00C8 */ std::string m_selectedFile;
  /* 00E0 */ void* m_decompressedData;
  /* 00E4 */ unsigned int m_decompressedSize;

protected:
  /* 00E8 */ UIButtonContext* m_controls;
};

#endif
