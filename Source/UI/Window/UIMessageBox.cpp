#include "Client/ReplayPacketRecorder.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Resource/SkinPalette.h"
#include "UI/Window/UICntWnd.h"
#include "UI/Window/UIChatRoomWnd.h"
#include "UI/Window/UIBank_NewWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Session.h"
#include "Utility/Text/GroupedInteger.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UILoginWnd.h"
#include "UI/Window/UIMessageBox.h"
#include "UI/Window/UINoticeWnd.h"
#include "UI/Window/UISelectReplayDataWnd.h"
#include "UI/Window/UISelectServerWnd.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>
#include <shellapi.h>

namespace {
  constexpr char prefix[] = "유저인터페이스\\";

  // INLINED
  std::uint32_t Palette(ResourceMgr& resources, int x, int y) {
    const auto* resource = static_cast<CBitmapRes*>(resources.Load((std::string(prefix) + "colorchip.bmp").c_str()));
    const auto& image = resource->image;
    const std::uint32_t pixel = x >= 0 && y >= 0 && x < image.width && y < image.height ? image.pixels[y * image.width + x] : 0x00FF0000u;

    return ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  }
}

// OFFSET: 0x008A3FC0
// FUNCTION: fn_11663
UIMessageBox::UIMessageBox(UIWindowContext& windows, UIFrameContext& frames, UIMessageBoxContext& dialog)
    : UIFrameWnd(windows, frames, 0), counting(0), protectedMessage(0), m_dialog(&dialog) {
}

// OFFSET: 0x008A5220
// OFFSET: 0x008A64E0
// FUNCTION: fn_11680
UIMessageBox::~UIMessageBox() = default;

// OFFSET: 0x008B8E60
// FUNCTION: fn_11768
void UIMessageBox::Paint() {
  auto& controls = this->m_dialog->text.buttons;
  const auto load = [&](const char* name) { return static_cast<CBitmapRes*>(controls.resources.Load((std::string(prefix) + name).c_str())); };
  int leftWidth = 0;
  int rightWidth = 0;
  int titleHeight = 0;
  const int width = this->geometry.width;
  const int height = this->geometry.height;

  if (const auto* left = load("basic_interface\\titlebar_left.bmp")) {
    this->DrawBitmap(0, 0, &left->image, false);
    leftWidth = left->image.width;
  }

  if (const auto* right = load("basic_interface\\titlebar_right.bmp")) {
    rightWidth = right->image.width;
    this->DrawBitmap(width - rightWidth, 0, &right->image, false);
  }

  if (const auto* middle = load("basic_interface\\titlebar_mid.bmp")) {
    titleHeight = middle->image.height;
    const int tileWidth = middle->image.width;
    const int extent = width - rightWidth - leftWidth;
    const int tiles = extent / tileWidth + (extent % tileWidth != 0);

    for (int index = 0; index < tiles; ++index) {
      this->DrawBitmap(leftWidth, 0, &middle->image, false);
      leftWidth += tileWidth;
    }
  }

  this->DrawTextAt(controls.fonts, controls.locale, 19, titleHeight - 13, this->caption.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 18, titleHeight - 14, this->caption.c_str(), 0, 0, 12, 0, 0, 0);
  const auto* base = load("basic_interface\\sys_base_off.bmp");
  this->DrawBitmap(3, 3, base ? &base->image : nullptr, true);

  const auto border = Palette(controls.resources, 14, 6);
  DrawRoundedBorder(*this, 0, 17, width, height - 17, border);
  this->FillRegion(0, 17, width, 5, border);
  const auto background = Palette(controls.resources, 2, 2);
  this->FillRegion(1, 17, width - 2, height - 19, background);
  this->FillRegion(2, height - 2, width - 4, 1, background);
}

// OFFSET: 0x008B7A90
// FUNCTION: fn_11756
void UIMessageBox::Slot18() {
}

// OFFSET: 0x008B7AA0
// FUNCTION: fn_11757
void UINoticeWnd::Slot18() {
}

// OFFSET: 0x008B7AB0
// FUNCTION: fn_11758
void UISelectReplayDataWnd::Slot18() {
}

