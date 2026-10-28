#include "Client/Application/ApplicationWindow.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Chat/UIWhisperWnd.h"
#include "UI/Dialogs/UIBookWnd.h"
#include "UI/Dialogs/UIChooseWarpWnd.h"
#include "UI/Dialogs/UIChooseWnd.h"
#include "UI/Dialogs/UIDisconnectedServerMsgWnd.h"
#include "UI/Dialogs/UIEmailAddressWnd.h"
#include "UI/Dialogs/UIGraffiStrboxWnd.h"
#include "UI/Dialogs/UILoadingWnd.h"
#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Dialogs/UIMinimapZoomWnd.h"
#include "UI/Dialogs/UINoticeConfirmWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "UI/Dialogs/UINotifyLevelUpWnd.h"
#include "UI/Dialogs/UINotifyQuestWnd.h"
#include "UI/Dialogs/UIPetTamingDeceiveWnd.h"
#include "UI/Dialogs/UIProhibitListWnd.h"
#include "UI/Dialogs/UIQuitWnd.h"
#include "UI/Dialogs/UIRestartWnd.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Dialogs/UISelectCartWnd.h"
#include "UI/Dialogs/UITipOfTheDayWnd.h"
#include "UI/Dialogs/UIToolTipWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Items/UIExchangeWnd.h"
#include "UI/Items/UIItemCollectionWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Items/UIItemParamChangeDisplayWnd.h"
#include "UI/Items/UINotifyItemObtainWnd.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Party/UIPartySettingWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Shop/UInCash_CallWnd.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Skill/UISkillNameChangeWnd.h"
#include "UI/Status/UIStatusWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char prefix[] = "유저인터페이스\\";
}

// OFFSET: 0x008C73C0
// FUNCTION: fn_11930
UIChooseSellBuyWnd::UIChooseSellBuyWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIFrameWnd(windows, frames, 0), identity(0), m_acceptance(&acceptance) {
}

// OFFSET: 0x008CC430
UIChooseSellBuyWnd::~UIChooseSellBuyWnd() = default;

// INLINED
void UIChooseSellBuyWnd::AddChoice(const char* name, int id, int x, int y) {
  auto* button = new UIBitmapButton(*this->m_context, this->m_acceptance->text.buttons);
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

// INLINED
void UIChooseSellBuyWnd::CreateViewer(int width, int height) {
  auto& text = this->m_acceptance->text;
  this->viewer = new UITextViewer(*this->m_context, text);
  this->viewer->Configure(10, 22, width - 20, height - 55, 0);
  const std::string chip = std::string(prefix) + "colorchip.bmp";
  const auto* bitmap = static_cast<CBitmapRes*>(text.buttons.resources.Load(chip.c_str()));
  const auto& image = bitmap->image;
  const std::uint32_t pixel = image.width > 2 && image.height > 2 ? image.pixels[2 + 2 * image.width] : 0x00FF0000u;
  this->viewer->textColor = 0xFF000000u | ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
}

// OFFSET: 0x008DD290
// FUNCTION: fn_12162
void UIChooseSellBuyWnd::Slot15(int width, int height) {
  this->AddChoice("btn_buy", 224, 144, 96);
  this->AddChoice("btn_sell", 223, 189, 96);
  this->AddChoice("btn_cancel", 185, 234, 96);
  this->m_message0 = 224;
  this->CreateViewer(width, height);
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->m_acceptance->text.buttons.services.MessageText(92)), 38, 0, 0);
}

