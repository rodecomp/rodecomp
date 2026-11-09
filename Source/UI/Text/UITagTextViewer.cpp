#include "UI/Text/UITagTextViewer.h"
#include "UI/Window/UIEnums.h"

#include <algorithm>

#include "Contents/MapSkyMgr.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/CreateLineInfo.h"

// OFFSET: 0x0083E1B0
// FUNCTION: fn_10515
UITagTextViewer::UITagTextViewer()
    : UIScroll(), m_backgroundColor(0xFFFFFF), m_textWidth(0), m_lineSpacing(6), m_margin(3), m_topMargin(4), m_font{0, 12, 0, 0}, m_outlined(0),
      m_outlineColor(0xFFFFFF), m_horizontalAlign(0), m_verticalAlign(3), m_cursorY(0) {
  m_offsetY = m_topMargin;
}

// OFFSET: 0x0083E740
// FUNCTION: fn_10520
void TagTextViewer::UIScroll::StorePosition(UIWindow* child) {
  const float y = static_cast<float>(child->geometry.y);
  const float x = static_cast<float>(child->geometry.x);
  Position& position = this->m_positions[child];
  position.x = x;
  position.y = y;
}

// OFFSET: 0x0083ECF0
// FUNCTION: fn_10522
void UITagTextViewer::Clear() {
  const auto end = this->m_positions.end();
  for (auto it = this->m_positions.begin(); it != end; ++it) {
    this->RemoveChild(it->first, 1);
  }

  this->m_positions.clear();
  this->m_scrollPos = 0;
  this->m_cursorY = 0;
  this->m_offsetY = this->m_topMargin;
  this->UpdateScroll();
  this->m_texts.clear();
}

// OFFSET: 0x0083EDE0
// FUNCTION: fn_10523
int TagTextViewer::UIScroll::ContentHeight() {
  int height = 0;

  for (std::map<UIWindow*, Position>::const_iterator it = this->m_positions.cbegin(); it != this->m_positions.cend(); ++it) {
    height = static_cast<int>(std::max(static_cast<float>(height), static_cast<float>(it->first->geometry.height) + it->second.y));
  }

  return height;
}

// OFFSET: 0x0083EE40
// FUNCTION: fn_10524
void TagTextViewer::UIScroll::OnCreate(int width, int height) {
  this->m_scrollBar = new UIScrollBar();
  this->m_scrollBar->Configure(width - 13, 0, 1, height);
  this->AddChild(this->m_scrollBar);
  this->m_scrollBar->Show(0);
  this->UpdateScroll();
}

