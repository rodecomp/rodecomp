#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIBOOKWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIBOOKWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

class UISpriteButton;
class UITextViewer;
struct UITextViewerContext;
struct WindowOrdering;

struct UIBookContext {
  UITextViewerContext& text;
  WindowOrdering& ordering;
};

class UIBookWnd : public UIFrameWnd {
public:
  /* 00B4 */ UITextViewer* m_viewer;
  /* 00B8 */ UIWindow* unknown_00B8;
  /* 00BC */ char m_bookName[100];
  /* 0120 */ int m_lineCount;
  /* 0124 */ int m_pageCount;
  /* 0128 */ int m_page;
  /* 012C */ int m_autoLine;
  /* 0130 */ std::uint32_t m_autoTick;
  /* 0134 */ unsigned char m_autoRead;
  /* 0138 */ unsigned m_red;
  /* 013C */ unsigned m_green;
  /* 0140 */ unsigned m_blue;
  /* 0144 */ std::vector<std::string> m_lines;
  /* 0150 */ UISpriteButton* m_buttons[4];

  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  int LoadBookmark(const char* name);

protected:
  UIBookContext* m_book;
};

extern const char* g_bookmarkKeySuffix;

#endif
