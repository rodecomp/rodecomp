#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Skills/SkillInfo.h"
#include "File/File.h"
#include "UI/Items/UISpellListWnd.h"
#include "Client/Registry.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/TipBoxTitles.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UISpriteButton.h"
#include "UI/Chat/UIChatRoomMakeWnd.h"
#include "UI/Chat/UIWhisperWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIItemTagCtrl.h"
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
#include "UI/Dialogs/UINotifyJobLevelUpWnd.h"
#include "UI/Dialogs/UINotifyLevelUpWnd.h"
#include "UI/Dialogs/UINotifyQuestWnd.h"
#include "UI/Dialogs/UIPetTamingDeceiveWnd.h"
#include "UI/Dialogs/UIProhibitListWnd.h"
#include "UI/Dialogs/UIQuitWnd.h"
#include "UI/Dialogs/UIRestartWnd.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Dialogs/UISelectCartWnd.h"
#include "UI/Dialogs/UITipOfTheDayWnd.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Dialogs/UIToolTipWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "UI/Items/UICardItemIllustWnd.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Items/UIExchangeWnd.h"
#include "UI/Items/UIItemCollectionWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Items/UIItemParamChangeDisplayWnd.h"
#include "UI/Items/UINotifyItemObtainWnd.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Party/UIPartySettingWnd.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Shop/UInCash_CallWnd.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Skill/UISkillNameChangeWnd.h"
#include "UI/Status/UIStatusWnd.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITagTextViewer.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIChangeNameWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIMonsterDisplayWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextTokens.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/TextEncoding.h"
#include "Render/Text/TextContext.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Utility/Text/GroupedInteger.h"
#include "Client/Mode/ModeTooltip.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <direct.h>
#include <io.h>
#include <mbctype.h>
#include <mbstring.h>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char prefix[] = "유저인터페이스\\";
#pragma pack(push, 1)
  struct FramedMessageHeader {
    /* 0000 */ std::uint16_t type;
    /* 0002 */ std::uint32_t length;
  };
#pragma pack(pop)
  static_assert(sizeof(FramedMessageHeader) == 6);
}

// STRING: st_16833
static const char menuSeparatorText[] = "-";
// GLOBAL: gl_24749
const char* g_menuSeparator = menuSeparatorText;
// STRING: st_03412
static const char bookmarkKeySuffixText[] = "\\BookMark";

// GLOBAL: gl_24751
const char* g_bookmarkKeySuffix = bookmarkKeySuffixText;

// OFFSET: 0x0087B500
// FUNCTION: fn_11241
void UIChatHisBox::Configure(int x, int y, int width, int height, unsigned char) {
  this->transparent = 1;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
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

// OFFSET: 0x008DD9A0
// FUNCTION: fn_12163
void UIChooseWarpWnd::Slot15(int width, int height) {
  auto* title = new UIStaticText(*this->m_context, *this->m_controls);
  title->SetSize(this->geometry.width - 30, 15);
  title->Move(12, 9);
  title->SetText(LookupMessage(213), 0);
  title->SetFont(0, 12, 0, 0);
  this->AddChild(title);

  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  int listWidth = width - 24;
  int listHeight = height - 55;
  if (listWidth < 20) {
    listWidth = 20;
  }
  if (listHeight < 30) {
    listHeight = 30;
  }
  this->m_list->Configure(12, 26, listWidth, listHeight, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{this->geometry.width - 92, this->geometry.height - 24}, {this->geometry.width - 46, this->geometry.height - 24}};
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}

// OFFSET: 0x008DE0E0
// FUNCTION: fn_12164
void UIChooseWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  int listHeight = height - 55;
  if (listHeight < 30) {
    listHeight = 30;
  }
  int listWidth = width - 24;
  if (listWidth < 20) {
    listWidth = 20;
  }
  this->m_list->Configure(12, 26, listWidth, listHeight, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);
  this->m_message0 = 184;

  this->m_okButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_okButton->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  this->m_okButton->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  this->m_okButton->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  this->m_okButton->SetSize(this->m_okButton->BitmapWidth(), this->m_okButton->BitmapHeight());
  this->m_okButton->Move(this->geometry.width - 92, this->geometry.height - 24);
  this->m_okButton->SetId(184);
  this->AddChild(this->m_okButton);

  this->m_cancelButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_cancelButton->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  this->m_cancelButton->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  this->m_cancelButton->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  this->m_cancelButton->SetSize(this->m_cancelButton->BitmapWidth(), this->m_cancelButton->BitmapHeight());
  this->m_cancelButton->Move(this->geometry.width - 46, this->geometry.height - 24);
  this->m_cancelButton->SetId(185);
  this->AddChild(this->m_cancelButton);
}

// OFFSET: 0x008DE520
// FUNCTION: fn_12169
void UIDisconnectedServerMsgWnd::Slot15(int width, int height) {
  auto& text = *this->m_textContext;
  auto* button = new UIBitmapButton(*this->m_context, text.buttons);
  button->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(234, 96);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;

  this->m_viewer = new UITextViewer(*this->m_context, text);
  this->m_viewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2)), 38, 0, 0);
}

// OFFSET: 0x008DE6D0
// FUNCTION: fn_12170
void UIEmailAddressWnd::Slot15(int, int) {
  g_modeMgr.gameMode->SendMsg(10013, 0, 0, 0, 0);
  this->m_edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edit->SetSize(this->geometry.width - 77, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 49;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->SetText("");
  this->m_edit->mask = 42;
  this->AddChild(this->m_edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);
  this->m_edit->SelectAll();

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 28;
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;
}

// OFFSET: 0x008E9360
// FUNCTION: fn_12202
void __stdcall Nullsub301(int, int) {
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

// OFFSET: 0x008EED00
// FUNCTION: fn_12230
void UIMenuWnd::Paint() {
  auto& controls = *this->m_controls;
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  unsigned fillRed, fillGreen, fillBlue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(fillRed, fillGreen, fillBlue));

  const auto* leftUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp")));
  this->DrawBitmap(0, 0, &leftUp->image, false);
  const auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, &leftDown->image, false);
  const auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, &rightUp->image, false);
  const auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, &rightDown->image, false);

  SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
  const int count = static_cast<int>(this->m_items.size());
  for (int index = 0; index < count; ++index) {
    const std::string& item = this->m_items[index];
    const bool separator = item == g_menuSeparator;
    const bool afterSeparator = index != 0 && this->m_items[index - 1] == item;
    if (separator) {
      unsigned lineRed, lineGreen, lineBlue;
      SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, lineRed, lineGreen, lineBlue);
      this->FillRegion(0, this->m_itemHeight * index / 2 + 2, this->geometry.width, 1, RGB(lineRed, lineGreen, lineBlue));
      continue;
    }

    const int row = afterSeparator ? index - 1 : index;
    if (index == this->m_selected) {
      this->FillRegion(3, this->m_itemHeight / 2 * row + 1, this->geometry.width - 6, this->m_itemHeight - 2, RGB(red, green, blue));
    }
    this->DrawTextAt(controls.fonts, controls.locale, 3, this->m_itemHeight * row / 2 + 1, item.c_str(), 0, 0, 12, this->m_colors[index], 0, 0);
  }
}

// OFFSET: 0x008EF0A0
// FUNCTION: fn_12231
void UIMinimapZoomWnd::Paint() {
  auto& controls = *this->m_controls;
  this->ClearSurface(0xFFFF00FF);
  int column = 0;
  int row = 0;
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->PlayerCell(&column, &row);
  const std::string text = FormatOwnedString(" %d  %d", column, row);
  this->DrawTextAt(controls.fonts, controls.locale, 19, 128, text.c_str(), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 20, 128, text.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
}

// OFFSET: 0x008EF1A0
// FUNCTION: fn_12232
void UIMonsterDisplayWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, "", 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));

  char spriteName[128];
  char actionName[128];
  std::sprintf(spriteName, "npc\\%s.spr", this->m_name.c_str());
  std::sprintf(actionName, "npc\\%s.act", this->m_name.c_str());
  if (!ResourceMgr::Instance().Exists(actionName) || !ResourceMgr::Instance().Exists(spriteName)) {
    return;
  }

  const auto* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));
  const auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));
  if (!action || !sprite) {
    return;
  }

  const ActionFrame& frame = action->Frame(0, 0);
  SpriteAtlasRecord record{};
  int left = 0;
  int top = 0;
  int offsetX = 0;
  int offsetY = 0;
  for (int index = 0; index < frame.cellCount; ++index) {
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    if (left < -cell.x) {
      left = -cell.x;
      offsetX = (75 - image->width) / 2;
    }
    if (top < -cell.y) {
      top = -cell.y;
      offsetY = (125 - image->height) / 2;
    }
  }

  for (int index = 0; index < frame.cellCount; ++index) {
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    record.u0 = 0.0f;
    record.v0 = 0.0f;
    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, sprite->palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, sprite->palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    }

    int x = cell.x + offsetX + left;
    if (x < 0) {
      x = 0;
    }
    int y = cell.y + offsetY + top;
    if (y < 0) {
      y = 0;
    }
    const int width = x + image->width > 75 ? 75 - x : image->width;
    const int height = y + image->height > 125 ? 125 - y : image->height;
    this->geometry.surface->DrawSurface(
      x, y, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f), width, height, cell.mirrored != 0, image->unknown_0004 + 1,
      image->unknown_0006 + 1, 0, 0
    );
  }
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

// OFFSET: 0x009002D0
// FUNCTION: fn_12367
int UIPartyInvitationToWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == 184) {
    std::string name;
    name = this->m_edit->Text();
    this->m_edit->SelectAll();
    if (std::strlen(name.c_str()) != 0) {
      g_modeMgr.GetGameMode()->SendMsg(59, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
    } else {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2072)), 0xFF, 0, 0, 0);
    }
  } else if (argument1 == 185) {
    g_windowMgr.CloseWindow(208);
  }

  return 0;
}

// OFFSET: 0x00900A80
// FUNCTION: fn_12369
int UIPasswordWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    if (a == 184) {
      CMode* mode = g_modeMgr.gameMode;
      mode->SendMsg(45, this->unknown_00BC, reinterpret_cast<int>(this->m_edit->Text()), 0, 0);
      this->m_frameContext->windows.CloseWindow(29);
    }
    return 0;
  case 47:
    this->unknown_00BC = static_cast<int>(a);
    return 0;
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008EFCF0
// FUNCTION: fn_12244
void UIProhibitListWnd::Paint() {
  auto& controls = *this->m_controls;
  char text[256];

  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(714), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - left->image.height, &left->image, false);
  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - right->image.width, this->geometry.height - right->image.height, &right->image, false);

  int y = 0;
  const int count = static_cast<int>(g_session.m_prohibitionLog.size());
  for (int index = 0; index < count; ++index) {
    const ProhibitionLogEntry& entry = g_session.m_prohibitionLog[index];
    if (std::strcmp(entry.ownerName, g_session.m_serverName.c_str()) != 0 || entry.aid != this->m_aid) {
      continue;
    }

    if (y == 0) {
      std::sprintf(text, "AID : %d        CharacterName : %s", entry.aid, entry.name);
      this->DrawTextAt(controls.fonts, controls.locale, 10, 25, text, 0, 0, 12, 0, 0, 0);
      std::sprintf(text, "ServerName : %s", g_session.m_prohibitionLog[index].ownerName);
      this->DrawTextAt(controls.fonts, controls.locale, 10, 41, text, 0, 0, 12, 0, 0, 0);
    }

    const ProhibitionLogEntry& record = g_session.m_prohibitionLog[index];
    const int date = static_cast<int>(record.date);
    const int time = static_cast<int>(record.time);
    std::sprintf(
      text, "ProhibitTime : %d분   LogTime : %d년 %d월 %d일   %d시 %d분 %d초", record.minutes, date >> 16 & 0xFFFF, date >> 8 & 0xFF, date & 0xFF, time >> 16 & 0xFF,
      time >> 8 & 0xFF, time & 0xFF
    );
    this->DrawTextAt(controls.fonts, controls.locale, 10, y + 65, text, 0, 0, 12, 0, 0, 0);
    y += 18;
  }
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