// OFFSET: 0x008B7AC0
// FUNCTION: fn_11759
void UISelectServerWnd::Slot18() {
}

// OFFSET: 0x008B8610
// FUNCTION: fn_11765
void UILoginWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x008A6C20

// OFFSET: 0x008C4B00
// FUNCTION: fn_11888
bool UIBank_NewWnd::IsNumeric(std::string text) {
  for (unsigned char character : text) {
    if (static_cast<unsigned char>(character - '0') > 9) {
      return false;
    }
  }
  return true;
}

// OFFSET: 0x008B7AD0

// OFFSET: 0x008B7CF0

// OFFSET: 0x008B7DA0
// FUNCTION: fn_11762
void UICntWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008A91E0
// FUNCTION: fn_11721
void UICntWnd::DrawBorder(int x, int y, int width, int height) {
  if (width == 0) {
    width = this->geometry.width;
  }
  if (height == 0) {
    height = this->geometry.height;
  }
  auto& names = *this->m_textureNames;
  const auto load = [&](const char* name) {
    return static_cast<CBitmapRes*>(names.resources.Load(names.Resolve(name)));
  };
  const int innerWidth = width - 28;
  const int columns = innerWidth / 14 + (innerWidth % 14 != 0);
  for (int i = 0; i < columns; ++i) {
    const auto* bitmap = load("유저인터페이스\\sysbox_mu.bmp");
    this->DrawBitmap(x + 14 + i * 14, y, &bitmap->image, false);
  }
  const auto* bitmap = load("유저인터페이스\\sysbox_lu.bmp");
  this->DrawBitmap(x, y, &bitmap->image, false);
  bitmap = load("유저인터페이스\\sysbox_ru.bmp");
  this->DrawBitmap(x + width - bitmap->image.width, y, &bitmap->image, false);
  const int innerHeight = height - 28;
  const int rows = innerHeight / 14 + (innerHeight % 14 != 0);
  for (int i = 0; i < rows; ++i) {
    bitmap = load("유저인터페이스\\sysbox_lm.bmp");
    this->DrawBitmap(x, y + 14 + i * 14, &bitmap->image, false);
  }
  for (int i = 0; i < rows; ++i) {
    bitmap = load("유저인터페이스\\sysbox_rm.bmp");
    this->DrawBitmap(x + width - bitmap->image.width, y + 14 + i * 14, &bitmap->image, false);
  }
  for (int i = 0; i < columns; ++i) {
    bitmap = load("유저인터페이스\\sysbox_md.bmp");
    this->DrawBitmap(x + 14 + i * 14, y + height - bitmap->image.height, &bitmap->image, false);
  }
  bitmap = load("유저인터페이스\\sysbox_ld.bmp");
  this->DrawBitmap(x, y + height - bitmap->image.height, &bitmap->image, false);
  bitmap = load("유저인터페이스\\sysbox_rd.bmp");
  this->DrawBitmap(x + width - bitmap->image.width, y + height - bitmap->image.height, &bitmap->image, false);
  unsigned red, green, blue;
  SkinPaletteChannels(names.resources, names, 2, 2, red, green, blue);
  this->FillRegion(x + 14, y + 14, innerWidth, innerHeight, RGB(red, green, blue));
}

// INLINED
void UIMessageBox::FormatCountdown() {
  char text[512];
  std::sprintf(text, this->format.c_str(), this->remaining);
  this->messageText = text;
}

// INLINED
void UIMessageBox::AddButton(const char* name, int id, int x, int y) {
  auto& controls = this->m_dialog->text.buttons;
  auto* button = new UIBitmapButton(*this->m_context, controls);
  const char* suffixes[] = {"", "_a", "_b"};

  for (int state = 0; state < 3; ++state) {
    const std::string path = std::string(prefix) + name + suffixes[state] + ".bmp";
    button->SetBitmap(path.c_str(), state, 0);
  }

  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(id);
  this->AddChild(button);
}

