#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Skills/SkillInfo.h"
#include "File/File.h"
#include "File/ShortcutFilename.h"
#include "UI/Exchange/UIExchangeUI.h"
#include "UI/Items/UISpellListWnd.h"
#include "Client/Registry.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/ClientItemLists.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "Contents/Items/ItemNaming.h"
#include "UI/Window/WindowRegistration.h"
#include "Contents/GuildInfo.h"
#include "Contents/TipBoxTitles.h"
#include "Contents/TipOfTheDay.h"
#include "Contents/SkinMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/EquipWindowLook.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
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
#include "UI/Text/TextWrapping.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UINoneVisibleButton.h"
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
#include "UI/Items/UISwapEquipmentWnd.h"
#include "UI/Items/UIExchangeWnd.h"
#include "UI/Items/UIItemCollectionWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Items/UIItemParamChangeDisplayWnd.h"
#include "UI/Items/UINotifyItemObtainWnd.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIMemberListBox.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Party/UIPartySettingWnd.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Shop/PriceColor.h"
#include "UI/Shop/UInCash_CallWnd.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Skill/UISkillNameChangeWnd.h"
#include "UI/Status/UIStatusWnd.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITagTextViewer.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIChangeNameWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIMonsterDisplayWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Window/FuncDrawWNDChar.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextTokens.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/TextEncoding.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Contents/TownInfoMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/NavigationManager.h"
#include "UI/Text/NavigationLink.h"
#include "Utility/Text/GroupedInteger.h"
#include "Client/Mode/ModeTooltip.h"
#include "Script/LuaScript.h"
#include "Utility/Text/StringUtil.h"
#include "UI/SkillDelayInfo/SkillDelayInfo.h"
#include "Contents/Skills/ClientSkills.h"
#include <algorithm>
#include <random>
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
#include "UI/Window/UICntWnd.h"

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

// GLOBAL: gl_25332
int g_window315Enabled = 1;

// GLOBAL: gl_25391
bool g_randomLoadingScreen;

// GLOBAL: gl_24646
UIWindow* g_linkInputOwner;

// GLOBAL: gl_25459
static const std::string s_partyItemLinkOpen = "<ITEML>";

// GLOBAL: gl_25460
static const std::string s_partyItemLinkClose = "</ITEML>";

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

// OFFSET: 0x008C6FA0
// FUNCTION: fn_11926
UIBookWnd::UIBookWnd(UIWindowContext& windows, UIFrameContext& frames, UIBookContext& book)
    : UIFrameWnd(windows, frames, 0), m_book(&book) {
  this->m_lineCount = 0;
  this->m_pageCount = 0;
  this->m_page = 1;
  this->m_autoTick = 0;
  this->m_autoLine = -1;
  this->m_autoRead = 0;
  this->unknown_00B8 = nullptr;
  this->m_bookName[0] = 0;
}

// OFFSET: 0x008C7860
// FUNCTION: fn_11940
UIEquipWnd::UIEquipWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->unknown_0124 = nullptr;
  this->unknown_0128 = 0;
  this->unknown_012C = 0;
  this->unknown_0130 = 0;
  this->unknown_0178 = 0;
  this->m_placement = nullptr;
  this->m_updateTick = timeGetTime();
  this->unknown_00D8 = 0;
  this->m_page = 0;
  this->m_contentTop = 20;
  for (int& value : this->unknown_00F8) {
    value = 0;
  }
  this->unknown_011C = 0;
  this->unknown_012C = 0;
  this->m_scrollBar = nullptr;
  for (UIWindow*& value : this->unknown_0138) {
    value = nullptr;
  }
  this->m_firstSlot = 0;
  this->unknown_015C = g_session.m_damageSkinMode;
  this->m_hoverSlot = g_session.m_damageSkinIndex;
  this->m_damageSkinNames.reserve(g_session.unknown_80C8);
  this->m_damageSkinIcons.reserve(g_session.unknown_80C8);
}

// OFFSET: 0x008CAC40
// FUNCTION: fn_11997
UIItemCollectionWnd::~UIItemCollectionWnd() {
  g_windowMgr.CloseWindow(276);
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

// OFFSET: 0x008C7420
// FUNCTION: fn_11931
UIChooseWarpWnd::UIChooseWarpWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIListBoxContext& listContext)
    : UIFrameWnd(windows, frames, 0), m_list(nullptr), unknown_00B8(0), m_choices(), m_controls(&controls), m_listContext(&listContext) {
  this->m_choices.reserve(6);
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

// OFFSET: 0x008ED240
// FUNCTION: fn_12221
void UIEquipWnd::Paint() {
  if (CBitmapRes* titleBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_fix.bmp")))) {
    this->DrawBitmap(0, 0, &titleBar->image, false);
  }
  this->DrawPanelBackground(17, 0, "");
  this->FillRegion(this->m_tabs->geometry.width, 34, this->geometry.width - this->m_tabs->geometry.width, 1, 0xC3C3C3);

  if (this->m_viewOther == 1) {
    char title[100];
    std::sprintf(title, LookupMessage(1361), reinterpret_cast<const char*>(&g_session.unknown_5974[0x2C]));
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 17, 4, title, 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 16, 3, title, 0, 0, 12, 0, 0, 0);
  } else {
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 17, 4, LookupMessage(104), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 16, 3, LookupMessage(104), 0, 0, 12, 0, 0, 0);
  }

  if (this->geometry.height == 17) {
    return;
  }

  const char* background = this->m_viewOther != 0 ? "유저인터페이스\\basic_interface\\equipwin_bg.bmp" : "유저인터페이스\\basic_interface\\equipwin_bg2.bmp";
  if (CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)))) {
    this->DrawBitmap(0, this->m_contentTop + 17, &bitmap->image, false);
  }

  if (this->m_page == 2) {
    this->FillRegion(1, this->m_contentTop + 15, this->geometry.width - 2, 131, 0xFFFFFF);
    DrawRectangleBorder(*this, 2, this->m_contentTop + 38, this->geometry.width - 4, 107, 0x646464);
    const std::string jobTitle = GetJobTitleString(g_session.unknown_615C);
    this->DrawCenteredText(1, this->m_contentTop + 20, jobTitle.c_str(), this->geometry.width - 2, 0, 13, 0, 1);
  }
  if (this->m_page == 3) {
    this->DrawDamageSkinPage();
  }

  if (this->m_page == 0 || this->m_page == 1) {
    if (CBitmapRes* invert = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\item_invert.bmp")))) {
      int columns[2] = {4, 248};
      for (int i = 0; i < 3; ++i) {
        const int slot = this->m_highlight[i];
        if (slot >= 0 && slot <= 7) {
          this->DrawBitmap(columns[slot / 4], this->m_contentTop + 51 + 26 * (slot % 4), &invert->image, true);
        } else if (slot == 8 || slot == 9) {
          this->DrawBitmap(columns[slot % 4], this->m_contentTop + 25, &invert->image, true);
        }
      }
    }

    this->DrawEquipSlots();
    this->RefreshLook();
    FuncDrawWNDChar drawer(
      this,
      this->geometry.width / 2 + 1,
      149,
      static_cast<unsigned char>(this->unknown_00F8[1]),
      static_cast<short>(this->unknown_00F8[0]),
      this->unknown_00F8[0],
      static_cast<short>(this->unknown_011C),
      static_cast<short>(this->unknown_00F8[2]),
      static_cast<short>(this->unknown_00F8[3]),
      static_cast<short>(this->unknown_00F8[4]),
      static_cast<short>(this->unknown_00F8[5]),
      this->unknown_0118,
      0,
      g_session.unknown_8074,
      this->unknown_00F8[6],
      this->unknown_00F8[7],
      0,
      0,
      this->m_updateTick
    );
    drawer.Draw(false);

    if (this->m_viewOther == 0) {
      if (this->m_page == 0) {
        ITEM_INFO dragItem = LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.unknown_16EC);
        if (dragItem.unknown_0000[1] != 0) {
          this->sub_8A8EE0(this->geometry.width / 2 - 25, this->m_contentTop + 44, &dragItem, 0);
          this->DrawItemName(this->geometry.width / 2 - 25, this->m_contentTop + 68, 40, &dragItem, 0, 0, 12);
        }
        this->m_otherButton->Enable(1);
        this->m_viewButton->Enable(1);
      } else {
        this->m_otherButton->Enable(0);
        this->m_viewButton->Enable(0);
      }
    }
  }

  if (this->unknown_0124) {
    this->unknown_0124->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
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

// OFFSET: 0x008E3340
// FUNCTION: fn_12182
void UIMinimapZoomWnd::Slot15(int, int) {
  const int ys[5] = {128, 128, 128, 128, 128};
  const int xs[5] = {1, this->geometry.width - 55, this->geometry.width - 41, this->geometry.width - 27, this->geometry.width - 13};
  const int ids[5] = {452, 216, 217, 189, 454};
  const char* tooltips[5] = {LookupMessage(2855), LookupMessage(2856), LookupMessage(2857), LookupMessage(2858), LookupMessage(2859)};
  const char* names[5] = {"object", "plus", "minus", "mini", "viewon"};
  const std::string prefix("minimap\\i_");

  auto& buttons = *reinterpret_cast<std::vector<UIWindow*>*>(&this->unknown_00BC);
  buttons.clear();
  for (int i = 0; i < 5; ++i) {
    const std::string name = prefix + names[i];
    UIWindow* button = this->AddNumberedButton(*this->m_controls, ids[i], xs[i], ys[i], name.c_str(), tooltips[i], true);
    buttons.push_back(button);
  }

  this->SendMsg(this, 6, 502, 0, 0, 0);

  for (int index = 0;; ++index) {
    TownInfo town = g_townInfoMgr->GetTownInfo(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->MapName(), index);
    if (town.image.empty()) {
      break;
    }
    this->AddTownButton(index, std::move(town));
  }
}

// OFFSET: 0x008FF130
// FUNCTION: fn_12357
int UIMinimapZoomWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return this->UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 189:
    if (!g_session.IsNavigationAllowed()) {
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2767), 0, true, false, 0, "Message", 280, 120, 0);
    } else if (!g_windowMgr.CloseWindow(273)) {
      g_windowMgr.sub_A978D0(273);
    }
    return 0;
  case 216:
    g_windowMgr.unknown_0050 += (g_windowMgr.unknown_0050 / 3.0f + g_windowMgr.unknown_0050) * 0.25f;
    if (g_windowMgr.unknown_0050 > 4.0f) {
      g_windowMgr.unknown_0050 = 4.0f;
    }
    return 0;
  case 217:
    g_windowMgr.unknown_0050 -= g_windowMgr.unknown_0050 * 0.25f;
    if (g_windowMgr.unknown_0050 < 1.0f) {
      g_windowMgr.unknown_0050 = 1.0f;
    }
    return 0;
  case 452:
    g_windowMgr.unknown_0054 = g_windowMgr.unknown_0054 == 0;
    for (auto it = this->m_zoom.begin(); it != this->m_zoom.end(); ++it) {
      static_cast<UIButton*>(it->second)->Enable(g_windowMgr.unknown_0054);
    }
    this->Invalidate();
    return 0;
  case 453: {
    const auto it = this->m_zoom.find(static_cast<int>(argument2));
    if (it != this->m_zoom.end()) {
      static_cast<UIButton*>(it->second)->Enable(1);
      it->second->Move(static_cast<int>(argument3) - this->geometry.x, static_cast<int>(argument4) - this->geometry.y);
    }
    this->Invalidate();
    return 0;
  }
  case 454:
    if (!g_windowMgr.CloseWindow(140)) {
      g_windowMgr.sub_A978D0(140);
    }
    return 0;
  case 455: {
    const auto it = this->m_zoom.find(static_cast<int>(argument2));
    if (it != this->m_zoom.end()) {
      static_cast<UIButton*>(it->second)->Enable(0);
    }
    this->Invalidate();
    return 0;
  }
  case 502: {
    const unsigned char enabled = g_serviceOptions.Get(223);
    const auto& buttons = *reinterpret_cast<std::vector<UIWindow*>*>(&this->unknown_00BC);
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
      static_cast<UIButton*>(*it)->Enable(enabled);
    }
    this->Invalidate();
    break;
  }
  }

  std::string mapName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->MapName();
  TownInfo town = g_townInfoMgr->GetTownInfo(mapName.c_str(), static_cast<unsigned int>(argument1 - 1000));
  if (!town.image.empty()) {
    const std::size_t extension = mapName.find_first_of(".");
    if (extension != std::string::npos) {
      mapName = mapName.substr(0, extension);
    }
    g_navigationManager.sub_B99030(mapName.c_str(), 0, 5, true, town.x, town.y, 1002);
  }
  return 0;
}

// OFFSET: 0x0090C020
// FUNCTION: fn_12437
void UIMinimapZoomWnd::AddTownButton(int index, TownInfo town) {
  UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap(town.image.c_str(), 0, 0);
  button->SetBitmap(town.image.c_str(), 1, 0);
  button->SetBitmap(town.image.c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(0, 0);
  button->Enable(0);
  button->SetId(index + 1000);
  button->SetTooltip(town.name.c_str());
  this->AddChild(button);
  this->m_zoom.insert(std::make_pair(index, static_cast<UIWindow*>(button)));
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

// OFFSET: 0x008EF850
// FUNCTION: fn_12238
void UINpcTextEditDialogWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  if (!this->m_tagCtrl) {
    return;
  }

  this->m_tagCtrl->SetTaggedText(std::string(this->edit->Text()), this->edit->geometry.width);
  this->m_tagCtrl->sub_8962C0();
  if (this->m_tagCtrl->unknown_011C == 0) {
    delete this->m_tagCtrl;
    this->m_tagCtrl = nullptr;
  }
}

// OFFSET: 0x008FFC90
// FUNCTION: fn_12365
int UINpcEditDialogWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (g_linkInputOwner) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2730)), 0x64FFFF, 0, 0, 0);
    return 0;
  }

  if (message == 6) {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets = {
      g_packetLengths,
      replay.baseFrame,
      replay.started,
      replay.frame,
      replay.header.key0,
      replay.header.key1,
      replay.header.options.recordRestricted,
      replay.file,
      replay.fileOffset,
      replay.records,
      replay.cipherIndex,
      replay.cipherBase,
      replay.cipherKey,
    };
    WindowResponseRecorder recorder{replay.state, packets};
    recorder.Record(this->geometry.id, static_cast<int>(argument1), argument2, argument3, argument4);
    if (argument1 == 184 && this->edit->Length()) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(99, this->unknown_00B8, std::atoi(this->edit->Text()), 0, 0);
      g_windowMgr.CloseWindow(56);
    }
    return 0;
  }

  if (message == 28) {
    this->unknown_00B8 = static_cast<int>(argument1);
    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008FFD70
// FUNCTION: fn_12366
int UINpcTextEditDialogWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    if (g_linkInputOwner) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2730)), 0x64FFFF, 0, 0, 0);
      return 0;
    }

    if (message == 28) {
      this->unknown_00B8 = static_cast<int>(argument1);
      return 0;
    }

    return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  Replay& replay = Replay::Instance();
  ReplayPacketRecorder packets = {
    g_packetLengths,
    replay.baseFrame,
    replay.started,
    replay.frame,
    replay.header.key0,
    replay.header.key1,
    replay.header.options.recordRestricted,
    replay.file,
    replay.fileOffset,
    replay.records,
    replay.cipherIndex,
    replay.cipherBase,
    replay.cipherKey,
  };
  WindowResponseRecorder recorder{replay.state, packets};
  recorder.Record(this->geometry.id, static_cast<int>(argument1), argument2, argument3, argument4);
  if (Replay::Instance().state == 7) {
    this->edit->SetText("임시");
  }

  if (argument1 != 184 || !this->edit->Length()) {
    return 0;
  }

  std::string text(this->edit->Text());
  const std::size_t newline = text.find('\n');
  if (newline != std::string::npos) {
    text = text.substr(0, newline);
    this->edit->SetText(text.c_str());
  }

  if (!g_forbiddenWords.Allows(this->edit->Text())) {
    this->edit->TextColor(RGB(255, 0, 0), 0xFFFFFFFF);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(14)), 0xFF, 0, 0, 0);
    return 0;
  }

  CMode* mode = static_cast<CMode*>(g_charViewMode);
  if (g_navigationLinkParser->HasLink(std::string(this->edit->Text()))
      || (!this->edit->LinkedText().empty() && this->m_tagCtrl && !this->m_tagCtrl->unknown_0140.empty())) {
    this->edit->SetText("");
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3042)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (!this->edit->LinkedText().empty()) {
    mode->SendMsg(164, this->unknown_00B8, reinterpret_cast<int>(this->edit->Text()), reinterpret_cast<int>(this->edit->LinkedText().c_str()), 0);
  } else if (this->m_tagCtrl && !this->m_tagCtrl->unknown_0140.empty()) {
    if (g_navigationLinkParser->HasUnownedItemLink(std::string(this->m_tagCtrl->unknown_0140.c_str())) == true) {
      this->edit->SetText("");
      return 0;
    }
    mode->SendMsg(164, this->unknown_00B8, reinterpret_cast<int>(this->edit->Text()), reinterpret_cast<int>(this->m_tagCtrl->unknown_0140.c_str()), 0);
  } else {
    mode->SendMsg(164, this->unknown_00B8, reinterpret_cast<int>(this->edit->Text()), 0, 0);
  }

  g_windowMgr.CloseWindow(100);
  return 0;
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

// OFFSET: 0x00900490
// FUNCTION: fn_12368
int UIPartySettingWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 184: {
      if (this->m_mode != 50) {
        g_windowMgr.CloseWindow(53);
      }
      CMode* mode = g_modeMgr.GetGameMode();
      if (ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount() != 0) {
        if (g_session.unknown_5464 != 0) {
          if (this->m_expShare != g_session.unknown_54A0 || this->m_itemShare != g_session.unknown_54A4 ||
              this->m_itemDistribute != g_session.unknown_54A8) {
            mode->SendMsg(259, this->m_expShare, this->m_itemShare, this->m_itemDistribute, 0);
          }
        }
        return 0;
      }
      if (this->m_mode != 50) {
        mode->SendMsg(168, 0, this->m_itemShare, this->m_itemDistribute, 0);
        return 0;
      }

      std::string name = this->m_nameEdit->Text();
      if (name.find('#') != std::string::npos) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2839)), 0xFF, 0, 0, 0);
      } else if (name.find(s_partyItemLinkOpen) != std::string::npos || name.find(s_partyItemLinkClose) != std::string::npos) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2812)), 0xFF, 0, 0, 0);
      } else if (std::strlen(name.c_str()) != 0) {
        mode->SendMsg(285, reinterpret_cast<std::intptr_t>(name.c_str()), 0, 0, 0);
        g_windowMgr.CloseWindow(53);
        g_windowMgr.CloseWindow(207);
        mode->SendMsg(168, 0, this->m_itemShare, this->m_itemDistribute, 0);
      } else {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2073)), 0xFF, 0, 0, 0);
      }
      return 0;
    }
    case 185:
    case 201:
      g_windowMgr.CloseWindow(53);
      g_windowMgr.CloseWindow(207);
      return 0;
    case 215:
      if (ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount() == 0 || g_session.unknown_5464 != 0) {
        if (sender == this->m_expShareButtons[0] || sender == this->m_expShareButtons[1]) {
          for (int i = 0; i < 2; ++i) {
            if (sender == this->m_expShareButtons[i]) {
              this->m_expShare = i;
              this->m_expShareButtons[i]->SendMsg(this, 13, 1, 0, 0, 0);
            } else {
              this->m_expShareButtons[i]->SendMsg(this, 13, 0, 0, 0, 0);
            }
          }
        }
        if (sender == this->m_itemShareButtons[0] || sender == this->m_itemShareButtons[1]) {
          for (int i = 0; i < 2; ++i) {
            if (sender == this->m_itemShareButtons[i]) {
              this->m_itemShare = i;
              this->m_itemShareButtons[i]->SendMsg(this, 13, 1, 0, 0, 0);
            } else {
              this->m_itemShareButtons[i]->SendMsg(this, 13, 0, 0, 0, 0);
            }
          }
        }
      } else {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(294)), 0xFFFF64, 10, 0, 0);
      }
      return 0;
    default:
      return 0;
    }
  case 34:
    this->m_placement = reinterpret_cast<WindowPlacement*>(a);
    if (!static_cast<PartySettingWndInfo*>(this->m_placement)->IsValid()) {
      static_cast<PartySettingWndInfo*>(this->m_placement)->Reset();
    }
    this->Move(this->m_placement->x, this->m_placement->y);
    break;
  case 60:
    if (this->m_mode == 5) {
      g_windowMgr.CloseWindow(53);
    } else {
      this->Invalidate();
      this->m_expShare = g_session.unknown_54A0;
      this->m_itemShare = g_session.unknown_54A4;
      this->m_itemDistribute = g_session.unknown_54A8;
      for (int i = 0; i < 2; ++i) {
        this->m_expShareButtons[i]->SendMsg(this, 13, this->m_expShare == i, 0, 0, 0);
        if (this->m_itemShareButtons[i]) {
          this->m_itemShareButtons[i]->SendMsg(this, 13, this->m_itemShare == i, 0, 0, 0);
        }
      }
      if (this->m_itemDistributeButton) {
        this->m_itemDistributeButton->SendMsg(this, 13, 1, 0, 0, 0);
      }
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
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

// OFFSET: 0x00905320
// FUNCTION: fn_12381
int UISpellListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 31: {
    this->m_skills.push_back(static_cast<std::uint32_t>(argument1));
    ITEM_INFO item;
    this->m_items.push_back(item);
    this->SendMsg(nullptr, 60, 0, 0, 0, 0);
    return 0;
  }
  case 27:
    this->unknown_00FC = static_cast<short>(argument1);
    return 0;
  case 60:
    this->UpdateScroll();
    this->Invalidate();
    return 0;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scroll;
    if (message == 9) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == 10) {
      this->m_scroll = previous + this->m_rows - 1;
    } else {
      this->m_scroll = previous + static_cast<int>(argument1);
    }
    if (previous != this->m_scroll) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scroll - previous;
  }
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets = {
      g_packetLengths,
      replay.baseFrame,
      replay.started,
      replay.frame,
      replay.header.key0,
      replay.header.key1,
      replay.header.options.recordRestricted,
      replay.file,
      replay.fileOffset,
      replay.records,
      replay.cipherIndex,
      replay.cipherBase,
      replay.cipherKey,
    };
    WindowResponseRecorder recorder{replay.state, packets};
    recorder.Record(this->geometry.id, static_cast<int>(argument1), argument2, argument3, argument4);
    CMode* mode = static_cast<CMode*>(g_charViewMode);
    if (argument1 == 184) {
      const int selected = this->m_selected;
      if (selected < 0 || static_cast<unsigned>(selected) >= this->m_skills.size()) {
        return 0;
      }
      if (this->unknown_00FC != 0) {
        mode->SendMsg(256, this->m_skills[selected], this->unknown_00FC, 0, 0);
      } else {
        mode->SendMsg(163, this->m_skills[selected], 0, 0, 0);
      }
    } else if (argument1 == 185) {
      if (this->unknown_00FC != 0) {
        mode->SendMsg(256, 0, this->unknown_00FC, 0, 0);
      } else {
        mode->SendMsg(163, 0, 0, 0, 0);
      }
    } else {
      return 0;
    }
    g_windowMgr.CloseWindow(99);
    return 0;
  }
  default:
    return 0;
  }
}

// OFFSET: 0x008F04F0
// FUNCTION: fn_12252
// OFFSET: 0x008CBA50
// FUNCTION: fn_11998
UISpellListWnd::~UISpellListWnd() {
  this->m_skills.clear();
}

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

