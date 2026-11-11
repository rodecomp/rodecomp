#ifndef RAGNAROK_SOURCE_UI_LOGIN_UIWAITDBSAVEWND_H
#define RAGNAROK_SOURCE_UI_LOGIN_UIWAITDBSAVEWND_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

class CActRes;
class CSprRes;

class UIWaitDBSaveWnd : public UIWindow {
public:
  UIWaitDBSaveWnd();
  UIWaitDBSaveWnd(const char* sprite, const char* action);
  void OnCreate(int width, int height) override;
  void OnDestroy() override;
  void OnProcess() override;
  void Paint() override;
  void OnMouseMove(int x, int y) override;

  /* 007C */ int m_animation;
  /* 0080 */ int m_frame;
  /* 0084 */ unsigned int m_loopLimit;
  /* 0088 */ CSprRes* m_sprite;
  /* 008C */ CActRes* m_action;
  /* 0090 */ std::uint32_t m_startTime;
  /* 0094 */ unsigned int m_loop;
  /* 0098 */ std::uint32_t m_closeTime;
};

#endif
