#include "Client/ReplayPacketRecorder.h"
#include "Contents/Items/ItemDescription.h"
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
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIReplayListBox.h"
#include "UI/Window/UISelectReplayDataWnd.h"
#include "UI/Window/UISelectServerWnd.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIMerInfoWnd.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/UIMessageBoxAutoreturn.h"
#include "UI/Window/UIMonsterInfoWnd.h"
#include "UI/Window/UITalkboxTrapInputWnd.h"
#include "UI/Window/UIHomunInfoWnd.h"
#include "UI/Window/UIPetInfoWnd.h"
#include "UI/Window/UIReplayRECControlWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Registry.h"
#include "Utility/Cipher/XorBytes.h"
#include "UI/Window/UILoginOTPWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Buttons/UIButton.h"
#include "Render/Text/TextContext.h"
#include "Contents/MessageTable.h"
#include "UI/Text/ColoredText.h"
#include "Client/Mode/CMode.h"
#include "UI/Window/UIPayConfirmWnd.h"
#include "UI/Window/UIReplayControlWnd.h"
#include "UI/Window/UIMakeCharWnd.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Replay.h"
#include "Client/Application/Launcher.h"
#include "UI/Lists/UIChatHisBox.h"
#include "Client/Application/ApplicationWindow.h"
#include "UI/Text/UIStaticText.h"
#include "UI/UIBarGraph.h"
#include "Utility/Text/FormatString.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include <cstring>
#include <format>
#include "Client/Mode/CGameMode.h"
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>
#include <shellapi.h>
#include <imm.h>
#include <direct.h>
#include <io.h>
#include "UI/Lists/UIListBox.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "UI/Window/UINewSelectCharWnd.h"

namespace {
  constexpr char prefix[] = "유저인터페이스\\";

  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };

  // INLINED
  std::uint32_t Palette(ResourceMgr& resources, int x, int y) {
    const auto* resource = static_cast<CBitmapRes*>(resources.Load((std::string(prefix) + "colorchip.bmp").c_str()));
    const auto& image = resource->image;
    const std::uint32_t pixel = x >= 0 && y >= 0 && x < image.width && y < image.height ? image.pixels[y * image.width + x] : 0x00FF0000u;

    return ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  }

  // INLINED
  void DrawTextRight(UIWindow& window, FontCache& fonts, const TextLocale& locale, int x, int y, const char* text, int bytes,
                     int family, int height, std::uint32_t color, unsigned char bold) {
    if (!bytes) {
      bytes = static_cast<int>(std::strlen(text));
    }
    TextContext context(*window.geometry.surface);
    context.SetFont(family, height, bold, 0);
    const int width = context.Measure(text, static_cast<int>(std::strlen(text))).cx;
    context.SetColor(color);
    context.Draw(x - width, y, text, bytes);
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

// OFFSET: 0x008AC410
// FUNCTION: fn_11729
bool UIBank_NewWnd::Slot03() {
  return true;
}

// OFFSET: 0x008AC420
// FUNCTION: fn_11730
bool UIReplayControlWnd::Slot03() {
  return true;
}

// OFFSET: 0x008AC430
// FUNCTION: fn_11731
bool UIReplayRECControlWnd::Slot03() {
  return true;
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

// OFFSET: 0x008B88F0
// FUNCTION: fn_11767
void UIMerInfoWnd::Paint() {
  auto& controls = *this->m_controls;
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\homuninfo_bg.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
  char title[50] = {};
  const unsigned kind = g_session.m_mercenaryClass;
  const int titleId = kind - 6017 <= 9 ? 1181 : kind - 6037 <= 9 ? 1182 : kind - 6027 <= 9 ? 1183 : 1367;
  std::sprintf(title, controls.services.MessageText(titleId));
  this->DrawTextAt(controls.fonts, controls.locale, 5, 2, title, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 24, controls.services.MessageText(407), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 47, controls.services.MessageText(408), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 111, controls.services.MessageText(1184), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 135, controls.services.MessageText(1185), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 100, 159, controls.services.MessageText(1186), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 230, 133, controls.services.MessageText(1187), 0, 0, 12, 0, 0, 0);
  char number[24] = {};
  for (int i = 0; i < 8; ++i) {
    const int value = i == 7 ? (2000 - g_session.m_mercenaryStats[i]) / 10 : g_session.m_mercenaryStats[i];
    const int bytes = std::sprintf(number, "%d", value);
    DrawTextRight(*this, controls.fonts, controls.locale, 79, 24 + 18 * i, number, bytes, 1, 14, 0, 0);
  }
  this->DrawTextAt(controls.fonts, controls.locale, 136, 24, g_session.m_mercenaryName, 0, 0, 12, 0x646464, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 135, 24, g_session.m_mercenaryName, 0, 0, 12, 0, 0, 0);
  char level[12] = {};
  _itoa(g_session.m_mercenaryLevel, level, 10);
  this->DrawTextAt(controls.fonts, controls.locale, 135, 47, level, 0, 0, 12, 0, 0, 0);
  if (g_session.m_mercenaryClass != -1) {
    char text[128] = {};
    int bytes = std::sprintf(text, "%d / %d", g_session.m_mercenaryHp, g_session.m_mercenaryMaxHp);
    this->DrawTextAt(controls.fonts, controls.locale, 218, 70, text, bytes, 1, 14, 0, 0, 0);
    bytes = std::sprintf(text, "%d / %d", g_session.m_mercenarySp, g_session.m_mercenaryMaxSp);
    this->DrawTextAt(controls.fonts, controls.locale, 218, 85, text, bytes, 1, 14, 0, 0, 0);
    std::strftime(text, 128, controls.services.MessageText(1260), _localtime32(&g_session.m_mercenaryExpiration));
    this->DrawTextAt(controls.fonts, controls.locale, 155, 111, text, static_cast<int>(std::strlen(text)), 0, 12, 255, 0, 0);
    bytes = std::sprintf(text, "%d", g_session.m_mercenaryLoyalty);
    this->DrawTextAt(controls.fonts, controls.locale, 170, 133, text, bytes, 1, 14, 0, 0, 0);
    bytes = std::sprintf(text, "%d", g_session.m_mercenarySummons);
    this->DrawTextAt(controls.fonts, controls.locale, 170, 157, text, bytes, 1, 14, 0, 0, 0);
    bytes = std::sprintf(text, "%d", g_session.m_mercenaryKills);
    this->DrawTextAt(controls.fonts, controls.locale, 245, 157, text, bytes, 1, 14, 0, 0, 0);
  }
}

// OFFSET: 0x008A6E20
// FUNCTION: fn_11712
long long UIBank_NewWnd::TransactionAmount(unsigned char deposit) {
  auto& services = this->m_controls->services;
  std::string text = this->m_amountEdit->Text();
  this->m_amountEdit->SetText("");
  const auto fail = [&](int notice, int status) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(services.MessageText(notice)), 255, 0, 0, 0);
    this->m_status = services.MessageText(status);
    return 0LL;
  };
  if (text == std::string(services.MessageText(2778))) {
    const long long cash = g_session.m_zeny;
    const long long balance = g_session.m_bankBalance;
    const long long excess = cash + balance > 0x7FFFFFFF ? cash + balance - 0x7FFFFFFF : 0;
    if (deposit) {
      if (balance >= 0x7FFFFFFF) {
        return fail(2787, 2783);
      }
      if (g_session.m_zeny == 0) {
        return fail(2770, 2780);
      }
      const long long amount = cash - excess;
      if (amount < 0) {
        return fail(2465, 2780);
      }
      return amount;
    }
    if (balance == 0) {
      return fail(2770, 2780);
    }
    const long long amount = balance - excess;
    return cash + amount > 0x7FFFFFFF ? static_cast<long long>(0x7FFFFFFF - g_session.m_zeny) : amount;
  }
  if (text.empty()) {
    return fail(2779, 2781);
  }
  if (!this->IsNumeric(text)) {
    return fail(2488, 2782);
  }
  const long long amount = _atoi64(text.c_str());
  if (amount > 0x7FFFFFFF) {
    return fail(2768, 2783);
  }
  if (amount < 1) {
    return fail(2769, 2784);
  }
  return amount;
}

