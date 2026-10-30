#ifndef RAGNAROK_SOURCE_UI_TEXT_UITAGTEXTVIEWER_H
#define RAGNAROK_SOURCE_UI_TEXT_UITAGTEXTVIEWER_H

#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class UIScrollBar;

namespace TagTextViewer {
  struct Position {
    Position() {
    }

    /* 0000 */ float x;
    /* 0004 */ float y;
  };

  struct TextFont {
    /* 0000 */ std::uint32_t color;
    /* 0004 */ int height;
    /* 0008 */ unsigned char bold;
    /* 0009 */ unsigned char italic;
  };

  class UIScroll : public UIWindow {
  public:
    explicit UIScroll(UIWindowContext& context) : UIWindow(context, 0), m_scrollPos(0), m_scrollBar(nullptr) {
    }
    void Slot15(int width, int height) override;
    void Slot35(int delta) override;
    int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
    void UpdateScroll();
    void StorePosition(UIWindow* child);
    int ContentHeight();

    /* 007C */ int m_scrollPos;
    /* 0080 */ UIScrollBar* m_scrollBar;
    /* 0084 */ std::map<UIWindow*, Position> m_positions;
  };
}

class UITagTextViewer : public TagTextViewer::UIScroll {
public:
  explicit UITagTextViewer(UIWindowContext& context);
  void Resize(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void SetAlign(unsigned char align);
  void Clear();

  /* 008C */ std::uint32_t m_backgroundColor;
  /* 0090 */ int m_textWidth;
  /* 0094 */ int m_lineSpacing;
  /* 0098 */ int m_margin;
  /* 009C */ int m_topMargin;
  /* 00A0 */ TagTextViewer::TextFont m_font;
  /* 00AC */ unsigned char m_outlined;
  /* 00B0 */ std::uint32_t m_outlineColor;
  /* 00B4 */ unsigned char m_horizontalAlign;
  /* 00B5 */ unsigned char m_verticalAlign;
  /* 00B8 */ int m_cursorY;
  /* 00BC */ int m_offsetY;
  /* 00C0 */ std::vector<std::string> m_texts;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TagTextViewer::Position) == 8);
static_assert(sizeof(TagTextViewer::TextFont) == 12);
#endif

#endif