// OFFSET: 0x008ED130
// FUNCTION: fn_12214
void UIChooseSellBuyWnd::Paint() {
  const std::string path = std::string(prefix) + "win_msgbox.bmp";
  const auto* bitmap = static_cast<CBitmapRes*>(this->m_acceptance->text.buttons.resources.Load(path.c_str()));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x008ED170
// FUNCTION: fn_12215
void UIChooseWarpWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008ED180
// FUNCTION: fn_12216
void UIChooseWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008ED190
// FUNCTION: fn_12217
void UIComboBoxIDWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x008ED1A0
// FUNCTION: fn_12218
void UIComboBoxWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x008ED1B0
// FUNCTION: fn_12219
void UIDisconnectedServerMsgWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x008ED1F0
// FUNCTION: fn_12220
void UIEmailAddressWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 15, 6, controls.services.MessageText(g_serviceType == 0 ? 1815 : 299), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EE230
// FUNCTION: fn_12223
void UIGraffiStrboxWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(controls.fonts, controls.locale, 17, 23, controls.services.MessageText(436), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 16, 22, controls.services.MessageText(436), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EE9D0
// FUNCTION: fn_12227
void UIItemParamChangeDisplayWnd::Paint() {
  auto& controls = *this->m_controls;
  char text[128];
  const auto drawArrow = [this](int x, int before, int after) {
    if (before < after) {
      FillTriangle(*this, x + 4, 11, x, 19, x + 8, 19, 0xFF);
    } else if (before == after) {
      FillTriangle(*this, x, 13, x + 8, 13, x, 17, 0x505050);
      FillTriangle(*this, x + 8, 13, x, 17, x + 8, 17, 0x505050);
    } else {
      FillTriangle(*this, x, 11, x + 8, 11, x + 4, 19, 0xFF0000);
    }
  };

  this->DrawBorder(0, 0, 0, 0);
  std::sprintf(text, controls.services.MessageText(269));
  this->DrawTextAt(controls.fonts, controls.locale, 20, 10, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%d", this->m_before);
  this->DrawTextAt(controls.fonts, controls.locale, 70, 10, text, 0, 0, 12, 0, 0, 0);
  drawArrow(90, this->m_before, this->m_after);
  std::sprintf(text, "%d", this->m_after);
  this->DrawTextAt(controls.fonts, controls.locale, 110, 10, text, 0, 0, 12, 0, 0, 0);

  std::sprintf(text, controls.services.MessageText(270));
  this->DrawTextAt(controls.fonts, controls.locale, 152, 10, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%d", this->m_beforeSecond);
  this->DrawTextAt(controls.fonts, controls.locale, 202, 10, text, 0, 0, 12, 0, 0, 0);
  drawArrow(222, this->m_beforeSecond, this->m_afterSecond);
  std::sprintf(text, "%d", this->m_afterSecond);
  this->DrawTextAt(controls.fonts, controls.locale, 242, 10, text, 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EECD0
// FUNCTION: fn_12228
void UILoadingWnd::Paint() {
}

// OFFSET: 0x008EF0A0
// FUNCTION: fn_12231
void UIMinimapZoomWnd::Paint() {
  auto& controls = *this->m_controls;
  this->ClearSurface(0xFFFF00FF);
  int column = 0;
  int row = 0;
  static_cast<CGameMode*>(g_world.ActiveGameMode())->PlayerCell(&column, &row);
  const std::string text = FormatOwnedString(" %d  %d", column, row);
  this->DrawTextAt(controls.fonts, controls.locale, 19, 128, text.c_str(), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 20, 128, text.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
}

// OFFSET: 0x008EF590
// FUNCTION: fn_12233
void UINoticeConfirmWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x008EF7F0
// FUNCTION: fn_12235
void UINotifyLevelUpWnd::Paint() {
}

// OFFSET: 0x008EF800
// FUNCTION: fn_12236
void UINotifyQuestWnd::Paint() {
}

// OFFSET: 0x008EF810
// FUNCTION: fn_12237
void UINpcEditDialogWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 15, 6, g_serviceType == 12 ? "Insira o Numero" : "Input number ", 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EF9A0
// FUNCTION: fn_12239
void UIPartyInvitationToWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(2056), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(controls.fonts, controls.locale, 10, 22, controls.services.MessageText(2058), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EFA00
// FUNCTION: fn_12240
void UIPartySettingWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(this->m_mode == 50 ? 2054 : 291), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);

  if (this->m_mode == 70) {
    this->DrawTextAt(controls.fonts, controls.locale, 10, 22, controls.services.MessageText(292), 0, 0, 12, 0, 0, 0);
  } else if (this->m_mode == 50) {
    this->DrawTextAt(controls.fonts, controls.locale, 10, 22, controls.services.MessageText(2057), 0, 0, 12, 0, 0, 0);
  }

  this->DrawTextAt(controls.fonts, controls.locale, 10, this->m_mode + 82, controls.services.MessageText(738), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 10, this->m_mode + 17, controls.services.MessageText(293), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EFAF0
// FUNCTION: fn_12241
void UIPasswordWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008EFF60
// FUNCTION: fn_12245
void UIQuitWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x008EFFA0
// FUNCTION: fn_12246
void UIRestartWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x008EFFE0
// FUNCTION: fn_12247
void UISayDialogWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008F0350
// FUNCTION: fn_12249
void UISelectCartWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(659), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - left->image.height, &left->image, false);
  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - right->image.width, this->geometry.height - right->image.height, &right->image, false);
}

// OFFSET: 0x008F0400
// FUNCTION: fn_12250
void UISkillDescribeWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  const UIWindow* text = this->m_text;
  this->FillRegion(
    text->geometry.width + text->geometry.x, text->geometry.y, this->geometry.width - text->geometry.width - text->geometry.x - 3, text->geometry.height,
    RGB(red, green, blue)
  );
  DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xC3C3C3);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, this->geometry.height - 4, 0xFFFFFF);
}

// OFFSET: 0x008F04B0
// FUNCTION: fn_12251
void UISkillNameChangeWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 15, 6, this->m_name.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008F2460
// FUNCTION: fn_12255
void UITipOfTheDayWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(174), 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &left->image, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, &right->image, false);
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slots = (this->geometry.height - 38) / 32;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(72, 17, columns * 32 - 32, slots * 32, RGB(red, green, blue));
}

