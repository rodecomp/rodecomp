#ifndef RAGNAROK_SOURCE_UI_TEXT_UITRANSBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UITRANSBALLOONTEXT_H

#include "UI/Text/UIBalloonText.h"

#include <cstddef>

struct BalloonRectangle {
  /* 00A0 */ int x;
  /* 00A4 */ int y;
  /* 00A8 */ int width;
  /* 00AC */ int height;
  /* 00B0 */ std::uint32_t unknown_00B0;
  /* 00B4 */ std::uint32_t color;
  /* 00B8 */ std::uint32_t unknown_00B8;
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

  // INLINED
  void SetRichText(unsigned char value) {
    this->m_richText = value;
  }
  void SetColors(std::uint32_t text, std::uint32_t outline);
  void SetBackground(std::uint32_t color);
  __declspec(noinline) void AddText(const char* text);
  void AddLine(const char* text, std::uint32_t color);
  void SetLine(const char* text, std::uint32_t color);
  void ChangeText(const char* text, int line);

private:
  /* 00A0 */ BalloonRectangle m_rectangle;
  /* 00BC */ std::vector<std::uint32_t> m_lineColors;
  /* 00C8 */ std::uint32_t m_borderColor;
};

// VTABLE: vt_00945
class UIColorTransBalloonText : public UITransBalloonText {
public:
  // INLINED
  UIColorTransBalloonText(UIWindowContext& windows, BalloonTextContext& text) : UITransBalloonText(windows, text) {}
  void Paint() override;
};

class UIAchBalloonText : public UITransBalloonText {
public:
  // INLINED
  UIAchBalloonText(UIWindowContext& windows, BalloonTextContext& text) : UITransBalloonText(windows, text), m_fontFamily(0) {}
  void Paint() override;

protected:
  /* 00CC */ int m_fontFamily;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BalloonRectangle) == 0x1C);
static_assert(offsetof(BalloonRectangle, x) + 0xA0 == 0xA0);
static_assert(offsetof(BalloonRectangle, y) + 0xA0 == 0xA4);
static_assert(offsetof(BalloonRectangle, width) + 0xA0 == 0xA8);
static_assert(offsetof(BalloonRectangle, height) + 0xA0 == 0xAC);
static_assert(offsetof(BalloonRectangle, unknown_00B0) + 0xA0 == 0xB0);
static_assert(offsetof(BalloonRectangle, color) + 0xA0 == 0xB4);
static_assert(offsetof(BalloonRectangle, unknown_00B8) + 0xA0 == 0xB8);
static_assert(sizeof(UITransBalloonText) == 212);
#endif

#endif