// OFFSET: 0x008A7350
// FUNCTION: fn_11713
void UIMessageBox::Configure(
  const char* text, int type, bool defaultAccept, int timeout, const char* title, int width, int height, int modalValue, const char* icon
) {
  this->caption = title;
  this->messageText = text;
  this->format = text;
  this->started = timeGetTime();
  this->deadline = timeGetTime() + static_cast<unsigned>(timeout);
  const unsigned interval = this->deadline - this->started;
  this->remaining = interval / 1000 + (interval % 1000 != 0);
  this->counting = timeout != 0;

  if (this->counting != 0) {
    this->FormatCountdown();
  }

  this->modal = modalValue;
  this->SetSize(width, height);
  this->Slot15(width, height);
  this->Slot22(width, height);
  this->Move((this->m_dialog->screenWidth - 640) / 2 + 185, (this->m_dialog->screenHeight - 480) / 2 + 180);
  int inset = 0;

  if (icon) {
    auto* image = new UIStaticBitmap(*this->m_context, this->m_dialog->text.buttons);
    image->SetBitmap(icon);
    image->SetSize(image->geometry.width, image->geometry.height);
    image->Move(10, 22);
    this->AddChild(image);
    inset = image->geometry.width + image->geometry.x;
  }

  this->viewer = new UITextViewer(*this->m_context, this->m_dialog->text);
  this->viewer->Configure(inset + 10, 22, width - inset - 20, height - 55, 0);
  this->viewer->backgroundColor = Palette(this->m_dialog->text.buttons.resources, 2, 2) | 0xFF000000u;
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->messageText.data()), static_cast<int>(static_cast<double>(width) / 7.5), 0, 0);
  this->m_message0 = type == 2 ? 188 : 185;

  switch (type) {
  case 0:
    this->AddButton("btn_ok", 184, width - 46, height - 24);
    this->m_message0 = 184;
    break;
  case 2:
    this->AddButton("btn_ok", 187, width - 91, height - 24);
    this->AddButton("btn_cancel", 188, width - 46, height - 24);
    this->m_message0 = defaultAccept ? 187 : 188;
    this->m_message1 = 188;
    break;
  case 4:
    this->AddButton("btn_cancel", 185, width - 46, height - 24);
    this->m_message0 = 185;
    break;
  case 5: {
    this->AddButton("btn_ok", 184, width - 46, height - 24);
    this->m_message0 = 184;
    auto& controls = this->m_dialog->text.buttons;

    auto* button = new UITextButton(*this->m_context, controls);
    button->Configure(controls.services.MessageText(3445), 0x00F4934A, 0, 12, 0, 0);
    button->m_hoverColor = 0x00FF7864;
    button->Move(14, 70);
    button->SetId(426);
    button->SetDisabled(0);
    button->Invalidate();
    this->AddChild(button);
    break;
  }
  default:
    break;
  }
}

// OFFSET: 0x008BADC0
// FUNCTION: fn_11791
void UIMessageBox::Slot19() {
  if (this->counting == 0) {
    return;
  }

  if (timeGetTime() > this->deadline) {
    this->m_dialog->response = 187;
    this->counting = 0;
    return;
  }

  const unsigned previous = this->remaining;
  const bool fraction = (this->deadline - timeGetTime()) % 1000 != 0;
  this->remaining = (this->deadline - timeGetTime()) / 1000 + fraction;

  if (previous != this->remaining) {
    this->FormatCountdown();
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->messageText.data()), 38, 0, 0);
  }
}

// OFFSET: 0x008BF950
// FUNCTION: fn_11808
int UIMessageBox::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    this->m_dialog->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);

    if (this->modal != 0) {
      this->m_dialog->response = static_cast<int>(a);
    } else if (a == 184 || a == 187 || a == 188) {
      this->m_dialog->retirement.Schedule(this);
    }

    return 0;
  case 57:
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, a, 38, 0, 0);
    return 0;
  case 67:
    this->format = reinterpret_cast<const char*>(a);
    return 0;
  case 98:
    if (a == 426) {
      auto& controls = this->m_dialog->text.buttons;
      ShellExecuteA(this->m_dialog->text.applicationWindow, "open", controls.services.MessageText(3445), nullptr, nullptr, SW_SHOWNORMAL);
    }

    return 0;
  case 156:
    this->modal = static_cast<int>(a);
    return 0;
  case 157:
    this->protectedMessage = a > 0;
    return 0;
  case 158:
    return this->protectedMessage == 1;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}