// OFFSET: 0x008F0750
// FUNCTION: fn_12253
void UIStatusWnd::Paint() {
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, LookupMessage(105), 0);
  if (this->geometry.height == this->m_titleHeight || this->geometry.unknown_0030 != 0) {
    return;
  }

  const char* background = this->m_extended == 1 ? "유저인터페이스\\statuswnd\\w_ex_statwin_bg.bmp" : "유저인터페이스\\statuswnd\\w_statwin_bg.bmp";
  if (CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)))) {
    this->DrawBitmap(0, this->m_titleHeight, &bitmap->image, true);
  }

  const int stats[6][2] = {
    {g_session.m_standardStats[0], g_session.unknown_166C},
    {g_session.m_standardStats[1], g_session.unknown_1670},
    {g_session.m_standardStats[2], g_session.unknown_1674},
    {g_session.m_standardStats[3], g_session.unknown_1678},
    {g_session.m_standardStats[4], g_session.unknown_167C},
    {g_session.m_standardStats[5], g_session.unknown_1680},
  };
  const int* entry = stats[0];
  for (int y = 23; y < 119; y += 16, entry += 2) {
    const int value = entry[0];
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 36, y, FormatOwnedString("%d", value).c_str(), 0, 1, 14, 0, 0, 0);
    const int bonus = entry[1];
    if (bonus > 0) {
      if (value < 100) {
        this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 49, y, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 14, 0, 0, 0);
      } else {
        this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 54, y + 2, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 12, 0, 0, 0);
      }
    } else if (bonus < 0) {
      if (value < 100) {
        this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 49, y, FormatOwnedString("%d", bonus).c_str(), 0, 1, 14, 0, 0, 0);
      } else {
        this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 54, y + 2, FormatOwnedString("%d", bonus).c_str(), 0, 1, 12, 0, 0, 0);
      }
    }
  }

  const int* derived = reinterpret_cast<const int*>(g_session.unknown_169C);
  const int required[6] = {derived[0], derived[1], derived[2], derived[3], derived[4], derived[5]};
  const int* cost = required;
  for (int y = 23; y < 119; y += 16, ++cost) {
    this->DrawRightAlignedText(101, y, FormatOwnedString("%d", *cost).c_str(), 0, 1, 14, 0, 0);
  }

  this->DrawRightAlignedText(189, 22, FormatOwnedString("%d + %d", derived[7], derived[12]).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(189, 38, FormatOwnedString("%d + %d", derived[14], derived[13]).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(189, 54, FormatOwnedString("%d", derived[16]).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(189, 70, FormatOwnedString("%d", derived[18]).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(275, 22, FormatOwnedString("%d + %d", derived[10], derived[11]).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(275, 38, FormatOwnedString("%d + %d", derived[8], derived[15]).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(275, 54, FormatOwnedString("%d + %d", derived[17], derived[19]).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(275, 70, FormatOwnedString("%d", (2000 - derived[6]) / 10).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(275, 86, FormatOwnedString("%d", *reinterpret_cast<const int*>(g_session.unknown_1654)).c_str(), 0, 1, 14, 0, 0);
  if (g_guildInfo.unknown_0000.size() != 0) {
    this->DrawRightAlignedText(275, 102, g_guildInfo.unknown_0000.c_str(), 0, 0, 12, 0, 0);
  }

  if (this->m_extended != 1) {
    return;
  }

  const int* traitBonus = reinterpret_cast<const int*>(g_session.unknown_7FC0 + 8);
  const std::int64_t* traitPoints = reinterpret_cast<const std::int64_t*>(&g_session.unknown_7FC0[0x20]);
  const int bonuses[6] = {traitBonus[0], traitBonus[1], traitBonus[2], traitBonus[3], traitBonus[4], traitBonus[5]};
  const std::int64_t points[6] = {traitPoints[0], traitPoints[1], traitPoints[2], traitPoints[3], traitPoints[4], traitPoints[5]};
  const int types[6] = {219, 220, 221, 222, 223, 224};
  const std::int64_t* point = points;
  int index = 0;
  for (int y = 144; y < 240; y += 16, point += 1, ++index) {
    const int value = g_session.GetTraitStat(types[index]);
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 36, y, FormatOwnedString("%d", value).c_str(), 0, 1, 14, 0, 0, 0);
    this->DrawRightAlignedText(101, y, FormatOwnedString("%lld", *point).c_str(), 0, 1, 14, 0, 0);
    const int bonus = bonuses[index];
    if (bonus > 0) {
      if (value < 100) {
        this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 49, y, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 14, 0, 0, 0);
      } else {
        this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 54, y + 2, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 12, 0, 0, 0);
      }
    }
  }

  this->DrawRightAlignedText(189, 143, FormatOwnedString("%d", g_session.GetTraitStat(225)).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(189, 159, FormatOwnedString("%d", g_session.GetTraitStat(226)).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(189, 175, FormatOwnedString("%d", g_session.GetTraitStat(229)).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(189, 191, FormatOwnedString("%d", g_session.GetTraitStat(230)).c_str(), 0, 1, 14, 0, 0);
  int value = g_session.GetTraitStat(227);
  if (value < 0) {
    value = 0;
  }
  this->DrawRightAlignedText(275, 143, FormatOwnedString("%d", value).c_str(), 0, 1, 14, 0, 0);
  value = g_session.GetTraitStat(228);
  if (value < 0) {
    value = 0;
  }
  this->DrawRightAlignedText(275, 159, FormatOwnedString("%d", value).c_str(), 0, 1, 14, 0, 0);
  this->DrawRightAlignedText(275, 207, FormatOwnedString("%d", g_session.GetTraitStat(231)).c_str(), 0, 1, 14, 0, 0);
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

// OFFSET: 0x008F4520
// FUNCTION: fn_12288
void UIItemCollectionWnd::Slot28(int x, int y) {
  int slotX;
  int slotY;
  const int index = this->CardSlotAt(x, y, &slotX, &slotY);
  if (index == -1 || this->m_compare != 1) {
    return;
  }

  ITEM_INFO card;
  int cardId;
  std::memcpy(&cardId, this->m_item.unknown_001C + index * sizeof(cardId), sizeof(cardId));
  card.identified = 1;
  card.unknown_0000[0] = 6;
  card.unknown_0060 = 0;
  card.SetItemId(cardId);

  ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_charViewMode);
  std::uint32_t color = 0xFFFFFFFF;
  if (index >= ItemSlotsCount(this->m_item)) {
    color = ItemCardColor(&this->m_item);
  }

  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  tooltip->Show(naming.DisplayName(card, 0).c_str(), slotX, slotY, color, 0, 1);
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

// OFFSET: 0x008FEF80
// FUNCTION: fn_12356
int UIMenuWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 72: {
    const std::uint32_t color = 0;
    this->AddItem(g_menuSeparator, color);
    this->Invalidate();
    return 0;
  }
  case 31: {
    std::uint32_t color = static_cast<std::uint32_t>(b);
    this->AddItem(reinterpret_cast<const char*>(a), color);
    color = static_cast<std::uint32_t>(b);
    this->AddItem(reinterpret_cast<const char*>(a), color);
    this->Invalidate();
    return 0;
  }
  case 83:
    this->m_menuId = static_cast<int>(a);
    this->unknown_00CC = static_cast<std::uint32_t>(b);
    return 0;
  case 30: {
    int width = 20;
    const int count = static_cast<int>(this->m_items.end() - this->m_items.begin());
    for (int index = 0; index < count; ++index) {
      const int textWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, this->m_items[index].c_str(), 0, 0, 12, 0, 0);
      if (width < textWidth) {
        width = textWidth;
      }
    }
    this->Resize(width + 6, this->m_itemHeight * count / 2);
    this->Invalidate();
    return 0;
  }
  case 75:
    this->m_items.clear();
    this->m_colors.clear();
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008CEED0
// FUNCTION: fn_12073
void UITipboxWnd::ShowTip(int id) {
  g_itemTagOwner->SelectTip(id);
  this->SendMsg(this, 6, 543, 0, 0, 0);
  reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this);
}

// OFFSET: 0x008F5AF0
// FUNCTION: fn_12313
void UITipboxWnd::UpdatePageText() {
  this->m_pageText->SetText(IntToString((std::max)(0, g_itemTagOwner->unknown_00B4) + 1, 0).c_str(), 0);
  this->m_pageText->SetFont(0, 12, 1, 0);

  const std::string separator = "/";
  const int pages = static_cast<int>(g_itemTagOwner->m_currentEntry.paragraphs.size());
  this->m_pageTotalText->SetText((separator + IntToString(pages, 0)).c_str(), 0);

  const int pageWidth = this->ColoredTextWidth(
    IntToString((std::max)(0, g_itemTagOwner->unknown_00B4) + 1, 0).c_str(),
    std::strlen(IntToString((std::max)(0, g_itemTagOwner->unknown_00B4) + 1, 0).c_str()), 0, 12, 1
  );
  const int totalWidth = this->ColoredTextWidth(
    (separator + IntToString(pages, 0)).c_str(), std::strlen((separator + IntToString(pages, 0)).c_str()), 0, 12, 0
  );

  const int offset = (36 - totalWidth - pageWidth) / 2;
  this->m_pageText->Move(offset + 426, 320);
  this->m_pageTotalText->Move(pageWidth + 426 + offset, 320);
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

// OFFSET: 0x008D0010
// FUNCTION: fn_12082
void UIEquipWnd::Slot10(int x, int y, std::intptr_t value) {
  if (this->m_viewOther != 0) {
    return;
  }

  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  ITEM_INFO item(0);
  bool fromStorage = false;
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 1:
    item = LookupClientItemByIdentity(g_session.m_inventoryItems, drag->id);
    break;
  case 17:
    fromStorage = true;
    item = g_session.GetItem61B8(drag->slot);
    break;
  case 18:
    fromStorage = true;
    item = g_session.GetItem6B68(drag->slot);
    break;
  }

  if (item.unknown_0000[1] != 0) {
    switch (item.unknown_0000[0]) {
    case 10:
    case 16:
    case 17:
    case 19:
      mode->SendMsg(87, drag->id, item.unknown_0000[2], 0, 0);
      break;
    default:
      if (fromStorage) {
        mode->SendMsg(305, reinterpret_cast<int>(&item), 0, 0, 0);
      } else if (item.identified && item.unknown_0000[2] != 0 && item.unknown_0000[3] == 0) {
        mode->SendMsg(19, drag->id, item.unknown_0000[2], 0, 0);
      }
      break;
    }
  }

  static_cast<CMode*>(g_charViewMode)->SendMsg(18, 0, 0, 0, 0);
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
// FUNCTION: fn_12147
void UINpcTextEditDialogWnd::InsertItemTag(const ITEM_INFO& item) {
  if (!this->m_tagCtrl) {
    this->m_tagCtrl = new UIItemTagOnMegaPhone;
    if (!this->m_tagCtrl) {
      return;
    }
  }

  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};

  if (!this->m_tagCtrl->CanInsertTag(naming.DisplayName(item, 0), this->edit->geometry.width)) {
    return;
  }

  {
    std::string text = this->edit->Text();
    this->m_tagCtrl->taggedText = text;
  }
  this->edit->SetText(this->m_tagCtrl->AppendTag(naming.DisplayName(item, 0)).c_str());
  this->edit->MoveCursorToEnd();
  this->m_tagCtrl->sub_89CFE0(item, 0x12B8A4);
}

// OFFSET: 0x008DA690

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
    this->m_readButtons[1]->Invalidate();
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

// OFFSET: 0x008F9520
// FUNCTION: fn_12336
int UICombinedCardItemCollection_ComparisonWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 34) {
    return this->UIItemCollectionWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  UIWindow* window = g_windowMgr.LookupWindow(82);
  if (window) {
    this->Move(window->geometry.x - window->geometry.width, window->geometry.y);
  }
  return 0;
}

// OFFSET: 0x008F9460
// FUNCTION: fn_12335
int UICombinedCardItemCollectionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 34) {
    return this->UIItemCollectionWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->m_cardPlacement = reinterpret_cast<CombinedCardItemCollectionWndInfo*>(argument1);
  if (!this->m_cardPlacement->IsValid()) {
    this->m_cardPlacement->Reset();
    this->Move((g_renderer->viewport.width - 640) / 2 + this->m_cardPlacement->x, this->m_cardPlacement->y * g_renderer->viewport.height / 480);
  } else {
    this->Move(this->m_cardPlacement->x, this->m_cardPlacement->y);
  }
  return 0;
}

// OFFSET: 0x008FD640
// FUNCTION: fn_12348
int UIItemCompositionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets{g_packetLengths, replay.baseFrame, replay.started, replay.frame, replay.header.key0, replay.header.key1,
                                 replay.header.options.recordRestricted, replay.file, replay.fileOffset, replay.records, replay.cipherIndex,
                                 replay.cipherBase, replay.cipherKey};
    WindowResponseRecorder{replay.state, packets}.Record(this->geometry.id, static_cast<int>(a), b, this->m_selected, d);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(c);
    }
    if (a == 184) {
      if (this->m_selected != -1) {
        int found;
        ITEM_INFO item = this->GetItem(this->m_selected, &found);
        if (found != 0) {
          static_cast<CMode*>(g_charViewMode)->SendMsg(0x7C, item.unknown_0000[1], this->m_compositionState, 0, 0);
        } else {
          static_cast<CMode*>(g_charViewMode)->SendMsg(0x7C, -1, -1, 0, 0);
        }
        g_windowMgr.CloseWindow(0x4A);
      }
    } else if (a == 185) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(0x7C, -1, -1, 0, 0);
      g_windowMgr.CloseWindow(0x4A);
    }
    return 0;
  }
  case 0x22: {
    ItemCompositionWndInfo* placement = reinterpret_cast<ItemCompositionWndInfo*>(a);
    this->m_compositionPlacement = placement;
    if (!placement->IsValid()) {
      placement->Reset();
    }
    this->Move(placement->x, placement->y);
    return 0;
  }
  case 0x4D:
    this->m_compositionState = static_cast<int>(a);
    if (a != 0) {
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(a));
      if (item.unknown_0000[1] != 0) {
        const std::string name = item.DisplayName(0);
        this->m_title = FormatOwnedString("%s (%s)", LookupMessage(0x20A), name.c_str());
      }
    }
    return 0;
  case 0x7B: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x574E) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        if (header.type == 0x574F) {
          break;
        }
        if (header.type == 0x5751) {
          std::vector<unsigned char> packet(header.length, 0);
          std::memcpy(packet.data(), records + position, header.length);
          position += header.length;
          static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ReceiveUnknownD4E2A0(packet.data());
        } else {
          position += header.length;
        }
      }
    }
    return 0;
  }
  default:
    return UIItemIdentifyWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008FDA10
// FUNCTION: fn_12349
int UIItemIdentifyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets{g_packetLengths, replay.baseFrame, replay.started, replay.frame, replay.header.key0, replay.header.key1,
                                 replay.header.options.recordRestricted, replay.file, replay.fileOffset, replay.records, replay.cipherIndex,
                                 replay.cipherBase, replay.cipherKey};
    WindowResponseRecorder{replay.state, packets}.Record(this->geometry.id, static_cast<int>(a), b, this->m_selected, d);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(c);
    }
    if (a == 184) {
      if (this->m_selected != -1) {
        int found;
        ITEM_INFO item = this->GetItem(this->m_selected, &found);
        if (found != 0) {
          static_cast<CMode*>(g_charViewMode)->SendMsg(0x79, item.unknown_0000[1], 0, 0, 0);
        }
        g_windowMgr.CloseWindow(0x49);
      }
    } else if (a == 185) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(0x79, -1, 0, 0, 0);
      g_windowMgr.CloseWindow(0x49);
    }
    return 0;
  }
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scroll;
    if (message == 9) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == 10) {
      this->m_scroll = previous + this->m_rows - 1;
    } else {
      this->m_scroll = previous + static_cast<int>(a);
    }
    if (this->m_scroll != previous) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scroll - previous;
  }
  case 0x1F: {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(b));
    this->m_items.push_back(item);
    this->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
    return 0;
  }
  case 0x22: {
    ItemIdentifyWndInfo* placement = reinterpret_cast<ItemIdentifyWndInfo*>(a);
    this->m_placement = placement;
    if (!placement->IsValid()) {
      placement->Reset();
    }
    this->Move(placement->x, placement->y);
    return 0;
  }
  case 0x3C:
    this->UpdateScroll();
    this->Invalidate();
    return 0;
  case 0x4B:
    this->m_items.clear();
    return 0;
  case 0x7B: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x571C) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        if (header.type == 0x571D) {
          break;
        }
        if (header.type == 0x571F) {
          std::vector<unsigned char> packet(header.length, 0);
          std::memcpy(packet.data(), records + position, header.length);
          position += header.length;
          static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ReceiveUnknownD4E3F0(packet.data());
        } else {
          position += header.length;
        }
      }
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008FD5F0
// FUNCTION: fn_12347
int UIItemCollection_ComparisonWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 34) {
    return this->UIItemCollectionWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  UIWindow* window = g_windowMgr.LookupWindow(12);
  if (window) {
    UIFrameWnd::Move(window->geometry.x - window->geometry.width, window->geometry.y);
  }
  return 0;
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

namespace {
#pragma pack(push, 1)
  struct ChangeTitlePacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::int32_t titleId;
  };

  struct TakeoffEquipAllPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint32_t location;
  };
#pragma pack(pop)
}

bool IsEquipViewHidden();

// OFFSET: 0x008F9880

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

struct SessionItemLists {
  void ClearTradeItems();
};

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

      reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();
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

// OFFSET: 0x008E1890
// FUNCTION: fn_12175
void UIItemCollectionWnd::Slot15(int width, int height) {
  this->m_kind = 12;
  this->m_viewer = new UITextViewer(*this->m_context, *reinterpret_cast<UITextViewerContext*>(nullptr));
  this->m_viewer->Configure(91, 31, width - 98, height - 40, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  this->m_closeButton = this->AddCloseButton(*reinterpret_cast<UIButtonContext*>(nullptr), "ESC", 0);

  std::string path = "유저인터페이스\\";
  this->m_viewButton = new UIBitmapButton(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
  this->m_viewButton->SetBitmap((path + "btn_view" + ".bmp").c_str(), 0, 0);
  this->m_viewButton->SetBitmap((path + "btn_view_a" + ".bmp").c_str(), 1, 0);
  this->m_viewButton->SetBitmap((path + "btn_view_b" + ".bmp").c_str(), 2, 0);
  this->m_viewButton->SetSize(this->m_viewButton->BitmapWidth(), this->m_viewButton->BitmapHeight());
  this->m_viewButton->Move(6, 6);
  this->m_viewButton->SetId(250);
  this->AddChild(this->m_viewButton);

  const std::string prefix = "book\\";
  const char* tooltips[2] = {LookupMessage(1294), LookupMessage(1295)};
  const int ids[2] = {287, 288};
  const char* names[2] = {"책읽기", "책읽기"};
  for (int index = 0; index < 2; ++index) {
    this->m_readButtons[index] = new UISpriteButton(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
    this->m_readButtons[index]->SetSprite((prefix + names[index] + ".spr").c_str());
    this->m_readButtons[index]->SetActName((prefix + names[index] + ".act").c_str());
    this->m_readButtons[index]->SetSize(this->m_readButtons[index]->FrameWidth(0, 0), this->m_readButtons[index]->FrameHeight(0, 0));
    this->m_readButtons[index]->Move(-100, -100);
    this->m_readButtons[index]->SetId(ids[index]);
    this->m_readButtons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->m_readButtons[index]);
  }

  this->m_resizer = new UIResizer(*this->m_context);
  this->m_resizer->SetSize(16, 16);
  this->m_resizer->SetDirection(7);
  this->m_resizer->SetBitmapName(this->m_kind == 234 ? "유저인터페이스\\basic_interface\\btn_comparison_resize.bmp" : "유저인터페이스\\btn_resize.bmp");
  this->m_resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->m_resizer->Move(width - 17, height - 17);
  this->AddChild(this->m_resizer);

  this->m_previewTick = 0;
  if (!this->m_moveInfo) {
    this->m_moveInfo = new UIItemMoveInfo(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr), this->m_frameContext->windows,
                                          *reinterpret_cast<WindowVisibility*>(nullptr));
    this->m_moveInfo->Configure(LookupMessage(2796), 0, 0, 12, 0, 1);
    this->m_moveInfo->SetId(487);
    UIItemMoveInfo* moveInfo = this->m_moveInfo;
    int textHeight = this->TextHeight(this->m_context->fonts, this->m_context->locale, LookupMessage(2796), 0, 0, 12, 0, 0);
    moveInfo->ResizeToTextClipped(this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(2796), 0, 0, 12, 0, 0) + 1, textHeight);
    this->AddChild(this->m_moveInfo);
  }

  this->m_helpButton = new UIBitmapButton(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_normal.bmp", 0, 0);
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_over.bmp", 1, 0);
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_pressed.bmp", 2, 0);
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_disabled.bmp", 3, 0);
  this->m_helpButton->SetSize(this->m_helpButton->BitmapWidth(), this->m_helpButton->BitmapHeight());
  this->m_helpButton->Move(10, 10);
  this->m_helpButton->SetId(343);
  this->m_helpButton->SetTooltip(LookupMessage(4115));
  this->AddChild(this->m_helpButton);
}

// OFFSET: 0x008DE360
// FUNCTION: fn_12165
void UICombinedCardItemCollectionWnd::Slot15(int width, int height) {
  this->UIItemCollectionWnd::Slot15(width, height);
  this->m_kind = 82;
}

// OFFSET: 0x008DE380
// FUNCTION: fn_12166
void UICombinedCardItemCollection_ComparisonWnd::Slot15(int width, int height) {
  this->UIItemCollectionWnd::Slot15(width, height);
  this->m_closeButton->Enable(0);
  this->m_kind = 250;
  const SkinPaletteCell cell = {7, 1};
  this->m_viewer->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), cell, g_skinInfo.Selected()) | 0xFF000000u;
  this->m_viewer->Invalidate();
}

// OFFSET: 0x008E24C0
// FUNCTION: fn_12176
void UIItemCollection_ComparisonWnd::Slot15(int width, int height) {
  this->UIItemCollectionWnd::Slot15(width, height);
  this->m_closeButton->Enable(0);
  this->m_kind = 234;
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 30, 6, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
}

// OFFSET: 0x008DBD00
// FUNCTION: fn_12160
void UICardItemIllustWnd::Slot15(int, int) {
  this->unknown_00E4 = 0;
  this->m_name = LookupMessage(527);
  this->AddCloseButton(*this->m_controls, "", 0);
}

