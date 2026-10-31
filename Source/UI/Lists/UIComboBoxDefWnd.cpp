#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIItemStoreComboBox.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include <cstdio>
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UIString.h"
namespace {
  constexpr char prefix[] = "유저인터페이스\\";
}

// OFFSET: 0x0086CC20
// FUNCTION: fn_11035
UIComboBoxDefWnd::UIComboBoxDefWnd(UIWindowContext& windows, UIComboBoxHeaderContext& controls)
    : UIWindow(windows, 0), button(nullptr), list(nullptr), label(nullptr), borderless(0), expanded(0), red(255), green(255), blue(255), borderRed(0),
      borderGreen(0), borderBlue(0), dropdownHeight(40), m_controls(&controls) {
}

// OFFSET: 0x00872AE0
// FUNCTION: fn_11134
UIComboBoxDefWnd::~UIComboBoxDefWnd() = default;

// OFFSET: 0x00885920
// FUNCTION: fn_11319
void UIComboBoxDefWnd::Slot15(int, int) {
  auto& controls = this->m_controls->buttons;
  this->button = new UIBitmapButton(*this->m_context, controls);
  const char* names[] = {"txtbox_btn_a.bmp", "txtbox_btn_b.bmp", "txtbox_btn_c.bmp"};

  for (int state = 0; state < 3; ++state) {
    this->button->SetBitmap((std::string(prefix) + "basic_interface\\" + names[state]).c_str(), state, 0);
  }

  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  this->list = new UIComboBoxEx(*this->m_context, this->m_controls->combo);
  this->list->SetSize(this->geometry.width, this->dropdownHeight);
  this->list->Move(0, this->button->BitmapHeight());
  this->AddChild(this->list);

  this->label = new UIStaticText(*this->m_context, controls);
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), this->button->BitmapHeight());
  this->label->SetText("(NO SELECT)", 0);
  this->label->Move(2, 3);
  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x00878DF0
// FUNCTION: fn_11213
void UIComboBox_BmpWnd::Create(int width, int height, const char* path, const char* name) {
  UIWindow::Resize(width, height);
  this->name = name;
  const std::string base(path);
  auto& controls = this->m_controls->buttons;

  this->button = new UIBitmapButton(*this->m_context, controls);
  this->button->SetBitmap((base + "_out.bmp").c_str(), 0, 0);
  this->button->SetBitmap((base + "_over.bmp").c_str(), 1, 0);
  this->button->SetBitmap((base + "_press.bmp").c_str(), 2, 0);
  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  this->list = new UIComboBoxEx(*this->m_context, this->m_controls->combo);
  this->list->SetSize(this->geometry.width, this->dropdownHeight);
  this->list->Move(0, this->button->BitmapHeight());
  this->AddChild(this->list);

  char text[12] = {};
  std::sprintf(text, "(NO SELECT)");
  this->label = new UIStaticText(*this->m_context, controls);
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), this->button->BitmapHeight());
  this->label->SetText(text, 0);
  this->label->Move(2, 3);
  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x00889D60
// FUNCTION: fn_11352
void UIComboBoxDefWnd::Paint() {
  const unsigned background =
    static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16);

  if (this->borderless != 0) {
    this->geometry.surface->Fill(nullptr, background);
  } else {
    const unsigned border =
      static_cast<unsigned char>(this->borderRed) | (static_cast<unsigned char>(this->borderGreen) << 8) | (static_cast<unsigned char>(this->borderBlue) << 16);
    this->geometry.surface->Fill(nullptr, border);
    this->FillRegion(1, 1, (this->geometry.width - 2), (this->geometry.height - 2), background);
  }
}

// OFFSET: 0x00890B50
// FUNCTION: fn_11434
void UIComboBoxDefWnd::Slot31(int, int) {
  this->SendMsg(nullptr, 6, 273, 0, 0, 0);
}

// OFFSET: 0x0089F5C0
// FUNCTION: fn_11623
void UIComboBoxDefWnd::Clear() {
  this->list->Clear();
}