// OFFSET: 0x008F2A80
// FUNCTION: fn_12257
void UIToolTipWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008F2A90
// FUNCTION: fn_12258
void UIWaitWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x008F2AD0
// FUNCTION: fn_12259
void UIWhisperWnd::Paint() {
  auto& controls = *this->m_controls;
  char title[128];

  if (!this->m_code.empty()) {
    std::sprintf(title, "With  %s [%s]", this->m_name.c_str(), this->m_code.c_str());
  } else {
    std::sprintf(title, "With  %s", this->m_name.c_str());
  }

  DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xC0C0C0);
  this->FillRegion(1, this->geometry.height - 20, this->geometry.width - 2, 1, 0xC0C0C0);
  this->DrawTextAt(controls.fonts, controls.locale, 10, 3, title, 0, 0, 12, 0xFFFFFF, 0, 0);
}

// OFFSET: 0x008F2DE0
// FUNCTION: fn_12261
void UInCash_CallWnd::Paint() {
}

// OFFSET: 0x008F32D0
// FUNCTION: fn_12264
void UIItemIdentifyWnd::Slot26(int x, int y) {
  this->Slot25(x, y);
  this->SendMsg(nullptr, 6, 184, 0, 0, 0);
}

// OFFSET: 0x008F3300
// FUNCTION: fn_12265
void UIStatusWnd::Slot26(int, int y) {
  if (y < 17) {
    this->SendMsg(this, 6, 202, 0, 0, 0);
  }
}

// OFFSET: 0x008F3330
// FUNCTION: fn_12266
void UIWhisperWnd::Slot26(int, int y) {
  if (y >= 17 || this->m_code.size() == 0) {
    return;
  }

  HGLOBAL memory = GlobalAlloc(GHND, this->m_code.size() + 1);
  void* data = GlobalLock(memory);
  std::memcpy(data, this->m_code.c_str(), this->m_code.size());
  GlobalUnWire(memory);

  if (OpenClipboard(hWnd)) {
    EmptyClipboard();
    SetClipboardData(CF_TEXT, memory);
    CloseClipboard();
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("C-Code copied to clipboard"), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x008F3630
// FUNCTION: fn_12268
void UIBookWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x008F3650
// FUNCTION: fn_12269
void UICombinedCardItemCollection_ComparisonWnd::Slot25(int, int) {
}

// OFFSET: 0x008F38D0
// FUNCTION: fn_12272
void UIItemCollection_ComparisonWnd::Slot25(int, int) {
}

// OFFSET: 0x008F38E0
// FUNCTION: fn_12273
void UIItemIdentifyWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
    return;
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  const int previous = this->m_selected;
  int selected = -1;

  if (x <= this->geometry.width - 14 && x >= 4 && y >= 34 && y < this->m_rows * 32 + 34) {
    const int row = (y - 34) / 32;
    int visible = 0;
    int index = 0;

    for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it, ++index) {
      if (index >= this->m_scroll * this->m_columns) {
        if (visible == this->m_columns * row) {
          selected = index;
          break;
        }
        ++visible;
      }
    }
  }

  this->m_selected = selected;
  if (previous != selected) {
    this->Invalidate();
  }
}

// OFFSET: 0x008F39B0
// FUNCTION: fn_12274
void UIMinimapZoomWnd::Slot25(int, int) {
}

