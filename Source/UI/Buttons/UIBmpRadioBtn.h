#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H

#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>
#include <vector>

struct UIButtonContext;

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
  friend class UICashShopWnd;

public:
  UIBmpRadioBtn(UIWindowContext& windows, UIButtonContext& controls);
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  bool SetBitmap(const char* name, int state);
  void AddText(int id, int height, std::uint32_t color, bool bold, bool unknown, int x, int y);

protected:
  /* 007C */ std::string m_names[4];
  /* 00DC */ bool m_hovered;
  /* 00DD */ unsigned char unknown_00DD[3];
  /* 00E0 */ std::string m_tooltip;
  UIButtonContext* m_controls;
  /* 00FC */ int m_value;
  /* 0100 */ std::vector<UIBmpRadioBtnText> m_texts;
  /* 010C */ std::string m_label;
  bool m_disabled;
};

class UIBmpRadioBtn2 : public UIBmpRadioBtn {
public:
  void Slot47() override;
  void Slot48() override;
};

class UICaptchaImage : public UIWindow {
public:
  void Paint() override;
  void Slot28(int x, int y) override;

  /* 007C */ CBitmapRes m_bitmap;

protected:
  /* 01A0 */ std::string m_tooltip;
  UIButtonContext* m_controls;
};

#endif