// OFFSET: 0x008E2590
// FUNCTION: fn_12178
void UIItemIdentifyWnd::Slot15(int, int) {
  this->m_title.assign(LookupMessage(521));
  this->m_selected = -1;
  this->m_scroll = 0;
  this->m_columns = 1;
  this->m_rows = 4;
  this->m_scrollable = 0;
  auto* scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 34, 1, this->geometry.height - 55);
  this->AddChild(this->m_scrollBar);
  this->UpdateScroll();

  {
    const std::string prefix("유저인터페이스\\");
    const char* names[1][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}};
    int positions[1][2] = {{this->geometry.width - 92, this->geometry.height - 24}};
    int ids[1] = {184};
    for (int i = 0; i < 1; ++i) {
      auto* button = new UIBitmapButton();
      button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[i][0], positions[i][1]);
      button->SetId(ids[i]);
      this->AddChild(button);
    }
  }
  this->m_message0 = 184;

  {
    const std::string prefix("유저인터페이스\\");
    const char* names[1][3] = {{"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    int positions[1][2] = {{this->geometry.width - 46, this->geometry.height - 24}};
    int ids[1] = {185};
    for (int i = 0; i < 1; ++i) {
      auto* button = new UIBitmapButton();
      button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
      const std::string hover = prefix + names[i][1] + ".bmp";
      button->SetBitmap(hover.c_str(), 1, 0);
      const std::string pressed = prefix + names[i][2] + ".bmp";
      button->SetBitmap(pressed.c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[i][0], positions[i][1]);
      button->SetId(ids[i]);
      this->AddChild(button);
    }
  }
}

// OFFSET: 0x008E2540
// FUNCTION: fn_12177
void UIItemCompositionWnd::Slot15(int width, int height) {
  this->UIItemIdentifyWnd::Slot15(width, height);
  this->m_title = LookupMessage(522);
  this->m_compositionState = 0;
}

// OFFSET: 0x008DE490
// FUNCTION: fn_12168
void UIComboBoxWnd::Slot15(int width, int height) {
  this->m_list = new UIComboBox();
  this->m_list->Configure(0, 0, width, height, 1);
  this->AddChild(this->m_list);
}

// OFFSET: 0x008E85C0
// FUNCTION: fn_12199
void UISkillDescribeWnd::Slot15(int width, int height) {
  this->unknown_00D4 = LookupMessage(526);
  this->m_skillId = -1;
  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(3, 3, width - 6, height - 6, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  const std::uint32_t color = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->backgroundColor = color;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);
  this->m_closeButton = this->AddCloseButton("ESC", 0);

  this->m_text = new UITextViewer();
  this->m_text->backgroundColor = color;
  this->m_text->Invalidate();
  this->m_text->Configure(3, 3, width - 31, height - 6, 0);
  this->AddChild(this->m_text);

  this->m_delayButton = new UIBitmapButton();
  this->m_delayButton->SetBitmap("유저인터페이스\\skilldelayinfo\\btn_sd_normal.bmp", 0, 0);
  this->m_delayButton->SetBitmap("유저인터페이스\\skilldelayinfo\\btn_sd_over.bmp", 1, 0);
  this->m_delayButton->SetBitmap("유저인터페이스\\skilldelayinfo\\btn_sd_pressed.bmp", 2, 0);
  this->m_delayButton->SetSize(this->m_delayButton->BitmapWidth(), this->m_delayButton->BitmapHeight());
  this->m_delayButton->Move(this->geometry.width - this->m_delayButton->BitmapWidth() - 4, 18);
  this->m_delayButton->SetId(343);
  this->m_delayButton->SetTooltip(LookupMessage(4254));
  this->m_delayButton->Enable(0);
  this->AddChild(this->m_delayButton);
  this->AddCloseButton("", 0);
}

// OFFSET: 0x008E8DC0
// FUNCTION: fn_12201
void UIStatusWnd::Slot15(int, int) {
  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "Alt+A", 0);
  this->AddMinimizeButton(*this->m_controls, "Ctrl+A", 0);

  {
    const int ids[6] = {205, 206, 207, 208, 209, 210};
    const std::string arrow = "유저인터페이스\\basic_interface\\arw_right.bmp";
    for (int i = 0; i < 6; ++i) {
      this->m_statButtons[i] = new UIBitmapButton(*this->m_context, *this->m_controls);
      this->m_statButtons[i]->SetBitmap(arrow.c_str(), 0, 0);
      this->m_statButtons[i]->SetBitmap(arrow.c_str(), 1, 0);
      this->m_statButtons[i]->SetBitmap(arrow.c_str(), 2, 0);
      this->m_statButtons[i]->SetSize(this->m_statButtons[0]->BitmapWidth(), this->m_statButtons[0]->BitmapHeight());
      this->m_statButtons[i]->Move(78, 24 + i * 16);
      this->m_statButtons[i]->SetId(ids[i]);
      this->AddChild(this->m_statButtons[i]);
    }
  }

  {
    const int ids[6] = {551, 552, 553, 554, 555, 556};
    const std::string arrow = "유저인터페이스\\basic_interface\\arw_right.bmp";
    for (int i = 0; i < 6; ++i) {
      this->m_extendedButtons[i] = new UIBitmapButton(*this->m_context, *this->m_controls);
      this->m_extendedButtons[i]->SetBitmap(arrow.c_str(), 0, 0);
      this->m_extendedButtons[i]->SetBitmap(arrow.c_str(), 1, 0);
      this->m_extendedButtons[i]->SetBitmap(arrow.c_str(), 2, 0);
      this->m_extendedButtons[i]->SetSize(this->m_extendedButtons[0]->BitmapWidth(), this->m_extendedButtons[0]->BitmapHeight());
      this->m_extendedButtons[i]->Move(78, 145 + i * 16);
      this->m_extendedButtons[i]->SetId(ids[i]);
      this->AddChild(this->m_extendedButtons[i]);
    }
  }

  this->m_expandButton = new UIBmpTextButton(*this->m_context, *this->m_controls);
  this->m_expandButton->SetBitmap("유저인터페이스\\statuswnd\\expand_on_normal.bmp", 0, 0);
  this->m_expandButton->SetBitmap("유저인터페이스\\statuswnd\\expand_on_normal.bmp", 1, 0);
  this->m_expandButton->SetBitmap("유저인터페이스\\statuswnd\\expand_on_press.bmp", 2, 0);
  this->m_expandButton->AddTextStyle(0, 12, 0, 0, 1, 16, 3);
  this->m_expandButton->AddTextStyle(1, 12, 0, 0, 1, 16, 3);
  this->m_expandButton->AddTextStyle(2, 12, 0, 0, 1, 16, 3);
  this->m_expandButton->SetSize(this->m_expandButton->BitmapWidth(), this->m_expandButton->BitmapHeight());
  {
    const std::string label = LookupMessage(3613);
    this->m_expandButton->text = label;
    this->m_expandButton->Invalidate();
  }
  this->m_expandButton->Move(2, 121);
  this->m_expandButton->SetId(550);
  this->AddChild(this->m_expandButton);
  this->m_expandedHeight = this->geometry.height;
}

// OFFSET: 0x008E8860
// FUNCTION: fn_12200
void UISkillNameChangeWnd::Slot15(int, int) {
  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(this->geometry.width - 77, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 29;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->SetText("");
  this->AddChild(this->m_edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);
  this->m_edit->SelectAll();

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
  this->m_skillId = -1;
  this->m_message0 = 184;
}

// OFFSET: 0x008EA780
// FUNCTION: fn_12205
void UIToolTipWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox();
  width -= 24;
  height -= 60;
  if (width < 20) {
    width = 20;
  }
  if (height < 30) {
    height = 30;
  }
  this->m_list->Configure(12, 22, width, height, 1);
  this->m_list->SetId(184);
  this->m_list->AddLine("__________");
  this->m_list->AddLine("**********");
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{this->geometry.width - 91, this->geometry.height - 24}, {this->geometry.width - 46, this->geometry.height - 24}};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }
}

// OFFSET: 0x008E7130
// FUNCTION: fn_12194
void UIProhibitListWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);
  char tooltip[] = "Command";

  auto* area = new UINoneVisibleButton(*this->m_context, *this->m_controls);
  area->SetSize(120, 18);
  area->Move(350, 59);
  area->SetId(268);
  this->AddChild(area);

  const std::string prefix("유저인터페이스\\basic_interface\\");
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "txtbox_btn_a" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "txtbox_btn_b" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "txtbox_btn_c" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(270 - button->BitmapWidth(), 44);
  button->SetTooltip(tooltip);
  button->SetId(268);
  this->AddChild(button);
}

// OFFSET: 0x008EE7F0
// FUNCTION: fn_12226
void UIItemIdentifyWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, false, this->m_title.c_str(), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);

  int row = 0;
  int index = 0;
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it, ++index) {
    if (index < this->m_scroll * this->m_columns) {
      continue;
    }

    const int top = row / this->m_columns * 32;
    const int y = top + 34;
    if (this->m_selected == index) {
      unsigned red, green, blue;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      this->FillRegion(5, top + 31, this->geometry.width - 24, 30, RGB(red, green, blue));
    }

    this->sub_8A8EE0(8, y, &*it, 0);
    this->DrawFittedItemName(40, y, &*it, 0, 0, 12, 0);
    if (it->unknown_0088 > 0) {
      this->DrawBitmap(
        8, y + 12,
        &static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->unknown_0088).c_str()))
        )->image,
        true
      );
    }

    ++row;
    if (row == this->m_rows * this->m_columns) {
      break;
    }
  }
}

// OFFSET: 0x008EECE0
// FUNCTION: fn_12229
void UIMakingArrowListWnd::Paint() {
  this->m_title = "LIST";
  this->UIItemIdentifyWnd::Paint();
}

// OFFSET: 0x008EFB00
// FUNCTION: fn_12242
void UIPetEggListWnd::Paint() {
  this->m_title = LookupMessage(599);
  this->UIItemIdentifyWnd::Paint();
}

// OFFSET: 0x008D2370
// FUNCTION: fn_12097
void UInCashWnd::DrawItems() {
  int row = 0;
  int index = 0;
  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->m_page * 8 - 8) {
      continue;
    }

    int x = 71;
    int column = row;
    if (column > 3) {
      x = 257;
      column -= 4;
    }
    const int y = column * this->m_rowHeight + this->m_listTop;
    const std::string path = "유저인터페이스\\collection\\" + std::string(ItemResourcePath(*it)) + ".bmp";
    if (!path.empty()) {
      this->DrawBitmap(x, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())))->image, true);
    }

    char text[64];
    char number[64];
    const int width = this->DrawFittedItemName(x, -30, &*it, 0, 0, 12, 0);
    this->DrawWrappedItemName(x - width / 2 + 90, y + 4, width, &*it, 0, 0, 12);
    int length = std::sprintf(text, LookupMessage(1854));
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, x + 112, y + 64, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, " %s C", FormatGroupedInteger(static_cast<int>(it->unknown_0014[0]), number, 64));
    this->DrawRightAlignedText(x + 152, y + 84, text, length, 0, 12, 0, 0);

    ++row;
    if (row > 7) {
      break;
    }
  }

  row = 0;
  index = 0;
  for (auto it = this->m_otherItems.begin(); it != this->m_otherItems.end(); ++it, ++index) {
    const int y = this->m_rowHeight * row + this->m_listTop;
    if (index < this->m_firstOther || index >= this->m_firstOther + 4) {
      continue;
    }

    const std::string path = "유저인터페이스\\collection\\" + std::string(ItemResourcePath(*it)) + ".bmp";
    if (!path.empty()) {
      this->DrawBitmap(453, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())))->image, true);
    }

    char text[64];
    char number[64];
    std::memset(text, 0, sizeof(text));
    std::memset(number, 0, sizeof(number));
    const int width = this->DrawFittedItemName(453, -30, &*it, 0, 0, 12, 0);
    this->DrawWrappedItemName(535 - width / 2, y + 4, width, &*it, 0, 0, 12);
    int length = std::sprintf(text, LookupMessage(1854));
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 565, y + 24, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, " %s C", FormatGroupedInteger(static_cast<int>(it->unknown_0014[0]), number, 64));
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 550, y + 42, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, LookupMessage(1855));
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 565, y + 66, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, " %d %s", it->quantity, LookupMessage(1856));
    this->DrawRightAlignedText(590, y + 84, text, length, 0, 12, 0, 0);

    ++row;
    if (row > 3) {
      break;
    }
  }
}

// OFFSET: 0x008E3C80
// FUNCTION: fn_12183
void UIMonsterDisplayWnd::Slot15(int, int) {
  this->unknown_00E4 = 0;
  this->unknown_00CC = LookupMessage(527);
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

// OFFSET: 0x008CED30
// FUNCTION: fn_12072
void UIFrameWnd::FitLastTextLine(int width, unsigned lineCount, int family, int height, FrameTextLines& lines) {
  if (lines.begin() == lines.end()) {
    return;
  }

  if (static_cast<unsigned>(lines.end() - lines.begin()) < lineCount - 1) {
    return;
  }

  auto& last = lines.back();
  if (last.begin() == last.end()) {
    return;
  }

  int total = 0;
  for (auto it = last.begin(); it != last.end(); ++it) {
    int segment = this->TextWidth(this->m_context->fonts, this->m_context->locale, it->second.c_str(), it->second.size(), 0, 12, 0, 0);
    total += segment;
    if (total >= width) {
      it->second = this->FitDisplayText(width - total + segment, it->second.c_str(), family, height, 0);
      last.erase(it + 1, last.end());
      break;
    }
  }
}

// OFFSET: 0x0090AD60
// FUNCTION: fn_12417
void UIFrameWnd::DrawTextLines(int x, int y, int family, int lineHeight, const FrameTextLines& lines) {
  y -= (static_cast<int>(lines.end() - lines.begin()) - 1) * (lineHeight / 2);
  for (auto line = lines.begin(); line != lines.end(); ++line) {
    int left = x;
    for (auto it = line->begin(); it != line->end(); ++it) {
      this->DrawShadowedText(left, y, it->second.c_str(), it->second.size(), it->first, 0xE0E0E0, family, 12, 0);
      left += this->TextWidth(this->m_context->fonts, this->m_context->locale, it->second.c_str(), it->second.size(), 0, 12, 0, 0);
    }
    y += lineHeight;
  }
}

// OFFSET: 0x008D8EE0
// FUNCTION: fn_12132
void UIFrameWnd::SplitHighlightText(
  const char* text, const char* highlight, const int& highlightColor, const int& color, const std::vector<char*>& breaks,
  std::vector<std::pair<int, std::string>>& segments
) {
  if (!highlight) {
    segments.emplace_back(color, text);
    return;
  }

  std::vector<char*> points = breaks;
  points.erase(std::unique(points.begin(), points.end()), points.end());
  auto it = std::find(points.begin(), points.end(), highlight);
  if (it == points.end()) {
    segments.emplace_back(color, text);
  } else if (it + 1 == points.end()) {
    segments.emplace_back(highlightColor, text);
  } else {
    std::string full(text);
    std::string word = full.substr(*it - text, *(it + 1) - *it);
    if (g_inlineCharset) {
      std::string::size_type marker = 0;
      while ((marker = full.find('|', marker)) != std::string::npos) {
        if (marker + 2 < full.size()) {
          full.erase(marker, 3);
        }
      }
    }

    std::string::size_type pos = full.find(word);
    if (pos != std::string::npos) {
      if (pos != 0) {
        segments.push_back(std::make_pair(color, full.substr(0, pos)));
      }
      segments.push_back(std::make_pair(highlightColor, full.substr(pos, word.size())));
      if (word.size() + pos < full.size()) {
        segments.push_back(std::make_pair(color, full.substr(word.size() + pos)));
      }
    }
  }
}

// OFFSET: 0x0090AB10
// FUNCTION: fn_12416
int UIWindow::DrawItemNameSegments(
  int x, int y, int width, const std::vector<char*>& breaks, const char* text, const char* highlight, std::uint32_t highlightColor,
  std::uint32_t color, int, int lineHeight
) {
  auto it = breaks.begin();
  int offset = 0;
  if (this->TextWidth(this->m_context->fonts, this->m_context->locale, text, 0, 0, 12, 0, 0) < width) {
    for (; it != breaks.end(); ++it) {
      const char* end = *it;
      int bytes = static_cast<int>(end - text);
      if (bytes) {
        this->DrawShadowedText(offset + x, y, text, bytes, text == highlight ? highlightColor : color, 0xE0E0E0, 0, 12, 0);
        offset += this->TextWidth(this->m_context->fonts, this->m_context->locale, text, bytes, 0, 12, 0, 0);
        text = end;
      }
    }
    return x;
  }

  int line = 1;
  y -= lineHeight / 2;
  for (; it != breaks.end(); ++it) {
    const char* segment = text;
    const char* end = *it;
    const char* cursor = text;
    while (cursor < end) {
      const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), cursor, 0);
      if (g_inlineCharset && !TextContext::Marker(cursor, static_cast<unsigned>(std::strlen(cursor)))) {
        cursor += 3;
        continue;
      }

      int bytes = static_cast<int>(next - cursor);
      int nextWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, next, static_cast<unsigned>(std::strlen(next)), 0, 12, 0, 0);
      int reach = this->TextWidth(this->m_context->fonts, this->m_context->locale, cursor, bytes, 0, 12, 0, 0) + offset;
      if (line == 2 && reach >= width - 15 && reach + nextWidth > width) {
        if (segment == highlight) {
          this->DrawShadowedText(bytes + offset + x, y, "...", 3, highlightColor, 0xE0E0E0, 0, 12, 0);
        } else {
          this->DrawShadowedText(bytes + offset + x, y - 1, "...", 3, color, 0xE0E0E0, 0, 12, 0);
        }
        break;
      }

      this->DrawShadowedText(x + offset, y, cursor, bytes, segment == highlight ? highlightColor : color, 0xE0E0E0, 0, 12, 0);
      offset += this->TextWidth(this->m_context->fonts, this->m_context->locale, cursor, bytes, 0, 12, 0, 0);
      if (offset > width) {
        offset = 0;
        y += lineHeight + 1;
        ++line;
      }
      cursor = next;
    }
    text = cursor;
  }
  return x;
}

// OFFSET: 0x0090AED0
// FUNCTION: fn_12418
int ScaleByScreenHeight(int value) {
  return g_renderer->viewport.height * value / 480;
}

// OFFSET: 0x009076C0
// FUNCTION: fn_12389
int UIWhisperWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 3:
    if (this->m_chatHistory->verticalOverflow != 0) {
      this->m_chatHistory->SendMsg(nullptr, 9, 0, 0, 0, 0);
    }
    return 0;
  case 4:
    if (this->m_chatHistory->verticalOverflow != 0) {
      this->m_chatHistory->SendMsg(nullptr, 10, 0, 0, 0, 0);
    }
    return 0;
  case 6: {
    if (a != 184) {
      if (a == 201) {
        g_windowMgr.Schedule(this);
      }
      return 0;
    }

    if (HasItemLinkTag(std::string(this->m_edit->Text())) == true) {
      this->m_edit->SetText("");
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3042)), 0xFF, 0, 0, 0);
      return 0;
    }

    char text[256];
    std::strcpy(text, this->m_edit->Text());
    this->m_edit->PushHistory();
    InputFocus{g_windowMgr.hoverWindow}.Set(this->m_edit);
    if (std::strlen(text) == 0) {
      return 0;
    }

    if (g_serviceType == 1 && !ValidateCharacterName(text)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(190)), 0xFF, 0, 0, 0);
      return 0;
    }

    if (this->m_name.size() == 0 || text[0] == '/') {
      if (g_session.ProcessEmotionShortcut(text) != 0) {
        return 0;
      }

      std::string parts[3];
      int talkType;
      const int offset = g_session.GetTalkType(text, &talkType, parts);
      if (offset != -1) {
        g_windowMgr.SetUnknown04DC(&text[offset]);
      }

      if (talkType == 24) {
        g_windowMgr.Schedule(this);
      } else if (talkType == 51) {
        this->SaveChatLog();
      } else {
        static_cast<CMode*>(g_charViewMode)->SendMsg(42, talkType, reinterpret_cast<std::intptr_t>(parts), 0, 0);
      }
      return 0;
    }

    if (this->m_edit->links.size() != 0) {
      g_windowMgr.SetUnknown04DC(this->m_edit->LinkedText().c_str());
    } else if (this->m_edit->unknown_0144 != nullptr) {
      g_windowMgr.SetUnknown04DC(this->m_edit->unknown_0144->unknown_0140.c_str());
    } else {
      g_windowMgr.unknown_04DC.assign(text, std::strlen(text));
    }

    static_cast<CMode*>(g_charViewMode)->SendMsg(11, reinterpret_cast<std::intptr_t>(this->m_target.c_str()), 0, 0, 0);
    return 0;
  }
  case 14:
    if (a == 7) {
      int width = b <= 512 ? static_cast<int>(b) : 512;
      int height = c <= 384 ? static_cast<int>(c) : 384;
      if (width < 280) {
        width = 280;
      }
      if (height < 120) {
        height = 120;
      }

      width = (width - 280) / 32 * 32 + 280;
      height = (height - 120) / 32 * 32 + 120;
      if (width != this->geometry.width || height != this->geometry.height) {
        this->Resize(width, height);
        this->m_resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->m_edit->Resize(this->geometry.width - 30, 16);
        this->m_edit->Move(3, this->geometry.height - 18);
        this->m_chatHistory->Resize(this->geometry.width - 2, this->geometry.height - 38, true);
        this->m_chatHistory->Move(1, 17);
        this->m_closeButton->Move(this->geometry.width - 14, 3);
      }
      this->m_frameContext->snapping.Store(this);
    }
    return 0;
  case 34: {
    WhisperWndInfo* info = reinterpret_cast<WhisperWndInfo*>(a);
    this->unknown_0100 = info;
    if (info->IsValid() == 0) {
      static_cast<WhisperWndInfo*>(this->unknown_0100)->Reset();
    }
    this->SendMsg(nullptr, 14, 7, this->unknown_0100->width, this->unknown_0100->height, 0);
    this->Move(this->unknown_0100->x, this->unknown_0100->y);
    return 0;
  }
  case 37: {
    char* line = reinterpret_cast<char*>(a);
    if (g_forbiddenWords.Contains(line)) {
      std::string masked;
      g_forbiddenWords.Mask(line, &masked);
    }

    const int color = b != 0x7800 ? 0xE8DDB6 : 0xFFFF;
    if (_mbsstr(reinterpret_cast<const unsigned char*>(line), reinterpret_cast<const unsigned char*>(s_partyItemLinkOpen.c_str()))) {
      this->m_chatHistory->AddText(line, color, nullptr);
    } else {
      this->m_chatHistory->AddTextWrapped(line, color, nullptr);
    }
    this->m_chatHistory->AddLinkRecord(line, color, "");
    return 0;
  }
  case 48:
    if (this->m_log[a].unknown_0018 == 11) {
      this->SaveChatLog();
    }
    return 0;
  case 49: {
    this->m_log.clear();
    if (g_windowMgr.unknown_0210 != nullptr) {
      g_windowMgr.CloseWindow(0x12);
    }

    this->m_log.push_back(WhisperLogLine{std::string(LookupMessage(312)), 11});
    if (!g_windowMgr.IsFriend(this->m_target.c_str()) && g_serviceMode != 3) {
      this->m_log.push_back(WhisperLogLine{std::string(LookupMessage(358)), 21});
    }

    static_cast<CMode*>(g_charViewMode)->SendMsg(101, reinterpret_cast<std::intptr_t>(this->m_target.c_str()), 0, 0, 0);
    if (this->m_log.empty()) {
      return 0;
    }

    UIWindow* menu = g_windowMgr.EnsureWindow(0x12);
    for (unsigned index = 0; index < this->m_log.size(); ++index) {
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(this->m_log[index].text.c_str()), 0, 0, 0);
    }
    menu->SendMsg(nullptr, 0x1E, 0, 0, 0, 0);
    menu->Move(
      std::min(std::max(g_screenWidth + 1, 1), g_renderer->viewport.width - menu->geometry.width),
      std::min(std::max(g_screenHeight + 1, 1), g_renderer->viewport.height - menu->geometry.height)
    );
    return 0;
  }
  case 154:
    this->m_chatHistory->ResolvePlayerTags(reinterpret_cast<const char*>(a));
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
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

