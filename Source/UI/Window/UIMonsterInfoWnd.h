#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERINFOWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

struct UIButtonContext;

struct MonsterInfo {
  /* 0000 */ short unknown_0000;
  /* 0002 */ short job;
  /* 0004 */ short level;
  /* 0006 */ short size;
  /* 0008 */ int hp;
  /* 000C */ short defense;
  /* 000E */ short race;
  /* 0010 */ short magicDefense;
  /* 0012 */ short element;
  /* 0014 */ unsigned char resistances[9];
};

class UIMonsterInfoWnd : public UIFrameWnd {
public:
  UIMonsterInfoWnd(UIWindowContext& windows, UIFrameContext& frames);

  void Slot15(int width, int height) override;
  void Slot28(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void DrawResistance(int x, int y, const char* label, int value, std::uint32_t border, std::uint32_t fill);
  void DrawNewItemIcon(int x, int y);

  /* 00B4 */ MonsterInfo info;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ std::string name;

protected:
  UIButtonContext* m_controls;
};

extern std::uint32_t g_monsterInfoActorId;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MonsterInfo) == 32);
#endif

#endif
