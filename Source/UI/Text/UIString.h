#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTRING_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTRING_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIString : public UIWindow {
public:
  UIString();
  void Paint() override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void ParseTags(std::string text);
  virtual void SetText(int width, int height, std::string text);

  /* 007C */ int unknown_007C;
  /* 0080 */ int enabled;
  /* 0084 */ std::string link;
  /* 009C */ int red;
  /* 00A0 */ int green;
  /* 00A4 */ int blue;
  /* 00A8 */ int fontSize;
  /* 00AC */ std::string image;
  /* 00C4 */ std::string fontName;
  /* 00DC */ std::string text;
};

#endif