// OFFSET: 0x008A6C20
// FUNCTION: fn_11710
long long UIBank_NewWnd::AdjustAmount(int amount) {
  std::string text = this->m_amountEdit->Text();
  this->m_amountEdit->SetText("");
  long long result = amount;
  if (!text.empty() && this->IsNumeric(text)) {
    result += _atoi64(text.c_str());
  }
  if (result > 0x7FFFFFFF) {
    result = 0x7FFFFFFF;
  } else if (result < 0) {
    result = 0;
  }
  return result;
}

// OFFSET: 0x008C4B00
// FUNCTION: fn_11888
bool UIBank_NewWnd::IsNumeric(std::string text) {
  if (text.size() == 0) {
    return false;
  }

  for (auto it = text.begin(); it != text.end(); ++it) {
    if (static_cast<unsigned char>(*it - '0') > 9) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x008B7AD0
// FUNCTION: fn_11760
void UIBank_NewWnd::Paint() {
  auto& controls = *this->m_controls;
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\bank\\bg_bank.bmp")));
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(2771), 0);
  this->DrawBitmap(0, titleHeight, &background->image, false);
  char number[260] = {};
  char text[260] = {};
  this->DrawTextAt(controls.fonts, controls.locale, 30, 29, controls.services.MessageText(2772), 0, 0, 12, 0, 0, 0);
  const int x = this->TextWidth(controls.fonts, controls.locale, controls.services.MessageText(2772), 0, 0, 12, 0, 0) + 43;
  FormatGroupedInteger(g_session.m_bankBalance, number, 260);
  std::sprintf(text, "%s z", number);
  this->DrawTextAt(controls.fonts, controls.locale, x, 29, text, 0, 0, 11, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 30, 46, controls.services.MessageText(2773), 0, 0, 12, 0, 0, 0);
  FormatGroupedInteger(g_session.m_zeny, number, 260);
  std::sprintf(text, "%s z", number);
  this->DrawTextAt(controls.fonts, controls.locale, x, 46, text, 0, 0, 11, 0, 0, 0);
  if (!this->m_status.empty()) {
    const int textWidth = this->TextWidth(controls.fonts, controls.locale, this->m_status.c_str(), 0, 0, 12, 0, 0);
    const int statusX = static_cast<int>(this->geometry.width * 0.5f - textWidth * 0.5f);
    this->DrawTextAt(controls.fonts, controls.locale, statusX, 112, this->m_status.c_str(), 0, 0, 11, 255, 0, 0);
  }
}

// OFFSET: 0x008B7CF0
// FUNCTION: fn_11761
void UIChatRoomWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 20, 6, red, green, blue);
  const int width = this->geometry.width;
  this->FillRegion(width - width * 56 / 280 - 2, 17, 1, this->geometry.height - 38, RGB(red, green, blue));
  this->DrawButtonBar(0, 0, 1);
}

// OFFSET: 0x008B7DA0
// FUNCTION: fn_11762
void UICntWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008A91E0
// FUNCTION: fn_11721
void UIFrameWnd::DrawBorder(int x, int y, int width, int height) {
  if (width == 0) {
    width = this->geometry.width;
  }
  if (height == 0) {
    height = this->geometry.height;
  }
  const auto load = [&](const char* name) {
    return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
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
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
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

// OFFSET: 0x008BFB70
// FUNCTION: fn_11809
int UIMessageBoxAutoreturn::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    g_windowMgr.unknown_0474 = static_cast<int>(a);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008BFBA0
// FUNCTION: fn_11810
int UIMonsterInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    if (a == 184) {
      g_windowMgr.Schedule(this);
    }

    return 0;
  case 79:
    if (a) {
      this->info = *reinterpret_cast<const MonsterInfo*>(a);
    }

    this->Invalidate();
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008C1DB0
// FUNCTION: fn_11817
int UITalkboxTrapInputWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 184) {
      TalkboxTrapRequest request;
      request.target = this->target;
      request.text = this->edit->Text();
      g_modeMgr.GetGameMode()->SendMsg(131, reinterpret_cast<int>(&request), 0, 0, 0);
    } else if (a != 185) {
      return 0;
    }

    g_windowMgr.Schedule(this);
    return 0;
  }

  if (message == 68) {
    this->target = *reinterpret_cast<const TalkboxTrapTarget*>(a);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008C1ED0
// FUNCTION: fn_11818
void UIWindowMgr::SetUnknown04DC(const char* text) {
  this->unknown_04DC = text;
}

// OFFSET: 0x008C35A0
// FUNCTION: fn_11824
void UIChatRoomWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x008C35E0
// FUNCTION: fn_11825
void UIHomunInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008C3630
// FUNCTION: fn_11827
void UIMerInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008BF720

// OFFSET: 0x008C3670
// FUNCTION: fn_11828
void UIPetInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008C36B0
// FUNCTION: fn_11829
void UIReplayRECControlWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x008A6DC0
// FUNCTION: fn_11711
void UIBank_NewWnd::AnimateLoading() {
  for (int index = 0; index < 4; ++index) {
    this->m_loading[index]->Move(-100, -100);
  }

  this->m_loading[static_cast<int>(timeGetTime() % 1000) / 250]->Move(6, 5);
}

// OFFSET: 0x008B84B0
// FUNCTION: fn_11764
void UILoginOTPWnd::Paint() {
  auto& controls = *this->m_controls;
  auto& resources = ResourceMgr::Instance();
  const std::uint8_t source[] = {0x11, 0xF4, 0x57, 0x1F, 0xF9, 0x49, 0x2E, 0xE2, 0x17, 0xBF, 0x57, 0x29, 0x84, 0x3D, 0xB7, 0x43,
                                 0xBE, 0x3C, 0xFE, 0x8B, 0x66, 0xEE, 0x85, 0x5B, 0xB4, 0x29, 0xF1, 0x84, 0x5A, 0xE2, 0xB7, 0x4D,
                                 0xB6, 0x04, 0xF9, 0x80, 0x4E, 0xE8, 0x9F, 0x5F, 0xE0, 0x75, 0xF5, 0x88, 0x49, 0x87};
  thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
  const auto* background = static_cast<CBitmapRes*>(resources.Load(Resolve(name.Text())));
  this->DrawBitmap(0, 0, &background->image, false);
  this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(3254), 280);
}

// OFFSET: 0x008C3620
// FUNCTION: fn_11826
void UILoginWnd::Slot11() {
  this->SaveLogin();
}

