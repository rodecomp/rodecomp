#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILDHELPERWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILDHELPERWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct GuildHelperWndInfo;

class UIGuildHelperWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;

protected:
  UIButtonContext* m_controls;
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ GuildHelperWndInfo* m_saved;
};

class UICreateGuildWnd : public UIGuildHelperWnd {
public:
  void Slot11() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;

protected:
  /* 00BC */ unsigned char unknown_00BC[0x4];
  /* 00C0 */ int m_mode;
};

class UIGuildTipWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

class UIGuildLeaveReasonDescWnd : public UIFrameWnd {
public:
  void Paint() override;
};

class UIGuildBanReasonDescWnd : public UIGuildLeaveReasonDescWnd {
};

#endif