// OFFSET: 0x008F04F0
// FUNCTION: fn_12252
void UISpellListWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, controls.services.MessageText(697), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);

  int row = 0;
  int index = 0;
  CSkillInfo skill;
  for (unsigned position = 0; position < this->m_skills.size(); ++position, ++index) {
    char name[64];
    std::strcpy(name, CSkillInfo::DisplayName(*g_session.m_hotkeyScript, static_cast<int>(this->m_skills[position])));
    if (index < this->m_scroll * this->m_columns) {
      continue;
    }

    const int y = row / this->m_columns * 32 + 34;
    if (this->m_selected == index) {
      unsigned red, green, blue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      this->FillRegion(5, y - 3, this->geometry.width - 24, 30, RGB(red, green, blue));
    }

    skill.id = this->m_skills[position];
    char path[256];
    g_session.ItemBitmapPath(skill.IdName(*g_session.m_hotkeyScript), path);
    this->DrawBitmap(8, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)))->image, true);
    this->DrawTextAt(controls.fonts, controls.locale, 40, y + 4, name, 0, 0, 12, 0, 0, 0);

    ++row;
    if (row == this->m_rows * this->m_columns) {
      break;
    }
  }
}

// OFFSET: 0x00DE0E20
// FUNCTION: fn_23473
void CSession::ItemBitmapPath(const char* name, char* output) {
  std::sprintf(output, "유저인터페이스\\item\\%s.bmp", name);
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
  g_modeMgr.GetGameMode()->SendMsg(149, this->m_target, 0, 0, 0);
}

// OFFSET: 0x008EFB40
// FUNCTION: fn_12243
void UIPetTamingDeceiveWnd::Paint() {
  const auto* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("slotmachine.act"));
  const auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("slotmachine.spr"));
  const int frame = this->m_tick / this->m_delay;
  const ActionFrame& current = action->Frame(this->m_state, frame);
  SpriteAtlasRecord record{};

  for (int index = 0; index < current.cellCount; ++index) {
    const SpriteFrameCell& cell = current.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[0][cell.image];
    CTexture* texture = g_renderer->atlas.Find(image, sprite->palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, sprite->palette, record, g_serviceType, g_resourceReuseDelay);
    }
    this->geometry.surface->DrawSurface(
      this->geometry.width / 2 + cell.x, this->geometry.height / 2 + cell.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->unknown_0004 + 1, image->unknown_0006 + 1, 0, 0
    );
  }

  if (this->m_frame != frame || this->m_delay > this->m_maxDelay) {
    ++this->m_tick;
  }
  if (frame >= static_cast<int>(action->FrameCount(this->m_state))) {
    this->m_tick = 0;
    if (this->m_frame != -1) {
      ++this->m_delay;
    }
  }
}

// OFFSET: 0x008F3A40
// FUNCTION: fn_12277
void UISelectCartWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x008CE2C0
// FUNCTION: fn_12069
void UISelectCartWnd::AddCart(int index, int type) {
  const char* names[] = {
      "NOTHING_CART",
      "손수레",
      "손수레1",
      "손수레2",
      "손수레3",
      "손수레4",
      "손수레5",
      "손수레6",
      "손수레7",
      "손수레8",
      "선물상자카트",
      "포링싣은카트",
      "포링카트",
      "마도카트",
  };
  std::string name = names[type];
  int x = (index % 5) * 70;
  int y = (index / 5) * 82 + 28;
  switch (type) {
  case 1:
  case 2:
  case 10:
    y -= 13;
    break;
  case 4:
    x += 18;
    y -= 10;
    break;
  case 5:
    x += 15;
    y -= 5;
    break;
  case 6:
  case 7:
  case 8:
  case 9:
    x += 13;
    break;
  case 11:
    x += 16;
    y -= 13;
    break;
  case 12:
    y -= 23;
    break;
  case 13:
    x += 4;
    y -= 10;
    break;
  }
  UISpriteButton* button = new UISpriteButton(*this->m_context, *this->m_controls);
  const std::string prefix = "이팩트\\";
  button->SetSprite((prefix + name + ".spr").c_str());
  button->SetActName((prefix + name + ".act").c_str());
  button->SetSize(100, 100);
  button->Move(x, y);
  button->unknown_00BC = 0;
  button->unknown_00C0 = 0;
  button->SetId(type + 9000);
  this->AddChild(button);
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

// OFFSET: 0x008F3C60
// FUNCTION: fn_12285
void UIMenuWnd::Slot31(int x, int y) {
  const int index = this->ItemAt(x, y);
  this->m_selected = index;
  if (index == -1 || this->m_items[index] == g_menuSeparator) {
    return;
  }

  int separators = 0;
  const int count = static_cast<int>(this->m_items.size());
  if (count != 0) {
    const int last = index < count ? index : count - 1;
    for (int i = 0; i < last; ++i) {
      if (this->m_items[i] == g_menuSeparator) {
        ++separators;
      }
    }
  }

  const int choice = (index - separators) / 2;
  UIWindow* target = this->m_menuId == 301 ? g_windowMgr.FindSnapExcludedWindow(static_cast<int>(this->unknown_00CC))
                                           : this->m_frameContext->windows.LookupWindow(this->m_menuId);
  if (target) {
    target->SendMsg(nullptr, 48, choice, 0, 0, 0);
  } else if (this->m_menuId == 10041) {
    this->m_context->mode->SendModeMessage(24, choice, 0, 0, 0);
  }
  this->m_frameContext->windows.CloseWindow(18);
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

// OFFSET: 0x008D9D00
// FUNCTION: fn_12136
ITEM_INFO UIItemParamChangeDisplayWnd::FindEquippedItem(int location) {
  if (location == -1) {
    return ITEM_INFO(0);
  }

  for (int index = 0; index < 8; ++index) {
    ITEM_INFO item = g_session.GetItem17F0(index);
    if (item.unknown_0000[1] > 0 && item.quantity == 1 && (item.unknown_0000[2] & location) != 0) {
      return item;
    }
  }
  return ITEM_INFO(0);
}

// OFFSET: 0x008CEA60
// FUNCTION: fn_12070
void UIMenuWnd::AddItem(const char* text, const std::uint32_t& color) {
  this->m_items.push_back(text);
  this->m_colors.push_back(color);
}

// OFFSET: 0x008CEED0
// FUNCTION: fn_12073
void UITipboxWnd::ShowTip(int id) {
  g_itemTagOwner->SelectTip(id);
  this->SendMsg(this, 6, 543, 0, 0, 0);
  reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this);
}

// OFFSET: 0x008D0230
// FUNCTION: fn_12083
void UIExchangeWnd::Slot10(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (this->unknown_00CC != 0) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 1) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.window_01EC->SendMsg(nullptr, 13, 5, 0, 0, 0);
}

// OFFSET: 0x008D02D0
// FUNCTION: fn_12084
void UInCashWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 4:
    if (x < 445) {
      const int quantity = g_session.GetItem1720QuantityByItemId(drag->name.c_str());
      if (quantity < 2) {
        this->SendMsg(nullptr, 38, 0, 0, 0, 0);
        return;
      }

      reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
      g_windowMgr.window_01EC->SendMsg(nullptr, 13, 20, quantity, 0, 0);
      return;
    }
    break;
  case 6:
    if (x > 444) {
      reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
      g_windowMgr.window_01EC->SendMsg(nullptr, 13, 19, 0, 0, 0);
      return;
    }
    break;
  }

  mode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x008D9F00
// FUNCTION: fn_12137
bool UISkillDescribeWnd::Slot42(void*) {
  return true;
}

// OFFSET: 0x008D9F10
// FUNCTION: fn_12138
bool UIWhisperWnd::Slot42(void*) {
  return true;
}

// OFFSET: 0x008DA220
// FUNCTION: fn_12141
void UIExchangeWnd::LayoutButtons() {
  const int x[3] = {3, 260, 510};
  const int y[3] = {this->unknown_00CC * 120, this->unknown_00CC != 0 && this->unknown_00D0 != 0 && this->unknown_00D8 == 0 ? 0 : 120, 0};

  for (int index = 0; index < 3; ++index) {
    this->m_buttons[index]->Move(x[index], this->geometry.height - 23 + y[index]);
  }
  this->Invalidate();
}

// OFFSET: 0x008DA2E0
// FUNCTION: fn_12142
void UIEquipWnd::NotifyEquipmentChanged() {
  if (this->unknown_0124) {
    this->unknown_0124->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x008DA420
// FUNCTION: fn_12143
bool UIMinimapZoomWnd::Slot08() {
  return true;
}

// OFFSET: 0x008DA430
// FUNCTION: fn_12144
bool UIEquipWnd::Slot03() {
  return true;
}

// OFFSET: 0x008DA440
// FUNCTION: fn_12145
bool UIStatusWnd::Slot03() {
  return true;
}

// OFFSET: 0x008DA460

// OFFSET: 0x008DACF0
// FUNCTION: fn_12149
void UINpcTextEditDialogWnd::InsertSkillTag(int tagId) {
  if (!this->m_tagCtrl) {
    this->m_tagCtrl = new UIItemTagOnMegaPhone;
    if (!this->m_tagCtrl) {
      return;
    }
  }

  if (this->m_tagCtrl->tags.size() >= 3) {
    return;
  }

  if (this->edit->Length() + this->edit->unknown_00F0 > this->edit->maximum) {
    return;
  }

  std::string tag = this->m_tagCtrl->sub_874A40(tagId, 0xD630A9);
  {
    std::string text = this->edit->Text();
    this->m_tagCtrl->taggedText = text;
  }
  this->edit->SetText((this->edit->Text() + tag).c_str());
  this->edit->MoveCursorToEnd();
}

// OFFSET: 0x008DAF40
// FUNCTION: fn_12150
void UICombinedCardItemCollectionWnd::Move(int x, int y) {
  UIFrameWnd::Move(x, y);
  if (UIWindow* comparison = g_windowMgr.unknown_0224) {
    if (UIWindow* window = g_windowMgr.LookupWindow(82)) {
      comparison->Move(window->geometry.x - window->geometry.width, window->geometry.y);
    }
  }
}

// OFFSET: 0x008DAF80
// FUNCTION: fn_12151
void UIEquipWnd::Move(int x, int y) {
  UIFrameWnd::Move(x, y);
}

// OFFSET: 0x008DAF90
// FUNCTION: fn_12152
void UIItemCollectionWnd::Move(int x, int y) {
  UIFrameWnd::Move(x, y);
  if (UIWindow* comparison = g_windowMgr.unknown_0220) {
    if (UIWindow* window = g_windowMgr.LookupWindow(12)) {
      comparison->Move(window->geometry.x - window->geometry.width, window->geometry.y);
    }
  }
}

// OFFSET: 0x008DAFD0
// FUNCTION: fn_12153
void UIItemCollection_ComparisonWnd::Move(int, int) {
}

// OFFSET: 0x008DAFE0
// FUNCTION: fn_12154
void UIMenuWnd::Move(int x, int y) {
  if (y > g_renderer->viewport.height - this->geometry.height) {
    y += -2 - this->geometry.height;
  }
  if (x > g_renderer->viewport.width - this->geometry.width) {
    x += -2 - this->geometry.width;
  }
  this->geometry.Move(x, y);
}

// OFFSET: 0x008DB030
// FUNCTION: fn_12155
void UIWhisperWnd::Move(int x, int y) {
  UIFrameWnd::Move(x, y);
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

// OFFSET: 0x008F6370
// FUNCTION: fn_12315
void UIItemParamChangeDisplayWnd::SumEquippedStats() {
  this->m_before = 0;
  this->m_beforeSecond = 0;

  int locations = 0;
  for (int index = 0; index < 8; ++index) {
    ITEM_INFO item = g_session.GetItem17F0(index);
    if (item.unknown_0000[1] > 0 && item.quantity == 1) {
      const ITEM_DESC& description = FindItemDescription(std::atoi(item.itemId.c_str()));
      const bool duplicate = (description.unknown_002C & locations) != 0;
      locations |= description.unknown_002C;
      if (duplicate) {
        continue;
      }

      if (description.unknown_0034.size() == 2) {
        this->m_before += description.unknown_0034[0];
        this->m_beforeSecond += description.unknown_0034[1];
      }
    }
  }
}

// OFFSET: 0x008F64A0
// FUNCTION: fn_12316
void __stdcall Nullsub313() {
}

// OFFSET: 0x008F6180
// FUNCTION: fn_12314
int UIBookWnd::LoadBookmark(const char* name) {
  char path[512];
  std::strcpy(path, g_subKey);
  std::strcat(path, g_bookmarkKeySuffix);
  std::strcat(path, "\\");
  std::strcat(path, g_session.m_serverName.c_str());
  std::strcat(path, "\\");
  std::strcat(path, g_session.DecodePlayerName());

  HKEY key;
  const LSTATUS status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &key);
  DWORD page = 1;
  if (status != ERROR_SUCCESS) {
    return 1;
  }
  RegCloseKey(key);

  HKEY values;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &values) == ERROR_SUCCESS) {
    DWORD type = REG_DWORD;
    DWORD size = sizeof(page);
    if (RegQueryValueExA(values, name, nullptr, &type, reinterpret_cast<BYTE*>(&page), &size) != ERROR_SUCCESS) {
      page = 1;
    }
    RegCloseKey(values);
  }
  return page;
}

