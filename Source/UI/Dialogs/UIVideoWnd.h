#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIVIDEOWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIVIDEOWND_H

#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>

class BinkMovie;
class CTexture;

class UIVideoSkipButton : public UIButton {
public:
  UIVideoSkipButton();

  void Paint() override;
};

class UIVideoWnd : public UIWindow {
public:
  UIVideoWnd();
  ~UIVideoWnd() override;
  bool IsFrameWnd() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ BinkMovie* m_movie;
  /* 0080 */ CTexture* m_texture;
  /* 0084 */ int m_savedVolume;
};

#endif