// OFFSET: 0x0083EEF0
// FUNCTION: fn_10525
void UITagTextViewer::Paint() {
  this->ClearSurface(this->m_backgroundColor);

  if (this->m_backgroundColor == 0xFF00FF && this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0083EF20
// FUNCTION: fn_10526
void UITagTextViewer::OnProcess() {
  for (UIWindow* child : this->m_children) {
    child->OnProcess();
  }
}

// OFFSET: 0x0083EF50
// FUNCTION: fn_10527
void TagTextViewer::UIScroll::OnWheel(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERTPAGEDOWN, 0, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERTPAGEUP, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0083F030
// FUNCTION: fn_10528
void TagTextViewer::UIScroll::UpdateScroll() {
  int height = this->geometry.height;
  for (std::map<UIWindow*, Position>::iterator it = this->m_positions.begin(); it != this->m_positions.end(); ++it) {
    height = static_cast<int>(std::max(static_cast<float>(height), static_cast<float>(it->first->geometry.height) + it->second.y));
  }

  if (height > this->geometry.height) {
    this->m_scrollBar->Show(1);
    this->m_scrollBar->SetRange(height - this->geometry.height, this->geometry.height, height);
    const int position = std::max(0, this->m_scrollPos);
    const int range = height - this->geometry.height;
    this->m_scrollPos = std::min(range, position);
    this->m_scrollBar->SetPosition(this->m_scrollPos);
    for (std::map<UIWindow*, Position>::iterator it = this->m_positions.begin(); it != this->m_positions.end(); ++it) {
      it->first->Move(static_cast<int>(it->second.x), static_cast<int>(it->second.y - static_cast<float>(this->m_scrollPos)));
    }
  } else {
    this->m_scrollBar->Show(0);
  }

  this->Invalidate();
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0083F140
// FUNCTION: fn_10529
void UITagTextViewer::Resize(int width, int height) {
  this->m_textWidth += width - this->geometry.width;
  this->m_textWidth = std::max(0, this->m_textWidth);

  UIWindow::Resize(width, height);
  this->m_scrollBar->Resize(this->m_scrollBar->geometry.width, height);
  this->m_scrollBar->Move(width - this->m_scrollBar->geometry.width, 0);
  this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
}

// OFFSET: 0x0083F1C0
// FUNCTION: fn_10530
int TagTextViewer::UIScroll::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_UPPUSH:
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERTPAGEUP, 0, 0, 0, 0);
    break;
  case UM_DOWNPUSH:
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERTPAGEDOWN, 0, 0, 0, 0);
    break;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_scrollPos;
    int delta = argument1;
    if (message == UM_SCROLLVERTPAGEUP) {
      delta = -10;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      delta = 10;
    }
    this->m_scrollPos = previous + delta;
    if (previous != this->m_scrollPos) {
      this->UpdateScroll();
    }
    return this->m_scrollPos - previous;
  }
  }
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0083F280
// FUNCTION: fn_10531
int UITagTextViewer::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_SETTEXT:
    if (argument1 != 0) {
      this->AddText(std::string(reinterpret_cast<const char*>(argument1)));
    }
    break;
  case UM_TEXT_LBTN_UP:
    switch (argument1) {
    case LINE_SEGMENT_URL:
      OpenTextButtonLink(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_ITEM:
      ShowTextButtonItem(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_NAVI:
      OpenTextButtonNavigation(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_QUEST:
      RequestTextButtonService(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_TIPBOX:
      ShowTextButtonTip(static_cast<UITextButton*>(sender));
      break;
    }
    break;
  case UM_REFRESH:
    this->Layout();
    break;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_scrollPos;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->m_scrollPos = previous - 30;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->m_scrollPos = previous + 30;
    } else if (message == UM_SCROLLVERT) {
      this->m_scrollPos = previous + static_cast<int>(argument1);
    }

    if (previous != this->m_scrollPos) {
      this->UpdateScroll();
    }
    return this->m_scrollPos - previous;
  }
  default:
    return TagTextViewer::UIScroll::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0083F590
// FUNCTION: fn_10532
void UITagTextViewer::SetAlign(unsigned char align) {
  switch (align) {
  case 0:
  case 1:
  case 2:
    this->m_horizontalAlign = align;
    break;
  case 3:
  case 4:
  case 5:
    this->m_verticalAlign = align;
    break;
  }
}

// OFFSET: 0x0083E870
// FUNCTION: fn_10521
void UITagTextViewer::AddText(const std::string& text) {
  this->m_texts.push_back(text);
  if (this->m_textWidth == 0) {
    this->m_textWidth = this->geometry.width - this->m_margin - 0xD;
  }

  const int previousCursor = this->m_cursorY;
  const int spacing = previousCursor > 0 ? this->m_lineSpacing : 0;
  this->m_cursorY = previousCursor + spacing;

  const LineFontFlags flags = {this->m_font.bold != 0, this->m_font.italic != 0};
  CCreateLineInfo info(
    this, text.c_str(), this->m_textWidth, this->m_lineSpacing, this->m_font.color, this->m_font.height, flags, this->m_outlined != 0, this->m_outlineColor,
    false, 0xFFFFFF, this->m_horizontalAlign
  );
  this->m_cursorY += info.m_height;

  int offset = std::max(this->m_topMargin, 0);
  if (this->m_topMargin + this->m_cursorY < this->geometry.height) {
    if (this->m_verticalAlign == 5) {
      offset = this->geometry.height - this->m_topMargin - this->m_cursorY;
    } else if (this->m_verticalAlign == 4) {
      offset = static_cast<int>(static_cast<unsigned int>(static_cast<float>(this->geometry.height - this->m_cursorY) * g_depthAverageHalf));
    }
  }

  for (std::map<UIWindow*, TagTextViewer::Position>::iterator it = this->m_positions.begin(); it != this->m_positions.end(); ++it) {
    UIWindow* window = it->first;
    window->Move(window->geometry.x, window->geometry.y - this->m_offsetY + offset);
    it->second.y = static_cast<float>(window->geometry.y);
    it->second.x = static_cast<float>(window->geometry.x);
  }
  this->m_offsetY = offset;

  const std::vector<std::vector<UIWindow*>> lines = info.m_lines;
  for (std::vector<UIWindow*> line : lines) {
    for (UIWindow* window : line) {
      window->Move(window->geometry.x + this->m_margin, window->geometry.y + offset + previousCursor + spacing);
      this->AddChild(window);
      this->StorePosition(window);
    }
  }

  this->UpdateScroll();
}

// OFFSET: 0x0083F5E0
// FUNCTION: fn_10533
void UITagTextViewer::Layout() {
  std::string joined;
  for (std::vector<std::string>::iterator it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
    joined += *it + (it + 1 != this->m_texts.end() ? "\n" : "");
  }

  this->Clear();
  this->m_texts.push_back(joined);
  if (this->m_textWidth == 0) {
    this->m_textWidth = this->geometry.width - this->m_margin - 0xD;
  }

  const LineFontFlags flags = {this->m_font.bold != 0, this->m_font.italic != 0};
  CCreateLineInfo info(
    this, joined.c_str(), this->m_textWidth, this->m_lineSpacing, this->m_font.color, this->m_font.height, flags, this->m_outlined != 0, this->m_outlineColor,
    false, 0xFFFFFF, this->m_horizontalAlign
  );
  const std::vector<std::vector<UIWindow*>> lines = info.m_lines;

  int offset = std::max(this->m_topMargin, 0);
  const unsigned int content = static_cast<unsigned int>(this->m_topMargin) + static_cast<unsigned int>(info.m_height);
  if (static_cast<unsigned int>(this->geometry.height) > content) {
    if (this->m_verticalAlign == 5) {
      offset = this->geometry.height - info.m_height - this->m_topMargin;
    } else if (this->m_verticalAlign == 4) {
      offset = static_cast<unsigned int>(static_cast<float>(static_cast<unsigned int>(this->geometry.height - info.m_height)) * g_depthAverageHalf);
    }
  }
  for (std::vector<UIWindow*> line : lines) {
    for (UIWindow* window : line) {
      window->Move(window->geometry.x + this->m_margin, window->geometry.y + offset);
      this->AddChild(window);
      this->StorePosition(window);
    }
  }

  this->UpdateScroll();
}
