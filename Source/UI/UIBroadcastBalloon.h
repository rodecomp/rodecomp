#ifndef RAGNAROK_SOURCE_UI_UIBROADCASTBALLOON_H
#define RAGNAROK_SOURCE_UI_UIBROADCASTBALLOON_H

#include "UI/Window/UIWindow.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <string>

struct UIButtonContext;

struct BroadcastBalloonLine {
  /* 0000 */ std::string text;
  /* 0018 */ std::uint32_t color;
  /* 001C */ std::uint32_t outlineColor;
};

struct BroadcastBalloonMessage {
  /* 0000 */ std::list<BroadcastBalloonLine> lines;
  /* 0008 */ std::chrono::system_clock::time_point start;
  /* 0010 */ std::chrono::seconds duration;
  /* 0018 */ int fontHeight;
  /* 001C */ std::uint32_t backgroundColor;
  /* 0020 */ int width;
  /* 0024 */ int height;
};

class UIBroadcastBalloon : public UIWindow {
public:
  UIBroadcastBalloon(UIWindowContext& windows, UIButtonContext& controls);
  void Paint() override;
  bool CanHitTest() override;
  void Slot19() override;
  bool Slot42(void* output) override;
  void Slot43() override;

  void AddMessage(int category, const BroadcastBalloonMessage& message);
  void sub_A86510(
    int x, int y, const char* text, int bytes, std::uint32_t color, std::uint32_t outline, int family, int height,
    unsigned char bold
  );

protected:
  /* 007C */ std::map<int, std::list<BroadcastBalloonMessage>> m_messages;
  UIButtonContext* m_controls;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BroadcastBalloonLine) == 0x20);
static_assert(offsetof(BroadcastBalloonMessage, start) == 0x08);
static_assert(offsetof(BroadcastBalloonMessage, height) == 0x24);
static_assert(sizeof(BroadcastBalloonMessage) == 0x28);
#endif

#endif
