#ifndef RAGNAROK_SOURCE_UI_TEXT_UITRANSBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UITRANSBALLOONTEXT_H

#include "UI/Text/UIBalloonText.h"

struct BalloonRectangle {
  int x;
  int y;
  int width;
  int height;
  std::uint32_t unknown_0010;
  std::uint32_t color;
  std::uint32_t unknown_0018;
};

class UITransBalloonText : public UIBalloonText {
public:
  UITransBalloonText(UIWindowContext& windows, BalloonTextContext& text);
  ~UITransBalloonText() override;

  void Resize(int width, int height) override;
  void Move(int x, int y) override;
  bool Slot07(void* output) override;
  void Slot15(int width, int height) override;
  void Paint() override;

  void SetRichText(unsigned char value) {
    this->m_richText = value;
  }
  void SetColors(std::uint32_t text, std::uint32_t outline);
  void SetBackground(std::uint32_t color);

private:
  /* 00A0 */ BalloonRectangle m_rectangle;
  /* 00BC */ std::vector<std::uint32_t> m_lineColors;
  /* 00C8 */ std::uint32_t m_borderColor;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UITransBalloonText) == 212);
#endif

#endif