// OFFSET: 0x008C39E0
// FUNCTION: fn_11831
void UILoginWnd::SaveLogin() {
  HKEY key;
  DWORD disposition = 0;

  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) != ERROR_SUCCESS) {
    return;
  }

  char id[52];
  int saveId;

  if (this->saveId != 0) {
    std::strcpy(id, this->idEdit->Text());
    saveId = this->saveId != 0;
    id[50] = 0;
  } else {
    std::memset(id, 0, sizeof(id));
    saveId = 0;
  }

  const std::uint8_t idSource[] = {0x24, 0xFF, 0x95};
  thread_local EncryptedName<sizeof(idSource), 0xC517FB932D95BB6DULL> idName(idSource);
  RegSetValueExA(key, idName.Text(), 0, REG_SZ, reinterpret_cast<const BYTE*>(id), 51);

  const std::uint8_t checkSource[] = {0xE6, 0xD5, 0x28, 0xBA, 0x28, 0x74, 0x36, 0x55, 0xE0, 0x9D};
  thread_local EncryptedName<sizeof(checkSource), 0x03772763F96D9DA5ULL> checkName(checkSource);
  RegSetValueExA(key, checkName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&saveId), 4);

  int bitwise = 1;
  const std::uint8_t bitwiseSource[] = {0xC5, 0x5A, 0x3F, 0x28, 0x70, 0x5A, 0x34, 0x0B};
  thread_local EncryptedName<sizeof(bitwiseSource), 0x0B7109397F6B1387ULL> bitwiseName(bitwiseSource);
  RegSetValueExA(key, bitwiseName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&bitwise), 4);
  RegCloseKey(key);
}

// OFFSET: 0x008C1F00
// FUNCTION: fn_11819
void UIReplayRECControlWnd::ApplyMode() {
  this->Resize(194, 34);

  if (this->mode == 0) {
    this->window_00C0->Move(-100, -100);

    for (int index = 0; index < 5; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->window_00C4->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->window_00EC->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->window_0128->Move(-100, -100);
    this->window_012C->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->window_00BC->Move(110, 5);
    this->button_00C8->Move(136, 5);
    this->button_00C8->SetDisabled(0);
    this->button_00C8->Invalidate();
    this->button_00D0->Move(162, 5);
    this->button_00D0->SetDisabled(0);
    this->button_00D0->Invalidate();
    this->window_00E8->Move(32, 12);
    this->tabs[0]->Move(6, 5);
  } else if (this->mode == 1) {
    this->recordStart = timeGetTime();
    this->window_00BC->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->tabs[4]->Move(-100, -100);
    this->window_00CC->Move(-100, -100);
    this->options[7]->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->window_0128->Move(-100, -100);
    this->window_012C->Move(-100, -100);
    this->window_00C4->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->window_00C0->Move(110, 5);
    this->button_00C8->Move(136, 5);
    this->button_00C8->SetDisabled(1);
    this->button_00C8->Invalidate();
    this->button_00D0->Move(162, 5);
    this->button_00D0->SetDisabled(1);
    this->button_00D0->Invalidate();
    this->window_00E8->Move(32, 12);
    this->tabs[0]->Move(6, 5);
  } else if (this->mode == 2) {
    this->Resize(194, 200);
    this->window_00BC->Move(-100, -100);
    this->window_00C0->Move(-100, -100);
    this->button_00C8->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->window_00E8->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->window_00C4->Move(-100, -100);
    this->window_0134->Move(-100, -100);
    this->tabs[4]->Move(6, 5);
    this->window_00CC->Move(162, 5);
    this->window_00EC->Move(42, 12);
    this->options[0]->Move(17, 63);
    this->options[1]->Move(17, 81);
    this->options[2]->Move(17, 99);
    this->options[3]->Move(17, 117);
    this->options[4]->Move(-100, -100);
    this->options[5]->Move(104, 81);
    this->options[6]->Move(104, 99);
    this->options[7]->Move(104, 117);
    this->options[9]->Move(104, 63);
    this->window_0128->Move(17, 160);
    this->window_012C->Move(17, 178);
  } else if (this->mode == 3) {
    this->window_00BC->Move(-100, -100);
    this->window_00C0->Move(-100, -100);
    this->button_00C8->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->window_00E8->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->tabs[4]->Move(-100, -100);
    this->window_00CC->Move(-100, -100);
    this->options[7]->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->window_0128->Move(-100, -100);
    this->window_012C->Move(-100, -100);
    this->window_00C4->Move(162, 5);
    this->window_0134->Move(7, 8);
    this->m_focus->Set(this->window_0134);
  }

  this->SendMsg(nullptr, 60, 0, 0, 0, 0);
}

// OFFSET: 0x008B9790

// OFFSET: 0x008BB070
// FUNCTION: fn_11793
void UINoticeWnd::Slot35(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, 7, 6, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, 7, -6, 0, 0, 0);
  }
}

// OFFSET: 0x008A8AD0
// FUNCTION: fn_11715
void UIPayConfirmWnd::Configure(const std::string& text, int itemId) {
  reinterpret_cast<UIMessageBox*>(this)->Configure(text.c_str(), 2, true, 0, LookupMessage(0xFDF), 0xDC, 0x85, 1, nullptr);

  this->viewer->Resize(0xA5, 0x50);
  this->viewer->Move(7, 0x17);
  this->viewer->backgroundColor = 0xFFFF00FF;
  this->viewer->Invalidate();

  for (UIWindow* child : this->m_children) {
    if (child->geometry.id == 187) {
      child->Move(0x59, 0x6D);
      break;
    }
  }

  for (UIWindow* child : this->m_children) {
    if (child->geometry.id == 188) {
      auto* button = static_cast<UIBitmapButton*>(child);
      button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0, 0);
      button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 1, 0);
      button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 2, 0);
      button->Resize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(this->geometry.width - 0xE, 3);
      break;
    }
  }

  std::string resource(FindItemDescription(itemId).identifiedResource);
  if (!resource.empty()) {
    this->m_image = reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(std::format("유저인터페이스\\collection\\{}.bmp", resource)));
  }
}

// OFFSET: 0x008B9940
// FUNCTION: fn_11772
void UIPayConfirmWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, this->m_title.c_str(), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);

  if (this->m_image) {
    this->DrawBitmapScaled(158, 27, 56, 75, this->m_image, 0);
  }
}

// OFFSET: 0x008B9E90
// FUNCTION: fn_11774
void UIReplayControlWnd::Paint() {
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\replay_interface\\replay_base.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
}

// OFFSET: 0x008B9ED0
// FUNCTION: fn_11775
void UIReplayRECControlWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, 34, 0xF5B899);
  DrawRoundedBorder(*this, 0, 0, this->geometry.width, 34, 0x945A63);
  DrawRoundedBorder(*this, 1, 1, this->geometry.width - 2, 32, 0xF7C6AD);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, 30, 0xFFFFFF);
  DrawRoundedBorder(*this, 3, 3, this->geometry.width - 6, 28, 0xAD7B84);

  if (this->mode == 2) {
    DrawFilledRoundedBorder(*this, 0, 34, this->geometry.width, this->geometry.height - 34, 0xDEDEDE);
    DrawFilledRoundedBorder(*this, 4, 38, this->geometry.width - 8, 95, 0xFFFFFF);
    DrawFilledRoundedBorder(*this, 4, 138, this->geometry.width - 8, 57, 0xFFFFFF);
    DrawOutlinedText(*this, 10, 42, LookupMessage(1973), 0, 0x7F3C00, 0xFFFFFF, 0, 12, 1, 0);
    DrawOutlinedText(*this, 10, 142, LookupMessage(1974), 0, 0x7F3C00, 0xFFFFFF, 0, 12, 1, 0);
  }
}

