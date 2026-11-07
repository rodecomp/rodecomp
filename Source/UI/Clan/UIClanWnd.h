#ifndef RAGNAROK_SOURCE_UI_CLAN_UICLANWND_H
#define RAGNAROK_SOURCE_UI_CLAN_UICLANWND_H

#include "UI/Window/UIFrameWnd.h"

struct ClanWndInfo;
class CBitmapRes;
class UIStaticText;

class UIClanWnd : public UIFrameWnd {
public:
  // INLINED
  UIClanWnd() : UIFrameWnd(0), m_saved(nullptr), m_title(nullptr) {
  }
  void StoreInfo() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual int Tab() = 0;

protected:
  /* 00B4 */ ClanWndInfo* m_saved;
  /* 00B8 */ UIStaticText* m_title;
};

class UIClanInfoManageWnd : public UIClanWnd {
public:
  UIClanInfoManageWnd();
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  int Tab() override;
  __declspec(noinline) void LoadEmblem();

protected:
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ CBitmapRes* m_emblem;
};

#endif
