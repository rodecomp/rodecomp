#include "UI/Window/UINormalMB.h"
#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>

// OFFSET: 0x00946C00
// FUNCTION: fn_12851
UINormalMB::UINormalMB(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0), m_owner(nullptr), m_viewer(nullptr) {
  this->m_keepOpen = 0;
}

// OFFSET: 0x0094D840
// FUNCTION: fn_12906
void UINormalMB::Paint() {
  UIButtonContext& controls = this->m_textContext->buttons;
  this->DrawTitleBar(controls.fonts, controls.locale, true, this->m_title.c_str(), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00950740
// FUNCTION: fn_12943
int UINormalMB::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    const int id = static_cast<int>(a);
    const auto action = this->m_actions.find(id);

    if (action != this->m_actions.end()) {
      action->second();
    } else {
      if (this->m_owner) {
        this->m_owner->SendMsg(this, 6, 488, id, 0, 0);
      }

      if (this->m_keepOpen) {
        g_windowMgr.unknown_0474 = id;
        return 0;
      }
    }

    g_windowMgr.Schedule(this);
    return 0;
  }

  case 57:
    this->m_viewer->Clear();
    this->m_viewer->SendMsg(nullptr, 29, a, 38, 0, 0);
    return 0;

  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// INLINED
__forceinline void UINormalMB::AddButton(const char* normal, const char* hover, const char* pressed, int x, int y, int id) {
  auto* button = new UIBitmapButton(*this->m_context, this->m_textContext->buttons);
  button->SetBitmap(normal, 0, 0);
  button->SetBitmap(hover, 1, 0);
  button->SetBitmap(pressed, 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(id);
  this->AddChild(button);
}

// OFFSET: 0x009487B0
// FUNCTION: fn_12880
void UINormalMB::Create(UIWindow* owner, const char* text, int type, bool defaultAccept, const char* title, int width, int height, const char* icon) {
  this->m_owner = owner;
  this->m_message = text;
  this->m_title = title ? title : LookupMessage(3700);

  this->SetSize(width, height);
  this->Slot15(width, height);
  this->Slot22(width, height);
  this->Move(static_cast<int>((g_renderer->viewport.width - width) * 0.5), static_cast<int>((g_renderer->viewport.height - height) * 0.5));

  int inset = 0;

  if (icon) {
    auto* image = new UIStaticBitmap(*this->m_context, this->m_textContext->buttons);
    image->SetBitmap(icon);
    image->SetSize(image->geometry.width, image->geometry.height);
    image->Move(10, 22);
    this->AddChild(image);
    inset = image->geometry.width + image->geometry.x;
  }

  this->m_viewer = new UITextViewer(*this->m_context, *this->m_textContext);
  this->m_viewer->Configure(inset + 10, 22, width - inset - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = RGB(red, green, blue) | 0xFF000000u;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->m_message.c_str()), static_cast<int>(width / 7.5), 0, 0);

  switch (type) {
  case 0:
    this->AddButton("유저인터페이스\\btn_ok.bmp", "유저인터페이스\\btn_ok_a.bmp", "유저인터페이스\\btn_ok_b.bmp", width - 46, height - 24, 184);
    this->m_message0 = 184;
    break;

  case 2:
    this->AddButton("유저인터페이스\\btn_ok.bmp", "유저인터페이스\\btn_ok_a.bmp", "유저인터페이스\\btn_ok_b.bmp", width - 91, height - 24, 187);
    this->AddButton("유저인터페이스\\btn_cancel.bmp", "유저인터페이스\\btn_cancel_a.bmp", "유저인터페이스\\btn_cancel_b.bmp", width - 46, height - 24, 188);
    this->m_message1 = 188;
    this->m_message0 = defaultAccept ? 187 : 188;
    break;

  case 4:
    this->AddButton("유저인터페이스\\btn_cancel.bmp", "유저인터페이스\\btn_cancel_a.bmp", "유저인터페이스\\btn_cancel_b.bmp", width - 46, height - 24, 185);
    this->m_message0 = 185;
    break;

  default:
    break;
  }
}
