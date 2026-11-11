#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UITRANSMENUWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UITRANSMENUWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

struct TransMenuItem {
  /* 0000 */ char text[24];
  /* 0018 */ int command;
  /* 001C */ bool enabled;
};

class UITransMenuWnd : public UIFrameWnd {
public:
  UITransMenuWnd();
  void Resize(int width, int height) override;
  void Move(int x, int y) override;
  bool GetTransBoxInfo(void* output) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void ClearItems();

protected:
  /* 00B4 */ std::vector<TransMenuItem> m_items;
  /* 00C0 */ std::uint32_t m_x;
  /* 00C4 */ std::uint32_t m_y;
  /* 00C8 */ int m_width;
  /* 00CC */ int m_height;
  /* 00D0 */ std::uint32_t m_rectangleReserved1;
  /* 00D4 */ std::uint32_t m_backgroundColor;
  /* 00D8 */ std::uint32_t m_rectangleReserved2;
  /* 00DC */ int m_lineHeight;
  /* 00E0 */ int m_margin;
  /* 00E4 */ int m_selected;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TransMenuItem) == 0x20);
#endif

#endif
