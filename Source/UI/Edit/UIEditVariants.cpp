#include "Client/Input/Language.h"
#include "Contents/SkinMgr.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/FormatString.h"
#include <cstring>
#include <windows.h>

// OFFSET: 0x0084F250
// FUNCTION: fn_10657
UIEditCtrl2::UIEditCtrl2(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit) {
}

// OFFSET: 0x008515E0
// OFFSET: 0x00852700
// FUNCTION: fn_10692
UIEditCtrl2::~UIEditCtrl2() = default;

// OFFSET: 0x00863BF0
// FUNCTION: fn_10911
int UIEditCtrl2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    InputFocus& focus = this->m_edit->focus;
    if (focus.IsLast(focus.window)) {
      focus.First();
    } else {
      focus.Next();
    }

    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00863C70

// OFFSET: 0x0085E420

// OFFSET: 0x0084F570
// FUNCTION: fn_10659
UIEditCtrl3::UIEditCtrl3(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl2(windows, edit) {
  this->unknown_0130 = 0;
  this->unknown_0134 = 0;
  this->unknown_0138 = 14;
  this->unknown_013C = 0;
  this->unknown_0140 = 0;
  this->unknown_0144 = 0;
  this->unknown_0148 = 0;
  this->unknown_014C = 0;
  this->unknown_0150 = 0;
  this->unknown_0154 = 0;
  this->unknown_0158 = 0;
  this->unknown_015C = 0;
  this->unknown_015E = 0;
  this->unknown_0184 = 0;
  this->unknown_0160 = 0;
  this->unknown_0164 = 0;
}

// OFFSET: 0x0084F2B0

// OFFSET: 0x0085AC40
// FUNCTION: fn_10786
void UIEditCtrl3::Slot15(int width, int height) {
  auto* scrollBar = new UIScrollBar(*this->m_context);
  this->unknown_0130 = scrollBar;
  scrollBar->Configure(width - 10, 0, 1, height);
  this->AddChild(scrollBar);
}

// OFFSET: 0x00863280
// FUNCTION: fn_10906
void UIEditCtrl3::Resize(int width, int height) {
  this->UIWindow::Resize(width, height);
  this->unknown_0130->Move(this->geometry.width - 10, 0);
  this->unknown_0130->Resize(this->unknown_0130->geometry.width, height);
  using ResizeHandler = void(__thiscall*)(UIEditCtrl3*);
  reinterpret_cast<ResizeHandler>((*reinterpret_cast<void***>(this))[58])(this);
}

// OFFSET: 0x00865220
// FUNCTION: fn_10922
int UIWhisperEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 18) {
    if (!this->history.empty()) {
      if (this->historyIndex != 0) {
        --this->historyIndex;
      }
      this->SetText(this->history[this->historyIndex].c_str());
      this->Slot23();
    }
  } else if (message == 19) {
    if (!this->history.empty() && static_cast<std::size_t>(this->historyIndex) < this->history.size()) {
      ++this->historyIndex;
      this->SetText(this->historyIndex == static_cast<int>(this->history.size()) ? "" : this->history[this->historyIndex].c_str());
      this->Slot23();
    }
  } else {
    return this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00867FC0

// OFFSET: 0x00853760
// FUNCTION: fn_10741
void UIEditCtrl_OldList::RecallHistory(int step) {
  if (this->historyIndex == -1) {
    return;
  }

  this->historyIndex += step;
  if (this->historyIndex < 0) {
    this->historyIndex = static_cast<int>(this->history.size()) - 1;
  }

  if (static_cast<std::size_t>(this->historyIndex) >= this->history.size()) {
    this->historyIndex = 0;
  }

  this->SetText(this->history[this->historyIndex].c_str());
  this->Slot23();
  this->Invalidate();
}

// OFFSET: 0x00853810
// FUNCTION: fn_10742
int UIEditCtrl::MeasureText(const std::string& text, const int& bytes) {
  const TextLocale locale = {};
  return this->TextWidth(g_fontCache, locale, text.c_str(), bytes, 0, 12, 0, 0);
}

// OFFSET: 0x0084F7B0
// FUNCTION: fn_10660
UIEditCtrl4::UIEditCtrl4(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit), placeholderColor(0xFFFFFFFFu), placeholderEnabled(0) {
}

// OFFSET: 0x00852870
UIEditCtrl4::~UIEditCtrl4() = default;

// OFFSET: 0x0085AA40
// FUNCTION: fn_10782
void UIEditCtrl4::Slot23() {
  this->text.clear();
  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
}

// OFFSET: 0x008563D0
// FUNCTION: fn_10754
void UIEditCtrl4::PaintText() {
  this->PaintTextWithPolicy(true, &this->placeholder, this->placeholderColor, this->placeholderEnabled == 1);
}

// OFFSET: 0x00869800
// FUNCTION: fn_10972
void UIEditCtrl4::SetPlaceholderEnabled(bool enabled) {
  this->placeholderEnabled = enabled;
}

// OFFSET: 0x00867960
// FUNCTION: fn_10943
void UIEditCtrl4::SetPlaceholder(const char* value) {
  this->placeholder.assign(value, std::strlen(value));
  this->Trim();

  if (this->m_edit->focus.window == this) {
    this->m_edit->input.ResetInput();
    this->m_edit->input.ResetInput();
    this->m_edit->input.Insert(this->placeholder.c_str());
  }

  this->SelectAll();

  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x008679F0
// FUNCTION: fn_10944
void UIEditCtrl4::SetPlaceholderColor(std::uint32_t color) {
  this->placeholderColor = color;
}

// OFFSET: 0x0084F7B0
// FUNCTION: fn_10661
UIEditCtrl5::UIEditCtrl5(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl4(windows, edit) {
}

// OFFSET: 0x008528F0
UIEditCtrl5::~UIEditCtrl5() = default;

// OFFSET: 0x00864140
// FUNCTION: fn_10913
int UIEditCtrl5::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    InputFocus& focus = this->m_edit->focus;
    if (focus.IsLast(focus.window)) {
      focus.First();
    } else {
      focus.Next();
    }

    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0085CBF0
// FUNCTION: fn_10806
void UIEditCtrl3::Paint() {
  const bool selected = g_skinInfo.Selected();
  const SkinPaletteCell cell = {1, 2};
  const std::uint32_t color = SkinPaletteColor(ResourceMgr::Instance(), cell, selected);
  this->red = color & 0xFF;
  this->blue = (color >> 16) & 0xFF;
  this->green = (color & 0xFFFF) >> 8;

  this->Invalidate();
  this->ClearSurface(static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16));
  this->PaintText();
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0085FF60
// FUNCTION: fn_10892
void UIEditCtrl3::Slot35(int value) {
  if (this->unknown_0134 == 0) {
    return;
  }

  if (value < 0) {
    this->SendMsg(this->unknown_0130, 7, 3, 0, 0, 0);
  } else {
    this->SendMsg(this->unknown_0130, 7, -3, 0, 0, 0);
  }
}

// OFFSET: 0x00868300
// FUNCTION: fn_10958
void UIEditCtrl3::Trim() {
  const int limit = this->maximum - this->unknown_00F0;
  const auto length = this->text.size();

  if (static_cast<int>(length) <= limit) {
    return;
  }

  const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), 0);

  while (length - std::strlen(next) <= static_cast<unsigned>(limit)) {
    next = CharNextExA(static_cast<WORD>(g_textCodePage), next, 0);
  }

  const char* previous = CharPrevExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), next, 0);
  this->text.resize(length - std::strlen(previous));

  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());

  while (this->text.size() < static_cast<unsigned>(limit)) {
    this->text += " ";
  }
}