// OFFSET: 0x008BA010
// FUNCTION: fn_11776
void UISelectReplayDataWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2084), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->FillRegion(6, 32, 268, 77, 0xFFE7E7E7);
  this->DrawTextAt(controls.fonts, controls.locale, 6, 19, LookupMessage(2085), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 6, 112, LookupMessage(2086), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008BA0B0
// FUNCTION: fn_11777
void UISelectServerWnd::Paint() {
  auto& controls = *this->m_controls;
  const char* title = LookupMessage(2082);

  if (g_modeMgr.GetLoginMode()->cursorType == 30) {
    title = LookupMessage(2083);
  }

  this->DrawTitleBar(controls.fonts, controls.locale, true, title, 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x008BA110
// FUNCTION: fn_11778
void UITalkboxTrapInputWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(controls.fonts, controls.locale, 17, 3, LookupMessage(435), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 2, LookupMessage(435), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 17, 23, LookupMessage(436), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 22, LookupMessage(436), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008BA660
// FUNCTION: fn_11787
void UILoginWnd::Slot27(int, int) {
}

// OFFSET: 0x008BADB0
// FUNCTION: fn_11790
void UIMakeCharWnd::Slot19() {
}

// OFFSET: 0x008AD130
// FUNCTION: fn_11734
void UIChatRoomWnd::Move(int x, int y) {
  this->geometry.Move(x, y);
  this->Slot36();
}

// OFFSET: 0x008B1760
// FUNCTION: fn_11746
void UINoticeWnd::Slot15(int, int) {
  const std::string prefix("유저인터페이스\\");
  const char* names[3][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_back", "btn_back_a", "btn_back_b"}, {"btn_next", "btn_next_a", "btn_next_b"}};
  int ids[3] = {184, 246, 218};
  int positions[3][2] = {{534, 376}, {244, 376}, {294, 376}};

  for (int index = 0; index < 3; ++index) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 184;

  if (!std::fopen(this->m_patchFile, "r")) {
    FILE* file = std::fopen(this->m_patchFile, "w");
    std::fwrite("공지사항파일없음", 16, 1, file);
    std::fclose(file);
    g_windowMgr.Schedule(this);
  }

  if (!std::fopen("notice_check.txt", "r")) {
    FILE* file = std::fopen("notice_check.txt", "w");
    std::fwrite("없으니만들자", 12, 1, file);
    std::fclose(file);
  }

  unsigned char flag;
  if (!std::fopen("notice_read.txt", "r")) {
    FILE* file = std::fopen("notice_read.txt", "wb");
    flag = 0;
    std::fwrite(&flag, 1, 1, file);
    std::fclose(file);
  }

  FILE* patch = std::fopen(this->m_patchFile, "r");
  FILE* check = std::fopen("notice_check.txt", "r");
  std::fseek(patch, 0, SEEK_END);
  const int patchSize = std::ftell(patch);
  std::fseek(check, 0, SEEK_END);
  const int checkSize = std::ftell(check);
  int changed = checkSize == 0 ? 1000 : 0;
  char* patchText = new char[patchSize + 1];
  char* checkText = new char[checkSize + 1];
  std::fseek(patch, 0, SEEK_SET);
  std::fread(patchText, patchSize, 1, patch);
  patchText[patchSize] = 0;
  std::fclose(patch);
  std::fseek(check, 0, SEEK_SET);
  std::fread(checkText, checkSize, 1, check);
  checkText[checkSize] = 0;
  std::fclose(check);

  int lines[20];
  lines[0] = 0;
  int lineCount = 1;
  this->unknown_0108 = 0;
  for (int i = 0; i < patchSize; ++i) {
    const char c = patchText[i];
    if (c == '\n') {
      patchText[i] = 0;
      lines[lineCount] = i + 1;
      ++lineCount;
      if (lineCount > 20) {
        lineCount = 20;
      }
      ++this->unknown_0108;
    } else if (c == '\r') {
      patchText[i] = 0;
    } else if (c == '^') {
      const int previous = lineCount - 1;
      this->m_bottomPositions[this->m_noticeCount] = this->unknown_0108 - 1;
      ++this->m_noticeCount;
      this->m_scrollPositions[this->m_noticeCount] = this->unknown_0108 + 1;
      lineCount = previous < 0 ? 0 : previous;
    } else if (i < checkSize && static_cast<unsigned int>(i) < 1000 && c != checkText[i]) {
      ++changed;
    }
  }

  if (lineCount > 19) {
    lineCount = 19;
  }

  this->m_bottomPositions[this->m_noticeCount] = this->unknown_0108;

  if (changed == 0) {
    FILE* file = std::fopen("notice_read.txt", "rb");
    std::fread(&flag, 1, 1, file);
    std::fclose(file);
    if (flag == 0xFF) {
      g_windowMgr.Schedule(this);
    }
  } else {
    FILE* file = std::fopen("notice_read.txt", "wb");
    flag = 0;
    std::fwrite(&flag, 1, 1, file);
    std::fclose(file);
  }

  for (int i = 0, y = 22; y < 364; ++i, y += 18) {
    this->m_texts[i]->SetSize(540, 16);
    this->m_texts[i]->SetFont(0, 16, 0, 0);
    if (i < lineCount) {
      this->m_texts[i]->SetText(patchText + lines[i], 1);
    } else {
      this->m_texts[i]->SetText("", 1);
    }
    this->m_texts[i]->Move(30, y);
    this->AddChild(this->m_texts[i]);
  }

  this->m_scrollBar = new UIScrollBar(*this->m_context);
  static_cast<UIScrollBar*>(this->m_scrollBar)->Configure(583, 18, 1, this->geometry.height - 48);
  const int total = this->m_bottomPositions[this->m_index] - this->m_scrollPositions[this->m_index];
  static_cast<UIScrollBar*>(this->m_scrollBar)->SetRange(total - 19, 19, total);
  if (this->m_bottomPositions[this->m_index] - this->m_scrollPositions[this->m_index] < 19) {
    static_cast<UIScrollBar*>(this->m_scrollBar)->SetRange(0, 19, 19);
  }
  static_cast<UIScrollBar*>(this->m_scrollBar)->SetPosition(this->m_scroll);
  this->AddChild(this->m_scrollBar);

  FILE* source = std::fopen(this->m_patchFile, "r");
  FILE* target = std::fopen("notice_check.txt", "w");
  std::fseek(source, 0, SEEK_END);
  std::fseek(target, 0, SEEK_END);
  const int size = std::ftell(source);
  char* copy = new char[size + 1];
  std::fseek(source, 0, SEEK_SET);
  std::fread(copy, size, 1, source);
  copy[size] = 0;
  std::fclose(source);
  for (int i = 0; i < size; ++i) {
    std::fwrite(&copy[i], 1, 1, target);
  }
  std::fclose(target);

  if (changed == 0) {
    FILE* file = std::fopen("notice_read.txt", "rb");
    std::fread(&flag, 1, 1, file);
    std::fclose(file);
    if (flag != 0xFF) {
      this->m_scroll = flag;
    }
    if (this->m_scroll > this->m_bottomPositions[this->m_index] - 19) {
      this->m_scroll = this->m_bottomPositions[this->m_index] - 19;
    }
    if (this->m_scroll < 0) {
      this->m_scroll = 0;
    }
    this->LoadNotice();
  }
}

// OFFSET: 0x008B5A50
// FUNCTION: fn_11750
void UISelectReplayDataWnd::Slot15(int width, int height) {
  this->m_list = new UIReplayListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(6, 127, width - 12, height - 170, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  const char* names[4][3] = {
    {"btn_ok", "btn_ok_a", "btn_ok_b"},
    {"btn_cancel", "btn_cancel_a", "btn_cancel_b"},
    {"login_interface\\btn_exit", "login_interface\\btn_exit_a", "login_interface\\btn_exit_b"},
    {"replay_interface\\btn_selectserver", "replay_interface\\btn_selectserver_a", "replay_interface\\btn_selectserver_b"},
  };
  int ids[4] = {184, 185, 221, 400};
  int positions[4][2] = {{189, height - 25}, {-100, -100}, {234, height - 25}, {100, height - 25}};

  for (int index = 0; index < 4; ++index) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
    if (ids[index] == 221) {
      this->m_serverButton = button;
    }
  }

  this->m_message0 = 184;

  UIStaticText** text = this->m_texts;
  for (int y = 35; y < 110; y += 15) {
    *text = new UIStaticText(*this->m_context, *this->m_controls);
    (*text)->SetSize(width - 12, 15);
    (*text)->SetText("", 0);
    (*text)->Move(12, y);
    (*text)->SetColor(100, 100, 200);
    this->AddChild(*text);
    ++text;
  }
}

// OFFSET: 0x008B7040
// FUNCTION: fn_11752
void UITalkboxTrapInputWnd::Slot15(int, int) {
  this->edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->edit->SetSize(240, 16);
  this->edit->Move(22, 50);
  this->edit->maximum = 78;
  this->edit->Background(242, 242, 242);
  this->AddChild(this->edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->edit);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{189, 96}, {234, 96}};

  for (int index = 0; index < 2; ++index) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}

// OFFSET: 0x008B7A60
// FUNCTION: fn_11753
void UILoginOTPWnd::Slot18() {
}

// OFFSET: 0x008B7A70
// FUNCTION: fn_11754
void UILoginWnd::Slot18() {
}

// OFFSET: 0x008B7A80
// FUNCTION: fn_11755
void UIMakeCharWnd::Slot18() {
}

// OFFSET: 0x008B8E90
// FUNCTION: fn_11769
void UIMessageBoxAutoreturn::Paint() {
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
}

// OFFSET: 0x008BA670
// FUNCTION: fn_11788
void UILoginOTPWnd::Slot19() {
  auto* login = static_cast<CLoginMode*>(g_modeMgr.GetLoginMode());
  if (login == nullptr) {
    return;
  }

  const unsigned remaining = static_cast<unsigned>(60.0 - (timeGetTime() - login->otpStarted) * 0.001);
  const std::uint8_t source[] = {0x6A, 0x91, 0xFD, 0x51, 0x43, 0xED, 0x61, 0x5C, 0x7F, 0x93, 0xAB, 0x35};
  thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> format(source);
  this->timerText->SetText(FormatOwnedString(format.Text(), remaining / 60, remaining % 60).c_str(), 0);
  this->Invalidate();
}

// OFFSET: 0x008BAEF0
// FUNCTION: fn_11792
void UIMessageBoxAutoreturn::Slot19() {
  if (this->counting == 0) {
    return;
  }

  if (timeGetTime() > this->deadline) {
    g_windowMgr.unknown_0474 = 187;
    this->counting = 0;
    return;
  }

  const unsigned previous = this->remaining;
  const bool fraction = (this->deadline - timeGetTime()) % 1000 != 0;
  this->remaining = (this->deadline - timeGetTime()) / 1000 + fraction;

  if (previous != this->remaining) {
    char text[512];
    std::sprintf(text, this->format.c_str(), this->remaining);
    this->messageText = text;
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->messageText.data()), 38, 0, 0);
  }

  const unsigned duration = this->seconds * 1000;
  const unsigned progress = (timeGetTime() + (duration - this->deadline)) * 100 / duration;
  this->gauge->SetValue(static_cast<float>(progress));
}

// OFFSET: 0x008AA9D0

// OFFSET: 0x008AAA10
// FUNCTION: fn_11724
void UIMonsterInfoWnd::DrawResistance(int x, int y, const char* label, int value, std::uint32_t border, std::uint32_t fill) {
  auto& controls = *this->m_controls;
  char text[64];
  wsprintfA(text, "%s:%d", label, value);
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 22, 2, red, green, blue);
  this->FillRegion(x, y, 70, 16, border);
  this->FillRegion(x + 1, y + 1, 68, 14, fill);
  const int width = this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, (70 - width) / 2 + x, y + 2, text, 0, 0, 12, RGB(red, blue, green), 0, 0);
}