// OFFSET: 0x008F39C0
// FUNCTION: fn_12275
void UIPartySettingWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x008F39E0
// FUNCTION: fn_12276
void UIPetTamingDeceiveWnd::Slot25(int, int) {
  if (g_replayAnimationScaling != 0 || this->m_started != 0) {
    return;
  }

  this->m_state = 1;
  this->m_frame = 7;
  this->m_started = 1;
  g_world.ActiveGameMode()->SendMsg(149, this->m_target, 0, 0, 0);
}

// OFFSET: 0x008F3A40
// FUNCTION: fn_12277
void UISelectCartWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x008F3860
// FUNCTION: fn_12271
void UIItemCollectionWnd::Slot25(int x, int y) {
  if (x <= 60 || y >= 17) {
    return;
  }

  this->UIFrameWnd::Slot25(x, y);
  if (this->m_kind == 12) {
    UIWindow* comparison = g_windowMgr.unknown_0220;
    if (!comparison) {
      return;
    }
    comparison->geometry.targetAlpha = this->geometry.targetAlpha;
    comparison->geometry.alphaStart = this->geometry.alphaStart;
  }

  if (this->m_kind == 82) {
    UIWindow* comparison = g_windowMgr.unknown_0224;
    if (comparison) {
      comparison->geometry.targetAlpha = this->geometry.targetAlpha;
      comparison->geometry.alphaStart = this->geometry.alphaStart;
    }
  }
}

// OFFSET: 0x008F3A60
// FUNCTION: fn_12278
void UIStatusWnd::Slot25(int x, int y) {
  if (y < this->m_titleHeight) {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x008F3BA0
// FUNCTION: fn_12281
void UICombinedCardItemCollection_ComparisonWnd::Slot31(int, int) {
}

// OFFSET: 0x008F3B70
// FUNCTION: fn_12280
void UICombinedCardItemCollectionWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
  if (UIWindow* comparison = g_windowMgr.unknown_0224) {
    comparison->geometry.targetAlpha = this->geometry.targetAlpha;
    comparison->geometry.alphaStart = this->geometry.alphaStart;
  }
}

// OFFSET: 0x008F3BB0
// FUNCTION: fn_12282
void UIEquipWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);

  if (this->m_page != 3) {
    return;
  }

  for (int i = 0; i < 4; ++i) {
    const int left = 6 + i * 68;
    if (left <= x && x <= left + 64 && y >= 58 && y <= 150) {
      const int slot = this->m_firstSlot + i;
      if (slot >= 0 && this->m_hoverSlot != slot) {
        this->m_hoverSlot = slot;
      }
      return;
    }
  }
}

// OFFSET: 0x008F3C20
// FUNCTION: fn_12283
void UIItemCollectionWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
  if (UIWindow* comparison = g_windowMgr.unknown_0220) {
    comparison->geometry.targetAlpha = this->geometry.targetAlpha;
    comparison->geometry.alphaStart = this->geometry.alphaStart;
  }
}

// OFFSET: 0x008F3C50
// FUNCTION: fn_12284
void UIItemCollection_ComparisonWnd::Slot31(int, int) {
}

