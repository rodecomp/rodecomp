#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILDHELPERWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILDHELPERWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct UIEditContext;
struct GuildHelperWndInfo;
class UIBitmapButton;
class UIEditCtrl;
class UIStaticText;
class UITextViewer;

struct GuildBanTarget {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
};

class UIGuildHelperWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
  /* 00B4 */ UIBitmapButton* m_button;
  /* 00B8 */ GuildHelperWndInfo* m_saved;
};

class UICreateGuildWnd : public UIGuildHelperWnd {
public:
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00BC */ UIEditCtrl* m_nameEdit;
  /* 00C0 */ int m_mode;
  UIEditContext* m_editContext;
};

class UIGuildTipWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
  /* 00B4 */ UITextViewer* m_textViewer;
  /* 00B8 */ UIBitmapButton* m_closeButton;
};

class UIGuildLeaveReasonDescWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ UIEditCtrl* m_reasonEdit;
  /* 00B8 */ UIStaticText* m_reasonText;
};

class UIGuildBanReasonDescWnd : public UIGuildLeaveReasonDescWnd {
public:
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00BC */ GuildBanTarget m_target;
};

#endif