// OFFSET: 0x008F6640
// FUNCTION: fn_12319
void UIExchangeWnd::CancelInput() {
  if (this->unknown_00CC == 0) {
    return;
  }

  if (this->unknown_00C0) {
    InputFocus focus{g_windowMgr.hoverWindow};
    focus.Set(nullptr);
    this->unknown_00C0->Move(129, 500);
    this->unknown_00C0 = nullptr;
    if (g_windowMgr.window_01C8) {
      g_windowMgr.window_01C8->SendMsg(nullptr, 54, 0, 0, 0, 0);
    }
  }
  this->Invalidate();
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

// OFFSET: 0x008F9570
// FUNCTION: fn_12337
int UIComboBoxIDWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (this->m_owner && message == 39) {
    return this->m_owner->SendMsg(this, 39, argument1, argument2, argument3, argument4);
  }
  return this->m_list->SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008F95B0
// FUNCTION: fn_12338
int UIComboBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (this->m_owner && message == 39) {
    return this->m_owner->SendMsg(this, 39, argument1, argument2, argument3, argument4);
  }
  return this->m_list->SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008F97D0
// FUNCTION: fn_12340
int UIDisconnectedServerMsgWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (argument1 == 184) {
    g_modeMgr.GetGameMode()->SendMsg(141, 0, 0, 0, 0);
  }
  return 0;
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

// OFFSET: 0x008F6AC0
// FUNCTION: fn_12325
int UIBabyAcceptWnd2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    this->m_acceptance->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);
    if (a == 184) {
      this->m_frameContext->windows.CloseWindow(110);
      this->m_context->mode->SendModeMessage(173, this->identity, 1, 0, 0);
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(110);
      this->m_context->mode->SendModeMessage(173, this->identity, 0, 0, 0);
    }
    return 0;
  }

  if (message == 91) {
    this->identity = static_cast<std::uint32_t>(a);
    const std::string text = this->m_acceptance->text.buttons.services.MessageText(2965);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }

  if (message != 123) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  const auto* packet = reinterpret_cast<const unsigned char*>(a);
  const unsigned length = static_cast<unsigned>(b);
  std::int16_t tag;
  std::memcpy(&tag, packet, sizeof(tag));
  if (tag == 0x542E) {
    unsigned offset = 6;
    bool scanning = true;
    while (scanning && offset < length) {
      std::int32_t size;
      std::memcpy(&tag, packet + offset, sizeof(tag));
      std::memcpy(&size, packet + offset + 2, sizeof(size));
      offset += 6;
      if (tag != 0x542F) {
        offset += size;
      }
      if (tag == 0x542F) {
        scanning = false;
      }
    }
  }
  return 0;
}

// OFFSET: 0x008F6D00
// FUNCTION: fn_12326
int UIBabyAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    this->m_acceptance->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);
    if (a == 184) {
      this->m_frameContext->windows.CloseWindow(105);
      this->m_frameContext->windows.EnsureWindow(110)->SendMsg(nullptr, 91, 184, b, 0, 0);
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(105);
      this->m_context->mode->SendModeMessage(173, this->identity, 0, 0, 0);
    }
    return 0;
  }

  if (message == 91) {
    this->identity = static_cast<std::uint32_t>(a);
    std::string text = "(";
    text += reinterpret_cast<const char*>(b);
    text += ")";
    text += this->m_acceptance->text.buttons.services.MessageText(794);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }

  if (message != 123) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  const auto* packet = reinterpret_cast<const unsigned char*>(a);
  const unsigned length = static_cast<unsigned>(b);
  std::int16_t tag;
  std::memcpy(&tag, packet, sizeof(tag));
  if (tag == 0x53FC) {
    unsigned offset = 6;
    while (offset < length) {
      std::uint32_t size;
      std::memcpy(&tag, packet + offset, sizeof(tag));
      std::memcpy(&size, packet + offset + 2, sizeof(size));
      offset += 6;
      if (tag == 0x53FD) {
        break;
      }
      if (tag == 0x53FE) {
        char name[260];
        std::memset(name, 0, sizeof(name));
        std::memcpy(name, packet + offset, size);
        offset += size;
        this->SendMsg(nullptr, 91, 0, reinterpret_cast<std::intptr_t>(name), 0, 0);
      } else {
        offset += size;
      }
    }
  }
  return 0;
}

// OFFSET: 0x008F7050
// FUNCTION: fn_12327
int UIBookWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 201:
      this->SendMsg(this, 6, 356, 0, 0, 0);
      this->m_frameContext->windows.CloseWindow(106);
      break;
    case 354:
      if (this->m_autoRead == 0) {
        this->SendMsg(this, 92, this->m_page == 1 ? 1 : this->m_page - 1, 0, 0, 0);
      }
      break;
    case 355:
      if (this->m_autoRead == 0) {
        this->SendMsg(this, 92, this->m_page == this->m_pageCount ? this->m_page : this->m_page + 1, 0, 0, 0);
      }
      break;
    case 356: {
      DWORD page = this->m_page;
      char path[512];
      std::strcpy(path, g_subKey);
      std::strcat(path, g_bookmarkKeySuffix);
      std::strcat(path, "\\");
      std::strcat(path, g_session.m_serverName.c_str());
      std::strcat(path, "\\");
      std::strcat(path, g_session.DecodePlayerName());

      HKEY key;
      DWORD disposition = 0;
      if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, path, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
        RegSetValueExA(key, this->m_bookName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&page), sizeof(page));
        RegCloseKey(key);
      }
      break;
    }
    }
    break;

  case 20:
    this->SendMsg(this, 6, 354, 0, 0, 0);
    break;

  case 21:
    this->SendMsg(this, 6, 355, 0, 0, 0);
    break;

  case 92: {
    this->m_autoRead = 0;
    this->Show(1);
    if (this->unknown_00B8) {
      g_windowMgr.Schedule(this->unknown_00B8);
      this->unknown_00B8 = nullptr;
    }

    int page = static_cast<int>(a);
    if (page == 0) {
      page = this->LoadBookmark(this->m_bookName);
    }
    this->m_page = page;
    if (page > this->m_pageCount) {
      this->m_page = 0;
    }

    this->m_viewer->Clear();
    for (int index = 0; index < 19; ++index) {
      const int line = this->m_page * 19 - 19 + index;
      if (line >= this->m_lineCount) {
        break;
      }

      if (g_inlineCharset) {
        char* text = new char[this->m_lines[line].size() + 17];
        std::sprintf(text, "%s%s", g_serverLanguage->Prefix(true), this->m_lines[line].c_str());
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text), 100, 0, 0);
      } else {
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->m_lines[line].c_str()), 100, 0, 0);
      }
    }
    this->Invalidate();
    this->m_book->ordering.Promote(this);
    break;
  }

  case 93:
    this->m_autoRead = this->m_autoRead == 0;
    this->Show(0);
    if (this->m_autoRead) {
      this->m_autoTick = timeGetTime() - 4000;
      int page = static_cast<int>(a);
      if (page == 0) {
        page = this->LoadBookmark(this->m_bookName);
      }
      this->m_page = page;
      if (page > this->m_pageCount) {
        this->m_page = 0;
        page = 0;
      }
      this->m_autoLine = page * 19 - 19;
      if (this->m_autoLine > this->m_lineCount) {
        this->m_autoLine = 0;
      }
      break;
    }

  case 114:
    if (this->unknown_00B8) {
      g_windowMgr.Schedule(this->unknown_00B8);
      this->unknown_00B8 = nullptr;
    }
    this->m_frameContext->windows.CloseWindow(106);
    break;

  case 94: {
    const char* name = reinterpret_cast<const char*>(a);
    const char* title = FindItemDescription(std::atoi(name)).identifiedName;
    std::intptr_t page;
    if (std::strcmp(this->m_bookName, title) == 0) {
      if (this->m_autoRead == 0 && b == 287) {
        break;
      }
      page = this->m_page;
    } else {
      page = c;
    }
    std::strcpy(this->m_bookName, title);

    char path[260];
    if (g_serviceType == 10) {
      std::sprintf(path, "%sbook\\%s.txt", g_serverLanguage->servicePrefix.c_str(), name);
    } else {
      std::sprintf(path, "book\\%s.txt", name);
    }

    CFile file;
    const char* fileName = path;
    if (g_serverLanguage) {
      fileName = g_serverLanguage->ServiceName(fileName);
    }
    if (file.Open(fileName, 0, true, false)) {
      const std::uint32_t size = file.GetSize();
      char* text = new char[size + 1];
      file.Read(text, size);
      text[size] = '\0';
      file.Close();

      this->m_lines.clear();
      WrapPixelText(*this->geometry.surface, this->m_book->text.list.codePage, text, this->m_lines, this->m_viewer->geometry.width - 10);
      this->m_lineCount = static_cast<int>(this->m_lines.size());
      if (text[0] == '%') {
        this->m_red = g_hexDigits[text[1]] * 16 + g_hexDigits[text[2]];
        this->m_green = g_hexDigits[text[3]] * 16 + g_hexDigits[text[4]];
        this->m_blue = g_hexDigits[text[5]] * 16 + g_hexDigits[text[6]];
        if (this->m_lineCount > 0) {
          --this->m_lineCount;
          this->m_lines.erase(this->m_lines.begin());
        }
      } else {
        SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, this->m_red, this->m_green, this->m_blue);
      }

      this->m_pageCount = this->m_lineCount / 19;
      if (this->m_lineCount % 19 > 0) {
        ++this->m_pageCount;
      }
      this->SendMsg(this, b == 287 ? 92 : 93, page, 0, 0, 0);
    }
    break;
  }
  }
  return 0;
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