// OFFSET: 0x00909F40
// FUNCTION: fn_12407
void UIItemCompositionWnd::Slot11() {
  if (this->m_compositionPlacement) {
    this->m_compositionPlacement->width = this->geometry.width;
    this->m_compositionPlacement->height = this->geometry.height;
    this->m_compositionPlacement->x = this->geometry.x;
    this->m_compositionPlacement->y = this->geometry.y;
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

// OFFSET: 0x00901520
// FUNCTION: fn_12375
int UISayDialogWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (g_windowMgr.unknown_0458) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2730)), 0x64FFFF, 0, 0, 0);
    return 0;
  }

  UITagTextViewer* viewer = static_cast<UITagTextViewer*>(this->m_viewer);
  switch (message) {
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets{g_packetLengths, replay.baseFrame, replay.started, replay.frame, replay.header.key0, replay.header.key1,
                                 replay.header.options.recordRestricted, replay.file, replay.fileOffset, replay.records, replay.cipherIndex,
                                 replay.cipherBase, replay.cipherKey};
    WindowResponseRecorder{replay.state, packets}.Record(this->geometry.id, static_cast<int>(a), b, c, this->unknown_00C8);
    if (g_replayAnimationScaling != 0) {
      this->unknown_00C8 = static_cast<int>(d);
    }
    if (a == 218) {
      const DWORD tick = GetTickCount();
      if (tick - *reinterpret_cast<const DWORD*>(&g_session.unknown_5490[0x04]) < 300) {
        return 0;
      }
      *reinterpret_cast<DWORD*>(&g_session.unknown_5490[0x04]) = tick;
      static_cast<CMode*>(g_charViewMode)->SendMsg(this->unknown_00C8, this->unknown_00CC, 218, 0, 0);
      this->unknown_00D4 = 1;
      this->SendMsg(nullptr, 32, 0, 0, 0, 0);
    } else if (a != 600) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(this->unknown_00C8, this->unknown_00CC, static_cast<int>(a), 0, 0);
    }
    return 0;
  }
  case 18:
    viewer->SendMsg(nullptr, 18, 0, 0, 0, 0);
    return 0;
  case 19:
    viewer->SendMsg(nullptr, 19, 0, 0, 0, 0);
    return 0;
  case 25:
    this->m_message0 = 218;
    this->unknown_00C8 = 22;
    this->SendMsg(nullptr, 85, 218, reinterpret_cast<std::intptr_t>("btn_next"), 0, 0);
    this->Invalidate();
    return 0;
  case 26:
    if (a != 0) {
      if (this->unknown_00D4 != 0) {
        viewer->Clear();
        this->unknown_00D4 = 0;
      }
      viewer->SendMsg(nullptr, 29, a, c, 1, 0);
    }
    this->unknown_00CC = static_cast<int>(b);
    return 0;
  case 32:
    this->m_message0 = 600;
    this->SendMsg(nullptr, 86, 218, 0, 0, 0);
    this->Invalidate();
    return 0;
  case 34:
    this->m_placement = reinterpret_cast<WindowPlacement*>(a);
    static_cast<SayDialogWndInfo*>(this->m_placement)->IsValid();
    this->Move(this->m_placement->x, this->m_placement->y);
    return 0;
  case 63:
    this->m_message0 = 201;
    this->unknown_00C8 = 89;
    this->SendMsg(nullptr, 85, 218, reinterpret_cast<std::intptr_t>("btn_close"), 0, 0);
    this->Invalidate();
    return 0;
  case 75:
    viewer->Clear();
    return 0;
  case 80:
    this->unknown_00C8 = static_cast<int>(a);
    return 0;
  case 85: {
    const std::string prefix("유저인터페이스\\");
    UIBitmapButton* button = new UIBitmapButton();
    const char* name = reinterpret_cast<const char*>(b);
    button->SetBitmap((prefix + name + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + name + "_a.bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->ButtonX(button), this->geometry.height - 24);
    button->SetId(static_cast<int>(a));
    this->AddChild(button);
    this->m_buttons.push_back(button);
    return 0;
  }
  case 86:
    for (auto it = this->m_buttons.begin(); it != this->m_buttons.end(); ++it) {
      if ((*it)->geometry.id == a) {
        (*it)->Move(-100, -100);
        g_windowMgr.Schedule(*it);
        this->m_buttons.erase(it);
        break;
      }
    }
    return 0;
  case 123: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 22500) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        if (header.type == 22501) {
          break;
        }
        if (header.type == 22503) {
          char text[260];
          std::memset(text, 0, sizeof(text));
          std::memcpy(text, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, 26, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
        } else if (header.type == 22504) {
          std::memcpy(&this->unknown_00D4, records + position, header.length);
          position += header.length;
        } else {
          position += header.length;
        }
      }
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
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
// FUNCTION: fn_12213
void UIChatRoomMakeWnd::Paint() {
  auto* titleBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_fix.bmp")));
  this->DrawBitmap(0, 0, titleBar ? &titleBar->image : nullptr, false);
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\chatwin0_bg.bmp")));
  this->DrawBitmap(0, 17, background ? &background->image : nullptr, false);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 9, 4, this->m_title.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 8, 3, this->m_title.c_str(), 0, 0, 12, 0, 0, 0);
}

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
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIEditCtrl* m_edit;
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
// FUNCTION: fn_12121
int UISayDialogWnd::ButtonX(UIBitmapButton* button) {
  int x = this->geometry.width - 4;
  for (UIWindow* other : this->m_buttons) {
    if (button != other) {
      x += -4 - static_cast<UIBitmapButton*>(other)->BitmapWidth();
    }
  }
  return x - button->BitmapWidth();
}

// OFFSET: 0x008D76B0
// FUNCTION: fn_12122
__declspec(noinline) int UIItemCollectionWnd::CardSlotAt(int x, int y, int* slotX, int* slotY) {
  const int localX = x - 3;
  *slotX = localX / 25 * 25 + 4;
  *slotY = this->geometry.height - 50;
  this->ToScreen(*slotX, *slotY);
  if (y <= this->geometry.height - 26) {
    return -1;
  }

  int left = 0;
  int slots = ItemSlotsCount(this->m_item);
  bool equipment;
  switch (this->m_item.unknown_0000[0]) {
  case 4:
  case 5:
  case 8:
  case 9:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 21:
    equipment = true;
    break;
  default:
    equipment = false;
    break;
  }
  if (equipment && !this->m_item.IsApprenticeItem() && !this->m_item.HasSpecialCard()) {
    slots = 4;
  } else if (slots <= 0) {
    return -1;
  }

  for (int index = 0; index < slots; ++index, left += 25) {
    if (left < localX && localX < left + 25) {
      int cardId;
      std::memcpy(&cardId, this->m_item.unknown_001C + index * sizeof(cardId), sizeof(cardId));
      if (cardId == 0) {
        return -1;
      }
      return index;
    }
  }
  return -1;
}

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
    std::list<ITEM_INFO>::iterator it = this->items.begin();
    for (int row = 0; row < 4; ++row) {
      if (y >= top && y <= top + this->m_rowHeight) {
        int index = row + (this->m_page - 1) * 8;
        int count = lists.GetList1788(this->m_tab);
        if (this->m_tab == 8) {
          count = static_cast<int>(this->items.size());
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
    return it != this->items.end() ? *it : ITEM_INFO(0);
  }

  if (x < 444) {
    std::list<ITEM_INFO>::iterator it = this->items.begin();
    for (int row = 0; row < 4; ++row) {
      if (y >= top && y <= top + this->m_rowHeight) {
        int index = this->m_page * 8 - 4 + row;
        int count = lists.GetList1788(this->m_tab);
        if (this->m_tab == 8) {
          count = static_cast<int>(this->items.size());
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
    return it != this->items.end() ? *it : ITEM_INFO(0);
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
    mode->drag.BeginItem(g_windowMgr, descriptions, source, item, 1, -1);
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

// OFFSET: 0x008C8890
// FUNCTION: fn_11962
UINpcTextEditDialogWnd::UINpcTextEditDialogWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UINpcEditDialogWnd(windows, frames, controls), m_tagCtrl(nullptr) {
}

// OFFSET: 0x008C8B50
// FUNCTION: fn_11968
UIProhibitListWnd::UIProhibitListWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& edits)
    : UIFrameWnd(windows, frames, 0), m_count(0), m_controls(&controls), m_edits(&edits) {
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

// OFFSET: 0x008C8F00
// FUNCTION: fn_11973
UISelectCartWnd::UISelectCartWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), unknown_00B4(0), unknown_00B8(0), unknown_00BC(0), unknown_00C0(0) {
}

// OFFSET: 0x008C9280
// FUNCTION: fn_11979
UIStorePasswordInputWnd::UIStorePasswordInputWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), unknown_00BC(-1), unknown_00C0(2) {
  std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
  std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
  std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
  this->unknown_00F0 = 0;
  this->unknown_0108 = 0;
  this->unknown_00F4 = 0;
  this->unknown_00EC = 0;
  this->unknown_00F8 = 40;
  this->unknown_00FC = 40;
  this->unknown_0100 = 80;
  this->unknown_0104 = 119;
  if (g_session.m_passwordInputAlternate) {
    this->unknown_00F4 = 2;
    this->unknown_00F8 = 80;
    this->unknown_00EC = 0;
    this->unknown_00F0 = 0;
    std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
    std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
    std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
  }
}

// OFFSET: 0x008C8D40
// FUNCTION: fn_11972
UISecondPasswordInputWnd::UISecondPasswordInputWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIStorePasswordInputWnd(windows, frames), unknown_010C(4), unknown_011C(0), unknown_0120(0), unknown_0124(), unknown_014C(0),
      unknown_0150(0), unknown_0154(0), unknown_0158(0), unknown_0164(0) {
}

// OFFSET: 0x008C8F80
// FUNCTION: fn_11974
UISkillDescribeWnd::UISkillDescribeWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_text(nullptr) {
}

// OFFSET: 0x008C9430
// FUNCTION: fn_11981
UITipboxWnd::UITipboxWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), unknown_00B4(0), unknown_00B8(0),
      unknown_00BC(0), m_pageText(nullptr), m_pageTotalText(nullptr), unknown_00D0(0), unknown_00D4(0), unknown_00D8(0), m_buildTime(0), m_frameTime(0),
      unknown_00FC(false), unknown_0100(0) {
  this->unknown_00C0 = 0;
  this->unknown_00C4 = 0;
}

// OFFSET: 0x008C86B0
// FUNCTION: fn_11958
UINotifyItemObtainWnd::UINotifyItemObtainWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_identified(false) {
}

// OFFSET: 0x008C7CB0
// FUNCTION: fn_11943
UIItemCollectionWnd::UIItemCollectionWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_helpButton(nullptr) {
  this->m_placement = nullptr;
  this->m_item.Reset();
  this->unknown_0238 = 0;
  this->unknown_0218 = -1;
  this->unknown_021A = -1;
  this->unknown_021C = -1;
  this->unknown_0236 = 0xFFFF;
  this->m_previewTick = 0;
  this->m_closeButton = nullptr;
  this->m_compare = 0;
  this->unknown_01FC = 0;
  this->m_moveInfo = nullptr;
  this->unknown_023C = 0;
  this->m_extraHeight = 0;
}

// OFFSET: 0x008C7570
// FUNCTION: fn_11933
UICombinedCardItemCollectionWnd::UICombinedCardItemCollectionWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIItemCollectionWnd(windows, frames), m_cardPlacement(nullptr) {
}

// OFFSET: 0x008C75D0
// FUNCTION: fn_11934
UICombinedCardItemCollection_ComparisonWnd::UICombinedCardItemCollection_ComparisonWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIItemCollectionWnd(windows, frames) {
}

// OFFSET: 0x008C7E40
// FUNCTION: fn_11944
UIItemCollection_ComparisonWnd::UIItemCollection_ComparisonWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIItemCollectionWnd(windows, frames) {
}

// OFFSET: 0x008C6FA0

// OFFSET: 0x008C7070
// FUNCTION: fn_11927
UICardItemIllustWnd::UICardItemIllustWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

// OFFSET: 0x008C8390
// FUNCTION: fn_11953
UIMenuWnd::UIMenuWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_menuId(10041), m_selected(-1), m_itemHeight(14), unknown_00CC(0), unknown_00DC(0), m_controls(&controls) {
}

// OFFSET: 0x008C85A0
// FUNCTION: fn_11956
UIMonsterDisplayWnd::UIMonsterDisplayWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

// OFFSET: 0x008C8900
// FUNCTION: fn_11963
UIPartyInvitationToWnd::UIPartyInvitationToWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIPartySettingWnd(windows, frames, controls) {
}

// OFFSET: 0x008C8980
// FUNCTION: fn_11964
UIPartySettingWnd::UIPartySettingWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
  this->m_mode = 0;
  this->m_placement = nullptr;
}

// OFFSET: 0x008C8AB0
// FUNCTION: fn_11967
UIPetTamingDeceiveWnd::UIPetTamingDeceiveWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_delay(1), m_state(0), m_tick(0), m_frame(-1), m_maxDelay(10000), m_started(0) {
  this->m_finishTime = 0xFFFFFFFF;
}

// OFFSET: 0x008C9060
// FUNCTION: fn_11975
UISkillNameChangeWnd::UISkillNameChangeWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

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

// OFFSET: 0x008FE310
// FUNCTION: fn_12352
int UIJoinGuildAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    this->m_acceptance->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);
    if (a == 184) {
      this->m_frameContext->windows.CloseWindow(70);
      this->m_frameContext->windows.CloseWindow(214);
      this->m_frameContext->windows.CloseWindow(213);
      this->m_frameContext->windows.CloseWindow(212);
      this->m_context->mode->SendModeMessage(114, this->identity, 1, 0, 0);
    } else if (a == 185) {
      this->m_frameContext->windows.CloseWindow(70);
      this->m_context->mode->SendModeMessage(114, this->identity, 0, 0, 0);
    }
    return 0;
  }

  if (message == 73) {
    char name[260] = {};
    strcpy_s(name, sizeof(name), reinterpret_cast<const char*>(b));
    if (g_forbiddenWords.Contains(name) == true) {
      std::string masked;
      g_forbiddenWords.Mask(name, &masked);
    }

    this->identity = static_cast<std::uint32_t>(a);
    std::string text = "(";
    text += name;
    text += ")";
    text += LookupMessage(377);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }

  if (message == 123) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    std::int16_t marker;
    std::memcpy(&marker, bytes, sizeof(marker));
    unsigned offset = 6;

    if (marker == 0x5528) {
      while (static_cast<unsigned>(b) > offset) {
        std::int16_t kind;
        std::uint32_t size;
        std::memcpy(&kind, bytes + offset, sizeof(kind));
        std::memcpy(&size, bytes + offset + 2, sizeof(size));
        offset += 6;

        if (kind == 0x5529) {
          break;
        }

        if (kind == 0x552A) {
          char name[260] = {};
          std::memcpy(name, bytes + offset, size);
          offset += size;
          this->SendMsg(nullptr, 73, 0, reinterpret_cast<std::intptr_t>(name), 0, 0);
        } else {
          offset += size;
        }
      }
    }

    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
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

// OFFSET: 0x00904960
// FUNCTION: fn_12379
int UISkillDescribeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 201) {
      g_windowMgr.CloseWindow(46);
    } else if (a == 343) {
      UIWindow* window = g_windowMgr.EnsureWindow(10030);
      if (window) {
        static_cast<CUISkillDelayInfo*>(window)->SetSkill(this->m_skillId);
        window->PlaceBeside(this);
        WindowOrdering{g_windowMgr.rendered, *reinterpret_cast<WindowManagerWindows*>(&g_windowMgr), reinterpret_cast<const int&>(g_windowMgr.modal), g_windowMgr.window_4F174}.Promote(window);
      }
    }
    return 0;
  }
  if (message != 61) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  if (d != 0) {
    this->m_closeButton->Move(-100, -100);
  } else {
    this->m_closeButton->Move(this->geometry.width - 14, 3);
  }

  CSkillInfo skill;
  skill.id = static_cast<std::uint32_t>(a);
  if (this->m_skillId == static_cast<int>(a)) {
    return -1;
  }
  if (g_windowMgr.LookupWindow(10030)) {
    g_windowMgr.CloseWindow(10030);
  }

  this->unknown_00EC = this->unknown_00D4 = skill.DisplayName(*g_luaScript);
  this->m_skillId = static_cast<int>(skill.id);
  {
    ClientSkills skills{
      g_session.m_skillBundle,
      g_session.m_homunSkills,
      g_session.m_mercSkills,
      g_session.m_additionalSkills,
      reinterpret_cast<const int&>(g_session.m_jobFlags),
      g_activeSkillEffects
    };
    CSkillInfo owned = skills.Lookup(static_cast<std::uint32_t>(a));
    if (owned.available != 0 && owned.unknown_0014 != 0 && owned.level != 0) {
      char cost[64];
      if (skill.UseApAmount(*g_luaScript, owned.level) != 0) {
        std::sprintf(cost, " (Sp : %d, Ap : %d)", owned.unknown_0014, owned.unknown_0034);
      } else {
        std::sprintf(cost, " (Sp : %d)", owned.unknown_0014);
      }
      this->unknown_00EC.append(cost, std::strlen(cost));
    }
    bool passive = false;
    g_luaScript->Call("IsPassiveSkill", "d>b", static_cast<int>(a), &passive);
    this->m_delayButton->Enable(!passive);
    this->m_text->Clear();
  }

  this->unknown_00BC = std::string("유저인터페이스\\item\\") + std::string(skill.IdName(*g_luaScript)) + ".bmp";
  this->m_viewer->Clear();

  const char* description = nullptr;
  if (g_luaScript->Call("GetSkillDescript", "ddd>s", g_session.m_job, static_cast<int>(a), b == 0 && c == 0 && d == 1, &description)) {
    const std::string text = description;
    if (!text.empty()) {
      if (FindShopId(g_session, std::string()) != -1) {
        const int delay = g_session.GetShopEntry(1, this->m_skillId).first;
        if (delay != 0) {
          const std::string line = FormatOwnedString(LookupMessage(4001), static_cast<int>(delay * 0.1), delay % 10);
          this->m_text->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(line.c_str()), 0, 0, 1);
        }
      }
      const std::vector<std::string> lines = Split(text, "\r\n");
      std::vector<char> buffer;
      for (std::size_t index = 0; index < lines.size(); ++index) {
        buffer.resize(lines[index].size() + 4);
        StringFormat(buffer.data(), buffer.size(), static_cast<std::size_t>(-1), "%s%s", g_serverLanguage->Prefix(true), lines[index].c_str());
        UIWindow* target = this->m_text->lines.size() < 2 ? static_cast<UIWindow*>(this->m_text) : static_cast<UIWindow*>(this->m_viewer);
        target->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(buffer.data()), 0, 1, 1);
      }
    }
  } else {
    this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>("Unknown Skill"), 33, 0, 0);
  }

  const int textHeight = 18 * static_cast<int>(this->m_text->lines.size());
  this->m_text->Resize(this->m_text->geometry.width, textHeight);
  this->m_viewer->Move(this->m_viewer->geometry.x, this->m_text->geometry.height + this->m_text->geometry.y);
  int viewerHeight = 18 * static_cast<int>(this->m_viewer->lines.size()) + 4;
  int total = viewerHeight + textHeight;
  if (total > g_renderer->viewport.height - 50) {
    total = g_renderer->viewport.height - 50;
    viewerHeight = total - this->m_text->geometry.height;
  }
  this->m_viewer->Resize(this->m_viewer->geometry.width, viewerHeight);
  this->Resize(this->geometry.width, total + 6);
  this->Invalidate();
  WindowOrdering{g_windowMgr.rendered, *reinterpret_cast<WindowManagerWindows*>(&g_windowMgr), reinterpret_cast<const int&>(g_windowMgr.modal), g_windowMgr.window_4F174}.Promote(this);
  return 0;
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
// FUNCTION: fn_11978
UIStatusWnd::UIStatusWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
  this->geometry.unknown_0030 = 0;
  this->m_info = nullptr;
  this->m_titleHeight = 17;
  this->m_offsetY = 11;
  this->m_offsetX = 16;
}

// OFFSET: 0x00906DE0
// FUNCTION: fn_12385
int UITipOfTheDayWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 201:
      g_windowMgr.CloseWindow(38);
      break;
    case 213: {
      this->m_showTipsAtStartup = static_cast<int>(b);
      g_window315Enabled = static_cast<int>(b);
      HKEY key;
      DWORD disposition = 0;
      if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
        RegSetValueExA(key, "SHOWTIPSATSTARTUP", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_showTipsAtStartup), sizeof(this->m_showTipsAtStartup));
        RegCloseKey(key);
      }
      break;
    }
    case 218:
      if (g_tipOfTheDay.Size() != 0) {
        ++this->m_tipIndex;
        if (this->m_tipIndex >= g_tipOfTheDay.Size()) {
          this->m_tipIndex = 0;
        }
        this->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(&g_tipOfTheDay.GetTip(this->m_tipIndex)), 0, 0, 0);
      }
      break;
    case 246:
      if (g_tipOfTheDay.Size() != 0) {
        if (--this->m_tipIndex < 0) {
          this->m_tipIndex = g_tipOfTheDay.Size() - 1;
        }
        this->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(&g_tipOfTheDay.GetTip(this->m_tipIndex)), 0, 0, 0);
      }
      break;
    }
    return 0;

  case 29: {
    const auto& tip = *reinterpret_cast<const StringList*>(a);
    this->m_textViewer->Clear();
    this->m_textViewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(175)), 38, 0, 0);
    for (unsigned i = 0; i < tip.size(); ++i) {
      this->m_textViewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(tip[i].c_str()), 38, 0, 0);
    }
    return 0;
  }

  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00905830
