#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/WindowPolicy.h"

namespace {
  constexpr char prefix[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\";
}

// OFFSET: 0x0086CC20
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
      static_cast<UIWindow*>(this->parent)->Invalidate();
      this->Invalidate();
      this->list->Invalidate();
      UIWindow* root = this;

      while (root->parent) {
        root = static_cast<UIWindow*>(root->parent);
      }

      root->Invalidate();

      if (this->parent) {
        static_cast<UIWindow*>(this->parent)->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
    }

    return 0;
  case 39:
    if (WindowPolicy(this->m_controls->buttons.policy, this->m_controls->buttons.restricted, 3, static_cast<UIWindow*>(this->parent)->geometry.id) == 0 ||
        value < 0) {
      return 0;
    }

    this->label->SetText(this->list->lines[value].text.c_str(), 0);
    this->SendMsg(nullptr, 6, 273, 0, 0, 0);
    static_cast<UIWindow*>(this->parent)->SendMsg(this, 39, value, 0, 0, 0);
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

// OFFSET: 0x0086CBA0
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