// OFFSET: 0x008A15A0
// FUNCTION: fn_11635
std::string UIComboBoxDefWnd::GetSelectedText() {
  return this->label->tooltip.c_str();
}

// OFFSET: 0x008A25F0
// FUNCTION: fn_11641
void UIComboBoxDefWnd::AddOption(std::string text, std::uint32_t color) {
  this->list->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(text.c_str()), color, 0, 0);
}

// OFFSET: 0x008A2730
// FUNCTION: fn_11642
void UIComboBoxDefWnd::Select(int index) {
  this->list->SendMsg(nullptr, 40, index, 0, 0, 0);
  this->label->SetText(this->list->lines[index].text.c_str(), 0);
}

// OFFSET: 0x008A28B0
// FUNCTION: fn_11643
void TrimTrailing(std::string& text, const char* characters) {
  const std::size_t end = text.find_last_not_of(characters) + 1;
  if (end < text.size()) {
    text.erase(end, text.size() - end);
  }
}

// OFFSET: 0x00897E90
// FUNCTION: fn_11544
int UIComboBoxDefWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case 6:
    if (value == 273) {
      this->expanded = this->expanded == 0;
      this->Resize(
        this->geometry.width, this->expanded ? (this->geometry.height + this->list->geometry.height) : (this->geometry.height - this->list->geometry.height)
      );
      this->parent->Invalidate();
      this->Invalidate();
      this->list->Invalidate();
      UIWindow* root = this;

      while (root->parent) {
        root = root->parent;
      }

      root->Invalidate();

      if (this->parent) {
        this->parent->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
    }

    return 0;
  case 39:
    if (WindowPolicy(this->m_controls->buttons.policy, this->m_controls->buttons.restricted, 3, this->parent->geometry.id) == 0 ||
        value < 0) {
      return 0;
    }

    this->label->SetText(this->list->lines[value].text.c_str(), 0);
    this->SendMsg(nullptr, 6, 273, 0, 0, 0);
    this->parent->SendMsg(this, 39, value, 0, 0, 0);
    return 0;
  case 57:
    if (value >= 0) {
      this->label->SetText(this->list->lines[value].text.c_str(), 0);
      this->Invalidate();
    }

    return 0;
  default:
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00898940
// FUNCTION: fn_11551
int UIItemStoreComboBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case 6:
    if (value == 273) {
      this->expanded = this->expanded == 0;

      if (this->expanded) {
        this->list->Show(1);
        this->Resize(this->geometry.width, this->list->geometry.height + this->geometry.height);
        this->Move(214, this->geometry.y - this->list->geometry.height);
        this->button->Move(this->geometry.width - this->button->BitmapWidth(), this->geometry.height - this->button->BitmapHeight());
        this->label->Move(2, this->list->geometry.height + 3);
      } else {
        this->list->Show(0);
        this->Resize(this->geometry.width, this->geometry.height - this->list->geometry.height);
        this->Move(214, this->list->geometry.height + this->geometry.y);
        this->button->Move(this->geometry.width - this->button->BitmapWidth(), 0);
        this->label->Move(2, 3);
      }

      this->parent->Invalidate();
      this->list->Invalidate();
      this->Invalidate();
    }

    return 0;
  case 39:
    if (value < 0) {
      return 0;
    }

    this->label->SetText(this->list->lines[value].text.c_str(), 0);
    this->SendMsg(nullptr, 6, 273, 0, 0, 0);
    this->parent->SendMsg(this, 39, value, 0, 0, 0);
    return 0;
  default:
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x0086CBA0
// FUNCTION: fn_11034
UIComboBoxDef2Wnd::UIComboBoxDef2Wnd(UIWindowContext& windows, UIComboBoxHeaderContext& controls)
    : UIComboBoxDefWnd(windows, controls), skin(0), alignment(0), textOffset(0), previousTextX(0) {
}

// OFFSET: 0x00872A70
// FUNCTION: fn_11133
UIComboBoxDef2Wnd::~UIComboBoxDef2Wnd() = default;

// OFFSET: 0x00885250
// FUNCTION: fn_11318
void UIComboBoxDef2Wnd::Slot15(int width, int) {
  auto& controls = this->m_controls->buttons;
  const std::string path = std::string(prefix) + (this->skin == 1 ? "login_interface\\" : "UIadventurerAgency\\");

  this->button = new UIBitmapButton(*this->m_context, controls);
  const char* names[] = {"btn_dropdown_out.bmp", "btn_dropdown_over.bmp", "btn_dropdown_press.bmp"};

  for (int state = 0; state < 3; ++state) {
    this->button->SetBitmap((path + names[state]).c_str(), state, 0);
  }

  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  auto* list = new UIComboBoxEx2(*this->m_context, this->m_controls->combo);
  this->list = list;
  list->alignment = this->alignment;
  list->SetSize(width, (this->dropdownHeight + 2));
  list->Move(0, this->button->BitmapHeight());
  this->AddChild(list);

  this->label = new UIStaticText(*this->m_context, controls);
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), (this->button->BitmapHeight() - 6));
  this->label->SetText("", 0);
  int x = 0;

  if (this->alignment == 0) {
    x = 2;
  } else if (this->alignment == 1) {
    x = static_cast<int>(static_cast<float>(this->geometry.width) * 0.5f);
  }

  if (this->alignment == 0 || this->alignment == 1) {
    this->label->Move(x, (this->textOffset + 3));
  }

  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x00889BB0
