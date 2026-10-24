#include "Client/TextInputBuffer.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext.h"
#include "UI/EditPainting.h"
#include "UI/EditTextState.h"
#include "UI/EditViewport.h"
#include "UI/InputFocus.h"
#include "UI/UIButton.h"
#include "UI/UIEditCtrl.h"
#include "UI/WindowManagerWindows.h"

#include <algorithm>
#include <bit>
#include <cstring>
#include <mmsystem.h>

namespace {
  int Add(int left, int right) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(left) + static_cast<std::uint32_t>(right));
  }
  class LinkLayout : public EditLinkLayout {
  public:
    explicit LinkLayout(UIEditCtrl& control) : m_control(control) {
    }
    void Update() override {
      this->m_control.UpdateLinks();
    }

  private:
    UIEditCtrl& m_control;
  };
}

// OFFSET: 0x0084FAE0
// FUNCTION: fn_10663
UIEditCtrl::UIEditCtrl(UIWindowContext& windows, UIEditContext& edit)
    : UIWindow(windows, 0), anchor(0), cursor(0), mask(0), maximum(255), flat(1), red(255), green(255), blue(255),
      borderRed(0), borderGreen(0), borderBlue(0), textColor(0), secondColor(0xFFFFFFFF), firstVisible(0), textY(0),
      invalidatesParent(0), disabled(0), unknown_00F0(0), topPadding(0), preserveInput(0), m_edit(&edit) {
  this->text.reserve(255);
}

// OFFSET: 0x008517E0
// OFFSET: 0x00852980
// FUNCTION: fn_10718
UIEditCtrl::~UIEditCtrl() = default;

// OFFSET: 0x005C3210
// FUNCTION: fn_03552
bool UIEditCtrl::Slot05() {
  return this->disabled == 0;
}

// OFFSET: 0x0085CCA0
// FUNCTION: fn_10807
void UIEditCtrl::Paint() {
  const auto component = [&](int value) {
    return static_cast<unsigned char>(this->disabled != 0 && value > 30 ? Add(value, -30) : value);
  };
  const unsigned color = component(this->red) | (component(this->green) << 8) | (component(this->blue) << 16);
  this->textY = 0;

  if (this->geometry.surface) {
    if (this->flat == 0) {
      RECT rectangle{0, 0, this->geometry.width, this->geometry.height};
      const unsigned border = static_cast<unsigned char>(this->borderRed) |
                              (static_cast<unsigned char>(this->borderGreen) << 8) |
                              (static_cast<unsigned char>(this->borderBlue) << 16);
      this->geometry.surface->Fill(&rectangle, border);
      rectangle = {1, 1, Add(this->geometry.width, -1), Add(this->geometry.height, -1)};
      this->geometry.surface->Fill(&rectangle, color);
    } else {
      this->geometry.surface->Fill(nullptr, color);
    }
  }

  this->textY = Add(this->topPadding, 2);
  this->PaintText();
}

// OFFSET: 0x0085AAB0
// FUNCTION: fn_10783
void UIEditCtrl::Slot23() {
  this->m_edit->input.CancelComposition();
  this->m_edit->input.CancelComposition();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
}

// OFFSET: 0x0085E3B0
// FUNCTION: fn_10826
void UIEditCtrl::Slot24(int) {
  this->Synchronize();
  this->Invalidate();
}

// OFFSET: 0x0085EC20
// FUNCTION: fn_10842
void UIEditCtrl::Slot25(int, int) {
  if (this->disabled == 0) {
    this->m_edit->focus.Set(this);
  }
}

// OFFSET: 0x0085F390
// FUNCTION: fn_10860
void UIEditCtrl::Slot28(int x, int y) {
  if (this->disabled != 0) {
    return;
  }

  this->Slot27(x, y);

  if (!this->tooltip.empty()) {
    int left = 10, top = -20;

    for (const UIWindow* window = this; window; window = static_cast<const UIWindow*>(window->parent)) {
      left = Add(left, window->geometry.x);
      top = Add(top, window->geometry.y);
    }

    this->m_edit->buttons.services.Tooltip(this->tooltip.c_str(), left, top, -1, false, true);
  }
}

// OFFSET: 0x008641C0
// FUNCTION: fn_10914
int UIEditCtrl::SendMsg(
  UIWindow* sender,
  int message,
  std::intptr_t a,
  std::intptr_t b,
  std::intptr_t c,
  std::intptr_t d
) {
  if (message == 153) {
    this->InsertLink(reinterpret_cast<const char*>(a));
  } else if (message == 98 && c > 0) {
    UIWindow* window = this->m_edit->windows.EnsureWindow(12);

    if (window) {
      window->SendMsg(nullptr, 24, c, 0, 0, 0);
    }
  }

  UIWindow::SendMsg(sender, message, a, b, c, d);

  if (message == 0 && this->preserveInput == 0) {
    this->m_edit->input.CancelComposition();
  }

  return 0;
}

// OFFSET: 0x005C44E0
int UIEditCtrl::Slot46() {
  return 2;
}