// OFFSET: 0x0085BB60
// FUNCTION: fn_10796
void UIBitmapEditCtrl::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  this->textY = 3;
  this->PaintText();
}

// OFFSET: 0x00865590
// FUNCTION: fn_10927
void UIBitmapEditCtrl::SetBitmapName(const char* name) {
  this->bitmapName.assign(name, std::strlen(name));

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  this->geometry.width = bitmap->image.width;
  this->geometry.height = bitmap->image.height;
}

// OFFSET: 0x0084EE90
// FUNCTION: fn_10654
UIChatEditCtrl::UIChatEditCtrl(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit), unknown_0140(0), historyLimit(30) {
  this->history.reserve(this->historyLimit);
  this->unknown_0144 = 0;
}

// OFFSET: 0x0085EB50
// FUNCTION: fn_10840
void UIChatEditCtrl::Slot25(int x, int y) {
  if (this->disabled == 0) {
    this->m_edit->focus.Set(this);
  }

  this->parent->Slot25(x + this->geometry.x, y + this->geometry.y);
}

// OFFSET: 0x00863640
// FUNCTION: fn_10909
int UIChatEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 18) {
    if (this->history.size() != 0) {
      if (this->unknown_0140 == this->history.size()) {
        this->pending = this->Text();
      }
      if (this->unknown_0140 != 0) {
        --this->unknown_0140;
      }

      this->SetText(this->history[this->unknown_0140].c_str());
      this->Slot23();
    }
  } else if (message == 19) {
    if (this->history.size() != 0 && this->unknown_0140 < this->history.size()) {
      ++this->unknown_0140;
      this->SetText(this->unknown_0140 == this->history.size() ? this->pending.c_str() : this->history[this->unknown_0140].c_str());
      this->Slot23();
    }
  } else {
    return UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0085CDE0
// FUNCTION: fn_10808
void UIEditCtrl_OldList::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (!this->bitmapName.empty()) {
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
    this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  }

  this->PaintText();
}