// OFFSET: 0x008F46C0
// FUNCTION: fn_12290
void UIStatusWnd::Slot28(int x, int y) {
  if (this->geometry.unknown_0030 != 0) {
    return;
  }

  struct Point {
    int x;
    int y;
  };
  const Point stats[16] = {
    {6, 25}, {6, 41}, {6, 57}, {6, 73}, {6, 89}, {6, 105}, {108, 24}, {108, 40}, {108, 56}, {108, 72}, {108, 88}, {108, 104}, {194, 24}, {194, 40}, {194, 56}, {194, 72},
  };
  const char* statTexts[16] = {
    LookupMessage(3112), LookupMessage(3113), LookupMessage(3114), LookupMessage(3115), LookupMessage(3116), LookupMessage(3117),
    LookupMessage(3118), LookupMessage(3124), LookupMessage(3120), LookupMessage(3121), LookupMessage(3123), LookupMessage(3122),
    LookupMessage(3119), LookupMessage(3125), LookupMessage(3126), LookupMessage(3127),
  };

  for (int index = 0; index < 16; ++index) {
    const Point& point = stats[index];
    if (point.x <= x && x <= point.x + 80 && point.y <= y && y <= point.y + 12) {
      int tipX = point.x - this->m_offsetX;
      int tipY = this->m_titleHeight - this->m_offsetY - 19 + point.y;
      this->ToScreen(tipX, tipY);
      this->m_controls->services.Tooltip(statTexts[index], tipX, tipY, -1, true, true);
      break;
    }
  }

  if (this->m_extended != 1) {
    return;
  }

  const Point traits[13] = {
    {6, 146}, {6, 162}, {6, 178}, {6, 194}, {6, 210}, {6, 226}, {108, 145}, {108, 161}, {108, 177}, {108, 193}, {108, 209}, {194, 145}, {194, 161},
  };
  const char* traitTexts[13] = {
    LookupMessage(3614), LookupMessage(3615), LookupMessage(3616), LookupMessage(3617), LookupMessage(3618), LookupMessage(3619), LookupMessage(3620),
    LookupMessage(3621), LookupMessage(3624), LookupMessage(3625), LookupMessage(3626), LookupMessage(3622), LookupMessage(3623),
  };

  for (int index = 0; index < 13; ++index) {
    const Point& point = traits[index];
    if (point.x <= x && x <= point.x + 80 && point.y <= y && y <= point.y + 12) {
      int tipX = point.x - 16;
      int tipY = point.y - 13;
      this->ToScreen(tipX, tipY);
      this->m_controls->services.Tooltip(traitTexts[index], tipX, tipY, -1, true, true);
      return;
    }
  }
}

// OFFSET: 0x008F4A80
// FUNCTION: fn_12291
void UIEquipWnd::Slot27(int x, int y) {
  this->UIFrameWnd::Slot27(x, y);
  this->Slot28(x, y);
}

// OFFSET: 0x008F4AB0
// FUNCTION: fn_12292
void UIMenuWnd::Slot27(int x, int y) {
  this->Slot28(x, y);
}

// OFFSET: 0x008F4680
// FUNCTION: fn_12289
void UIMenuWnd::Slot28(int x, int y) {
  const int previous = this->m_selected;
  this->m_selected = this->ItemAt(x, y);
  if (previous != this->m_selected) {
    this->Invalidate();
  }
}

// OFFSET: 0x008D99D0
// FUNCTION: fn_12134
int UIMenuWnd::ItemAt(int x, int y) {
  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    return -1;
  }

  int row = y / (this->m_itemHeight / 2);
  const unsigned index = row < 0 ? 0 : row;
  const unsigned last = static_cast<unsigned>(this->m_items.size()) - 1;
  return index <= last ? index : last;
}

// OFFSET: 0x008F4D40
// FUNCTION: fn_12294
void UIEquipWnd::Slot19() {
}

// OFFSET: 0x008F4F80
// FUNCTION: fn_12296
void UIItemCollectionWnd::Slot19() {
  if (this->m_previewTick != 0 && timeGetTime() > this->m_previewTick + 100) {
    this->m_preview->Invalidate();
    this->m_previewTick = timeGetTime();
  }
}

// OFFSET: 0x008F4FC0
// FUNCTION: fn_12297
void UINotifyItemObtainWnd::Slot19() {
  if (timeGetTime() > this->m_shownTick + 5000) {
    this->m_frameContext->windows.CloseWindow(58);
  }
}

// OFFSET: 0x008F4FF0
// FUNCTION: fn_12298
void UIPetTamingDeceiveWnd::Slot19() {
  this->Invalidate();
  const int frame = this->m_tick / this->m_delay;

  if (this->m_finishTime == 0xFFFFFFFF) {
    if (this->m_frame == frame && this->m_delay <= this->m_maxDelay) {
      this->m_finishTime = timeGetTime();
    }
  } else if (this->m_frame == frame && this->m_delay <= this->m_maxDelay && timeGetTime() - this->m_finishTime >= 2000) {
    this->m_frameContext->windows.CloseWindow(91);
  }
}

// OFFSET: 0x008F5070
// FUNCTION: fn_12299
void UIProhibitListWnd::Slot19() {
  const int count = static_cast<int>(g_session.m_prohibitionLog.size());
  if (this->m_count != count) {
    this->m_count = count;
    this->Invalidate();
  }
}

// OFFSET: 0x008F50B0
// FUNCTION: fn_12300
void UISayDialogWnd::Slot19() {
  for (UIWindow* child : this->m_children) {
    child->Slot19();
  }
}