// OFFSET: 0x008680D0
// FUNCTION: fn_10952
void UIEditCtrl::SetText(const char* value) {
  this->text.assign(value, std::strlen(value));
  this->Trim();

  if (this->m_edit->focus.window == this) {
    this->m_edit->input.CancelComposition();
    this->m_edit->input.CancelComposition();
    this->m_edit->input.Insert(this->text.c_str());
  }

  this->SelectAll();

  if (this->parent) {
    static_cast<UIWindow*>(this->parent)->Invalidate();
  }
}

// OFFSET: 0x0085A590
// FUNCTION: fn_10778
void UIEditCtrl::SelectAll() {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor,        this->cursor,  this->maximum,
                      focused,    this->m_edit->input, g_textCodePage};
  state.SelectAll();
}

// OFFSET: 0x00856980
// FUNCTION: fn_10755
void UIEditCtrl::PaintText() {
  this->PaintTextWithPolicy(false, nullptr, 0, false);
}

void UIEditCtrl::PaintTextWithPolicy(
  bool alternate,
  const std::string* placeholder,
  std::uint32_t placeholderColor,
  bool placeholderEnabled
) {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor,        this->cursor,  this->maximum,
                      focused,    this->m_edit->input, g_textCodePage};
  EditViewport viewport{
    this->text,
    this->anchor,
    this->cursor,
    this->firstVisible,
    this->geometry.width,
    g_textCodePage,
    this->m_edit->input,
    state,
    *this->geometry.surface,
    this->m_edit->buttons.fonts,
    this->m_edit->buttons.locale
  };
  UIWindow* parent = static_cast<UIWindow*>(this->parent);
  int left = 0, top = 0;

  for (const UIWindow* window = this; window; window = static_cast<const UIWindow*>(window->parent)) {
    left = Add(left, window->geometry.x);
    top = Add(top, window->geometry.y);
  }

  const bool chat = this->m_edit->input.chatInput == this || this->m_edit->otherChatInput == this;
  const DWORD tick = timeGetTime();
  LinkLayout layout(*this);
  EditPainting painting{
    this->text,
    this->anchor,
    this->cursor,
    this->mask,
    this->firstVisible,
    this->textY,
    this->invalidatesParent,
    this->textColor,
    this->secondColor,
    focused,
    chat,
    this->m_edit->input.suggestionsEnabled,
    tick,
    this->m_edit->candidateCursor,
    this->geometry.width,
    this->geometry.height,
    this->m_edit->screenWidth,
    this->m_edit->screenHeight,
    this->m_edit->input,
    this->m_edit->charset,
    viewport,
    *this->geometry.surface,
    this->m_edit->buttons.fonts,
    this->m_edit->buttons.locale,
    this->m_edit->colorChip,
    parent,
    this->m_edit->candidateWindow,
    this->m_edit->readingWindow,
    left,
    top,
    this->links,
    layout
  };
  painting.alternate = alternate;
  painting.placeholder = placeholder;
  painting.placeholderColor = placeholderColor;
  painting.placeholderEnabled = placeholderEnabled;
  painting.Draw();
}

// OFFSET: 0x00862FB0
// FUNCTION: fn_10905
void UIEditCtrl::Synchronize() {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor,        this->cursor,  this->maximum,
                      focused,    this->m_edit->input, g_textCodePage};
  EditViewport viewport{
    this->text,
    this->anchor,
    this->cursor,
    this->firstVisible,
    this->geometry.width,
    g_textCodePage,
    this->m_edit->input,
    state,
    *this->geometry.surface,
    this->m_edit->buttons.fonts,
    this->m_edit->buttons.locale
  };
  viewport.Synchronize();
}

// OFFSET: 0x00868450
// FUNCTION: fn_10959
void UIEditCtrl::Trim() {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor,        this->cursor,  this->maximum,
                      focused,    this->m_edit->input, g_textCodePage};
  state.Trim();
}

// OFFSET: 0x00858D50
// FUNCTION: fn_10768
int UIEditCtrl::Length() const {
  return static_cast<int>(this->m_edit->focus.window == this ? this->m_edit->input.text.size() : this->text.size());
}

// OFFSET: 0x00858D20
// FUNCTION: fn_10767
const char* UIEditCtrl::Text() {
  if (this->m_edit->focus.window == this) {
    this->Synchronize();
  }

  return this->text.c_str();
}

// OFFSET: 0x00865DD0
// FUNCTION: fn_10937
void UIEditCtrl::Background(int red, int green, int blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->Invalidate();
}

// OFFSET: 0x00868570
// FUNCTION: fn_10960
void UIEditCtrl::TextColor(std::uint32_t primary, std::uint32_t secondary) {
  this->secondColor = secondary;
  this->textColor = primary;
  this->Invalidate();
}

// OFFSET: 0x0097C230
// FUNCTION: fn_13324
UIEditCtrlWheel::UIEditCtrlWheel(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit) {
}

// OFFSET: 0x009701B0
// FUNCTION: fn_13241
UIEditCtrlWheel::~UIEditCtrlWheel() = default;

// OFFSET: 0x0098A240
// FUNCTION: fn_13408
void UIEditCtrlWheel::Slot35(int value) {
  if (this->parent) {
    static_cast<UIWindow*>(this->parent)->Slot35(value);
  }
}