// OFFSET: 0x008B8ED0
// FUNCTION: fn_11770
void UIMonsterInfoWnd::Paint() {
  auto& controls = *this->m_controls;
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  const std::uint32_t fill = RGB(red, green, blue);
  this->ClearSurface(fill);
  this->DrawPanelBackground(this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0), 0, "");
  this->DrawButtonBar(2, 0, 1);
  SkinPaletteChannels(ResourceMgr::Instance(), 22, 2, red, green, blue);
  const std::uint32_t border = RGB(red, green, blue);

  this->DrawTextAt(controls.fonts, controls.locale, 17, 3, LookupMessage(406), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 2, LookupMessage(406), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 30, LookupMessage(407), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 30, this->name.c_str(), 0, 0, 12, 0, 0, 0);

  char text[256];
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.level);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 50, LookupMessage(408), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 50, text, 0, 0, 12, 0, 0, 0);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.hp);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 70, LookupMessage(409), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 70, text, 0, 0, 12, 0, 0, 0);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.defense);
  this->DrawTextAt(controls.fonts, controls.locale, 120, 90, LookupMessage(3910), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 160, 90, text, 0, 0, 12, 0, 0, 0);

  this->DrawTextAt(controls.fonts, controls.locale, 260, 30, LookupMessage(410), 0, 0, 12, 0, 0, 0);
  int message;
  switch (this->info.size) {
  case 0:
    message = 3907;
    break;
  case 1:
    message = 3908;
    break;
  case 2:
    message = 3909;
    break;
  default:
    message = 0;
    break;
  }
  if (message != 0) {
    this->DrawTextAt(controls.fonts, controls.locale, 300, 30, LookupMessage(message), 0, 0, 12, 0, 0, 0);
  }

  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.race);
  this->DrawTextAt(controls.fonts, controls.locale, 260, 50, LookupMessage(411), 0, 0, 12, 0, 0, 0);
  if (static_cast<unsigned short>(this->info.race) <= 9) {
    this->DrawTextAt(controls.fonts, controls.locale, 300, 50, LookupMessage(3887 + static_cast<unsigned short>(this->info.race)), 0, 0, 12, 0, 0, 0);
  }

  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.magicDefense);
  this->DrawTextAt(controls.fonts, controls.locale, 260, 90, LookupMessage(412), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 300, 90, text, 0, 0, 12, 0, 0, 0);

  this->DrawTextAt(controls.fonts, controls.locale, 260, 70, LookupMessage(413), 0, 0, 12, 0, 0, 0);
  const int element = this->info.element % 20;
  if (element >= 0 && element <= 9) {
    this->DrawTextAt(controls.fonts, controls.locale, 300, 70, LookupMessage(3897 + element), 0, 0, 12, 0, 0, 0);
  }

  this->DrawResistance(120, 110, LookupMessage(3898), this->info.resistances[0], border, fill);
  this->DrawResistance(120, 128, LookupMessage(3899), this->info.resistances[1], border, fill);
  this->DrawResistance(120, 146, LookupMessage(3900), this->info.resistances[2], border, fill);
  this->DrawResistance(200, 110, LookupMessage(3901), this->info.resistances[3], border, fill);
  this->DrawResistance(200, 128, LookupMessage(3902), this->info.resistances[4], border, fill);
  this->DrawResistance(200, 146, LookupMessage(3903), this->info.resistances[5], border, fill);
  this->DrawResistance(280, 110, LookupMessage(3904), this->info.resistances[6], border, fill);
  this->DrawResistance(280, 128, LookupMessage(3905), this->info.resistances[7], border, fill);
  this->DrawResistance(280, 146, LookupMessage(3906), this->info.resistances[8], border, fill);

  char spriteName[128];
  StringFormat(spriteName, sizeof(spriteName), static_cast<std::size_t>(-1), "몬스터\\%s.spr", g_session.GetJobName(this->info.job, -1));
  char actionName[128];
  StringFormat(actionName, sizeof(actionName), static_cast<std::size_t>(-1), "몬스터\\%s.act", g_session.GetJobName(this->info.job, -1));
  if (ResourceMgr::Instance().Exists(actionName) && ResourceMgr::Instance().Exists(spriteName)) {
    auto* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));
    auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));
    this->DrawActionSprite(sprite, action, 2, 19, 116, 152, 0, true, nullptr);
  }
}