// OFFSET: 0x008D9AD0
// FUNCTION: fn_12135
int UIListBox::LineCount() const {
  return static_cast<int>(this->lines.size());
}

// OFFSET: 0x008DA450
// FUNCTION: fn_12146
bool UInCashWnd::Slot03() {
  return true;
}

// OFFSET: 0x008DB750
// FUNCTION: fn_12159
void UIBookWnd::Slot15(int width, int height) {
  this->m_viewer = new UITextViewer(*this->m_context, this->m_book->text);
  this->m_viewer->Configure(30, 45, width - 60, height - 60, 0);
  this->AddChild(this->m_viewer);

  const std::string prefix = "book\\";
  const char* names[4] = {"책갈피", "책왼쪽", "책오른쪽", "책닫기"};
  const int ids[4] = {356, 354, 355, 201};
  const struct {
    int x;
    int y;
  } positions[4] = {{10, 25}, {width - 57, 25}, {width - 35, 25}, {width - 25, height - 25}};
  const char* tooltips[4] = {LookupMessage(1296), LookupMessage(1297), LookupMessage(1298), LookupMessage(1299)};

  for (int index = 0; index < 4; ++index) {
    this->m_buttons[index] = new UISpriteButton(*this->m_context, this->m_book->text.buttons);
    this->m_buttons[index]->SetSprite((prefix + names[index] + ".spr").c_str());
    this->m_buttons[index]->SetActName((prefix + names[index] + ".act").c_str());
    this->m_buttons[index]->SetSize(this->m_buttons[index]->FrameWidth(0, 0), this->m_buttons[index]->FrameHeight(0, 0));
    this->m_buttons[index]->Move(positions[index].x, positions[index].y);
    this->m_buttons[index]->SetId(ids[index]);
    this->m_buttons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->m_buttons[index]);
  }
}

// OFFSET: 0x008DBD00
// FUNCTION: fn_12160
void UICardItemIllustWnd::Slot15(int, int) {
  this->unknown_00E4 = 0;
  this->m_name = LookupMessage(527);
  this->AddCloseButton(*this->m_controls, "", 0);
}

// OFFSET: 0x008E3090
// FUNCTION: fn_12179
void UIItemParamChangeDisplayWnd::Slot15(int, int) {
  this->SendMsg(nullptr, 60, 0, 0, 0, 0);
}

// OFFSET: 0x008E3330
// FUNCTION: fn_12181
void UIMenuWnd::Slot15(int, int) {
}

// OFFSET: 0x0090AB10

// OFFSET: 0x0090AED0
// FUNCTION: fn_12418
int ScaleByScreenHeight(int value) {
  return g_renderer->viewport.height * value / 480;
}

// OFFSET: 0x0090B080
// FUNCTION: fn_12419
void UIWhisperWnd::SaveChatLog() {
  char path[2048];
  int index = 0;
  std::sprintf(path, "%s\\Whisper", g_applicationDirectory);
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  std::string name(this->m_target);
  for (std::size_t i = 0; i < name.size(); ++i) {
    const char c = name[i];
    if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
      name[i] = '_';
    }
  }

  std::sprintf(path, "%s\\Whisper\\%s.txt", g_applicationDirectory, name.c_str());
  while (_access(path, 0) == 0) {
    ++index;
    std::sprintf(path, "%s\\Whisper\\%s%03d.txt", g_applicationDirectory, name.c_str(), index);
  }

  if (this->m_chatHistory->SaveChatLog(path)) {
    std::string message(path);
    message += " is Saved.";
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF00, 0, 0, 0);
  }
}

struct WhisperWndEx {
  unsigned char unknown_0000[0x110];
  unsigned char unknown_0110[10];
  unsigned char unknown_011A[10];
  UIWindow* unknown_0124[11];
  UIWindow* unknown_0150[5];

  int sub_90C350(unsigned int target);
  void sub_90C380();
};

// OFFSET: 0x0090C350
// FUNCTION: fn_12443
int WhisperWndEx::sub_90C350(unsigned int target) {
  for (int i = 0; i < 10; ++i) {
    if (this->unknown_0110[i] == target) {
      return i;
    }
  }
  return -1;
}

// OFFSET: 0x0090C380
// FUNCTION: fn_12444
void WhisperWndEx::sub_90C380() {
  for (int i = 0; i < 11; ++i) {
    UIWindow* window = this->unknown_0124[i];
    *reinterpret_cast<unsigned char*>(reinterpret_cast<char*>(window) + 0xac) = 1;
    window->Invalidate();
  }
  for (int i = 0; i < 5; ++i) {
    UIWindow* window = this->unknown_0150[i];
    *reinterpret_cast<unsigned char*>(reinterpret_cast<char*>(window) + 0xac) = 1;
    window->Invalidate();
  }
}

// OFFSET: 0x0090C8C0
// FUNCTION: fn_12450
std::vector<char> MakeFramedMessage(std::uint16_t type, const std::vector<char>& body) {
  std::vector<char> message(body.size() + sizeof(FramedMessageHeader), 0);
  FramedMessageHeader* header = reinterpret_cast<FramedMessageHeader*>(message.data());
  header->type = type;
  header->length = static_cast<std::uint32_t>(body.size());
  if (body.size() != 0) {
    std::memcpy(message.data() + sizeof(FramedMessageHeader), body.data(), body.size());
  }
  return message;
}

// OFFSET: 0x00901370
// FUNCTION: fn_12373
int UIQuitWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 220:
    mode->SendMsg(25, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(48);
    break;
  case 221:
    mode->SendMsg(88, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(48);
    break;
  case 185:
    g_windowMgr.CloseWindow(48);
    break;
  }

  return 0;
}

// OFFSET: 0x009013F0
// FUNCTION: fn_12374
int UIRestartWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 252:
    mode->SendMsg(25, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(19);
    g_windowMgr.CloseWindow(269);
    break;
  case 220:
    mode->SendMsg(25, 0, 0, 0, 0);
    [[fallthrough]];
  case 185:
    g_windowMgr.CloseWindow(19);
    break;
  case 251:
    g_windowMgr.CloseWindow(19);
    mode->SendMsg(88, 0, 0, 0, 0);
    break;
  }

  return 0;
}

// OFFSET: 0x00909830
// FUNCTION: fn_12392
int UInCash_CallWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  if (a == 192) {
    if (g_session.unknown_5912) {
      g_modeMgr.GetGameMode()->SendMsg(331, 2, 0, 0, 0);
      return 0;
    }
    g_modeMgr.GetGameMode()->SendMsg(323, 0, 0, 0, 0);
  }
  return 0;
}

// OFFSET: 0x009098A0
// FUNCTION: fn_12393
void UIEquipWnd::UpdateCharacterView(int reset) {
  CMode* mode = g_modeMgr.gameMode;
  if (!reset && g_session.unknown_5C1C > 0) {
    mode->SendMsg(80, 6, 0, 0, 0);
    return;
  }

  int view = 0;
  if (StateFlag04(g_session.m_jobFlags)) {
    view = 5;
  } else if (StateFlag05(g_session.m_jobFlags)) {
    view = 4;
  } else if (StateFlag22(g_session.m_jobFlags)) {
    view = 3;
  } else if (StateFlag19(g_session.m_jobFlags) || StateFlag23(g_session.m_jobFlags) || StateFlag24(g_session.m_jobFlags) ||
             StateFlag25(g_session.m_jobFlags) || StateFlag26(g_session.m_jobFlags)) {
    view = 1;
  }
  mode->SendMsg(80, view, 0, 0, 0);
}

// OFFSET: 0x009099C0
// FUNCTION: fn_12394
void UIComboBoxWnd::SetOwner(UIWindow* owner) {
  if (owner) {
    this->m_owner = owner;
  }
}

// OFFSET: 0x00909C80
// FUNCTION: fn_12396
void __stdcall Nullsub314(int, int, int) {
}

// OFFSET: 0x00909C90
// FUNCTION: fn_12397
void UIWaitWnd::SetMessage(const char* text, int, int) {
  this->m_textViewer->Clear();
  this->m_textViewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text), 38, 0, 0);
}

// OFFSET: 0x00909CE0
// FUNCTION: fn_12399
bool UINotifyItemObtainWnd::CanHitTest() {
  return false;
}

// OFFSET: 0x00909D20
// FUNCTION: fn_12401
void UITipboxWnd::TrimAt(std::string& text, const char* marker) {
  const std::size_t position = text.find(marker);
  if (position != std::string::npos) {
    text = text.substr(0, position);
  }
}

// OFFSET: 0x00909DF0
// FUNCTION: fn_12402
void UIChooseWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
  }
}

// OFFSET: 0x00909E30
// FUNCTION: fn_12403
void UICombinedCardItemCollectionWnd::Slot11() {
  if (this->m_cardPlacement) {
    this->m_cardPlacement->x = this->geometry.x;
    this->m_cardPlacement->y = this->geometry.y;
    this->m_cardPlacement->width = this->geometry.width;
    this->m_cardPlacement->height = this->geometry.height;
  }
}

// OFFSET: 0x00909E70
// FUNCTION: fn_12404
void UIEquipWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->show = this->m_show;
  }
}

// OFFSET: 0x00909F00
// FUNCTION: fn_12406
void UIItemCollectionWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x00909F80
// FUNCTION: fn_12408
void UIItemIdentifyWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
  }
}

// OFFSET: 0x00909FC0
// FUNCTION: fn_12409
void UIItemParamChangeDisplayWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x0090A000
// FUNCTION: fn_12410
void UIPartySettingWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x0090A040
// FUNCTION: fn_12411
void UISayDialogWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
  }
}

// OFFSET: 0x0090A180
// FUNCTION: fn_12414
void UInCashWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
  }
}