// FUNCTION: fn_12383
int UIStatusWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  CMode* mode = static_cast<CMode*>(g_charViewMode);
  const int* statCosts = reinterpret_cast<const int*>(g_session.unknown_169C);
  const int statusPoint = *reinterpret_cast<const int*>(g_session.unknown_1654);
  switch (message) {
  case 6:
    switch (a) {
    case 201:
      g_windowMgr.CloseWindow(11);
      break;
    case 202: {
      const int height = this->geometry.height;
      if (height == 17) {
        this->Resize(this->geometry.width, this->m_expandedHeight);
      } else {
        this->m_expandedHeight = height;
        this->Resize(this->geometry.width, 17);
      }
      this->m_frameContext->snapping.Store(this);
      reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this);
      const int delta = this->geometry.height - height;
      if (delta != 0) {
        g_snapMgr.MoveEdge(this, 0, delta, 3, this->m_frameContext->snapping);
      }
      break;
    }
    case 205:
    case 206:
    case 207:
    case 208:
    case 209:
    case 210:
      if (statCosts[a - 205] <= statusPoint) {
        mode->SendMsg(30, static_cast<int>(a) - 192, 1, 0, 0);
        this->SendMsg(this, 35, 0, 0, 0, 0);
      }
      break;
    case 550: {
      const char* normal;
      const char* pressed;
      if (this->m_extended == 1) {
        this->m_extended = 0;
        this->Resize(280, 141);
        normal = "유저인터페이스\\statuswnd\\expand_on_normal.bmp";
        pressed = "유저인터페이스\\statuswnd\\expand_on_press.bmp";
      } else {
        this->m_extended = 1;
        this->Resize(280, 241);
        normal = "유저인터페이스\\statuswnd\\expand_off_normal.bmp";
        pressed = "유저인터페이스\\statuswnd\\expand_off_press.bmp";
      }
      this->m_expandButton->SetBitmap(normal, 0, 0);
      this->m_expandButton->SetBitmap(normal, 1, 0);
      this->m_expandButton->SetBitmap(pressed, 2, 0);
      this->m_expandedHeight = this->geometry.height;
      break;
    }
    case 551:
    case 552:
    case 553:
    case 554:
    case 555:
    case 556:
      if (g_session.GetTraitStat(231) > 0 && reinterpret_cast<const std::int64_t*>(&g_session.unknown_7FC0[0x20])[a - 551] > 0) {
        mode->SendMsg(314, static_cast<int>(a) - 332, 1, 0, 0);
      }
      break;
    default:
      return 0;
    }
    break;
  case 34:
    this->m_info = reinterpret_cast<StatusWndInfo*>(a);
    if (!this->m_info->IsValid()) {
      this->m_info->Reset();
    }
    this->SendMsg(nullptr, 14, 7, this->m_info->width, this->m_info->unknown_0010, 0);
    if (this->m_info->height != this->m_info->unknown_0010) {
      this->SendMsg(nullptr, 6, 202, 0, 0, 0);
    }
    this->SendMsg(nullptr, 35, 0, 0, 0, 0);
    this->Move(this->m_info->x, this->m_info->y);
    this->m_extended = this->m_info->unknown_0018 != 1;
    this->SendMsg(nullptr, 6, 550, 0, 0, 0);
    break;
  case 35:
    if (this->geometry.unknown_0030 == 0) {
      int y = 24;
      for (int i = 0; i < 6; ++i, y += 16) {
        if (statCosts[i] != 0 && statusPoint >= statCosts[i]) {
          this->m_statButtons[i]->Move(78, y);
        } else {
          this->m_statButtons[i]->Move(-100, -100);
        }
      }
      if (g_session.GetTraitStat(231) < 1) {
        for (int i = 0; i < 6; ++i) {
          this->m_extendedButtons[i]->Move(-100, -100);
        }
      } else {
        std::int64_t traitCosts[6];
        std::memcpy(traitCosts, &g_session.unknown_7FC0[0x20], sizeof(traitCosts));
        y = 145;
        for (int i = 0; i < 6; ++i, y += 16) {
          if (traitCosts[i] != 0) {
            this->m_extendedButtons[i]->Move(78, y);
          } else {
            this->m_extendedButtons[i]->Move(-100, -100);
          }
        }
      }
      this->Invalidate();
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x00905F40
// FUNCTION: fn_12384
int UIStorePasswordInputWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  CMode* mode = static_cast<CMode*>(g_charViewMode);
  auto& dialogs = *reinterpret_cast<WindowDialogs*>(&g_windowMgr);
  const TextLocale locale = {};

  if (message == 6) {
    switch (a) {
    case 184:
      switch (this->unknown_00F4) {
      case 0:
        if (std::strlen(this->unknown_00D0) < 9 && std::strlen(this->unknown_00D0) > 3) {
          int password = std::atoi(this->unknown_00D0);
          for (unsigned i = 0; i < std::strlen(this->unknown_00D0); ++i) {
            password += 100000000;
          }
          mode->SendMsg(188, password, g_session.m_storePasswordMode, 0, 0);
          g_windowMgr.CloseWindow(115);
          break;
        }
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        this->unknown_00F4 = 0;
        this->unknown_00F8 = this->unknown_00FC;
        this->unknown_00EC = 0;
        this->unknown_00F0 = 0;
        std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
        std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
        std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
        this->Paint();
        break;

      case 1:
        if (std::strlen(this->unknown_00D0) < 9 && std::strlen(this->unknown_00D0) > 3) {
          this->unknown_00F4 = 2;
          this->unknown_00F8 = this->unknown_0100;
          this->unknown_00EC = 0;
          this->unknown_00F0 = 0;
          std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
          std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
          this->FillRegion(111, 79, 98, 14, 0xFFFFFF);
          break;
        }
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        this->unknown_00F4 = 1;
        this->unknown_00F8 = this->unknown_00FC;
        this->unknown_00EC = 0;
        this->unknown_00F0 = 0;
        std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
        std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
        std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
        this->Paint();
        break;

      case 2:
        if (std::strlen(this->unknown_00D9) < 9 && std::strlen(this->unknown_00D9) > 3) {
          this->unknown_00F8 = this->unknown_0104;
          this->unknown_00F4 = 3;
          this->unknown_00EC = 0;
          this->unknown_00F0 = 0;
          std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
          this->FillRegion(111, 118, 98, 14, 0xFFFFFF);
          break;
        }
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        this->unknown_00F8 = this->unknown_0100;
        this->unknown_00F4 = 2;
        this->unknown_00EC = 0;
        this->unknown_00F0 = 0;
        std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
        std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
        std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
        this->FillRegion(111, 79, 98, 14, 0xFFFFFF);
        break;

      case 3:
        if (std::strlen(this->unknown_00E2) < 9 && std::strlen(this->unknown_00E2) > 3) {
          if (std::strcmp(this->unknown_00D9, this->unknown_00E2) == 0) {
            int password = std::atoi(this->unknown_00D0);
            int newPassword = std::atoi(this->unknown_00E2);
            for (unsigned i = 0; i < std::strlen(this->unknown_00D0); ++i) {
              password += 100000000;
            }
            for (unsigned i = 0; i < std::strlen(this->unknown_00E2); ++i) {
              newPassword += 100000000;
            }
            mode->SendMsg(188, password, g_session.m_storePasswordMode, newPassword, 0);
            g_windowMgr.CloseWindow(115);
          } else {
            dialogs.Confirm(LookupMessage(1016), 0, true, false, 0, "Message", 280, 120, 0);
            this->unknown_00F4 = 2;
            this->unknown_00F8 = this->unknown_0100;
            this->unknown_00EC = 0;
            this->unknown_00F0 = 0;
            std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
            std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
            this->FillRegion(111, 79, 98, 14, 0xFFFFFF);
            this->FillRegion(110, 117, 100, 16, 0xFAE6C8);
          }
          break;
        }
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        this->unknown_00F4 = 3;
        this->unknown_00F8 = this->unknown_0104;
        this->unknown_00EC = 0;
        this->unknown_00F0 = 0;
        std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
        std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
        std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
        this->FillRegion(111, 118, 98, 14, 0xFFFFFF);
        break;
      }
      break;

    case 185:
      g_windowMgr.CloseWindow(115);
      if (mode->cursorType == 9) {
        g_windowMgr.EnsureWindow(277);
      }
      break;

    case 292:
      g_session.m_storePasswordMode = 2;
      this->unknown_0108 = 1;
      this->unknown_00F4 = 1;
      this->unknown_00F8 = this->unknown_00FC;
      this->unknown_00EC = 0;
      this->unknown_00F0 = 0;
      std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
      std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
      std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
      this->Paint();
      break;

    case 293:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "0");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "0");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "0");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 294:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "1");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "1");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "1");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 295:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "2");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "2");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "2");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 296:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "3");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "3");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "3");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 297:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "4");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "4");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "4");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 298:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "5");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "5");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "5");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 299:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "6");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "6");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "6");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 300:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "7");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "7");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "7");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 301:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "8");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "8");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "8");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 302:
      if (++this->unknown_00EC > 8) {
        dialogs.Confirm(LookupMessage(1011), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->unknown_00F4) {
      case 0:
      case 1:
        std::strcat(this->unknown_00D0, "9");
        break;
      case 2:
        std::strcat(this->unknown_00D9, "9");
        break;
      case 3:
        std::strcat(this->unknown_00E2, "9");
        break;
      }
      this->DrawTextAt(g_fontCache, locale, this->unknown_00F0 + 112, this->unknown_00F8, "*", 0, 0, 12, 0, 0, 0);
      this->unknown_00F0 += 5;
      break;

    case 303:
      if (this->unknown_00F4 == 0) {
        this->unknown_00F4 = 0;
        this->unknown_00F8 = this->unknown_00FC;
      } else if (g_session.m_passwordInputAlternate) {
        this->unknown_00F4 = 2;
        this->unknown_00F8 = this->unknown_0100;
      } else {
        this->unknown_00F4 = 1;
        this->unknown_00F8 = this->unknown_00FC;
      }
      this->unknown_00EC = 0;
      this->unknown_00F0 = 0;
      std::memset(this->unknown_00D0, 0, sizeof(this->unknown_00D0));
      std::memset(this->unknown_00D9, 0, sizeof(this->unknown_00D9));
      std::memset(this->unknown_00E2, 0, sizeof(this->unknown_00E2));
      this->Paint();
      break;

    case 304:
      this->SendMsg(this, 6, 184, 0, 0, 0);
      break;
    }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008ECE60
// FUNCTION: fn_12211
void UIBookWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");

  char* title = new char[110];
  std::sprintf(title, "%s ( %d / %d )", this->m_bookName, this->m_page, this->m_pageCount);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, this->geometry.width / 2 - 72, 2, title, 0, 0, 12, 0, 0, 0);

  this->FillRegion(1, titleHeight, this->geometry.width - 2, this->geometry.height - 18, RGB(this->m_red, this->m_green, this->m_blue));
  this->m_viewer->backgroundColor = 0xFF000000u | RGB(this->m_red, this->m_green, this->m_blue);
  this->m_viewer->Invalidate();
}

// OFFSET: 0x008ECF50
// FUNCTION: fn_12212
void UICardItemIllustWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  if (this->m_bitmapPath.size() != 0) {
    this->DrawBitmap(0, 17, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapPath.c_str()))), false);
  }

  ITEM_INFO item;
  item.itemId = this->m_name;
  item.identified = this->unknown_00E4;
  item.unknown_0060 = 0;
  this->DrawFittedItemName(10, 3, &item, 0, 0, 12, 0);
}

// OFFSET: 0x008E8480
// FUNCTION: fn_12198
void UISelectCartWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);

  const int levels[] = {0, 40, 65, 80, 90, 100, 110, 120, 130};
  std::vector<int>& carts = *reinterpret_cast<std::vector<int>*>(&this->unknown_00B8);
  const int level = g_session.m_level;
  carts.clear();
  for (int type = 1; level > levels[type - 1]; ++type) {
    carts.push_back(type);
    if (type >= 9) {
      break;
    }
  }
  if (level > 100) {
    carts.push_back(13);
  }

  int index = 0;
  for (std::vector<int>::iterator cart = carts.begin(); cart != carts.end(); ++cart, ++index) {
    this->AddCart(index, *cart);
  }

  const int count = static_cast<int>(carts.size());
  if (count != 0) {
    this->Resize(this->geometry.width, ((count - 1) / 5 + 1) * 98);
  }
}

// OFFSET: 0x00904810
// FUNCTION: fn_12378
int UISelectCartWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  std::vector<int>& carts = *reinterpret_cast<std::vector<int>*>(&this->unknown_00B8);
  if (message == 6) {
    if (a != 201) {
      const int type = static_cast<int>(a) - 9000;
      std::vector<int>::iterator cart = carts.begin();
      for (;; ++cart) {
        if (cart == carts.end()) {
          return 0;
        }
        if (*cart == type) {
          break;
        }
      }
      g_modeMgr.GetGameMode()->SendMsg(154, type, this->unknown_00B4, 0, 0);
    }
    g_windowMgr.CloseWindow(95);
  } else if (message == 34) {
    this->DeleteChildren();
    this->AddCloseButton(*this->m_controls, "", 0);
    carts.clear();
    this->unknown_00B4 = static_cast<int>(c);
    const char* types = reinterpret_cast<const char*>(a);
    for (int i = 0; i < b; ++i) {
      carts.push_back(types[i]);
    }
    int index = 0;
    for (std::vector<int>::iterator cart = carts.begin(); cart != carts.end(); ++cart, ++index) {
      this->AddCart(index, *cart);
    }
    if (b != 0) {
      this->Resize(this->geometry.width, ((static_cast<int>(b) - 1) / 5 + 1) * 98);
    }
  }
  return 0;
}

// OFFSET: 0x008D3AB0
// FUNCTION: fn_12104
std::vector<unsigned char> UIBabyAcceptWnd2::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(21550, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(21551, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008D4100
// FUNCTION: fn_12106
std::vector<unsigned char> UIChooseSellBuyWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(20850, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20851, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// INLINED
static __forceinline std::vector<unsigned char> BuildAcceptanceReplayRecords(UITextViewer* viewer, std::uint16_t id, int message) {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(id, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }

  char text[260];
  std::memset(text, 0, sizeof(text));
  viewer->CopyText(text, 0);
  const char parentheses[] = {'(', ')', '\0'};
  std::string decoration(parentheses);
  decoration.append(LookupMessage(message));
  {
    const auto record = BuildReplayTaggedRecord(id + 2, text + 1, viewer->TextLength() - decoration.size());
    records.insert(records.end(), record.begin(), record.end());
  }

  {
    const auto record = BuildReplayTaggedRecord(id + 1, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008D4A40
// FUNCTION: fn_12109
std::vector<unsigned char> UIExchangeAcceptWnd::Slot44() {
  return BuildAcceptanceReplayRecords(this->viewer, 21650, 93);
}

// OFFSET: 0x008D3CD0
// FUNCTION: fn_12105
std::vector<unsigned char> UIBabyAcceptWnd::Slot44() {
  return BuildAcceptanceReplayRecords(this->viewer, 21500, 794);
}

// OFFSET: 0x008D5C80
// FUNCTION: fn_12113
std::vector<unsigned char> UIJoinFriendAcceptWnd::Slot44() {
  return BuildAcceptanceReplayRecords(this->viewer, 21600, 818);
}

// OFFSET: 0x008D60B0
// FUNCTION: fn_12114
std::vector<unsigned char> UIJoinGuildAcceptWnd::Slot44() {
  return BuildAcceptanceReplayRecords(this->viewer, 21800, 377);
}

// OFFSET: 0x008D64E0
// FUNCTION: fn_12115
std::vector<unsigned char> UIJoinPartyAcceptWnd::Slot44() {
  return BuildAcceptanceReplayRecords(this->viewer, 21750, 94);
}

// OFFSET: 0x008D7110
// FUNCTION: fn_12118
std::vector<unsigned char> UIStarPlaceAcceptWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(21700, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(21702, &this->m_place, sizeof(this->m_place));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(21701, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008D4600
// FUNCTION: fn_12108
std::vector<unsigned char> UIChooseWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(21900, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }

  const int count = static_cast<int>(this->m_list->lines.size());
  for (int index = 0; index < count; ++index) {
    const std::string& line = this->m_list->lines[index];
    const auto record = BuildReplayTaggedRecord(21903, line.c_str(), line.size());
    records.insert(records.end(), record.begin(), record.end());
  }

  {
    const auto record = BuildReplayTaggedRecord(21901, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// INLINED
static __forceinline std::vector<unsigned char> BuildItemListReplayRecords(const std::list<ITEM_INFO>& items, std::uint16_t id, std::int16_t packetId) {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(id, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }

  std::vector<unsigned char> packet(items.size() * 2 + 4, 0);
  std::int16_t* fields = reinterpret_cast<std::int16_t*>(packet.data());
  fields[0] = packetId;
  fields[1] = static_cast<std::int16_t>(packet.size());
  std::int16_t* index = fields + 2;
  for (std::list<ITEM_INFO>::const_iterator item = items.begin(); item != items.end(); ++item) {
    *index++ = static_cast<std::int16_t>(item->unknown_0000[1]);
  }
  {
    const auto message = BuildReplayTaggedRecord(id + 3, packet.data(), packet.size());
    records.insert(records.end(), message.begin(), message.end());
  }

  {
    const auto record = BuildReplayTaggedRecord(id + 1, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008D59B0
// FUNCTION: fn_12112
std::vector<unsigned char> UIItemIdentifyWnd::Slot44() {
  return BuildItemListReplayRecords(this->m_items, 22300, 0x177);
}

// OFFSET: 0x008D56E0
// FUNCTION: fn_12111
std::vector<unsigned char> UIItemCompositionWnd::Slot44() {
  return BuildItemListReplayRecords(this->m_items, 22350, 0x17B);
}

// OFFSET: 0x008D6910
// FUNCTION: fn_12116
std::vector<unsigned char> UIMakingArrowListWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(22100, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }

  for (std::list<ITEM_INFO>::iterator item = this->m_items.begin(); item != this->m_items.end(); ++item) {
    const int itemId = std::atoi(item->itemId.c_str());
    const auto record = BuildReplayTaggedRecord(22103, &itemId, sizeof(itemId));
    records.insert(records.end(), record.begin(), record.end());
  }

  {
    const auto record = BuildReplayTaggedRecord(22102, &this->unknown_00F0, sizeof(this->unknown_00F0));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(22101, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008EF5D0
// FUNCTION: fn_12234
void UINotifyItemObtainWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  if (this->m_itemId.size() != 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    DrawItemIconById(*this, descriptions, 13, 5, this->m_itemId.c_str(), this->m_identified);
  }

  const int kind = this->m_kind;
  const int width = this->geometry.width - 49;
  char text[256];
  char* highlight = nullptr;
  char* cursor = text;
  std::vector<char*> breaks;
  std::size_t remaining = sizeof(text);
  int highlightColor;
  this->BuildItemName(&this->m_item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);

  const int written = sprintf_s(cursor, remaining, LookupMessage(696), this->m_item.quantity);
  if (written > 0) {
    remaining -= written;
    cursor += written;
    breaks.push_back(cursor);
  }

  this->DrawItemNameSegments(kind == 0 ? 41 : 13, 10, width, breaks, text, highlight, highlightColor, 0, 0, 12);
}

// OFFSET: 0x008EB010
// FUNCTION: fn_12207
void UIWhisperWnd::Slot15(int, int) {
  this->m_message0 = 184;

  this->m_resizer = new UIResizer(*this->m_context);
  this->m_resizer->SetSize(16, 16);
  this->m_resizer->SetDirection(7);
  this->m_resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->m_resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->m_resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->m_resizer);

  this->m_edit = new UIChatEditCtrl();
  this->m_edit->SetSize(this->geometry.width - 30, 16);
  this->m_edit->Move(3, this->geometry.height - 18);
  this->m_edit->maximum = 70;
  this->m_edit->Background(255, 0, 255);
  this->m_edit->TextColor(RGB(255, 255, 255), 0xFFFFFFFF);
  this->AddChild(this->m_edit);
  InputFocus{g_windowMgr.hoverWindow}.Set(this->m_edit);
  this->m_edit->SetId(490);

  UIChatHisBox2* history = new UIChatHisBox2();
  this->m_chatHistory = history;
  history->Configure(0, 0, this->geometry.width - 2, this->geometry.height - 38, 1);
  this->m_chatHistory->Move(1, 17);
  this->m_chatHistory->SetId(184);
  static_cast<UIChatHisBox2*>(this->m_chatHistory)->backgroundRed = 255;
  static_cast<UIChatHisBox2*>(this->m_chatHistory)->backgroundGreen = 0;
  static_cast<UIChatHisBox2*>(this->m_chatHistory)->backgroundBlue = 255;
  this->AddChild(this->m_chatHistory);

  this->m_closeButton = this->AddCloseButton("/q", 0);
}

// OFFSET: 0x008F6A30
// FUNCTION: fn_12324
int UIAutoMessageWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  if (a == 184) {
    reinterpret_cast<std::string&>(g_session.unknown_56B8[4]) = this->m_edit->Text();
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(942)), 0x64FFFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs&>(g_windowMgr).RestoreInput();
    g_windowMgr.CloseWindow(116);
  }
  return 0;
}

// OFFSET: 0x008F79C0
// FUNCTION: fn_12328
int UICardItemIllustWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a != 201) {
      return 0;
    }
  } else {
    if (message != 62) {
      return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
    }

    const char* id = reinterpret_cast<const char*>(a);
    this->unknown_00E4 = b != 0;
    const char* name = FindItemCardName(std::atoi(id)).name;
    if (IsItemCard(std::atoi(id)) && this->m_name != id) {
      this->m_name = id;
      this->m_bitmapPath = "유저인터페이스\\cardBmp\\";
      this->m_bitmapPath += name;
      this->m_bitmapPath += ".bmp";
      this->Invalidate();
      reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this);
      return 0;
    }
  }

  g_windowMgr.CloseWindow(47);
  return 0;
}

// OFFSET: 0x008EFFF0
// FUNCTION: fn_12248
void UISecondPasswordInputWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, LookupMessage(1899), 0);
  this->DrawPanelBackground(titleHeight, 540, "유저인터페이스\\basic_interface\\2ndpassword_bg.bmp");
  this->DrawButtonBar(2, 0, 1);

  std::uint32_t boxColor;
  std::uint32_t textColor;
  std::uint32_t shadowColor;
  if (!g_session.m_passwordInputAlternate) {
    boxColor = 0xFAC8AA;
    shadowColor = 0;
    textColor = 0xE6E6E6;
  } else {
    boxColor = 0xE6E6E6;
    textColor = 0xF0F0F0;
    shadowColor = 0xE6E6E6;
  }
  this->DrawEmboldenedText(414, 76, LookupMessage(1012), textColor, shadowColor, 0, 0, 12);
  this->FillRegion(414, 89, 100, 16, boxColor);
  this->FillRegion(415, 90, 98, 14, 0xFFFFFF);

  if (!g_session.m_passwordInputAlternate && this->unknown_0108 == 0) {
    this->DrawEmboldenedText(414, 112, LookupMessage(1013), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(414, 127, 100, 16, 0xE6E6E6);
    this->DrawEmboldenedText(414, 152, LookupMessage(1014), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(414, 166, 100, 16, 0xE6E6E6);
  } else {
    this->DrawEmboldenedText(414, 112, LookupMessage(1013), 0xE6E6E6, 0, 0, 0, 12);
    this->FillRegion(414, 127, 100, 16, 0xFAE6C8);
    this->DrawEmboldenedText(414, 152, LookupMessage(1014), 0xE6E6E6, 0, 0, 0, 12);
    this->FillRegion(414, 166, 100, 16, 0xFAE6C8);
    if (g_session.m_passwordInputAlternate) {
      this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
    }
  }

  if (this->unknown_00F4 == 2 || this->unknown_00F4 == 3) {
    if (this->unknown_00F4 == 3) {
      this->FillRegion(415, 167, 98, 14, 0xFFFFFF);
    }
    this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
  }

  for (std::size_t index = 0, x = 416; index < std::strlen(this->unknown_00D0); ++index, x += 5) {
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, static_cast<int>(x), this->unknown_00FC, "*", 0, 0, 12, 0, 0, 0);
  }
  for (std::size_t index = 0, x = 416; index < std::strlen(this->unknown_00D9); ++index, x += 5) {
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, static_cast<int>(x), this->unknown_0100, "*", 0, 0, 12, 0, 0, 0);
  }
  for (std::size_t index = 0, x = 416; index < std::strlen(this->unknown_00E2); ++index, x += 5) {
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, static_cast<int>(x), this->unknown_0104, "*", 0, 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x008F2BC0
// FUNCTION: fn_12260
void UInCashWnd::Paint() {
  if (const auto* background =
        reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\NC_BackGround3.bmp")))) {
    this->DrawBitmap(0, 0, background, false);
  }

  char text[64];
  std::memset(text, 0, sizeof(text));
  char number[64];
  std::memset(number, 0, sizeof(number));
  const int height = this->geometry.height;

  int length = std::sprintf(text, "%d / %d", this->m_page, this->m_pageCount);
  this->DrawRightAlignedText(270, height - 55, text, length, 0, 12, 0, 0);

  length = std::sprintf(text, LookupMessage(1857), FormatGroupedInteger(this->m_cashBalance, number, sizeof(number)));
  this->DrawShadowedText(74, this->geometry.height - 36, text, length, 0, 0xFFFFFF, 0, 12, 0);
  length = std::sprintf(text, LookupMessage(1858), FormatGroupedInteger(this->m_pointBalance, number, sizeof(number)));
  this->DrawShadowedText(74, this->geometry.height - 17, text, length, 0, 0xFFFFFF, 0, 12, 0);

  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 260, this->geometry.height - 36, LookupMessage(1306), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 427, this->geometry.height - 36, "C", 0, 0, 12, 0, 0, 0);
  length = std::sprintf(text, "Total              :      %s", FormatGroupedInteger(this->m_totalPrice, number, sizeof(number)));
  this->DrawShadowedText(260, this->geometry.height - 17, text, length, 0, 0xFFFFFF, 0, 12, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 427, this->geometry.height - 17, "C", 0, 0, 12, 0, 0, 0);

  this->DrawItems();
}

// OFFSET: 0x008F4D50
// FUNCTION: fn_12295
void UIExchangeWnd::Slot19() {
  UIEditCtrl* edit = this->unknown_00C0;
  if (edit == nullptr) {
    return;
  }

  const int length = edit->Length();
  if (length == this->m_moneyLength) {
    return;
  }
  this->m_moneyLength = length;
  if (length == 0) {
    return;
  }

  char text[24];
  std::sprintf(text, "%s", edit->Text());
  if (!g_session.IsNumberWithCommas(text)) {
    reinterpret_cast<WindowDialogs&>(g_windowMgr).Confirm(LookupMessage(602), 0, true, false, 0, "Message", 280, 120, 0);
    edit->SetText(this->m_moneyText.c_str());
    InputFocus{g_windowMgr.hoverWindow}.Set(edit);
    return;
  }

  std::int64_t money = _atoi64(text);
  if (money > 1000000000) {
    reinterpret_cast<WindowDialogs&>(g_windowMgr).Confirm(LookupMessage(2467), 2, true, false, 0, "Message", 280, 120, 0);
    FormatGroupedInteger(1000000000, text, sizeof(text));
    edit->SetText(text);
    money = 1000000000;
  } else {
    if (money < 0) {
      money = 0;
    }
    FormatGroupedInteger(money, text, sizeof(text));
    edit->SetText(text);
    this->m_moneyText = edit->Text();
  }

  const PriceColors colors = PriceColor(money);
  edit->TextColor(RGB(GetRValue(colors.primary), GetGValue(colors.primary), GetBValue(colors.primary)), colors.secondary);
  this->m_moneyLength = edit->Length();
  edit->SetCursor(edit->Length());
}

// OFFSET: 0x008EE2B0
// FUNCTION: fn_12224
void UIItemCollectionWnd::Paint() {
  int height = this->geometry.height;
  if (this->m_item.identified == 1 && this->unknown_01FC != 0) {
    height -= this->m_extraHeight;
  }
  const int bottom = this->m_compare == 1 ? height - 29 : height;

  const char* background;
  if (this->m_kind == 234 || this->m_kind == 250) {
    this->DrawSkinBox("유저인터페이스\\basic_interface\\sysbox_comparison_", 0, 0, this->geometry.width, bottom, true);
    background = "유저인터페이스\\basic_interface\\collection_Comparison_bg.bmp";
  } else if (this->m_item.unknown_0088 > 0) {
    const int width = this->geometry.width;
    this->DrawSkinBox(FormatOwnedString("유저인터페이스\\sysbox_g%d_", this->m_item.unknown_0088).c_str(), 0, 0, width, bottom, false);
    this->DrawBitmap(
      0, 0,
      reinterpret_cast<const BitmapImage*>(
        ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\basic_interface\\collection_bg_g%d.bmp", this->m_item.unknown_0088).c_str()))
      ),
      true
    );
    background = nullptr;
  } else {
    this->DrawSkinBox("유저인터페이스\\sysbox_", 0, 0, this->geometry.width, bottom, true);
    background = "유저인터페이스\\basic_interface\\collection_bg.bmp";
  }
  if (background != nullptr) {
    this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(background))), true);
  }

  if (this->unknown_01C4.size() != 0) {
    this->DrawBitmap(10, 11, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(this->unknown_01C4.c_str()))), true);
  }
  this->DrawFittedItemName(90, 10, &this->m_item, this->m_item.identified ? 0 : 0x808080, 0, 12, 0);

  if (this->m_compare == 1) {
    int offset = 0;
    if (this->m_item.identified == 1 && this->unknown_01FC > 0) {
      for (std::map<int, std::list<std::string>>::iterator card = this->m_cardTexts.begin(); card != this->m_cardTexts.end(); ++card) {
        const char* box = this->m_kind == 234 ? "유저인터페이스\\basic_interface\\sysbox_comparison_" : "유저인터페이스\\sysbox_";
        this->DrawSkinBox(box, 0, bottom + offset, this->geometry.width, static_cast<int>(card->second.size()) * 15 + 14, true);
        offset += 8;
        for (std::list<std::string>::iterator line = card->second.begin();;) {
          if (*line == LookupMessage(3886)) {
            this->DrawCenteredText(0, bottom + offset, line->c_str(), this->geometry.width, 0, 12, 0xFF, 0);
            break;
          }
          this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 12, bottom + offset, line->c_str(), 0, 0, 12, 0, 0, 0);
          ++line;
          if (line == card->second.end()) {
            break;
          }
          offset += 15;
        }
        offset += 21;
      }
    }

    if (!this->m_item.HasSpecialCard()) {
      const int top = bottom + offset;
      const char* box = this->m_kind == 234 ? "유저인터페이스\\basic_interface\\sysbox_comparison_" : "유저인터페이스\\sysbox_";
      this->DrawSkinBox(box, 0, top, this->geometry.width, 29, true);
      const int y = top + 2;
      int slot = 0;
      for (int x = 4; x < 104; x += 25, ++slot) {
        const int card = reinterpret_cast<const int*>(this->m_item.unknown_001C)[slot];
        if (card < 1) {
          const BitmapImage* bitmap = reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\empty_card_slot.bmp")));
          if (this->m_kind == 234) {
            const char* name = slot < ItemSlotsCount(this->m_item) ? "유저인터페이스\\basic_interface\\comparison_empty_card_slot.bmp"
                                                                   : "유저인터페이스\\basic_interface\\comparison_disable_card_slot.bmp";
            bitmap = reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(name)));
          } else if (ItemSlotsCount(this->m_item) <= slot) {
            bitmap = reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\disable_card_slot.bmp")));
          }
          this->DrawBitmap(x, y, bitmap, true);
        } else {
          ItemDescriptions descriptions{g_itemDescriptions};
          DrawItemIconById(*this, descriptions, x, y, FormatOwnedString("%d", card).c_str(), 0);
        }
      }
    }
  }

  if (FindItemMoveInfo(std::atoi(this->m_item.itemId.c_str())).itemId < 1) {
    this->m_moveInfo->Move(-1000, -1000);
  } else {
    this->m_moveInfo->Move(this->m_viewer->geometry.x, this->m_viewer->geometry.y + this->m_viewer->geometry.height);
  }
}

