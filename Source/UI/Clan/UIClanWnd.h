#ifndef RAGNAROK_SOURCE_UI_CLAN_UICLANWND_H
#define RAGNAROK_SOURCE_UI_CLAN_UICLANWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct ClanWndInfo;
class CBitmapRes;
class UIStaticText;

class UIClanWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual int Slot53() = 0;

protected:
  UIButtonContext* m_controls;
  /* 00B4 */ ClanWndInfo* m_saved;
};

class UIClanInfoManageWnd : public UIClanWnd {
public:
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  int Slot53() override;
  __declspec(noinline) void LoadEmblem();

protected:
  /* 00B8 */ UIStaticText* m_title;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ CBitmapRes* m_emblem;
};

#endif