// OFFSET: 0x008FB850
// FUNCTION: fn_12345
int UIGraffiStrboxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 184) {
      struct {
        int area[4];
        const char* text;
      } request;
      request.area[0] = this->area[0];
      request.area[1] = this->area[1];
      request.area[2] = this->area[2];
      request.area[3] = this->area[3];
      request.text = this->edit->Text();
      g_modeMgr.GetGameMode()->SendMsg(131, reinterpret_cast<int>(&request), 0, 0, 0);
    } else if (a != 185) {
      return 0;
    }

    g_windowMgr.Schedule(this);
    return 0;
  }

  if (message == 68) {
    const int* area = reinterpret_cast<const int*>(a);
    this->area[0] = area[0];
    this->area[1] = area[1];
    this->area[2] = area[2];
    this->area[3] = area[3];
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008E4380
// FUNCTION: fn_12185
void UINotifyItemObtainWnd::Slot15(int, int) {
}

// OFFSET: 0x008E7120
// FUNCTION: fn_12193
void UIPetTamingDeceiveWnd::Slot15(int, int) {
}

// OFFSET: 0x008D3230
// FUNCTION: fn_12100
void UITipboxWnd::UpdateAnimations() {
  for (unsigned int i = 0; i < this->m_animations.size(); ++i) {
    if (this->m_animations[i].blink != 0 && this->m_frameTime - this->m_animations[i].blinkStart >= 500) {
      this->m_animations[i].visible = true;
      if (this->m_frameTime - this->m_animations[i].blinkStart >= 1000) {
        this->m_animations[i].visible = false;
        this->m_animations[i].blinkStart = this->m_frameTime;
      }
    }
  }

  for (unsigned int i = 0; i < this->m_animations.size(); ++i) {
    if (this->m_animations[i].fromX != this->m_animations[i].toX || this->m_animations[i].fromY != this->m_animations[i].toY) {
      const std::uint32_t frameTime = this->m_frameTime;
      const std::uint32_t elapsed = timeGetTime() - frameTime;
      const std::uint32_t running = frameTime - this->m_animations[i].moveStart;
      TipBoxAnimation& animation = this->m_animations[i];
      const float duration = animation.duration;
      const float step = static_cast<float>(elapsed) * 0.001f;
      if (static_cast<float>(running) >= duration * 1000.0f) {
        animation.x = static_cast<float>(animation.fromX);
        this->m_animations[i].y = static_cast<float>(this->m_animations[i].fromY);
        this->m_animations[i].moveStart = this->m_frameTime;
      } else {
        animation.x += static_cast<float>(animation.toX - animation.fromX) * step / duration;
        animation.y += static_cast<float>(animation.toY - animation.fromY) * step / duration;
      }
    }
  }
}

// OFFSET: 0x008D3430
// FUNCTION: fn_12101
bool UITipboxWnd::BuildAnimations() {
  CTipBoxMgr* manager = g_itemTagOwner;
  this->m_animations.clear();
  this->m_dots.clear();
  if (manager->m_currentEntry.images.empty()) {
    return false;
  }

  for (unsigned int i = 0; i < manager->m_currentEntry.images.size(); ++i) {
    if (manager->m_currentEntry.images[i].unknown_0000 == 6) {
      const TipBoxImage& image = manager->m_currentEntry.images[i];
      const float dx = static_cast<float>(image.unknown_0010 - image.unknown_0008);
      const float dy = static_cast<float>(image.unknown_0014 - image.unknown_000C);
      int count = 5;
      if (image.unknown_0018 != 0.0f) {
        count = static_cast<int>(image.unknown_0018);
      }

      for (int step = 0; step < count; ++step) {
        TipBoxDot dot;
        dot.x = static_cast<float>(step) * dx / static_cast<float>(count) + static_cast<float>(manager->m_currentEntry.images[i].unknown_0008);
        dot.y = static_cast<float>(step) * dy / static_cast<float>(count) + static_cast<float>(manager->m_currentEntry.images[i].unknown_000C);
        dot.visible = false;
        this->m_dots.push_back(dot);
      }
    } else {
      TipBoxAnimation animation;
      animation.kind = manager->m_currentEntry.images[i].unknown_0000;
      animation.x = static_cast<float>(manager->m_currentEntry.images[i].unknown_0008);
      animation.y = static_cast<float>(manager->m_currentEntry.images[i].unknown_000C);
      animation.moveStart = timeGetTime();
      animation.blink = manager->m_currentEntry.images[i].unknown_0004;
      animation.blinkStart = timeGetTime();
      animation.fromX = manager->m_currentEntry.images[i].unknown_0008;
      animation.fromY = manager->m_currentEntry.images[i].unknown_000C;
      animation.toX = manager->m_currentEntry.images[i].unknown_0010;
      animation.toY = manager->m_currentEntry.images[i].unknown_0014;
      animation.duration = manager->m_currentEntry.images[i].unknown_0018;
      animation.visible = true;
      this->m_animations.push_back(animation);
    }
  }

  this->m_buildTime = timeGetTime();
  return true;
}

// OFFSET: 0x008D36C0
// FUNCTION: fn_12102
void UIStatusWnd::ValidatePlacement() {
  if (!this->m_info->IsValid()) {
    this->m_info->Reset();
    this->m_expandedHeight = this->m_info->unknown_0010;
    if (this->geometry.height != 17) {
      this->Resize(this->geometry.width, this->m_expandedHeight);
    }
  }

  this->SendMsg(nullptr, 35, 0, 0, 0, 0);
}

// OFFSET: 0x008FEAA0
// FUNCTION: fn_12354
int UILoadingWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 17) {
    char text[100];
    std::sprintf(text, "Loading %d %s", static_cast<int>(argument1), "%");
    this->m_progressBar->SetValue(static_cast<float>(argument1));
  }
  return 0;
}

// OFFSET: 0x008CED00
// FUNCTION: fn_12071
int UIItemTagCtrl::MeasureText(const std::string& text, const unsigned& bytes) {
  return this->TextWidth(this->m_context->fonts, this->m_context->locale, text.c_str(), bytes, 0, 12, 0, 0);
}

// OFFSET: 0x008E8390
// FUNCTION: fn_12197
void UISayDialogWnd::Slot15(int width, int height) {
  auto* viewer = new UITagTextViewer(*this->m_context);
  this->m_viewer = viewer;
  viewer->SetSize(width - 20, height - 40);
  viewer->m_backgroundColor = 0xFFF2F2F2;
  viewer->Invalidate();
  viewer->m_textWidth = width - 65;
  viewer->Move(10, 10);
  this->AddChild(viewer);
  this->m_message0 = 600;
  this->unknown_00CC = 0;
  this->unknown_00D4 = 0;
}

// OFFSET: 0x008FF810
// FUNCTION: fn_12359
int UIMonsterDisplayWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 201) {
      g_windowMgr.CloseWindow(201);
    }
    return 0;
  }

  if (message == 62) {
    this->m_name = reinterpret_cast<const char*>(argument1);
    this->Invalidate();
    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008FFBA0
// FUNCTION: fn_12362
int UINotifyJobLevelUpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }
  if (a == 192) {
    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(g_windowMgr.EnsureWindow(37));
  }
  return 0;
}

// OFFSET: 0x008E0A20
// FUNCTION: fn_12173
void UIExchangeWnd::Slot15(int, int) {
  this->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  this->unknown_00CC = 0;
  this->unknown_00D0 = 0;
  this->unknown_00D4 = 0;
  this->unknown_00D8 = 0;
  this->unknown_00DC = 0;

  const std::string path("유저인터페이스\\btn_");
  const char* const names[3] = {"ok", "exchange", "cancel"};
  const int ids[3] = {0xB8, 0xE6, 0xB9};
  const int x[3] = {3, 260, 510};
  const int y[3] = {
      this->unknown_00CC * 120, this->unknown_00CC != 0 && this->unknown_00D0 != 0 && this->unknown_00D8 == 0 ? 0 : 120, 0
  };

  for (int index = 0; index < 3; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
    this->m_buttons[index] = button;
    button->SetBitmap((path + names[index] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((path + names[index] + "_a.bmp").c_str(), 1, 0);
    button->SetBitmap((path + names[index] + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(x[index], this->geometry.height - 23 + y[index]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  UIEditCtrl* edit = new UIEditCtrl();
  this->unknown_00C0 = edit;
  edit->SetSize(0x58, 0x10);
  edit->Move(0x91, 0x14A);
  edit->maximum = 0xD;
  edit->Background(0xE8, 0xE8, 0xE8);
  edit->SetText("0");
  this->AddChild(edit);
  InputFocus{g_windowMgr.hoverWindow}.Set(edit);
  edit->SelectAll();

  UICheckBox* checkBox = new UICheckBox();
  this->unknown_0118 = checkBox;
  checkBox->SetSize(0x30, 0xC);
  checkBox->SetBitmaps("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp");
  checkBox->Move(0x69, this->geometry.height - 0x13);
  checkBox->SetChecked(this->unknown_011C);
  this->AddChild(checkBox);

  this->m_message0 = 600;
}

// OFFSET: 0x008DE400
// FUNCTION: fn_12167
void UIComboBoxIDWnd::Slot15(int width, int height) {
  auto* list = new UIComboBoxID();
  this->m_list = list;
  list->Configure(0, 0, width, height, 1);
  this->AddChild(list);
}

// OFFSET: 0x008D9F20
// FUNCTION: fn_12139
bool IsEquipViewHidden() {
  if (g_session.unknown_5C1C > 0) {
    return false;
  }

  const std::uint32_t flags = g_session.m_jobFlags;
  if (StateFlag04(flags) || StateFlag05(flags) || StateFlag19(flags) || StateFlag23(flags) || StateFlag24(flags) || StateFlag25(flags) ||
      StateFlag26(flags) || StateFlag22(flags)) {
    return false;
  }

  return true;
}

// OFFSET: 0x008ED080

// OFFSET: 0x008F64B0
// FUNCTION: fn_12317
void UIItemIdentifyWnd::UpdateScroll() {
  if (this->m_scrollBar == nullptr) {
    return;
  }

  auto* bar = static_cast<UIScrollBar*>(this->m_scrollBar);

  const int count = static_cast<int>(this->m_items.size());
  int total = count / this->m_columns;
  if (count % this->m_columns != 0) {
    ++total;
  }

  int maximum = total - this->m_rows;
  if (maximum < 0) {
    maximum = 0;
  }
  this->m_scrollable = maximum != 0;

  if (maximum == 0) {
    this->m_scroll = 0;
    bar->SetRange(0, this->m_rows, total);
  } else {
    bar->SetRange(maximum, this->m_rows, total);
    if (this->m_scroll < 0) {
      this->m_scroll = 0;
    }
    if (this->m_scroll > total - this->m_rows) {
      this->m_scroll = total - this->m_rows;
    }
    bar->SetPosition(this->m_scroll);
  }

  bar->Invalidate();
}

// OFFSET: 0x00909CD0
// FUNCTION: fn_12398
void UIPasswordWnd::Show(int visible) {
  this->geometry.Show(visible);
}

// OFFSET: 0x00907610
// FUNCTION: fn_12387
int UIToolTipWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == 184 || argument1 == 185) {
    g_windowMgr.Schedule(this);
  }

  return 0;
}

// OFFSET: 0x00900D50
// FUNCTION: fn_12371
int UIPetTamingDeceiveWnd::SendMsg(
  UIWindow*, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == 80) {
    if (argument1 == 0) {
      this->m_target = static_cast<int>(argument2);
      return 0;
    }

    this->m_frame = static_cast<int>(argument2);
    this->m_delay = 7;
    this->m_maxDelay = 7;
  }

  return 0;
}

// OFFSET: 0x0090A080
// FUNCTION: fn_12412
void UIStatusWnd::Slot11() {
  if (this->m_info == nullptr) {
    return;
  }

  this->m_info->width = this->geometry.width;
  this->m_info->height = this->geometry.height;
  this->m_info->x = this->geometry.x;
  this->m_info->y = this->geometry.y;
  this->m_info->unknown_0010 = this->m_expandedHeight;
  this->m_info->unknown_0018 = this->m_extended == 1;
}

// OFFSET: 0x00907650
// FUNCTION: fn_12388
int UIWaitWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 185) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(10021, 0, 0, 0, 0);
    }
  } else if (message == 88) {
    this->m_button->Move((this->geometry.width - this->m_button->BitmapWidth()) / 2, this->geometry.height - this->m_button->BitmapHeight() - 5);
  }

  return 0;
}