// OFFSET: 0x008DB1F0
// FUNCTION: fn_12158
void UIAutoMessageWnd::Slot15(int, int) {
  this->m_edit = new UIEditCtrl(*this->m_context, *reinterpret_cast<UIEditContext*>(nullptr));
  this->m_edit->SetSize(this->geometry.width - 77, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 100;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->SetText(reinterpret_cast<const std::string&>(g_session.unknown_56B8[4]).c_str());
  this->m_edit->SelectAll();
  this->AddChild(this->m_edit);

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;
}

// OFFSET: 0x008EC840
// FUNCTION: fn_12209
void UInCash_CallWnd::Slot15(int, int) {
  const int ids[] = {192};
  const int positions[] = {0};
  const char* names[] = {"NC_CashShop"};
  const std::string prefix("유저인터페이스\\basic_interface\\");
  for (int index = 0; index < 1; ++index) {
    auto* button = new UIBitmapButton(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
    button->SetBitmap((prefix + names[index] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(0, positions[index]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }
}

// OFFSET: 0x008D7790
// FUNCTION: fn_12123
std::string CartTypeName(int type) {
  std::string names[] = {
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
  return std::move(names[type]);
}

// OFFSET: 0x008D7460
// FUNCTION: fn_12119
int CSession::GetTraitStat(int type) {
  std::map<int, int>& traitStats = reinterpret_cast<std::map<int, int>&>(this->unknown_7FC0);
  return traitStats[type];
}

// OFFSET: 0x008C7F00
// FUNCTION: fn_11946
UIItemIdentifyWnd::UIItemIdentifyWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_placement(nullptr) {
}

// OFFSET: 0x008C7EA0
// FUNCTION: fn_11945
UIItemCompositionWnd::UIItemCompositionWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIItemIdentifyWnd(windows, frames), m_compositionPlacement(nullptr) {
}

// OFFSET: 0x008C90E0
// FUNCTION: fn_11976
UISpellListWnd::UISpellListWnd(UIWindowContext& windows, UIFrameContext& frames) : UIItemIdentifyWnd(windows, frames) {
  this->unknown_00FC = 0;
}

// OFFSET: 0x00900B10
// FUNCTION: fn_12370
int UIPetEggListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets = {
      g_packetLengths,
      replay.baseFrame,
      replay.started,
      replay.frame,
      replay.header.key0,
      replay.header.key1,
      replay.header.options.recordRestricted,
      replay.file,
      replay.fileOffset,
      replay.records,
      replay.cipherIndex,
      replay.cipherBase,
      replay.cipherKey,
    };
    WindowResponseRecorder recorder{replay.state, packets};
    recorder.Record(this->geometry.id, static_cast<int>(argument1), argument2, this->m_selected, argument4);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(argument3);
    }
    if (argument1 == 184) {
      int found = 0;
      ITEM_INFO item = this->GetItem(this->m_selected, &found);
      if (found) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(146, item.unknown_0000[1], 0, 0, 0);
      }
    } else if (argument1 == 185) {
      g_windowMgr.CloseWindow(90);
    }
    return 0;
  }
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scroll;
    if (message == 9) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == 10) {
      this->m_scroll = previous + this->m_rows - 1;
    } else {
      this->m_scroll = previous + static_cast<int>(argument1);
    }
    if (previous != this->m_scroll) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scroll - previous;
  }
  case 31:
    this->m_items.push_back(LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(argument2)));
    this->SendMsg(nullptr, 60, 0, 0, 0, 0);
    return 0;
  case 60:
    this->UpdateScroll();
    this->Invalidate();
    return 0;
  default:
    return 0;
  }
}

// OFFSET: 0x008C8330
// FUNCTION: fn_11952
UIMakingArrowListWnd::UIMakingArrowListWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIItemIdentifyWnd(windows, frames) {
}

// OFFSET: 0x008C8A50

// OFFSET: 0x008D4320
// FUNCTION: fn_12107
std::vector<unsigned char> UIChooseWarpWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(21950, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  for (auto choice = this->m_choices.begin(); choice < this->m_choices.end(); ++choice) {
    const auto record = BuildReplayTaggedRecord(21953, choice->c_str(), choice->size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(21951, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008D4E70
// FUNCTION: fn_12110
std::vector<unsigned char> UIExchangeWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(20600, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20603, &this->unknown_0108, sizeof(this->unknown_0108));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20604, &this->unknown_010C, sizeof(this->unknown_010C));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20602, this->unknown_00F0.c_str(), this->unknown_00F0.size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20605, &this->unknown_00CC, sizeof(this->unknown_00CC));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20606, &this->unknown_00D0, sizeof(this->unknown_00D0));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20607, &this->unknown_00D4, sizeof(this->unknown_00D4));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20608, &this->unknown_00D8, sizeof(this->unknown_00D8));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20609, &this->unknown_00DC, sizeof(this->unknown_00DC));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(20601, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008D6D40
// FUNCTION: fn_12117
std::vector<unsigned char> UISayDialogWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(22500, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  const UITagTextViewer* viewer = static_cast<UITagTextViewer*>(this->m_viewer);
  for (auto text = viewer->m_texts.begin(); text != viewer->m_texts.end(); ++text) {
    const auto record = BuildReplayTaggedRecord(22503, text->c_str(), text->size());
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(22504, &this->unknown_00D4, sizeof(this->unknown_00D4));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(22501, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008E30B0
// FUNCTION: fn_12180
void UILoadingWnd::Slot15(int, int) {
  // GLOBAL: gl_25392
  static int s_loadingScreenIndex;

  this->m_progressBar = new UIBarGraph2(*this->m_context);
  this->m_progressBar->SetSize(240, 15);
  this->m_progressBar->Move(0, 0);
  this->m_progressBar->SetValue(0.0f);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 22, 2, red, green, blue);
  this->m_progressBar->m_fillRed = red;
  this->m_progressBar->m_fillGreen = green;
  this->m_progressBar->m_fillBlue = blue;
  this->AddChild(this->m_progressBar);

  const int count = static_cast<int>(g_loadingScreens.size());
  char name[256];
  if (g_localMapMove == 1) {
    std::strcpy(name, "유저인터페이스\\loading_airplane.jpg");
  } else if (!g_randomLoadingScreen) {
    if (s_loadingScreenIndex < count) {
      std::strcpy(name, g_loadingScreens[s_loadingScreenIndex++].c_str());
    } else {
      s_loadingScreenIndex = 0;
      std::strcpy(name, "유저인터페이스\\loading00.jpg");
    }
  } else if (count != 0) {
    std::strcpy(name, g_loadingScreens[std::rand() % count].c_str());
  } else {
    std::strcpy(name, "유저인터페이스\\loading00.jpg");
  }

  ResourceMgr::Instance().Load(Resolve(name));
  g_windowMgr.SetSurface0478(static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name))));
}
// FUNCTION: fn_11966
UIPetEggListWnd::UIPetEggListWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIItemIdentifyWnd(windows, frames) {
}

// OFFSET: 0x008D1E80
// FUNCTION: fn_12096
void UIEquipWnd::DrawEquipSlots() {
  if (this->m_page == 1) {
    const auto* frame = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\equipwin_special.bmp")));
    if (frame) {
      this->DrawBitmap(0, this->m_contentTop + 14, &frame->image, false);
    }
  }

  const int rowOffsets[4] = {0, 26, 52, 78};
  int iconX[2] = {6, 251};
  int nameX[2] = {28, 167};
  int shopId = FindShopId(g_session, std::string());
  if (g_session.GetShopType(shopId)) {
    shopId = -1;
  }

  for (unsigned slot = 0; slot < 10; ++slot) {
    ITEM_INFO item;
    if (this->m_page == 1) {
      if (this->m_viewOther == 0) {
        item = g_session.GetItem2B50(slot);
      } else {
        item = g_session.GetItem3500(slot);
      }
    } else if (this->m_viewOther == 0) {
      item = g_session.GetItem17F0(slot);
    } else {
      item = g_session.GetItem21A0(slot);
    }

    if (item.unknown_0000[1] == 0) {
      continue;
    }

    if (item.quantity == 1) {
      ItemDescriptions descriptions{g_itemDescriptions};
      const unsigned row = slot & 3;
      if (slot < 8) {
        DrawItemIcon(*this, ResourceMgr::Instance(), descriptions, iconX[slot / 4], this->m_contentTop + 44 + row * 26, item, 0);
        this->DrawItemName(nameX[slot / 4] + 5, this->m_contentTop + rowOffsets[row] + 51, 70, &item, 0, 0, 12);
      } else {
        DrawItemIcon(*this, ResourceMgr::Instance(), descriptions, iconX[row], this->m_contentTop + 18, item, 0);
        this->DrawItemName(nameX[slot - 8] + 5, this->m_contentTop + 25, 70, &item, 0, 0, 12);
      }
    }

    if (item.unknown_0088 > 0) {
      if (slot < 8) {
        const auto* grade = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088).c_str())
        ));
        this->DrawBitmap(iconX[slot / 4], this->m_contentTop + 56 + (slot & 3) * 26, grade ? &grade->image : nullptr, true);
      } else {
        const auto* grade = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088).c_str())
        ));
        this->DrawBitmap(iconX[slot & 3], this->m_contentTop + 30, grade ? &grade->image : nullptr, true);
      }
    }

    if (shopId != -1 && g_session.GetShopEntry(0, std::atoi(item.itemId.c_str())).first == 0) {
      const unsigned row = slot & 3;
      const unsigned column = slot < 8 ? slot / 4 : row;
      const auto* banned = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\ban.bmp")));
      const int y = slot < 8 ? this->m_contentTop + (row + 1) * 26 : this->m_contentTop;
      this->DrawBitmap(iconX[column], y + 18, banned ? &banned->image : nullptr, true);
    }
  }
}

// OFFSET: 0x008F6560
// FUNCTION: fn_12318
void UIEquipWnd::RefreshTitleList() {
  this->m_titleList->Clear();
  this->m_titleList->AddLine(LookupMessage(2686), 0);
  for (std::vector<int>::iterator it = g_session.unknown_6160.begin(); it != g_session.unknown_6160.end(); ++it) {
    const std::string title = GetJobTitleString(*it);
    this->m_titleList->AddLine(title.c_str(), *it);
  }
}

// OFFSET: 0x008F5580
// FUNCTION: fn_12304
void UIItemCollectionWnd::Slot32(int x, int y) {
  int slotX;
  int slotY;
  const int index = this->CardSlotAt(x, y, &slotX, &slotY);
  if (index == -1) {
    return;
  }

  ITEM_INFO card;
  int cardId;
  std::memcpy(&cardId, this->m_item.unknown_001C + index * sizeof(cardId), sizeof(cardId));
  card.identified = 1;
  card.unknown_0000[0] = 6;
  card.unknown_0060 = 0;
  card.SetItemId(cardId);
  UIWindow* window = g_windowMgr.sub_A978D0(82);
  window->SendMsg(nullptr, 76, card.identified, 0, 0, 0);
  window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&card), index >= ItemSlotsCount(this->m_item), 0, 0);

  if (UIItemCollectionWnd* comparison = static_cast<UIItemCollectionWnd*>(g_windowMgr.unknown_0220)) {
    int compareId;
    std::memcpy(&compareId, comparison->m_item.unknown_001C + index * sizeof(compareId), sizeof(compareId));
    if (compareId != 0 && compareId != 254 && compareId != 255) {
      ITEM_INFO compareCard;
      compareCard.identified = 1;
      compareCard.unknown_0000[0] = 6;
      compareCard.unknown_0060 = 0;
      compareCard.SetItemId(compareId);
      UIWindow* compareWindow = g_windowMgr.sub_A978D0(250);
      compareWindow->SendMsg(nullptr, 76, compareCard.identified, 0, 0, 0);
      compareWindow->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&compareCard), 1, 0, 0);
    }
  }
}

// OFFSET: 0x008F5790
// FUNCTION: fn_12305
void UIItemCollection_ComparisonWnd::Slot32(int x, int y) {
  int slotX;
  int slotY;
  const int index = this->CardSlotAt(x, y, &slotX, &slotY);
  if (index == -1) {
    return;
  }

  ITEM_INFO card;
  int cardId;
  std::memcpy(&cardId, this->m_item.unknown_001C + index * sizeof(cardId), sizeof(cardId));
  card.identified = 1;
  card.unknown_0000[0] = 6;
  card.unknown_0060 = 0;
  card.SetItemId(cardId);
  UIWindow* window = g_windowMgr.sub_A978D0(82);
  window->SendMsg(nullptr, 76, card.identified, 0, 0, 0);
  window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&card), index >= ItemSlotsCount(this->m_item), 0, 0);
}

// OFFSET: 0x008F3660
// FUNCTION: fn_12270
void UIEquipWnd::Slot25(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::Slot25(x, y);
  }

  if (this->m_viewOther != 0) {
    return;
  }

  ITEM_INFO item = this->ItemIconAt(x, y);
  if (item.unknown_0000[1] == 0) {
    return;
  }

  if (item.quantity > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, this->m_page == 1 ? 24 : 2, item, item.quantity, slots.values[2]);
  }

  if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
    if (g_windowMgr.GetHoverWindow() && item.identified != 0) {
      if (g_windowMgr.GetHoverWindow()->geometry.id == 492) {
        reinterpret_cast<UINpcTextEditDialogWnd*>(g_windowMgr.unknown_0318)->InsertItemTag(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 490) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 493) {
        reinterpret_cast<UIChatEditCtrl*>(g_chatSuggestions->unknown_00BC)->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 491) {
        reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->sub_7FF710(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 494) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      }
    }
  }
}

// OFFSET: 0x008C9880
// FUNCTION: fn_11986
UInCashWnd::UInCashWnd(UIWindowContext& windows, UIFrameContext& frames) : UIItemBaseWnd(windows, frames) {
  this->m_placement = nullptr;
  this->m_tab = 0;
  this->m_selected = 0;
  this->m_pageCount = 0;
  this->m_page = 0;
  this->m_selectedIndex = 0;
  this->m_totalPrice = 0;
  this->m_firstOther = 0;
  this->m_pointBalance = 0;
  this->m_cashBalance = 0;
  this->m_rowHeight = 106;
  this->m_listTop = 56;
  this->m_listHeight = 32;
}

// OFFSET: 0x008F33F0
// FUNCTION: fn_12267
void UInCashWnd::Slot26(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.unknown_0000[1] == 0 || this->m_selected >= 0) {
    return;
  }

  g_modeMgr.GetGameMode();
  this->m_selected = ~this->m_selected;
  reinterpret_cast<ClientItemLists&>(g_session).RemoveItem1720ByName(item);
  this->m_totalPrice -= item.quantity * item.unknown_0014[1];
  this->m_otherItems.clear();

  const SessionMessengerContext& lists = reinterpret_cast<const SessionMessengerContext&>(g_session);
  const int count = lists.GetList2Size();
  for (int index = 0; index < count; ++index) {
    ITEM_INFO other = g_session.GetItem1720At(index);
    if (other.unknown_0000[1] != 0) {
      this->m_otherItems.push_back(other);
    }
  }

  if (lists.GetList2Size() == 3) {
    this->m_firstOther = 0;
    this->m_otherScroll->SetPosition(0);
    this->m_otherScroll->SetRange(4, 4, 4);
    this->m_otherScroll->Invalidate();
    this->m_otherScroll->Show(0);
  } else if (lists.GetList2Size() > 3 && lists.GetList2Size() < 7) {
    this->m_otherScroll->SetRange(4, 4, lists.GetList2Size() + 1);
    if (this->m_firstOther > lists.GetList2Size() - 3) {
      this->m_firstOther = lists.GetList2Size() - 3;
      this->m_otherScroll->SetPosition(this->m_firstOther);
    }
    this->m_otherScroll->Invalidate();
  }
}

// OFFSET: 0x008FEB00
// FUNCTION: fn_12355
int UIMakingArrowListWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets{g_packetLengths, replay.baseFrame, replay.started, replay.frame, replay.header.key0, replay.header.key1,
                                 replay.header.options.recordRestricted, replay.file, replay.fileOffset, replay.records, replay.cipherIndex,
                                 replay.cipherBase, replay.cipherKey};
    WindowResponseRecorder{replay.state, packets}.Record(this->geometry.id, static_cast<int>(a), b, c, this->m_selected);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(d);
      this->Invalidate();
    }
    if (a == 184) {
      int found;
      ITEM_INFO item = this->GetItem(this->m_selected, &found);
      if (found != 0) {
        if (this->unknown_00F0 == 2) {
          static_cast<CMode*>(g_charViewMode)->SendMsg(0x99, item.GetItemId(), 0, 0, 0);
        } else {
          static_cast<CMode*>(g_charViewMode)->SendMsg(0xCF, item.GetItemId(), this->unknown_00F0, 0, 0);
        }
        g_windowMgr.CloseWindow(0x5E);
      }
    } else if (a == 185) {
      if (this->unknown_00F0 == 2) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(0x99, -1, 0, 0, 0);
      } else {
        static_cast<CMode*>(g_charViewMode)->SendMsg(0xCF, -1, this->unknown_00F0, 0, 0);
      }
      g_windowMgr.CloseWindow(0x5E);
    }
    break;
  }
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scroll;
    if (message == 9) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == 10) {
      this->m_scroll = previous + this->m_rows - 1;
    } else {
      this->m_scroll = previous + static_cast<int>(a);
    }
    if (this->m_scroll != previous) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scroll - previous;
  }
  case 0x1F: {
    ITEM_INFO item;
    item.SetItemId(*reinterpret_cast<const int*>(a));
    item.identified = 1;
    this->m_items.push_back(item);
    this->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
    break;
  }
  case 0x22:
    this->unknown_00F0 = static_cast<std::int16_t>(a);
    break;
  case 0x3C:
    this->UpdateScroll();
    this->Invalidate();
    break;
  case 0x4B:
    this->m_items.clear();
    break;
  case 0x7B: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x5654) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        if (header.type == 0x5655) {
          break;
        }
        if (header.type == 0x5656) {
          int value = 0;
          std::memcpy(&value, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, 0x22, static_cast<std::int16_t>(value), 0, 0, 0);
        } else if (header.type == 0x5657) {
          int value = 0;
          std::memcpy(&value, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(&value), 0, 0, 0);
        } else {
          position += header.length;
        }
      }
    }
    break;
  }
  }
  return 0;
}