// OFFSET: 0x0085FFC0
// FUNCTION: fn_10894
void UIEditCtrl_OldList::Slot35(int value) {
  if (value < 0) {
    this->SendMsg(nullptr, 19, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, 18, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0085E3D0
// FUNCTION: fn_10827
void UIEditCtrlVarId::Slot24(int) {
  this->Synchronize();
  this->Invalidate();
  this->SendMsg(this, 0, 0, 0, 0, 0);
}

// OFFSET: 0x0085FFA0
// FUNCTION: fn_10893
void UIEditCtrlVarId::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00864CE0
// FUNCTION: fn_10915
int UIEditCtrlVarId::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 0) {
    return UIEditCtrl::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (this->parent) {
    this->parent->SendMsg(this, 6, this->geometry.id, 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x00868160
// FUNCTION: fn_10953
void UIEditCtrlVarId::SetText(const char* value) {
  this->UIEditCtrl::SetText(value);
  this->Invalidate();
}

// OFFSET: 0x0085AB10
// FUNCTION: fn_10784
void UIWhisperEditCtrl::Slot23() {
  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
  this->SelectAll();
}

// OFFSET: 0x00868230
// FUNCTION: fn_10956
void UIWhisperEditCtrl::SetText(const char* value) {
  this->UIEditCtrl::SetText(value);

  if (std::strlen(value) == 0) {
    this->historyIndex = static_cast<int>(this->history.size());
  }
}

// OFFSET: 0x00864D20
// FUNCTION: fn_10916
int UIEditCtrl_OldList::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 18:
    this->RecallHistory(-1);
    return 0;
  case 19:
    this->RecallHistory(1);
    return 0;
  default:
    return UIEditCtrl::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x00886F90
// FUNCTION: fn_11330
void UIShowAndEditTimeCtrl::Slot15(int, int) {
  for (int i = 0; i < 2; ++i) {
    UIEditCtrlVarId*& field = this->fields[i];
    field = new UIEditCtrlVarId(*this->m_context, *this->m_edit);
    field->SetSize(20, 18);
    field->Background(232, 232, 232);
    field->Border(192, 192, 192);
    field->flat = 0;
    field->maximum = 2;
    field->Move(i * 38, 0);
    field->SetText("0");
    field->SetId(184);
    this->AddChild(field);
  }
}

// OFFSET: 0x008996B0
// FUNCTION: fn_11556
int UIShowAndEditTimeCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6 && argument1 == 184) {
    for (UIEditCtrlVarId* field : this->fields) {
      if (sender == field) {
        int value = std::atoi(field->Text());
        if (value > 60) {
          value = 60;
        }
        const std::string text = FormatOwnedString("%d", value);
        field->SetText(text.c_str());
      }
    }
  }
  return 0;
}

// OFFSET: 0x00853F20