// OFFSET: 0x008FF8A0
// FUNCTION: fn_12360
int UINoticeConfirmWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == 6) {
    if (argument1 == 184) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(this->m_event, 0, 0, 0, 0);
    } else if (argument1 == 185) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
    }

    return 0;
  }

  if (message == 80) {
    this->m_event = static_cast<int>(argument1);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0090BF40
// FUNCTION: fn_12435
int UIChangeNameWnd::IsNameWithinLimit(std::string name, int limit) {
  _setmbcp(949);
  int width = 0;
  for (std::size_t index = 0; index < name.size(); ++index) {
    switch (_mbsbtype(reinterpret_cast<const unsigned char*>(name.c_str()), index)) {
    case _MBC_SINGLE:
      ++width;
      break;
    case _MBC_TRAIL:
      width += 2;
      break;
    }
  }
  return width < limit;
}

// OFFSET: 0x008F6840
// FUNCTION: fn_12323
void UIItemCollectionWnd::SetHeight(int height) {
  const int minimum = (this->m_compare == 1 ? 175 : 120) + this->m_extraHeight;
  if (height < minimum) {
    height = minimum;
  }
  if (height > 450) {
    height = 450;
  }
  if (height == this->geometry.height) {
    return;
  }

  const int moveInfoId = FindItemMoveInfo(std::atoi(this->m_itemIdText.c_str())).itemId;
  if (moveInfoId != 0) {
    this->Resize(this->geometry.width, height + this->m_moveInfo->geometry.height + 3);
    this->m_moveInfo->m_moveIndex = std::atoi(this->m_itemIdText.c_str());
  } else {
    this->Resize(this->geometry.width, height);
  }

  int resizerY;
  if (this->m_compare != 0) {
    this->m_viewer->Resize(this->geometry.width - 98, height - this->m_extraHeight - 69);
    resizerY = this->geometry.height - this->m_extraHeight - 46;
  } else {
    this->m_viewer->Resize(this->geometry.width - 98, height - this->m_extraHeight - 40);
    resizerY = this->geometry.height - this->m_extraHeight - 17;
  }
  this->m_resizer->Move(this->geometry.width - 17, resizerY);

  if (moveInfoId != 0) {
    const int bottom = this->m_viewer->geometry.y + this->m_viewer->geometry.height;
    this->m_moveInfo->Move(this->m_viewer->geometry.x, this->m_compare != 0 ? bottom - 29 : bottom);
  } else {
    this->m_moveInfo->Move(-1000, -1000);
  }
  this->m_viewer->SendMsg(nullptr, 7, 0, 0, 0, 0);
}

class UIAutoMessageWnd : public UIFrameWnd {
public:
  UIAutoMessageWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
};

// OFFSET: 0x008ECE20
// FUNCTION: fn_12210
void UIAutoMessageWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 15, 6, LookupMessage(0x3B0), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008FFC40
// FUNCTION: fn_12364
int UINotifyQuestWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }
  if (a == 192) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(0x2718)) {
      reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(window);
    }
  }
  return 0;
}

// OFFSET: 0x008F5AA0
// FUNCTION: fn_12312
void UInCashWnd::Slot35(int delta) {
  if (static_cast<int>(g_session.unknown_1720.size()) < 4) {
    return;
  }
  if (delta < 0) {
    this->SendMsg(this, 10, 0, 0, 0, 0);
  } else {
    this->SendMsg(this, 9, 0, 0, 0, 0);
  }
}

// OFFSET: 0x008D7660

// OFFSET: 0x008FFBE0
// FUNCTION: fn_12363
int UINotifyLevelUpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }
  if (a == 192) {
    UIWindow* window = g_windowMgr.EnsureWindow(11);
    if (window->geometry.height == 17) {
      window->SendMsg(nullptr, 6, 0xCA, 0, 0, 0);
    }
    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(window);
  }
  return 0;
}

// OFFSET: 0x008D84A0
// FUNCTION: fn_12126
ITEM_INFO UIEquipWnd::GetSlotItem(int slot) {
  if (slot == -1) {
    return ITEM_INFO(0);
  }

  if (this->m_page == 1) {
    if (this->m_viewOther == 0) {
      return g_session.GetItem2B50(slot);
    }
    return g_session.GetItem3500(slot);
  }

  if (this->m_page != 0) {
    return ITEM_INFO(0);
  }

  if (this->m_viewOther == 0) {
    return g_session.GetItem17F0(slot);
  }
  return g_session.GetItem21A0(slot);
}

// OFFSET: 0x008D8580
// FUNCTION: fn_12127
__declspec(noinline) ITEM_INFO UIEquipWnd::ItemIconAt(int x, int y) {
  int slot = -1;
  const int top = this->m_contentTop;
  if (y < top + 17 || y > top + 145) {
    return ITEM_INFO(0);
  }

  if ((x >= 6 && x < 30) || (x >= 251 && x < 275)) {
    slot = (y - top - 17) / 26;
    if (x / 251 != 0) {
      slot = slot == 0 ? 9 : slot + 3;
    } else {
      slot = slot == 0 ? 8 : slot - 1;
    }
  }

  const int center = this->geometry.width / 2;
  if (center - 25 < x && x < center - 1 && top + 44 < y && y < top + 68) {
    slot = 15;
  }
  return this->GetSlotItem(slot);
}

// OFFSET: 0x008D8D90
// FUNCTION: fn_12131
ITEM_INFO UIEquipWnd::ItemEntryAt(int x, int y) {
  int slot = -1;
  const int top = this->m_contentTop;
  if (y < top + 17 || y > top + 145) {
    return ITEM_INFO(0);
  }

  if ((x >= 6 && x < 106) || (x >= 170 && x < this->geometry.width - 6)) {
    slot = (y - top - 17) / 26;
    if (x / 170 != 0) {
      slot = slot == 0 ? 9 : slot + 3;
    } else {
      slot = slot == 0 ? 8 : slot - 1;
    }
  }

  const int center = this->geometry.width / 2;
  if (center - 25 < x && x < center - 1 && top + 44 < y && y < top + 68) {
    slot = 15;
  }
  return this->GetSlotItem(slot);
}

// OFFSET: 0x008F31C0
// FUNCTION: fn_12263
void UIEquipWnd::Slot26(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, 6, 202, 0, 0, 0);
  }

  if (this->m_viewOther != 0) {
    return;
  }

  ITEM_INFO item = this->ItemIconAt(x, y);
  if (item.unknown_0000[1] != 0 && item.quantity > 0) {
    const EquipSlots slots = g_session.GetEquipSlots(static_cast<int>(item.unknown_0000[3]));
    g_modeMgr.gameMode->SendMsg(20, slots.values[2], this->m_page == 1, 0, 0);
  }
}

// OFFSET: 0x008F3DD0
// FUNCTION: fn_12286
void UIEquipWnd::Slot28(int x, int y) {
  ITEM_INFO item = this->ItemEntryAt(x, y);
  if (item.unknown_0000[1] != 0) {
    item.identified = 1;
    int tooltipX = 6;
    int tooltipY = (y - this->m_contentTop - 17) / 24 * 26 - 3 + this->m_contentTop;
    if (x >= 170) {
      tooltipX = 175;
    } else if (x >= 106) {
      tooltipX = this->geometry.width / 2 - 20;
    }
    this->ToScreen(tooltipX, tooltipY);

    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
    if (item.quantity < 2) {
      if (item.identified == 1 && item.unknown_0098 != 0) {
        tooltip->Show(FormatOwnedString(LookupMessage(0xA28), item.DisplayName(0).c_str(), item.unknown_0098).c_str(), tooltipX, tooltipY,
                      g_session.GetItemNameColor(item), 0, 1);
      } else {
        tooltip->Show(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
      }
    } else if (item.identified == 1 && item.unknown_0098 != 0) {
      tooltip->Show(FormatOwnedString(LookupMessage(0x9F7), item.DisplayName(0).c_str(), item.unknown_0098, item.quantity).c_str(), tooltipX,
                    tooltipY, g_session.GetItemNameColor(item), 0, 1);
    } else {
      tooltip->Show(FormatOwnedString(LookupMessage(0xB7), item.DisplayName(0).c_str(), item.quantity).c_str(), tooltipX, tooltipY,
                    g_session.GetItemNameColor(item), 0, 1);
    }
  }

  const int previous0 = this->m_highlight[0];
  const int previous1 = this->m_highlight[1];
  const int previous2 = this->m_highlight[2];
  this->m_highlight[0] = -1;
  this->m_highlight[1] = -1;
  this->m_highlight[2] = -1;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode->drag.source == 1 && x >= 0 && y >= 0 && x < this->geometry.width && y < this->geometry.height) {
    ITEM_INFO dragged = LookupClientItemByIdentity(g_session.m_inventoryItems, mode->drag.id);
    if (dragged.unknown_0000[1] != 0 && dragged.unknown_0000[2] != 0) {
      const EquipSlots slots = g_session.GetEquipSlots(static_cast<int>(dragged.unknown_0000[2]));
      this->m_highlight[0] = slots.values[2];
      if (slots.values[1] != -1) {
        this->m_highlight[1] = slots.values[1];
      }
      if (slots.values[0] != -1) {
        this->m_highlight[2] = slots.values[0];
      }
    }
  }

  if (previous0 != this->m_highlight[0] || previous1 != this->m_highlight[1] || previous2 != this->m_highlight[2]) {
    this->Invalidate();
  }
}

