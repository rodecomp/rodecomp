#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODETOOLTIP_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODETOOLTIP_H

#include <cstdint>
#include <list>

class UITransBalloonText;
class UIWindow;
class UIWindowMgr;
class UIWindowMgr;

struct ModeTooltip {
  UITransBalloonText*& window;
  std::uint32_t& tick;
  std::list<UIWindow*>& rendered;
  UIWindowMgr& ordering;
  const int& screenWidth;
  const int& screenHeight;

  void Show(const char* value, int x, int y, std::uint32_t color, unsigned char option, unsigned char convert);
};

#endif