// OFFSET: 0x008C3440
// FUNCTION: fn_11823
void UIReplayRECControlWnd::UpdateRecordTime() {
  for (int index = 0; index < 4; ++index) {
    this->tabs[index]->Move(-100, -100);
  }

  this->tabs[timeGetTime() % 1000 / 250]->Move(6, 5);
  if (timeGetTime() - this->recordStart <= 200) {
    return;
  }

  const int seconds = static_cast<int>(timeGetTime() - Replay::Instance().started) / 1000;
  const int minutes = seconds / 60;
  const int hours = minutes / 60;
  if (hours == 100) {
    this->window_00E8->Move(124, 47);
  } else if (hours == 1000) {
    this->window_00E8->Move(116, 47);
  }

  char text[100] = {};
  std::sprintf(text, "%03d:%02d:%02d", hours, minutes % 60, seconds % 60);
  this->window_00E8->SetTextFitted(text);
  this->Invalidate();
  this->InvalidateChildren();
  this->recordStart = timeGetTime();
}

// OFFSET: 0x008BA820
// FUNCTION: fn_11789
void UILoginWnd::Slot19() {
  if (g_windowMgr.GetHoverWindow() == this->idEdit) {
    this->idEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
    if (this->idPlaceholder == true) {
      const std::uint8_t source[] = {0xA4, 0x31, 0x61};
      thread_local EncryptedName<sizeof(source), 0x2D51E3B7236175EDULL> placeholder(source);
      if (std::strcmp(placeholder.Text(), this->idEdit->Text()) == 0) {
        this->idEdit->SetText("");
        goto password;
      }
    }
    this->idPlaceholder = false;
  } else {
    if (std::strcmp("", this->idEdit->Text()) == 0) {
      this->idEdit->TextColor(RGB(156, 156, 156), 0xFFFFFFFF);
      this->idPlaceholder = true;
      const std::uint8_t source[] = {0x5E, 0x5F, 0x2D};
      thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> placeholder(source);
      this->idEdit->SetText(placeholder.Text());
    }
    if (this->idPlaceholder == false) {
      this->idEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
    }
  }

password:
  if (g_windowMgr.GetHoverWindow() == this->passwordEdit) {
    HIMC context = ImmGetContext(hWnd);
    DWORD conversion;
    DWORD sentence;
    ImmGetConversionStatus(context, &conversion, &sentence);
    conversion &= ~(IME_CMODE_NATIVE | IME_CMODE_KATAKANA);
    ImmSetConversionStatus(context, IME_CMODE_ALPHANUMERIC, sentence);
    ImmReleaseContext(hWnd, context);
    this->passwordEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
    if (this->passwordPlaceholder == true) {
      const std::uint8_t source[] = {0x99, 0x52, 0xAA, 0x20, 0xAE, 0x96, 0x47, 0x35, 0xC9};
      thread_local EncryptedName<sizeof(source), 0x7115D9F973F913C9ULL> placeholder(source);
      if (std::strcmp(placeholder.Text(), this->passwordEdit->Text()) == 0) {
        this->passwordEdit->SetText("");
        this->passwordEdit->mask = 0;
        return;
      }
    }
    this->passwordPlaceholder = false;
  } else {
    if (std::strcmp("", this->passwordEdit->Text()) == 0) {
      this->passwordPlaceholder = true;
      this->passwordEdit->TextColor(RGB(156, 156, 156), 0xFFFFFFFF);
      const std::uint8_t source[] = {0x33, 0xB4, 0xB2, 0x2C, 0x6C, 0x6A, 0x4F, 0x8F, 0x63};
      thread_local EncryptedName<sizeof(source), 0xCB1D253B7FE1F563ULL> placeholder(source);
      this->passwordEdit->SetText(placeholder.Text());
      this->passwordEdit->mask = 0;
    }
    if (this->passwordPlaceholder != false) {
      return;
    }
    this->passwordEdit->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
  }

  this->passwordEdit->mask = '*';
}

// OFFSET: 0x008C36F0
// FUNCTION: fn_11830
void UIChatRoomWnd::SaveChatLog() {
  int index = 0;
  char path[2048];
  std::sprintf(path, "%s\\Chat", g_applicationDirectory);
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  std::string name = this->title;
  for (unsigned position = 0; position < name.size(); ++position) {
    const char character = name[position];
    if (character == '\\' || character == '/' || character == ':' || character == '*' || character == '?' || character == '"' || character == '<' ||
        character == '>' || character == '|') {
      name[position] = '_';
    }
  }

  std::sprintf(path, "%s\\Chat\\%s.txt", g_applicationDirectory, name.c_str());
  while (_access(path, 0) == 0) {
    ++index;
    std::sprintf(path, "%s\\Chat\\%s%03d.txt", g_applicationDirectory, name.c_str(), index);
  }

  if (this->history->SaveChatLog(path)) {
    std::string message = path;
    message.append(" is Saved.", 10);
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF00, 0, 0, 0);
  }
}

// OFFSET: 0x008ABDB0
// FUNCTION: fn_11728
void UILoginWnd::LoadLogin() {
  HKEY probe;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &probe) != ERROR_SUCCESS) {
    this->idEdit->SetText("");
    this->passwordEdit->SetText("");
    return;
  }

  RegCloseKey(probe);
  HKEY key;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS) {
    return;
  }

  DWORD type = REG_DWORD;
  DWORD size = 4;
  int saveId;
  const std::uint8_t checkSource[] = {0xB2, 0x8F, 0x7C, 0x4A, 0x5C, 0x2A, 0x0E, 0xA7, 0xB4, 0xC7};
  thread_local EncryptedName<sizeof(checkSource), 0xF14F79170939C7F1ULL> checkName(checkSource);
  RegQueryValueExA(key, checkName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(&saveId), &size);
  saveId = saveId == 1;

  int bitwise;
  const std::uint8_t bitwiseSource[] = {0xDD, 0x8C, 0x39, 0xFC, 0x9E, 0x7A, 0xB8, 0xA5};
  thread_local EncryptedName<sizeof(bitwiseSource), 0xA5FD29D7AB6DC59FULL> bitwiseName(bitwiseSource);
  RegQueryValueExA(key, bitwiseName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(&bitwise), &size);

  char id[52];
  std::memset(id, 0, sizeof(id));
  char password[26] = {};
  type = REG_SZ;
  size = 25;
  const std::uint8_t passwordSource[] = {0xB1, 0x3A, 0xAA, 0x84, 0x82, 0x5C, 0x6B, 0x99, 0xE1};
  thread_local EncryptedName<sizeof(passwordSource), 0xDD3913D5D7F97BE1ULL> passwordName(passwordSource);
  RegQueryValueExA(key, passwordName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(password), &size);
  password[24] = 0;

  type = REG_SZ;
  size = 51;
  const std::uint8_t idSource[] = {0xD2, 0x13, 0x03};
  thread_local EncryptedName<sizeof(idSource), 0x050505679903579BULL> idName(idSource);
  RegQueryValueExA(key, idName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(id), &size);
  id[50] = 0;
  password[24] = 0;
  this->saveId = saveId;

  if (saveId != 0) {
    unsigned char shade;
    bool placeholder = std::strcmp("", id) == 0;
    if (!placeholder) {
      const std::uint8_t compareSource[] = {0xF4, 0xF9, 0xED};
      thread_local EncryptedName<sizeof(compareSource), 0x83EFBFBB37EDBDBDULL> compareName(compareSource);
      placeholder = std::strcmp(compareName.Text(), id) == 0;
    }

    if (placeholder) {
      this->idPlaceholder = true;
      const std::uint8_t textSource[] = {0xEE, 0xB1, 0x7F};
      thread_local EncryptedName<sizeof(textSource), 0xBF2B4B31397FF5A7ULL> textName(textSource);
      this->idEdit->SetText(textName.Text());
      shade = 156;
    } else {
      shade = 0;
      this->idPlaceholder = false;
    }

    this->idEdit->TextColor(RGB(shade, shade, shade), 0xFFFFFFFF);
    this->idEdit->SetText(id);
    this->passwordEdit->SetText("");
  }

  RegCloseKey(key);
}