// OFFSET: 0x008F50E0
// FUNCTION: fn_12301
void UITipboxWnd::Slot19() {
  if (this->unknown_00FC == true) {
    this->unknown_00FC = false;
    const int index = (std::max)(0, g_itemTagOwner->unknown_00B4);
    const std::string text = g_itemTagOwner->m_currentEntry.paragraphs[index];
    std::vector<std::string> lines;
    WrapPlainText(text.c_str(), lines, 1000, static_cast<unsigned short>(g_textCodePage));
    this->unknown_00BC->Clear();
    for (const std::string& line : lines) {
      this->unknown_00BC->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(line.c_str()), 32, 0, 0);
    }
    this->Invalidate();
  }
  if (g_itemTagOwner->HasImages() == true) {
    this->Invalidate();
  }
  if (this->m_frameTime - this->m_buildTime > 299) {
    this->m_buildTime = timeGetTime();
    for (std::size_t index = 0; index < this->m_dots.size(); ++index) {
      if (!this->m_dots[index].visible) {
        this->m_dots[index].visible = true;
        break;
      }
      if (index + 1 == this->m_dots.size()) {
        for (TipBoxDot& dot : this->m_dots) {
          dot.visible = false;
        }
      }
    }
  }
  this->UpdateAnimations();
  this->m_frameTime = timeGetTime();
}

// OFFSET: 0x008F8A40
// FUNCTION: fn_12333
int UIChooseWarpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets{g_packetLengths, replay.baseFrame, replay.started, replay.frame, replay.header.key0, replay.header.key1,
                                 replay.header.options.recordRestricted, replay.file, replay.fileOffset, replay.records, replay.cipherIndex,
                                 replay.cipherBase, replay.cipherKey};
    WindowResponseRecorder{replay.state, packets}.Record(this->geometry.id, static_cast<int>(a), b, c, d);
    if (a == 184) {
      const int selected = this->m_list->selected;
      if (selected != -1) {
        g_modeMgr.GetGameMode()->SendMsg(0x4A, reinterpret_cast<std::intptr_t>(this->m_choices[selected].c_str()), this->unknown_00B8, 0, 0);
        g_windowMgr.CloseWindow(0x27);
      }
    } else if (a == 185) {
      g_modeMgr.GetGameMode()->SendMsg(0x4A, reinterpret_cast<std::intptr_t>("cancel"), this->unknown_00B8, 0, 0);
      g_windowMgr.CloseWindow(0x27);
    }
    return 0;
  }
  case 0x12:
  case 0x13:
    this->m_list->SendMsg(nullptr, message, 0, 0, 0, 0);
    return 0;
  case 0x1B:
    if (a != 0) {
      const char* choice = reinterpret_cast<const char*>(a);
      if (std::string(choice) == "Random") {
        this->m_list->AddLine(LookupMessage(0xD4));
      } else if (std::string(choice) == "cancel") {
        this->m_list->AddLine(LookupMessage(0xDA));
      } else {
        this->m_list->AddLine(g_session.GetMapDisplayName(g_session.GatName2RswName(choice)));
      }
      this->m_choices.emplace_back(choice);
    }
    return 0;
  case 0x3B:
    this->unknown_00B8 = static_cast<int>(a);
    return 0;
  case 0x7B: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x55BE) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        if (header.type == 0x55BF) {
          this->SendMsg(nullptr, 0x1B, reinterpret_cast<std::intptr_t>("cancel"), 0, 0, 0);
          break;
        }
        if (header.type == 0x55C1) {
          char text[260] = {};
          std::memcpy(text, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, 0x1B, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
        } else {
          position += header.length;
        }
      }
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008F26B0
// FUNCTION: fn_12256
void UITipboxWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, LookupMessage(0xCA6), 0);

  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\TipboxBG.bmp")));
  this->DrawBitmap(0, 17, background ? &background->image : nullptr, false);

  if (this->unknown_00B8->geometry.visible == 1) {
    const char* path = g_itemTagOwner->HasSearchResults() ? "유저인터페이스\\tipbox\\img_loading.bmp"
                                                       : "유저인터페이스\\tipbox\\img_NoSearchResults.bmp";
    const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    this->DrawBitmap(1, 17, bitmap ? &bitmap->image : nullptr, false);
  } else {
    CTipBoxMgr* manager = g_itemTagOwner;
    switch (manager->m_image.kind) {
    case 0:
      if (manager->m_currentEntry.unknown_0044 == 5 && manager->m_currentEntry.unknown_0048 == 100) {
        const std::string path = manager->m_currentEntry.unknown_001C + ".bmp";
        const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
        this->DrawBitmap(1, 17, bitmap ? &bitmap->image : nullptr, false);
      } else {
        CTexture* texture = g_texMgr.Load(manager->m_imagePath.c_str(), 0, false, true, false);
        const int sourceHeight = texture->Height();
        const int sourceWidth = texture->Width();
        this->geometry.surface->DrawScaledSurface(
          static_cast<int>(manager->m_image.x), static_cast<int>(manager->m_image.y), static_cast<int>(manager->m_image.width),
          static_cast<int>(manager->m_image.height), texture, sourceWidth, sourceHeight, manager->m_image.scale, manager->m_image.scale, 0xFFFFFFFF,
          false, 0, 0, manager->m_image.clipX, manager->m_image.clipY
        );
      }
      break;
    case 1: {
      auto* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(manager->m_imagePath + ".act"));
      auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(manager->m_imagePath + ".spr"));
      if (!action || !sprite) {
        return;
      }

      SpriteFrameCell& cell = const_cast<SpriteFrameCell&>(action->Frame(0, 0).Cell(0));
      cell.x = 0;
      cell.y = 0;
      cell.scaleX = 1.0f;
      cell.scaleY = 1.0f;
      this->AddSpriteCellOverlay(
        static_cast<int>(manager->m_image.x), static_cast<int>(manager->m_image.y), 0, 0, sprite->images[0][0], &cell, 1.0f, 0.0f, 0xFFFFFFFF,
        sprite->palette, manager->m_image.scale
      );
      break;
    }
    case 2: {
      const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\img_noImage.bmp")));
      this->DrawBitmap(1, 17, bitmap ? &bitmap->image : nullptr, false);
      break;
    }
    }
  }

  this->PaintAnimations();
  const auto* border = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\bg_tipbox.bmp")));
  this->DrawBitmap(241, 17, border ? &border->image : nullptr, false);
  const auto* bottom = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\btnbar_bottom.bmp")));
  this->DrawBitmap(0, 337, bottom ? &bottom->image : nullptr, false);
}

// OFFSET: 0x008E9370
// FUNCTION: fn_12203
void UITipOfTheDayWnd::Slot15(int width, int height) {
  const std::string prefix("유저인터페이스\\");
  const char* names[3][3] = {{"btn_back", "btn_back_a", "btn_back_b"}, {"btn_next", "btn_next_a", "btn_next_b"}, {"btn_close", "btn_close_a", "btn_close_b"}};
  int positions[3][2] = {{144, this->geometry.height - 24}, {189, this->geometry.height - 24}, {234, this->geometry.height - 24}};
  int ids[3] = {246, 218, 201};
  const char* tooltips[3] = {"", "", "/tip"};

  for (int index = 0; index < 3; ++index) {
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    button->SetTooltip(tooltips[index]);
    this->AddChild(button);
  }

  this->m_message0 = 246;

  this->m_textViewer = new UITextViewer();
  this->m_textViewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_textViewer->backgroundColor = 0xFF000000u | RGB(red, green, blue);
  this->m_textViewer->Invalidate();
  this->AddChild(this->m_textViewer);

  this->m_showTipsAtStartup = g_window315Enabled;
  UICheckBox* checkBox = new UICheckBox(*this->m_context, *this->m_controls);
  checkBox->SetSize(this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(176), 0, 0, 12, 0, 0) + 12, 12);
  checkBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(176));
  checkBox->Move(10, height - 20);
  checkBox->SetChecked(this->m_showTipsAtStartup);
  this->AddChild(checkBox);

  if (g_tipOfTheDay.Size() != 0) {
    const int random = std::rand();
    const int index = random % g_tipOfTheDay.Size();
    this->m_tipIndex = index;
    const StringList& tip = g_tipOfTheDay.GetTip(index);
    this->m_textViewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(175)), 38, 0, 0);
    for (unsigned line = 0; line < tip.size(); ++line) {
      this->m_textViewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(tip[line].c_str()), 38, 0, 0);
    }
  }
}

// OFFSET: 0x008E60A0

// OFFSET: 0x008F1430
// FUNCTION: fn_12254
void UIStorePasswordInputWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, LookupMessage(1017), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);

  std::string prefix("유저인터페이스\\");
  {
    int positions[3][2] = {{this->geometry.width - 91, 146}, {this->geometry.width - 46, 146}, {this->geometry.width - 210, 146}};
    const char* names[3][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}, {"btn_change", "btn_change_a", "btn_change_b"}};
    int ids[3] = {184, 185, 292};
    const int count = g_session.m_passwordInputAlternate ? 2 : 3;

    for (int i = 0; i < count; ++i) {
      auto* button = new UIBitmapButton();
      button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[i][0], positions[i][1]);
      button->SetId(ids[i]);
      this->AddChild(button);
    }
  }

  prefix = "유저인터페이스\\basic_interface\\";
  {
    const char* names[11][3] = {
      {"btn_num1", "btn_num1_a", "btn_num1_b"}, {"btn_num2", "btn_num2_a", "btn_num2_b"}, {"btn_num3", "btn_num3_a", "btn_num3_b"},
      {"btn_num4", "btn_num4_a", "btn_num4_b"}, {"btn_num5", "btn_num5_a", "btn_num5_b"}, {"btn_num6", "btn_num6_a", "btn_num6_b"},
      {"btn_num7", "btn_num7_a", "btn_num7_b"}, {"btn_num8", "btn_num8_a", "btn_num8_b"}, {"btn_num9", "btn_num9_a", "btn_num9_b"},
      {"btn_num0", "btn_num0_a", "btn_num0_b"}, {"btn_numR", "btn_numR_a", "btn_numR_b"}
    };
    int positions[11][2] = {{10, 25}, {40, 25}, {70, 25}, {10, 55}, {40, 55}, {70, 55}, {10, 85}, {40, 85}, {70, 85}, {10, 115}, {40, 115}};
    int ids[11] = {294, 295, 296, 297, 298, 299, 300, 301, 302, 293, 303};
    int used[10] = {};
    int placed = 0;

    for (int i = 0; i < 11; ++i) {
      auto* button = new UIBitmapButton();
      button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      if (i == 10) {
        button->Move(positions[10][0], positions[10][1]);
      } else {
        int slot = rand() % 10;
        if (placed != 10) {
          while (used[slot] != 0) {
            slot = rand() % 10;
          }
          used[slot] = 1;
          button->Move(positions[slot][0], positions[slot][1]);
          ++placed;
        }
      }
      button->SetId(ids[i]);
      this->AddChild(button);
    }
  }

  std::uint32_t boxColor;
  std::uint32_t textColor;
  std::uint32_t shadowColor;
  if (!g_session.m_passwordInputAlternate) {
    boxColor = 0xFAC8AA;
    shadowColor = 0;
    textColor = 0xE6E6E6;
  } else {
    boxColor = 0xE6E6E6;
    textColor = 0xF0F0F0;
    shadowColor = 0xE6E6E6;
  }

  this->DrawEmboldenedText(110, 27, LookupMessage(1012), textColor, shadowColor, 0, 0, 12);
  this->FillRegion(110, 40, 100, 16, boxColor);
  this->FillRegion(111, 41, 98, 14, 0xFFFFFF);
  if (!g_session.m_passwordInputAlternate && this->unknown_0108 == 0) {
    this->DrawEmboldenedText(110, 63, LookupMessage(1013), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(110, 78, 100, 16, 0xE6E6E6);
    this->DrawEmboldenedText(110, 103, LookupMessage(1014), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(110, 117, 100, 16, 0xE6E6E6);
  } else {
    this->DrawEmboldenedText(110, 63, LookupMessage(1013), 0xE6E6E6, 0, 0, 0, 12);
    this->FillRegion(110, 78, 100, 16, 0xFAE6C8);
    this->DrawEmboldenedText(110, 103, LookupMessage(1014), 0xE6E6E6, 0, 0, 0, 12);
    this->FillRegion(110, 117, 100, 16, 0xFAE6C8);
    if (g_session.m_passwordInputAlternate) {
      this->FillRegion(111, 79, 98, 14, 0xFFFFFF);
    }
  }
}

// OFFSET: 0x0090C5A0
// FUNCTION: fn_12449
void UISecondPasswordInputWnd::ShuffleKeypad()
{
  unsigned char digits[10];
  for (int index = 0; index < 10; ++index)
    digits[index] = static_cast<unsigned char>(index);
  std::mt19937 engine(std::random_device{}());
  std::shuffle(digits, digits + 10, engine);
  const int positions[11][2] = {
    {219, 78}, {279, 78}, {339, 78}, {219, 138}, {279, 138}, {339, 138},
    {219, 198}, {279, 198}, {339, 198}, {219, 258}, {279, 258}
  };
  for (int index = 0; index < 10; ++index)
    unknown_0124[index]->Move(positions[digits[index]][0], positions[digits[index]][1]);
  Invalidate();
}

// OFFSET: 0x009099E0
// FUNCTION: fn_12395
void UIEquipWnd::RefreshLook()
{
  if (m_viewOther == 0) {
    unknown_00F8[0] = g_session.GetDisplayJob();
    unknown_00F8[1] = g_session.SyncUnknown0E98();
    unknown_00F8[2] = g_session.m_head;
    unknown_00F8[6] = g_session.m_bodyStyle;
    unknown_00F8[7] = g_session.m_headPalette;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->scene && mode->scene->player)
      unknown_011C = mode->scene->player->Slot46();
  } else {
    unknown_00F8[0] = g_equipWindowLook.job;
    unknown_00F8[1] = g_equipWindowLook.sex;
    unknown_00F8[2] = g_equipWindowLook.head;
    unknown_00F8[6] = g_equipWindowLook.bodyPalette;
    unknown_00F8[7] = g_equipWindowLook.headPalette;
    unknown_011C = g_equipWindowLook.unknown_0024;
  }
  unknown_00F8[3] = 0;
  unknown_00F8[4] = 0;
  unknown_00F8[5] = 0;
  unknown_0118 = 0;
  ITEM_INFO middle = GetSlotItem(8);
  if (middle.unknown_0000[1] != 0)
    unknown_00F8[4] = middle.unknown_0070;
  ITEM_INFO lower = GetSlotItem(9);
  if (lower.unknown_0000[1] != 0 && middle.unknown_0000[1] != lower.unknown_0000[1])
    unknown_00F8[5] = lower.unknown_0070;
  ITEM_INFO upper = GetSlotItem(0);
  if (upper.unknown_0000[1] != 0 && middle.unknown_0000[1] != upper.unknown_0000[1] && lower.unknown_0000[1] != upper.unknown_0000[1])
    unknown_00F8[3] = upper.unknown_0070;
  ITEM_INFO robe(0);
  robe = GetSlotItem(2);
  if (robe.unknown_0000[1] != 0)
    unknown_0118 = robe.unknown_0070;
}

// OFFSET: 0x008D7CC0
// FUNCTION: fn_12124
char* UIBookWnd::StripColorCodes(const char* text) {
  const unsigned length = static_cast<unsigned>(std::strlen(text));
  const char* const end = text + length;
  const char* const marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));
  char* segmentBuffer = new char[length + 1];
  char* result = new char[length + 1];
  *result = '\0';
  std::uint32_t color = 0;
  while (text < end) {
    const char* const start = ParseColorCode(text, color);
    text = SegmentEnd(start, end);
    std::string segment;
    if (start != text) {
      segment.assign(start, text - start);
    }
    if (marker != nullptr && *marker != '\0') {
      segment.insert(0, marker, 3);
    }
    if (text != start) {
      std::strcpy(segmentBuffer, segment.c_str());
      std::strcat(result, segmentBuffer);
    }
  }
  return result;
}

// OFFSET: 0x008F4AC0
// FUNCTION: fn_12293
void UIBookWnd::Slot19() {
  if (!this->m_autoRead || this->m_autoTick == 0 || timeGetTime() <= this->m_autoTick + 4000) {
    return;
  }
  int line = this->m_autoLine;
  if (line >= this->m_lineCount) {
    this->m_autoTick = 0;
    if (this->unknown_00B8 != nullptr) {
      g_windowMgr.Schedule(this->unknown_00B8);
      this->unknown_00B8 = nullptr;
    }
    g_windowMgr.CloseWindow(106);
    return;
  }
  const char* text = this->m_lines[line].c_str();
  if ((std::strlen(text) == 0 || std::strcmp(text, " ") == 0) && line < this->m_lineCount - 1) {
    this->m_autoLine = line + 1;
  }
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(this->StripColorCodes(this->m_lines[this->m_autoLine].c_str())), 0xE1E4FF, 0, 0, 0);
  UIWindow* balloon = this->unknown_00B8;
  if (balloon == nullptr) {
    balloon = new UITransBalloonText(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<BalloonTextContext*>(nullptr));
    this->unknown_00B8 = balloon;
    balloon->SetSize(8, 8);
    g_windowMgr.AddRenderedFront(this->unknown_00B8);
    balloon = this->unknown_00B8;
  }
  static_cast<UITransBalloonText*>(balloon)->SetText(this->StripColorCodes(this->m_lines[this->m_autoLine].c_str()), 100);
  this->unknown_00B8->Move((g_renderer->viewport.width - 640) / 2 - this->unknown_00B8->geometry.width / 2 + 320, 81);
  static_cast<UITransBalloonText*>(this->unknown_00B8)->m_textColor = 0xE1E4FF;
  static_cast<UITransBalloonText*>(this->unknown_00B8)->m_outlineColor = 0;
  ++this->m_autoLine;
  this->m_autoTick = timeGetTime();
}

int ParseItemIdTag(const char* text, const char* end, int& length, int& itemId);

// OFFSET: 0x008F8ED0

// OFFSET: 0x008F2DF0
// FUNCTION: fn_12262
void UITipboxWnd::PaintAnimations() {
  std::string path;
  for (unsigned int i = 0; i < this->m_animations.size(); ++i) {
    path = "유저인터페이스\\tipbox\\ico_effect" + IntToString(this->m_animations[i].kind, 0) + ".bmp";
    if (this->m_animations[i].visible) {
      const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
      this->DrawBitmap(
        static_cast<int>(this->m_animations[i].x), static_cast<int>(this->m_animations[i].y), bitmap ? &bitmap->image : nullptr, true
      );
    }
  }

  for (unsigned int i = 0; i < this->m_dots.size(); ++i) {
    if (this->m_dots[i].visible) {
      const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\ico_effect6.bmp")));
      this->DrawBitmap(static_cast<int>(this->m_dots[i].x), static_cast<int>(this->m_dots[i].y), bitmap ? &bitmap->image : nullptr, true);
    }
  }
}