// OFFSET: 0x008F5400
// FUNCTION: fn_12302
void UIEquipWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->ItemEntryAt(x, y);
  if (item.unknown_0000[1] != 0) {
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x008D86D0
// FUNCTION: fn_12128
ITEM_INFO UIExchangeWnd::ItemAt(int x, int y) {
  if (x < 4 || x >= this->geometry.width - 3 || y < 20 || y >= this->geometry.height - 40) {
    return ITEM_INFO(0);
  }

  const int row = (y - 17) / this->m_rowHeight;
  const std::list<ITEM_INFO>& items = x < 280 ? this->m_myItems : this->m_otherItems;
  auto it = items.begin();
  for (int index = 0; index < row; ++index) {
    if (it == items.end()) {
      return ITEM_INFO(0);
    }
    ++it;
  }

  if (it == items.end()) {
    return ITEM_INFO(0);
  }
  return *it;
}

// OFFSET: 0x008F4300
// FUNCTION: fn_12287
void UIExchangeWnd::Slot28(int x, int y) {
  int tooltipY = y;
  ITEM_INFO item = this->ItemAt(x, y);
  this->m_hoverX = static_cast<short>(x);
  if (item.unknown_0000[1] == 0) {
    return;
  }

  int tooltipX = x >= 280 ? 321 : 42;
  tooltipY = (tooltipY - 17) / this->m_rowHeight * this->m_rowHeight + 4;
  this->ToScreen(tooltipX, tooltipY);

  ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
  item.unknown_0000[0] = 21;
  if (item.identified == 1 && item.unknown_0098 != 0) {
    tooltip->Show(FormatOwnedString(LookupMessage(0xA28), item.DisplayName(0).c_str(), item.unknown_0098).c_str(), tooltipX, tooltipY,
                  g_session.GetItemNameColor(item), 0, 1);
  } else {
    tooltip->Show(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x008F54C0
// FUNCTION: fn_12303
void UIExchangeWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.unknown_0000[1] != 0) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(12)) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x008D87F0
// FUNCTION: fn_12129
__declspec(noinline) ITEM_INFO UIItemIdentifyWnd::GetItem(int index, int* found) {
  int position = 0;
  *found = 1;
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it, ++position) {
    if (index == position) {
      return *it;
    }
  }

  *found = 0;
  ITEM_INFO empty(0);
  return empty;
}

// OFFSET: 0x008D7F30
// FUNCTION: fn_12125
ITEM_INFO UIItemIdentifyWnd::GetSelectedItem(int* found) {
  return this->GetItem(this->m_selected, found);
}

// OFFSET: 0x0090A0E0
// FUNCTION: fn_12413
void UIWhisperWnd::Slot11() {
  if (this->unknown_0100) {
    this->unknown_0100->width = this->geometry.width;
    this->unknown_0100->height = this->geometry.height;
    this->unknown_0100->x = this->geometry.x;
    this->unknown_0100->y = this->geometry.y;
  }
  g_windowMgr.UnregisterNamedWindow(std::string(this->m_target.c_str(), this->m_target.size()));
}

struct SessionMessengerContext {
  int GetList1788(int index) const;
  int GetList2Size() const;
};

// OFFSET: 0x008D88F0
// FUNCTION: fn_12130
ITEM_INFO UInCashWnd::ItemAt(int x, int y) {
  if (static_cast<unsigned>(x - 64) > 556 || y < 50 || y > this->m_listHeight + 440) {
    return ITEM_INFO(0);
  }

  const SessionMessengerContext& lists = reinterpret_cast<const SessionMessengerContext&>(g_session);
  int top = this->m_listTop;
  if (x < 252) {
    std::list<ITEM_INFO>::iterator it = this->m_items.begin();
    for (int row = 0; row < 4; ++row) {
      if (y >= top && y <= top + this->m_rowHeight) {
        int index = row + (this->m_page - 1) * 8;
        int count = lists.GetList1788(this->m_tab);
        if (this->m_tab == 8) {
          count = static_cast<int>(this->m_items.size());
        }
        if (count <= index) {
          return ITEM_INFO(0);
        }
        this->m_selected = row;
        for (; index > 0; --index) {
          ++it;
        }
        break;
      }
      top += this->m_rowHeight;
    }
    return it != this->m_items.end() ? *it : ITEM_INFO(0);
  }

  if (x < 444) {
    std::list<ITEM_INFO>::iterator it = this->m_items.begin();
    for (int row = 0; row < 4; ++row) {
      if (y >= top && y <= top + this->m_rowHeight) {
        int index = this->m_page * 8 - 4 + row;
        int count = lists.GetList1788(this->m_tab);
        if (this->m_tab == 8) {
          count = static_cast<int>(this->m_items.size());
        }
        if (count <= index) {
          return ITEM_INFO(0);
        }
        this->m_selected = row + 4;
        for (; index > 0; --index) {
          ++it;
        }
        break;
      }
      top += this->m_rowHeight;
    }
    return it != this->m_items.end() ? *it : ITEM_INFO(0);
  }

  int index = 0;
  int row = 0;
  for (; row < 4; ++row) {
    if (y >= top && y <= top + this->m_rowHeight) {
      index = this->m_firstOther + row;
      this->m_selected = index;
      if (lists.GetList2Size() <= index) {
        return ITEM_INFO(0);
      }
      break;
    }
    top += this->m_rowHeight;
  }
  if (row == 4) {
    index = this->m_selected;
  }

  std::list<ITEM_INFO>::iterator it = this->m_otherItems.begin();
  for (int remaining = index; remaining > 0; --remaining) {
    ++it;
  }
  this->m_selected = ~index;
  return it != this->m_otherItems.end() ? *it : ITEM_INFO(0);
}

// OFFSET: 0x008F3A80
// FUNCTION: fn_12279
void UInCashWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
  }

  ITEM_INFO item = this->ItemAt(x, y);
  if (item.unknown_0000[1] != 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ItemDescriptions descriptions{g_itemDescriptions};
    int source;
    if (this->m_selected < 0) {
      this->m_selected = ~this->m_selected;
      source = 4;
    } else {
      this->m_selectedIndex = item.unknown_0064;
      source = 6;
    }
    mode->drag.BeginItem(*reinterpret_cast<WindowManagerWindows*>(&g_windowMgr), descriptions, source, item, 1, -1);
  }
}

// OFFSET: 0x008F58C0
// FUNCTION: fn_12306
void UInCashWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.unknown_0000[1] != 0 && this->m_selected >= 0) {
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x008C6E20
// FUNCTION: fn_11923
UIAutoMessageWnd::UIAutoMessageWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008C6E80
// FUNCTION: fn_11924
UIBabyAcceptWnd2::UIBabyAcceptWnd2(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C6F10
// FUNCTION: fn_11925
UIBabyAcceptWnd::UIBabyAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C7630
// FUNCTION: fn_11935
UIComboBoxIDWnd::UIComboBoxIDWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_list(nullptr), m_owner(nullptr) {
}

// OFFSET: 0x008C76A0
// FUNCTION: fn_11936
UIComboBoxWnd::UIComboBoxWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_list(nullptr), m_owner(nullptr) {
}

// OFFSET: 0x008C77A0
// FUNCTION: fn_11938
UIDisconnectedServerMsgWnd::UIDisconnectedServerMsgWnd(UIWindowContext& windows, UIFrameContext& frames, UITextViewerContext& text)
    : UIFrameWnd(windows, frames, 0), m_viewer(nullptr), m_textContext(&text) {
}

// OFFSET: 0x008C82D0
// FUNCTION: fn_11951
UILoadingWnd::UILoadingWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_progressBar(nullptr) {
}

// OFFSET: 0x008C8650
// FUNCTION: fn_11957
UINoticeConfirmWnd::UINoticeConfirmWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_event(28) {
}

// OFFSET: 0x008C8770
// FUNCTION: fn_11959
UINotifyLevelUpWnd::UINotifyLevelUpWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008C87D0
// FUNCTION: fn_11960
UINotifyQuestWnd::UINotifyQuestWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008C8830
// FUNCTION: fn_11961
UINpcEditDialogWnd::UINpcEditDialogWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

// OFFSET: 0x008C8B50
// FUNCTION: fn_11968
UIProhibitListWnd::UIProhibitListWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_count(0), m_controls(&controls) {
}

// OFFSET: 0x008C8BB0
// FUNCTION: fn_11969
UIQuitWnd::UIQuitWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008C8C10
// FUNCTION: fn_11970
UIRestartWnd::UIRestartWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008C93D0
// FUNCTION: fn_11980
UITipOfTheDayWnd::UITipOfTheDayWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

// OFFSET: 0x008C9560
// FUNCTION: fn_11982
UIToolTipWnd::UIToolTipWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008C95C0
// FUNCTION: fn_11983
UIWaitWnd::UIWaitWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_button(nullptr) {
}

// OFFSET: 0x008C9990
// FUNCTION: fn_11987
UInCash_CallWnd::UInCash_CallWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
}

// OFFSET: 0x008C6FA0

// OFFSET: 0x008C7070

// OFFSET: 0x008C8390

// OFFSET: 0x008C85A0

// OFFSET: 0x008C8900

// OFFSET: 0x008C8980

// OFFSET: 0x008C8AB0

// OFFSET: 0x008C9060

// OFFSET: 0x008C91F0

// OFFSET: 0x008C7710
// FUNCTION: fn_11937
UICoupleAcceptWnd::UICoupleAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C8120
// FUNCTION: fn_11948
UIJoinFriendAcceptWnd::UIJoinFriendAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C81B0
// FUNCTION: fn_11949
UIJoinGuildAcceptWnd::UIJoinGuildAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C8510
// FUNCTION: fn_11955
UIMixAcceptWnd::UIMixAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C9160
// FUNCTION: fn_11977
UIStarPlaceAcceptWnd::UIStarPlaceAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C97F0
// FUNCTION: fn_11985
UIYourItemWnd::UIYourItemWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance)
    : UIExchangeAcceptWnd(windows, frames, acceptance) {
}

// OFFSET: 0x008C8C70
// FUNCTION: fn_11971
UISayDialogWnd::UISayDialogWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0) {
  this->m_viewer = nullptr;
  this->m_placement = nullptr;
}

// OFFSET: 0x008E3CD0
// FUNCTION: fn_12184
void UINoticeConfirmWnd::Slot15(int width, int height) {
  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{189, 96}, {234, 96}};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, this->m_textContext->buttons);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
  this->m_viewer = new UITextViewer(*this->m_context, *this->m_textContext);
  this->m_viewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(0)), 38, 0, 0);
}

// OFFSET: 0x008E4390
// FUNCTION: fn_12186
void UINotifyLevelUpWnd::Slot15(int, int) {
  int ids[3] = {192};
  int positions[3] = {0};
  const char* names[8] = {"lv_up"};
  const std::string prefix("유저인터페이스\\basic_interface\\");

  for (int i = 0; i < 1; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i] + "_off.bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i] + "_off.bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i] + "_on.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(0, positions[i]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }
}

// OFFSET: 0x008E4970
// FUNCTION: fn_12187
void UINotifyQuestWnd::Slot15(int, int) {
  int ids[3] = {192};
  int positions[3] = {0};
  const char* names[8] = {"quest"};
  const std::string prefix("유저인터페이스\\basic_interface\\");

  for (int i = 0; i < 1; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i] + "_off.bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i] + "_off.bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i] + "_on.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(0, positions[i]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }
}

// OFFSET: 0x008E4F50
// FUNCTION: fn_12188
void UINpcEditDialogWnd::Slot15(int, int) {
  this->edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->edit->SetSize(this->geometry.width - 77, 16);
  this->edit->Move(15, 22);
  this->edit->maximum = 12;
  this->edit->Background(232, 232, 232);
  this->edit->SetText("0");
  this->AddChild(this->edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->edit);
  this->edit->SelectAll();

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->unknown_00B8 = -1;
  this->m_message0 = 184;
}

// OFFSET: 0x008E54B0
// FUNCTION: fn_12189
void UINpcTextEditDialogWnd::Slot15(int, int) {
  this->edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->edit->SetSize(this->geometry.width - 77, 16);
  this->edit->Move(15, 22);
  this->edit->maximum = 70;
  this->edit->Background(232, 232, 232);
  this->edit->SetText("");
  this->AddChild(this->edit);
  this->edit->SetId(492);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->edit);
  this->edit->SelectAll();

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->unknown_00B8 = -1;
  this->m_message0 = 184;
}
// OFFSET: 0x008E7660
// FUNCTION: fn_12195
void UIQuitWnd::Slot15(int, int) {
  const std::string prefix("유저인터페이스\\");
  int positions[3][3] = {{29, 30, 0}, {29, 61, 0}, {29, 96, 0}};
  int ids[3] = {220, 221, 185};
  const char* names[3][3] = {{"esc_01a", "esc_01b", "esc_01c"}, {"esc_03a", "esc_03b", "esc_03c"}, {"esc_02a", "esc_02b", "esc_02c"}};

  for (int i = 0; i < 3; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 221;
}

// OFFSET: 0x008E7CB0
// FUNCTION: fn_12196
void UIRestartWnd::Slot15(int, int) {
  const std::string prefix("유저인터페이스\\");
  int ids[4] = {220, 252, 251, 185};
  const char* names[4][4] = {
      {"esc_04a", "esc_04b", "esc_04c", nullptr}, {"esc_01a", "esc_01b", "esc_01c", nullptr}, {"esc_03a", "esc_03b", "esc_03c", nullptr}, {"esc_02a", "esc_02b", "esc_02c", nullptr}};
  int positions[4][4] = {{29, 22, 0, 0}, {29, 45, 0, 0}, {29, 68, 0, 0}, {29, 96, 0, 0}};

  for (int i = 0; i < 4; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 252;
}

// OFFSET: 0x008EAE30
// FUNCTION: fn_12206
void UIWaitWnd::Slot15(int width, int height) {
  this->m_button = new UIBitmapButton(*this->m_context, this->m_textContext->buttons);
  this->m_button->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  this->m_button->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  this->m_button->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  this->m_button->SetSize(this->m_button->BitmapWidth(), this->m_button->BitmapHeight());
  this->m_button->Move((width - this->m_button->BitmapWidth()) / 2, 50);
  this->m_button->SetId(185);
  this->m_button->SetTooltip("");
  this->AddChild(this->m_button);

  this->m_textViewer = new UITextViewer(*this->m_context, *this->m_textContext);
  this->m_textViewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_textViewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->m_textViewer->Invalidate();
  this->AddChild(this->m_textViewer);
  this->m_textViewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(121)), 38, 0, 0);
}