// FUNCTION: fn_11351
void UIComboBoxDef2Wnd::Paint() {
  this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);

  if (this->alignment != 1) {
    return;
  }

  auto& controls = this->m_controls->buttons;
  const int width = this->TextWidth(controls.fonts, controls.locale, this->label->text.c_str(), 0, 0, 12, 0, 0);
  const int x = static_cast<int>(static_cast<float>(this->geometry.width) * 0.5f - static_cast<float>(width) * 0.5f);

  if (x != this->previousTextX) {
    this->previousTextX = x;
    this->label->Move(x, (this->textOffset + 3));
    this->label->Invalidate();
  }
}

// OFFSET: 0x00886C30
// FUNCTION: fn_11329
void UINewComboBoxCtrl::Slot15(int, int) {
  const std::string base = "유저인터페이스\\basic_interface\\txtbox_btn_";
  this->button = new UIBitmapButton(*this->m_context, *this->m_buttons);
  this->button->SetBitmap((base + "a.bmp").c_str(), 0, 0);
  this->button->SetBitmap((base + "b.bmp").c_str(), 1, 0);
  this->button->SetBitmap((base + "c.bmp").c_str(), 2, 0);
  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move((this->geometry.width - this->button->BitmapWidth()), 0);
  this->button->SetId(273);
  this->AddChild(this->button);

  this->label = new UIStaticText(*this->m_context, *this->m_buttons);
  this->label->SetSize((this->geometry.width - this->button->BitmapWidth()), this->button->BitmapHeight());
  this->label->SetText("(No Select)", 0);
  this->label->Move(2, 3);
  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x008994B0
// FUNCTION: fn_11555
int UINewComboBoxCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 273) {
      this->dropped = !this->dropped;
      if (g_windowMgr.unknown_0214) {
        g_windowMgr.CloseWindow(26);
      }

      if (this->dropped) {
        UIWindow* window = g_windowMgr.EnsureWindow(26);
        const int count = static_cast<int>(this->items.size());
        for (int index = 0; index < count; ++index) {
          window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(this->items[index].c_str()), 0, 0, 0);
        }
        window->SendMsg(nullptr, 40, this->selected, 0, 0, 0);
        window->Resize(this->geometry.width, count * 16);
        static_cast<UIComboBoxWnd*>(window)->SetOwner(this);
        int x = 0;
        int y = this->geometry.height;
        this->ToScreen(x, y);
        window->Move(x, y);
      }
    }
    return 0;
  }

  if (message != 39) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->dropped = false;
  const int index = static_cast<int>(argument1);
  if (index < 0) {
    return 0;
  }

  if (static_cast<unsigned int>(index) <= this->items.size()) {
    this->label->SetText(this->items[index].c_str(), 0);
  }

  this->selected = index;
  this->parent->SendMsg(this, 39, index, 0, 0, 0);
  this->Invalidate();
  this->parent->Invalidate();
  if (this->Root()) {
    this->Root()->Invalidate();
  }
  return 0;
}

