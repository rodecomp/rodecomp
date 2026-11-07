#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H

#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>
#include <vector>

struct UIBmpRadioBtnText {
  /* 0000 */ int id;
  /* 0004 */ int x;
  /* 0008 */ int y;
  /* 000C */ int height;
  /* 0010 */ bool bold;
  /* 0014 */ std::uint32_t color;
  /* 0018 */ bool unknown_0018;
};

class UIBmpRadioBtn : public UIWindow {
  friend class UICashShopWnd; friend class UINewMakeCharWnd;
  friend class UIRodexInboxWnd;

public:
  UIBmpRadioBtn();
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  // INLINED
  void OnMouseEnter() override {
    this->m_hovered = true;
    this->Invalidate();
  }
  // INLINED
  void OnMouseLeave() override {
    this->m_hovered = false;
    this->Invalidate();
  }

  bool SetBitmap(const char* name, int state);
  void AddText(int id, int height, std::uint32_t color, bool bold, bool unknown, int x, int y);

protected:
  /* 007C */ std::string m_names[4];
  /* 00DC */ bool m_hovered;
  /* 00DD */ unsigned char unknown_00DD[3];
  /* 00E0 */ std::string m_tooltip;
  /* 00F8 */ bool m_disabled;
  /* 00FC */ int m_value;
  /* 0100 */ std::vector<UIBmpRadioBtnText> m_texts;
  /* 010C */ std::string m_label;
};

class UIBmpRadioBtn2 : public UIBmpRadioBtn {
public:
  // INLINED
  UIBmpRadioBtn2() : UIBmpRadioBtn() {
  }
  void Invalidate() override;
  void OnMouseEnter() override;
  void OnMouseLeave() override;
};

class UICaptchaImage : public UIWindow {
public:
  UICaptchaImage();
  ~UICaptchaImage() override;
  void Paint() override;
  void OnMouseHover(int x, int y) override;

  /* 007C */ CBitmapRes m_bitmap;

protected:
  /* 01A0 */ std::string m_tooltip;
};

#endif