namespace {
#pragma pack(push, 1)
  struct WearEquipPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t index;
    /* 0004 */ std::int32_t location;
  };
#pragma pack(pop)
}

struct SessionItemLists {
  void ClearTradeItems();
};

// OFFSET: 0x008E5A20
// FUNCTION: fn_12190
void UIPartyInvitationToWnd::Slot15(int, int) {
  this->m_edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edit->SetSize(120, 20);
  this->m_edit->Move(20, 42);
  this->m_edit->maximum = 23;
  this->m_edit->Background(230, 230, 230);
  this->AddChild(this->m_edit);

  {
    const std::string prefix("유저인터페이스\\");
    int positions[2][2] = {{this->geometry.width - 92, this->geometry.height - 24}, {this->geometry.width - 46, this->geometry.height - 24}};
    const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    int ids[2] = {184, 185};

    for (int i = 0; i < 2; ++i) {
      auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
      button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[i][0], positions[i][1]);
      button->SetId(ids[i]);
      this->AddChild(button);
    }
  }

  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);
}

// OFFSET: 0x008F95F0
// FUNCTION: fn_12339
int UICoupleAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    int accepted;
    if (a == 184) {
      this->m_frameContext->windows.CloseWindow(104);
      accepted = 1;
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(104);
      accepted = 0;
    } else {
      return 0;
    }
    this->m_context->mode->SendModeMessage(167, this->identity, accepted, 0, 0);
    return 0;
  }

  if (message != 90) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->identity = static_cast<std::uint32_t>(a);
  std::string text = "(";
  text += reinterpret_cast<const char*>(b);
  text += ")";
  text += LookupMessage(737);
  this->viewer->Clear();
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
  return 0;
}

// OFFSET: 0x008FDE10
// FUNCTION: fn_12350
int UIItemParamChangeDisplayWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 34) {
    this->m_placement = reinterpret_cast<WindowPlacement*>(a);
    auto* info = static_cast<ItemParamChangeDisplayWndInfo*>(this->m_placement);
    if (!info->IsValid()) {
      info->Reset();
    }
    this->Move(this->m_placement->x, this->m_placement->y);
    return 0;
  }

  if (message == 60) {
    this->SumEquippedStats();
    this->unknown_00CC = 0;
    this->unknown_00D0 = 0;
    const ITEM_DESC& description = FindItemDescription(std::atoi(this->m_label.c_str()));
    ITEM_INFO equipped = this->FindEquippedItem(description.unknown_002C);
    if (equipped.unknown_0000[1] != 0) {
      const ITEM_DESC& equippedDescription = FindItemDescription(std::atoi(equipped.itemId.c_str()));
      if (equippedDescription.unknown_0034.size() == 2) {
        this->unknown_00CC = equippedDescription.unknown_0034[0];
        this->unknown_00D0 = equippedDescription.unknown_0034[1];
      }
    }
  } else if (message == 64) {
    this->SumEquippedStats();
    this->m_label = reinterpret_cast<const char*>(a);
    this->unknown_00EC = static_cast<int>(b);
    const ITEM_DESC& description = FindItemDescription(std::atoi(this->m_label.c_str()));
    if (description.unknown_0034.size() == 2) {
      this->unknown_00C4 = description.unknown_0034[0];
      this->unknown_00C8 = description.unknown_0034[1];
    }
    this->unknown_00CC = 0;
    this->unknown_00D0 = 0;
    const ITEM_DESC& slot = FindItemDescription(std::atoi(this->m_label.c_str()));
    ITEM_INFO equipped = this->FindEquippedItem(slot.unknown_002C);
    if (equipped.unknown_0000[1] != 0) {
      const ITEM_DESC& equippedDescription = FindItemDescription(std::atoi(equipped.itemId.c_str()));
      if (equippedDescription.unknown_0034.size() == 2) {
        this->unknown_00CC = equippedDescription.unknown_0034[0];
        this->unknown_00D0 = equippedDescription.unknown_0034[1];
      }
    }
  } else {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->m_after = this->unknown_00C4 + this->m_before - this->unknown_00CC;
  this->m_afterSecond = this->unknown_00C8 + this->m_beforeSecond - this->unknown_00D0;
  this->Invalidate();
  return 0;
}

// OFFSET: 0x008FE0E0
// FUNCTION: fn_12351
int UIJoinFriendAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    this->m_acceptance->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);
    int accepted;
    if (a == 184) {
      this->m_frameContext->windows.CloseWindow(109);
      accepted = 1;
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(109);
      accepted = 0;
    } else {
      return 0;
    }
    this->m_context->mode->SendModeMessage(175, this->m_accountId, this->m_characterId, accepted, 0);
    return 0;
  }

  if (message == 95) {
    this->m_accountId = static_cast<int>(a);
    this->m_characterId = static_cast<int>(b);
    char text[128];
    std::sprintf(text, LookupMessage(818), reinterpret_cast<const char*>(c));
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text), 38, 0, 0);
    return 0;
  }

  if (message != 123) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  const auto* packet = reinterpret_cast<const unsigned char*>(a);
  const unsigned length = static_cast<unsigned>(b);
  std::int16_t tag;
  std::memcpy(&tag, packet, sizeof(tag));
  if (tag == 0x5460) {
    unsigned offset = 6;
    while (offset < length) {
      std::uint32_t size;
      std::memcpy(&tag, packet + offset, sizeof(tag));
      std::memcpy(&size, packet + offset + 2, sizeof(size));
      offset += 6;
      if (tag == 0x5461) {
        break;
      }
      if (tag == 0x5462) {
        char name[260];
        std::memset(name, 0, sizeof(name));
        std::memcpy(name, packet + offset, size);
        offset += size;
        this->SendMsg(nullptr, 95, 0, 0, reinterpret_cast<std::intptr_t>(name), 0);
      } else {
        offset += size;
      }
    }
  }
  return 0;
}

// OFFSET: 0x008FF640
// FUNCTION: fn_12358
int UIMixAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 184) {
      this->m_frameContext->windows.CloseWindow(160);
      this->m_frameContext->windows.CloseWindow(161);
      this->m_context->mode->SendModeMessage(266, 0, 0, 0, 0);
      this->m_frameContext->windows.CloseWindow(163);
      reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(163);
    }
    return 0;
  }

  if (message != 91) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->m_kind = static_cast<unsigned char>(a);
  std::string text;
  text = LookupMessage(1584);
  this->viewer->Clear();
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
  return 0;
}

// OFFSET: 0x008FF910
// FUNCTION: fn_12361
int UINotifyItemObtainWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 34:
    this->m_item = *reinterpret_cast<const ITEM_INFO*>(a);
    return 0;
  case 36: {
    this->m_kind = static_cast<int>(a);
    char text[128];
    if (a != 0) {
      char grouped[24];
      std::sprintf(text, LookupMessage(327), FormatGroupedInteger(static_cast<int>(b), grouped, sizeof(grouped)));
      this->m_message = text;
      this->m_itemId = "";
    } else {
      this->m_itemId = reinterpret_cast<const char*>(c);
      ITEM_INFO item;
      item.itemId = reinterpret_cast<const char*>(c);
      item.identified = this->m_identified;
      item.unknown_0060 = 0;
      std::sprintf(text, LookupMessage(153), item.DisplayName(0).c_str(), static_cast<int>(b));
      this->m_message = text;
    }

    int width = this->TextWidth(this->m_context->fonts, this->m_context->locale, this->m_message.c_str(), 0, 0, 12, 0, 0) + 54;
    width += width % 28 != 0 ? 28 : 0;
    if (width < 34) {
      width = 34;
    }
    if (width != this->geometry.width) {
      this->Resize(width, this->geometry.height);
    }
    this->Invalidate();
    this->m_shownTick = timeGetTime();
    return 0;
  }
  case 76:
    this->m_identified = a != 0;
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00905210
// FUNCTION: fn_12380
int UISkillNameChangeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 184) {
      if (this->m_skillId != -1 && this->m_edit->Length() != 0) {
        this->m_context->mode->SendModeMessage(98, this->m_skillId, reinterpret_cast<std::intptr_t>(this->m_edit->Text()), 0, 0);
      }
      this->m_frameContext->windows.CloseWindow(55);
    }
    return 0;
  }

  if (message != 68) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->m_skillId = static_cast<int>(a);
  this->m_name = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, static_cast<int>(a));
  this->m_edit->SetText(this->m_name.c_str());
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);
  this->m_edit->SelectAll();
  return 0;
}

// OFFSET: 0x00905590
// FUNCTION: fn_12382
int UIStarPlaceAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    this->m_acceptance->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);
    if (a == 184) {
      CMode* mode = g_modeMgr.GetGameMode();
      this->m_frameContext->windows.CloseWindow(119);
      mode->SendMsg(190, this->m_place, 0, 0, 0);
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(119);
    }
    return 0;
  }

  if (message == 91) {
    this->m_place = static_cast<char>(a);
    std::string text;
    if (this->m_place == 10) {
      text = LookupMessage(1263);
    } else {
      text = LookupMessage(1028);
    }
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }

  if (message != 123) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  const auto* packet = reinterpret_cast<const unsigned char*>(a);
  const unsigned length = static_cast<unsigned>(b);
  std::int16_t tag;
  std::memcpy(&tag, packet, sizeof(tag));
  if (tag == 0x54C4) {
    unsigned offset = 6;
    while (offset < length) {
      std::uint32_t size;
      std::memcpy(&tag, packet + offset, sizeof(tag));
      std::memcpy(&size, packet + offset + 2, sizeof(size));
      offset += 6;
      if (tag == 0x54C5) {
        break;
      }
      if (tag == 0x54C6) {
        std::memcpy(&this->m_place, packet + offset, size);
        offset += size;
        this->SendMsg(nullptr, 91, this->m_place, 0, 0, 0);
      } else {
        offset += size;
      }
    }
  }
  return 0;
}

// OFFSET: 0x00908080
// FUNCTION: fn_12390
int UIYourItemWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 184) {
      g_modeMgr.GetGameMode();
      WearEquipPacket packet;
      packet.packetType = ID_CZ_REQ_WEAR_EQUIP_V5;
      packet.index = static_cast<std::uint16_t>(this->m_index);
      packet.location = this->m_location;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
    } else if (a != 185) {
      return 0;
    }
    this->m_frameContext->windows.CloseWindow(138);
    return 0;
  }

  if (message != 91) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  std::string text;
  text = LookupMessage(1327);
  this->viewer->Clear();
  this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
  this->m_index = static_cast<int>(a);
  this->m_location = static_cast<int>(b);
  return 0;
}

// OFFSET: 0x008E6B40
// FUNCTION: fn_12192
void UIPasswordWnd::Slot15(int, int) {
  this->m_edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edit->SetSize(90, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 8;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->mask = 42;
  this->m_edit->SetText("");
  this->AddChild(this->m_edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);

  auto* title = new UIStaticText(*this->m_context, *this->m_controls);
  this->unknown_00B8 = reinterpret_cast<std::uintptr_t>(title);
  title->SetSize(this->geometry.width - 30, 15);
  title->Move(15, 6);
  title->SetText(LookupMessage(15), 0);
  this->AddChild(title);

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;
  this->geometry.unknown_0030 = 0;
  this->unknown_00BC = 0;
}
