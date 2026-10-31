#ifndef RAGNAROK_SOURCE_UI_LISTS_UISCROLLBARVARIANTS_H
#define RAGNAROK_SOURCE_UI_LISTS_UISCROLLBARVARIANTS_H

#include "UI/Lists/UIScrollBar.h"

#include <string>

class CBitmapRes;

class UIScrollBar2 : public UIScrollBar {
public:
  explicit UIScrollBar2(UIWindowContext& windows) : UIScrollBar(windows) {
    this->arrowLength = 10;
  }

  void Paint() override;
};

class UIScrollBar3 : public UIScrollBar {
public:
  explicit UIScrollBar3(UIWindowContext& windows) : UIScrollBar(windows) {
    this->arrowLength = 9;
  }

  void Paint() override;
};

class UIScrollBar4 : public UIScrollBar {
public:
  void Paint() override;
};

class UIScrollBar_Custom : public UIScrollBar {
public:
  /* 00B4 */ CBitmapRes* m_trackTop;
  /* 00B8 */ CBitmapRes* m_trackMiddle;
  /* 00BC */ CBitmapRes* m_trackBottom;
  /* 00C0 */ CBitmapRes* m_arrowUp;
  /* 00C4 */ CBitmapRes* m_arrowDown;
  /* 00C8 */ CBitmapRes* m_thumbTop;
  /* 00CC */ CBitmapRes* m_thumbMiddle;
  /* 00D0 */ CBitmapRes* m_thumbBottom;

  void Paint() override;
  void SetArrowBitmaps(std::string up, std::string down);
  void SetThumbBitmaps(std::string top, std::string middle, std::string bottom);
};

class UIZeroQuestScrollBar : public UIScrollBar {
public:
  void Paint() override;
};

#endif