// OFFSET: 0x008F5990
// FUNCTION: fn_12307
void UIItemIdentifyWnd::Slot33(int, int) {
}

// OFFSET: 0x008F59A0
// FUNCTION: fn_12308
void UIWhisperWnd::Slot33(int, int) {
  this->SendMsg(nullptr, 49, 0, 0, 0, 0);
}

// OFFSET: 0x008F5A10
// FUNCTION: fn_12310
void UIExchangeWnd::Slot35(int delta) {
  this->SendMsg(this->m_scrollBar, 7, delta < 0 ? 1 : -1, 0, 0, 0);
}

// OFFSET: 0x008F59C0
// FUNCTION: fn_12309
void UIEquipWnd::Slot35(int delta) {
  if (g_session.unknown_80C8 > 4 && this->m_page == 3) {
    this->SendMsg(this->m_scrollBar, 8, delta < 0 ? 1 : -1, 0, 0, 0);
  }
}

// OFFSET: 0x008F5A50
// FUNCTION: fn_12311
void UIItemIdentifyWnd::Slot35(int delta) {
  if (this->m_scrollable == 0) {
    return;
  }

  int step = this->m_rows - 1;
  if (step > 3) {
    step = 3;
  }
  this->SendMsg(this->m_scrollBar, 7, delta < 0 ? step : -step, 0, 0, 0);
}

// OFFSET: 0x008F66B0
// FUNCTION: fn_12320
void UIComboBoxIDWnd::Resize(int width, int height) {
  this->UIFrameWnd::Resize(width, height);
  this->m_list->Resize(width, height);
  this->m_list->Refresh();
  this->m_list->Invalidate();
}

// OFFSET: 0x008F6700
// FUNCTION: fn_12321
void UIComboBoxWnd::Resize(int width, int height) {
  this->UIFrameWnd::Resize(width, height);
  this->m_list->Resize(width, height);
  this->m_list->Refresh();
  this->m_list->Invalidate();
}

// OFFSET: 0x008F6750
// FUNCTION: fn_12322
void UISayDialogWnd::Resize(int width, int height) {
  this->UIFrameWnd::Resize(width, height);
  this->m_viewer->Resize(width - 20, height - 40);

  for (UIWindow* button : this->m_buttons) {
    if (button == nullptr) {
      continue;
    }

    int x = this->geometry.width - 4;
    const int y = this->geometry.height - 24;
    for (UIWindow* other : this->m_buttons) {
      if (button != other) {
        x += -4 - other->geometry.width;
      }
    }
    button->Move(x - button->geometry.width, y);
  }
}

// OFFSET: 0x008F8860
// FUNCTION: fn_12332
int UIChooseSellBuyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    const int response = static_cast<int>(a);
    this->m_acceptance->recorder.Record(this->geometry.id, response, b, c, d);

    if (this->geometry.visible != 0) {
      if (response == 185) {
        this->m_context->mode->SendModeMessage(40, 0, 0, 0, 0);
        this->m_frameContext->windows.CloseWindow(25);
      } else if (response == 223 || response == 224) {
        this->m_context->mode->SendModeMessage(response == 223 ? 37 : 38, this->identity, 0, 0, 0);
        this->m_frameContext->windows.LookupWindow(25)->Show(0);
      } else {
        return 0;
      }

      this->m_acceptance->firstShopItems.clear();
      this->m_acceptance->secondShopItems.clear();
      this->m_acceptance->thirdShopItems.clear();
    }

    return 0;
  }

  if (message == 28) {
    this->identity = static_cast<std::uint32_t>(a);
    return 0;
  }

  if (message == 123) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    std::uint16_t marker;
    std::memcpy(&marker, bytes, sizeof(marker));
    unsigned offset = 6;

    if (marker == 20850) {
      while (static_cast<unsigned>(b) > offset) {
        std::uint16_t kind;
        std::uint32_t size;
        std::memcpy(&kind, bytes + offset, sizeof(kind));
        std::memcpy(&size, bytes + offset + 2, sizeof(size));
        offset += 6;

        if (kind == 20851) {
          this->m_acceptance->replayShopState = 1;
          this->SendMsg(nullptr, 28, 0, 0, 0, 0);
          this->m_frameContext->windows.CloseWindow(22);
          this->m_frameContext->windows.CloseWindow(23);
          this->m_frameContext->windows.CloseWindow(24);

          return 0;
        }

        offset += size;
      }
    }

    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