// OFFSET: 0x008AB050
// FUNCTION: fn_11726
int sub_8AB050() {
  return 4;
}

// OFFSET: 0x008AB060
// FUNCTION: fn_11727
int sub_8AB060() {
  return 6;
}

// OFFSET: 0x008A4220
// FUNCTION: fn_11666
UINoticeWnd::UINoticeWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->m_patchFile = "notice_patch.txt";
  for (int i = 0; i < 19; ++i) {
    this->m_texts[i] = new UIStaticText(*this->m_context, *this->m_controls);
  }
  this->unknown_0108 = 20;
  this->unknown_010C = 0;
  this->m_noticeCount = 0;
  this->m_index = 0;
  this->m_scrollPositions[0] = 0;
  this->m_bottomPositions[0] = 0;
  this->m_scroll = 0;
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this);
}

// OFFSET: 0x008AD190
// FUNCTION: fn_11737
void UIBank_NewWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "Exit", 0);
  this->AddBitmapButton(*this->m_controls, 462, 209, 77, "bank\\btn_deposit", LookupMessage(2774));
  this->AddBitmapButton(*this->m_controls, 463, 209, 102, "bank\\btn_withdraw", LookupMessage(2775));
  this->AddBitmapButton(*this->m_controls, 279, 177, 89, "bank\\btn_upper", LookupMessage(2776));
  this->AddBitmapButton(*this->m_controls, 280, 177, 99, "bank\\btn_buttom", LookupMessage(2777));
  this->AddBitmapButton(*this->m_controls, 392, 189, 132, "bank\\btn_10mil", "+ 100,000");
  this->AddBitmapButton(*this->m_controls, 391, 141, 132, "bank\\btn_100mil", "+ 1,000,000");
  this->AddBitmapButton(*this->m_controls, 390, 93, 132, "bank\\btn_1000mil", "+ 10,000,000");
  this->AddBitmapButton(*this->m_controls, 389, 45, 132, "bank\\btn_max", LookupMessage(2778));

  this->m_amountEdit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_amountEdit->SetSize(99, 17);
  this->m_amountEdit->maximum = 10;
  this->m_amountEdit->Move(78, 90);
  this->m_amountEdit->Background(247, 247, 247);
  this->m_amountEdit->Border(198, 198, 198);
  this->m_amountEdit->flat = 0;
  this->m_amountEdit->topPadding = 1;
  this->AddChild(this->m_amountEdit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_amountEdit);
}

// OFFSET: 0x008AD770
// FUNCTION: fn_11739
void UICntWnd::Slot15(int, int) {
  this->m_edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edit->SetSize(80, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 6;
  this->m_edit->Background(232, 232, 232);
  this->AddChild(this->m_edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);
  this->m_edit->SelectAll();

  this->m_text = new UIStaticText(*this->m_context, *this->m_controls);
  this->m_text->SetSize(this->geometry.width - 30, 15);
  this->m_text->Move(15, 6);
  this->AddChild(this->m_text);

  const std::string prefix("유저인터페이스\\");
  const int width = this->geometry.width;
  const int height = this->geometry.height;
  UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 55, height - 32);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;
}

// OFFSET: 0x008C0B20
// FUNCTION: fn_11813
int UIReplayControlWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 401:
      g_modeMgr.gameMode->SendMsg(276, 0, 0, 0, 0);
      return 0;
    case 402:
      g_modeMgr.gameMode->SendMsg(277, 0, 0, 0, 0);
      return 0;
    case 403:
      g_modeMgr.gameMode->SendMsg(278, 0, 0, 0, 0);
      Replay::Instance().speed = 1.0f;
      return 0;
    case 404:
      g_windowMgr.Schedule(this);
      g_modeMgr.gameMode->SendMsg(281, 0, 0, 0, 0);
      Replay::Instance().speed = 1.0f;
      return 0;
    case 405:
      g_modeMgr.gameMode->SendMsg(279, 0, 0, 0, 0);
      this->SendMsg(nullptr, 60, 0, 0, 0, 0);
      return 0;
    case 406:
      g_modeMgr.gameMode->SendMsg(280, 0, 0, 0, 0);
      this->SendMsg(nullptr, 60, 0, 0, 0, 0);
      return 0;
    }
    return 0;
  case 35:
    if (!this->m_paused) {
      this->m_playButton->Move(-100, -100);
      this->m_pauseButton->Move(5, 89);
      this->m_paused = true;
    } else {
      this->m_playButton->Move(5, 89);
      this->m_pauseButton->Move(-100, -100);
      this->m_paused = false;
    }
    this->Invalidate();
    this->InvalidateChildren();
    return 0;
  case 60:
    this->RefreshInfo();
    return 0;
  case 251:
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x008C3110
// FUNCTION: fn_11822
bool UIReplayControlWnd::RefreshInfo() {
  const int seconds = static_cast<int>(Replay::Instance().elapsed) / 1000;
  const int minutes = seconds / 60;
  const ReplayTime duration = Replay::Instance().Duration();
  char text[100];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, LookupMessage(1955), minutes / 60, minutes % 60, seconds % 60, duration.hours, duration.minutes, duration.seconds);
  this->m_texts[0]->SetTextFitted(text);

  std::memset(text, 0, sizeof(text));
  const int level = Replay::Instance().SpeedLevel();
  int speedMessage;
  if (level == 1) {
    speedMessage = 1956;
  } else if (level == 2) {
    speedMessage = 1957;
  } else if (level == 3) {
    speedMessage = 1958;
  } else if (level == 4) {
    speedMessage = 1959;
  } else if (level == 5) {
    speedMessage = 1960;
  } else if (level == 6) {
    speedMessage = 1961;
  } else {
    speedMessage = (level != 7) + 1962;
  }
  std::sprintf(text, LookupMessage(speedMessage));
  this->m_texts[1]->SetTextFitted(text);

  std::string location;
  if (g_modeMgr.gameMode != nullptr) {
    location = static_cast<CGameMode*>(g_modeMgr.gameMode)->LocationText();
  }
  this->m_texts[3]->SetTextFitted(location.c_str());
  this->Invalidate();
  this->InvalidateChildren();
  return true;
}

