#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILDHELPERWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILDHELPERWND_H

#include "UI/Window/UIFrameWnd.h"

struct GuildHelperWndInfo;
class UIBitmapButton;
class UIEditCtrl;
class UIStaticText;
class UITextViewer;

struct GuildBanTarget {
  /* 0000 */ int guildId;
  /* 0004 */ int accountId;
  /* 0008 */ int characterId;
};

class UIGuildHelperWnd : public UIFrameWnd {
public:
  UIGuildHelperWnd();
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ UIBitmapButton* m_button;
  /* 00B8 */ GuildHelperWndInfo* m_saved;
};

class UICreateGuildWnd : public UIGuildHelperWnd {
public:
  explicit UICreateGuildWnd(int mode);
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00BC */ UIEditCtrl* m_nameEdit;
  /* 00C0 */ int m_mode;
};

class UIGuildTipWnd : public UIFrameWnd {
public:
  UIGuildTipWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ UITextViewer* m_textViewer;
  /* 00B8 */ UIBitmapButton* m_closeButton;
};

class UIGuildLeaveReasonDescWnd : public UIFrameWnd {
public:
  UIGuildLeaveReasonDescWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ UIEditCtrl* m_reasonEdit;
  /* 00B8 */ UIStaticText* m_reasonText;
};

class UIGuildBanReasonDescWnd : public UIGuildLeaveReasonDescWnd {
public:
  UIGuildBanReasonDescWnd();
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00BC */ GuildBanTarget m_target;
};

#endif