// OFFSET: 0x008DA690
// FUNCTION: fn_12148
int UIFrameWnd::BuildItemName(
  const ITEM_INFO* item, int* creatorColor, std::vector<char*>* breaks, char** cursor, std::size_t* remaining, char** creator,
  char splitCreator, char withSlots
) {
  if (breaks->capacity() < 5) {
    breaks->reserve(5);
  }

  int& capacity = reinterpret_cast<int&>(*remaining);
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNamingContext naming{{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language}};

  const auto unknownCreator = [&]() {
    this->m_context->registration.Add(this);
    const int error = strncpy_s(*cursor, *remaining, LookupMessage(581), _TRUNCATE);
    if (error == 0 || error == STRUNCATE) {
      const std::size_t bytes = *cursor ? strnlen(*cursor, *remaining) : 0;
      *cursor += bytes;
      *remaining -= bytes;
    }
  };
  const auto isEquipment = [&]() {
    switch (item->unknown_0000[0]) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 21:
      return true;
    default:
      return false;
    }
  };

  if (!item->identified) {
    *cursor += ItemUnidentifiedName(*item, *cursor, capacity, 1);
    breaks->push_back(*cursor);
    return 1;
  }

  int cards[4];
  std::memcpy(cards, item->unknown_001C, sizeof(cards));
  if (item->unknown_0000[0] == 4 && item->IsApprenticeItem() == true && cards[3] == 1) {
    *cursor += naming.Apprentice(*item, *cursor, capacity, 1);
    breaks->push_back(*cursor);
  }

  if (cards[0] == 254 && cards[1] == 0) {
    if (item->unknown_0060 > 0) {
      *cursor += item->Refine(*cursor, capacity, 1);
      breaks->push_back(*cursor);
    }
    if (item->unknown_0088 > 0) {
      *cursor += naming.Grade(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
    }

    if (g_serviceType == 12 || g_serviceType == 14) {
      *cursor += descriptions.Name(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      *cursor += naming.Element(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      const int bytes = naming.Creator(*item, *cursor, capacity, 1);
      if (bytes) {
        *creator = *cursor;
        *cursor += bytes;
        *creatorColor = 0xE04040;
      } else {
        unknownCreator();
        *creatorColor = 0x4040E0;
      }
      breaks->push_back(*cursor);
      return 1;
    }

    const int bytes = naming.Creator(*item, *cursor, capacity, 1);
    if (bytes) {
      *creator = *cursor;
      *cursor += bytes;
      *creatorColor = 0xE04040;
    } else {
      unknownCreator();
      *creatorColor = 0x4040E0;
    }
  } else {
    if (!isEquipment()) {
      *cursor += descriptions.Name(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      if (withSlots == 1) {
        *cursor += naming.Slots(*item, *cursor, capacity, 1);
        breaks->push_back(*cursor);
      }
      return 1;
    }

    if (item->unknown_0060 > 0) {
      *cursor += item->Refine(*cursor, capacity, 1);
      breaks->push_back(*cursor);
    }
    if (item->unknown_0088 > 0) {
      *cursor += naming.Grade(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
    }

    if (naming.HasCards(*item)) {
      *cursor += naming.Prefixes(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      *cursor += descriptions.Name(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      *cursor += naming.Suffixes(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      if (withSlots == 1) {
        *cursor += naming.Slots(*item, *cursor, capacity, 1);
        breaks->push_back(*cursor);
      }
      return 1;
    }

    if (!item->HasSpecialCard()) {
      *cursor += descriptions.Name(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      if (withSlots == 1) {
        *cursor += naming.Slots(*item, *cursor, capacity, 1);
        breaks->push_back(*cursor);
      }
      return 1;
    }

    if (g_serviceType == 12 || g_serviceType == 14) {
      if (g_serviceType == 12) {
        *cursor += descriptions.Name(*item, *cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += naming.Element(*item, *cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += naming.Star(*item, *cursor, capacity, 1);
      } else {
        *cursor += naming.Star(*item, *cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += descriptions.Name(*item, *cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += naming.Element(*item, *cursor, capacity, 1);
      }
      breaks->push_back(*cursor);
      *creator = *cursor;
      const int bytes = naming.Creator(*item, *cursor, capacity, 1);
      if (bytes) {
        *creator = *cursor;
        *cursor += bytes;
        *creatorColor = 0xE04040;
      } else {
        unknownCreator();
        *creatorColor = 0x4040E0;
      }
      breaks->push_back(*cursor);
      if (splitCreator) {
        breaks->push_back(*cursor);
      }
      return 1;
    }

    *cursor += naming.Star(*item, *cursor, capacity, 1);
    breaks->push_back(*cursor);
    *creator = *cursor;
    const int bytes = naming.Creator(*item, *cursor, capacity, 1);
    if (bytes) {
      *creator = *cursor;
      *cursor += bytes;
      *creatorColor = 0xE04040;
    } else {
      unknownCreator();
    }
    breaks->push_back(*cursor);
    if (!splitCreator) {
      *cursor += naming.Element(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      *cursor += descriptions.Name(*item, *cursor, capacity, 1);
      breaks->push_back(*cursor);
      return 1;
    }
  }

  breaks->push_back(*cursor);
  *cursor += naming.Element(*item, *cursor, capacity, 1);
  breaks->push_back(*cursor);
  *cursor += descriptions.Name(*item, *cursor, capacity, 1);
  breaks->push_back(*cursor);
  return 1;
}

// OFFSET: 0x008D9420
// FUNCTION: fn_12133
bool UIItemCollectionWnd::WrapText(std::string text, int width, std::list<std::string>& lines) {
  if (this->TextWidth(this->m_context->fonts, this->m_context->locale, text.c_str(), 0, 0, 12, 0, 0) <= width) {
    lines.push_back(text);
    return false;
  }

  const char* start = text.c_str();
  const char* cursor = start;
  while (cursor != text.c_str() + text.size()) {
    if (*cursor == '\n') {
      lines.emplace_back(start, cursor);
      ++cursor;
      start = cursor;
    } else if (cursor[0] < 0 && cursor[1] < 0) {
      const std::string candidate(start, cursor + 2);
      if (this->TextWidth(this->m_context->fonts, this->m_context->locale, candidate.c_str(), 0, 0, 12, 0, 0) > width) {
        lines.emplace_back(start, cursor);
        start = cursor;
      }
      cursor += 2;
    } else {
      const std::string candidate(start, cursor + 1);
      if (this->TextWidth(this->m_context->fonts, this->m_context->locale, candidate.c_str(), 0, 0, 12, 0, 0) > width) {
        lines.emplace_back(start, cursor);
        start = cursor;
      }
      ++cursor;
    }
  }

  const std::string rest(start, text.c_str() + text.size());
  if (!rest.empty()) {
    lines.push_back(rest);
  }
  return true;
}

// OFFSET: 0x008F8ED0
// FUNCTION: fn_12334
int UIChooseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (g_linkInputOwner != nullptr) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xAAA)), 0x64FFFF, 0, 0, 0);
    return 0;
  }

  switch (message) {
  case 6: {
    Replay& replay = Replay::Instance();
    ReplayPacketRecorder packets{g_packetLengths, replay.baseFrame, replay.started, replay.frame, replay.header.key0, replay.header.key1,
                                 replay.header.options.recordRestricted, replay.file, replay.fileOffset, replay.records, replay.cipherIndex,
                                 replay.cipherBase, replay.cipherKey};
    WindowResponseRecorder{replay.state, packets}.Record(this->geometry.id, static_cast<int>(a), b, c, d);
    if (a == 184) {
      const std::uint32_t tick = GetTickCount();
      if (tick - *reinterpret_cast<const DWORD*>(&g_session.unknown_5490[0x04]) >= 300) {
        *reinterpret_cast<DWORD*>(&g_session.unknown_5490[0x04]) = tick;
        if (this->m_items.empty()) {
          this->SendMsg(nullptr, 6, 185, 0, 0, 0);
          return 0;
        }
        const int selected = this->m_list->selected;
        if (selected != -1) {
          g_modeMgr.GetGameMode()->SendMsg(0x17, this->m_items[selected] + 1, this->unknown_00C8, 0, 0);
          g_windowMgr.CloseWindow(0x11);
        }
      }
    } else if (a == 185) {
      g_modeMgr.GetGameMode()->SendMsg(0x28, 0, 0, 0, 0);
    }
    return 0;
  }
  case 0x12:
  case 0x13:
    this->m_list->SendMsg(nullptr, message, 0, 0, 0, 0);
    return 0;
  case 0x1B:
    if (a != 0) {
      const char* value = reinterpret_cast<const char*>(a);
      if (std::strstr(value, LookupMessage(0x29A)) == nullptr) {
        std::string text(value);
        int length = 0;
        int itemId = 0;
        if (ParseItemIdTag(value, value + std::strlen(value), length, itemId) == 1 && itemId > 0) {
          text = FindItemDescription(itemId).identifiedName;
        }
        this->m_list->AddLine(text.c_str());
        ++this->unknown_00B4;
        this->m_items.push_back(this->unknown_00B4);
        const int previousWidth = this->m_list->geometry.width;
        if (previousWidth < this->m_list->contentWidth + 15) {
          this->m_list->Resize(this->m_list->contentWidth + 15, this->m_list->geometry.height);
          this->Resize(this->geometry.width + this->m_list->geometry.width - previousWidth, this->geometry.height);
          this->m_okButton->Move(this->geometry.width - 92, this->geometry.height - 24);
          this->m_cancelButton->Move(this->geometry.width - 46, this->geometry.height - 24);
        }
      }
    }
    return 0;
  case 0x1C:
    this->unknown_00C8 = static_cast<int>(a);
    return 0;
  case 0x22: {
    ChooseWndInfo* placement = reinterpret_cast<ChooseWndInfo*>(a);
    this->m_placement = placement;
    placement->IsValid();
    this->Move(placement->x, placement->y);
    return 0;
  }
  case 0x7B: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x558C) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        if (header.type == 0x558D) {
          break;
        }
        if (header.type == 0x558F) {
          char text[260] = {};
          std::memcpy(text, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, 0x1B, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
        } else {
          position += header.length;
        }
      }
      if (g_windowMgr.unknown_01F0 != nullptr) {
        g_windowMgr.unknown_01F0->SendMsg(nullptr, 0x20, 0, 0, 0, 0);
      }
    }
    return 0;
  }
  case 0xA6:
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(0x17, -1, this->unknown_00C8, 0, 0);
    }
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008FD640

// OFFSET: 0x008F8640
// FUNCTION: fn_12331
int UIChoose3Wnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const char* text = reinterpret_cast<const char*>(a);
  if (message != 27 || text == nullptr || std::strlen(text) == 0 || std::strcmp(text, LookupMessage(666)) == 0) {
    return this->UIChooseWnd::SendMsg(sender, message, a, b, c, d);
  }
  int length = 0;
  int itemId = 0;
  const char* line = text;
  if (ParseItemIdTag(text, text + std::strlen(text), length, itemId) == 1 && itemId > 0) {
    line = FindItemDescription(itemId).identifiedName;
  }
  this->m_list->AddLine(line);
  ++this->unknown_00B4;
  this->m_items.push_back(this->unknown_00B4);
  UIListBox* list = this->m_list;
  const int listWidth = list->geometry.width;
  const int growWidth = 0;
  int growHeight = 0;
  if (this->m_minimumLines < static_cast<int>(list->lines.size()) && list->LineCount() <= this->m_maximumLines) {
    growHeight = list->lineHeight;
  }
  if (growWidth > 0 || growHeight > 0) {
    const int okX = this->geometry.width - this->m_okButton->geometry.x;
    const int okY = this->geometry.height - this->m_okButton->geometry.y;
    const int cancelX = this->geometry.width - this->m_cancelButton->geometry.x;
    const int cancelY = this->geometry.height - this->m_cancelButton->geometry.y;
    list->Resize(listWidth + growWidth, list->geometry.height + growHeight);
    this->Resize(this->geometry.width + growWidth, this->geometry.height + growHeight);
    this->m_okButton->Move(this->geometry.width - okX, this->geometry.height - okY);
    this->m_cancelButton->Move(this->geometry.width - cancelX, this->geometry.height - cancelY);
  }
  return 0;
}

// OFFSET: 0x008D3720
// FUNCTION: fn_12103
void UITipboxWnd::Search(std::string& keyword) {
  const std::vector<int>& results = g_itemTagOwner->Search(keyword);
  this->unknown_00B8->Clear();
  for (const int id : results) {
    const std::string& source = g_itemTagOwner->GetTip(id).title;
    std::string title(source.c_str(), source.size());
    const std::size_t separator = title.find('#');
    if (separator != std::string::npos) {
      title = title.substr(0, separator);
    }
    if (title.size() != 0) {
      this->unknown_00B8->AddLine(title.c_str(), id);
    }
  }
  this->unknown_00B8->Invalidate();
}

// OFFSET: 0x008D9FE0
// FUNCTION: fn_12140
void UITipboxWnd::RefreshTip() {
  this->UpdatePageText();
  this->BuildAnimations();
  this->unknown_00B8->Show(0);

  std::string title(g_itemTagOwner->m_currentEntry.title.c_str(), g_itemTagOwner->m_currentEntry.title.size());
  this->TrimAt(title, "#");
  this->unknown_00D8->Enable(g_itemTagOwner->m_currentEntry.searchable);

  std::vector<std::string> lines;
  WrapPlainText(title.c_str(), lines, 30, static_cast<unsigned short>(g_textCodePage));

  UIStaticText* const texts[2] = {this->unknown_00C0, this->unknown_00C4};
  for (int index = 0; index < 2; ++index) {
    texts[index]->SetText("", 0);
    texts[index]->Show(0);
  }

  const unsigned count = static_cast<unsigned>(lines.size());
  if (count != 0) {
    const int shown = (count > 1 ? 1 : 0) + 1;
    for (int index = 0; index < shown; ++index) {
      texts[index]->SetText(lines[index].c_str(), 0);
      texts[index]->Show(1);
    }

    int height;
    if (count < 2) {
      this->unknown_00BC->Move(250, 70);
      height = 245;
    } else {
      this->unknown_00BC->Move(250, 90);
      height = 225;
    }
    this->unknown_00BC->Resize(226, height);
    this->unknown_00FC = true;
  }
}

// OFFSET: 0x00907050
// FUNCTION: fn_12386
int UITipboxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  if (a > 0xF6) {
    if (a == 0x16D) {
      std::string keyword(this->unknown_00B4->Text());
      if (keyword.size() != 0) {
        this->unknown_00B8->Show(1);
        this->unknown_00B8->Clear();
        this->unknown_00BC->Clear();
        UIStaticText* const texts[2] = {this->unknown_00C0, this->unknown_00C4};
        for (int index = 0; index < 2; ++index) {
          texts[index]->SetText("", 0);
        }
        this->m_pageText->SetText("", 0);
        this->m_pageTotalText->SetText("", 0);
        this->unknown_00D0->Enable(0);
        this->unknown_00D4->Enable(0);
        this->unknown_00B4->SetText("");
        this->m_animations.clear();
        this->m_dots.clear();
        this->Search(keyword);
        this->unknown_00D8->Enable(0);
      }
    } else if (a == 0x205) {
      if (!g_windowMgr.GetHoverWindow()) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x1062)), 0xFF, 0, 0, 0);
      } else {
        const int id = g_itemTagOwner->unknown_0114;
        switch (g_windowMgr.GetHoverWindow()->geometry.id) {
        case 0x1EA:
        case 0x1ED:
        case 0x1EE:
          static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemTag(id);
          break;
        case 0x1EB:
          static_cast<UIRodexComposeWnd*>(g_linkInputOwner)->sub_7FF940(id);
          break;
        case 0x1EC:
          static_cast<UINpcTextEditDialogWnd*>(g_windowMgr.unknown_0318)->InsertSkillTag(id);
          break;
        }
      }
    } else if (a == 0x21F) {
      this->unknown_00D4->SetDisabled(g_itemTagOwner->IsLastTip() == true ? 1 : 0);
      this->unknown_00D4->Invalidate();
      this->unknown_00D4->Invalidate();
      this->unknown_00D0->SetDisabled(1);
      this->unknown_00D0->Invalidate();
      this->unknown_00D0->Invalidate();
      this->RefreshTip();
      this->BuildAnimations();
    }
    return 0;
  }

  if (a == 0xF6) {
    const int page = (std::max)(0, g_itemTagOwner->unknown_00B4);
    g_itemTagOwner->unknown_00B4 = page - 1;
    this->unknown_00D4->SetDisabled(0);
    this->unknown_00D4->Invalidate();
    this->unknown_00D4->Invalidate();
    if (page == 1) {
      this->unknown_00D0->SetDisabled(1);
      this->unknown_00D0->Invalidate();
      this->unknown_00D0->Invalidate();
    }
    this->RefreshTip();
    return 0;
  }

  switch (a) {
  case 0xB8:
    this->unknown_00D0->Enable(1);
    this->unknown_00D4->Enable(1);
    g_itemTagOwner->SelectTip(this->unknown_00B8->lines[this->unknown_00B8->selected].id);
    this->unknown_00D4->SetDisabled(g_itemTagOwner->IsLastTip() == true ? 1 : 0);
    this->unknown_00D4->Invalidate();
    this->unknown_00D4->Invalidate();
    this->unknown_00D0->SetDisabled(1);
    this->unknown_00D0->Invalidate();
    this->unknown_00D0->Invalidate();
    this->RefreshTip();
    this->BuildAnimations();
    break;
  case 0xC9:
    g_windowMgr.CloseWindow(0x13B);
    break;
  case 0xD5: {
    DWORD disposition = 0;
    this->unknown_0100 = static_cast<int>(b);
    g_window315Enabled = static_cast<int>(b);
    HKEY key;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
      RegSetValueExA(key, "SHOWTIPSATSTARTUP", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->unknown_0100), 4);
      RegCloseKey(key);
    }
    break;
  }
  case 0xDA: {
    const int page = (std::max)(0, g_itemTagOwner->unknown_00B4);
    g_itemTagOwner->unknown_00B4 = page + 1;
    if (g_itemTagOwner->IsLastTip() == true) {
      this->unknown_00D4->SetDisabled(1);
      this->unknown_00D4->Invalidate();
      this->unknown_00D4->Invalidate();
    }
    this->unknown_00D0->SetDisabled(0);
    this->unknown_00D0->Invalidate();
    this->unknown_00D0->Invalidate();
    this->RefreshTip();
    break;
  }
  }
  return 0;
}

// OFFSET: 0x0090A2E0
// FUNCTION: fn_12415
void UIFrameWnd::DrawItemNameText(
  const void* info, const char* text, const char* highlight, std::uint32_t color, std::uint32_t highlightColor, int width,
  const std::vector<char*>* breaks
) {
  struct ItemNameTextInfo {
    int x;
    int y;
    int height;
    int family;
  };
  const ItemNameTextInfo* position = static_cast<const ItemNameTextInfo*>(info);

  FrameTextCache::iterator cached;
  {
    const FrameTextKey key{color, static_cast<std::uint32_t>(width), std::string(text, std::strlen(text))};
    cached = this->m_textCache.find(key);
  }
  if (cached != this->m_textCache.end()) {
    this->DrawTextLines(position->x, position->y, position->family, position->height, cached->second);
    return;
  }

  std::vector<std::pair<int, std::string>> segments;
  const int highlightValue = static_cast<int>(highlightColor);
  const int colorValue = static_cast<int>(color);
  this->SplitHighlightText(text, highlight, highlightValue, colorValue, *breaks, segments);

  FrameTextLines lines(2);
  std::vector<std::pair<int, std::string>>* line = &lines[0];
  int lineCount = 1;
  int lineWidth = 0;
  for (auto segment = segments.begin(); segment != segments.end(); ++segment) {
    const char* begin = segment->second.c_str();
    const char* end = begin + segment->second.size();
    const char* lineStart = begin;
    const char* next = begin;
    std::size_t offset = 0;
    int length;
    int measured;
    do {
      next = CharNextExA(static_cast<WORD>(g_textCodePage), next, 0);
      length = static_cast<int>(next - lineStart);
      measured = this->TextWidth(this->m_context->fonts, this->m_context->locale, begin, length, 0, 12, 0, 0);
      if (measured + lineWidth > width) {
        line->push_back(std::make_pair(segment->first, segment->second.substr(offset, length)));
        ++line;
        ++lineCount;
        lineStart = next;
        offset = length;
        lineWidth = 0;
      }
    } while (next < end && lineCount < 3);

    lineWidth += measured;
    if (lineCount > 2) {
      break;
    }
    if (next >= end) {
      line->push_back(std::make_pair(segment->first, segment->second.substr(offset, length)));
    }
  }

  if (this->TextWidth(this->m_context->fonts, this->m_context->locale, text, std::strlen(text), position->family, position->height, 0, 0) > width * 2) {
    this->FitLastTextLine(width, 2, position->family, position->height, lines);
  }

  {
    FrameTextKey key{color, static_cast<std::uint32_t>(width), std::string(text, std::strlen(text))};
    FrameTextLines& stored = this->m_textCache[std::move(key)];
    if (&stored != &lines) {
      for (const auto& source : lines) {
        stored.emplace_back();
        for (const auto& piece : source) {
          stored.back().emplace_back(piece.first, std::string(piece.second.c_str(), piece.second.size()));
        }
      }
    }
  }
  this->DrawTextLines(position->x, position->y, position->family, position->height, lines);
}

// OFFSET: 0x00900D90
// FUNCTION: fn_12372
int UIProhibitListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 0xC9) {
      g_windowMgr.CloseWindow(0x67);
    } else if (a == 0x10C) {
      UIWindow* menu = g_windowMgr.EnsureWindow(0x1A);
      const std::string labels[4] = {LookupMessage(0x250), LookupMessage(0x251), LookupMessage(0x252), LookupMessage(0x253)};
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0x2CF)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0x2D0)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0x2D1)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x28, 0, 0, 0, 0);
      menu->Resize(0x82, 0x30);
      menu->SendMsg(nullptr, 0x53, 0x67, 0, 0, 0);
      int x = 0xA0;
      int y = 0x2C;
      this->ToScreen(x, y);
      menu->Move(x, y);
    }
  } else if (message == 0x22) {
    this->m_aid = static_cast<std::uint32_t>(a);
  } else if (message == 0x27) {
    CMode* mode = g_modeMgr.GetGameMode();
    int text;
    UICntWnd* counter;
    if (a == 0) {
      if (!IsMonsterJob(g_session.m_aid)) {
        return 0;
      }
      counter = new UICntWnd(*this->m_context, *this->m_frameContext, *this->m_controls, *this->m_edits);
      counter->SetSize(0xB6, 0x2E);
      counter->Move((g_renderer->viewport.width - 640) / 2 + 200, ScaleByScreenHeight(200));
      g_windowMgr.AddRendered(counter);
      counter->SendMsg(nullptr, 0x50, 0x98, 0, 0, 0);
      text = 0x186;
    } else if (a == 1) {
      if (IsAdminAccount(g_session.m_aid)) {
        mode->SendMsg(0x97, 30000, 0, 0, 0);
      }
      return 0;
    } else {
      if (a != 2 || !IsAdminAccount(g_session.m_aid)) {
        return 0;
      }
      counter = new UICntWnd(*this->m_context, *this->m_frameContext, *this->m_controls, *this->m_edits);
      counter->SetSize(0xB6, 0x2E);
      counter->Move((g_renderer->viewport.width - 640) / 2 + 200, ScaleByScreenHeight(200));
      g_windowMgr.AddRendered(counter);
      counter->SendMsg(nullptr, 0x50, 0x97, 0, 0, 0);
      text = 0x185;
    }
    counter->SendMsg(nullptr, 0x1D, reinterpret_cast<std::intptr_t>(LookupMessage(text)), 0, 0, 0);
    counter->SendMsg(nullptr, 0x51, 1, 0, 0, 0);
  }
  return 0;
}

// OFFSET: 0x008ED760
// FUNCTION: fn_12222
void UIExchangeWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_fix2.bmp")));
  this->DrawBitmap(0, 0, &bitmap->image, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\exchange_bg2.bmp")));
  this->DrawBitmap(0, 17, &bitmap->image, false);
  DrawOutlinedText(*this, 123, this->geometry.height - 20, LookupMessage(0x727), 0, 0xFF, 0xFFFFFF, 0, 12, 1, 0);

  char title[128];
  std::sprintf(title, "%s : ", LookupMessage(0xCE));
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 13, 4, title, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 12, 3, title, 0, 0, 12, 0, 0, 0);
  char partner[128];
  if (this->unknown_0108 < 1) {
    std::sprintf(partner, "%s", this->unknown_00F0.c_str());
  } else if (g_serviceType == 6) {
    std::sprintf(partner, "%s  Lv%d", this->unknown_00F0.c_str(), this->unknown_010C);
  } else {
    const std::string identifier = ShortcutFilename(this->unknown_0108);
    std::sprintf(partner, "%s  Lv%d (%s)", this->unknown_00F0.c_str(), this->unknown_010C, identifier.c_str());
  }
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 63, 4, partner, 0, 0, 12, 0xB4B4B4, 0, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 62, 3, partner, 0, 0, 12, 0, 0, 0);

  const std::string prefix = "유저인터페이스\\btn_";
  const char* buttons[2] = {"ok", "exchange"};
  const int buttonX[2] = {3, 260};
  for (int index = 0; index < 2; ++index) {
    const std::string path = prefix + buttons[index] + "_dis.bmp";
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
    this->DrawBitmap(buttonX[index], this->geometry.height - 23, &bitmap->image, false);
  }
  for (int row = 0; row < 10; ++row) {
    DrawItemSlotBackground(*this, 12, this->m_rowHeight * row + 20);
    DrawItemSlotBackground(*this, 291, this->m_rowHeight * row + 20);
  }
  unsigned red;
  unsigned green;
  unsigned blue;
  if (this->unknown_00CC != 0) {
    SkinPaletteChannels(ResourceMgr::Instance(), 10, 6, red, green, blue);
    this->FillRegion(4, 21, 272, 310, RGB(red, green, blue));
  }
  if (this->unknown_00D0 != 0) {
    SkinPaletteChannels(ResourceMgr::Instance(), 10, 6, red, green, blue);
    this->FillRegion(283, 21, 272, 310, RGB(red, green, blue));
  }
  ItemDescriptions descriptions{g_itemDescriptions};
  int row = 0;
  for (const ITEM_INFO& item : this->m_myItems) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) != 0 && item.unknown_005D != 0) {
      continue;
    }
    const int y = this->m_rowHeight * row;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, this->m_context->fonts, this->m_context->locale, 16, y + 21, item, 0, 0);
    this->DrawFittedItemName(48, y + 29, &item, 0, 0, 12, 1);
    if (item.unknown_0088 > 0) {
      const std::string path = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088);
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
      this->DrawBitmap(16, y + 33, &bitmap->image, true);
    }
    if (row == 9) {
      break;
    }
    ++row;
  }
  row = 0;
  for (const ITEM_INFO& item : this->m_otherItems) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) != 0 && item.unknown_005D != 0) {
      continue;
    }
    const int y = this->m_rowHeight * row;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, this->m_context->fonts, this->m_context->locale, 295, y + 21, item, 0, 0);
    this->DrawFittedItemName(327, y + 29, &item, 0, 0, 12, 1);
    if (item.unknown_0088 > 0) {
      const std::string path = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088);
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
      this->DrawBitmap(295, y + 33, &bitmap->image, true);
    }
    if (row == 9) {
      break;
    }
    ++row;
  }
  char grouped[32];
  char amount[128];
  int length = std::sprintf(amount, "%s", FormatGroupedInteger(g_exchangeMyZeny, grouped, sizeof(grouped)));
  PriceColors colors = PriceColor(g_exchangeMyZeny);
  this->DrawRightAlignedText(219, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(220, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(220, 332, amount, length, 1, 16, 0x323232, 0);
  length = std::sprintf(amount, "%s", FormatGroupedInteger(g_exchangeOtherZeny, grouped, sizeof(grouped)));
  colors = PriceColor(g_exchangeOtherZeny);
  this->DrawRightAlignedText(499, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(500, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(500, 332, amount, length, 1, 16, 0x323232, 0);
}