// OFFSET: 0x008C2460
// FUNCTION: fn_11820
bool UISelectReplayDataWnd::UpdateInfo() {
  if (!ReplayFileList::Instance().Select(this->m_list->selected)) {
    this->m_texts[0]->SetTextFitted("");
    this->m_texts[1]->SetTextFitted("");
    this->m_texts[2]->SetTextFitted("");
    this->m_texts[3]->SetTextFitted("");
    this->m_texts[4]->SetTextFitted("");
    return true;
  }

  if (ReplayFileList::Instance().Open(32, 0, 0) != 1) {
    this->m_texts[0]->SetTextFitted("");
    this->m_texts[1]->SetTextFitted(LookupMessage(1969));
    this->m_texts[2]->SetTextFitted("");
    this->m_texts[3]->SetTextFitted("");
    this->m_texts[4]->SetTextFitted("");
    return true;
  }

  Replay::Instance().CloseFile();
  char buffer[100];
  std::memset(buffer, 0, sizeof(buffer));
  std::string service;
  switch (Replay::Instance().header.serviceType) {
  case 0:
    service = "Primary";
    break;
  case 1:
    service = "Sakray";
    break;
  case 2:
    service = "Local";
    break;
  case 3:
    service = "InstantEvent";
    break;
  case 4:
    service = "PVP";
    break;
  case 5:
    service = "PK";
    break;
  default:
    service = "Data Error";
    this->m_texts[0]->SetTextFitted("");
    this->m_texts[1]->SetTextFitted(LookupMessage(1969));
    this->m_texts[2]->SetTextFitted("");
    this->m_texts[3]->SetTextFitted("");
    this->m_texts[4]->SetTextFitted("");
    return false;
  }

  std::sprintf(buffer, LookupMessage(1964), service.c_str());
  this->m_texts[0]->SetTextFitted(buffer);
  std::memset(buffer, 0, sizeof(buffer));

  char name[64];
  std::memcpy(name, Replay::Instance().header.characterName, sizeof(name));
  std::sprintf(buffer, LookupMessage(1965), name);
  this->m_texts[1]->SetTextFitted(buffer);

  std::sprintf(buffer, LookupMessage(1966), Replay::Instance().header.mapName);
  this->m_texts[2]->SetTextFitted(buffer);

  const std::uint32_t date = Replay::Instance().header.key0;
  const std::uint32_t time = Replay::Instance().header.key1;
  std::sprintf(
    buffer, LookupMessage(1967), date & 0xFFFF, static_cast<char>(date >> 16), static_cast<char>(date >> 24), static_cast<char>(time >> 8),
    static_cast<char>(time >> 16), static_cast<char>(time >> 24)
  );
  this->m_texts[3]->SetTextFitted(buffer);

  const ReplayTime duration = Replay::Instance().Duration();
  std::sprintf(buffer, LookupMessage(1968), duration.hours, duration.minutes, duration.seconds);
  this->m_texts[4]->SetTextFitted(buffer);
  return true;
}

// OFFSET: 0x008C18C0
// FUNCTION: fn_11816
int UISelectServerWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 184:
      if (this->m_serverList->selected != -1) {
        g_windowMgr.Schedule(this);
        static_cast<CMode*>(g_charViewMode)->SendMsg(this->m_event, this->m_serverList->selected, 0, 0, 0);
        g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
        const char* name = this->m_valueName.c_str();
        this->m_selectedServer = this->m_serverList->selected;
        HKEY key;
        DWORD disposition = 0;
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
          RegSetValueExA(key, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_selectedServer), 4);
          RegCloseKey(key);
          return 0;
        }
      }
      break;
    case 185:
      g_windowMgr.Schedule(this);
      static_cast<CMode*>(g_charViewMode)->SendMsg(10006, 0, 0, 0, 0);
      return 0;
    case 221:
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(17), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
        return 0;
      }
      break;
    case 400:
      reinterpret_cast<Replay&>(ReplayFileList::Instance()).SetBaseFrame(reinterpret_cast<std::uint32_t>(&Replay::Instance()));
      ReplayFileList::Instance().unknown_011C = g_session.m_aid;
      ReplayFileList::Instance().unknown_0120 = g_session.SyncUnknown0E98();
      static_cast<CMode*>(g_charViewMode)->SendMsg(10026, 0, 0, 0, 0);
      g_windowMgr.Schedule(this);
      return 0;
    }
    break;
  case 18:
    this->m_serverList->SendMsg(nullptr, 18, 0, 0, 0, 0);
    return 0;
  case 19:
    this->m_serverList->SendMsg(nullptr, 19, 0, 0, 0, 0);
    return 0;
  case 40:
    this->m_serverList->SendMsg(nullptr, 40, this->m_selectedServer, 0, 0, 0);
    return 0;
  case 80: {
    this->m_event = a;
    this->m_valueName = reinterpret_cast<const char*>(b);
    const char* name = this->m_valueName.c_str();
    this->m_selectedServer = g_serviceType;
    HKEY probe;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &probe) == ERROR_SUCCESS) {
      RegCloseKey(probe);
      HKEY key;
      if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
        DWORD type = REG_DWORD;
        DWORD size = 4;
        RegQueryValueExA(key, name, nullptr, &type, reinterpret_cast<BYTE*>(&this->m_selectedServer), &size);
        RegCloseKey(key);
        return 0;
      }
    }
    break;
  }
  case 89:
    this->m_okButton->Move(234, this->m_okButton->geometry.y);
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008C2E30
// FUNCTION: fn_11821
void UIPetInfoWnd::UpdateFriendlyGauge() {
  const std::string previous = this->friendlyText;
  int low;
  int high;
  if (g_session.m_petFriendly >= 0 && g_session.m_petFriendly <= 100) {
    if (g_session.GetDisplayJob() >= 4050 && g_session.GetDisplayJob() <= 4052) {
      int message;
      if (g_session.m_petFriendly <= 20) {
        low = 0;
        message = 1022;
        high = 100;
      } else if (g_session.m_petFriendly <= 40) {
        low = 20;
        message = 1021;
        high = 40;
      } else {
        low = 40;
        message = 672;
        high = 100;
      }
      this->friendlyText = LookupMessage(message);
    } else {
      this->friendlyText = LookupMessage(672);
      low = 0;
      high = 100;
    }
  } else if (g_session.m_petFriendly > 100 && g_session.m_petFriendly <= 250) {
    this->friendlyText = LookupMessage(673);
    low = 100;
    high = 250;
  } else if (g_session.m_petFriendly > 250 && g_session.m_petFriendly <= 750) {
    this->friendlyText = LookupMessage(669);
    low = 250;
    high = 750;
  } else if (g_session.m_petFriendly > 750 && g_session.m_petFriendly <= 900) {
    this->friendlyText = LookupMessage(674);
    low = 750;
    high = 900;
  } else if (g_session.m_petFriendly > 900 && g_session.m_petFriendly <= 1000) {
    this->friendlyText = LookupMessage(675);
    low = 900;
    high = 1000;
  } else {
    this->friendlyText = LookupMessage(676);
    low = 0;
    high = 0;
  }
  const float ratio = static_cast<float>(static_cast<unsigned>(g_session.m_petFriendly - low)) / static_cast<float>(static_cast<unsigned>(high - low));
  this->friendlyGauge->SetValue(static_cast<unsigned>(ratio * 100.0f));
  if (!(previous == this->friendlyText)) {
    this->Invalidate();
  }
}