// OFFSET: 0x008A1640
// FUNCTION: fn_11636
void UITextViewEX::AddLineLinks(UITextButton* const* first, UITextButton* const* last) {
  std::vector<UITextButton*> links;
  for (; first != last; ++first) {
    links.push_back(*first);
  }

  this->lineLinks.push_back(std::vector<UITextButton*>(links));
}

// INLINED
static void TrimLeading(std::string& text, const char* characters) {
  if (!text.empty()) {
    const std::size_t first = text.find_first_not_of(characters);
    if (first != 0) {
      text.erase(0, first);
    }
  }
}

// OFFSET: 0x008A1880
// FUNCTION: fn_11638
void UIString::ParseTags(std::string text) {
  TrimLeading(text, " \t");

  if (text[0] != '<') {
    this->unknown_00DC = text;
    return;
  }

  const std::size_t begin = text.find_first_not_of("<");
  const std::size_t end = text.find_first_of(">", begin);
  std::string tags = text.substr(begin, end - begin);
  std::size_t position = 0;

  for (;;) {
    std::size_t index = tags.find_first_of("=", position);
    if (index == std::string::npos) {
      break;
    }

    std::string key = tags.substr(position, index - position);
    if (key.empty()) {
      break;
    }

    TrimLeading(key, " \t");
    TrimTrailing(key, " \t");

    if (key == "image") {
      const std::size_t start = tags.find_first_of("\"", index) + 1;
      index = tags.find_first_of("\"", start);
      this->unknown_00AC = tags.substr(start, index - start);
      TrimLeading(this->unknown_00AC, " \t");
      TrimTrailing(this->unknown_00AC, " \t");
      TrimLeading(this->unknown_00AC, "\"");
      TrimTrailing(this->unknown_00AC, "\"");
    } else if (key == "color") {
      const std::size_t start = tags.find_first_of(" \t", index) + 1;
      index = tags.find_first_of(" \t", start);
      const std::string value = tags.substr(start, index - start);
      const std::string red = value.substr(0, 2);
      const std::string green = value.substr(2, 2);
      const std::string blue = value.substr(4, 2);
      char* stop;
      this->unknown_009C = std::strtol(red.c_str(), &stop, 16);
      this->unknown_00A0 = std::strtol(green.c_str(), &stop, 16);
      this->unknown_00A4 = std::strtol(blue.c_str(), &stop, 16);
    } else if (key == "link") {
      const std::size_t start = tags.find_first_of("\"", index) + 1;
      index = tags.find_first_of("\"", start);
      this->unknown_0084 = tags.substr(start, index - start);
      TrimLeading(this->unknown_0084, " \t");
      TrimTrailing(this->unknown_0084, " \t");
      TrimLeading(this->unknown_0084, "\"");
      TrimTrailing(this->unknown_0084, "\"");
      this->unknown_009C = 10;
      this->unknown_00A0 = 10;
      this->unknown_00A4 = 255;
    } else if (key == "font") {
      const std::size_t start = tags.find_first_of("\"", index) + 1;
      index = tags.find_first_of("\"", start);
      this->unknown_00C4 = tags.substr(start, index - start);
      TrimLeading(this->unknown_00C4, " \t");
      TrimTrailing(this->unknown_00C4, " \t");
      TrimLeading(this->unknown_00C4, "\"");
      TrimTrailing(this->unknown_00C4, "\"");
    } else if (key == "bold") {
    } else if (key == "size") {
      const std::size_t start = tags.find_first_of(" \t", index) + 1;
      index = tags.find_first_of(" \t", start);
      const std::string value = tags.substr(start, index - start);
      this->unknown_00A8 = std::atoi(value.c_str());
    }

    position = index + 1;
  }

  this->unknown_00DC = text.substr(end + 1, text.find_first_of("<", end + 1) - (end + 1));
}
