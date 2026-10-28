#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERINFOWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct UIButtonContext;

struct MonsterInfo {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ unsigned char unknown_001C;
};

class UIMonsterInfoWnd : public UIFrameWnd {
public:
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ MonsterInfo info;

protected:
  UIButtonContext* m_controls;
};

extern std::uint32_t g_monsterInfoActorId;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MonsterInfo) == 32);
#endif

#endif
