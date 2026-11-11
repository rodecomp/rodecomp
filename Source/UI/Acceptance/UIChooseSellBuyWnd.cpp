#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Items/ItemType.h"
#include "Client/Mode/CLoginMode.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/CombatTypes.h"
#include "File/File.h"
#include "File/ShortcutFilename.h"
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Exchange/UIExchangeUI.h"
#include "UI/Items/UISpellListWnd.h"
#include "Client/Application/Registry.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/NeoPackageItemMgr.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "UI/ProbabilityTable/UIProbabilityTable.h"
#include "UI/Buttons/UIBasicButton.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Contents/Tables/TipOfTheDay.h"
#include "Contents/Skin/SkinMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/EquipWindowLook.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Replay/Replay.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Text/TextWrapping.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/CashShop/UICashShopWnd.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UINoneVisibleButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UISpriteButton.h"
#include "UI/Chat/UIChatRoomMakeWnd.h"
#include "UI/Chat/UIWhisperWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Dialogs/UIBookWnd.h"
#include "UI/Navigation/UIChooseWarpWnd.h"
#include "UI/Dialogs/UIChooseWnd.h"
#include "UI/Dialogs/UIDisconnectedServerMsgWnd.h"
#include "UI/Login/UIEmailAddressWnd.h"
#include "UI/Skill/UIGraffiStrboxWnd.h"
#include "UI/Dialogs/UILoadingWnd.h"
#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Navigation/UIMinimapZoomWnd.h"
#include "UI/Dialogs/UINoticeConfirmWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Login/UIPasswordWnd.h"
#include "UI/Dialogs/UINotifyJobLevelUpWnd.h"
#include "UI/Dialogs/UINotifyLevelUpWnd.h"
#include "UI/Quest/UINotifyQuestWnd.h"
#include "UI/Status/UIPetTamingDeceiveWnd.h"
#include "UI/Dialogs/UIProhibitListWnd.h"
#include "UI/Dialogs/UIQuitWnd.h"
#include "UI/Dialogs/UIRestartWnd.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Items/UISelectCartWnd.h"
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
#include "UI/Login/UIChangeNameWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Monster/UIMonsterDisplayWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Login/FuncDrawWNDChar.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextTokens.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Network/TextEncoding.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Contents/Map/TownInfoMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Navigation/NavigationManager.h"
#include "UI/Navigation/NavigationLink.h"
#include "Utility/Text/GroupedInteger.h"
#include "Utility/Math/Int64Math.h"
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
#include "UI/Dialogs/UICntWnd.h"

#include "Resource/MSGSTRINGID.h"
namespace {
  constexpr char prefix[] = "유저인터페이스\\";
#pragma pack(push, 1)
  struct FramedMessageHeader {
    /* 0000 */ std::int16_t type;
    /* 0002 */ std::uint32_t length;

    FramedMessageHeader() : type(0), length(0) {}
    ~FramedMessageHeader() {
      this->type = 0;
      this->length = 0;
    }
  };
#pragma pack(pop)
  static_assert(sizeof(FramedMessageHeader) == 6);
#pragma pack(push, 1)
  struct PackedItemOption {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t value;
    /* 0004 */ unsigned char parameter;
  };
#pragma pack(pop)
  static_assert(sizeof(PackedItemOption) == 5);
}

std::string CartTypeName(int type);

// STRING: st_16833
static const char menuSeparatorText[] = "-";
// GLOBAL: gl_24749
const char* g_menuSeparator = menuSeparatorText;
// STRING: st_03412
static const char bookmarkKeySuffixText[] = "\\BookMark";

// GLOBAL: gl_24751
const char* g_bookmarkKeySuffix = bookmarkKeySuffixText;

// GLOBAL: gl_25391
bool g_randomLoadingScreen;

// GLOBAL: gl_25459
static const std::string s_partyItemLinkOpen = "<ITEML>";

// GLOBAL: gl_25460
static const std::string s_partyItemLinkClose = "</ITEML>";

// GLOBAL: gl_25473
int g_equipAbilityButtonPosition[2] = {167, 167};

// OFFSET: 0x0087B500
// FUNCTION: fn_11241
void UIChatHisBox::Configure(int x, int y, int width, int height, unsigned char) {
  this->transparent = 1;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->OnCreate(width, height);
  this->OnSize(width, height);
}

// OFFSET: 0x008C73C0
// FUNCTION: fn_11930
UIChooseSellBuyWnd::UIChooseSellBuyWnd()
    : UIFrameWnd(0), identity(0) {
}

// OFFSET: 0x008C7AA0
// FUNCTION: fn_11941
UIExchangeAcceptWnd::UIExchangeAcceptWnd()
    : UIChooseSellBuyWnd(), saved(nullptr) {
}

// OFFSET: 0x008C6FA0
// FUNCTION: fn_11926
UIBookWnd::UIBookWnd()
    : UIFrameWnd(0) {
  this->m_lineCount = 0;
  this->m_pageCount = 0;
  this->m_page = 1;
  this->m_autoTick = 0;
  this->m_autoLine = -1;
  this->m_autoRead = 0;
  this->m_balloon = nullptr;
  this->m_bookName[0] = 0;
}

// OFFSET: 0x008C7860
// FUNCTION: fn_11940
UIEquipWnd::UIEquipWnd() : UIFrameWnd(0) {
  this->swapWindow = nullptr;
  this->swapState = 0;
  this->swapButton = 0;
  this->unequipButton = 0;
  this->abilityButton = 0;
  this->m_placement = nullptr;
  this->m_updateTick = timeGetTime();
  this->contentOffset = 0;
  this->m_page = 0;
  this->m_contentTop = 20;
  for (int& value : this->appearance) {
    value = 0;
  }
  this->costumeJob = 0;
  this->swapButton = 0;
  this->m_scrollBar = nullptr;
  for (UIWindow*& value : this->damageSkinButtons) {
    value = nullptr;
  }
  this->m_firstSlot = 0;
  this->damageSkinMode = g_session.m_damageSkinMode;
  this->m_hoverSlot = g_session.m_damageSkinIndex;
  this->m_damageSkinNames.reserve(g_session.damageSkinCount);
  this->m_damageSkinIcons.reserve(g_session.damageSkinCount);
}

// OFFSET: 0x008CAC40
// FUNCTION: fn_11997
UIItemCollectionWnd::~UIItemCollectionWnd() {
  g_windowMgr.CloseWindow(WID_ROUNDBOXTOOLTIPWND);
}

UIChooseSellBuyWnd::~UIChooseSellBuyWnd() = default;

// INLINED
void UIChooseSellBuyWnd::AddChoice(const char* name, int id, int x, int y) {
  auto* button = new UIBitmapButton();
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
  this->viewer = new UITextViewer();
  this->viewer->Configure(10, 22, width - 20, height - 55, 0);
  const std::string chip = std::string(prefix) + "colorchip.bmp";
  const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(chip.c_str()));
  const auto& image = bitmap->image;
  const std::uint32_t pixel = image.width > 2 && image.height > 2 ? image.pixels[2 + 2 * image.width] : 0x00FF0000u;
  this->viewer->textColor = 0xFF000000u | ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
}

// OFFSET: 0x008C7420
// FUNCTION: fn_11931
UIChooseWarpWnd::UIChooseWarpWnd()
    : UIFrameWnd(0), m_list(nullptr), skillId(0), m_choices() {
  this->m_choices.reserve(6);
}

// OFFSET: 0x008DD290
// FUNCTION: fn_12162
void UIChooseSellBuyWnd::OnCreate(int width, int height) {
  const std::string buttonPrefix("유저인터페이스\\");
  const char* names[3][3] = {{"btn_buy", "btn_buy_a", "btn_buy_b"}, {"btn_sell", "btn_sell_a", "btn_sell_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[3] = {224, 223, 185};
  int positions[3][2] = {{144, 96}, {189, 96}, {234, 96}};

  for (int i = 0; i < 3; ++i) {
    auto* button = new UIBitmapButton();
    button->SetBitmap((buttonPrefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((buttonPrefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((buttonPrefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 224;
  this->viewer = new UITextViewer();
  this->viewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->viewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
  this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SELECT_DEAL_TYPE)), 38, 0, 0);
}

// OFFSET: 0x008DD9A0
// FUNCTION: fn_12163
void UIChooseWarpWnd::OnCreate(int width, int height) {
  auto* title = new UIStaticText();
  title->SetSize(this->geometry.width - 30, 15);
  title->Move(12, 9);
  title->SetText(LookupMessage(MSI_SELECTWHERETOWARP), 0);
  title->SetFont(0, 12, 0, 0);
  this->AddChild(title);

  this->m_list = new UIListBox();
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
    auto* button = new UIBitmapButton();
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
void UIChooseWnd::OnCreate(int width, int height) {
  this->m_list = new UIListBox();
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

  this->m_okButton = new UIBitmapButton();
  this->m_okButton->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  this->m_okButton->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  this->m_okButton->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  this->m_okButton->SetSize(this->m_okButton->BitmapWidth(), this->m_okButton->BitmapHeight());
  this->m_okButton->Move(this->geometry.width - 92, this->geometry.height - 24);
  this->m_okButton->SetId(184);
  this->AddChild(this->m_okButton);

  this->m_cancelButton = new UIBitmapButton();
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
void UIDisconnectedServerMsgWnd::OnCreate(int width, int height) {
  auto* button = new UIBitmapButton();
  button->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(234, 96);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;

  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UNABLE_TO_CONNECT_SERVER)), 38, 0, 0);
}

// OFFSET: 0x008DE6D0
// FUNCTION: fn_12170
void UIEmailAddressWnd::OnCreate(int, int) {
  g_modeMgr.gameMode->SendMsg(LMM_GET_EMAIL_ADDRESS, 0, 0, 0, 0);
  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(this->geometry.width - 77, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 49;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->SetText("");
  this->m_edit->mask = 42;
  this->AddChild(this->m_edit);
  g_windowMgr.SetFocus(this->m_edit);
  this->m_edit->SelectAll();

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 28;
  auto* button = new UIBitmapButton();
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
  auto* bitmap = static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap, false);
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
  this->DrawBitmap(0, 0, bitmap, false);
}

// OFFSET: 0x008ED1F0
// FUNCTION: fn_12220
void UIEmailAddressWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  const char* text = LookupMessage(g_serviceType == 0 ? 1815 : 299);
  this->DrawTextAt(15, 6, text, 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008ED240
// FUNCTION: fn_12221
void UIEquipWnd::Paint() {
  if (CBitmapRes* titleBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_fix.bmp")))) {
    this->DrawBitmap(0, 0, titleBar, false);
  }
  this->DrawPanelBackground(17, 0, "");
  this->FillRegion(this->m_tabs->geometry.width, 34, this->geometry.width - this->m_tabs->geometry.width, 1, 0xC3C3C3);

  if (this->m_viewOther == 1) {
    char title[100];
    std::sprintf(title, LookupMessage(MSI_OTHERUSER_EQUIPED_ITEM), reinterpret_cast<const char*>(&g_session.equipmentViewState[0x2C]));
    this->DrawTextAt(17, 4, title, 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(16, 3, title, 0, 0, 12, 0, 0, 0);
  } else {
    this->DrawTextAt(17, 4, LookupMessage(MSI_EQUIPED_ITEM), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(16, 3, LookupMessage(MSI_EQUIPED_ITEM), 0, 0, 12, 0, 0, 0);
  }

  if (this->geometry.height == 17) {
    return;
  }

  const char* background = this->m_viewOther != 0 ? "유저인터페이스\\basic_interface\\equipwin_bg.bmp" : "유저인터페이스\\basic_interface\\equipwin_bg2.bmp";
  if (CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)))) {
    this->DrawBitmap(0, this->m_contentTop + 17, bitmap, false);
  }

  if (this->m_page == 2) {
    this->FillRegion(1, this->m_contentTop + 15, this->geometry.width - 2, 131, 0xFFFFFF);
    DrawRectangleBorder(*this, 2, this->m_contentTop + 38, this->geometry.width - 4, 107, 0x646464);
    const std::string jobTitle = GetJobTitleString(g_session.jobTitle);
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
          this->DrawBitmap(columns[slot / 4], this->m_contentTop + 51 + 26 * (slot % 4), invert, true);
        } else if (slot == 8 || slot == 9) {
          this->DrawBitmap(columns[slot % 4], this->m_contentTop + 25, invert, true);
        }
      }
    }

    this->DrawEquipSlots();
    this->RefreshLook();
    FuncDrawWNDChar drawer(
      this,
      this->geometry.width / 2 + 1,
      149,
      static_cast<unsigned char>(this->appearance[1]),
      static_cast<short>(this->appearance[0]),
      this->appearance[0],
      static_cast<short>(this->costumeJob),
      static_cast<short>(this->appearance[2]),
      static_cast<short>(this->appearance[3]),
      static_cast<short>(this->appearance[4]),
      static_cast<short>(this->appearance[5]),
      this->robeId,
      0,
      g_session.madogearType,
      this->appearance[6],
      this->appearance[7],
      0,
      0,
      this->m_updateTick
    );
    drawer.Draw(false);

    if (this->m_viewOther == 0) {
      if (this->m_page == 0) {
        ITEM_INFO dragItem = LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.equippedArrowIndex);
        if (dragItem.identity[1] != 0) {
          DrawItemIcon(*this, ResourceMgr::Instance(), this->geometry.width / 2 - 25, this->m_contentTop + 44, dragItem, 0);
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

  if (this->swapWindow) {
    this->swapWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x008EE230
// FUNCTION: fn_12223
void UIGraffiStrboxWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(17, 23, LookupMessage(MSI_TALKBOX_WINDOW_MSG), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(16, 22, LookupMessage(MSI_TALKBOX_WINDOW_MSG), 0, 0, 12, 0, 0, 0);
}

// INLINED
static __forceinline void DrawParamChangeArrow(UIWindow& window, int x, int before, int after) {
  if (before < after) {
    FillTriangle(window, x + 4, 11, x, 19, x + 8, 19, 0xFF);
  } else if (before == after) {
    FillTriangle(window, x, 13, x + 8, 13, x, 17, 0x505050);
    FillTriangle(window, x + 8, 13, x, 17, x + 8, 17, 0x505050);
  } else {
    FillTriangle(window, x, 11, x + 8, 11, x + 4, 19, 0xFF0000);
  }
}

// OFFSET: 0x008EE9D0
// FUNCTION: fn_12227
void UIItemParamChangeDisplayWnd::Paint() {
  char text[128];
  this->DrawBorder(0, 0, 0, 0);
  std::sprintf(text, LookupMessage(MSI_ATK));
  this->DrawTextAt(20, 10, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%d", this->m_before);
  this->DrawTextAt(70, 10, text, 0, 0, 12, 0, 0, 0);
  DrawParamChangeArrow(*this, 90, this->m_before, this->m_after);
  std::sprintf(text, "%d", this->m_after);
  this->DrawTextAt(110, 10, text, 0, 0, 12, 0, 0, 0);

  std::sprintf(text, LookupMessage(MSI_DEF));
  this->DrawTextAt(152, 10, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%d", this->m_beforeSecond);
  this->DrawTextAt(202, 10, text, 0, 0, 12, 0, 0, 0);
  DrawParamChangeArrow(*this, 222, this->m_beforeSecond, this->m_afterSecond);
  std::sprintf(text, "%d", this->m_afterSecond);
  this->DrawTextAt(242, 10, text, 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EECD0
// FUNCTION: fn_12228
void UILoadingWnd::Paint() {
}

// OFFSET: 0x008EED00
// FUNCTION: fn_12230
void UIMenuWnd::Paint() {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  unsigned fillRed, fillGreen, fillBlue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(fillRed, fillGreen, fillBlue));

  auto* leftUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp")));
  this->DrawBitmap(0, 0, leftUp, false);
  auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, leftDown, false);
  auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, rightUp, false);
  auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, rightDown, false);

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
    this->DrawTextAt(3, this->m_itemHeight * row / 2 + 1, item.c_str(), 0, 0, 12, this->m_colors[index], 0, 0);
  }
}

// OFFSET: 0x008EF0A0
// FUNCTION: fn_12231
void UIMinimapZoomWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  int column = 0;
  int row = 0;
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->PlayerCell(&column, &row);
  const std::string text = FormatOwnedString(" %d  %d", column, row);
  this->DrawTextAt(19, 128, text.c_str(), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(20, 128, text.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
}

// INLINED
static __forceinline UIBitmapButton* AddNumberedButtonInline(UIFrameWnd* frame, int id, int x, int y, const char* name, const char* tooltip, bool prefix) {
  if (!name) {
    return nullptr;
  }

  UIBitmapButton* button = new UIBitmapButton();
  std::string path;
  if (prefix) {
    path += "유저인터페이스\\";
  }
  path += name;
  button->SetBitmap((path + "_1" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((path + "_2" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((path + "_3" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(id);
  button->SetTooltip(tooltip);
  frame->AddChild(button);
  return button;
}

// OFFSET: 0x008E3340
// FUNCTION: fn_12182
void UIMinimapZoomWnd::OnCreate(int, int) {
  const int ys[5] = {128, 128, 128, 128, 128};
  const int xs[5] = {1, this->geometry.width - 55, this->geometry.width - 41, this->geometry.width - 27, this->geometry.width - 13};
  const int ids[5] = {452, 216, 217, 189, 454};
  const char* tooltips[5] = {LookupMessage(MSI_MINIMAP_TIP_MAP_VIEW), LookupMessage(MSI_MINIMAP_TIP_WORLD_VIEW), LookupMessage(MSI_INVENTORY_NUMBER_OF_ITEM), LookupMessage(MSI_MINIMAP_BUTTON_ONOFF), LookupMessage(MSI_MINIMAP_BUTTON_ON)};
  const char* names[5] = {"object", "plus", "minus", "mini", "viewon"};
  const std::string prefix("minimap\\i_");

  auto& buttons = *reinterpret_cast<std::vector<UIWindow*>*>(&this->townButtonsStorage);
  buttons.clear();
  for (int i = 0; i < 5; ++i) {
    const std::string name = prefix + names[i];
    UIWindow* button = AddNumberedButtonInline(this, ids[i], xs[i], ys[i], name.c_str(), tooltips[i], true);
    buttons.push_back(button);
  }

  this->SendMsg(this, UM_COMMAND, 502, 0, 0, 0);

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
  if (message != UM_COMMAND) {
    return this->UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_BUTTON1:
    if (!g_session.IsNavigationAllowed()) {
      g_windowMgr.Confirm(LookupMessage(MSI_BANK_0_MONEY), 0, true, false, 0, "Message", 280, 120, 0);
    } else if (!g_windowMgr.CloseWindow(WID_MINIMAPWND)) {
      g_windowMgr.EnsureWindow(WID_MINIMAPWND);
    }
    return 0;
  case ID_PLUS:
    g_windowMgr.minimapZoom += (g_windowMgr.minimapZoom / 3.0f + g_windowMgr.minimapZoom) * 0.25f;
    if (g_windowMgr.minimapZoom > 4.0f) {
      g_windowMgr.minimapZoom = 4.0f;
    }
    return 0;
  case ID_MINUS:
    g_windowMgr.minimapZoom -= g_windowMgr.minimapZoom * 0.25f;
    if (g_windowMgr.minimapZoom < 1.0f) {
      g_windowMgr.minimapZoom = 1.0f;
    }
    return 0;
  case 452:
    g_windowMgr.minimapMarkersVisible = g_windowMgr.minimapMarkersVisible == 0;
    for (auto it = this->m_zoom.begin(); it != this->m_zoom.end(); ++it) {
      static_cast<UIButton*>(it->second)->Enable(g_windowMgr.minimapMarkersVisible);
    }
    this->Invalidate();
    return 0;
  case 454:
    if (!g_windowMgr.CloseWindow(WID_ROMAPWND)) {
      g_windowMgr.EnsureWindow(WID_ROMAPWND);
    }
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
    const auto& buttons = *reinterpret_cast<std::vector<UIWindow*>*>(&this->townButtonsStorage);
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
    g_navigationManager.StartNavigation(mapName.c_str(), 0, 5, true, town.x, town.y, 1002);
  }
  return 0;
}

// OFFSET: 0x0090C020
// FUNCTION: fn_12437
void UIMinimapZoomWnd::AddTownButton(int index, TownInfo town) {
  UIBitmapButton* button = new UIBitmapButton();
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
  this->DrawTitleBar(true, "", 0);
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
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, sprite->palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
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
      x, y, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f), width, height, cell.mirrored != 0, image->doubleWidth + 1,
      image->doubleHeight + 1, 0, 0
    );
  }
}

// OFFSET: 0x008EF590
// FUNCTION: fn_12233
void UINoticeConfirmWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap, false);
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
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(15, 6, g_serviceType == 12 ? "Insira o Numero" : "Input number ", 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EF850
// FUNCTION: fn_12238
void UINpcTextEditDialogWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  if (!this->m_tagCtrl) {
    return;
  }

  this->m_tagCtrl->SetTaggedText(std::string(this->edit->Text()), this->edit->geometry.width);
  this->m_tagCtrl->UpdateItemTags();
  if (this->m_tagCtrl->previousTagCount == 0) {
    delete this->m_tagCtrl;
    this->m_tagCtrl = nullptr;
  }
}

// OFFSET: 0x008FFC90
// FUNCTION: fn_12365
int UINpcEditDialogWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (g_windowMgr.rodexComposeWnd) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UNEQUIP_ARROW2)), 0x64FFFF, 0, 0, 0);
    return 0;
  }

  switch (message) {
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
    if (argument1 == ID_OK && this->edit->Length()) {
      g_modeMgr.gameMode->SendMsg(MM_SEND_NUMBER_TO_NPC, this->npcId, std::atoi(this->edit->Text()), 0, 0);
      g_windowMgr.CloseWindow(WID_NPCEDITDIALOGWND);
    }
    return 0;
  }
  case UM_SETNAID: {
    this->npcId = static_cast<int>(argument1);
    return 0;
  }
  }

  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008FFD70
// FUNCTION: fn_12366
int UINpcTextEditDialogWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    if (g_windowMgr.rodexComposeWnd) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UNEQUIP_ARROW2)), 0x64FFFF, 0, 0, 0);
      return 0;
    }

    if (message == UM_SETNAID) {
      this->npcId = static_cast<int>(argument1);
      return 0;
    }

    return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->RecordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
  if (Replay::Instance().state == 7) {
    this->edit->SetText("임시");
  }

  if (argument1 != ID_OK || !this->edit->Length()) {
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
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BAD_SENTANGE)), 0xFF, 0, 0, 0);
    return 0;
  }

  CMode* mode = g_modeMgr.gameMode;
  if (g_navigationLinkParser->HasLink(std::string(this->edit->Text()))
      || (!this->edit->LinkedText().empty() && this->m_tagCtrl && !this->m_tagCtrl->linkedText.empty())) {
    this->edit->SetText("");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CLOSE)), 0xFF, 0, 0, 0);
    return 0;
  }

  if (!this->edit->LinkedText().empty()) {
    mode->SendMsg(MM_SEND_TEXT_TO_NPC, this->npcId, reinterpret_cast<int>(this->edit->Text()), reinterpret_cast<int>(this->edit->LinkedText().c_str()), 0);
  } else if (this->m_tagCtrl && !this->m_tagCtrl->linkedText.empty()) {
    if (g_navigationLinkParser->HasUnownedItemLink(std::string(this->m_tagCtrl->linkedText.c_str())) == true) {
      this->edit->SetText("");
      return 0;
    }
    mode->SendMsg(MM_SEND_TEXT_TO_NPC, this->npcId, reinterpret_cast<int>(this->edit->Text()), reinterpret_cast<int>(this->m_tagCtrl->linkedText.c_str()), 0);
  } else {
    mode->SendMsg(MM_SEND_TEXT_TO_NPC, this->npcId, reinterpret_cast<int>(this->edit->Text()), 0, 0);
  }

  g_windowMgr.CloseWindow(WID_NPCTEXTEDITDIALOGWND);
  return 0;
}

// OFFSET: 0x008EF9A0
// FUNCTION: fn_12239
void UIPartyInvitationToWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_INVITE_PARTY), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(10, 22, LookupMessage(MSI_PUT_PLAYER_NAME_IN), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EFA00
// FUNCTION: fn_12240
void UIPartySettingWnd::Paint() {
  const char* title = LookupMessage(this->m_mode != 50 ? 291 : 2054);
  const int titleHeight = this->DrawTitleBar(true, title, 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);

  if (this->m_mode == 70) {
    this->DrawTextAt(10, 22, LookupMessage(MSI_HOWEXPDIV), 0, 0, 12, 0, 0, 0);
  } else if (this->m_mode == 50) {
    this->DrawTextAt(10, 22, LookupMessage(MSI_PUT_PARTY_NAME_IN), 0, 0, 12, 0, 0, 0);
  }

  this->DrawTextAt(10, this->m_mode + 82, LookupMessage(MSI_HOWITEMDIV), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(10, this->m_mode + 17, LookupMessage(MSI_HOWITEMCOLLECT), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008EFAF0
// FUNCTION: fn_12241
void UIPasswordWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x009002D0
// FUNCTION: fn_12367
int UIPartyInvitationToWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == ID_OK) {
    std::string name;
    name = this->m_edit->Text();
    this->m_edit->SelectAll();
    if (std::strlen(name.c_str()) != 0) {
      g_modeMgr.GetGameMode()->SendMsg(MM_REQ_JOIN_GROUP, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
    } else {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PLEASE_PUT_CHARACTER_NAME_IN)), 0xFF, 0, 0, 0);
    }
  } else if (argument1 == ID_CANCEL) {
    g_windowMgr.CloseWindow(WID_PARTYINVITATIONTOWND);
  }

  return 0;
}

// OFFSET: 0x00900490
// FUNCTION: fn_12368
int UIPartySettingWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      if (this->m_mode != 50) {
        g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
      }
      CMode* mode = g_modeMgr.GetGameMode();
      if (g_session.PartyCount() != 0) {
        if (g_session.partyLeader != 0) {
          if (this->m_expShare != g_session.partyExpShare || this->m_itemShare != g_session.partyItemCollect ||
              this->m_itemDistribute != g_session.partyItemDistribute) {
            mode->SendMsg(MM_GROUPINFO_CHANGE_V2, this->m_expShare, this->m_itemShare, this->m_itemDistribute, 0);
          }
        }
        return 0;
      }
      if (this->m_mode != 50) {
        mode->SendMsg(MM_CONFORM_MAKEGROUP, 0, this->m_itemShare, this->m_itemDistribute, 0);
        return 0;
      }

      std::string name = this->m_nameEdit->Text();
      if (name.find('#') != std::string::npos) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVENTORY_ITEM_COMPARE)), 0xFF, 0, 0, 0);
      } else if (name.find(s_partyItemLinkOpen) != std::string::npos || name.find(s_partyItemLinkClose) != std::string::npos) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON)), 0xFF, 0, 0, 0);
      } else if (std::strlen(name.c_str()) != 0) {
        mode->SendMsg(MM_WHISPER_FILTER, reinterpret_cast<std::intptr_t>(name.c_str()), 0, 0, 0);
        g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
        g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_HEIGHT_220);
        mode->SendMsg(MM_CONFORM_MAKEGROUP, 0, this->m_itemShare, this->m_itemDistribute, 0);
      } else {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PLEASE_PUT_PARTY_NAME_IN)), 0xFF, 0, 0, 0);
      }
      return 0;
    }
    case ID_CANCEL:
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
      g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_HEIGHT_220);
      return 0;
    case ID_RADIOBTN:
      if (g_session.PartyCount() == 0 || g_session.partyLeader != 0) {
        if (sender == this->m_expShareButtons[0] || sender == this->m_expShareButtons[1]) {
          for (int i = 0; i < 2; ++i) {
            if (sender == this->m_expShareButtons[i]) {
              this->m_expShare = i;
              this->m_expShareButtons[i]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
            } else {
              this->m_expShareButtons[i]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
            }
          }
        }
        if (sender == this->m_itemShareButtons[0] || sender == this->m_itemShareButtons[1]) {
          for (int i = 0; i < 2; ++i) {
            if (sender == this->m_itemShareButtons[i]) {
              this->m_itemShare = i;
              this->m_itemShareButtons[i]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
            } else {
              this->m_itemShareButtons[i]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
            }
          }
        }
      } else {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ONLY_PARTYMASTER)), 0xFFFF64, 10, 0, 0);
      }
      return 0;
    default:
      return 0;
    }
  case UM_SETINFO:
    this->m_placement = reinterpret_cast<WindowPlacement*>(a);
    if (!static_cast<PartySettingWndInfo*>(this->m_placement)->IsValid()) {
      static_cast<PartySettingWndInfo*>(this->m_placement)->Reset();
    }
    this->Move(this->m_placement->x, this->m_placement->y);
    break;
  case UM_REFRESH:
    if (this->m_mode == 5) {
      g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
    } else {
      this->Invalidate();
      this->m_expShare = g_session.partyExpShare;
      this->m_itemShare = g_session.partyItemCollect;
      this->m_itemDistribute = g_session.partyItemDistribute;
      for (int i = 0; i < 2; ++i) {
        this->m_expShareButtons[i]->SendMsg(this, UM_SETSTATE, this->m_expShare == i, 0, 0, 0);
        if (this->m_itemShareButtons[i]) {
          this->m_itemShareButtons[i]->SendMsg(this, UM_SETSTATE, this->m_itemShare == i, 0, 0, 0);
        }
      }
      if (this->m_itemDistributeButton) {
        this->m_itemDistributeButton->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
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
  case UM_COMMAND:
    if (a == ID_OK) {
      CMode* mode = g_modeMgr.gameMode;
      mode->SendMsg(MM_REQ_ENTER_ROOM, this->roomId, reinterpret_cast<int>(this->m_edit->Text()), 0, 0);
      g_windowMgr.CloseWindow(WID_PASSWORDWND);
    }
    return 0;
  case UM_SETROOMID:
    this->roomId = static_cast<int>(a);
    return 0;
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008EFCF0
// FUNCTION: fn_12244
void UIProhibitListWnd::Paint() {
  char text[256];

  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_PROHIBIT_LIST), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - left->image.height, left, false);
  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - right->image.width, this->geometry.height - right->image.height, right, false);

  int y = 0;
  const int count = static_cast<int>(g_session.m_prohibitionLog.size());
  for (int index = 0; index < count; ++index) {
    const ProhibitionLogEntry& entry = g_session.m_prohibitionLog[index];
    if (std::strcmp(entry.ownerName, g_session.m_serverName.c_str()) != 0 || entry.aid != this->m_aid) {
      continue;
    }

    if (y == 0) {
      std::sprintf(text, "AID : %d        CharacterName : %s", entry.aid, entry.name);
      this->DrawTextAt(10, 25, text, 0, 0, 12, 0, 0, 0);
      std::sprintf(text, "ServerName : %s", g_session.m_prohibitionLog[index].ownerName);
      this->DrawTextAt(10, 41, text, 0, 0, 12, 0, 0, 0);
    }

    const ProhibitionLogEntry& record = g_session.m_prohibitionLog[index];
    const int date = static_cast<int>(record.date);
    const int time = static_cast<int>(record.time);
    std::sprintf(
      text, "ProhibitTime : %d분   LogTime : %d년 %d월 %d일   %d시 %d분 %d초", record.minutes, date >> 16 & 0xFFFF, date >> 8 & 0xFF, date & 0xFF, time >> 16 & 0xFF,
      time >> 8 & 0xFF, time & 0xFF
    );
    this->DrawTextAt(10, y + 65, text, 0, 0, 12, 0, 0, 0);
    y += 18;
  }
}

// OFFSET: 0x008EFF60
// FUNCTION: fn_12245
void UIQuitWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap, false);
}

// OFFSET: 0x008EFFA0
// FUNCTION: fn_12246
void UIRestartWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, bitmap, false);
}

// OFFSET: 0x008EFFE0
// FUNCTION: fn_12247
void UISayDialogWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x008F0350
// FUNCTION: fn_12249
void UISelectCartWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_CART_LIST), 0);
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
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(15, 6, this->m_name.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x00905320
// FUNCTION: fn_12381
int UISpellListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_ADDITEM: {
    this->m_skills.push_back(static_cast<std::uint32_t>(argument1));
    ITEM_INFO item;
    this->m_items.push_back(item);
    this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    return 0;
  }
  case UM_SETCHOOSE:
    this->m_sourceSkillId = static_cast<short>(argument1);
    return 0;
  case UM_REFRESH:
    this->UpdateScroll();
    this->Invalidate();
    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_scroll;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
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
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
    CMode* mode = g_modeMgr.gameMode;
    if (argument1 == ID_OK) {
      const int selected = this->m_selected;
      if (selected < 0 || static_cast<unsigned>(selected) >= this->m_skills.size()) {
        return 0;
      }
      if (this->m_sourceSkillId != 0) {
        mode->SendMsg(MM_SKILL_SELECT_RESPONSE, this->m_skills[selected], this->m_sourceSkillId, 0, 0);
      } else {
        mode->SendMsg(MM_SELECT_SPELL, this->m_skills[selected], 0, 0, 0);
      }
    } else if (argument1 == ID_CANCEL) {
      if (this->m_sourceSkillId != 0) {
        mode->SendMsg(MM_SKILL_SELECT_RESPONSE, 0, this->m_sourceSkillId, 0, 0);
      } else {
        mode->SendMsg(MM_SELECT_SPELL, 0, 0, 0, 0);
      }
    } else {
      return 0;
    }
    g_windowMgr.CloseWindow(WID_SPELLLISTWND);
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
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_SPELL_LIST), 0);
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
    this->DrawTextAt(40, y + 4, name, 0, 0, 12, 0, 0, 0);

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
  this->DrawTitleBar(true, LookupMessage(MSI_TIPOFTHEDAY), 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
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
  this->DrawBitmap(0, 0, bitmap, false);
}

// OFFSET: 0x008F2AD0
// FUNCTION: fn_12259
void UIWhisperWnd::Paint() {
  char title[128];

  if (!this->m_code.empty()) {
    std::sprintf(title, "With  %s [%s]", this->m_name.c_str(), this->m_code.c_str());
  } else {
    std::sprintf(title, "With  %s", this->m_name.c_str());
  }

  DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xC0C0C0);
  this->FillRegion(1, this->geometry.height - 20, this->geometry.width - 2, 1, 0xC0C0C0);
  this->DrawTextAt(10, 3, title, 0, 0, 12, 0xFFFFFF, 0, 0);
}

// OFFSET: 0x008F2DE0
// FUNCTION: fn_12261
void UInCash_CallWnd::Paint() {
}

// OFFSET: 0x008F32D0
// FUNCTION: fn_12264
void UIItemIdentifyWnd::OnLBtnDblClk(int x, int y) {
  this->OnLBtnDown(x, y);
  this->SendMsg(nullptr, UM_COMMAND, ID_OK, 0, 0, 0);
}

// OFFSET: 0x008F0750
// FUNCTION: fn_12253
void UIStatusWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_STATUS), 0);
  if (this->geometry.height == this->m_titleHeight || this->geometry.state != 0) {
    return;
  }

  const char* background = this->m_extended == 1 ? "유저인터페이스\\statuswnd\\w_ex_statwin_bg.bmp" : "유저인터페이스\\statuswnd\\w_statwin_bg.bmp";
  if (CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)))) {
    this->DrawBitmap(0, this->m_titleHeight, bitmap, true);
  }

  const int stats[6][2] = {
    {g_session.m_standardStats[0], g_session.strengthBonus},
    {g_session.m_standardStats[1], g_session.agilityBonus},
    {g_session.m_standardStats[2], g_session.vitalityBonus},
    {g_session.m_standardStats[3], g_session.intelligenceBonus},
    {g_session.m_standardStats[4], g_session.dexterityBonus},
    {g_session.m_standardStats[5], g_session.luckBonus},
  };
  const int* entry = stats[0];
  for (int y = 23; y < 119; y += 16, entry += 2) {
    const int value = entry[0];
    this->DrawTextAt(36, y, FormatOwnedString("%d", value).c_str(), 0, 1, 14, 0, 0, 0);
    const int bonus = entry[1];
    if (bonus > 0) {
      if (value < 100) {
        this->DrawTextAt(49, y, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 14, 0, 0, 0);
      } else {
        this->DrawTextAt(54, y + 2, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 12, 0, 0, 0);
      }
    } else if (bonus < 0) {
      if (value < 100) {
        this->DrawTextAt(49, y, FormatOwnedString("%d", bonus).c_str(), 0, 1, 14, 0, 0, 0);
      } else {
        this->DrawTextAt(54, y + 2, FormatOwnedString("%d", bonus).c_str(), 0, 1, 12, 0, 0, 0);
      }
    }
  }

  const int* derived = reinterpret_cast<const int*>(g_session.combatStats);
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
  this->DrawRightAlignedText(275, 86, FormatOwnedString("%d", *reinterpret_cast<const int*>(g_session.statusPoints)).c_str(), 0, 1, 14, 0, 0);
  if (g_guildInfo.guildName.size() != 0) {
    this->DrawRightAlignedText(275, 102, g_guildInfo.guildName.c_str(), 0, 0, 12, 0, 0);
  }

  if (this->m_extended != 1) {
    return;
  }

  const int* traitBonus = g_session.m_traits.bonus;
  const std::int64_t* traitPoints = g_session.m_traits.points;
  const int bonuses[6] = {traitBonus[0], traitBonus[1], traitBonus[2], traitBonus[3], traitBonus[4], traitBonus[5]};
  const std::int64_t points[6] = {traitPoints[0], traitPoints[1], traitPoints[2], traitPoints[3], traitPoints[4], traitPoints[5]};
  const int types[6] = {219, 220, 221, 222, 223, 224};
  const std::int64_t* point = points;
  int index = 0;
  for (int y = 144; y < 240; y += 16, point += 1, ++index) {
    const int value = g_session.GetTraitStat(types[index]);
    this->DrawTextAt(36, y, FormatOwnedString("%d", value).c_str(), 0, 1, 14, 0, 0, 0);
    this->DrawRightAlignedText(101, y, FormatOwnedString("%lld", *point).c_str(), 0, 1, 14, 0, 0);
    const int bonus = bonuses[index];
    if (bonus > 0) {
      if (value < 100) {
        this->DrawTextAt(49, y, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 14, 0, 0, 0);
      } else {
        this->DrawTextAt(54, y + 2, FormatOwnedString("+%d", bonus).c_str(), 0, 1, 12, 0, 0, 0);
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
void UIStatusWnd::OnLBtnDblClk(int, int y) {
  if (y < 17) {
    this->SendMsg(this, UM_COMMAND, ID_MINI, 0, 0, 0);
  }
}

// OFFSET: 0x008F3330
// FUNCTION: fn_12266
void UIWhisperWnd::OnLBtnDblClk(int, int y) {
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
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("C-Code copied to clipboard"), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x008F3630
// FUNCTION: fn_12268
void UIBookWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x008F3650
// FUNCTION: fn_12269
void UICombinedCardItemCollection_ComparisonWnd::OnLBtnDown(int, int) {
}

// OFFSET: 0x008F38D0
// FUNCTION: fn_12272
void UIItemCollection_ComparisonWnd::OnLBtnDown(int, int) {
}

// OFFSET: 0x008F38E0
// FUNCTION: fn_12273
void UIItemIdentifyWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
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
void UIMinimapZoomWnd::OnLBtnDown(int, int) {
}

// OFFSET: 0x008F39C0
// FUNCTION: fn_12275
void UIPartySettingWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x008F39E0
// FUNCTION: fn_12276
void UIPetTamingDeceiveWnd::OnLBtnDown(int, int) {
  if (g_replayAnimationScaling != 0 || this->m_started != 0) {
    return;
  }

  this->m_state = 1;
  this->m_frame = 7;
  this->m_started = 1;
  g_modeMgr.GetGameMode()->SendMsg(MM_PETDECEIVE_SHOOT, this->m_target, 0, 0, 0);
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
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, sprite->palette, record, g_serviceType, g_resourceReuseDelay);
    }
    this->geometry.surface->DrawSurface(
      this->geometry.width / 2 + cell.x, this->geometry.height / 2 + cell.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->doubleWidth + 1, image->doubleHeight + 1, 0, 0
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
void UISelectCartWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x008CE2C0
// FUNCTION: fn_12069
void UISelectCartWnd::AddCart(int index, int type) {
  const std::string name = CartTypeName(type);
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
  UISpriteButton* button = new UISpriteButton();
  button->SetSprite((std::string("이팩트\\") + name + std::string(".spr")).c_str());
  button->SetActName((std::string("이팩트\\") + name + std::string(".act")).c_str());
  button->SetSize(100, 100);
  button->Move(x, y);
  button->animationIndex = 0;
  button->frameIndex = 0;
  button->SetId(type + 9000);
  this->AddChild(button);
}

// OFFSET: 0x008F3860
// FUNCTION: fn_12271
void UIItemCollectionWnd::OnLBtnDown(int x, int y) {
  if (x <= 60 || y >= 17) {
    return;
  }

  this->UIFrameWnd::OnLBtnDown(x, y);
  if (this->m_kind == 12) {
    UIWindow* comparison = g_windowMgr.itemCollectionComparisonWnd;
    if (!comparison) {
      return;
    }
    const int targetAlpha = this->geometry.targetAlpha;
    const int alphaStart = this->geometry.alphaStart;
    comparison->geometry.targetAlpha = targetAlpha;
    comparison->geometry.alphaStart = alphaStart;
  }

  if (this->m_kind == 82) {
    UIWindow* comparison = g_windowMgr.combinedCardItemCollectionComparisonWnd;
    if (comparison) {
      const int targetAlpha = this->geometry.targetAlpha;
      const int alphaStart = this->geometry.alphaStart;
      comparison->geometry.targetAlpha = targetAlpha;
      comparison->geometry.alphaStart = alphaStart;
    }
  }
}

// OFFSET: 0x008F3A60
// FUNCTION: fn_12278
void UIStatusWnd::OnLBtnDown(int x, int y) {
  if (y < this->m_titleHeight) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x008F3BA0
// FUNCTION: fn_12281
void UICombinedCardItemCollection_ComparisonWnd::OnLBtnUp(int, int) {
}

// OFFSET: 0x008F3B70
// FUNCTION: fn_12280
void UICombinedCardItemCollectionWnd::OnLBtnUp(int x, int y) {
  this->UIFrameWnd::OnLBtnUp(x, y);
  if (UIWindow* comparison = g_windowMgr.combinedCardItemCollectionComparisonWnd) {
    const int targetAlpha = this->geometry.targetAlpha;
    const int alphaStart = this->geometry.alphaStart;
    comparison->geometry.targetAlpha = targetAlpha;
    comparison->geometry.alphaStart = alphaStart;
  }
}

// OFFSET: 0x008F3BB0
// FUNCTION: fn_12282
void UIEquipWnd::OnLBtnUp(int x, int y) {
  this->UIFrameWnd::OnLBtnUp(x, y);

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
void UIItemCollectionWnd::OnLBtnUp(int x, int y) {
  this->UIFrameWnd::OnLBtnUp(x, y);
  if (UIWindow* comparison = g_windowMgr.itemCollectionComparisonWnd) {
    const int targetAlpha = this->geometry.targetAlpha;
    const int alphaStart = this->geometry.alphaStart;
    comparison->geometry.targetAlpha = targetAlpha;
    comparison->geometry.alphaStart = alphaStart;
  }
}

// OFFSET: 0x008F3C50
// FUNCTION: fn_12284
void UIItemCollection_ComparisonWnd::OnLBtnUp(int, int) {
}

// OFFSET: 0x008F3C60
// FUNCTION: fn_12285
void UIMenuWnd::OnLBtnUp(int x, int y) {
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
  UIWindow* target = this->m_menuId == 301 ? g_windowMgr.FindSnapExcludedWindow(static_cast<int>(this->m_targetWindowId))
                                           : g_windowMgr.LookupWindow(this->m_menuId);
  if (target) {
    target->SendMsg(nullptr, UM_MENU_SELECTED, choice, 0, 0, 0);
  } else if (this->m_menuId == 10041) {
    g_modeMgr.gameMode->SendMsg(MM_MENUSELECTED, choice, 0, 0, 0);
  }
  g_windowMgr.CloseWindow(WID_MENUWND);
}

// OFFSET: 0x008F4520
// FUNCTION: fn_12288
void UIItemCollectionWnd::OnMouseHover(int x, int y) {
  int slotX;
  int slotY;
  const int index = this->CardSlotAt(x, y, &slotX, &slotY);
  if (index == -1 || this->m_compare != 1) {
    return;
  }

  ITEM_INFO card;
  int cardId;
  std::memcpy(&cardId, this->m_item.cards + index * sizeof(cardId), sizeof(cardId));
  card.identified = 1;
  card.identity[0] = 6;
  card.refiningLevel = 0;
  card.SetItemId(cardId);

  CMode* tooltip = g_modeMgr.gameMode;
  std::uint32_t color = 0xFFFFFFFF;
  if (index >= ItemSlotsCount(this->m_item)) {
    color = ItemCardColor(&this->m_item);
  }

  tooltip->ShowTooltip(card.DisplayName(0).c_str(), slotX, slotY, color, 0, 1);
}

// OFFSET: 0x008F46C0
// FUNCTION: fn_12290
void UIStatusWnd::OnMouseHover(int x, int y) {
  if (this->geometry.state != 0) {
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
    LookupMessage(MSI_DESC_INT2), LookupMessage(MSI_DESC_DEX2), LookupMessage(MSI_DESC_LUK2), LookupMessage(MSI_DESC_ATK2), LookupMessage(MSI_DESC_DEF2), LookupMessage(MSI_DESC_HIT2),
    LookupMessage(MSI_DESC_CRI2), LookupMessage(MSI_DESC_ASPD2), LookupMessage(MSI_DESC_POINT2), LookupMessage(MSI_DESC_MATK2), LookupMessage(MSI_DESC_FLEE2), LookupMessage(MSI_DESC_MDEF2),
    LookupMessage(MSI_DESC_GUILD2), LookupMessage(MSI_HOTKEY_GUIDE), LookupMessage(MSI_OPTION), LookupMessage(MSI_SKILL_HOTKEY_F1_F9),
  };

  for (int index = 0; index < 16; ++index) {
    const Point& point = stats[index];
    if (point.x <= x && x <= point.x + 80 && point.y <= y && y <= point.y + 12) {
      int tipX = point.x - this->m_offsetX;
      int tipY = this->m_titleHeight - this->m_offsetY - 19 + point.y;
      this->ToScreen(tipX, tipY);
      g_modeMgr.gameMode->ShowTooltip(statTexts[index], tipX, tipY, -1, true, true);
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
    LookupMessage(MSI_DESC_SPL), LookupMessage(MSI_DESC_CON), LookupMessage(MSI_DESC_CRT), LookupMessage(MSI_DESC_ADV_STATUS_POWER_ATK), LookupMessage(MSI_DESC_ADV_STATUS_SPELL_MATK), LookupMessage(MSI_DESC_ADV_STATUS_STAMINA_RESIST), LookupMessage(MSI_DESC_ADV_STATUS_WISDOM_MRESIST),
    LookupMessage(MSI_DESC_ADV_STATUS_CREATIVE_HEALPULS), LookupMessage(MSI_BASICINFO_JOBLEVEL2), LookupMessage(MSI_BASICINFO_AP), LookupMessage(MSI_GUILD_AGIT_MOVE_AGIT), LookupMessage(MSI_DESC_ADV_STATUS_CREATIVE_CRIDAMAGE_RATE), LookupMessage(MSI_DESC_ADV_POINT),
  };

  for (int index = 0; index < 13; ++index) {
    const Point& point = traits[index];
    if (point.x <= x && x <= point.x + 80 && point.y <= y && y <= point.y + 12) {
      int tipX = point.x - 16;
      int tipY = point.y - 13;
      this->ToScreen(tipX, tipY);
      g_modeMgr.gameMode->ShowTooltip(traitTexts[index], tipX, tipY, -1, true, true);
      return;
    }
  }
}

// OFFSET: 0x008F4A80
// FUNCTION: fn_12291
void UIEquipWnd::OnMouseMove(int x, int y) {
  this->UIFrameWnd::OnMouseMove(x, y);
  this->OnMouseHover(x, y);
}

// OFFSET: 0x008F4AB0
// FUNCTION: fn_12292
void UIMenuWnd::OnMouseMove(int x, int y) {
  this->OnMouseHover(x, y);
}

// OFFSET: 0x008F4680
// FUNCTION: fn_12289
void UIMenuWnd::OnMouseHover(int x, int y) {
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
  const unsigned index = row >= 0 ? row : 0;
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
    ITEM_INFO item = g_session.GetEquippedItemAt(index);
    if (item.identity[1] > 0 && item.quantity == 1 && (item.identity[2] & location) != 0) {
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
  case UM_ADDBOUNDARY: {
    const std::uint32_t color = 0;
    this->AddItem(g_menuSeparator, color);
    this->Invalidate();
    return 0;
  }
  case UM_ADDITEM: {
    std::uint32_t color = static_cast<std::uint32_t>(b);
    this->AddItem(reinterpret_cast<const char*>(a), color);
    color = static_cast<std::uint32_t>(b);
    this->AddItem(reinterpret_cast<const char*>(a), color);
    this->Invalidate();
    return 0;
  }
  case UM_SET_CALLER_WID:
    this->m_menuId = static_cast<int>(a);
    this->m_targetWindowId = static_cast<std::uint32_t>(b);
    return 0;
  case UM_ADJUSTSIZE: {
    int width = 20;
    const int count = static_cast<int>(this->m_items.end() - this->m_items.begin());
    for (int index = 0; index < count; ++index) {
      const int textWidth = this->TextWidth(this->m_items[index].c_str(), 0, 0, 12, 0, 0);
      if (width < textWidth) {
        width = textWidth;
      }
    }
    this->Resize(width + 6, this->m_itemHeight * count / 2);
    this->Invalidate();
    return 0;
  }
  case UM_CLEAR_ITEM:
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
  this->SendMsg(this, UM_COMMAND, 543, 0, 0, 0);
  g_windowMgr.Promote(this);
}

// OFFSET: 0x008F5AF0
// FUNCTION: fn_12313
void UITipboxWnd::UpdatePageText() {
  this->m_pageText->SetText(IntToString((std::max)(0, g_itemTagOwner->paragraphIndex) + 1, 0).c_str(), 0);
  this->m_pageText->SetFont(0, 12, 1, 0);

  const std::string separator = "/";
  const int pages = static_cast<int>(g_itemTagOwner->m_currentEntry.paragraphs.size());
  this->m_pageTotalText->SetText((separator + IntToString(pages, 0)).c_str(), 0);

  const int pageWidth = this->ColoredTextWidth(
    IntToString((std::max)(0, g_itemTagOwner->paragraphIndex) + 1, 0).c_str(),
    std::strlen(IntToString((std::max)(0, g_itemTagOwner->paragraphIndex) + 1, 0).c_str()), 0, 12, 1
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
void UIExchangeWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (this->isLocalTrader != 0) {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 1) {
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 5, 0, 0, 0);
    return;
  }

  mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x008D0010
// FUNCTION: fn_12082
void UIEquipWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (this->m_viewOther != 0) {
    return;
  }

  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
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
    item = g_session.GetSwapCostumeItem(drag->slot);
    break;
  case 18:
    fromStorage = true;
    item = g_session.GetSwapEquippedItem(drag->slot);
    break;
  }

  if (item.identity[1] > 0) {
    switch (item.identity[0]) {
    case 10:
    case 16:
    case 17:
    case 19:
      mode->SendMsg(MM_WEAR_EQUIP_ARROW, drag->id, item.identity[2], 0, 0);
      break;
    default:
      if (fromStorage == true) {
        mode->SendMsg(MM_SWAP_EQUIPMENT_BY_INDEX, reinterpret_cast<int>(&item), 0, 0, 0);
      } else if (item.identified && item.identity[2] != 0 && item.identity[3] == 0) {
        mode->SendMsg(MM_WEAR_EQUIP, drag->id, item.identity[2], 0, 0);
      }
      break;
    }
  }

  g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x008D02D0
// FUNCTION: fn_12084
void UInCashWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 4:
    if (x < 445) {
      const int quantity = g_session.GetItem1720QuantityByItemId(drag->name.c_str());
      if (quantity < 2) {
        this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
        return;
      }

      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 20, quantity, 0, 0);
      return;
    }
    break;
  case 6:
    if (x > 444) {
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 19, 0, 0, 0);
      return;
    }
    break;
  }

  mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x008D9F00
// FUNCTION: fn_12137
bool UISkillDescribeWnd::GetCustomTransBoxInfo(void*) {
  return true;
}

// OFFSET: 0x008D9F10
// FUNCTION: fn_12138
bool UIWhisperWnd::GetCustomTransBoxInfo(void*) {
  return true;
}

// OFFSET: 0x008DA220
// FUNCTION: fn_12141
void UIExchangeWnd::LayoutButtons() {
  const int x[3] = {3, 260, 510};
  const int y[3] = {this->isLocalTrader * 120, this->isLocalTrader != 0 && this->localConfirmed != 0 && this->localAccepted == 0 ? 0 : 120, 0};

  for (int index = 0; index < 3; ++index) {
    this->m_buttons[index]->Move(x[index], this->geometry.height - 23 + y[index]);
  }
  this->Invalidate();
}

// OFFSET: 0x008DA2E0
// FUNCTION: fn_12142
void UIEquipWnd::NotifyEquipmentChanged() {
  if (this->swapWindow) {
    this->swapWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x008DA420
// FUNCTION: fn_12143
bool UIMinimapZoomWnd::IsTransmitMouseInput() {
  return true;
}

// OFFSET: 0x008DA430
// FUNCTION: fn_12144
bool UIEquipWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x008DA440
// FUNCTION: fn_12145
bool UIStatusWnd::NeedsFrameRefresh() {
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

  if (!this->m_tagCtrl->CanInsertTag(item.DisplayName(0), this->edit->geometry.width)) {
    return;
  }

  {
    std::string text = this->edit->Text();
    this->m_tagCtrl->taggedText = text;
  }
  this->edit->SetText(this->m_tagCtrl->AppendTag(item.DisplayName(0)).c_str());
  this->edit->MoveCursorToEnd();
  this->m_tagCtrl->AddItemTag(item, 0x12B8A4);
}

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

  if (this->edit->Length() + this->edit->tagTextLength > this->edit->maximum) {
    return;
  }

  std::string tag = this->m_tagCtrl->AddSkillTag(tagId, 0xD630A9);
  {
    auto* const tagCtrl = this->m_tagCtrl;
    std::string text = this->edit->Text();
    tagCtrl->taggedText = text;
  }
  this->edit->SetText((this->edit->Text() + tag).c_str());
  this->edit->MoveCursorToEnd();
}

// OFFSET: 0x008DAF40
// FUNCTION: fn_12150
void UICombinedCardItemCollectionWnd::Move(int x, int y) {
  UIFrameWnd::Move(x, y);
  if (UIWindow* comparison = g_windowMgr.combinedCardItemCollectionComparisonWnd) {
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_COMBINEDCARDITEMCOLLECTIONWND)) {
      static_cast<UIFrameWnd*>(comparison)->UIFrameWnd::Move(window->geometry.x - window->geometry.width, window->geometry.y);
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
  if (UIWindow* comparison = g_windowMgr.itemCollectionComparisonWnd) {
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMCOLLECTIONWND)) {
      static_cast<UIFrameWnd*>(comparison)->UIFrameWnd::Move(window->geometry.x - window->geometry.width, window->geometry.y);
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
  UIWindow::Move(x, y);
}

// OFFSET: 0x008DB030
// FUNCTION: fn_12155
void UIWhisperWnd::Move(int x, int y) {
  UIFrameWnd::Move(x, y);
}

// OFFSET: 0x008F4D40
// FUNCTION: fn_12294
void UIEquipWnd::OnProcess() {
}

// OFFSET: 0x008F4F80
// FUNCTION: fn_12296
void UIItemCollectionWnd::OnProcess() {
  if (this->m_previewTick != 0 && timeGetTime() > this->m_previewTick + 100) {
    this->m_readButtons[1]->Invalidate();
    this->m_previewTick = timeGetTime();
  }
}

// OFFSET: 0x008F4FC0
// FUNCTION: fn_12297
void UINotifyItemObtainWnd::OnProcess() {
  if (timeGetTime() > this->m_shownTick + 5000) {
    g_windowMgr.CloseWindow(WID_NOTIFYITEMOBTAINWND);
  }
}

// OFFSET: 0x008F4FF0
// FUNCTION: fn_12298
void UIPetTamingDeceiveWnd::OnProcess() {
  this->Invalidate();
  const int frame = this->m_tick / this->m_delay;

  if (this->m_finishTime == 0xFFFFFFFF) {
    if (this->m_frame == frame && this->m_delay <= this->m_maxDelay) {
      this->m_finishTime = timeGetTime();
    }
  } else if (this->m_frame == frame && this->m_delay <= this->m_maxDelay && timeGetTime() - this->m_finishTime >= 2000) {
    g_windowMgr.CloseWindow(WID_PETTAMINGDECEIVEWND);
  }
}

// OFFSET: 0x008F5070
// FUNCTION: fn_12299
void UIProhibitListWnd::OnProcess() {
  const int count = static_cast<int>(g_session.m_prohibitionLog.size());
  if (this->m_count != count) {
    this->m_count = count;
    this->Invalidate();
  }
}

// OFFSET: 0x008F50B0
// FUNCTION: fn_12300
void UISayDialogWnd::OnProcess() {
  for (UIWindow* child : this->m_children) {
    child->OnProcess();
  }
}

// OFFSET: 0x008F5990
// FUNCTION: fn_12307
void UIItemIdentifyWnd::OnRBtnUp(int, int) {
}

// OFFSET: 0x008F59A0
// FUNCTION: fn_12308
void UIWhisperWnd::OnRBtnUp(int, int) {
  this->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
}

// OFFSET: 0x008F5A10
// FUNCTION: fn_12310
void UIExchangeWnd::OnWheel(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, 1, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, -1, 0, 0, 0);
  }
}

// OFFSET: 0x008F59C0
// FUNCTION: fn_12309
void UIEquipWnd::OnWheel(int delta) {
  if (g_session.damageSkinCount > 4 && this->m_page == 3) {
    if (delta < 0) {
      this->SendMsg(this->m_scrollBar, UM_SCROLLHORZ, 1, 0, 0, 0);
    } else {
      this->SendMsg(this->m_scrollBar, UM_SCROLLHORZ, -1, 0, 0, 0);
    }
  }
}

// OFFSET: 0x008F5A50
// FUNCTION: fn_12311
void UIItemIdentifyWnd::OnWheel(int delta) {
  if (this->m_scrollable == 0) {
    return;
  }

  const int step = (std::min)(this->m_rows - 1, 3);
  this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, delta < 0 ? step : -step, 0, 0, 0);
}

// OFFSET: 0x008F6370
// FUNCTION: fn_12315
void UIItemParamChangeDisplayWnd::SumEquippedStats() {
  this->m_before = 0;
  this->m_beforeSecond = 0;

  int locations = 0;
  for (int index = 0; index < 8; ++index) {
    ITEM_INFO item = g_session.GetEquippedItemAt(index);
    if (item.identity[1] > 0 && item.quantity == 1) {
      const ITEM_DESC& description = FindItemDescription(std::atoi(item.itemId.c_str()));
      const bool duplicate = (description.descriptionFlags & locations) != 0;
      locations |= description.descriptionFlags;
      if (duplicate) {
        continue;
      }

      if (description.descriptionValues.size() == 2) {
        this->m_before += description.descriptionValues[0];
        this->m_beforeSecond += description.descriptionValues[1];
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
  if (this->isLocalTrader == 0) {
    return;
  }

  if (this->zenyEdit) {
    g_windowMgr.SetFocus(nullptr);
    this->zenyEdit->Move(129, 500);
    this->zenyEdit = nullptr;
    if (g_windowMgr.chatWnd) {
      g_windowMgr.chatWnd->SendMsg(nullptr, UM_GET_EDIT_FOCUS, 0, 0, 0, 0);
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
  if (message != UM_SETINFO) {
    return this->UIItemCollectionWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_COMBINEDCARDITEMCOLLECTIONWND);
  if (window) {
    this->UIFrameWnd::Move(window->geometry.x - window->geometry.width, window->geometry.y);
  }
  return 0;
}

// OFFSET: 0x008F9460
// FUNCTION: fn_12335
int UICombinedCardItemCollectionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_SETINFO) {
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
  case UM_SETINFO: {
    ItemCompositionWndInfo* placement = reinterpret_cast<ItemCompositionWndInfo*>(a);
    this->m_compositionPlacement = placement;
    if (!placement->IsValid()) {
      placement->Reset();
    }
    this->Move(placement->x, placement->y);
    return 0;
  }
  case UM_SET_CARD_ITEM_INDEX:
    this->m_compositionState = static_cast<int>(a);
    if (a != 0) {
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(a));
      if (item.identity[1] != 0) {
        const std::string name = item.DisplayName(0);
        this->m_title = FormatOwnedString("%s (%s)", LookupMessage(MSI_ITEM_COMPOUNDING), name.c_str());
      }
    }
    return 0;
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, this->m_selected, d);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(c);
    }
    switch (a) {
    case ID_OK:
      if (this->m_selected != -1) {
        int found;
        ITEM_INFO item = this->GetItem(this->m_selected, &found);
        if (found != 0) {
          g_modeMgr.gameMode->SendMsg(MM_REQ_ITEMCOMPOSITION, item.identity[1], this->m_compositionState, 0, 0);
        } else {
          g_modeMgr.gameMode->SendMsg(MM_REQ_ITEMCOMPOSITION, -1, -1, 0, 0);
        }
        g_windowMgr.CloseWindow(WID_ITEMCOMPOSITIONWND);
      }
      break;
    case ID_CANCEL:
      g_modeMgr.gameMode->SendMsg(MM_REQ_ITEMCOMPOSITION, -1, -1, 0, 0);
      g_windowMgr.CloseWindow(WID_ITEMCOMPOSITIONWND);
      break;
    }
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x574E) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        switch (header.type) {
        case 0x574F:
          return 0;
        case 0x5751: {
            std::vector<unsigned char> packet(header.length, 0);
            std::memcpy(packet.data(), records + position, header.length);
            position += header.length;
            static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ReceiveCardCompositionItemList(packet.data());
          break;
        }
        default:
          position += header.length;
          break;
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
  case UM_SETINFO: {
    ItemIdentifyWndInfo* placement = reinterpret_cast<ItemIdentifyWndInfo*>(a);
    this->m_placement = placement;
    if (!placement->IsValid()) {
      placement->Reset();
    }
    this->Move(placement->x, placement->y);
    return 0;
  }
  case UM_CLEAR_ITEM:
    this->m_items.clear();
    return 0;
  case UM_REFRESH:
    this->UpdateScroll();
    this->Invalidate();
    return 0;
  case UM_ADDITEM: {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(b));
    this->m_items.push_back(item);
    this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    return 0;
  }
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, this->m_selected, d);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(c);
    }
    switch (a) {
    case ID_OK:
      if (this->m_selected != -1) {
        int found;
        ITEM_INFO item = this->GetItem(this->m_selected, &found);
        if (found != 0) {
          g_modeMgr.gameMode->SendMsg(MM_REQ_ITEMIDENTIFY, item.identity[1], 0, 0, 0);
        }
        g_windowMgr.CloseWindow(WID_ITEMIDENTIFYWND);
      }
      break;
    case ID_CANCEL:
      g_modeMgr.gameMode->SendMsg(MM_REQ_ITEMIDENTIFY, -1, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_ITEMIDENTIFYWND);
      break;
    }
    return 0;
  }
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_scroll;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
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
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x571C) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        switch (header.type) {
        case 0x571D:
          return 0;
        case 0x571F: {
          std::vector<unsigned char> packet(header.length, 0);
          std::memcpy(packet.data(), records + position, header.length);
          position += header.length;
          static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ReceiveItemIdentificationList(packet.data());
          break;
        }
        default:
          position += header.length;
          break;
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
  if (message != UM_SETINFO) {
    return this->UIItemCollectionWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMCOLLECTIONWND);
  if (window) {
    UIFrameWnd::Move(window->geometry.x - window->geometry.width, window->geometry.y);
  }
  return 0;
}

// OFFSET: 0x008F9570
// FUNCTION: fn_12337
int UIComboBoxIDWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (this->m_owner && message == UM_COMBOBOX_SELECTED) {
    return this->m_owner->SendMsg(this, UM_COMBOBOX_SELECTED, argument1, argument2, argument3, argument4);
  }
  return this->m_list->SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008F95B0
// FUNCTION: fn_12338
int UIComboBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (this->m_owner && message == UM_COMBOBOX_SELECTED) {
    return this->m_owner->SendMsg(this, UM_COMBOBOX_SELECTED, argument1, argument2, argument3, argument4);
  }
  return this->m_list->SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008F97D0
// FUNCTION: fn_12340
int UIDisconnectedServerMsgWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (argument1 == ID_OK) {
    g_modeMgr.GetGameMode()->SendMsg(MM_QUIT_TO_IDENTRY, 0, 0, 0, 0);
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
// FUNCTION: fn_12342
int UIEquipWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_TAB:
    this->m_page = a;
    if (a == 2) {
      this->m_titleList->Show(1);
      this->RefreshTitleList();
    } else {
      this->m_titleList->Show(0);
    }

    if (this->m_viewOther == 0) {
      if (this->m_page == 3) {
        static_cast<UIButton*>(this->damageSkinButtons[0])->Enable(1);
        static_cast<UIButton*>(this->damageSkinButtons[1])->Enable(1);
        if (g_session.damageSkinCount > 4) {
          this->m_scrollBar->Show(1);
        }
        for (int index = 2; index < 6; ++index) {
          this->damageSkinButtons[index]->Show(1);
        }
        this->allowViewCheck->Show(0);
        this->unequipButton->Enable(0);
        this->m_viewButton->Enable(0);
        this->m_otherButton->Enable(0);
      } else {
        static_cast<UIButton*>(this->damageSkinButtons[0])->Enable(0);
        static_cast<UIButton*>(this->damageSkinButtons[1])->Enable(0);
        this->m_scrollBar->Show(0);
        for (int index = 2; index < 6; ++index) {
          this->damageSkinButtons[index]->Show(0);
        }
        this->unequipButton->Enable(1);
        this->allowViewCheck->Show(1);
      }

      if (this->m_page == 1) {
        this->swapModeCheck->Show(1);
        this->allowViewCheck->Show(0);
      } else {
        this->swapModeCheck->Show(0);
        this->allowViewCheck->Show(1);
      }
    }

    if (this->m_page == 2 || this->m_page == 3) {
      if (this->swapWindow && this->swapWindow->geometry.state != 0) {
        this->SendMsg(nullptr, UM_COMMAND, 508, 0, 0, 0);
      }
      if (this->swapButton) {
        this->swapButton->SetState(0);
        this->swapButton->Enable(0);
      }
    } else if (this->swapButton) {
      this->swapButton->Enable(1);
      if (this->swapWindow) {
        if (g_session.m_swapEquipmentMode == 1) {
          this->swapWindow->geometry.state = 0;
          this->SendMsg(nullptr, UM_COMMAND, 508, 0, 0, 0);
        }
        static_cast<UISwapEquipmentWnd*>(this->swapWindow)->equipmentPage = this->m_page + 1;
        this->swapWindow->Invalidate();
      }
    }

    if (this->m_viewOther == 0) {
      switch (this->m_page) {
      case 0:
        this->allowViewCheck->Show(1);
        this->swapModeCheck->Show(0);
        this->swapButton->Enable(1);
        this->unequipButton->Enable(1);
        break;
      case 1:
        this->allowViewCheck->Show(0);
        this->swapModeCheck->Show(1);
        this->swapButton->Enable(1);
        this->unequipButton->Enable(1);
        break;
      case 2:
      case 3:
        this->allowViewCheck->Show(0);
        this->swapModeCheck->Show(0);
        this->swapButton->Enable(0);
        this->unequipButton->Enable(0);
        break;
      }
    }

    if (this->m_viewOther == 0) {
      switch (this->m_page) {
      case 0:
      case 1:
        this->abilityButton->Enable(1);
        break;
      case 2:
      case 3:
        this->abilityButton->Enable(0);
        break;
      }
    }

    this->SendMsg(this, UM_SET_DISABLE, 575, 0, 0, 0);
    this->Invalidate();
    return 0;
  case UM_SETINFO:
    this->m_placement = reinterpret_cast<NormalItemSlotWindow*>(a);
    if (!reinterpret_cast<EquipWndInfo*>(this->m_placement)->IsValid()) {
      reinterpret_cast<EquipWndInfo*>(this->m_placement)->Reset();
    }
    if (this->m_placement->height != this->m_placement->show && g_session.m_swapEquipmentMode == 0) {
      this->SendMsg(nullptr, UM_COMMAND, ID_MINI, 0, 0, 0);
    } else {
      const int height = this->geometry.height + 20;
      this->m_placement->show = height;
      this->m_placement->height = height;
    }
    this->Move(this->m_placement->x, this->m_placement->y);
    return 0;
  case UM_REFRESH: {
    if (this->m_page == 2) {
      this->RefreshTitleList();
      this->Invalidate();
      return 0;
    }

    if (this->m_viewOther == 0) {
      UICheckBox* check = this->allowViewCheck;
      check->geometry.state = *reinterpret_cast<const int*>(g_session.equipmentViewState);
      check->Invalidate();
    }

    const int height = this->contentOffset + 167;
    this->m_viewButton->Move(this->geometry.width / 2 - 18, IsEquipViewHidden() * 230 + height - this->contentOffset - 40);
    this->m_otherButton->Move(this->geometry.width / 2 - 14, height - this->contentOffset + (g_session.replayPacketMode > 0 ? -60 : 170));
    this->Invalidate();
    return 0;
  }
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(this->m_viewOther == 1 ? 139 : 10);
      return 0;
    case ID_MINI: {
      const int height = this->geometry.height;
      if (height == 17) {
        this->Resize(this->geometry.width, this->m_show);
      } else {
        this->m_show = height;
        this->Resize(this->geometry.width, 17);
      }
      g_windowMgr.StoreSnapRectangle(this);
      g_windowMgr.Promote(this);
      const int delta = this->geometry.height - height;
      if (delta != 0) {
        g_snapMgr.MoveEdge(this, 0, delta, 3);
      }
      return 0;
    }
    case ID_CHECK:
      if (this->m_viewOther != 0) {
        return 0;
      }
      if (this->m_page == 1) {
        g_modeMgr.gameMode->SendMsg(MM_CONFIG_CALL, b, 0, 0, 0);
        return 0;
      }
      g_modeMgr.gameMode->SendMsg(MM_OPEN_EQUIPEDITEM_CHANGE, b, 0, 0, 0);
      return 0;
    case ID_RADIOBTN:
      for (int index = 0; index < 4; ++index) {
        this->damageSkinButtons[index + 2]->Invalidate();
        if (sender == this->damageSkinButtons[index + 2]) {
          this->damageSkinMode = index;
          this->damageSkinButtons[index + 2]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
        } else {
          this->damageSkinButtons[index + 2]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        }
      }
      return 0;
    case ID_EQUIP_OPTION_OFF: {
      if (g_windowMgr.merchantItemMyShopWnd != nullptr) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PLEASE_CLOSE_STORE)), 255, 0, 0, 0);
        return 0;
      }

      std::string notice;
      const bool enabled = g_session.replayPacketMode > 0;
      if (StateFlag05(g_session.m_jobFlags)) {
        const int job = g_session.GetDisplayJob();
        notice = LookupMessage(job == 4082 || job == 4083 || job == 4110 ? 2096 : 442);
      } else if (StateFlag22(g_session.m_jobFlags)) {
        notice = LookupMessage(MSI_MADOOFF);
      } else if (StateFlag19(g_session.m_jobFlags) || StateFlag23(g_session.m_jobFlags) || StateFlag24(g_session.m_jobFlags) || StateFlag25(g_session.m_jobFlags) ||
                 StateFlag26(g_session.m_jobFlags)) {
        notice = LookupMessage(MSI_DRAGONOFF);
      } else {
        notice.clear();
      }

      if (enabled && notice.size() != 0) {
        if (g_windowMgr.menuWnd != nullptr) {
          g_windowMgr.CloseWindow(WID_MENUWND);
        }
        UIWindow* menu = g_windowMgr.EnsureWindow(WID_MENUWND);
        if (!menu) {
          return 0;
        }
        menu->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>("Cart Off"), 0, 0, 0);
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(notice.c_str()), 0, 0, 0);
        menu->SendMsg(nullptr, UM_SET_CALLER_WID, 10, 0, 0, 0);
        menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
        static_cast<UIMenuWnd*>(menu)->m_menuFlags = 1;
        int x = g_mouse.x + 1;
        int y = g_mouse.y + 1;
        if (x < 1) {
          x = 1;
        }
        int maxX = g_renderer->viewport.width - menu->geometry.width;
        if (x <= maxX) {
          maxX = x;
        }
        if (y < 1) {
          y = 1;
        }
        int maxY = g_renderer->viewport.height - menu->geometry.height;
        if (y <= maxY) {
          maxY = y;
        }
        menu->Move(maxX, maxY);
      } else {
        this->UpdateCharacterView(0);
      }

      const int bottom = this->geometry.height - this->contentOffset;
      const int half = this->geometry.width / 2;
      this->m_viewButton->Move(half - 18, IsEquipViewHidden() * 230 + bottom - 40);
      this->m_otherButton->Move(half - 14, bottom + (g_session.replayPacketMode > 0 ? -60 : 170));
      this->Invalidate();
      return 0;
    }
    case ID_EQUIP_OPTION_OPEN:
      if ((g_replayAnimationScaling == 0 || Replay::Instance().header.options.Get(ReplayRecordOption::Item) != 0) && !g_windowMgr.LookupWindow(WID_MERGEITEMWND) &&
          !g_windowMgr.CloseWindow(WID_MERCHANTITEMWND) && g_session.replayPacketMode > 0) {
        g_windowMgr.EnsureWindow(WID_MERCHANTITEMWND);
      }
      return 0;
    case ID_APPLY: {
      if (this->m_hoverSlot >= 0) {
        g_session.m_damageSkinIndex = this->m_hoverSlot;
      }
      g_session.m_damageSkinMode = this->damageSkinMode;
      const char* resourceName = nullptr;
      const char* damageSpriteName = nullptr;
      const char* damageSkinAuxiliaryName = nullptr;
      g_session.m_hotkeyScript->Call("GetDamageResourceName", "d>sss", this->m_hoverSlot, &resourceName, &damageSpriteName, &damageSkinAuxiliaryName);
      g_session.m_damageResourceName = resourceName;
      g_session.damageSpriteName = damageSpriteName;
      g_session.damageSkinAuxiliaryName = damageSkinAuxiliaryName;
      int millionUnit = 0;
      g_session.m_hotkeyScript->Call("GetDamageMillionUnit", "d>d", this->m_hoverSlot, &millionUnit);
      g_session.m_damageMillionUnit = millionUnit;
      return 0;
    }
    case 480: {
      ChangeTitlePacket packet;
      packet.packetType = ID_CZ_REQ_CHANGE_TITLE;
      packet.titleId = b;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
      return 0;
    }
    case 508: {
      const int height = this->contentOffset + 167;
      this->Resize(this->geometry.width, height + 158);
      if (!this->swapWindow) {
        UISwapEquipmentWnd* created = new (std::nothrow) UISwapEquipmentWnd();
        this->swapWindow = created;
        created->SetSize(280, 179);
        if (this->m_page == 0) {
          static_cast<UISwapEquipmentWnd*>(this->swapWindow)->equipmentPage = 1;
        } else if (this->m_page == 1) {
          static_cast<UISwapEquipmentWnd*>(this->swapWindow)->equipmentPage = 2;
        }
        this->AddChild(this->swapWindow);
      }
      UIWindow* swap = this->swapWindow;
      if (swap->geometry.state == 0) {
        swap->Move(0, 167);
        this->swapWindow->geometry.state = 1;
        if (sender == this->swapButton) {
          g_session.m_swapEquipmentMode = 1;
        }
        return 0;
      }
      swap->Move(-1000, -1000);
      this->swapWindow->geometry.state = 0;
      this->Resize(this->geometry.width, height);
      return 0;
    }
    case 575: {
      TakeoffEquipAllPacket packet;
      if (this->m_page == 1) {
        packet.location = 0x3F7C00;
      } else if (this->m_page == 0) {
        packet.location = 0x83FF;
      } else {
        return 0;
      }
      packet.packetType = ID_CZ_REQ_TAKEOFF_EQUIP_ALL_0bf5;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
      this->unequipButton->SetDisabled(1);
      this->unequipButton->Invalidate();
      return 0;
    }
    case 582:
      if (!g_windowMgr.CloseWindow(WID_EQUIPMENTPROPERTIESWND)) {
        g_windowMgr.EnsureWindow(WID_EQUIPMENTPROPERTIESWND);
      }
      return 0;
    }
    return 0;
  case UM_SET_DISABLE:
    if (a == 575) {
      if (this->m_page == 1) {
        UIBitmapButton* button = this->unequipButton;
        button->SetDisabled(g_session.CountDistinctCostumeItems() == 0);
        button->Invalidate();
        return 0;
      }
      if (this->m_page == 0) {
        UIBitmapButton* button = this->unequipButton;
        button->SetDisabled(g_session.CountDistinctEquippedItems() == 0);
        button->Invalidate();
        return 0;
      }
    }
    return 0;
  case UM_MENU_SELECTED:
    this->UpdateCharacterView(a);
    return 0;
  case UM_SCROLLHORZ: {
    const int previous = this->m_firstSlot;
    if (a != 0) {
      this->m_firstSlot = previous + a;
    }
    const int last = g_session.damageSkinCount - 4;
    if (this->m_firstSlot < 0) {
      this->m_firstSlot = 0;
    }
    if (this->m_firstSlot > last) {
      this->m_firstSlot = last;
    }
    static_cast<UIScrollBar*>(this->m_scrollBar)->SetPosition(this->m_firstSlot);
    this->m_scrollBar->Invalidate();
    this->Invalidate();
    return this->m_firstSlot - previous;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008DEC50
// FUNCTION: fn_12171
void UIEquipWnd::OnCreate(int width, int height) {
  const int top = this->m_viewOther != 0 ? 0 : 20;
  this->contentOffset = top;
  if (height != top + this->geometry.height) {
    this->Resize(this->geometry.width, top + this->geometry.height);
  }

  if (this->m_viewOther == 0) {
    this->allowViewCheck = new UICheckBox();
    this->allowViewCheck->SetSize(80, 12);
    this->allowViewCheck->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_OPEN_EQUIPED_ITEM));
    this->allowViewCheck->Move(10, this->geometry.height - 15);
    this->allowViewCheck->SetChecked(*reinterpret_cast<const int*>(g_session.equipmentViewState));
    this->AddChild(this->allowViewCheck);

    this->swapModeCheck = new UICheckBox();
    this->swapModeCheck->SetSize(80, 12);
    this->swapModeCheck->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_SHORTCUT_SKILL_TOOLTIP_SP_PERCENT_AP_VALUE));
    this->swapModeCheck->SetChecked(*reinterpret_cast<const int*>(g_session.feedOptions) == 0);
    this->swapModeCheck->Show(0);
    this->swapModeCheck->Move(10, this->geometry.height - 15);
    this->AddChild(this->swapModeCheck);
  }

  const std::string closeTooltip("Alt+Q");
  const std::string minimizeTooltip("Ctrl+Q");
  this->AddBaseButton();
  this->AddCloseButton(closeTooltip.c_str(), 0);
  this->AddMinimizeButton(minimizeTooltip.c_str(), 0);

  if (this->m_viewOther == 0) {
    {
      std::string prefix("유저인터페이스\\basic_interface\\");
      const int x = this->geometry.width / 2 - 18;
      const int bottom = this->geometry.height - this->contentOffset;
      this->m_viewButton = new UIBitmapButton();
      this->m_viewButton->SetBitmap((prefix + "btn_off" + ".bmp").c_str(), 0, 0);
      this->m_viewButton->SetBitmap((prefix + "btn_off" + ".bmp").c_str(), 1, 0);
      this->m_viewButton->SetBitmap((prefix + "btn_off" + ".bmp").c_str(), 2, 0);
      this->m_viewButton->SetSize(this->m_viewButton->BitmapWidth(), this->m_viewButton->BitmapHeight());
      this->m_viewButton->Move(x, IsEquipViewHidden() * 230 + bottom - 40);
      this->m_viewButton->SetId(248);

      const char* tooltip = nullptr;
      const std::uint32_t flags = g_session.m_jobFlags;
      if (StateFlag04(flags)) {
        tooltip = LookupMessage(MSI_BIRDOFF);
      } else if (StateFlag05(flags)) {
        switch (g_session.GetDisplayJob()) {
        case JT_ROYAL_CHICKEN:
        case JT_ROYAL_CHICKEN_H:
        case JT_ROYAL_CHICKEN_B:
        case JT_IMPERIAL_GUARD_CHICKEN:
          tooltip = LookupMessage(MSI_BATTLEFIELD_MSG_REQUEST_JOINWAIT);
          break;
        default:
          tooltip = LookupMessage(MSI_CHIKENOFF);
          break;
        }
      } else if (StateFlag22(flags)) {
        tooltip = LookupMessage(MSI_MADOOFF);
      } else if (StateFlag19(flags) || StateFlag23(flags) || StateFlag24(flags) || StateFlag25(flags) || StateFlag26(flags)) {
        tooltip = LookupMessage(MSI_DRAGONOFF);
      }
      if (g_session.replayPacketMode > 0) {
        tooltip = LookupMessage(tooltip ? 0xE34 : 0xDF);
      }
      this->m_viewButton->SetTooltip(tooltip);
      this->AddChild(this->m_viewButton);
    }

    if (this->m_viewOther == 0) {
      std::string prefix("유저인터페이스\\basic_interface\\");
      const int x = this->geometry.width / 2 - 14;
      const int bottom = this->geometry.height - this->contentOffset;
      this->m_otherButton = new UIBitmapButton();
      this->m_otherButton->SetBitmap((prefix + "btn_items_off" + ".bmp").c_str(), 0, 0);
      this->m_otherButton->SetBitmap((prefix + "btn_items_on" + ".bmp").c_str(), 1, 0);
      this->m_otherButton->SetBitmap((prefix + "btn_items_on" + ".bmp").c_str(), 2, 0);
      this->m_otherButton->SetSize(this->m_otherButton->BitmapWidth(), this->m_otherButton->BitmapHeight());
      this->m_otherButton->Move(x, bottom + (g_session.replayPacketMode > 0 ? -60 : 170));
      this->m_otherButton->SetId(249);
      this->m_otherButton->SetTooltip(g_session.replayPacketMode > 0 ? LookupMessage(MSI_OPENCARTWINDOW) : "");
      this->AddChild(this->m_otherButton);
    }
  }

  this->m_show = this->geometry.height;
  this->m_highlight[0] = -1;
  this->m_highlight[1] = -1;
  this->m_highlight[2] = -1;

  this->m_tabs = new UITabControl();
  this->m_tabs->Configure(1, 17, 15, 1, 0, 0, 0, 0, 0, 0, 0, 0);
  this->m_tabs->Move(0, 17);
  this->AddChild(this->m_tabs);
  this->m_tabs->AddTab(LookupMessage(MSI_GENERAL_EQUIPMENT), LookupMessage(MSI_HELP_QUAKE_ONOFF));
  this->m_tabs->AddTab(LookupMessage(MSI_NAVIGATION_ALL_SD), LookupMessage(MSI_HELP_AURA_EFFECT_OFF));
  if (this->m_viewOther == 0) {
    this->m_tabs->AddTab(LookupMessage(MSI_GUILD_DISORGANIZE_GUILD_NOT_PERMITTED_MAP), LookupMessage(MSI_GRAPHIC_SETTING_TAB_BASIC));
  }

  this->m_titleList = new UIListBoxTitle();
  this->m_titleList->Configure(3, this->m_contentTop + 39, this->geometry.width - 6, 105, 0);
  reinterpret_cast<int*>(this->m_titleList->backgroundChannels)[0] = 225;
  reinterpret_cast<int*>(this->m_titleList->backgroundChannels)[1] = 225;
  reinterpret_cast<int*>(this->m_titleList->backgroundChannels)[2] = 225;
  this->m_titleList->SetId(480);
  this->m_titleList->Show(0);
  this->AddChild(this->m_titleList);

  if (this->m_viewOther == 0 && this->swapButton == nullptr) {
    this->swapButton = new UIBitmapButton();
    this->swapButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_change_a.bmp", 0, 0);
    this->swapButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_change_b.bmp", 1, 0);
    this->swapButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_change_b.bmp", 2, 0);
    this->swapButton->SetSize(this->swapButton->BitmapWidth(), this->swapButton->BitmapHeight());
    this->swapButton->Move(193, 167);
    this->swapButton->SetId(508);
    this->swapButton->SetTooltip(LookupMessage(MSI_REFINING_FAIL_ITEM_DOWMGRADE));
    this->AddChild(this->swapButton);
  }

  this->unequipButton = new UIBitmapButton();
  this->unequipButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_off_a.bmp", 0, 0);
  this->unequipButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_off_b.bmp", 1, 0);
  this->unequipButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_off_b.bmp", 2, 0);
  this->unequipButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_off_c.bmp", 3, 0);
  this->unequipButton->SetSize(this->unequipButton->BitmapWidth(), this->unequipButton->BitmapHeight());
  this->unequipButton->Move(235, 167);
  this->unequipButton->SetId(575);
  this->unequipButton->SetTooltip(LookupMessage(MSI_USESKILL_FAIL_NEED_SOULCOLLECT));
  this->AddChild(this->unequipButton);
  this->SendMsg(this, UM_SET_DISABLE, 575, 0, 0, 0);

  if (this->m_viewOther == 0) {
    this->m_tabs->AddTab(LookupMessage(MSI_DAMAGE_DIR_CENTER), LookupMessage(MSI_DAMAGE_DIR_RIGHT));

    UIBitmapButton* apply = new UIBitmapButton();
    this->damageSkinButtons[0] = apply;
    apply->SetBitmap("유저인터페이스\\btn_apply.bmp", 0, 0);
    apply->SetBitmap("유저인터페이스\\btn_apply_a.bmp", 1, 0);
    apply->SetBitmap("유저인터페이스\\btn_apply_b.bmp", 2, 0);
    apply->SetSize(apply->BitmapWidth(), apply->BitmapHeight());
    apply->Move(160, 167);
    apply->SetId(271);
    apply->Enable(0);
    this->AddChild(apply);

    UIBitmapButton* close = new UIBitmapButton();
    this->damageSkinButtons[1] = close;
    close->SetBitmap("유저인터페이스\\btn_close.bmp", 0, 0);
    close->SetBitmap("유저인터페이스\\btn_close_a.bmp", 1, 0);
    close->SetBitmap("유저인터페이스\\btn_close_b.bmp", 2, 0);
    close->SetSize(close->BitmapWidth(), close->BitmapHeight());
    close->Move(220, 167);
    close->SetId(201);
    close->Enable(0);
    this->AddChild(close);

    const int count = g_session.damageSkinCount;
    for (int index = 0; index < count; ++index) {
      const char* name = nullptr;
      const char* icon = nullptr;
      g_session.m_hotkeyScript->Call("GetDamageSkinName", "d>ss", index, &name, &icon);
      if (name) {
        this->m_damageSkinNames.insert(this->m_damageSkinNames.end(), std::string(name));
      }
      if (icon) {
        this->m_damageSkinIcons.insert(this->m_damageSkinIcons.end(), std::string(icon));
      }
    }

    UIScrollBar* scrollBar = new UIScrollBar();
    this->m_scrollBar = scrollBar;
    scrollBar->Configure(1, 153, 0, width);
    scrollBar->Show(0);
    scrollBar->SetRange(count - 4 > 0 ? count - 4 : 0, 4, count);
    this->AddChild(this->m_scrollBar);

    const char* labels[4] = {LookupMessage(MSI_MACRO_CHECK_REMOVE_NOT_EXISTS), LookupMessage(MSI_MACRO_CHECK_REMOVE_SUCCESS), LookupMessage(MSI_MACRO_CHECK_REMOVE_RUNNING), LookupMessage(MSI_DAMAGE_DIR_DEFAULT)};
    for (int index = 0; index < 4; ++index) {
      UIRadioBtn* radio = new UIRadioBtn();
      this->damageSkinButtons[index + 2] = radio;
      radio->SetSize(60, 12);
      if (index == 0) {
        radio->Move(8, this->m_contentTop + 20);
      } else {
        radio->Move(index * 60 + 28, this->m_contentTop + 20);
      }
      radio->SendMsg(this, UM_SETSTATE, index == this->damageSkinMode, 0, 0, 0);
      radio->SetText(labels[index]);
      const std::string unchecked("유저인터페이스\\tipbox\\checkBox.bmp");
      const std::string checked("유저인터페이스\\tipbox\\checkBox_check.bmp");
      radio->checkedBitmap = checked;
      radio->uncheckedBitmap = unchecked;
      radio->Invalidate();
      radio->Show(0);
      this->AddChild(radio);
    }

    if (this->m_viewOther == 0) {
      this->abilityButton = new UIBitmapButton();
      this->abilityButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_ability_a.bmp", 0, 0);
      this->abilityButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_ability_b.bmp", 1, 0);
      this->abilityButton->SetBitmap("유저인터페이스\\basic_interface\\btn_e_ability_b.bmp", 2, 0);
      this->abilityButton->SetSize(this->abilityButton->BitmapWidth(), this->abilityButton->BitmapHeight());
      this->abilityButton->Move(g_equipAbilityButtonPosition[0], g_equipAbilityButtonPosition[1]);
      this->abilityButton->SetId(582);
      this->abilityButton->SetTooltip(LookupMessage(MSI_SEND_TO_ALLY));
      this->AddChild(this->abilityButton);
    }
  }

  if (g_session.m_swapEquipmentMode == 1) {
    this->SendMsg(nullptr, UM_COMMAND, 508, 0, 0, 0);
  }
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
    for (auto it = this->m_buttons.begin(); it != this->m_buttons.end(); ++it) {
      UIWindow* other = *it;
      if (button != other) {
        x += -4 - static_cast<UIBitmapButton*>(other)->BitmapWidth();
      }
    }
    button->Move(x - static_cast<UIBitmapButton*>(button)->BitmapWidth(), y);
  }
}

// OFFSET: 0x008F6AC0
// FUNCTION: fn_12325
int UIBabyAcceptWnd2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_BABYACCEPTWND2);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_BABY, this->identity, 1, 0, 0);
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_BABYACCEPTWND2);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_BABY, this->identity, 0, 0, 0);
      break;
    }
    return 0;
  }
  case UM_SET_BABY_MSG: {
    this->identity = static_cast<std::uint32_t>(a);
    const std::string text = LookupMessage(MSI_NOTENOUGH_ZENY);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* packet = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, packet, sizeof(header));
    unsigned offset = 6;
    if (header.type == 0x542E) {
      bool scanning = true;
      while (scanning && offset < static_cast<unsigned>(b)) {
        std::memcpy(&header, packet + offset, sizeof(header));
        offset += 6;
        if (header.type != 0x542F) {
          offset += header.length;
        }
        if (header.type == 0x542F) {
          scanning = false;
        }
      }
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008F6D00
// FUNCTION: fn_12326
int UIBabyAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_BABYACCEPTWND);
      g_windowMgr.EnsureWindow(WID_BABYACCEPTWND2)->SendMsg(nullptr, UM_SET_BABY_MSG, 184, b, 0, 0);
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_BABYACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_BABY, this->identity, 0, 0, 0);
      break;
    }
    return 0;
  }
  case UM_SET_BABY_MSG: {
    this->identity = static_cast<std::uint32_t>(a);
    std::string text = "(";
    text += reinterpret_cast<const char*>(b);
    text += ")";
    text += LookupMessage(MSI_SUGGEST_BABY);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* packet = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, packet, sizeof(header));
    unsigned offset = 6;
    if (header.type == 0x53FC) {
      while (offset < static_cast<unsigned>(b)) {
        std::memcpy(&header, packet + offset, sizeof(header));
        offset += 6;
        switch (header.type) {
        case 0x53FD:
          return 0;
        case 0x53FE: {
          char name[260];
          std::memset(name, 0, sizeof(name));
          std::memcpy(name, packet + offset, header.length);
          offset += header.length;
          this->SendMsg(nullptr, UM_SET_BABY_MSG, 0, reinterpret_cast<std::intptr_t>(name), 0, 0);
          break;
        }
        default:
          offset += header.length;
          break;
        }
      }
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008F7050
// FUNCTION: fn_12327
int UIBookWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_LEFTPUSH:
    this->SendMsg(this, UM_COMMAND, ID_PREVIEW, 0, 0, 0);
    break;

  case UM_RIGHTPUSH:
    this->SendMsg(this, UM_COMMAND, ID_NEXTVIEW, 0, 0, 0);
    break;

  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      this->SendMsg(this, UM_COMMAND, ID_BOOKMARK, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_BOOKWND);
      break;
    case ID_PREVIEW:
      if (this->m_autoRead == 0) {
        this->SendMsg(this, UM_READBOOK, this->m_page == 1 ? 1 : this->m_page - 1, 0, 0, 0);
      }
      break;
    case ID_NEXTVIEW:
      if (this->m_autoRead == 0) {
        this->SendMsg(this, UM_READBOOK, this->m_page == this->m_pageCount ? this->m_page : this->m_page + 1, 0, 0, 0);
      }
      break;
    case ID_BOOKMARK: {
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

  case UM_GETBOOKCONTENTS: {
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
      this->WrapPixelText(text, this->m_lines, this->m_viewer->geometry.width - 10);
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
  case UM_READBOOK: {
    this->m_autoRead = 0;
    this->Show(1);
    if (this->m_balloon) {
      g_windowMgr.Schedule(this->m_balloon);
      this->m_balloon = nullptr;
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
        this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text), 100, 0, 0);
      } else {
        this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->m_lines[line].c_str()), 100, 0, 0);
      }
    }
    this->Invalidate();
    g_windowMgr.Promote(this);
    break;
  }

  case UM_AUTOREAD:
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

  case UM_ENDPUSH:
    if (this->m_balloon) {
      g_windowMgr.Schedule(this->m_balloon);
      this->m_balloon = nullptr;
    }
    g_windowMgr.CloseWindow(WID_BOOKWND);
    break;

  }
  return 0;
}


// OFFSET: 0x008F8860
// FUNCTION: fn_12332
int UIChooseSellBuyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND: {
    const int response = static_cast<int>(a);
    this->RecordResponse(response, b, c, d);

    if (this->geometry.visible != 0) {
      switch (response) {
      case ID_CANCEL:
        g_modeMgr.gameMode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_CHOOSESELLBUYWND);
        break;
      case ID_SELL:
        g_modeMgr.gameMode->SendMsg(MM_ACK_SELL, this->identity, 0, 0, 0);
        g_windowMgr.LookupWindow(WID_CHOOSESELLBUYWND)->Show(0);
        break;
      case ID_BUY:
        g_modeMgr.gameMode->SendMsg(MM_ACK_BUY, this->identity, 0, 0, 0);
        g_windowMgr.LookupWindow(WID_CHOOSESELLBUYWND)->Show(0);
        break;
      default:
        return 0;
      }

      g_session.ClearTradeItems();
    }

    return 0;
  }
  case UM_SETNAID: {
    this->identity = static_cast<std::uint32_t>(a);
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, bytes, sizeof(header));
    unsigned offset = 6;

    if (header.type == 20850) {
      while (static_cast<unsigned>(b) > offset) {
        std::memcpy(&header, bytes + offset, sizeof(header));
        offset += 6;

        if (header.type == 20851) {
          static_cast<CGameMode*>(g_modeMgr.GetGameMode())->isOnQuest = 1;
          this->SendMsg(nullptr, UM_SETNAID, 0, 0, 0, 0);
          g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
          g_windowMgr.CloseWindow(WID_ITEMPURCHASEWND);
          g_windowMgr.CloseWindow(WID_ITEMSELLWND);
          break;
        }

        offset += header.length;
      }
    }

    return 0;
  }
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
bool UInCashWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x008DB750
// FUNCTION: fn_12159
void UIBookWnd::OnCreate(int width, int height) {
  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(30, 45, width - 60, height - 60, 0);
  this->AddChild(this->m_viewer);

  const std::string prefix = "book\\";
  const char* names[4] = {"책갈피", "책왼쪽", "책오른쪽", "책닫기"};
  const int ids[4] = {356, 354, 355, 201};
  const struct {
    int x;
    int y;
  } positions[4] = {{10, 25}, {width - 57, 25}, {width - 35, 25}, {width - 25, height - 25}};
  const char* tooltips[4] = {LookupMessage(MSI_BOOK_KEEP), LookupMessage(MSI_BOOK_PREV), LookupMessage(MSI_BOOK_NEXT), LookupMessage(MSI_BOOK_CLOSE)};

  for (int index = 0; index < 4; ++index) {
    this->m_buttons[index] = new UISpriteButton();
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
void UIItemCollectionWnd::OnCreate(int width, int height) {
  this->m_kind = 12;
  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(91, 31, width - 98, height - 40, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  this->m_closeButton = this->AddCloseButton("ESC", 0);

  std::string path = "유저인터페이스\\";
  this->m_viewButton = new UIBitmapButton();
  this->m_viewButton->SetBitmap((path + "btn_view" + ".bmp").c_str(), 0, 0);
  this->m_viewButton->SetBitmap((path + "btn_view_a" + ".bmp").c_str(), 1, 0);
  this->m_viewButton->SetBitmap((path + "btn_view_b" + ".bmp").c_str(), 2, 0);
  this->m_viewButton->SetSize(this->m_viewButton->BitmapWidth(), this->m_viewButton->BitmapHeight());
  this->m_viewButton->Move(6, 6);
  this->m_viewButton->SetId(250);
  this->AddChild(this->m_viewButton);

  const std::string prefix = "book\\";
  const char* tooltips[2] = {LookupMessage(MSI_BOOK_READ), LookupMessage(MSI_BOOK_AUTOREAD)};
  const int ids[2] = {287, 288};
  const char* names[2] = {"책읽기", "책읽기"};
  for (int index = 0; index < 2; ++index) {
    this->m_readButtons[index] = new UISpriteButton();
    this->m_readButtons[index]->SetSprite((prefix + names[index] + ".spr").c_str());
    this->m_readButtons[index]->SetActName((prefix + names[index] + ".act").c_str());
    this->m_readButtons[index]->SetSize(this->m_readButtons[index]->FrameWidth(0, 0), this->m_readButtons[index]->FrameHeight(0, 0));
    this->m_readButtons[index]->Move(-100, -100);
    this->m_readButtons[index]->SetId(ids[index]);
    this->m_readButtons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->m_readButtons[index]);
  }

  this->m_resizer = new UIResizer();
  this->m_resizer->SetSize(16, 16);
  this->m_resizer->SetDirection(7);
  this->m_resizer->SetBitmapName(this->m_kind == 234 ? "유저인터페이스\\basic_interface\\btn_comparison_resize.bmp" : "유저인터페이스\\btn_resize.bmp");
  this->m_resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->m_resizer->Move(width - 17, height - 17);
  this->AddChild(this->m_resizer);

  this->m_previewTick = 0;
  if (!this->m_moveInfo) {
    this->m_moveInfo = new UIItemMoveInfo();
    this->m_moveInfo->Configure(LookupMessage(MSI_REFUSE_MDENGEON_IS_FULL), 0, 0, 12, 0, 1);
    this->m_moveInfo->SetId(487);
    UIItemMoveInfo* moveInfo = this->m_moveInfo;
    int textHeight = this->TextHeight(LookupMessage(MSI_REFUSE_MDENGEON_IS_FULL), 0, 0, 12, 0, 0);
    moveInfo->ResizeToTextClipped(this->TextWidth(LookupMessage(MSI_REFUSE_MDENGEON_IS_FULL), 0, 0, 12, 0, 0) + 1, textHeight);
    this->AddChild(this->m_moveInfo);
  }

  this->m_helpButton = new UIBitmapButton();
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_normal.bmp", 0, 0);
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_over.bmp", 1, 0);
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_pressed.bmp", 2, 0);
  this->m_helpButton->SetBitmap("유저인터페이스\\basic_interface\\btn_help_disabled.bmp", 3, 0);
  this->m_helpButton->SetSize(this->m_helpButton->BitmapWidth(), this->m_helpButton->BitmapHeight());
  this->m_helpButton->Move(10, 10);
  this->m_helpButton->SetId(343);
  this->m_helpButton->SetTooltip(LookupMessage(MSI_PROBABILITY_TITLE));
  this->AddChild(this->m_helpButton);
}

// OFFSET: 0x008DE360
// FUNCTION: fn_12165
void UICombinedCardItemCollectionWnd::OnCreate(int width, int height) {
  this->UIItemCollectionWnd::OnCreate(width, height);
  this->m_kind = 82;
}

// OFFSET: 0x008DE380
// FUNCTION: fn_12166
void UICombinedCardItemCollection_ComparisonWnd::OnCreate(int width, int height) {
  this->UIItemCollectionWnd::OnCreate(width, height);
  this->m_closeButton->Enable(0);
  this->m_kind = 250;
  const SkinPaletteCell cell = {7, 1};
  this->m_viewer->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), cell, g_skinInfo.Selected()) | 0xFF000000u;
  this->m_viewer->Invalidate();
}

// OFFSET: 0x008E24C0
// FUNCTION: fn_12176
void UIItemCollection_ComparisonWnd::OnCreate(int width, int height) {
  this->UIItemCollectionWnd::OnCreate(width, height);
  this->m_closeButton->Enable(0);
  this->m_kind = 234;
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 30, 6, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
}

// OFFSET: 0x008DBD00
// FUNCTION: fn_12160
void UICardItemIllustWnd::OnCreate(int, int) {
  this->identified = 0;
  this->m_name = LookupMessage(MSI_ITEM_NAME);
  this->AddCloseButton("", 0);
}

// OFFSET: 0x008E2590
// FUNCTION: fn_12178
void UIItemIdentifyWnd::OnCreate(int, int) {
  this->m_title.assign(LookupMessage(MSI_ITEM_IDENTIFY));
  this->m_selected = -1;
  this->m_scroll = 0;
  this->m_columns = 1;
  this->m_rows = 4;
  this->m_scrollable = 0;
  auto* scrollBar = new UIScrollBar();
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
void UIItemCompositionWnd::OnCreate(int width, int height) {
  this->UIItemIdentifyWnd::OnCreate(width, height);
  this->m_title = LookupMessage(MSI_ITEM_COMPOUNDING);
  this->m_compositionState = 0;
}

// OFFSET: 0x008DE490
// FUNCTION: fn_12168
void UIComboBoxWnd::OnCreate(int width, int height) {
  this->m_list = new UIComboBox();
  this->m_list->Configure(0, 0, width, height, 1);
  this->AddChild(this->m_list);
}

// OFFSET: 0x008E85C0
// FUNCTION: fn_12199
void UISkillDescribeWnd::OnCreate(int width, int height) {
  this->skillName = LookupMessage(MSI_SKILL_NAME);
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
void UIStatusWnd::OnCreate(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("Alt+A", 0);
  this->AddMinimizeButton("Ctrl+A", 0);

  {
    const int ids[6] = {205, 206, 207, 208, 209, 210};
    const std::string arrow = "유저인터페이스\\basic_interface\\arw_right.bmp";
    for (int i = 0; i < 6; ++i) {
      this->m_statButtons[i] = new UIBitmapButton();
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
      this->m_extendedButtons[i] = new UIBitmapButton();
      this->m_extendedButtons[i]->SetBitmap(arrow.c_str(), 0, 0);
      this->m_extendedButtons[i]->SetBitmap(arrow.c_str(), 1, 0);
      this->m_extendedButtons[i]->SetBitmap(arrow.c_str(), 2, 0);
      this->m_extendedButtons[i]->SetSize(this->m_extendedButtons[0]->BitmapWidth(), this->m_extendedButtons[0]->BitmapHeight());
      this->m_extendedButtons[i]->Move(78, 145 + i * 16);
      this->m_extendedButtons[i]->SetId(ids[i]);
      this->AddChild(this->m_extendedButtons[i]);
    }
  }

  this->m_expandButton = new UIBmpTextButton();
  this->m_expandButton->SetBitmap("유저인터페이스\\statuswnd\\expand_on_normal.bmp", 0, 0);
  this->m_expandButton->SetBitmap("유저인터페이스\\statuswnd\\expand_on_normal.bmp", 1, 0);
  this->m_expandButton->SetBitmap("유저인터페이스\\statuswnd\\expand_on_press.bmp", 2, 0);
  this->m_expandButton->AddTextStyle(0, 12, 0, 0, 1, 16, 3);
  this->m_expandButton->AddTextStyle(1, 12, 0, 0, 1, 16, 3);
  this->m_expandButton->AddTextStyle(2, 12, 0, 0, 1, 16, 3);
  this->m_expandButton->SetSize(this->m_expandButton->BitmapWidth(), this->m_expandButton->BitmapHeight());
  {
    const std::string label = LookupMessage(MSI_DESC_WIS);
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
void UISkillNameChangeWnd::OnCreate(int, int) {
  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(this->geometry.width - 77, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 29;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->SetText("");
  this->AddChild(this->m_edit);
  g_windowMgr.SetFocus(this->m_edit);
  this->m_edit->SelectAll();

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton();
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
void UIToolTipWnd::OnCreate(int width, int height) {
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
void UIProhibitListWnd::OnCreate(int, int) {
  this->AddCloseButton("", 0);
  char tooltip[] = "Command";

  auto* area = new UINoneVisibleButton();
  area->SetSize(120, 18);
  area->Move(350, 59);
  area->SetId(268);
  this->AddChild(area);

  const std::string prefix("유저인터페이스\\basic_interface\\");
  auto* button = new UIBitmapButton();
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
  const int titleHeight = this->DrawTitleBar(false, this->m_title.c_str(), 0);
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

    DrawItemIcon(*this, ResourceMgr::Instance(), 8, y, *it, 0);
    this->DrawFittedItemName(40, y, &*it, 0, 0, 12, 0);
    if (it->grade > 0) {
      this->DrawBitmap(
        8, y + 12,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
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
  this->m_title = LookupMessage(MSI_PET_EGG_LIST);
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

    const int x = row >= 4 ? 257 : 71;
    const int y = (row >= 4 ? row - 4 : row) * this->m_rowHeight + this->m_listTop;
    const std::string path = "유저인터페이스\\collection\\" + std::string(ItemResourcePath(*it)) + ".bmp";
    if (!path.empty()) {
      this->DrawBitmap(x, y, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str()))), true);
    }

    char text[64];
    char number[64];
    const int width = this->DrawFittedItemName(x, -30, &*it, 0, 0, 12, 0);
    this->DrawWrappedItemName(x - width / 2 + 90, y + 4, width, &*it, 0, 0, 12);
    int length = std::sprintf(text, LookupMessage(MSI_NC_COST));
    this->DrawTextAt(x + 112, y + 64, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, " %s C", FormatGroupedInteger(static_cast<int>(it->prices[0]), number, 64));
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
      this->DrawBitmap(453, y, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str()))), true);
    }

    char text[64];
    char number[64];
    std::memset(text, 0, sizeof(text));
    std::memset(number, 0, sizeof(number));
    const int width = this->DrawFittedItemName(453, -30, &*it, 0, 0, 12, 0);
    this->DrawWrappedItemName(535 - width / 2, y + 4, width, &*it, 0, 0, 12);
    int length = std::sprintf(text, LookupMessage(MSI_NC_COST));
    this->DrawTextAt(565, y + 24, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, " %s C", FormatGroupedInteger(static_cast<int>(it->prices[0]), number, 64));
    this->DrawTextAt(550, y + 42, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, LookupMessage(MSI_NC_NUM));
    this->DrawTextAt(565, y + 66, text, length, 0, 12, 0, 0, 0);
    length = std::sprintf(text, " %d %s", it->quantity, LookupMessage(MSI_NC_COUNT));
    this->DrawRightAlignedText(590, y + 84, text, length, 0, 12, 0, 0);

    ++row;
    if (row > 3) {
      break;
    }
  }
}

// OFFSET: 0x008E3C80
// FUNCTION: fn_12183
void UIMonsterDisplayWnd::OnCreate(int, int) {
  this->padding_00E4 = 0;
  this->titleText = LookupMessage(MSI_ITEM_NAME);
  this->AddCloseButton("", 0);
}

// OFFSET: 0x008E3090
// FUNCTION: fn_12179
void UIItemParamChangeDisplayWnd::OnCreate(int, int) {
  this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
}

// OFFSET: 0x008E3330
// FUNCTION: fn_12181
void UIMenuWnd::OnCreate(int, int) {
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
    int segment = this->TextWidth(it->second.c_str(), it->second.size(), 0, 12, 0, 0);
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
  for (const auto& line : lines) {
    int left = x;
    for (const auto& segment : line) {
      this->DrawShadowedText(left, y, segment.second.c_str(), segment.second.size(), segment.first, 0xE0E0E0, family, 12, 0);
      left += this->TextWidth(segment.second.c_str(), segment.second.size(), 0, 12, 0, 0);
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
  if (this->TextWidth(text, 0, 0, 12, 0, 0) < width) {
    for (; it != breaks.end(); ++it) {
      const char* end = *it;
      int bytes = static_cast<int>(end - text);
      if (bytes) {
        this->DrawShadowedText(offset + x, y, text, bytes, text != highlight ? color : highlightColor, 0xE0E0E0, 0, 12, 0);
        offset += this->TextWidth(text, bytes, 0, 12, 0, 0);
        text = end;
      }
    }
    return x;
  }

  int line = 1;
  y -= lineHeight / 2;
  for (; it != breaks.end(); ++it) {
    const char* segment = text;
    const char* start = text;
    const char* end = *it;
    while (text < end) {
      const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), text, 0);
      if (g_inlineCharset && !TextContext::Marker(text, static_cast<unsigned>(std::strlen(text)))) {
        text += 3;
        continue;
      }

      int bytes = static_cast<int>(next - text);
      int nextWidth = this->TextWidth(next, static_cast<unsigned>(std::strlen(next)), 0, 12, 0, 0);
      int reach = this->TextWidth(text, bytes, 0, 12, 0, 0) + offset;
      if (line == 2 && reach >= width - 15 && reach + nextWidth > width) {
        if (start == highlight) {
          this->DrawShadowedText(bytes + offset + x, y, "...", 3, highlightColor, 0xE0E0E0, 0, 12, 0);
        } else {
          this->DrawShadowedText(bytes + offset + x, y - 1, "...", 3, color, 0xE0E0E0, 0, 12, 0);
        }
        break;
      }

      this->DrawShadowedText(x + offset, y, text, bytes, segment != highlight ? color : highlightColor, 0xE0E0E0, 0, 12, 0);
      offset += this->TextWidth(text, bytes, 0, 12, 0, 0);
      if (offset > width) {
        offset = 0;
        y += lineHeight + 1;
        ++line;
      }
      text = next;
    }
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
  case UM_RESOLVE_PLAYER_TAGS:
    this->m_chatHistory->ResolvePlayerTags(reinterpret_cast<const char*>(a));
    return 0;
  case UM_SETINFO: {
    WhisperWndInfo* info = reinterpret_cast<WhisperWndInfo*>(a);
    this->placement = info;
    if (info->IsValid() == 0) {
      static_cast<WhisperWndInfo*>(this->placement)->Reset();
    }
    this->SendMsg(nullptr, UM_RESIZE, 7, this->placement->width, this->placement->height, 0);
    this->Move(this->placement->x, this->placement->y);
    return 0;
  }
  case UM_COMMAND: {
    if (a != ID_OK) {
      if (a == ID_CLOSE) {
        g_windowMgr.Schedule(this);
      }
      return 0;
    }

    if (HasItemLinkTag(std::string(this->m_edit->Text())) == true) {
      this->m_edit->SetText("");
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CLOSE)), 0xFF, 0, 0, 0);
      return 0;
    }

    char text[256];
    std::strcpy(text, this->m_edit->Text());
    this->m_edit->PushHistory();
    g_windowMgr.SetFocus(this->m_edit);
    if (std::strlen(text) == 0) {
      return 0;
    }

    if (g_serviceType == 1 && !ValidateCharacterName(text)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ENGLISHONLY)), 0xFF, 0, 0, 0);
      return 0;
    }

    if (this->m_target.size() == 0 || text[0] == '/') {
      if (g_session.ProcessEmotionShortcut(text) != 0) {
        return 0;
      }

      std::string parts[3];
      int talkType;
      const int offset = g_session.GetTalkType(text, &talkType, parts);
      if (offset != -1) {
        g_windowMgr.SetChatInputText(&text[offset]);
      }

      if (talkType == 24) {
        g_windowMgr.Schedule(this);
      } else if (talkType == 51) {
        this->SaveChatLog();
      } else {
        g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, talkType, reinterpret_cast<std::intptr_t>(parts), 0, 0);
      }
      return 0;
    }

    if (this->m_edit->links.size() != 0) {
      g_windowMgr.SetChatInputText(this->m_edit->LinkedText().c_str());
    } else if (this->m_edit->itemTags != nullptr) {
      g_windowMgr.SetChatInputText(this->m_edit->itemTags->linkedText.c_str());
    } else {
      g_windowMgr.chatInputText.assign(text, std::strlen(text));
    }

    g_modeMgr.gameMode->SendMsg(MM_WHISPERMSG, reinterpret_cast<std::intptr_t>(this->m_target.c_str()), 0, 0, 0);
    return 0;
  }
  case UM_MENU_SELECTED:
    if (this->m_log[a].value == 11) {
      this->SaveChatLog();
    }
    return 0;
  case UM_MAKEMENU: {
    this->m_log.clear();
    if (g_windowMgr.menuWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }

    this->m_log.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_WRITE_CHAT_TO_FILE)).c_str(), 11));
    if (!g_windowMgr.IsFriend(this->m_target.c_str()) && g_serviceMode != 3) {
      this->m_log.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_ADD_TO_FRIEND_CHAR_LIST)).c_str(), 21));
    }

    g_modeMgr.gameMode->SendMsg(MM_INITWHISPERMENU, reinterpret_cast<std::intptr_t>(this->m_target.c_str()), 0, 0, 0);
    if (this->m_log.empty()) {
      return 0;
    }

    UIWindow* menu = g_windowMgr.EnsureWindow(WID_MENUWND);
    for (unsigned index = 0; index < this->m_log.size(); ++index) {
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->m_log[index].text.c_str()), 0, 0, 0);
    }
    menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
    menu->Move(
      std::min(std::max(g_mouse.x + 1, 1), g_renderer->viewport.width - menu->geometry.width),
      std::min(std::max(g_mouse.y + 1, 1), g_renderer->viewport.height - menu->geometry.height)
    );
    return 0;
  }
  case UM_ADDCHAT: {
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
  case UM_PAGEUPPUSH:
    if (this->m_chatHistory->verticalOverflow != 0) {
      this->m_chatHistory->SendMsg(nullptr, UM_SCROLLVERTPAGEUP, 0, 0, 0, 0);
    }
    return 0;
  case UM_PAGEDOWNPUSH:
    if (this->m_chatHistory->verticalOverflow != 0) {
      this->m_chatHistory->SendMsg(nullptr, UM_SCROLLVERTPAGEDOWN, 0, 0, 0, 0);
    }
    return 0;
  case UM_RESIZE:
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
      g_windowMgr.StoreSnapRectangle(this);
    }
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
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF00, 0, 0, 0);
  }
}

// OFFSET: 0x0090C350
// FUNCTION: fn_12443
int UISecondPasswordInputWnd::FindKeypadDigit(unsigned int target) {
  for (int i = 0; i < 10; ++i) {
    if (this->m_digits[i] == target) {
      return i;
    }
  }
  return -1;
}

// OFFSET: 0x0090C380
// FUNCTION: fn_12444
void UISecondPasswordInputWnd::DisableButtons() {
  for (int i = 0; i < 11; ++i) {
    UIButton* button = this->keypadButtons[i];
    button->SetDisabled(1);
    button->Invalidate();
  }
  for (int i = 0; i < 5; ++i) {
    UIButton* button = this->actionButtons[i];
    button->SetDisabled(1);
    button->Invalidate();
  }
}

// OFFSET: 0x0090C8C0
// FUNCTION: fn_12450
std::vector<unsigned char> MakeFramedMessage(std::uint16_t type, const std::vector<unsigned char>& body) {
  std::vector<unsigned char> message(body.size() + sizeof(FramedMessageHeader), 0);
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
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_RESTART:
    mode->SendMsg(MM_RESTART, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_QUITWND);
    break;
  case ID_EXIT:
    mode->SendMsg(MM_QUIT_GAME, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_QUITWND);
    break;
  case ID_CANCEL:
    g_windowMgr.CloseWindow(WID_QUITWND);
    break;
  }

  return 0;
}

// OFFSET: 0x009013F0
// FUNCTION: fn_12374
int UIRestartWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_TO_CHARACTER_SELECT:
    mode->SendMsg(MM_RESTART, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_RESTARTWND);
    g_windowMgr.CloseWindow(WID_ROULLETTEWND);
    break;
  case ID_RESTART:
    mode->SendMsg(MM_RESTART, 0, 0, 0, 0);
    [[fallthrough]];
  case ID_CANCEL:
    g_windowMgr.CloseWindow(WID_RESTARTWND);
    break;
  case ID_QUIT:
    g_windowMgr.CloseWindow(WID_RESTARTWND);
    mode->SendMsg(MM_QUIT_GAME, 0, 0, 0, 0);
    break;
  }

  return 0;
}

// OFFSET: 0x00909830
// FUNCTION: fn_12392
int UInCash_CallWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  if (a == ID_STATUS) {
    if (g_session.characterRenameEnabled) {
      g_modeMgr.GetGameMode()->SendMsg(MM_STOVE_AUTHENTICATION, 2, 0, 0, 0);
      return 0;
    }
    g_modeMgr.GetGameMode()->SendMsg(MM_REQUEST_CASH_SHOP, 0, 0, 0, 0);
  }
  return 0;
}

// OFFSET: 0x009098A0
// FUNCTION: fn_12393
void UIEquipWnd::UpdateCharacterView(int reset) {
  CMode* mode = g_modeMgr.gameMode;
  if (!reset && g_session.replayPacketMode > 0) {
    mode->SendMsg(MM_REQ_CARTOFF, 6, 0, 0, 0);
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
  mode->SendMsg(MM_REQ_CARTOFF, view, 0, 0, 0);
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
  this->m_textViewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text), 38, 0, 0);
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
void UIChooseWnd::StoreInfo() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
  }
}

// OFFSET: 0x00909E30
// FUNCTION: fn_12403
void UICombinedCardItemCollectionWnd::StoreInfo() {
  if (this->m_cardPlacement) {
    this->m_cardPlacement->x = this->geometry.x;
    this->m_cardPlacement->y = this->geometry.y;
    this->m_cardPlacement->width = this->geometry.width;
    this->m_cardPlacement->height = this->geometry.height;
  }
}

// OFFSET: 0x00909E70
// FUNCTION: fn_12404
void UIEquipWnd::StoreInfo() {
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
void UIItemCollectionWnd::StoreInfo() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x00909F40
// FUNCTION: fn_12407
void UIItemCompositionWnd::StoreInfo() {
  if (this->m_compositionPlacement) {
    this->m_compositionPlacement->width = this->geometry.width;
    this->m_compositionPlacement->height = this->geometry.height;
    this->m_compositionPlacement->x = this->geometry.x;
    this->m_compositionPlacement->y = this->geometry.y;
  }
}

// OFFSET: 0x00909F80
// FUNCTION: fn_12408
void UIItemIdentifyWnd::StoreInfo() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
  }
}

// OFFSET: 0x00909FC0
// FUNCTION: fn_12409
void UIItemParamChangeDisplayWnd::StoreInfo() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x0090A000
// FUNCTION: fn_12410
void UIPartySettingWnd::StoreInfo() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x0090A040
// FUNCTION: fn_12411
void UISayDialogWnd::StoreInfo() {
  if (this->m_placement) {
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
  }
}

// OFFSET: 0x0090A180
// FUNCTION: fn_12414
void UInCashWnd::StoreInfo() {
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
  if (g_windowMgr.rodexComposeWnd) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UNEQUIP_ARROW2)), 0x64FFFF, 0, 0, 0);
    return 0;
  }

  switch (message) {
  case UM_SET_TARGET:
    this->m_responseMessage = static_cast<int>(a);
    return 0;
  case UM_SETINFO:
    this->m_placement = reinterpret_cast<WindowPlacement*>(a);
    static_cast<SayDialogWndInfo*>(this->m_placement)->IsValid();
    this->Move(this->m_placement->x, this->m_placement->y);
    return 0;
  case UM_UPPUSH:
    this->m_viewer->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
    return 0;
  case UM_DOWNPUSH:
    this->m_viewer->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    return 0;
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, this->m_responseMessage);
    if (g_replayAnimationScaling != 0) {
      this->m_responseMessage = static_cast<int>(d);
    }
    switch (a) {
    case ID_NEXT: {
      const DWORD tick = GetTickCount();
      if (tick - *reinterpret_cast<const DWORD*>(&g_session.itemSendState[0x04]) < 300) {
        return 0;
      }
      *reinterpret_cast<DWORD*>(&g_session.itemSendState[0x04]) = tick;
      g_modeMgr.gameMode->SendMsg(this->m_responseMessage, this->m_responseId, 218, 0, 0);
      this->m_responsePending = 1;
      this->SendMsg(nullptr, UM_DISABLENEXTBUTTON, 0, 0, 0, 0);
      break;
    }
    case 600:
      break;
    default:
      g_modeMgr.gameMode->SendMsg(this->m_responseMessage, this->m_responseId, static_cast<int>(a), 0, 0);
      break;
    }
    return 0;
  }
  case UM_SETSAYDIALOG:
    if (a != 0) {
      if (this->m_responsePending != 0) {
        static_cast<UITagTextViewer*>(this->m_viewer)->Clear();
        this->m_responsePending = 0;
      }
      this->m_viewer->SendMsg(nullptr, UM_SETTEXT, a, c, 1, 0);
    }
    this->m_responseId = static_cast<int>(b);
    return 0;
  case UM_ENABLENEXTBUTTON:
    this->m_message0 = 218;
    this->m_responseMessage = 22;
    this->SendMsg(nullptr, UM_ADD_BUTTON, 218, reinterpret_cast<std::intptr_t>("btn_next"), 0, 0);
    this->Invalidate();
    return 0;
  case UM_ENABLECLOSEBUTTON:
    this->m_message0 = 201;
    this->m_responseMessage = 89;
    this->SendMsg(nullptr, UM_ADD_BUTTON, 218, reinterpret_cast<std::intptr_t>("btn_close"), 0, 0);
    this->Invalidate();
    return 0;
  case UM_ADD_BUTTON: {
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
  case UM_DISABLENEXTBUTTON:
    this->m_message0 = 600;
    this->SendMsg(nullptr, UM_DELETE_BUTTON, 218, 0, 0, 0);
    this->Invalidate();
    return 0;
  case UM_DELETE_BUTTON:
    for (auto it = this->m_buttons.begin(); it != this->m_buttons.end(); ++it) {
      if ((*it)->geometry.id == a) {
        (*it)->Move(-100, -100);
        g_windowMgr.Schedule(*it);
        this->m_buttons.erase(it);
        break;
      }
    }
    return 0;
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 22500) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        switch (header.type) {
        case 22501:
          return 0;
        case 22503: {
          char text[260];
          std::memset(text, 0, sizeof(text));
          std::memcpy(text, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, UM_SETSAYDIALOG, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
          break;
        }
        case 22504:
          std::memcpy(&this->m_responsePending, records + position, header.length);
          position += header.length;
          break;
        default:
          position += header.length;
          break;
        }
      }
    }
    return 0;
  }
  case UM_CLEAR_ITEM:
    static_cast<UITagTextViewer*>(this->m_viewer)->Clear();
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008FAF50
// FUNCTION: fn_12344
int UIExchangeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SET_EXCHANGE_CHAR_NAME:
    this->partnerName = reinterpret_cast<const char*>(a);
    this->partnerId = static_cast<int>(b);
    this->partnerLevel = static_cast<short>(c);
    return 0;
  case UM_MYDRAGCLOSED:
    this->isLocalTrader = 1;
    this->CancelInput();
    this->LayoutButtons();
    return 0;
  case UM_OTHERDRAGCLOSED:
    this->localConfirmed = 1;
    this->CancelInput();
    this->LayoutButtons();
    return 0;
  case UM_COMMAND: {
    CMode* mode = g_modeMgr.GetGameMode();
    switch (a) {
    case ID_CHECK:
      this->takeScreenshot = b != 0;
      return 0;
    case ID_OK:
      if (this->remoteConfirmed != 0) {
        return 0;
      }
      if (this->zenyEdit) {
        char text[24];
        std::sprintf(text, "%s ", this->zenyEdit->Text());
        if (!g_session.IsNumberWithCommas(text)) {
          g_windowMgr.Confirm(LookupMessage(MSI_CAN_INPUT_NUMBER_ONLY), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }

        std::int64_t money = _atoi64(text);
        if (money > 0x7FFFFFFF) {
          money = 0x7FFFFFFF;
        }
        int amount = static_cast<int>(MaxInt64(money, 0));
        if (amount < 0) {
          amount = 0;
        }
        if (amount > g_session.m_zeny) {
          amount = g_session.m_zeny;
        }
        mode->SendMsg(MM_ADD_EXCHANGE_ITEM, 0, g_session.exchangeMoney + amount, 0, 0);
        this->CancelInput();
      }
      mode->SendMsg(MM_CONCLUDE_EXCHANGE_ITEM, 0, 0, 0, 0);
      this->remoteConfirmed = 1;
      return 0;
    case ID_EXCHANGE:
      if (this->localAccepted != 0) {
        return 0;
      }
      if (this->takeScreenshot) {
        const std::string text = FormatOwnedString(LookupMessage(MSI_SCREENSHOT_FOR_EXCHANGE_CHARNAME), this->partnerName.c_str());
        g_modeMgr.gameMode->SendMsg(MM_SCREENSHOT, reinterpret_cast<int>(text.c_str()), 0, 0, 0);
      }
      mode->SendMsg(MM_EXEC_EXCHANGE_ITEM, 0, 0, 0, 0);
      this->localAccepted = 1;
      this->LayoutButtons();
      return 0;
    case ID_CANCEL:
      if (this->remoteAccepted != 0) {
        return 0;
      }
      mode->SendMsg(MM_CANCEL_EXCHANGE_ITEM, 0, 0, 0, 0);
      this->remoteAccepted = 1;
      return 0;
    case ID_RIGHT: {
      if (!this->zenyEdit) {
        return 0;
      }
      int amount = std::atoi(this->zenyEdit->Text());
      if (amount < 0) {
        amount = 0;
      }
      if (amount > g_session.m_zeny) {
        amount = g_session.m_zeny;
      }
      mode->SendMsg(MM_ADD_EXCHANGE_ITEM, 0, g_session.exchangeMoney + amount, 0, 0);
      this->CancelInput();
      return 0;
    }
    }
    return 0;
  }
  case UM_REFRESHITEM:
    this->m_myItems.erase(this->m_myItems.begin(), this->m_myItems.end());
    this->m_otherItems.erase(this->m_otherItems.begin(), this->m_otherItems.end());
    for (int index = 0; index < 10; ++index) {
      ITEM_INFO item = g_session.GetExchangeItem(index);
      if (item.quantity != 0) {
        this->m_myItems.push_back(item);
      }
      ITEM_INFO slot = g_session.GetItemSlot(index);
      if (slot.quantity != 0) {
        this->m_otherItems.push_back(slot);
      }
    }
    this->Invalidate();
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    mode->SendMsg(MM_ADD_EXCHANGE_ITEM, mode->drag.id, mode->drag.quantity, 0, 0);
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header{};
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 20600) {
      unsigned int position = sizeof(header);
      int number = 0;
      short level = 0;
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        switch (header.type) {
        case 20603:
          std::memcpy(&number, records + position, header.length);
          position += header.length;
          break;
        case 20604:
          std::memcpy(&level, records + position, header.length);
          position += header.length;
          break;
        case 20602: {
          char name[260];
          std::memset(name, 0, sizeof(name));
          std::memcpy(name, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, UM_SET_EXCHANGE_CHAR_NAME, reinterpret_cast<std::intptr_t>(name), number, level, 0);
          break;
        }
        case 20605:
          std::memcpy(&this->isLocalTrader, records + position, header.length);
          position += header.length;
          if (this->isLocalTrader != 0) {
            this->SendMsg(nullptr, UM_MYDRAGCLOSED, 0, 0, 0, 0);
          }
          break;
        case 20606:
          std::memcpy(&this->localConfirmed, records + position, header.length);
          position += header.length;
          if (this->localConfirmed != 0) {
            this->SendMsg(nullptr, UM_OTHERDRAGCLOSED, 0, 0, 0, 0);
          }
          break;
        case 20607:
          std::memcpy(&this->remoteConfirmed, records + position, header.length);
          position += header.length;
          if (this->remoteConfirmed != 0) {
            this->SendMsg(nullptr, UM_COMMAND, ID_OK, 0, 0, 0);
          }
          break;
        case 20608:
          std::memcpy(&this->localAccepted, records + position, header.length);
          position += header.length;
          if (this->localAccepted != 0) {
            this->SendMsg(nullptr, UM_COMMAND, ID_EXCHANGE, 0, 0, 0);
          }
          break;
        case 20609:
          std::memcpy(&this->remoteAccepted, records + position, header.length);
          position += header.length;
          if (this->remoteAccepted != 0) {
            this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
          }
          break;
        case 20601:
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          return 0;
        default:
          position += header.length;
          break;
        }
      }
    }
    return 0;
  }
  case UM_SCROLLVERT:
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008FB850
// FUNCTION: fn_12345
int UIGraffiStrboxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND: {
    if (a == ID_OK) {
      struct {
        int area[4];
        const char* text;
      } request;
      request.area[0] = this->area[0];
      request.area[1] = this->area[1];
      request.area[2] = this->area[2];
      request.area[3] = this->area[3];
      request.text = this->edit->Text();
      g_modeMgr.GetGameMode()->SendMsg(MM_USE_SKILL_TOGROUND_WITH_TALKBOX, reinterpret_cast<int>(&request), 0, 0, 0);
    } else if (a != ID_CANCEL) {
      return 0;
    }

    g_windowMgr.Schedule(this);
    return 0;
  }
  case UM_SET_SKILL_INFO: {
    const int* area = reinterpret_cast<const int*>(a);
    this->area[0] = area[0];
    this->area[1] = area[1];
    this->area[2] = area[2];
    this->area[3] = area[3];
    return 0;
  }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008E4380
// FUNCTION: fn_12185
void UINotifyItemObtainWnd::OnCreate(int, int) {
}

// OFFSET: 0x008E7120
// FUNCTION: fn_12193
void UIPetTamingDeceiveWnd::OnCreate(int, int) {
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
    if (manager->m_currentEntry.images[i].kind == 6) {
      const TipBoxImage& image = manager->m_currentEntry.images[i];
      const float dx = static_cast<float>(image.toX - image.fromX);
      const float dy = static_cast<float>(image.toY - image.fromY);
      int count = 5;
      if (image.duration != 0.0f) {
        count = static_cast<int>(image.duration);
      }

      for (int step = 0; step < count; ++step) {
        TipBoxDot dot;
        dot.x = static_cast<float>(step) * dx / static_cast<float>(count) + static_cast<float>(manager->m_currentEntry.images[i].fromX);
        dot.y = static_cast<float>(step) * dy / static_cast<float>(count) + static_cast<float>(manager->m_currentEntry.images[i].fromY);
        dot.visible = false;
        this->m_dots.push_back(dot);
      }
    } else {
      TipBoxAnimation animation;
      animation.kind = manager->m_currentEntry.images[i].kind;
      animation.x = static_cast<float>(manager->m_currentEntry.images[i].fromX);
      animation.y = static_cast<float>(manager->m_currentEntry.images[i].fromY);
      animation.moveStart = timeGetTime();
      animation.blink = manager->m_currentEntry.images[i].blink;
      animation.blinkStart = timeGetTime();
      animation.fromX = manager->m_currentEntry.images[i].fromX;
      animation.fromY = manager->m_currentEntry.images[i].fromY;
      animation.toX = manager->m_currentEntry.images[i].toX;
      animation.toY = manager->m_currentEntry.images[i].toY;
      animation.duration = manager->m_currentEntry.images[i].duration;
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
    this->m_expandedHeight = this->m_info->expandedHeight;
    if (this->geometry.height != 17) {
      this->Resize(this->geometry.width, this->m_expandedHeight);
    }
  }

  this->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
}

// OFFSET: 0x008FEAA0
// FUNCTION: fn_12354
int UILoadingWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == UM_LOADINGPERCENT) {
    char text[100];
    std::sprintf(text, "Loading %d %s", static_cast<int>(argument1), "%");
    this->m_progressBar->SetValue(static_cast<float>(argument1));
  }
  return 0;
}

// OFFSET: 0x008CED00
// FUNCTION: fn_12071
int UIItemTagCtrl::MeasureText(const std::string& text, const unsigned& bytes) {
  return this->TextWidth(text.c_str(), bytes, 0, 12, 0, 0);
}

// OFFSET: 0x008E8390
// FUNCTION: fn_12197
void UISayDialogWnd::OnCreate(int width, int height) {
  auto* viewer = new UITagTextViewer();
  this->m_viewer = viewer;
  viewer->SetSize(width - 20, height - 40);
  viewer->m_backgroundColor = 0xFFF2F2F2;
  viewer->Invalidate();
  viewer->m_textWidth = width - 65;
  viewer->Move(10, 10);
  this->AddChild(viewer);
  this->m_message0 = 600;
  this->m_responseId = 0;
  this->m_responsePending = 0;
}

// OFFSET: 0x008FF810
// FUNCTION: fn_12359
int UIMonsterDisplayWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND: {
    if (argument1 == ID_CLOSE) {
      g_windowMgr.CloseWindow(WID_MONSTERDISPLAYWND);
    }
    return 0;
  }
  case UM_CARDITEMILLUSTRATE: {
    this->m_name = reinterpret_cast<const char*>(argument1);
    this->Invalidate();
    g_windowMgr.Promote(this);
    return 0;
  }
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008FFBA0
// FUNCTION: fn_12362
int UINotifyJobLevelUpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }
  if (a == ID_STATUS) {
    g_windowMgr.Promote(g_windowMgr.EnsureWindow(WID_SKILLLISTWND));
  }
  return 0;
}

// OFFSET: 0x008E0A20
// FUNCTION: fn_12173
void UIExchangeWnd::OnCreate(int, int) {
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  this->isLocalTrader = 0;
  this->localConfirmed = 0;
  this->remoteConfirmed = 0;
  this->localAccepted = 0;
  this->remoteAccepted = 0;

  const std::string path("유저인터페이스\\btn_");
  const char* const names[3] = {"ok", "exchange", "cancel"};
  const int ids[3] = {0xB8, 0xE6, 0xB9};
  const int x[3] = {3, 260, 510};
  const int y[3] = {
      this->isLocalTrader * 120, this->isLocalTrader != 0 && this->localConfirmed != 0 && this->localAccepted == 0 ? 0 : 120, 0
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
  this->zenyEdit = edit;
  edit->SetSize(0x58, 0x10);
  edit->Move(0x91, 0x14A);
  edit->maximum = 0xD;
  edit->Background(0xE8, 0xE8, 0xE8);
  edit->SetText("0");
  this->AddChild(edit);
  g_windowMgr.SetFocus(edit);
  edit->SelectAll();

  UICheckBox* checkBox = new UICheckBox();
  this->screenshotCheckBox = checkBox;
  checkBox->SetSize(0x30, 0xC);
  checkBox->SetBitmaps("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp");
  checkBox->Move(0x69, this->geometry.height - 0x13);
  checkBox->SetChecked(this->takeScreenshot);
  this->AddChild(checkBox);

  this->m_message0 = 600;
}

// OFFSET: 0x008DE400
// FUNCTION: fn_12167
void UIComboBoxIDWnd::OnCreate(int width, int height) {
  auto* list = new UIComboBoxID();
  this->m_list = list;
  list->Configure(0, 0, width, height, 1);
  this->AddChild(list);
}

// OFFSET: 0x008D9F20
// FUNCTION: fn_12139
bool IsEquipViewHidden() {
  if (g_session.replayPacketMode > 0) {
    return false;
  }

  if (StateFlag04(g_session.m_jobFlags) || StateFlag05(g_session.m_jobFlags) || StateFlag19(g_session.m_jobFlags) || StateFlag23(g_session.m_jobFlags) ||
      StateFlag24(g_session.m_jobFlags) || StateFlag25(g_session.m_jobFlags) || StateFlag26(g_session.m_jobFlags) || StateFlag22(g_session.m_jobFlags)) {
    return false;
  }

  return true;
}

// OFFSET: 0x008ED080
// FUNCTION: fn_12213
void UIChatRoomMakeWnd::Paint() {
  auto* titleBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_fix.bmp")));
  this->DrawBitmap(0, 0, titleBar, false);
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\chatwin0_bg.bmp")));
  this->DrawBitmap(0, 17, background, false);
  this->DrawTextAt(9, 4, this->m_title.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(8, 3, this->m_title.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008DBD50
// FUNCTION: fn_12161
void UIChatRoomMakeWnd::OnCreate(int width, int height) {
  if (g_session.IsTrickDeadActive()) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_MAKE_CHAT_ROOM)), 0xFF, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_CHATROOMMAKEWND);
  }

  this->m_limit = 20;
  this->m_type = 0;
  this->m_typeLabels.push_back(std::string(LookupMessage(MSI_CHAT_ROOM)));
  {
    const std::uint32_t value = 0;
    this->m_types.push_back(value);
  }

  this->m_limitLabels.push_back("2 " + std::string(LookupMessage(MSI_COUNT_UNIT_OF_PEOPLE)));
  {
    const std::uint32_t value = 2;
    this->m_limits.push_back(value);
  }
  this->m_limitLabels.push_back("3 " + std::string(LookupMessage(MSI_COUNT_UNIT_OF_PEOPLE)));
  {
    const std::uint32_t value = 3;
    this->m_limits.push_back(value);
  }
  this->m_limitLabels.push_back("5 " + std::string(LookupMessage(MSI_COUNT_UNIT_OF_PEOPLE)));
  {
    const std::uint32_t value = 5;
    this->m_limits.push_back(value);
  }
  this->m_limitLabels.push_back("10 " + std::string(LookupMessage(MSI_COUNT_UNIT_OF_PEOPLE)));
  {
    const std::uint32_t value = 10;
    this->m_limits.push_back(value);
  }
  this->m_limitLabels.push_back("15 " + std::string(LookupMessage(MSI_COUNT_UNIT_OF_PEOPLE)));
  {
    const std::uint32_t value = 15;
    this->m_limits.push_back(value);
  }
  this->m_limitLabels.push_back("20 " + std::string(LookupMessage(MSI_COUNT_UNIT_OF_PEOPLE)));
  {
    const std::uint32_t value = 20;
    this->m_limits.push_back(value);
  }

  this->m_closeButton = this->AddCloseButton("Alt+C", 0);

  {
    const std::string prefix("유저인터페이스\\");
    const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    int positions[2][2] = {{189, 96}, {234, 96}};
    int ids[2] = {184, 185};
    for (int index = 0; index < 2; ++index) {
      UIBitmapButton* button = new UIBitmapButton();
      button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[index][0], positions[index][1]);
      button->SetId(ids[index]);
      this->AddChild(button);
    }
  }
  this->m_message0 = 184;

  {
    const std::string prefix("유저인터페이스\\basic_interface\\");
    const char* names[2][3] = {{"txtbox_btn_a", "txtbox_btn_b", "txtbox_btn_c"}, {"txtbox_btn_a", "txtbox_btn_b", "txtbox_btn_c"}};
    int positions[2][2] = {{119, 46}, {256, 46}};
    int ids[2] = {228, 229};
    for (int index = 0; index < 2; ++index) {
      UIBitmapButton* button = new UIBitmapButton();
      button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[index][0], positions[index][1]);
      button->SetId(ids[index]);
      this->AddChild(button);
    }
  }

  this->m_titleEdit = new UIEditCtrl();
  this->m_titleEdit->SetSize(228, 16);
  this->m_titleEdit->Move(45, 24);
  this->m_titleEdit->maximum = 36;
  this->m_titleEdit->Background(242, 242, 242);
  this->m_titleEdit->SetText("");
  this->AddChild(this->m_titleEdit);
  g_windowMgr.SetFocus(this->m_titleEdit);

  this->m_passwordEdit = new UIEditCtrl();
  this->m_passwordEdit->SetSize(90, 16);
  this->m_passwordEdit->Move(182, 70);
  this->m_passwordEdit->maximum = 8;
  this->m_passwordEdit->mask = 42;
  this->m_passwordEdit->Background(242, 242, 242);
  this->m_passwordEdit->SetText("");
  this->AddChild(this->m_passwordEdit);

  this->m_public = 0;
  const char* labels[2] = {LookupMessage(MSI_ROOM_PUBLIC2), LookupMessage(MSI_ROOM_PRIVATE2)};
  const int states[2] = {1, 0};
  for (int index = 0, width = 40, x = 44; width < 64; ++index, width += 12, x += 52) {
    this->m_publicButtons[index] = new UIRadioBtn();
    this->m_publicButtons[index]->SetSize(width, 12);
    this->m_publicButtons[index]->Move(x, 70);
    this->m_publicButtons[index]->SetText(labels[index]);
    this->m_publicButtons[index]->SendMsg(this, UM_SETSTATE, states[index], 0, 0, 0);
    this->AddChild(this->m_publicButtons[index]);
  }

  this->m_limitText = new UIStaticText();
  this->m_limitText->SetSize(68, 14);
  this->m_limitText->Move(48, 49);
  this->m_limitText->SetText(this->m_limitLabels.back().c_str(), 0);
  this->m_limitText->background = 0xFFF2F2F2;
  this->m_limitText->Invalidate();
  this->AddChild(this->m_limitText);

  this->m_typeText = new UIStaticText();
  this->m_typeText->SetSize(68, 14);
  this->m_typeText->Move(187, 49);
  this->m_typeText->SetText(this->m_typeLabels.front().c_str(), 0);
  this->m_typeText->background = 0xFFF2F2F2;
  this->m_typeText->Invalidate();
  this->AddChild(this->m_typeText);
}

// OFFSET: 0x008F80B0
// FUNCTION: fn_12330
int UIChatRoomMakeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      if (g_windowMgr.HasTitleAtActorCell(g_session.m_aid)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_MAKE_CHAT_ROOM)), 0xFF, 0, 0, 0);
        break;
      }

      int error;
      if (this->m_titleEdit->Length() == 0) {
        error = 13;
      } else if (g_forbiddenWords.Contains(this->m_titleEdit->Text())) {
        error = 14;
      } else if (g_serviceType == 1 && !ValidateCharacterName(this->m_titleEdit->Text())) {
        error = 190;
      } else if (this->m_passwordEdit->Length() < 4 && this->m_public == 1) {
        error = this->m_passwordEdit->Length() != 0 ? 16 : 15;
      } else {
        ChatRoomSetting setting;
        setting.limit = this->m_limit;
        setting.memberCount = 0;
        setting.isPublic = this->m_public != 1;
        setting.roomId = -1;
        setting.title = this->m_titleEdit->Text();
        setting.password = this->m_passwordEdit->Text();
        g_modeMgr.gameMode->SendMsg(MM_CREATE_CHATROOM, reinterpret_cast<int>(&setting), 0, 0, 0);
        g_windowMgr.CloseWindow(WID_CHATROOMMAKEWND);
        break;
      }
      g_windowMgr.Confirm(LookupMessage(error), 0, true, false, 0, "Message", 280, 120, 0);
      break;
    }
    case ID_CANCEL:
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_CHATROOMMAKEWND);
      break;
    case ID_RADIOBTN:
      for (int i = 0; i < 2; ++i) {
        this->m_publicButtons[i]->Invalidate();
        if (sender == this->m_publicButtons[i]) {
          this->m_public = i;
          this->m_publicButtons[i]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
        } else {
          this->m_publicButtons[i]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        }
      }
      break;
    case ID_NUMCOMBO:
    case ID_CLASSCOMBO: {
      if (g_windowMgr.comboBoxWnd) {
        g_windowMgr.CloseWindow(WID_COMBOBOXWND);
      }
      UIWindow* list = g_windowMgr.EnsureWindow(WID_COMBOBOXWND);
      int count = 1;
      if (a == ID_NUMCOMBO) {
        this->m_listKind = 0;
        count = static_cast<int>(this->m_limitLabels.size());
        for (unsigned int i = 0; i < this->m_limitLabels.size(); ++i) {
          list->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->m_limitLabels[i].c_str()), 0, 0, 0);
        }
      } else if (a == ID_CLASSCOMBO) {
        this->m_listKind = 1;
        count = static_cast<int>(this->m_typeLabels.size());
        for (unsigned int i = 0; i < this->m_typeLabels.size(); ++i) {
          list->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->m_typeLabels[i].c_str()), 0, 0, 0);
        }
      }

      const int height = count * 16;
      list->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
      list->Resize(74, height > 48 ? 48 : height);
      list->SendMsg(nullptr, UM_SET_CALLER_WID, 27, 0, 0, 0);
      int x = this->m_listKind * 137 + 44;
      int y = 64;
      this->ToScreen(x, y);
      list->Move(x + 1, y);
      break;
    }
    }
    break;
  case UM_COMBOBOX_SELECTED:
    if (this->m_listKind == 0) {
      this->m_limitText->SetText(this->m_limitLabels[a].c_str(), 0);
      this->m_limit = this->m_limits[a];
    } else if (this->m_listKind == 1) {
      this->m_typeText->SetText(this->m_typeLabels[a].c_str(), 0);
      this->m_type = this->m_types[a];
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008F7B10
// FUNCTION: fn_12329
int UIChatRoomChangeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      int error;
      if (this->m_titleEdit->Length() == 0) {
        error = 13;
      } else if (g_forbiddenWords.Contains(this->m_titleEdit->Text())) {
        error = 14;
      } else if (g_serviceType == 1 && !ValidateCharacterName(this->m_titleEdit->Text())) {
        error = 190;
      } else if (this->m_passwordEdit->Length() < 4 && this->m_public == 1) {
        error = this->m_passwordEdit->Length() != 0 ? 16 : 15;
      } else {
        ChatRoomSetting setting;
        setting.limit = this->m_limit;
        setting.memberCount = 0;
        setting.isPublic = this->m_public != 1;
        setting.roomId = -1;
        setting.title = this->m_titleEdit->Text();
        setting.password = this->m_passwordEdit->Text();
        g_modeMgr.gameMode->SendMsg(MM_CHANGE_CHATROOM, reinterpret_cast<int>(&setting), 0, 0, 0);
        g_windowMgr.CloseWindow(WID_CHATROOMCHANGEWND);
        return 0;
      }
      g_windowMgr.Confirm(LookupMessage(error), 0, true, false, 0, "Message", 280, 120, 0);
      return 0;
    }
    case ID_CANCEL:
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_CHATROOMCHANGEWND);
      return 0;
    case ID_NUMCOMBO:
    case ID_CLASSCOMBO: {
      if (g_windowMgr.comboBoxWnd) {
        g_windowMgr.CloseWindow(WID_COMBOBOXWND);
      }
      UIWindow* list = g_windowMgr.EnsureWindow(WID_COMBOBOXWND);
      int count = 1;
      if (a == ID_NUMCOMBO) {
        this->m_listKind = 0;
        count = static_cast<int>(this->m_limitLabels.size());
        for (unsigned int i = 0; i < this->m_limitLabels.size(); ++i) {
          list->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->m_limitLabels[i].c_str()), 0, 0, 0);
        }
      } else if (a == ID_CLASSCOMBO) {
        this->m_listKind = 1;
        count = static_cast<int>(this->m_typeLabels.size());
        for (unsigned int i = 0; i < this->m_typeLabels.size(); ++i) {
          list->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->m_typeLabels[i].c_str()), 0, 0, 0);
        }
      }

      const int height = count * 16;
      list->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
      list->Resize(74, height > 48 ? 48 : height);
      list->SendMsg(nullptr, UM_SET_CALLER_WID, 30, 0, 0, 0);
      int x = this->m_listKind * 137 + 44;
      int y = 64;
      this->ToScreen(x, y);
      list->Move(x + 1, y);
      return 0;
    }
    }
    break;
  case UM_SETINFO: {
    const ChatRoomSetting* setting = reinterpret_cast<const ChatRoomSetting*>(a);
    this->m_limit = setting->limit;
    this->m_limitText->SetText(this->m_limitLabels[this->m_limitLabels.size() - 1].c_str(), 0);
    this->m_limitText->Invalidate();
    this->m_passwordEdit->SetText(setting->password.c_str());
    this->m_titleEdit->SetText(setting->title.c_str());
    g_windowMgr.SetFocus(this->m_titleEdit);
    this->m_titleEdit->SelectAll();
    this->m_public = setting->isPublic == 0;
    for (int i = 0; i < 2; ++i) {
      this->m_publicButtons[i]->Invalidate();
      this->m_publicButtons[i]->SendMsg(this, UM_SETSTATE, i == this->m_public ? 1 : 0, 0, 0, 0);
    }
    this->m_closeButton->SetTooltip("");
    this->Invalidate();
    return 0;
  }
  }
  return UIChatRoomMakeWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008F64B0
// FUNCTION: fn_12317
void UIItemIdentifyWnd::UpdateScroll() {
  if (this->m_scrollBar == nullptr) {
    return;
  }

  auto* bar = static_cast<UIScrollBar*>(this->m_scrollBar);

  const int total = this->m_items.size() / this->m_columns + (this->m_items.size() % this->m_columns != 0);

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
  this->geometry.visible = visible;
}

// OFFSET: 0x00907610
// FUNCTION: fn_12387
int UIToolTipWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_OK:
  case ID_CANCEL:
    g_windowMgr.Schedule(this);
    break;
  }

  return 0;
}

// OFFSET: 0x00900D50
// FUNCTION: fn_12371
int UIPetTamingDeceiveWnd::SendMsg(
  UIWindow*, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == UM_SET_TARGET) {
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
void UIStatusWnd::StoreInfo() {
  if (this->m_info == nullptr) {
    return;
  }

  this->m_info->width = this->geometry.width;
  this->m_info->height = this->geometry.height;
  this->m_info->x = this->geometry.x;
  this->m_info->y = this->geometry.y;
  this->m_info->expandedHeight = this->m_expandedHeight;
  this->m_info->extended = this->m_extended == 1;
}

// OFFSET: 0x00907650
// FUNCTION: fn_12388
int UIWaitWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    if (argument1 == ID_CANCEL) {
      g_modeMgr.gameMode->SendMsg(LMM_DISCONNECT_ACSVR, 0, 0, 0, 0);
    }
    break;
  case UM_ENABLE_CANCEL:
    this->m_button->Move((this->geometry.width - this->m_button->BitmapWidth()) / 2, this->geometry.height - this->m_button->BitmapHeight() - 5);
    break;
  }

  return 0;
}

// OFFSET: 0x008FF8A0
// FUNCTION: fn_12360
int UINoticeConfirmWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_COMMAND: {
    switch (argument1) {
    case ID_OK:
      g_modeMgr.gameMode->SendMsg(this->m_event, 0, 0, 0, 0);
      break;
    case ID_CANCEL:
      g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
      break;
    }

    return 0;
  }
  case UM_SET_TARGET: {
    this->m_event = static_cast<int>(argument1);
    return 0;
  }
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
  if (width < limit) {
    return 1;
  }
  return 0;
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
  this->m_viewer->SendMsg(nullptr, UM_SCROLLVERT, 0, 0, 0, 0);
}

class UIAutoMessageWnd : public UIFrameWnd {
public:
  UIAutoMessageWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIEditCtrl* m_edit;
};

// OFFSET: 0x008ECE20
// FUNCTION: fn_12210
void UIAutoMessageWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(15, 6, LookupMessage(MSI_ENTER_AUTO_MSG), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008FFC40
// FUNCTION: fn_12364
int UINotifyQuestWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }
  if (a == ID_STATUS) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_RENEWQUESTUI)) {
      g_windowMgr.Promote(window);
    }
  }
  return 0;
}

namespace {
#pragma pack(push, 1)
struct NCashClosePacket {
  /* 0000 */ std::uint16_t packetType;
};

struct NCashBuyHeader {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t packetLength;
  /* 0004 */ std::uint16_t count;
  /* 0006 */ std::int32_t points;
};

struct NCashBuyItem {
  /* 0000 */ std::int32_t itemId;
  /* 0004 */ std::int32_t quantity;
  /* 0008 */ std::uint16_t tab;
};

struct NCashRentalPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ std::int32_t itemId;
};
#pragma pack(pop)
}

// OFFSET: 0x008EB280
// FUNCTION: fn_12208
void UInCashWnd::OnCreate(int width, int height) {
  g_session.ClearTradeItems();
  this->m_otherItems.clear();
  this->m_pageCount = 1;
  this->m_page = 1;
  g_session.ClearInventoryCategoryItems(10);
  this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
  this->AddCloseButton("Alt+X", 0);

  const int titleWidth = this->TextWidth(LookupMessage(MSI_NC_ITEM_SEARCH), 0, 0, 12, 1, 0);
  this->m_title = new UIStaticText();
  this->m_title->SetSize(titleWidth, 12);
  this->m_title->SetText(LookupMessage(MSI_NC_ITEM_SEARCH), 1);
  this->m_title->Move(20, 26);
  this->AddChild(this->m_title);

  this->m_searchEdit = new UIEditCtrl2();
  this->m_searchEdit->SetSize(64, 18);
  this->m_searchEdit->invalidatesParent = 1;
  this->m_searchEdit->flat = 0;
  this->m_searchEdit->Move(this->m_title->geometry.x + titleWidth, 22);
  this->m_searchEdit->maximum = 20;
  this->m_searchEdit->Background(215, 215, 215);
  this->AddChild(this->m_searchEdit);

  const char* names[19][3] = {
    {"신규b", "신규a", "신규a"}, {"인기b", "인기a", "인기a"}, {"한정b", "한정a", "한정a"}, {"임대장비b", "임대장비a", "임대장비a"},
    {"영구장비b", "영구장비a", "영구장비a"}, {"버프b", "버프a", "버프a"}, {"회복b", "회복a", "회복a"}, {"기타b", "기타a", "기타a"},
    {"특가b", "특가a", "특가a"}, {"검색b", "검색a", "검색a"}, {"NC_FirstPage", "NC_FirstPage", "NC_FirstPage"},
    {"NC_PrevPage", "NC_PrevPage", "NC_PrevPage"}, {"NC_NextPage", "NC_NextPage", "NC_NextPage"}, {"NC_LastPage", "NC_LastPage", "NC_LastPage"},
    {"btn_buy", "btn_buy_a", "btn_buy_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}, {"btn_close", "btn_close_a", "btn_close_b"},
    {"btn_find", "btn_find_a", "btn_find_b"}, {"btn_refresh", "btn_refresh_a", "btn_refresh_b"}};
  const int top = this->m_listHeight;
  const int bottom = this->geometry.height;
  const int right = this->geometry.width;
  int positions[19][2] = {
    {4, top + 42}, {4, top + 87}, {4, top + 132}, {4, top + 177}, {4, top + 222}, {4, top + 267}, {4, top + 312}, {4, top + 357},
    {4, top + 402}, {4, top + 447}, {203, bottom - 55}, {216, bottom - 55}, {276, bottom - 55}, {290, bottom - 55},
    {right - 150, bottom - 40}, {right - 100, bottom - 40}, {right - 50, bottom - 40},
    {this->m_searchEdit->geometry.x + this->m_searchEdit->geometry.width + 10, 22}, {-100, -100}};
  const int ids[19] = {389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 387, 316, 317, 388, 224, 386, 201, 365, 415};

  const std::string interfacePrefix("유저인터페이스\\basic_interface\\");
  const std::string prefix("유저인터페이스\\");
  for (int index = 0; index < 20; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
    this->m_buttons[index] = button;
    if (index < 15) {
      button->SetBitmap((interfacePrefix + names[index][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((interfacePrefix + names[index][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((interfacePrefix + names[index][2] + ".bmp").c_str(), 2, 0);
      button->SetBitmap((interfacePrefix + names[index][1] + ".bmp").c_str(), 3, 0);
    } else {
      button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
      button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 3, 0);
    }
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);

    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      this->m_buttons[index]->SetTooltip(LookupMessage(index + 0x736));
      break;
    case 8:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_BARGAINSALE_CASHSHOP_REFRESH_MSG));
      break;
    case 10:
    case 18:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_AUCTION_SEARCH));
      break;
    case 11:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_NC_FIRSTPAGE));
      break;
    case 12:
      this->m_buttons[index]->SetTooltip("<=");
      break;
    case 13:
      this->m_buttons[index]->SetTooltip("=>");
      break;
    case 14:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_NC_LASTPAGE));
      break;
    case 15:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_NC_BUY));
      break;
    case 16:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_NC_BLANK));
      break;
    case 17:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_BOOK_CLOSE));
      break;
    case 19:
      this->m_buttons[index]->SetTooltip(LookupMessage(MSI_BARGAINSALE_CASHSHOP_CHECK_COUNT_MSG));
      break;
    default:
      break;
    }
    this->AddChild(this->m_buttons[index]);
  }

  this->m_pointEdit = new UIEditCtrl2();
  this->m_pointEdit->SetSize(64, 14);
  this->m_pointEdit->invalidatesParent = 1;
  this->m_pointEdit->flat = 0;
  this->m_pointEdit->Move(358, this->geometry.height - 38);
  this->m_pointEdit->maximum = 64;
  this->m_pointEdit->Background(215, 215, 215);
  this->AddChild(this->m_pointEdit);

  this->m_otherScroll = new UIScrollBar();
  this->m_otherScroll->Configure(this->geometry.width - 15, 17, 1, this->m_listHeight + 431);
  this->m_otherScroll->SetRange(4, 4, 8);
  this->m_firstOther = 0;
  this->m_otherScroll->SetPosition(0);
  this->AddChild(this->m_otherScroll);
  this->m_otherScroll->Show(0);
}

// OFFSET: 0x00908260
// FUNCTION: fn_12391
int UInCashWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  char key[512];

  switch (message) {
  case UM_SETINFO:
    this->m_placement = reinterpret_cast<WindowPlacement*>(argument1);
    if (!static_cast<NCashWndInfo*>(this->m_placement)->IsValid()) {
      static_cast<NCashWndInfo*>(this->m_placement)->Reset();
    }
    this->Move(this->m_placement->x, this->m_placement->y);
    return 0;

  case UM_UPDATE_CASHSHOP_BALANCE:
    this->m_pointBalance = argument1;
    this->m_cashBalance = argument2;
    this->m_pointEdit->SetText("0");
    this->SendMsg(this, UM_COMMAND, 386, 0, 0, 0);
    return 0;

  case 120: {
    std::strcpy(key, g_subKey);
    std::strcat(key, "\\NeoCash");
    HKEY handle;
    const LSTATUS status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &handle);
    DWORD type = REG_DWORD;
    DWORD size = 4;
    this->m_tab = 0;
    if (status == ERROR_SUCCESS) {
      RegQueryValueExA(handle, "CASH_CATEGORY", nullptr, &type, reinterpret_cast<BYTE*>(&this->m_tab), &size);
    }
    RegCloseKey(handle);
    if (this->m_tab > 10) {
      this->m_tab = 10;
    } else if (this->m_tab < 0) {
      this->m_tab = 0;
    }
    this->m_buttons[this->m_tab]->SetDisabled(1);
    this->m_buttons[this->m_tab]->Invalidate();
    if (this->m_tab == 8) {
      this->m_otherScroll->Move(this->geometry.width - this->m_otherScroll->geometry.width - 10, 22);
    } else {
      this->m_otherScroll->Move(-100, -100);
    }
    this->m_pointBalance = argument1;
    this->m_cashBalance = argument2;
    this->SendMsg(this, UM_REFRESH_PAGES, 0, 0, 0, 0);
    return 0;
  }

  case UM_REFRESH_PAGES:
    this->m_pageCount = (g_session.GetInventoryCategorySize(this->m_tab) + 7) / 8;
    if (this->m_pageCount < 1) {
      this->m_pageCount = 1;
    }
    this->m_page = 1;
    this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
    return 0;

  case UM_SCROLLVERT:
    if (g_session.GetList2Size() < 4) {
      return 0;
    }
    if (argument1 < 0) {
      --this->m_firstOther;
    } else if (argument1 > 0) {
      ++this->m_firstOther;
    }
    if (this->m_firstOther < 0) {
      this->m_firstOther = 0;
    }
    if (this->m_firstOther > g_session.GetList2Size() - 3) {
      this->m_firstOther = g_session.GetList2Size() - 3;
    }
    this->m_otherScroll->SetPosition(this->m_firstOther);
    this->m_otherScroll->Invalidate();
    return 0;

  case UM_SCROLLVERTPAGEUP:
    if (--this->m_firstOther < 0) {
      this->m_firstOther = 0;
    }
    this->m_otherScroll->SetPosition(this->m_firstOther);
    this->m_otherScroll->Invalidate();
    return 0;

  case UM_SCROLLVERTPAGEDOWN:
    ++this->m_firstOther;
    if (this->m_firstOther > std::min(g_session.GetList2Size() - 3, 4)) {
      this->m_firstOther = std::min(g_session.GetList2Size() - 3, 4);
    }
    this->m_otherScroll->SetPosition(this->m_firstOther);
    this->m_otherScroll->Invalidate();
    return 0;

  case UM_REFRESH: {
    this->items.clear();
    const int count = g_session.GetInventoryCategorySize(this->m_tab);
    for (int index = 0; index < count; ++index) {
      ITEM_INFO item = g_session.GetItem1784At(this->m_tab, index);
      if (item.identity[1] != 0 && (this->m_tab != 8 || item.bindOnEquip != 0xFFFF)) {
        this->items.push_back(item);
      }
    }
    this->Invalidate();
    return 0;
  }

  case UM_COMMAND:
    switch (argument1) {
    case 386:
      this->m_totalPrice = 0;
      g_session.ClearPurchaseItems();
      this->m_otherItems.clear();
      this->m_firstOther = 0;
      this->m_otherScroll->SetPosition(0);
      this->m_otherScroll->Invalidate();
      this->m_otherScroll->Show(0);
      this->Invalidate();
      return 0;

    case ID_BUY: {
      if (this->m_totalPrice <= 0) {
        break;
      }
      int notice;
      if (this->m_cashBalance < std::atoi(this->m_pointEdit->Text()) || std::atoi(this->m_pointEdit->Text()) + this->m_pointBalance < this->m_totalPrice) {
        notice = 1254;
      } else if (std::atoi(this->m_pointEdit->Text()) <= this->m_totalPrice) {
        NCashBuyHeader header = {};
        header.packetType = ID_CZ_SE_PC_BUY_CASHITEM_LIST;
        const int points = std::atoi(this->m_pointEdit->Text());
        const int total = this->m_totalPrice;
        header.points = points;
        char text[0x800];
        if (points == 0) {
          FormatMessageBuffer(text, LookupMessage(MSI_BILLING_INFO_URL), total);
        } else {
          FormatMessageBuffer(text, LookupMessage(MSI_CHANGE_MATERIAL_INVSORT), total - points, points);
        }
        if (g_windowMgr.Confirm(text, 2, true, true, 0, "Message", 280, 120, 0) != 187) {
          break;
        }

        const int count = g_session.GetList2Size();
        const unsigned int length = (count + 1) * 10;
        if (length > 0x800) {
          break;
        }
        header.packetLength = static_cast<std::uint16_t>(length);
        header.count = static_cast<std::uint16_t>(count);
        char buffer[0x800];
        std::memcpy(buffer, &header, sizeof(header));
        if (count == 0) {
          break;
        }
        for (int index = 0; index < count; ++index) {
          ITEM_INFO item = g_session.GetItem1720At(index);
          NCashBuyItem entry;
          entry.itemId = std::atoi(item.itemId.c_str());
          entry.quantity = item.quantity;
          entry.tab = item.bindOnEquip;
          std::memcpy(buffer + sizeof(header) + index * sizeof(entry), &entry, sizeof(entry));
        }
        GetRagConnection().Send(static_cast<short>(length), buffer);
        return 0;
      } else {
        notice = 1860;
      }
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(notice)), 0xFF, 0, 0, 0);
      return 0;
    }

    case ID_CLOSE: {
      NCashClosePacket packet;
      packet.packetType = ID_CZ_SE_CASHSHOP_CLOSE;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
      g_windowMgr.CloseWindow(WID_NCASHWND);
      return 0;
    }

    case 365: {
      std::string query(this->m_searchEdit->Text());
      if (query.size() != 0) {
        g_session.ClearInventoryCategoryItems(10);
        for (int list = 0; list < 9; ++list) {
          const int count = g_session.GetInventoryCategorySize(list);
          for (int index = 0; index < count; ++index) {
            ITEM_INFO item = g_session.GetItem1784At(list, index);
            std::string name(item.displayName);
            if (name.find(query) != std::string::npos && !g_session.HasInventorySearchItem(std::atoi(item.itemId.c_str()))) {
              g_session.inventoryCategories[10].push_back(item);
            }
          }
        }
        if (g_session.GetInventoryCategorySize(10) == 0) {
          g_windowMgr.Confirm(LookupMessage(MSI_NOTHING_SEARCH_ITEM), 0, true, false, 0, "Message", 280, 120, 0);
        }
        this->m_searchEdit->SetText("");
        this->SendMsg(this, UM_COMMAND, 398, 0, 0, 0);
      }
      return 0;
    }

    case 387:
      this->m_page = 1;
      return 0;

    case ID_PREV_PAGE:
      if (this->m_page > 1) {
        --this->m_page;
        return 0;
      }
      break;

    case ID_NEXT_PAGE:
      if (this->m_page < this->m_pageCount) {
        ++this->m_page;
        return 0;
      }
      break;

    case 388:
      this->m_page = this->m_pageCount;
      return 0;

    case 415:
      if (this->items.begin() != this->items.end()) {
        for (ITEM_INFO& item : this->items) {
          NCashRentalPacket packet;
          packet.packetType = ID_CZ_REQ_COUNT_BARGAIN_SALE_ITEM;
          packet.accountId = g_session.m_aid;
          packet.itemId = std::atoi(item.itemId.c_str());
          GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
        }
        return 0;
      }
      break;
    case 389:
    case 390:
    case 391:
    case 392:
    case 393:
    case 394:
    case 395:
    case 396:
    case 397:
    case 398:
    case 477: {
      if (static_cast<unsigned int>(this->m_tab) < 11 && this->m_buttons[this->m_tab] != nullptr) {
        this->m_buttons[this->m_tab]->SetDisabled(0);
        this->m_buttons[this->m_tab]->Invalidate();
        this->m_buttons[this->m_tab]->SetState(0);
      }
      this->m_tab = argument1 - 389;
      this->m_buttons[argument1 - 389]->SetDisabled(1);
      this->m_buttons[argument1 - 389]->Invalidate();
      if (this->m_tab == 8) {
        this->m_otherScroll->Move(this->geometry.width - this->m_otherScroll->geometry.width - 10, 22);
      } else {
        this->m_otherScroll->Move(-100, -100);
      }

      std::strcpy(key, g_subKey);
      std::strcat(key, "\\NeoCash");
      HKEY handle;
      DWORD disposition = 0;
      if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, key, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &handle, &disposition) == ERROR_SUCCESS) {
        RegSetValueExA(handle, "CASH_CATEGORY", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_tab), 4);
        RegCloseKey(handle);
      }
      this->SendMsg(this, UM_REFRESH_PAGES, 0, 0, 0, 0);
      return 0;
    }

    }
    return 0;

  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->drag.source == 4) {
      ITEM_INFO item;
      item.Assign(reinterpret_cast<const RawItemData&>(mode->drag));
      g_session.RemoveItem1720ByName(item);
      this->m_totalPrice -= item.quantity * item.prices[1];
      if (g_session.GetList2Size() == 3) {
        this->m_firstOther = 0;
        this->m_otherScroll->SetPosition(0);
        this->m_otherScroll->SetRange(4, 4, 4);
        this->m_otherScroll->Invalidate();
        this->m_otherScroll->Show(0);
      } else if (g_session.GetList2Size() > 3 && g_session.GetList2Size() < 7) {
        this->m_otherScroll->SetRange(4, 4, g_session.GetList2Size() + 1);
        if (this->m_firstOther > g_session.GetList2Size() - 3) {
          this->m_firstOther = g_session.GetList2Size() - 3;
          this->m_otherScroll->SetPosition(this->m_firstOther);
        }
        this->m_otherScroll->Invalidate();
      }
    } else if (mode->drag.source == 6) {
      if (g_session.GetList2Size() >= 8) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return 0;
      }
      if (this->m_tab == 8) {
        const int owned = g_session.GetItem1720QuantityByItemId(mode->drag.name.c_str());
        for (ITEM_INFO& entry : this->items) {
          if (std::atoi(entry.itemId.c_str()) == std::atoi(mode->drag.name.c_str())) {
            if (entry.quantity <= 0) {
              g_windowMgr.Confirm(LookupMessage(MSI_BARGAINSALE_CLOSE_MSG), 0, true, false, 0, "Message", 280, 120, 0);
              mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
              return 0;
            }
            if (mode->drag.quantity + owned > entry.quantity) {
              ModeDragState saved = mode->drag;
              g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_BARGAINSALE_TOOLWND_CALL), entry.quantity).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
              mode->drag = saved;
              mode->drag.quantity = entry.quantity - owned;
              break;
            }
          }
        }
      }
      ITEM_INFO item;
      item.Assign(reinterpret_cast<const RawItemData&>(mode->drag));
      item.bindOnEquip = static_cast<std::uint16_t>(this->m_selectedIndex);
      g_session.MergeItem1720ByName(item);
      this->m_totalPrice += item.quantity * item.prices[1];
      if (g_session.GetList2Size() >= 4 && g_session.GetList2Size() < 8) {
        int total;
        if (g_session.GetList2Size() == 4) {
          this->m_firstOther = 1;
          this->m_otherScroll->Show(1);
          total = 5;
        } else {
          this->m_firstOther = g_session.GetList2Size() - 3;
          total = g_session.GetList2Size() + 1;
        }
        this->m_otherScroll->SetRange(4, 4, total);
        this->m_otherScroll->SetPosition(this->m_firstOther);
        this->m_otherScroll->Invalidate();
      }
    }

    this->m_otherItems.clear();
    const int count = g_session.GetList2Size();
    for (int index = 0; index < count; ++index) {
      ITEM_INFO other = g_session.GetItem1720At(index);
      if (other.identity[1] != 0) {
        this->m_otherItems.push_back(other);
      }
    }
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }

  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  return 0;
}

// OFFSET: 0x008F5AA0
// FUNCTION: fn_12312
void UInCashWnd::OnWheel(int delta) {
  if (g_session.GetList2Size() < 4) {
    return;
  }
  if (delta < 0) {
    this->SendMsg(this, UM_SCROLLVERTPAGEDOWN, 0, 0, 0, 0);
  } else {
    this->SendMsg(this, UM_SCROLLVERTPAGEUP, 0, 0, 0, 0);
  }
}

// OFFSET: 0x008D7660
// FUNCTION: fn_12121
int UISayDialogWnd::ButtonX(UIBitmapButton* button) {
  int x = this->geometry.width - 4;
  for (auto it = this->m_buttons.begin(); it != this->m_buttons.end(); ++it) {
    UIWindow* other = *it;
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
  switch (this->m_item.identity[0]) {
  case TYPE_ARMOR:
  case TYPE_WEAPON:
  case TYPE_BOW:
  case TYPE_BOTHHAND:
  case TYPE_ARMORTM:
  case TYPE_ARMORTB:
  case TYPE_ARMORMB:
  case TYPE_ARMORTMB:
  case TYPE_GUN:
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
      std::memcpy(&cardId, this->m_item.cards + index * sizeof(cardId), sizeof(cardId));
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
  if (message != UM_COMMAND) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }
  if (a == ID_STATUS) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_STATUSWND);
    if (window->geometry.height == 17) {
      window->SendMsg(nullptr, UM_COMMAND, ID_MINI, 0, 0, 0);
    }
    g_windowMgr.Promote(window);
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
      return g_session.GetCostumeItemAt(slot);
    }
    return g_session.GetAlternateCostumeItemAt(slot);
  }

  if (this->m_page != 0) {
    return ITEM_INFO(0);
  }

  if (this->m_viewOther == 0) {
    return g_session.GetEquippedItemAt(slot);
  }
  return g_session.GetAlternateEquippedItemAt(slot);
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
void UIEquipWnd::OnLBtnDblClk(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, UM_COMMAND, ID_MINI, 0, 0, 0);
  }

  if (this->m_viewOther != 0) {
    return;
  }

  ITEM_INFO item = this->ItemIconAt(x, y);
  if (item.identity[1] != 0 && item.quantity > 0) {
    const EquipSlots slots = g_session.GetEquipSlots(static_cast<int>(item.identity[3]));
    g_modeMgr.gameMode->SendMsg(MM_TAKE_OFF_EQUIP, slots.values[2], this->m_page == 1, 0, 0);
  }
}

// OFFSET: 0x008F3DD0
// FUNCTION: fn_12286
void UIEquipWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->ItemEntryAt(x, y);
  if (item.identity[1] != 0) {
    item.identified = 1;
    int tooltipX = 6;
    int tooltipY = (y - this->m_contentTop - 17) / 24 * 26 - 3 + this->m_contentTop;
    if (x >= 170) {
      tooltipX = 175;
    } else if (x >= 106) {
      tooltipX = this->geometry.width / 2 - 20;
    }
    this->ToScreen(tooltipX, tooltipY);

    CMode* tooltip = g_modeMgr.gameMode;
    if (item.quantity < 2) {
      if (item.identified == 1 && item.optionCount != 0) {
        tooltip->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX, tooltipY,
                      g_session.GetItemNameColor(item), 0, 1);
      } else {
        tooltip->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
      }
    } else if (item.identified == 1 && item.optionCount != 0) {
      tooltip->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_GATLING), item.DisplayName(0).c_str(), item.optionCount, item.quantity).c_str(), tooltipX,
                    tooltipY, g_session.GetItemNameColor(item), 0, 1);
    } else {
      tooltip->ShowTooltip(FormatOwnedString(LookupMessage(MSI_EA2), item.DisplayName(0).c_str(), item.quantity).c_str(), tooltipX, tooltipY,
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
    if (dragged.identity[1] != 0 && dragged.identity[2] != 0) {
      const EquipSlots slots = g_session.GetEquipSlots(static_cast<int>(dragged.identity[2]));
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
void UIEquipWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->ItemEntryAt(x, y);
  if (item.identity[1] != 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
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
void UIExchangeWnd::OnMouseHover(int x, int y) {
  int tooltipY = y;
  ITEM_INFO item = this->ItemAt(x, y);
  this->m_hoverX = static_cast<short>(x);
  if (item.identity[1] == 0) {
    return;
  }

  int tooltipX = x >= 280 ? 321 : 42;
  tooltipY = (tooltipY - 17) / this->m_rowHeight * this->m_rowHeight + 4;
  this->ToScreen(tooltipX, tooltipY);

  CMode* tooltip = g_modeMgr.gameMode;
  item.identity[0] = 21;
  if (item.identified == 1 && item.optionCount != 0) {
    tooltip->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX, tooltipY,
                  g_session.GetItemNameColor(item), 0, 1);
  } else {
    tooltip->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x008F54C0
// FUNCTION: fn_12303
void UIExchangeWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.identity[1] != 0) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
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
void UIWhisperWnd::StoreInfo() {
  if (this->placement) {
    this->placement->width = this->geometry.width;
    this->placement->height = this->geometry.height;
    this->placement->x = this->geometry.x;
    this->placement->y = this->geometry.y;
  }
  g_windowMgr.UnregisterNamedWindow(std::string(this->m_target.c_str(), this->m_target.size()));
}


// OFFSET: 0x008D88F0
// FUNCTION: fn_12130
ITEM_INFO UInCashWnd::ItemAt(int x, int y) {
  if (static_cast<unsigned>(x - 64) > 556 || y < 50 || y > this->m_listHeight + 440) {
    return ITEM_INFO(0);
  }

  const CSession& lists = g_session;
  int top = this->m_listTop;
  if (x < 252) {
    std::list<ITEM_INFO>::iterator it = this->items.begin();
    for (int row = 0; row < 4; ++row) {
      if (y >= top && y <= top + this->m_rowHeight) {
        int index = row + (this->m_page - 1) * 8;
        int count = lists.GetInventoryCategorySize(this->m_tab);
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
        int count = lists.GetInventoryCategorySize(this->m_tab);
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
void UInCashWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }

  ITEM_INFO item = this->ItemAt(x, y);
  if (item.identity[1] != 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    int source;
    if (this->m_selected < 0) {
      this->m_selected = ~this->m_selected;
      source = 4;
    } else {
      this->m_selectedIndex = item.bindOnEquip;
      source = 6;
    }
    mode->drag.BeginItem(g_windowMgr, source, item, 1, -1);
  }
}

// OFFSET: 0x008F58C0
// FUNCTION: fn_12306
void UInCashWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.identity[1] != 0 && this->m_selected >= 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x008C6E20
// FUNCTION: fn_11923
UIAutoMessageWnd::UIAutoMessageWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C6E80
// FUNCTION: fn_11924
UIBabyAcceptWnd2::UIBabyAcceptWnd2()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008C6F10
// FUNCTION: fn_11925
UIBabyAcceptWnd::UIBabyAcceptWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008C7630
// FUNCTION: fn_11935
UIComboBoxIDWnd::UIComboBoxIDWnd()
    : UIFrameWnd(0), m_list(nullptr), m_owner(nullptr) {
}

// OFFSET: 0x008C76A0
// FUNCTION: fn_11936
UIComboBoxWnd::UIComboBoxWnd()
    : UIFrameWnd(0), m_list(nullptr), m_owner(nullptr) {
}

// OFFSET: 0x008C77A0
// FUNCTION: fn_11938
UIDisconnectedServerMsgWnd::UIDisconnectedServerMsgWnd()
    : UIFrameWnd(0), m_viewer(nullptr) {
}

// OFFSET: 0x008C82D0
// FUNCTION: fn_11951
UILoadingWnd::UILoadingWnd()
    : UIFrameWnd(0), m_progressBar(nullptr) {
}

// OFFSET: 0x008C8650
// FUNCTION: fn_11957
UINoticeConfirmWnd::UINoticeConfirmWnd()
    : UIFrameWnd(0), m_event(28) {
}

// OFFSET: 0x008C8770
// FUNCTION: fn_11959
UINotifyLevelUpWnd::UINotifyLevelUpWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C87D0
// FUNCTION: fn_11960
UINotifyQuestWnd::UINotifyQuestWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C8830
// FUNCTION: fn_11961
UINpcEditDialogWnd::UINpcEditDialogWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C8890
// FUNCTION: fn_11962
UINpcTextEditDialogWnd::UINpcTextEditDialogWnd()
    : UINpcEditDialogWnd(), m_tagCtrl(nullptr) {
}

// OFFSET: 0x008C8B50
// FUNCTION: fn_11968
UIProhibitListWnd::UIProhibitListWnd()
    : UIFrameWnd(0), m_count(0) {
}

// OFFSET: 0x008C8BB0
// FUNCTION: fn_11969
UIQuitWnd::UIQuitWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C8C10
// FUNCTION: fn_11970
UIRestartWnd::UIRestartWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C93D0
// FUNCTION: fn_11980
UITipOfTheDayWnd::UITipOfTheDayWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C9560
// FUNCTION: fn_11982
UIToolTipWnd::UIToolTipWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C95C0
// FUNCTION: fn_11983
UIWaitWnd::UIWaitWnd()
    : UIFrameWnd(0), m_button(nullptr) {
}

// OFFSET: 0x008C9990
// FUNCTION: fn_11987
UInCash_CallWnd::UInCash_CallWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C8F00
// FUNCTION: fn_11973
UISelectCartWnd::UISelectCartWnd()
    : UIFrameWnd(0), selectionContext(0), cartTypesBegin(0), cartTypesEnd(0), cartTypesCapacity(0) {
}

// OFFSET: 0x008C9280
// FUNCTION: fn_11979
UIStorePasswordInputWnd::UIStorePasswordInputWnd()
    : UIFrameWnd(0), padding_00BC(-1), padding_00C0(2) {
  std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
  std::memset(this->newPassword, 0, sizeof(this->newPassword));
  std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
  this->maskTextX = 0;
  this->isChangingPassword = 0;
  this->passwordStage = 0;
  this->inputDigitCount = 0;
  this->activeInputY = 40;
  this->currentPasswordY = 40;
  this->newPasswordY = 80;
  this->confirmPasswordY = 119;
  if (g_session.m_passwordInputAlternate) {
    this->passwordStage = 2;
    this->activeInputY = 80;
    this->inputDigitCount = 0;
    this->maskTextX = 0;
    std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
    std::memset(this->newPassword, 0, sizeof(this->newPassword));
    std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
  }
}

// OFFSET: 0x008C8D40
// FUNCTION: fn_11972
UISecondPasswordInputWnd::UISecondPasswordInputWnd()
    : UIStorePasswordInputWnd(), passwordLength(4), inputMode(0), padding_0120(0), keypadButtons(), instructionText(nullptr) {
  this->actionButtons[0] = nullptr;
  this->actionButtons[1] = nullptr;
  this->actionButtons[2] = nullptr;
}

// OFFSET: 0x008C8F80
// FUNCTION: fn_11974
UISkillDescribeWnd::UISkillDescribeWnd()
    : UIFrameWnd(0), m_text(nullptr) {
}

// OFFSET: 0x008C9430
// FUNCTION: fn_11981
UITipboxWnd::UITipboxWnd()
    : UIFrameWnd(0), searchEdit(0), searchResults(0),
      tipViewer(0), m_pageText(nullptr), m_pageTotalText(nullptr), previousButton(0), nextButton(0), tagButton(0), m_buildTime(0), m_frameTime(0),
      refreshPending(false), showAtStartup(0) {
  this->titleText = 0;
  this->subtitleText = 0;
}

// OFFSET: 0x008C86B0
// FUNCTION: fn_11958
UINotifyItemObtainWnd::UINotifyItemObtainWnd()
    : UIFrameWnd(0), m_identified(false) {
}

// OFFSET: 0x008C7CB0
// FUNCTION: fn_11943
UIItemCollectionWnd::UIItemCollectionWnd()
    : UIFrameWnd(0), m_helpButton(nullptr) {
  this->m_placement = nullptr;
  this->m_item.Reset();
  this->m_petRenamed = 0;
  this->m_petLevel = -1;
  this->m_petHunger = -1;
  this->m_petFriendly = -1;
  this->m_petAccessory = 0xFFFF;
  this->m_previewTick = 0;
  this->m_closeButton = nullptr;
  this->m_compare = 0;
  this->m_optionCount = 0;
  this->m_moveInfo = nullptr;
  this->m_withdrawButton = 0;
  this->m_extraHeight = 0;
}

// OFFSET: 0x008C7570
// FUNCTION: fn_11933
UICombinedCardItemCollectionWnd::UICombinedCardItemCollectionWnd()
    : UIItemCollectionWnd(), m_cardPlacement(nullptr) {
}

// OFFSET: 0x008C75D0
// FUNCTION: fn_11934
UICombinedCardItemCollection_ComparisonWnd::UICombinedCardItemCollection_ComparisonWnd()
    : UIItemCollectionWnd() {
}

// OFFSET: 0x008C7E40
// FUNCTION: fn_11944
UIItemCollection_ComparisonWnd::UIItemCollection_ComparisonWnd()
    : UIItemCollectionWnd() {
}

// OFFSET: 0x008C7070
// FUNCTION: fn_11927
UICardItemIllustWnd::UICardItemIllustWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C8390
// FUNCTION: fn_11953
UIMenuWnd::UIMenuWnd()
    : UIFrameWnd(0), m_menuId(10041), m_selected(-1), m_itemHeight(14), m_targetWindowId(0), m_menuFlags(0) {
}

// OFFSET: 0x008C85A0
// FUNCTION: fn_11956
UIMonsterDisplayWnd::UIMonsterDisplayWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C8900
// FUNCTION: fn_11963
UIPartyInvitationToWnd::UIPartyInvitationToWnd()
    : UIPartySettingWnd() {
}

// OFFSET: 0x008E60A0
// FUNCTION: fn_12191
void UIPartySettingWnd::OnCreate(int width, int height) {
  this->m_expShare = g_session.partyExpShare == 2 ? 0 : g_session.partyExpShare;
  this->m_itemShare = g_session.partyItemCollect;
  this->m_itemDistribute = 0;
  g_session.partyItemDistribute = 0;

  if (height == 245) {
    this->m_mode = 70;
  } else if (height == 180) {
    this->m_mode = 5;
  } else if (height == 220) {
    this->m_mode = 50;
  }

  if (this->m_mode == 70) {
    const char* texts[2] = {LookupMessage(MSI_EXPDIV1), LookupMessage(MSI_EXPDIV2)};
    for (int i = 0, y = 42; y < 82; ++i, y += 20) {
      this->m_expShareButtons[i] = new UIRadioBtn();
      this->m_expShareButtons[i]->SetSize(160, g_serviceType == 12 ? 17 : 12);
      this->m_expShareButtons[i]->Move(20, y);
      this->m_expShareButtons[i]->SetText(texts[i]);
      this->m_expShareButtons[i]->SendMsg(this, UM_SETSTATE, this->m_expShare == i, 0, 0, 0);
      this->AddChild(this->m_expShareButtons[i]);
      if (g_session.partyLeader == 0) {
        this->m_expShareButtons[i]->enabled = 0;
      }
    }
  } else if (this->m_mode == 50) {
    this->m_nameEdit = new UIEditCtrl();
    this->m_nameEdit->SetSize(100, 20);
    this->m_nameEdit->Move(20, 38);
    this->m_nameEdit->maximum = 23;
    this->m_nameEdit->Background(230, 230, 230);
    this->AddChild(this->m_nameEdit);
  }

  {
    const char* texts[2] = {LookupMessage(MSI_ITEMCOLLECT1), LookupMessage(MSI_ITEMCOLLECT2)};
    for (int i = 0, offset = 0; offset < 40; ++i, offset += 20) {
      this->m_itemShareButtons[i] = new UIRadioBtn();
      this->m_itemShareButtons[i]->SetSize(160, g_serviceType == 12 ? 17 : 12);
      this->m_itemShareButtons[i]->SetText(texts[i]);
      this->m_itemShareButtons[i]->Move(20, this->m_mode + offset + 37);
      this->m_itemShareButtons[i]->SendMsg(this, UM_SETSTATE, this->m_itemShare == i, 0, 0, 0);
      this->m_itemShareButtons[i]->enabled = g_session.PartyCount() == 0;
      if (g_session.partyLeader != 0) {
        this->m_itemShareButtons[i]->enabled = 1;
      }
      this->AddChild(this->m_itemShareButtons[i]);
    }
  }

  this->m_itemDistributeButton = new UIRadioBtn();
  this->m_itemDistributeButton->SetSize(160, g_serviceType == 12 ? 17 : 12);
  this->m_itemDistributeButton->Move(20, this->m_mode + 102);
  this->m_itemDistributeButton->SetText(LookupMessage(MSI_ITEMDIV1));
  this->m_itemDistributeButton->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
  this->m_itemDistributeButton->enabled = g_session.PartyCount() == 0;
  if (g_session.partyLeader != 0) {
    this->m_itemDistributeButton->enabled = 1;
  }
  this->AddChild(this->m_itemDistributeButton);

  this->m_message0 = 184;

  {
    const std::string prefix("유저인터페이스\\");
    int positions[2][2] = {{this->geometry.width - 92, this->geometry.height - 24}, {this->geometry.width - 46, this->geometry.height - 24}};
    const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    int ids[2] = {184, 185};

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

  this->AddCloseButton("Alt+P", 0);
  if (this->m_mode == 50) {
    g_windowMgr.SetFocus(this->m_nameEdit);
  }
}

// OFFSET: 0x008C8980
// FUNCTION: fn_11964
UIPartySettingWnd::UIPartySettingWnd()
    : UIFrameWnd(0) {
  this->m_mode = 0;
  this->m_placement = nullptr;
}

// OFFSET: 0x008C8AB0
// FUNCTION: fn_11967
UIPetTamingDeceiveWnd::UIPetTamingDeceiveWnd()
    : UIFrameWnd(0), m_delay(1), m_state(0), m_tick(0), m_frame(-1), m_maxDelay(10000), m_started(0) {
  this->m_finishTime = 0xFFFFFFFF;
}

// OFFSET: 0x008C9060
// FUNCTION: fn_11975
UISkillNameChangeWnd::UISkillNameChangeWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008C7710
// FUNCTION: fn_11937
UICoupleAcceptWnd::UICoupleAcceptWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008C8120
// FUNCTION: fn_11948
UIJoinFriendAcceptWnd::UIJoinFriendAcceptWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008C81B0
// FUNCTION: fn_11949
UIJoinGuildAcceptWnd::UIJoinGuildAcceptWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008FE310
// FUNCTION: fn_12352
int UIJoinGuildAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_JOINGUILDACCEPTWND);
      g_windowMgr.CloseWindow(WID_GUILDTIPWND);
      g_windowMgr.CloseWindow(WID_CREATEGUILDWND_MODE_0);
      g_windowMgr.CloseWindow(WID_GUILDHELPERWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_GUILD, this->identity, 1, 0, 0);
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_JOINGUILDACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_GUILD, this->identity, 0, 0, 0);
      break;
    }
    break;
  case UM_SET_JOIN_GUILD_MSG: {
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
    text += LookupMessage(MSI_SUGGEST_JOIN_GUILD);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    break;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, bytes, sizeof(header));
    unsigned offset = 6;
    if (header.type == 0x5528) {
      while (static_cast<unsigned>(b) > offset) {
        std::memcpy(&header, bytes + offset, sizeof(header));
        offset += 6;
        switch (header.type) {
        case 0x5529:
          return 0;
        case 0x552A: {
          char name[260] = {};
          std::memcpy(name, bytes + offset, header.length);
          offset += header.length;
          this->SendMsg(nullptr, UM_SET_JOIN_GUILD_MSG, 0, reinterpret_cast<std::intptr_t>(name), 0, 0);
          break;
        }
        default:
          offset += header.length;
          break;
        }
      }
    }
    break;
  }
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008C8510
// FUNCTION: fn_11955
UIMixAcceptWnd::UIMixAcceptWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008C9160
// FUNCTION: fn_11977
UIStarPlaceAcceptWnd::UIStarPlaceAcceptWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008C97F0
// FUNCTION: fn_11985
UIYourItemWnd::UIYourItemWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008C8C70
// FUNCTION: fn_11971
UISayDialogWnd::UISayDialogWnd()
    : UIFrameWnd(0) {
  this->m_viewer = nullptr;
  this->m_placement = nullptr;
}

// OFFSET: 0x008E3CD0
// FUNCTION: fn_12184
void UINoticeConfirmWnd::OnCreate(int width, int height) {
  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{189, 96}, {234, 96}};

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

  this->m_message0 = 184;
  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DO_YOU_AGREE)), 38, 0, 0);
}

// OFFSET: 0x008E4390
// FUNCTION: fn_12186
void UINotifyLevelUpWnd::OnCreate(int, int) {
  int ids[3] = {192};
  int positions[3] = {0};
  const char* names[8] = {"lv_up"};
  const std::string prefix("유저인터페이스\\basic_interface\\");

  for (int i = 0; i < 1; ++i) {
    auto* button = new UIBitmapButton();
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
void UINotifyQuestWnd::OnCreate(int, int) {
  int ids[3] = {192};
  int positions[3] = {0};
  const char* names[8] = {"quest"};
  const std::string prefix("유저인터페이스\\basic_interface\\");

  for (int i = 0; i < 1; ++i) {
    auto* button = new UIBitmapButton();
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
void UINpcEditDialogWnd::OnCreate(int, int) {
  this->edit = new UIEditCtrl();
  this->edit->SetSize(this->geometry.width - 77, 16);
  this->edit->Move(15, 22);
  this->edit->maximum = 12;
  this->edit->Background(232, 232, 232);
  this->edit->SetText("0");
  this->AddChild(this->edit);
  g_windowMgr.SetFocus(this->edit);
  this->edit->SelectAll();

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton();
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->npcId = -1;
  this->m_message0 = 184;
}

// OFFSET: 0x008E54B0
// FUNCTION: fn_12189
void UINpcTextEditDialogWnd::OnCreate(int, int) {
  this->edit = new UIEditCtrl();
  this->edit->SetSize(this->geometry.width - 77, 16);
  this->edit->Move(15, 22);
  this->edit->maximum = 70;
  this->edit->Background(232, 232, 232);
  this->edit->SetText("");
  this->AddChild(this->edit);
  this->edit->SetId(492);
  g_windowMgr.SetFocus(this->edit);
  this->edit->SelectAll();

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton();
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->npcId = -1;
  this->m_message0 = 184;
}
// OFFSET: 0x008E7660
// FUNCTION: fn_12195
void UIQuitWnd::OnCreate(int, int) {
  const std::string prefix("유저인터페이스\\");
  int positions[3][3] = {{29, 30, 0}, {29, 61, 0}, {29, 96, 0}};
  int ids[3] = {220, 221, 185};
  const char* names[3][3] = {{"esc_01a", "esc_01b", "esc_01c"}, {"esc_03a", "esc_03b", "esc_03c"}, {"esc_02a", "esc_02b", "esc_02c"}};

  for (int i = 0; i < 3; ++i) {
    auto* button = new UIBitmapButton();
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
void UIRestartWnd::OnCreate(int, int) {
  const std::string prefix("유저인터페이스\\");
  int ids[4] = {220, 252, 251, 185};
  const char* names[4][4] = {
      {"esc_04a", "esc_04b", "esc_04c", nullptr}, {"esc_01a", "esc_01b", "esc_01c", nullptr}, {"esc_03a", "esc_03b", "esc_03c", nullptr}, {"esc_02a", "esc_02b", "esc_02c", nullptr}};
  int positions[4][4] = {{29, 22, 0, 0}, {29, 45, 0, 0}, {29, 68, 0, 0}, {29, 96, 0, 0}};

  for (int i = 0; i < 4; ++i) {
    auto* button = new UIBitmapButton();
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
void UIWaitWnd::OnCreate(int width, int height) {
  this->m_button = new UIBitmapButton();
  this->m_button->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  this->m_button->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  this->m_button->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  this->m_button->SetSize(this->m_button->BitmapWidth(), this->m_button->BitmapHeight());
  this->m_button->Move((width - this->m_button->BitmapWidth()) / 2, 50);
  this->m_button->SetId(185);
  this->m_button->SetTooltip("");
  this->AddChild(this->m_button);

  this->m_textViewer = new UITextViewer();
  this->m_textViewer->Configure(10, 22, width - 20, height - 55, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_textViewer->textColor = 0xFF000000u | RGB(red, green, blue);
  this->m_textViewer->Invalidate();
  this->AddChild(this->m_textViewer);
  this->m_textViewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PLEASE_BE_PATIENT)), 38, 0, 0);
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
void UIPartyInvitationToWnd::OnCreate(int, int) {
  this->m_edit = new UIEditCtrl();
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

  g_windowMgr.SetFocus(this->m_edit);
}

// OFFSET: 0x008F95F0
// FUNCTION: fn_12339
int UICoupleAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_COUPLEACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_COUPLE, this->identity, 1, 0, 0);
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_COUPLEACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_COUPLE, this->identity, 0, 0, 0);
      break;
    }
    break;
  case UM_SET_COUPLE_MSG: {
    this->identity = static_cast<std::uint32_t>(a);
    std::string text = "(";
    text += reinterpret_cast<const char*>(b);
    text += ")";
    text += LookupMessage(MSI_SUGGEST_COUPLE);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    break;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008FDE10
// FUNCTION: fn_12350
int UIItemParamChangeDisplayWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO: {
    this->m_placement = reinterpret_cast<WindowPlacement*>(a);
    auto* info = static_cast<ItemParamChangeDisplayWndInfo*>(this->m_placement);
    if (!info->IsValid()) {
      info->Reset();
    }
    this->Move(this->m_placement->x, this->m_placement->y);
    return 0;
  }
  case UM_REFRESH: {
    this->SumEquippedStats();
    this->equippedParam = 0;
    this->equippedSecondParam = 0;
    const ITEM_DESC& description = FindItemDescription(std::atoi(this->m_label.c_str()));
    ITEM_INFO equipped = this->FindEquippedItem(description.descriptionFlags);
    if (equipped.identity[1] != 0) {
      const ITEM_DESC& equippedDescription = FindItemDescription(std::atoi(equipped.itemId.c_str()));
      if (equippedDescription.descriptionValues.size() == 2) {
        this->equippedParam = equippedDescription.descriptionValues[0];
        this->equippedSecondParam = equippedDescription.descriptionValues[1];
      }
    }
    break;
  }
  case UM_PARAM: {
    this->SumEquippedStats();
    this->m_label = reinterpret_cast<const char*>(a);
    this->itemIndex = static_cast<int>(b);
    const ITEM_DESC& description = FindItemDescription(std::atoi(this->m_label.c_str()));
    if (description.descriptionValues.size() == 2) {
      this->itemParam = description.descriptionValues[0];
      this->itemSecondParam = description.descriptionValues[1];
    }
    this->equippedParam = 0;
    this->equippedSecondParam = 0;
    const ITEM_DESC& slot = FindItemDescription(std::atoi(this->m_label.c_str()));
    ITEM_INFO equipped = this->FindEquippedItem(slot.descriptionFlags);
    if (equipped.identity[1] != 0) {
      const ITEM_DESC& equippedDescription = FindItemDescription(std::atoi(equipped.itemId.c_str()));
      if (equippedDescription.descriptionValues.size() == 2) {
        this->equippedParam = equippedDescription.descriptionValues[0];
        this->equippedSecondParam = equippedDescription.descriptionValues[1];
      }
    }
    break;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->m_after = this->itemParam + this->m_before - this->equippedParam;
  this->m_afterSecond = this->itemSecondParam + this->m_beforeSecond - this->equippedSecondParam;
  this->Invalidate();
  return 0;
}

// OFFSET: 0x008FE0E0
// FUNCTION: fn_12351
int UIJoinFriendAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_JOINFRIENDACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_FRIEND, this->m_accountId, this->m_characterId, 1, 0);
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_JOINFRIENDACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_JOIN_FRIEND, this->m_accountId, this->m_characterId, 0, 0);
      break;
    }
    break;
  case UM_SET_JOIN_FRIEND_MSG: {
    this->m_accountId = static_cast<int>(a);
    this->m_characterId = static_cast<int>(b);
    char text[128];
    std::sprintf(text, LookupMessage(MSI_SUGGEST_JOIN_FRIEND), reinterpret_cast<const char*>(c));
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text), 38, 0, 0);
    break;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* packet = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, packet, sizeof(header));
    unsigned offset = 6;
    if (header.type == 0x5460) {
      while (offset < static_cast<unsigned>(b)) {
        std::memcpy(&header, packet + offset, sizeof(header));
        offset += 6;
        switch (header.type) {
        case 0x5461:
          return 0;
        case 0x5462: {
          char name[260];
          std::memset(name, 0, sizeof(name));
          std::memcpy(name, packet + offset, header.length);
          offset += header.length;
          this->SendMsg(nullptr, UM_SET_JOIN_FRIEND_MSG, 0, 0, reinterpret_cast<std::intptr_t>(name), 0);
          break;
        }
        default:
          offset += header.length;
          break;
        }
      }
    }
    break;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008FF640
// FUNCTION: fn_12358
int UIMixAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND2);
      g_windowMgr.CloseWindow(WID_ITEMSELLWND2);
      g_modeMgr.gameMode->SendMsg(MM_ITEMLISTWIN_RES, 0, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_MIXACCEPTWND);
      g_session.ClearTradeItems();
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_MIXACCEPTWND);
      break;
    }
    break;
  case UM_SET_BABY_MSG: {
    this->m_kind = static_cast<unsigned char>(a);
    std::string text;
    text = LookupMessage(MSI_MIX_ACCEPT);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    break;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008FF910
// FUNCTION: fn_12361
int UINotifyItemObtainWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->m_item = *reinterpret_cast<const ITEM_INFO*>(a);
    return 0;
  case UM_SETTITLE: {
    this->m_kind = static_cast<int>(a);
    char text[128];
    if (a != 0) {
      char grouped[24];
      std::sprintf(text, LookupMessage(MSI_GET_ZENY), FormatGroupedInteger(static_cast<int>(b), grouped, sizeof(grouped)));
      this->m_message = text;
      this->m_itemId = "";
    } else {
      this->m_itemId = reinterpret_cast<const char*>(c);
      ITEM_INFO item;
      item.itemId = reinterpret_cast<const char*>(c);
      item.identified = this->m_identified;
      item.refiningLevel = 0;
      std::sprintf(text, LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), static_cast<int>(b));
      this->m_message = text;
    }

    int width = this->TextWidth(this->m_message.c_str(), 0, 0, 12, 0, 0) + 54;
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
  case UM_SET_IDENTIFIY_INFO:
    this->m_identified = a != 0;
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00904960
// FUNCTION: fn_12379
int UISkillDescribeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_SKILLDESCRIBEWND);
      break;
    case ID_HELP: {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_SKILLDELAYINFO);
      if (window) {
        static_cast<CUISkillDelayInfo*>(window)->SetSkill(this->m_skillId);
        window->PlaceBeside(this);
        g_windowMgr.Promote(window);
      }
      break;
    }
    }
    break;
  case UM_SKILLDESCRIBE: {
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
    if (g_windowMgr.LookupWindow(WID_SKILLDELAYINFO)) {
      g_windowMgr.CloseWindow(WID_SKILLDELAYINFO);
    }

    this->descriptionText = this->skillName = skill.DisplayName(*g_session.m_hotkeyScript);
    this->m_skillId = static_cast<int>(skill.id);
    {
      CSkillInfo owned = g_session.LookupSkill(static_cast<std::uint32_t>(a));
      if (owned.available != 0 && owned.spCost != 0 && owned.level != 0) {
        char cost[64];
        if (skill.UseApAmount(*g_session.m_hotkeyScript, owned.level) != 0) {
          std::sprintf(cost, " (Sp : %d, Ap : %d)", owned.spCost, owned.apCost);
        } else {
          std::sprintf(cost, " (Sp : %d)", owned.spCost);
        }
        this->descriptionText.append(cost, std::strlen(cost));
      }
      bool passive = false;
      g_session.m_hotkeyScript->Call("IsPassiveSkill", "d>b", static_cast<int>(a), &passive);
      this->m_delayButton->Enable(!passive);
      this->m_text->Clear();
    }

    this->iconName = "유저인터페이스\\item\\" + std::string(skill.IdName(*g_session.m_hotkeyScript)) + ".bmp";
    this->m_viewer->Clear();

    const char* description = nullptr;
    if (g_session.m_hotkeyScript->Call("GetSkillDescript", "ddd>s", g_session.m_job, static_cast<int>(a), b == 0 && c == 0 && d == 1, &description)) {
      const std::string text = description;
      if (!text.empty()) {
        if (g_session.FindShopId("") != -1) {
          const int delay = g_session.GetShopEntry(1, this->m_skillId).first;
          if (delay != 0) {
            const std::string line = FormatOwnedString(LookupMessage(MSI_FAIL_CALLHOMUN_OVER_100LEVEL), static_cast<int>(delay * 0.1), delay % 10);
            this->m_text->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(line.c_str()), 0, 0, 1);
          }
        }
        const std::vector<std::string> lines = Split(text, "\r\n");
        std::vector<char> buffer;
        for (std::size_t index = 0; index < lines.size(); ++index) {
          buffer.resize(lines[index].size() + 4);
          StringFormat(buffer.data(), buffer.size(), static_cast<std::size_t>(-1), "%s%s", g_serverLanguage->Prefix(true), lines[index].c_str());
          UIWindow* target = static_cast<int>(this->m_text->lines.size()) < 2 ? static_cast<UIWindow*>(this->m_text) : static_cast<UIWindow*>(this->m_viewer);
          target->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(buffer.data()), 0, 1, 1);
        }
      }
    } else {
      this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>("Unknown Skill"), 33, 0, 0);
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
    g_windowMgr.Promote(this);
    break;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x00905210
// FUNCTION: fn_12380
int UISkillNameChangeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    if (a == ID_OK) {
      if (this->m_skillId != -1 && this->m_edit->Length() != 0) {
        g_modeMgr.gameMode->SendMsg(98, this->m_skillId, reinterpret_cast<std::intptr_t>(this->m_edit->Text()), 0, 0);
      }
      g_windowMgr.CloseWindow(WID_SKILLNAMECHANGEWND);
    }
    break;
  case UM_SET_SKILL_INFO:
    this->m_skillId = static_cast<int>(a);
    this->m_name = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, static_cast<int>(a));
    this->m_edit->SetText(this->m_name.c_str());
    g_windowMgr.SetFocus(this->m_edit);
    this->m_edit->SelectAll();
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x00905590
// FUNCTION: fn_12382
int UIStarPlaceAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    if (a == ID_OK) {
      CMode* mode = g_modeMgr.GetGameMode();
      g_windowMgr.CloseWindow(WID_STARPLACEACCEPTWND);
      mode->SendMsg(MM_STARPLACE_ACCEPT, this->m_place, 0, 0, 0);
    } else if (a == ID_CANCEL) {
      g_windowMgr.CloseWindow(WID_STARPLACEACCEPTWND);
    }
    return 0;
  }
  case UM_SET_BABY_MSG: {
    this->m_place = static_cast<char>(a);
    std::string text;
    if (this->m_place == 10) {
      text = LookupMessage(MSI_DA_EXP_ACCEPT);
    } else {
      text = LookupMessage(MSI_STARPLACE_ACCEPT);
    }
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }
  }

  if (message != UM_REPLAY_FRAMED_DATA) {
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
        this->SendMsg(nullptr, UM_SET_BABY_MSG, this->m_place, 0, 0, 0);
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
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      g_modeMgr.GetGameMode();
      WearEquipPacket packet;
      packet.packetType = ID_CZ_REQ_WEAR_EQUIP_V5;
      packet.index = static_cast<std::uint16_t>(this->m_index);
      packet.location = this->m_location;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      g_windowMgr.CloseWindow(WID_YOURITEMWND);
      break;
    }
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_YOURITEMWND);
      break;
    }
    break;
  case UM_SET_BABY_MSG: {
    std::string text;
    text = LookupMessage(MSI_YOURITEM_EQUIP);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    this->m_index = static_cast<int>(a);
    this->m_location = static_cast<int>(b);
    break;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008E6B40
// FUNCTION: fn_12192
void UIPasswordWnd::OnCreate(int, int) {
  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(90, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 8;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->mask = 42;
  this->m_edit->SetText("");
  this->AddChild(this->m_edit);
  g_windowMgr.SetFocus(this->m_edit);

  auto* title = new UIStaticText();
  this->itemTitle = reinterpret_cast<std::uintptr_t>(title);
  title->SetSize(this->geometry.width - 30, 15);
  title->Move(15, 6);
  title->SetText(LookupMessage(MSI_ENTER_PASSWORD), 0);
  this->AddChild(title);

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton();
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;
  this->geometry.state = 0;
  this->roomId = 0;
}
// OFFSET: 0x008C91F0
// FUNCTION: fn_11978
UIStatusWnd::UIStatusWnd()
    : UIFrameWnd(0) {
  this->geometry.state = 0;
  this->m_info = nullptr;
  this->m_titleHeight = 17;
  this->m_offsetY = 11;
  this->m_offsetX = 16;
}

// OFFSET: 0x00906DE0
// FUNCTION: fn_12385
int UITipOfTheDayWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_TIPOFTHEDAYWND);
      break;
    case ID_CHECK: {
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
    case ID_NEXT:
      if (g_tipOfTheDay.Size() != 0) {
        ++this->m_tipIndex;
        if (this->m_tipIndex >= g_tipOfTheDay.Size()) {
          this->m_tipIndex = 0;
        }
        this->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(&g_tipOfTheDay.GetTip(this->m_tipIndex)), 0, 0, 0);
      }
      break;
    case ID_PREV:
      if (g_tipOfTheDay.Size() != 0) {
        if (--this->m_tipIndex < 0) {
          this->m_tipIndex = g_tipOfTheDay.Size() - 1;
        }
        this->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(&g_tipOfTheDay.GetTip(this->m_tipIndex)), 0, 0, 0);
      }
      break;
    }
    return 0;

  case UM_SETTEXT: {
    const auto& tip = *reinterpret_cast<const StringList*>(a);
    this->m_textViewer->Clear();
    this->m_textViewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DIDYOUKNOW)), 38, 0, 0);
    for (unsigned i = 0; i < tip.size(); ++i) {
      this->m_textViewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(tip[i].c_str()), 38, 0, 0);
    }
    return 0;
  }

  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// INLINED
static __forceinline void RequestStatusPointUp(UIStatusWnd* window, CMode* mode, const int* statCosts, int statusPoint, int index) {
  if (statCosts[index] <= statusPoint) {
    mode->SendMsg(MM_STATUS_CHANGE, 13 + index, 1, 0, 0);
    window->SendMsg(window, UM_REFRESHBUTTON, 0, 0, 0, 0);
  }
}

// INLINED
static __forceinline void RequestTraitPointUp(CMode* mode, int index) {
  if (g_session.GetTraitStat(231) > 0 && g_session.m_traits.points[index] > 0) {
    mode->SendMsg(MM_ADVANCED_STATUS_CHANGE, 219 + index, 1, 0, 0);
  }
}

// OFFSET: 0x00905830
// FUNCTION: fn_12383
int UIStatusWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  CMode* mode = g_modeMgr.gameMode;
  const int* statCosts = reinterpret_cast<const int*>(g_session.combatStats);
  const int statusPoint = *reinterpret_cast<const int*>(g_session.statusPoints);
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_STATUSWND);
      break;
    case ID_MINI: {
      const int height = this->geometry.height;
      if (height == 17) {
        this->Resize(this->geometry.width, this->m_expandedHeight);
      } else {
        this->m_expandedHeight = height;
        this->Resize(this->geometry.width, 17);
      }
      g_windowMgr.StoreSnapRectangle(this);
      g_windowMgr.Promote(this);
      const int delta = this->geometry.height - height;
      if (delta != 0) {
        g_snapMgr.MoveEdge(this, 0, delta, 3);
      }
      break;
    }
    case ID_STR:
      RequestStatusPointUp(this, mode, statCosts, statusPoint, 0);
      break;
    case ID_AGI:
      RequestStatusPointUp(this, mode, statCosts, statusPoint, 1);
      break;
    case ID_VIT:
      RequestStatusPointUp(this, mode, statCosts, statusPoint, 2);
      break;
    case ID_INT:
      RequestStatusPointUp(this, mode, statCosts, statusPoint, 3);
      break;
    case ID_DEX:
      RequestStatusPointUp(this, mode, statCosts, statusPoint, 4);
      break;
    case ID_LUK:
      RequestStatusPointUp(this, mode, statCosts, statusPoint, 5);
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
      RequestTraitPointUp(mode, 0);
      break;
    case 552:
      RequestTraitPointUp(mode, 1);
      break;
    case 553:
      RequestTraitPointUp(mode, 2);
      break;
    case 554:
      RequestTraitPointUp(mode, 3);
      break;
    case 555:
      RequestTraitPointUp(mode, 4);
      break;
    case 556:
      RequestTraitPointUp(mode, 5);
      break;
    default:
      return 0;
    }
    break;
  case UM_SETINFO:
    this->m_info = reinterpret_cast<StatusWndInfo*>(a);
    if (!this->m_info->IsValid()) {
      this->m_info->Reset();
    }
    this->SendMsg(nullptr, UM_RESIZE, 7, this->m_info->width, this->m_info->expandedHeight, 0);
    if (this->m_info->height != this->m_info->expandedHeight) {
      this->SendMsg(nullptr, UM_COMMAND, ID_MINI, 0, 0, 0);
    }
    this->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    this->Move(this->m_info->x, this->m_info->y);
    this->m_extended = this->m_info->extended != 1;
    this->SendMsg(nullptr, UM_COMMAND, 550, 0, 0, 0);
    break;
  case UM_REFRESHBUTTON:
    if (this->geometry.state == 0) {
      int costs[6];
      std::memcpy(costs, statCosts, sizeof(costs));
      int y = 24;
      for (int i = 0; i < 6; ++i, y += 16) {
        if (costs[i] != 0 && statusPoint >= costs[i]) {
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
        std::memcpy(traitCosts, g_session.m_traits.points, sizeof(traitCosts));
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
  CMode* mode = g_modeMgr.gameMode;

  if (message == UM_COMMAND) {
    switch (a) {
    case ID_CANCEL:
      g_windowMgr.CloseWindow(115);
      if (mode->cursorType == 9) {
        g_windowMgr.EnsureWindow(WID_NEWSELECTCHARWND);
      }
      break;

    case ID_OK:
      switch (this->passwordStage) {
      case 0:
        if (std::strlen(this->currentPassword) < 9 && std::strlen(this->currentPassword) > 3) {
          int password = std::atoi(this->currentPassword);
          for (unsigned i = 0; i < std::strlen(this->currentPassword); ++i) {
            password += 100000000;
          }
          mode->SendMsg(188, password, g_session.m_storePasswordMode, 0, 0);
          g_windowMgr.CloseWindow(115);
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 0;
        this->activeInputY = this->currentPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->Paint();
        break;

      case 1:
        if (std::strlen(this->currentPassword) < 9 && std::strlen(this->currentPassword) > 3) {
          this->passwordStage = 2;
          this->activeInputY = this->newPasswordY;
          this->inputDigitCount = 0;
          this->maskTextX = 0;
          std::memset(this->newPassword, 0, sizeof(this->newPassword));
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->FillRegion(111, 79, 98, 14, 0xFFFFFF);
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 1;
        this->activeInputY = this->currentPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->Paint();
        break;

      case 2:
        if (std::strlen(this->newPassword) < 9 && std::strlen(this->newPassword) > 3) {
          this->activeInputY = this->confirmPasswordY;
          this->passwordStage = 3;
          this->inputDigitCount = 0;
          this->maskTextX = 0;
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->FillRegion(111, 118, 98, 14, 0xFFFFFF);
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        this->activeInputY = this->newPasswordY;
        this->passwordStage = 2;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->FillRegion(111, 79, 98, 14, 0xFFFFFF);
        break;

      case 3:
        if (std::strlen(this->confirmPassword) < 9 && std::strlen(this->confirmPassword) > 3) {
          if (std::strcmp(this->newPassword, this->confirmPassword) == 0) {
            int password = std::atoi(this->currentPassword);
            int newPassword = std::atoi(this->confirmPassword);
            for (unsigned i = 0; i < std::strlen(this->currentPassword); ++i) {
              password += 100000000;
            }
            for (unsigned i = 0; i < std::strlen(this->confirmPassword); ++i) {
              newPassword += 100000000;
            }
            mode->SendMsg(188, password, g_session.m_storePasswordMode, newPassword, 0);
            g_windowMgr.CloseWindow(115);
          } else {
            g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_MISMATCH), 0, true, false, 0, "Message", 280, 120, 0);
            this->passwordStage = 2;
            this->activeInputY = this->newPasswordY;
            this->inputDigitCount = 0;
            this->maskTextX = 0;
            std::memset(this->newPassword, 0, sizeof(this->newPassword));
            std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
            this->FillRegion(111, 79, 98, 14, 0xFFFFFF);
            this->FillRegion(110, 117, 100, 16, 0xFAE6C8);
          }
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 3;
        this->activeInputY = this->confirmPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->FillRegion(111, 118, 98, 14, 0xFFFFFF);
        break;
      }
      break;

    case ID_CHANGE_STORE_PASSWORD:
      g_session.m_storePasswordMode = 2;
      this->isChangingPassword = 1;
      this->passwordStage = 1;
      this->activeInputY = this->currentPasswordY;
      this->inputDigitCount = 0;
      this->maskTextX = 0;
      std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
      std::memset(this->newPassword, 0, sizeof(this->newPassword));
      std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
      this->Paint();
      break;

    case ID_STORE_PASSWORD_NUM1:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "1");
        break;
      case 2:
        std::strcat(this->newPassword, "1");
        break;
      case 3:
        std::strcat(this->confirmPassword, "1");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_RETURN:
      this->SendMsg(this, UM_COMMAND, ID_OK, 0, 0, 0);
      break;
    case ID_STORE_PASSWORD_NUM0:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "0");
        break;
      case 2:
        std::strcat(this->newPassword, "0");
        break;
      case 3:
        std::strcat(this->confirmPassword, "0");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM2:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "2");
        break;
      case 2:
        std::strcat(this->newPassword, "2");
        break;
      case 3:
        std::strcat(this->confirmPassword, "2");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM3:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "3");
        break;
      case 2:
        std::strcat(this->newPassword, "3");
        break;
      case 3:
        std::strcat(this->confirmPassword, "3");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM4:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "4");
        break;
      case 2:
        std::strcat(this->newPassword, "4");
        break;
      case 3:
        std::strcat(this->confirmPassword, "4");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM5:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "5");
        break;
      case 2:
        std::strcat(this->newPassword, "5");
        break;
      case 3:
        std::strcat(this->confirmPassword, "5");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM6:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "6");
        break;
      case 2:
        std::strcat(this->newPassword, "6");
        break;
      case 3:
        std::strcat(this->confirmPassword, "6");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM7:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "7");
        break;
      case 2:
        std::strcat(this->newPassword, "7");
        break;
      case 3:
        std::strcat(this->confirmPassword, "7");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM8:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "8");
        break;
      case 2:
        std::strcat(this->newPassword, "8");
        break;
      case 3:
        std::strcat(this->confirmPassword, "8");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM9:
      if (++this->inputDigitCount > 8) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_8), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, "9");
        break;
      case 2:
        std::strcat(this->newPassword, "9");
        break;
      case 3:
        std::strcat(this->confirmPassword, "9");
        break;
      }
      this->DrawTextAt(this->maskTextX + 112, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM_RESET:
      if (this->passwordStage == 0) {
        this->passwordStage = 0;
        this->activeInputY = this->currentPasswordY;
      } else if (g_session.m_passwordInputAlternate) {
        this->passwordStage = 2;
        this->activeInputY = this->newPasswordY;
      } else {
        this->passwordStage = 1;
        this->activeInputY = this->currentPasswordY;
      }
      this->inputDigitCount = 0;
      this->maskTextX = 0;
      std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
      std::memset(this->newPassword, 0, sizeof(this->newPassword));
      std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
      this->Paint();
      break;

    }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008ECE60
// FUNCTION: fn_12211
void UIBookWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");

  char* title = new char[110];
  std::sprintf(title, "%s ( %d / %d )", this->m_bookName, this->m_page, this->m_pageCount);
  this->DrawTextAt(this->geometry.width / 2 - 72, 2, title, 0, 0, 12, 0, 0, 0);

  this->FillRegion(1, titleHeight, this->geometry.width - 2, this->geometry.height - 18, RGB(this->m_red, this->m_green, this->m_blue));
  this->m_viewer->backgroundColor = 0xFF000000u | RGB(this->m_red, this->m_green, this->m_blue);
  this->m_viewer->Invalidate();
}

// OFFSET: 0x008ECF50
// FUNCTION: fn_12212
void UICardItemIllustWnd::Paint() {
  this->DrawTitleBar(false, "", 0);
  if (this->m_bitmapPath.size() != 0) {
    this->DrawBitmap(0, 17, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapPath.c_str()))), false);
  }

  ITEM_INFO item;
  item.itemId = this->m_name;
  item.identified = this->identified;
  item.refiningLevel = 0;
  this->DrawFittedItemName(10, 3, &item, 0, 0, 12, 0);
}

// OFFSET: 0x008E8480
// FUNCTION: fn_12198
void UISelectCartWnd::OnCreate(int, int) {
  this->AddCloseButton("", 0);

  const int levels[] = {0, 40, 65, 80, 90, 100, 110, 120, 130};
  std::vector<int>& carts = *reinterpret_cast<std::vector<int>*>(&this->cartTypesBegin);
  const int level = g_session.m_level;
  carts.clear();
  for (int i = 0; i < 9; ++i) {
    if (level <= levels[i]) {
      break;
    }
    carts.push_back(i + 1);
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
  std::vector<int>& carts = *reinterpret_cast<std::vector<int>*>(&this->cartTypesBegin);
  switch (message) {
  case UM_SETINFO: {
    this->DeleteChildren();
    this->AddCloseButton("", 0);
    carts.clear();
    this->selectionContext = static_cast<int>(c);
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
    break;
  }
  case UM_COMMAND:
    if (a != ID_CLOSE) {
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
      g_modeMgr.GetGameMode()->SendMsg(MM_SELECT_CART, type, this->selectionContext, 0, 0);
    }
    g_windowMgr.CloseWindow(WID_SELECTCARTWND);
    break;
  }
  return 0;
}

namespace {
#pragma pack(push, 1)
struct ReplayTaggedRecordHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t bytes;
};
#pragma pack(pop)

// INLINED
inline std::vector<unsigned char> MakeReplayTaggedRecord(std::uint16_t id, const void* data, std::uint32_t bytes) {
  std::vector<unsigned char> record(bytes + 6u, 0);
  ReplayTaggedRecordHeader* header = reinterpret_cast<ReplayTaggedRecordHeader*>(record.data());
  header->id = id;
  header->bytes = bytes;
  if (bytes != 0) {
    std::memcpy(record.data() + sizeof(ReplayTaggedRecordHeader), data, bytes);
  }
  return record;
}

// INLINED
template <class Record> inline void AppendReplayRecord(std::vector<unsigned char>& records, const Record& record) {
  records.insert(records.end(), record.begin(), record.end());
}
}


// OFFSET: 0x008D3AB0
// FUNCTION: fn_12104
std::vector<unsigned char> UIBabyAcceptWnd2::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(21550, nullptr, 0));
  AppendReplayRecord(records, MakeReplayTaggedRecord(21551, nullptr, 0));
  return records;
}

// OFFSET: 0x008D4100
// FUNCTION: fn_12106
std::vector<unsigned char> UIChooseSellBuyWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(20850, nullptr, 0));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20851, nullptr, 0));
  return records;
}

// INLINED
template <class Window> static __forceinline void AppendAcceptanceReplayRecords(std::vector<unsigned char>& records, Window* window, std::uint16_t id, int message) {
  AppendReplayRecord(records, MakeReplayTaggedRecord(id, nullptr, 0));

  char text[260];
  std::memset(text, 0, sizeof(text));
  window->viewer->CopyText(text, 0);
  const char parentheses[] = {'(', ')'};
  std::string decoration(parentheses, sizeof(parentheses));
  decoration.append(LookupMessage(message));
  AppendReplayRecord(records, BuildReplayTaggedRecord(id + 2, text + 1, window->viewer->TextLength() - decoration.size()));

  AppendReplayRecord(records, MakeReplayTaggedRecord(id + 1, nullptr, 0));
}

// OFFSET: 0x008D4A40
// FUNCTION: fn_12109
std::vector<unsigned char> UIExchangeAcceptWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendAcceptanceReplayRecords(records, this, 21650, 93);
  return records;
}

// OFFSET: 0x008D3CD0
// FUNCTION: fn_12105
std::vector<unsigned char> UIBabyAcceptWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendAcceptanceReplayRecords(records, this, 21500, 794);
  return records;
}

// OFFSET: 0x008D5C80
// FUNCTION: fn_12113
std::vector<unsigned char> UIJoinFriendAcceptWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendAcceptanceReplayRecords(records, this, 21600, 818);
  return records;
}

// OFFSET: 0x008D60B0
// FUNCTION: fn_12114
std::vector<unsigned char> UIJoinGuildAcceptWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendAcceptanceReplayRecords(records, this, 21800, 377);
  return records;
}

// OFFSET: 0x008D64E0
// FUNCTION: fn_12115
std::vector<unsigned char> UIJoinPartyAcceptWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendAcceptanceReplayRecords(records, this, 21750, 94);
  return records;
}

// OFFSET: 0x008D7110
// FUNCTION: fn_12118
std::vector<unsigned char> UIStarPlaceAcceptWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(21700, nullptr, 0));
  AppendReplayRecord(records, MakeReplayTaggedRecord(21702, &this->m_place, sizeof(this->m_place)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(21701, nullptr, 0));
  return records;
}

// OFFSET: 0x008D4600
// FUNCTION: fn_12108
std::vector<unsigned char> UIChooseWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(21900, nullptr, 0));

  const int count = static_cast<int>(this->m_list->lines.size());
  for (int index = 0; index < count; ++index) {
    const std::string line = this->m_list->lines[index];
    AppendReplayRecord(records, MakeReplayTaggedRecord(21903, line.c_str(), line.size()));
  }

  AppendReplayRecord(records, MakeReplayTaggedRecord(21901, nullptr, 0));
  return records;
}

// INLINED
static __forceinline void AppendItemListReplayRecords(std::vector<unsigned char>& records, const std::list<ITEM_INFO>& items, std::uint16_t id, std::int16_t packetId) {
  AppendReplayRecord(records, MakeReplayTaggedRecord(id, nullptr, 0));

  std::vector<unsigned char> packet(items.size() * 2 + 4, 0);
  std::int16_t* fields = reinterpret_cast<std::int16_t*>(packet.data());
  fields[0] = packetId;
  fields[1] = static_cast<std::int16_t>(packet.size());
  std::int16_t* index = fields + 2;
  for (std::list<ITEM_INFO>::const_iterator item = items.begin(); item != items.end(); ++item) {
    *index++ = static_cast<std::int16_t>(item->identity[1]);
  }
  AppendReplayRecord(records, MakeReplayTaggedRecord(id + 3, packet.data(), packet.size()));

  AppendReplayRecord(records, MakeReplayTaggedRecord(id + 1, nullptr, 0));
}

// OFFSET: 0x008D59B0
// FUNCTION: fn_12112
std::vector<unsigned char> UIItemIdentifyWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendItemListReplayRecords(records, this->m_items, 22300, 0x177);
  return records;
}

// OFFSET: 0x008D56E0
// FUNCTION: fn_12111
std::vector<unsigned char> UIItemCompositionWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendItemListReplayRecords(records, this->m_items, 22350, 0x17B);
  return records;
}

// OFFSET: 0x008D6910
// FUNCTION: fn_12116
std::vector<unsigned char> UIMakingArrowListWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(22100, nullptr, 0));

  for (std::list<ITEM_INFO>::iterator item = this->m_items.begin(); item != this->m_items.end(); ++item) {
    const int itemId = std::atoi(item->itemId.c_str());
    AppendReplayRecord(records, MakeReplayTaggedRecord(22103, &itemId, sizeof(itemId)));
  }

  AppendReplayRecord(records, MakeReplayTaggedRecord(22102, &this->listType, sizeof(this->listType)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(22101, nullptr, 0));
  return records;
}

// OFFSET: 0x008EF5D0
// FUNCTION: fn_12234
void UINotifyItemObtainWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  if (this->m_itemId.size() != 0) {
    DrawItemIconById(*this, 13, 5, this->m_itemId.c_str(), this->m_identified);
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

  const int written = sprintf_s(cursor, remaining, LookupMessage(MSI_EA_OBTAIN), this->m_item.quantity);
  if (written > 0) {
    remaining -= written;
    cursor += written;
    breaks.push_back(cursor);
  }

  this->DrawItemNameSegments(kind == 0 ? 41 : 13, 10, width, breaks, text, highlight, highlightColor, 0, 0, 12);
}

// OFFSET: 0x008EB010
// FUNCTION: fn_12207
void UIWhisperWnd::OnCreate(int, int) {
  this->m_message0 = 184;

  this->m_resizer = new UIResizer();
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
  this->m_edit->TextColor(255, 255, 255, 0xFFFFFFFF);
  this->AddChild(this->m_edit);
  g_windowMgr.SetFocus(this->m_edit);
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
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  if (a == ID_OK) {
    reinterpret_cast<std::string&>(g_session.autoReplyState[4]) = this->m_edit->Text();
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_AUTO_MSG_ON)), 0x64FFFF, 0, 0, 0);
    g_windowMgr.RestoreInput();
    g_windowMgr.CloseWindow(WID_AUTOMESSAGEWND);
  }
  return 0;
}

// OFFSET: 0x008F79C0
// FUNCTION: fn_12328
int UICardItemIllustWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    if (a == ID_CLOSE) {
      g_windowMgr.CloseWindow(WID_CARDITEMILLUSTWND);
    }
    break;
  case UM_CARDITEMILLUSTRATE: {
    const char* id = reinterpret_cast<const char*>(a);
    this->identified = b != 0;
    const char* name = FindItemCardName(std::atoi(id)).name;
    if (IsItemCard(std::atoi(id)) && this->m_name != id) {
      this->m_name = id;
      this->m_bitmapPath = "유저인터페이스\\cardBmp\\";
      this->m_bitmapPath += name;
      this->m_bitmapPath += ".bmp";
      this->Invalidate();
      g_windowMgr.Promote(this);
      break;
    }
    g_windowMgr.CloseWindow(WID_CARDITEMILLUSTWND);
    break;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008EFFF0
// FUNCTION: fn_12248
void UISecondPasswordInputWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_SECOND_PASSWORD), 0);
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
  this->DrawEmboldenedText(414, 76, LookupMessage(MSI_STORE_PASSWORD), textColor, shadowColor, 0, 0, 12);
  this->FillRegion(414, 89, 100, 16, boxColor);
  this->FillRegion(415, 90, 98, 14, 0xFFFFFF);

  if (!g_session.m_passwordInputAlternate && this->isChangingPassword == 0) {
    this->DrawEmboldenedText(414, 112, LookupMessage(MSI_STORE_NEW_PASSWORD), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(414, 127, 100, 16, 0xE6E6E6);
    this->DrawEmboldenedText(414, 152, LookupMessage(MSI_STORE_RENEW_PASSWORD), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(414, 166, 100, 16, 0xE6E6E6);
  } else {
    this->DrawEmboldenedText(414, 112, LookupMessage(MSI_STORE_NEW_PASSWORD), 0xE6E6E6, 0, 0, 0, 12);
    this->FillRegion(414, 127, 100, 16, 0xFAE6C8);
    this->DrawEmboldenedText(414, 152, LookupMessage(MSI_STORE_RENEW_PASSWORD), 0xE6E6E6, 0, 0, 0, 12);
    this->FillRegion(414, 166, 100, 16, 0xFAE6C8);
    if (g_session.m_passwordInputAlternate) {
      this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
    }
  }

  if (this->passwordStage == 2 || this->passwordStage == 3) {
    if (this->passwordStage == 3) {
      this->FillRegion(415, 167, 98, 14, 0xFFFFFF);
    }
    this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
  }

  for (std::size_t index = 0, x = 416; index < std::strlen(this->currentPassword); ++index, x += 5) {
    this->DrawTextAt(static_cast<int>(x), this->currentPasswordY, "*", 0, 0, 12, 0, 0, 0);
  }
  for (std::size_t index = 0, x = 416; index < std::strlen(this->newPassword); ++index, x += 5) {
    this->DrawTextAt(static_cast<int>(x), this->newPasswordY, "*", 0, 0, 12, 0, 0, 0);
  }
  for (std::size_t index = 0, x = 416; index < std::strlen(this->confirmPassword); ++index, x += 5) {
    this->DrawTextAt(static_cast<int>(x), this->confirmPasswordY, "*", 0, 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x008F2BC0
// FUNCTION: fn_12260
void UInCashWnd::Paint() {
  if (auto* background =
        static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\NC_BackGround3.bmp")))) {
    this->DrawBitmap(0, 0, background, false);
  }

  char text[64];
  std::memset(text, 0, sizeof(text));
  char number[64];
  std::memset(number, 0, sizeof(number));

  int length = std::sprintf(text, "%d / %d", this->m_page, this->m_pageCount);
  this->DrawRightAlignedText(270, this->geometry.height - 55, text, length, 0, 12, 0, 0);

  length = std::sprintf(text, LookupMessage(MSI_NC_FREECASH), FormatGroupedInteger(this->m_cashBalance, number, sizeof(number)));
  this->DrawShadowedText(74, this->geometry.height - 36, text, length, 0, 0xFFFFFF, 0, 12, 0);
  length = std::sprintf(text, LookupMessage(MSI_NC_CASH), FormatGroupedInteger(this->m_pointBalance, number, sizeof(number)));
  this->DrawShadowedText(74, this->geometry.height - 17, text, length, 0, 0xFFFFFF, 0, 12, 0);

  this->DrawTextAt(260, this->geometry.height - 36, LookupMessage(MSI_USE_FREE_POINT), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(427, this->geometry.height - 36, "C", 0, 0, 12, 0, 0, 0);
  length = std::sprintf(text, "Total              :      %s", FormatGroupedInteger(this->m_totalPrice, number, sizeof(number)));
  this->DrawShadowedText(260, this->geometry.height - 17, text, length, 0, 0xFFFFFF, 0, 12, 0);
  this->DrawTextAt(427, this->geometry.height - 17, "C", 0, 0, 12, 0, 0, 0);

  this->DrawItems();
}

// OFFSET: 0x008F4D50
// FUNCTION: fn_12295
void UIExchangeWnd::OnProcess() {
  if (this->zenyEdit == nullptr) {
    return;
  }

  const int length = this->zenyEdit->Length();
  if (length == this->m_moneyLength) {
    return;
  }
  this->m_moneyLength = length;
  if (length == 0) {
    return;
  }

  char text[24];
  std::sprintf(text, "%s ", this->zenyEdit->Text());
  if (!g_session.IsNumberWithCommas(text)) {
    g_windowMgr.Confirm(LookupMessage(MSI_CAN_INPUT_NUMBER_ONLY), 0, true, false, 0, "Message", 280, 120, 0);
    this->zenyEdit->SetText(this->m_moneyText.c_str());
    g_windowMgr.SetFocus(this->zenyEdit);
    return;
  }

  std::int64_t money = std::max<std::int64_t>(_atoi64(text), 0);
  if (money > 1000000000) {
    g_windowMgr.Confirm(LookupMessage(MSI_BASIC_EXP_MSG_INDONESIA), 2, true, false, 0, "Message", 280, 120, 0);
    FormatGroupedInteger(1000000000LL, text, sizeof(text));
    this->zenyEdit->SetText(text);
    money = 1000000000;
  } else {
    FormatGroupedInteger(money, text, sizeof(text));
    this->zenyEdit->SetText(text);
    this->m_moneyText = this->zenyEdit->Text();
  }

  const PriceColors colors = PriceColor(money);
  this->zenyEdit->TextColor(GetRValue(colors.primary), GetGValue(colors.primary), GetBValue(colors.primary), colors.secondary);
  this->m_moneyLength = this->zenyEdit->Length();
  this->zenyEdit->SetCursor(this->zenyEdit->Length());
}

// OFFSET: 0x008EE2B0
// FUNCTION: fn_12224
void UIItemCollectionWnd::Paint() {
  int height = this->geometry.height;
  if (this->m_item.identified == 1 && this->m_optionCount != 0) {
    height -= this->m_extraHeight;
  }
  const int bottom = this->m_compare == 1 ? height - 29 : height;

  const char* background;
  if (this->m_kind == 234 || this->m_kind == 250) {
    this->DrawSkinBox("유저인터페이스\\basic_interface\\sysbox_comparison_", 0, 0, this->geometry.width, bottom, true);
    background = "유저인터페이스\\basic_interface\\collection_Comparison_bg.bmp";
  } else if (this->m_item.grade > 0) {
    const int width = this->geometry.width;
    this->DrawSkinBox(FormatOwnedString("유저인터페이스\\sysbox_g%d_", this->m_item.grade).c_str(), 0, 0, width, bottom, false);
    this->DrawBitmap(
      0, 0,
      static_cast<CBitmapRes*>(
        ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\basic_interface\\collection_bg_g%d.bmp", this->m_item.grade).c_str()))
      ),
      true
    );
    background = nullptr;
  } else {
    this->DrawSkinBox("유저인터페이스\\sysbox_", 0, 0, this->geometry.width, bottom, true);
    background = "유저인터페이스\\basic_interface\\collection_bg.bmp";
  }
  if (background != nullptr) {
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background))), true);
  }

  if (this->m_bitmapName.size() != 0) {
    this->DrawBitmap(10, 11, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str()))), true);
  }
  this->DrawFittedItemName(90, 10, &this->m_item, this->m_item.identified ? 0 : 0x808080, 0, 12, 0);

  if (this->m_compare == 1) {
    int offset = 0;
    if (this->m_item.identified == 1 && this->m_optionCount > 0) {
      for (std::map<int, std::list<std::string>>::iterator card = this->m_cardTexts.begin(); card != this->m_cardTexts.end(); ++card) {
        const char* box = this->m_kind == 234 ? "유저인터페이스\\basic_interface\\sysbox_comparison_" : "유저인터페이스\\sysbox_";
        this->DrawSkinBox(box, 0, bottom + offset, this->geometry.width, static_cast<int>(card->second.size()) * 15 + 14, true);
        offset += 8;
        for (std::list<std::string>::iterator line = card->second.begin();;) {
          if (*line == LookupMessage(MSI_MONSTERINFO_TRIBE_ANIMAL)) {
            this->DrawCenteredText(0, bottom + offset, line->c_str(), this->geometry.width, 0, 12, 0xFF, 0);
            break;
          }
          this->DrawTextAt(12, bottom + offset, line->c_str(), 0, 0, 12, 0, 0, 0);
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
        const int card = reinterpret_cast<const int*>(this->m_item.cards)[slot];
        if (card < 1) {
          CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\empty_card_slot.bmp")));
          if (this->m_kind == 234) {
            const char* name = slot < ItemSlotsCount(this->m_item) ? "유저인터페이스\\basic_interface\\comparison_empty_card_slot.bmp"
                                                                   : "유저인터페이스\\basic_interface\\comparison_disable_card_slot.bmp";
            bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
          } else if (ItemSlotsCount(this->m_item) <= slot) {
            bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\disable_card_slot.bmp")));
          }
          this->DrawBitmap(x, y, bitmap, true);
        } else {
          DrawItemIconById(*this, x, y, FormatOwnedString("%d", card).c_str(), 0);
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
void UIAutoMessageWnd::OnCreate(int, int) {
  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(this->geometry.width - 77, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 100;
  this->m_edit->Background(232, 232, 232);
  this->m_edit->SetText(reinterpret_cast<const std::string&>(g_session.autoReplyState[4]).c_str());
  this->m_edit->SelectAll();
  this->AddChild(this->m_edit);

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton();
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
void UInCash_CallWnd::OnCreate(int, int) {
  const int ids[] = {192};
  const int positions[] = {0};
  const char* names[] = {"NC_CashShop"};
  const std::string prefix("유저인터페이스\\basic_interface\\");
  for (int index = 0; index < 1; ++index) {
    auto* button = new UIBitmapButton();
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
  std::map<int, int>& traitStats = this->m_traits.stats;
  return traitStats[type];
}

// OFFSET: 0x008C7F00
// FUNCTION: fn_11946
UIItemIdentifyWnd::UIItemIdentifyWnd()
    : UIFrameWnd(0), m_placement(nullptr) {
}

// OFFSET: 0x008C7EA0
// FUNCTION: fn_11945
UIItemCompositionWnd::UIItemCompositionWnd()
    : UIItemIdentifyWnd(), m_compositionPlacement(nullptr) {
}

// OFFSET: 0x008C90E0
// FUNCTION: fn_11976
UISpellListWnd::UISpellListWnd() : UIItemIdentifyWnd() {
  this->m_sourceSkillId = 0;
}

// OFFSET: 0x00900B10
// FUNCTION: fn_12370
int UIPetEggListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_ADDITEM:
    this->m_items.push_back(LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(argument2)));
    this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    return 0;
  case UM_REFRESH:
    this->UpdateScroll();
    this->Invalidate();
    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_scroll;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
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
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(argument1), argument2, this->m_selected, argument4);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(argument3);
    }
    switch (argument1) {
    case ID_OK: {
      int found = 0;
      ITEM_INFO item = this->GetItem(this->m_selected, &found);
      if (found) {
        g_modeMgr.gameMode->SendMsg(MM_PETEGG_SELECT_FROM_LIST, item.identity[1], 0, 0, 0);
      }
      break;
    }
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_PETEGGLISTWND);
      break;
    }
    return 0;
  }
  default:
    return 0;
  }
}

// OFFSET: 0x008C8330
// FUNCTION: fn_11952
UIMakingArrowListWnd::UIMakingArrowListWnd()
    : UIItemIdentifyWnd() {
}

// OFFSET: 0x008D4320
// FUNCTION: fn_12107
std::vector<unsigned char> UIChooseWarpWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(21950, nullptr, 0));
  for (auto choice = this->m_choices.begin(); choice < this->m_choices.end(); ++choice) {
    AppendReplayRecord(records, MakeReplayTaggedRecord(21953, choice->c_str(), choice->size()));
  }
  AppendReplayRecord(records, MakeReplayTaggedRecord(21951, nullptr, 0));
  return records;
}

// OFFSET: 0x008D4E70
// FUNCTION: fn_12110
std::vector<unsigned char> UIExchangeWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(20600, nullptr, 0));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20603, &this->partnerId, sizeof(this->partnerId)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20604, &this->partnerLevel, sizeof(this->partnerLevel)));
  AppendReplayRecord(records, BuildReplayTaggedRecord(20602, this->partnerName.c_str(), this->partnerName.size()));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20605, &this->isLocalTrader, sizeof(this->isLocalTrader)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20606, &this->localConfirmed, sizeof(this->localConfirmed)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20607, &this->remoteConfirmed, sizeof(this->remoteConfirmed)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20608, &this->localAccepted, sizeof(this->localAccepted)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20609, &this->remoteAccepted, sizeof(this->remoteAccepted)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(20601, nullptr, 0));
  return records;
}

// OFFSET: 0x008D6D40
// FUNCTION: fn_12117
std::vector<unsigned char> UISayDialogWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, MakeReplayTaggedRecord(22500, nullptr, 0));
  const UITagTextViewer* viewer = static_cast<UITagTextViewer*>(this->m_viewer);
  for (auto text = viewer->m_texts.begin(); text != viewer->m_texts.end(); ++text) {
    AppendReplayRecord(records, MakeReplayTaggedRecord(22503, text->c_str(), text->size()));
  }
  AppendReplayRecord(records, MakeReplayTaggedRecord(22504, &this->m_responsePending, sizeof(this->m_responsePending)));
  AppendReplayRecord(records, MakeReplayTaggedRecord(22501, nullptr, 0));
  return records;
}

// OFFSET: 0x008E30B0
// FUNCTION: fn_12180
void UILoadingWnd::OnCreate(int, int) {
  // GLOBAL: gl_25392
  static int s_loadingScreenIndex;

  this->m_progressBar = new UIBarGraph2();
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
  if (g_session.m_localMapMove == 1) {
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
  g_windowMgr.SetBackgroundSurface(static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name))));
}
// OFFSET: 0x008C8A50
// FUNCTION: fn_11966
UIPetEggListWnd::UIPetEggListWnd()
    : UIItemIdentifyWnd() {
}

// OFFSET: 0x008D1E80
// FUNCTION: fn_12096
void UIEquipWnd::DrawEquipSlots() {
  if (this->m_page == 1) {
    auto* frame = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\equipwin_special.bmp")));
    if (frame) {
      this->DrawBitmap(0, this->m_contentTop + 14, frame, false);
    }
  }

  const int rowOffsets[4] = {0, 26, 52, 78};
  int iconX[2] = {6, 251};
  int nameX[2] = {28, 167};
  int shopId = g_session.FindShopId("");
  if (g_session.GetShopType(shopId)) {
    shopId = -1;
  }

  for (unsigned slot = 0; slot < 10; ++slot) {
    ITEM_INFO item;
    if (this->m_page == 1) {
      if (this->m_viewOther == 0) {
        item = g_session.GetCostumeItemAt(slot);
      } else {
        item = g_session.GetAlternateCostumeItemAt(slot);
      }
    } else if (this->m_viewOther == 0) {
      item = g_session.GetEquippedItemAt(slot);
    } else {
      item = g_session.GetAlternateEquippedItemAt(slot);
    }

    if (item.identity[1] == 0) {
      continue;
    }

    if (item.quantity == 1) {
      const unsigned row = slot & 3;
      if (slot < 8) {
        DrawItemIcon(*this, ResourceMgr::Instance(), iconX[slot / 4], this->m_contentTop + 44 + row * 26, item, 0);
        this->DrawItemName(nameX[slot / 4] + 5, this->m_contentTop + rowOffsets[row] + 51, 70, &item, 0, 0, 12);
      } else {
        DrawItemIcon(*this, ResourceMgr::Instance(), iconX[row], this->m_contentTop + 18, item, 0);
        this->DrawItemName(nameX[slot - 8] + 5, this->m_contentTop + 25, 70, &item, 0, 0, 12);
      }
    }

    if (item.grade > 0) {
      if (slot < 8) {
        auto* grade = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade).c_str())
        ));
        this->DrawBitmap(iconX[slot / 4], this->m_contentTop + 56 + (slot & 3) * 26, grade, true);
      } else {
        auto* grade = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade).c_str())
        ));
        this->DrawBitmap(iconX[slot & 3], this->m_contentTop + 30, grade, true);
      }
    }

    if (shopId != -1 && g_session.GetShopEntry(0, std::atoi(item.itemId.c_str())).first == 0) {
      const unsigned row = slot & 3;
      const unsigned column = slot < 8 ? slot / 4 : row;
      auto* banned = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\ban.bmp")));
      const int y = slot < 8 ? this->m_contentTop + (row + 1) * 26 : this->m_contentTop;
      this->DrawBitmap(iconX[column], y + 18, banned, true);
    }
  }
}

// OFFSET: 0x008F6560
// FUNCTION: fn_12318
void UIEquipWnd::RefreshTitleList() {
  this->m_titleList->Clear();
  this->m_titleList->AddLine(LookupMessage(MSI_GUILD_JOIN_GUILD_NOT_PERMITTED_MAP), 0);
  for (std::vector<int>::iterator it = g_session.jobTitles.begin(); it != g_session.jobTitles.end(); ++it) {
    const std::string title = GetJobTitleString(*it);
    this->m_titleList->AddLine(title.c_str(), *it);
  }
}

// OFFSET: 0x008F5580
// FUNCTION: fn_12304
void UIItemCollectionWnd::OnRBtnDown(int x, int y) {
  int slotX;
  int slotY;
  const int index = this->CardSlotAt(x, y, &slotX, &slotY);
  if (index == -1) {
    return;
  }

  ITEM_INFO card;
  int cardId;
  std::memcpy(&cardId, this->m_item.cards + index * sizeof(cardId), sizeof(cardId));
  card.identified = 1;
  card.identity[0] = 6;
  card.refiningLevel = 0;
  card.SetItemId(cardId);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_COMBINEDCARDITEMCOLLECTIONWND);
  window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, card.identified, 0, 0, 0);
  window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&card), index >= ItemSlotsCount(this->m_item), 0, 0);

  if (UIItemCollectionWnd* comparison = static_cast<UIItemCollectionWnd*>(g_windowMgr.itemCollectionComparisonWnd)) {
    int compareId;
    std::memcpy(&compareId, comparison->m_item.cards + index * sizeof(compareId), sizeof(compareId));
    if (compareId != 0 && compareId != 254 && compareId != 255) {
      ITEM_INFO compareCard;
      compareCard.identified = 1;
      compareCard.identity[0] = 6;
      compareCard.refiningLevel = 0;
      compareCard.SetItemId(compareId);
      UIWindow* compareWindow = g_windowMgr.EnsureWindow(WID_COMBINEDCARDITEMCOLLECTION_COMPARISONWND);
      compareWindow->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, compareCard.identified, 0, 0, 0);
      compareWindow->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&compareCard), 1, 0, 0);
    }
  }
}

// OFFSET: 0x008F5790
// FUNCTION: fn_12305
void UIItemCollection_ComparisonWnd::OnRBtnDown(int x, int y) {
  int slotX;
  int slotY;
  const int index = this->CardSlotAt(x, y, &slotX, &slotY);
  if (index == -1) {
    return;
  }

  ITEM_INFO card;
  int cardId;
  std::memcpy(&cardId, this->m_item.cards + index * sizeof(cardId), sizeof(cardId));
  card.identified = 1;
  card.identity[0] = 6;
  card.refiningLevel = 0;
  card.SetItemId(cardId);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_COMBINEDCARDITEMCOLLECTIONWND);
  window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, card.identified, 0, 0, 0);
  window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&card), index >= ItemSlotsCount(this->m_item), 0, 0);
}

// OFFSET: 0x008F3660
// FUNCTION: fn_12270
void UIEquipWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    this->UIFrameWnd::OnLBtnDown(x, y);
  }

  if (this->m_viewOther != 0) {
    return;
  }

  ITEM_INFO item = this->ItemIconAt(x, y);
  if (item.identity[1] == 0) {
    return;
  }

  if (item.quantity > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
    mode->drag.BeginItem(g_windowMgr, this->m_page == 1 ? 24 : 2, item, item.quantity, slots.values[2]);
  }

  if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
    if (g_windowMgr.GetHoverWindow() && item.identified != 0) {
      if (g_windowMgr.GetHoverWindow()->geometry.id == 492) {
        reinterpret_cast<UINpcTextEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->InsertItemTag(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 490) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 493) {
        reinterpret_cast<UIChatEditCtrl*>(ChatWindow()->m_chatEdit)->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 491) {
        reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->InsertItemTag(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 494) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      }
    }
  }
}

// OFFSET: 0x008C9880
// FUNCTION: fn_11986
UInCashWnd::UInCashWnd() : UIItemBaseWnd() {
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
void UInCashWnd::OnLBtnDblClk(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.identity[1] == 0 || this->m_selected >= 0) {
    return;
  }

  g_modeMgr.GetGameMode();
  this->m_selected = ~this->m_selected;
  g_session.RemoveItem1720ByName(item);
  this->m_totalPrice -= item.quantity * item.prices[1];
  this->m_otherItems.erase(this->m_otherItems.begin(), this->m_otherItems.end());

  const CSession& lists = g_session;
  const int count = lists.GetList2Size();
  for (int index = 0; index < count; ++index) {
    ITEM_INFO other = g_session.GetItem1720At(index);
    if (other.identity[1] != 0) {
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
  case UM_CLEAR_ITEM:
    this->m_items.clear();
    break;
  case UM_ADDITEM: {
    ITEM_INFO item;
    item.SetItemId(*reinterpret_cast<const int*>(a));
    item.identified = 1;
    this->m_items.push_back(item);
    this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    break;
  }
  case UM_REFRESH:
    this->UpdateScroll();
    this->Invalidate();
    break;
  case UM_SETINFO:
    this->listType = static_cast<std::int16_t>(a);
    break;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_scroll;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->m_scroll = previous - this->m_rows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
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
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, this->m_selected);
    if (g_replayAnimationScaling != 0) {
      this->m_selected = static_cast<int>(d);
      this->Invalidate();
    }
    switch (a) {
    case ID_OK: {
      int found;
      ITEM_INFO item = this->GetItem(this->m_selected, &found);
      if (found != 0) {
        if (this->listType == 2) {
          g_modeMgr.gameMode->SendMsg(MM_ARROW_FROM_LIST, item.GetItemId(), 0, 0, 0);
        } else {
          g_modeMgr.gameMode->SendMsg(MM_ITEM_FROM_LIST, item.GetItemId(), this->listType, 0, 0);
        }
        g_windowMgr.CloseWindow(WID_MAKINGARROWLISTWND);
      }
      break;
    }
    case ID_CANCEL:
      if (this->listType == 2) {
        g_modeMgr.gameMode->SendMsg(MM_ARROW_FROM_LIST, -1, 0, 0, 0);
      } else {
        g_modeMgr.gameMode->SendMsg(MM_ITEM_FROM_LIST, -1, this->listType, 0, 0);
      }
      g_windowMgr.CloseWindow(WID_MAKINGARROWLISTWND);
      break;
    }
    break;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x5654) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        switch (header.type) {
        case 0x5655:
          return 0;
        case 0x5656: {
          int value = 0;
          std::memcpy(&value, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, UM_SETINFO, static_cast<std::int16_t>(value), 0, 0, 0);
          break;
        }
        case 0x5657: {
          int value = 0;
          std::memcpy(&value, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(&value), 0, 0, 0);
          break;
        }
        default:
          position += header.length;
          break;
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
void UITipboxWnd::OnProcess() {
  if (this->refreshPending == true) {
    this->refreshPending = false;
    const int index = (std::max)(0, g_itemTagOwner->paragraphIndex);
    const std::string text = g_itemTagOwner->m_currentEntry.paragraphs[index];
    std::vector<std::string> lines;
    WrapPlainText(text.c_str(), lines, 1000);
    this->tipViewer->Clear();
    for (const std::string& line : lines) {
      this->tipViewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(line.c_str()), 32, 0, 0);
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
  case UM_DOWNPUSH:
    this->m_list->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    return 0;
  case UM_UPPUSH:
    this->m_list->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
    return 0;
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_OK: {
      const int selected = this->m_list->selected;
      if (selected != -1) {
        g_modeMgr.GetGameMode()->SendMsg(MM_SELECT_WARPPOINT, reinterpret_cast<std::intptr_t>(this->m_choices[selected].c_str()), this->skillId, 0, 0);
        g_windowMgr.CloseWindow(WID_CHOOSEWARPWND);
      }
      break;
    }
    case ID_CANCEL:
      g_modeMgr.GetGameMode()->SendMsg(MM_SELECT_WARPPOINT, reinterpret_cast<std::intptr_t>("cancel"), this->skillId, 0, 0);
      g_windowMgr.CloseWindow(WID_CHOOSEWARPWND);
      break;
    }
    return 0;
  }
  case UM_SETSKID:
    this->skillId = static_cast<int>(a);
    return 0;
  case UM_SETCHOOSE:
    if (a != 0) {
      const char* choice = reinterpret_cast<const char*>(a);
      if (std::string(choice) == "Random") {
        this->m_list->AddLine(LookupMessage(MSI_RANDOM_POS));
      } else if (std::string(choice) == "cancel") {
        this->m_list->AddLine(LookupMessage(MSI_CANCEL));
      } else {
        this->m_list->AddLine(g_session.GetMapDisplayName(g_session.GatName2RswName(choice)));
      }
      this->m_choices.push_back(choice);
    }
    return 0;
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x55BE) {
      unsigned int position = sizeof(header);
      while (position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        switch (header.type) {
        case 0x55BF:
          this->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>("cancel"), 0, 0, 0);
          return 0;
        case 0x55C1: {
          char text[260] = {};
          std::memcpy(text, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
          break;
        }
        default:
          position += header.length;
          break;
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
  this->DrawTitleBar(true, LookupMessage(MSI_AGAIN), 0);

  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\TipboxBG.bmp")));
  this->DrawBitmap(0, 17, background, false);

  if (this->searchResults->geometry.visible == 1) {
    const char* path = g_itemTagOwner->HasSearchResults() ? "유저인터페이스\\tipbox\\img_loading.bmp"
                                                       : "유저인터페이스\\tipbox\\img_NoSearchResults.bmp";
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    this->DrawBitmap(1, 17, bitmap, false);
  } else {
    CTipBoxMgr* manager = g_itemTagOwner;
    switch (manager->m_image.kind) {
    case 0:
      if (manager->m_currentEntry.imageCell == 5 && manager->m_currentEntry.imageScalePercent == 100) {
        const std::string path = manager->m_currentEntry.bitmapName + ".bmp";
        auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
        this->DrawBitmap(1, 17, bitmap, false);
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
      auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\img_noImage.bmp")));
      this->DrawBitmap(1, 17, bitmap, false);
      break;
    }
    }
  }

  this->PaintAnimations();
  auto* border = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\bg_tipbox.bmp")));
  this->DrawBitmap(241, 17, border, false);
  auto* bottom = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\btnbar_bottom.bmp")));
  this->DrawBitmap(0, 337, bottom, false);
}

// OFFSET: 0x008E9370
// FUNCTION: fn_12203
void UITipOfTheDayWnd::OnCreate(int width, int height) {
  const std::string prefix("유저인터페이스\\");
  const char* names[3][3] = {{"btn_back", "btn_back_a", "btn_back_b"}, {"btn_next", "btn_next_a", "btn_next_b"}, {"btn_close", "btn_close_a", "btn_close_b"}};
  int positions[3][2] = {{144, this->geometry.height - 24}, {189, this->geometry.height - 24}, {234, this->geometry.height - 24}};
  int ids[3] = {246, 218, 201};
  const char* tooltips[3] = {"", "", "/tip"};

  for (int index = 0; index < 3; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
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
  UICheckBox* checkBox = new UICheckBox();
  checkBox->SetSize(this->TextWidth(LookupMessage(MSI_SHOWTIPSATSTARTUP), 0, 0, 12, 0, 0) + 12, 12);
  checkBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_SHOWTIPSATSTARTUP));
  checkBox->Move(10, height - 20);
  checkBox->SetChecked(this->m_showTipsAtStartup);
  this->AddChild(checkBox);

  if (g_tipOfTheDay.Size() != 0) {
    const int random = std::rand();
    const int index = random % g_tipOfTheDay.Size();
    this->m_tipIndex = index;
    const StringList& tip = g_tipOfTheDay.GetTip(index);
    this->m_textViewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DIDYOUKNOW)), 38, 0, 0);
    for (unsigned line = 0; line < tip.size(); ++line) {
      this->m_textViewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(tip[line].c_str()), 38, 0, 0);
    }
  }
}

// OFFSET: 0x008E9C90
// FUNCTION: fn_12204
void UITipboxWnd::OnCreate(int width, int) {
  this->showAtStartup = g_window315Enabled;
  this->m_message0 = 365;

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);

  this->tipViewer = new UITagTextViewer();
  this->tipViewer->SetSize(226, 245);
  this->tipViewer->m_backgroundColor = 0xFF000000u | RGB(red, green, blue);
  this->tipViewer->Invalidate();
  this->tipViewer->m_textWidth = 188;
  this->tipViewer->Move(250, 70);
  this->AddChild(this->tipViewer);
  this->tipViewer->Show(1);

  UIStaticText** titles = &this->titleText;
  for (int y = 50; y < 90; y += 20) {
    *titles = new UIStaticText();
    (*titles)->SetSize(220, 18);
    (*titles)->SetText("", 0);
    (*titles)->SetFont(0, 13, 1, 0);
    (*titles)->SetColor(44, 77, 160);
    (*titles)->Move(254, y);
    this->AddChild(*titles);
    ++titles;
  }

  TipBoxEntry& entry = g_itemTagOwner->SelectRandomTip();

  this->m_pageText = new UIStaticText();
  this->m_pageText->SetSize(150, 20);
  this->m_pageText->SetText("1", 0);
  this->m_pageText->SetFont(0, 12, 1, 0);
  this->m_pageText->SetColor(44, 77, 160);
  this->AddChild(this->m_pageText);

  const std::string separator = "/";
  this->m_pageTotalText = new UIStaticText();
  this->m_pageTotalText->SetSize(90, 15);
  this->m_pageTotalText->SetText((separator + IntToString(static_cast<int>(entry.paragraphs.size()), 0)).c_str(), 0);
  this->m_pageTotalText->SetFont(0, 12, 0, 0);
  this->m_pageTotalText->SetColor(44, 77, 160);
  this->AddChild(this->m_pageTotalText);

  UIBitmapButton* searchButton = new UIBitmapButton();
  searchButton->SetBitmap("유저인터페이스\\tipbox\\btn_searchbar_normal.bmp", 0, 0);
  searchButton->SetBitmap("유저인터페이스\\tipbox\\btn_searchbar_over.bmp", 1, 0);
  searchButton->SetBitmap("유저인터페이스\\tipbox\\btn_searchbar_press.bmp", 2, 0);
  searchButton->SetSize(searchButton->BitmapWidth(), searchButton->BitmapHeight());
  searchButton->Move(448, 21);
  searchButton->SetId(365);
  this->AddChild(searchButton);

  UICheckBox* checkBox = new UICheckBox();
  checkBox->SetSize(this->TextWidth(LookupMessage(MSI_ZEROCHATWND_RESIZE), 0, 0, 12, 0, 0) + 12, 14);
  checkBox->SetImages("유저인터페이스\\tipbox\\checkBox_check.bmp", "유저인터페이스\\tipbox\\checkBox.bmp", LookupMessage(MSI_ZEROCHATWND_RESIZE));
  checkBox->Move(9, 347);
  checkBox->SetChecked(this->showAtStartup);
  checkBox->SetId(213);
  this->AddChild(checkBox);

  UIBmpTextButton* closeButton = new UIBmpTextButton();
  closeButton->SetBitmap("유저인터페이스\\tipbox\\btn_close_normal.bmp", 0, 0);
  closeButton->SetBitmap("유저인터페이스\\tipbox\\btn_close_over.bmp", 1, 0);
  closeButton->SetBitmap("유저인터페이스\\tipbox\\btn_close_press.bmp", 2, 0);
  closeButton->SetSize(closeButton->BitmapWidth(), closeButton->BitmapHeight());
  closeButton->SetText(LookupMessage(MSI_BOOK_CLOSE));
  closeButton->Move(415, 341);
  closeButton->SetId(201);
  this->AddChild(closeButton);

  UIEditCtrl4* search = new UIEditCtrl4();
  this->searchEdit = search;
  search->SetSize(193, 15);
  search->maximum = 150;
  search->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
  search->SetPlaceholderEnabled(true);
  search->SetPlaceholder(LookupMessage(MSI_ENTER_YOUR_AUTHENTICATION_NUMBER));
  search->SetPlaceholderColor(0xB1B1B1);
  search->Move(253, 22);
  search->Background(255, 255, 255);
  this->AddChild(this->searchEdit);

  this->previousButton = new UIBitmapButton();
  this->previousButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowL_on.bmp", 0, 0);
  this->previousButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowL_on.bmp", 1, 0);
  this->previousButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowL_on.bmp", 2, 0);
  this->previousButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowL_off.bmp", 3, 0);
  this->previousButton->SetSize(this->previousButton->BitmapWidth(), this->previousButton->BitmapHeight());
  this->previousButton->Move(415, 319);
  this->previousButton->SetId(246);
  this->AddChild(this->previousButton);

  this->nextButton = new UIBitmapButton();
  this->nextButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowR_on.bmp", 0, 0);
  this->nextButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowR_on.bmp", 1, 0);
  this->nextButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowR_on.bmp", 2, 0);
  this->nextButton->SetBitmap("유저인터페이스\\tipbox\\bt_arrowR_off.bmp", 3, 0);
  this->nextButton->SetSize(this->nextButton->BitmapWidth(), this->nextButton->BitmapHeight());
  this->nextButton->Move(462, 319);
  this->nextButton->SetId(218);
  this->AddChild(this->nextButton);

  this->searchResults = new UIListBoxID();
  this->searchResults->Configure(250, 50, 216, 266, 0);
  int* background = reinterpret_cast<int*>(this->searchResults->backgroundChannels);
  background[0] = 240;
  background[1] = 240;
  background[2] = 240;
  this->searchResults->SetId(184);
  this->AddChild(this->searchResults);
  this->searchResults->Show(0);

  this->tagButton = new UIBitmapButton();
  this->tagButton->SetBitmap("유저인터페이스\\tipbox\\bt_tagcatch_normal.bmp", 0, 0);
  this->tagButton->SetBitmap("유저인터페이스\\tipbox\\bt_tagcatch_mouseon.bmp", 1, 0);
  this->tagButton->SetBitmap("유저인터페이스\\tipbox\\bt_tagcatch_pressed.bmp", 2, 0);
  this->tagButton->SetBitmap("유저인터페이스\\tipbox\\bt_tagcatch_disabled.bmp", 3, 0);
  this->tagButton->SetSize(this->tagButton->BitmapWidth(), this->tagButton->BitmapHeight());
  this->tagButton->Move(450, 47);
  this->tagButton->SetId(517);
  this->AddChild(this->tagButton);

  this->AddCloseButton("", 0)->Move(width - 15, 3);
  this->RefreshTip();

  if (entry.paragraphs.size() == 1) {
    this->nextButton->SetDisabled(1);
    this->nextButton->Invalidate();
    this->nextButton->Invalidate();
  }
  this->previousButton->SetDisabled(1);
  this->previousButton->Invalidate();
  this->previousButton->Invalidate();
}

// OFFSET: 0x008F1430
// FUNCTION: fn_12254
void UIStorePasswordInputWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_STORE_PASSWORD_INPUT), 0);
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

  this->DrawEmboldenedText(110, 27, LookupMessage(MSI_STORE_PASSWORD), textColor, shadowColor, 0, 0, 12);
  this->FillRegion(110, 40, 100, 16, boxColor);
  this->FillRegion(111, 41, 98, 14, 0xFFFFFF);
  if (!g_session.m_passwordInputAlternate && this->isChangingPassword == 0) {
    this->DrawEmboldenedText(110, 63, LookupMessage(MSI_STORE_NEW_PASSWORD), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(110, 78, 100, 16, 0xE6E6E6);
    this->DrawEmboldenedText(110, 103, LookupMessage(MSI_STORE_RENEW_PASSWORD), 0xF0F0F0, 0xE6E6E6, 0, 0, 12);
    this->FillRegion(110, 117, 100, 16, 0xE6E6E6);
  } else {
    this->DrawEmboldenedText(110, 63, LookupMessage(MSI_STORE_NEW_PASSWORD), 0xE6E6E6, 0, 0, 0, 12);
    this->FillRegion(110, 78, 100, 16, 0xFAE6C8);
    this->DrawEmboldenedText(110, 103, LookupMessage(MSI_STORE_RENEW_PASSWORD), 0xE6E6E6, 0, 0, 0, 12);
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
    keypadButtons[index]->Move(positions[digits[index]][0], positions[digits[index]][1]);
  Invalidate();
}

namespace {
#pragma pack(push, 1)
struct SecondPasswordPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int32_t accountId;
  /* 0006 */ char password[4];
};

struct SecondPasswordChangePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int32_t accountId;
  /* 0006 */ char password[4];
  /* 000A */ char newPassword[4];
};
#pragma pack(pop)
}

class UISecondPasswordEditWnd : public UISecondPasswordInputWnd {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

// OFFSET: 0x00903230
// FUNCTION: fn_12377
int UISecondPasswordInputWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_COMMAND) {
    switch (a) {
    case 419:
      if (std::strlen(this->currentPassword) == static_cast<std::size_t>(this->passwordLength)) {
        std::atoi(this->currentPassword);
        SecondPasswordPacket packet;
        packet.packetType = ID_CH_NOT_AVAILABLE_SECOND_PASSWD;
        packet.accountId = g_session.m_passwordWndAccountId;
        std::memcpy(packet.password, this->currentPassword, sizeof(packet.password));
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        this->DisableButtons();
        break;
      }
      g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
      this->passwordStage = 0;
      this->activeInputY = this->currentPasswordY;
      this->inputDigitCount = 0;
      this->maskTextX = 0;
      std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
      std::memset(this->newPassword, 0, sizeof(this->newPassword));
      std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
      break;
    case ID_CANCEL:
      if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
      }
      break;

    case ID_OK:
    case ID_NEXT:
      switch (this->passwordStage) {
      case 0:
        if (std::strlen(this->currentPassword) == static_cast<std::size_t>(this->passwordLength)) {
        std::atoi(this->currentPassword);
        SecondPasswordPacket packet;
        packet.packetType = ID_CH_SECOND_PASSWD_ACK;
        packet.accountId = g_session.m_passwordWndAccountId;
        std::memcpy(packet.password, this->currentPassword, sizeof(packet.password));
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        this->DisableButtons();
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 0;
        this->activeInputY = this->currentPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->actionButtons[0]->Enable(0);
        this->actionButtons[3]->Enable(1);
        this->Invalidate();
        break;

      case 1:
        if (std::strlen(this->currentPassword) == static_cast<std::size_t>(this->passwordLength)) {
          this->passwordStage = 2;
          this->activeInputY = this->newPasswordY;
          this->inputDigitCount = 0;
          this->maskTextX = 0;
          std::memset(this->newPassword, 0, sizeof(this->newPassword));
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        this->activeInputY = this->currentPasswordY;
        this->passwordStage = 1;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->Invalidate();
        break;

      case 2:
        if (std::strlen(this->newPassword) == static_cast<std::size_t>(this->passwordLength)) {
          this->passwordStage = 3;
          this->activeInputY = this->confirmPasswordY;
          this->inputDigitCount = 0;
          this->maskTextX = 0;
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->FillRegion(415, 167, 98, 14, 0xFFFFFF);
          this->actionButtons[0]->Enable(1);
          this->actionButtons[3]->Enable(0);
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 2;
        this->activeInputY = this->newPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
        this->Invalidate();
        break;

      case 3:
        if (std::strlen(this->confirmPassword) == static_cast<std::size_t>(this->passwordLength)) {
          if (std::strcmp(this->newPassword, this->confirmPassword) == 0) {
            std::atoi(this->currentPassword);
            std::atoi(this->confirmPassword);
            SecondPasswordChangePacket packet;
            packet.packetType = ID_CH_EDIT_SECOND_PASSWD;
            packet.accountId = g_session.m_passwordWndAccountId;
            std::memcpy(packet.password, this->currentPassword, sizeof(packet.password));
            std::memcpy(packet.newPassword, this->confirmPassword, sizeof(packet.newPassword));
            GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
            this->DisableButtons();
          } else {
            g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_MISMATCH), 0, true, false, 0, "Message", 280, 120, 0);
            this->passwordStage = 2;
            this->activeInputY = this->newPasswordY;
            this->inputDigitCount = 0;
            this->maskTextX = 0;
            std::memset(this->newPassword, 0, sizeof(this->newPassword));
            std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
            this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
            this->FillRegion(414, 166, 100, 16, 0xFAE6C8);
            this->actionButtons[0]->Enable(0);
            this->actionButtons[3]->Enable(1);
          }
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 3;
        this->activeInputY = this->confirmPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->FillRegion(415, 167, 98, 14, 0xFFFFFF);
        this->Invalidate();
        break;
      }
      break;

    case ID_CHANGE_STORE_PASSWORD:
      g_session.m_storePasswordMode = 2;
      this->isChangingPassword = 1;
      this->passwordStage = 1;
      this->activeInputY = this->currentPasswordY;
      this->inputDigitCount = 0;
      this->maskTextX = 0;
      std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
      std::memset(this->newPassword, 0, sizeof(this->newPassword));
      std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
      this->actionButtons[0]->Enable(0);
      this->actionButtons[3]->Enable(1);
      this->actionButtons[1]->SetDisabled(1);
      this->actionButtons[1]->Invalidate();
      this->actionButtons[2]->SetDisabled(1);
      this->actionButtons[2]->Invalidate();
      this->Invalidate();
      break;

    case ID_STORE_PASSWORD_NUM1:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(1), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(1), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(1), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_RETURN:
      this->SendMsg(this, UM_COMMAND, ID_OK, 0, 0, 0);
      break;

    case ID_STORE_PASSWORD_NUM0:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(0), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(0), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(0), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM2:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(2), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(2), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(2), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM3:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(3), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(3), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(3), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM4:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(4), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(4), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(4), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM5:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(5), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(5), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(5), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM6:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(6), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(6), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(6), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM7:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(7), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(7), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(7), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM8:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(8), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(8), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(8), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM9:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(9), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(9), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(9), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM_RESET:
      if (this->passwordStage == 0) {
        this->passwordStage = 0;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        this->activeInputY = this->currentPasswordY;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->actionButtons[0]->Enable(1);
        this->actionButtons[3]->Enable(0);
      } else {
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        if (g_session.m_passwordInputAlternate) {
          this->passwordStage = 2;
          this->activeInputY = this->newPasswordY;
          std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
          std::memset(this->newPassword, 0, sizeof(this->newPassword));
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->actionButtons[0]->Enable(1);
          this->actionButtons[3]->Enable(0);
        } else {
          this->passwordStage = 1;
          this->activeInputY = this->currentPasswordY;
          std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
          std::memset(this->newPassword, 0, sizeof(this->newPassword));
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->actionButtons[0]->Enable(0);
          this->actionButtons[3]->Enable(1);
        }
      }
      this->ShuffleKeypad();
      break;

    }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00901D40
// FUNCTION: fn_12376
int UISecondPasswordEditWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_COMMAND) {
    switch (a) {
    case 419:
      if (std::strlen(this->currentPassword) == static_cast<std::size_t>(this->passwordLength)) {
        std::atoi(this->currentPassword);
        SecondPasswordPacket packet;
        packet.packetType = ID_CH_NOT_AVAILABLE_SECOND_PASSWD;
        packet.accountId = g_session.m_passwordWndAccountId;
        std::memcpy(packet.password, this->currentPassword, sizeof(packet.password));
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        this->DisableButtons();
        break;
      }
      g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
      this->passwordStage = 0;
      this->activeInputY = this->currentPasswordY;
      this->inputDigitCount = 0;
      this->maskTextX = 0;
      std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
      std::memset(this->newPassword, 0, sizeof(this->newPassword));
      std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
      break;
    case ID_CANCEL:
      if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
      }
      break;

    case ID_OK:
    case ID_NEXT:
      switch (this->passwordStage) {
      case 0:
        if (std::strlen(this->currentPassword) == static_cast<std::size_t>(this->passwordLength)) {
        std::atoi(this->currentPassword);
        SecondPasswordPacket packet;
        packet.packetType = ID_CH_DELETE_SECOND_PASSWD;
        packet.accountId = g_session.m_passwordWndAccountId;
        std::memcpy(packet.password, this->currentPassword, sizeof(packet.password));
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        this->DisableButtons();
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 0;
        this->activeInputY = this->currentPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->actionButtons[0]->Enable(0);
        this->actionButtons[3]->Enable(1);
        break;

      case 1:
        this->passwordStage = 2;
        this->activeInputY = this->newPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
        break;

      case 2:
        if (std::strlen(this->newPassword) == static_cast<std::size_t>(this->passwordLength)) {
          this->passwordStage = 3;
          this->activeInputY = this->confirmPasswordY;
          this->inputDigitCount = 0;
          this->maskTextX = 0;
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->FillRegion(415, 167, 98, 14, 0xFFFFFF);
          this->actionButtons[0]->Enable(1);
          this->actionButtons[3]->Enable(0);
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 2;
        this->activeInputY = this->newPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
        break;

      case 3:
        if (std::strlen(this->confirmPassword) == static_cast<std::size_t>(this->passwordLength)) {
          if (std::strcmp(this->newPassword, this->confirmPassword) == 0) {
            std::atoi(this->currentPassword);
            std::atoi(this->confirmPassword);
            SecondPasswordPacket packet;
            packet.packetType = ID_CH_MAKE_SECOND_PASSWD;
            packet.accountId = g_session.m_passwordWndAccountId;
            std::memcpy(packet.password, this->confirmPassword, sizeof(packet.password));
            GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
            this->DisableButtons();
            break;
          }
          g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_MISMATCH), 0, true, false, 0, "Message", 280, 120, 0);
          this->passwordStage = 2;
          this->activeInputY = this->newPasswordY;
          this->inputDigitCount = 0;
          this->maskTextX = 0;
          std::memset(this->newPassword, 0, sizeof(this->newPassword));
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->FillRegion(415, 128, 98, 14, 0xFFFFFF);
          this->FillRegion(414, 166, 100, 16, 0xFAE6C8);
          this->actionButtons[0]->Enable(0);
          this->actionButtons[3]->Enable(1);
          break;
        }
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        this->passwordStage = 3;
        this->activeInputY = this->confirmPasswordY;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->FillRegion(415, 167, 98, 14, 0xFFFFFF);
        break;
      }
      break;

    case ID_CHANGE_STORE_PASSWORD:
      g_session.m_storePasswordMode = 2;
      this->isChangingPassword = 1;
      this->passwordStage = 1;
      this->activeInputY = this->currentPasswordY;
      this->inputDigitCount = 0;
      this->maskTextX = 0;
      std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
      std::memset(this->newPassword, 0, sizeof(this->newPassword));
      std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
      this->actionButtons[0]->Enable(0);
      this->actionButtons[3]->Enable(1);
      this->actionButtons[1]->SetDisabled(1);
      this->actionButtons[1]->Invalidate();
      this->actionButtons[2]->SetDisabled(1);
      this->actionButtons[2]->Invalidate();
      this->Invalidate();
      break;

    case ID_STORE_PASSWORD_NUM1:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(1), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(1), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(1), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_RETURN:
      this->SendMsg(this, UM_COMMAND, ID_OK, 0, 0, 0);
      break;

    case ID_STORE_PASSWORD_NUM0:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(0), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(0), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(0), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM2:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(2), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(2), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(2), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM3:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(3), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(3), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(3), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM4:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(4), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(4), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(4), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM5:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(5), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(5), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(5), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM6:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(6), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(6), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(6), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM7:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(7), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(7), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(7), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM8:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(8), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(8), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(8), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;
    case ID_STORE_PASSWORD_NUM9:
      if (++this->inputDigitCount > this->passwordLength) {
        g_windowMgr.Confirm(LookupMessage(MSI_STORE_PASSWORD_4_6), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      switch (this->passwordStage) {
      case 0:
      case 1:
        std::strcat(this->currentPassword, IntToString(this->FindKeypadDigit(9), 0).c_str());
        break;
      case 2:
        std::strcat(this->newPassword, IntToString(this->FindKeypadDigit(9), 0).c_str());
        break;
      case 3:
        std::strcat(this->confirmPassword, IntToString(this->FindKeypadDigit(9), 0).c_str());
        break;
      }
      this->DrawTextAt(this->maskTextX + 416, this->activeInputY, "*", 0, 0, 12, 0, 0, 0);
      this->maskTextX += 5;
      break;

    case ID_STORE_PASSWORD_NUM_RESET:
      if (this->passwordStage == 0) {
        this->passwordStage = 0;
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        this->activeInputY = this->currentPasswordY;
        std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
        std::memset(this->newPassword, 0, sizeof(this->newPassword));
        std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
        this->actionButtons[0]->Enable(1);
        this->actionButtons[3]->Enable(0);
      } else {
        this->inputDigitCount = 0;
        this->maskTextX = 0;
        if (g_session.m_passwordInputAlternate) {
          this->passwordStage = 2;
          this->activeInputY = this->newPasswordY;
          std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
          std::memset(this->newPassword, 0, sizeof(this->newPassword));
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->actionButtons[0]->Enable(1);
          this->actionButtons[3]->Enable(0);
        } else {
          this->passwordStage = 1;
          this->activeInputY = this->currentPasswordY;
          std::memset(this->currentPassword, 0, sizeof(this->currentPassword));
          std::memset(this->newPassword, 0, sizeof(this->newPassword));
          std::memset(this->confirmPassword, 0, sizeof(this->confirmPassword));
          this->actionButtons[0]->Enable(0);
          this->actionButtons[3]->Enable(1);
        }
      }
      this->ShuffleKeypad();
      break;

    }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009099E0
// FUNCTION: fn_12395
void UIEquipWnd::RefreshLook()
{
  if (m_viewOther == 0) {
    appearance[0] = g_session.GetDisplayJob();
    appearance[1] = g_session.GetCurrentSex();
    appearance[2] = g_session.m_head;
    appearance[6] = g_session.m_bodyStyle;
    appearance[7] = g_session.m_headPalette;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->scene && mode->scene->player)
      costumeJob = mode->scene->player->GetBodyCostume();
  } else {
    appearance[0] = g_equipWindowLook.job;
    appearance[1] = g_equipWindowLook.sex;
    appearance[2] = g_equipWindowLook.head;
    appearance[6] = g_equipWindowLook.bodyPalette;
    appearance[7] = g_equipWindowLook.headPalette;
    costumeJob = g_equipWindowLook.bodyCostume;
  }
  appearance[3] = 0;
  appearance[4] = 0;
  appearance[5] = 0;
  robeId = 0;
  ITEM_INFO middle = GetSlotItem(8);
  if (middle.identity[1] != 0)
    appearance[4] = middle.sprite;
  ITEM_INFO lower = GetSlotItem(9);
  if (lower.identity[1] != 0 && middle.identity[1] != lower.identity[1])
    appearance[5] = lower.sprite;
  ITEM_INFO upper = GetSlotItem(0);
  if (upper.identity[1] != 0 && middle.identity[1] != upper.identity[1] && lower.identity[1] != upper.identity[1])
    appearance[3] = upper.sprite;
  ITEM_INFO robe(0);
  robe = GetSlotItem(2);
  if (robe.identity[1] != 0)
    robeId = robe.sprite;
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
      segmentBuffer[segment.size()] = '\0';
      std::strcat(result, segmentBuffer);
    }
  }
  return result;
}

// OFFSET: 0x008F4AC0
// FUNCTION: fn_12293
void UIBookWnd::OnProcess() {
  if (!this->m_autoRead || this->m_autoTick == 0 || timeGetTime() <= this->m_autoTick + 4000) {
    return;
  }
  int line = this->m_autoLine;
  if (line >= this->m_lineCount) {
    this->m_autoTick = 0;
    if (this->m_balloon != nullptr) {
      g_windowMgr.Schedule(this->m_balloon);
      this->m_balloon = nullptr;
    }
    g_windowMgr.CloseWindow(WID_BOOKWND);
    return;
  }
  const char* text = this->m_lines[line].c_str();
  if ((std::strlen(text) == 0 || std::strcmp(text, " ") == 0) && line < this->m_lineCount - 1) {
    this->m_autoLine = line + 1;
  }
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(this->StripColorCodes(this->m_lines[this->m_autoLine].c_str())), 0xE1E4FF, 0, 0, 0);
  UIWindow* balloon = this->m_balloon;
  if (balloon == nullptr) {
    balloon = new UITransBalloonText();
    this->m_balloon = balloon;
    balloon->SetSize(8, 8);
    g_windowMgr.AddRenderedFront(this->m_balloon);
    balloon = this->m_balloon;
  }
  static_cast<UITransBalloonText*>(balloon)->SetText(this->StripColorCodes(this->m_lines[this->m_autoLine].c_str()), 100);
  this->m_balloon->Move((g_renderer->viewport.width - 640) / 2 - this->m_balloon->geometry.width / 2 + 320, 81);
  static_cast<UITransBalloonText*>(this->m_balloon)->m_textColor = 0xE1E4FF;
  static_cast<UITransBalloonText*>(this->m_balloon)->m_outlineColor = 0;
  ++this->m_autoLine;
  this->m_autoTick = timeGetTime();
}

int ParseItemIdTag(const char* text, const char* end, int& length, int& itemId);

// OFFSET: 0x008F2DF0
// FUNCTION: fn_12262
void UITipboxWnd::PaintAnimations() {
  std::string path;
  for (unsigned int i = 0; i < this->m_animations.size(); ++i) {
    path = "유저인터페이스\\tipbox\\ico_effect" + IntToString(this->m_animations[i].kind, 0) + ".bmp";
    if (this->m_animations[i].visible) {
      auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
      this->DrawBitmap(
        static_cast<int>(this->m_animations[i].x), static_cast<int>(this->m_animations[i].y), bitmap, true
      );
    }
  }

  for (unsigned int i = 0; i < this->m_dots.size(); ++i) {
    if (this->m_dots[i].visible) {
      auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\tipbox\\ico_effect6.bmp")));
      this->DrawBitmap(static_cast<int>(this->m_dots[i].x), static_cast<int>(this->m_dots[i].y), bitmap, true);
    }
  }
}

// INLINED
static __forceinline void AppendItemNameText(char*& cursor, std::size_t& remaining, const char* text) {
  const int error = strncpy_s(cursor, remaining, text, _TRUNCATE);
  if (error == 0 || error == STRUNCATE) {
    const std::size_t bytes = strnlen_s(cursor, remaining);
    cursor += bytes;
    remaining -= bytes;
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

  if (item->identified) {
    const int* cards = reinterpret_cast<const int*>(item->cards);
    if (item->identity[0] == 4 && item->IsApprenticeItem() == true && cards[3] == 1) {
      *cursor += item->Apprentice(*cursor, capacity, 1);
      breaks->push_back(*cursor);
    }

    if (cards[0] == 254 && cards[1] == 0) {
      if (item->refiningLevel > 0) {
        *cursor += item->Refine(*cursor, capacity, 1);
        breaks->push_back(*cursor);
      }
      if (0 < item->grade) {
        *cursor += item->Grade(*cursor, capacity, 1);
        breaks->push_back(*cursor);
      }

      if (g_serviceType != 12 && g_serviceType != 14) {
        const int bytes = item->Creator(*cursor, capacity, 1);
        if (bytes) {
          *creator = *cursor;
          *cursor += bytes;
          *creatorColor = 0xE04040;
        } else {
          g_windowMgr.RegisterWindow(this);
          AppendItemNameText(*cursor, *remaining, LookupMessage(MSI_NAMELESS_USER));
          *creatorColor = 0x4040E0;
        }
        breaks->push_back(*cursor);
        *cursor += item->Element(*cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
        breaks->push_back(*cursor);
      } else {
        *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += item->Element(*cursor, capacity, 1);
        breaks->push_back(*cursor);
        const int bytes = item->Creator(*cursor, capacity, 1);
        if (bytes) {
          *creator = *cursor;
          *cursor += bytes;
          *creatorColor = 0xE04040;
        } else {
          g_windowMgr.RegisterWindow(this);
          AppendItemNameText(*cursor, *remaining, LookupMessage(MSI_NAMELESS_USER));
          *creatorColor = 0x4040E0;
        }
        breaks->push_back(*cursor);
      }
    } else if (Equipment(*item)) {
      if (item->refiningLevel > 0) {
        *cursor += item->Refine(*cursor, capacity, 1);
        breaks->push_back(*cursor);
      }
      if (0 < item->grade) {
        *cursor += item->Grade(*cursor, capacity, 1);
        breaks->push_back(*cursor);
      }

      if (item->HasCards()) {
        *cursor += item->Prefixes(*cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
        breaks->push_back(*cursor);
        *cursor += item->Suffixes(*cursor, capacity, 1);
        breaks->push_back(*cursor);
        if (withSlots == 1) {
          *cursor += item->Slots(*cursor, capacity, 1);
          breaks->push_back(*cursor);
        }
      } else if (item->HasSpecialCard()) {
        if (g_serviceType == 12) {
          *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *cursor += item->Element(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *cursor += item->Star(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *creator = *cursor;
          const int bytes = item->Creator(*cursor, capacity, 1);
          if (bytes) {
            *creator = *cursor;
            *cursor += bytes;
            *creatorColor = 0xE04040;
          } else {
            g_windowMgr.RegisterWindow(this);
            AppendItemNameText(*cursor, *remaining, LookupMessage(MSI_NAMELESS_USER));
            *creatorColor = 0x4040E0;
          }
          breaks->push_back(*cursor);
          if (splitCreator) {
            breaks->push_back(*cursor);
          }
        } else if (g_serviceType == 14) {
          *cursor += item->Star(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *cursor += item->Element(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *creator = *cursor;
          const int bytes = item->Creator(*cursor, capacity, 1);
          if (bytes) {
            *creator = *cursor;
            *cursor += bytes;
            *creatorColor = 0xE04040;
          } else {
            g_windowMgr.RegisterWindow(this);
            AppendItemNameText(*cursor, *remaining, LookupMessage(MSI_NAMELESS_USER));
            *creatorColor = 0x4040E0;
          }
          breaks->push_back(*cursor);
          if (splitCreator) {
            breaks->push_back(*cursor);
          }
        } else {
          *cursor += item->Star(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *creator = *cursor;
          const int bytes = item->Creator(*cursor, capacity, 1);
          if (bytes) {
            *creator = *cursor;
            *cursor += bytes;
            *creatorColor = 0xE04040;
          } else {
            g_windowMgr.RegisterWindow(this);
            AppendItemNameText(*cursor, *remaining, LookupMessage(MSI_NAMELESS_USER));
          }
          breaks->push_back(*cursor);
          if (splitCreator) {
            breaks->push_back(*cursor);
          }
          *cursor += item->Element(*cursor, capacity, 1);
          breaks->push_back(*cursor);
          *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
          breaks->push_back(*cursor);
        }
      } else {
        *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
        breaks->push_back(*cursor);
        if (withSlots == 1) {
          *cursor += item->Slots(*cursor, capacity, 1);
          breaks->push_back(*cursor);
        }
      }
    } else {
      *cursor += item->GetIdDisplayName(*cursor, capacity, 1);
      breaks->push_back(*cursor);
      if (withSlots == 1) {
        *cursor += item->Slots(*cursor, capacity, 1);
        breaks->push_back(*cursor);
      }
    }
  } else {
    *cursor += ItemUnidentifiedName(*item, *cursor, capacity, 1);
    breaks->push_back(*cursor);
  }
  return 1;
}

// OFFSET: 0x008D9420
// FUNCTION: fn_12133
bool UIItemCollectionWnd::WrapText(std::string text, int width, std::list<std::string>& lines) {
  if (this->TextWidth(text.c_str(), 0, 0, 12, 0, 0) <= width) {
    lines.push_back(text);
    return false;
  }

  const char* start = text.c_str();
  const char* cursor = start;
  while (cursor != text.c_str() + text.size()) {
    if (*cursor == '\n') {
      lines.push_back(std::string(start, cursor));
      ++cursor;
      start = cursor;
    } else if (cursor[0] < 0 && cursor[1] < 0) {
      const std::string candidate(start, cursor + 2);
      if (this->TextWidth(candidate.c_str(), 0, 0, 12, 0, 0) > width) {
        lines.push_back(std::string(start, cursor));
        start = cursor;
      }
      cursor += 2;
    } else {
      const std::string candidate(start, cursor + 1);
      if (this->TextWidth(candidate.c_str(), 0, 0, 12, 0, 0) > width) {
        lines.push_back(std::string(start, cursor));
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
  if (g_windowMgr.rodexComposeWnd != nullptr) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UNEQUIP_ARROW2)), 0x64FFFF, 0, 0, 0);
    return 0;
  }

  switch (message) {
  case UM_DOWNPUSH:
    this->m_list->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    return 0;
  case UM_SETINFO: {
    ChooseWndInfo* placement = reinterpret_cast<ChooseWndInfo*>(a);
    this->m_placement = placement;
    placement->IsValid();
    this->Move(placement->x, placement->y);
    return 0;
  }
  case UM_UPPUSH:
    this->m_list->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
    return 0;
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_OK: {
      const std::uint32_t tick = GetTickCount();
      if (tick - *reinterpret_cast<const DWORD*>(&g_session.itemSendState[0x04]) >= 300) {
        *reinterpret_cast<DWORD*>(&g_session.itemSendState[0x04]) = tick;
        if (this->m_items.empty()) {
          this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
          return 0;
        }
        const int selected = this->m_list->selected;
        if (selected != -1) {
          g_modeMgr.GetGameMode()->SendMsg(MM_CHOOSE_MENU, this->m_items[selected] + 1, this->m_npcAid, 0, 0);
          g_windowMgr.CloseWindow(WID_CHOOSEWND);
        }
      }
      break;
    }
    case ID_CANCEL:
      g_modeMgr.GetGameMode()->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      break;
    }
    return 0;
  }
  case UM_SETCHOOSE:
    if (a != 0) {
      const char* value = reinterpret_cast<const char*>(a);
      if (std::strstr(value, LookupMessage(MSI_REF_UNEQUIP)) == nullptr) {
        std::string text(value);
        int length = 0;
        int itemId = 0;
        if (ParseItemIdTag(value, value + std::strlen(value), length, itemId) == 1 && itemId > 0) {
          text = FindItemDescription(itemId).identifiedName;
        }
        this->m_list->AddLine(text.c_str());
        ++this->m_lastItemIndex;
        this->m_items.push_back(this->m_lastItemIndex);
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
  case UM_SETNAID:
    this->m_npcAid = static_cast<int>(a);
    return 0;
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* records = reinterpret_cast<const unsigned char*>(a);
    FramedMessageHeader header;
    std::memcpy(&header, records, sizeof(header));
    if (header.type == 0x558C) {
      unsigned int position = sizeof(header);
      bool done = false;
      while (!done && position < static_cast<unsigned int>(b)) {
        std::memcpy(&header, records + position, sizeof(header));
        position += sizeof(header);
        switch (header.type) {
        case 0x558D:
          done = true;
          break;
        case 0x558F: {
          char text[260] = {};
          std::memcpy(text, records + position, header.length);
          position += header.length;
          this->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
          break;
        }
        default:
          position += header.length;
          break;
        }
      }
      if (g_windowMgr.sayDialogWnd != nullptr) {
        g_windowMgr.sayDialogWnd->SendMsg(nullptr, UM_DISABLENEXTBUTTON, 0, 0, 0, 0);
      }
    }
    return 0;
  }
  case UM_CANCEL_NPC_CHOICE:
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(MM_CHOOSE_MENU, -1, this->m_npcAid, 0, 0);
    }
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008F8640
// FUNCTION: fn_12331
int UIChoose3Wnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const char* text = reinterpret_cast<const char*>(a);
  if (message != UM_SETCHOOSE || text == nullptr || std::strlen(text) == 0 || std::strcmp(text, LookupMessage(MSI_REF_UNEQUIP)) == 0) {
    return this->UIChooseWnd::SendMsg(sender, message, a, b, c, d);
  }
  int length = 0;
  int itemId = 0;
  const char* line = text;
  if (ParseItemIdTag(text, text + std::strlen(text), length, itemId) == 1 && itemId > 0) {
    line = FindItemDescription(itemId).identifiedName;
  }
  this->m_list->AddLine(line);
  ++this->m_lastItemIndex;
  this->m_items.push_back(this->m_lastItemIndex);
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
  this->searchResults->Clear();
  for (const int id : results) {
    const std::string& source = g_itemTagOwner->GetTip(id).title;
    std::string title(source.c_str(), source.size());
    const std::size_t separator = title.find('#');
    if (separator != std::string::npos) {
      title = title.substr(0, separator);
    }
    if (title.size() != 0) {
      this->searchResults->AddLine(title.c_str(), id);
    }
  }
  this->searchResults->Invalidate();
}

// OFFSET: 0x008D9FE0
// FUNCTION: fn_12140
void UITipboxWnd::RefreshTip() {
  this->UpdatePageText();
  this->BuildAnimations();
  this->searchResults->Show(0);

  std::string title(g_itemTagOwner->m_currentEntry.title.c_str(), g_itemTagOwner->m_currentEntry.title.size());
  this->TrimAt(title, "#");
  this->tagButton->Enable(g_itemTagOwner->m_currentEntry.searchable);

  std::vector<std::string> lines;
  WrapPlainText(title.c_str(), lines, 30);

  UIStaticText* const texts[2] = {this->titleText, this->subtitleText};
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
      this->tipViewer->Move(250, 70);
      height = 245;
    } else {
      this->tipViewer->Move(250, 90);
      height = 225;
    }
    this->tipViewer->Resize(226, height);
    this->refreshPending = true;
  }
}

// OFFSET: 0x00907050
// FUNCTION: fn_12386
int UITipboxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  switch (a) {
  case ID_CHECK: {
    DWORD disposition = 0;
    this->showAtStartup = static_cast<int>(b);
    g_window315Enabled = static_cast<int>(b);
    HKEY key;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
      RegSetValueExA(key, "SHOWTIPSATSTARTUP", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->showAtStartup), 4);
      RegCloseKey(key);
    }
    break;
  }
  case ID_NEXT: {
    const int page = (std::max)(0, g_itemTagOwner->paragraphIndex);
    g_itemTagOwner->paragraphIndex = page + 1;
    if (g_itemTagOwner->IsLastTip() == true) {
      this->nextButton->SetDisabled(1);
      this->nextButton->Invalidate();
      this->nextButton->Invalidate();
    }
    this->previousButton->SetDisabled(0);
    this->previousButton->Invalidate();
    this->previousButton->Invalidate();
    this->RefreshTip();
    break;
  }
  case ID_OK:
    this->previousButton->Enable(1);
    this->nextButton->Enable(1);
    g_itemTagOwner->SelectTip(this->searchResults->lines[this->searchResults->selected].id);
    this->nextButton->SetDisabled(g_itemTagOwner->IsLastTip() == true ? 1 : 0);
    this->nextButton->Invalidate();
    this->nextButton->Invalidate();
    this->previousButton->SetDisabled(1);
    this->previousButton->Invalidate();
    this->previousButton->Invalidate();
    this->RefreshTip();
    this->BuildAnimations();
    break;
  case ID_CLOSE:
    g_windowMgr.CloseWindow(WID_TIPBOXWND);
    break;
  case ID_PREV: {
    const int page = (std::max)(0, g_itemTagOwner->paragraphIndex);
    g_itemTagOwner->paragraphIndex = page - 1;
    this->nextButton->SetDisabled(0);
    this->nextButton->Invalidate();
    this->nextButton->Invalidate();
    if (page == 1) {
      this->previousButton->SetDisabled(1);
      this->previousButton->Invalidate();
      this->previousButton->Invalidate();
    }
    this->RefreshTip();
    break;
  }
  case 0x205:
    if (!g_windowMgr.GetHoverWindow()) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_RS_REFAILTARGETROUTE)), 0xFF, 0, 0, 0);
    } else {
      const int id = g_itemTagOwner->currentTipId;
      switch (g_windowMgr.GetHoverWindow()->geometry.id) {
      case 0x1EC:
        static_cast<UINpcTextEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->InsertSkillTag(id);
        break;
      case 0x1EB:
        static_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->InsertSkillTag(id);
        break;
      case 0x1EA:
      case 0x1ED:
      case 0x1EE:
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemTag(id);
        break;
      }
    }
    break;
  case 0x16D: {
    std::string keyword(this->searchEdit->Text());
    if (keyword.size() != 0) {
      this->searchResults->Show(1);
      this->searchResults->Clear();
      this->tipViewer->Clear();
      UIStaticText* const texts[2] = {this->titleText, this->subtitleText};
      for (int index = 0; index < 2; ++index) {
        texts[index]->SetText("", 0);
      }
      this->m_pageText->SetText("", 0);
      this->m_pageTotalText->SetText("", 0);
      this->previousButton->Enable(0);
      this->nextButton->Enable(0);
      this->searchEdit->SetText("");
      this->m_animations.clear();
      this->m_dots.clear();
      this->Search(keyword);
      this->tagButton->Enable(0);
    }
    break;
  }
  case 0x21F:
    this->nextButton->SetDisabled(g_itemTagOwner->IsLastTip() == true ? 1 : 0);
    this->nextButton->Invalidate();
    this->nextButton->Invalidate();
    this->previousButton->SetDisabled(1);
    this->previousButton->Invalidate();
    this->previousButton->Invalidate();
    this->RefreshTip();
    this->BuildAnimations();
    break;
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
      measured = this->TextWidth(begin, length, 0, 12, 0, 0);
      if (measured + lineWidth > width) {
        std::pair<int, std::string> piece;
        piece.second = segment->second.substr(offset, length);
        piece.first = segment->first;
        line->push_back(piece);
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
      std::pair<int, std::string> piece;
      piece.second = segment->second.substr(offset, length);
      piece.first = segment->first;
      line->push_back(piece);
    }
  }

  if (this->TextWidth(text, std::strlen(text), position->family, position->height, 0, 0) > width * 2) {
    this->FitLastTextLine(width, 2, position->family, position->height, lines);
  }

  {
    FrameTextKey key{color, static_cast<std::uint32_t>(width), std::string(text, std::strlen(text))};
    this->m_textCache[std::move(key)] = lines;
  }
  this->DrawTextLines(position->x, position->y, position->family, position->height, lines);
}

// OFFSET: 0x00900D90
// FUNCTION: fn_12372
int UIProhibitListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND: {
    if (a == ID_CLOSE) {
      g_windowMgr.CloseWindow(WID_PROHIBITLISTWND);
    } else if (a == 0x10C) {
      UIWindow* menu = g_windowMgr.EnsureWindow(WID_COMBOBOXWND);
      const std::string labels[4] = {LookupMessage(MSI_PET_FEEDING), LookupMessage(MSI_PET_PERFORMANCE), LookupMessage(MSI_PET_RETURN_EGG), LookupMessage(MSI_PET_ACC_OFF)};
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_APPEND_PROHIBIT)), 0, 0, 0);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ERASE_PROHIBIT)), 0, 0, 0);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REDUCE_PROHIBIT)), 0, 0, 0);
      menu->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
      menu->Resize(0x82, 0x30);
      menu->SendMsg(nullptr, UM_SET_CALLER_WID, 0x67, 0, 0, 0);
      int x = 0xA0;
      int y = 0x2C;
      this->ToScreen(x, y);
      menu->Move(x, y);
    }
  
    break;
  }
  case UM_SETINFO:
    this->m_aid = static_cast<std::uint32_t>(a);
  
    break;
  case UM_COMBOBOX_SELECTED: {
    CMode* mode = g_modeMgr.GetGameMode();
    int text;
    UICntWnd* counter;
    if (a == 0) {
      if (!IsMonsterJob(g_session.m_aid)) {
        return 0;
      }
      counter = new UICntWnd();
      counter->SetSize(0xB6, 0x2E);
      counter->Move((g_renderer->viewport.width - 640) / 2 + 200, ScaleByScreenHeight(200));
      g_windowMgr.AddRendered(counter);
      counter->SendMsg(nullptr, UM_SET_TARGET, 0x98, 0, 0, 0);
      text = 0x186;
    } else if (a == 1) {
      if (IsAdminAccount(g_session.m_aid)) {
        mode->SendMsg(MM_REQ_GIVE_MANNER_POINT_PLUS, 30000, 0, 0, 0);
      }
      return 0;
    } else {
      if (a != 2 || !IsAdminAccount(g_session.m_aid)) {
        return 0;
      }
      counter = new UICntWnd();
      counter->SetSize(0xB6, 0x2E);
      counter->Move((g_renderer->viewport.width - 640) / 2 + 200, ScaleByScreenHeight(200));
      g_windowMgr.AddRendered(counter);
      counter->SendMsg(nullptr, UM_SET_TARGET, 0x97, 0, 0, 0);
      text = 0x185;
    }
    counter->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(text)), 0, 0, 0);
    counter->SendMsg(nullptr, UM_SET_COUNT, 1, 0, 0, 0);
  
    break;
  }
  }
  return 0;
}

// OFFSET: 0x008ED760
// FUNCTION: fn_12222
void UIExchangeWnd::Paint() {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\titlebar_fix2.bmp")));
  this->DrawBitmap(0, 0, bitmap, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\exchange_bg2.bmp")));
  this->DrawBitmap(0, 17, bitmap, false);
  DrawOutlinedText(*this, 123, this->geometry.height - 20, LookupMessage(MSI_SCREENSHOT_FOR_EXCHANGE), 0, 0xFF, 0xFFFFFF, 0, 12, 1, 0);

  char title[128];
  std::sprintf(title, "%s : ", LookupMessage(MSI_EXCHANGE));
  this->DrawTextAt(13, 4, title, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(12, 3, title, 0, 0, 12, 0, 0, 0);
  char partner[128];
  if (this->partnerId < 1) {
    std::sprintf(partner, "%s", this->partnerName.c_str());
  } else if (g_serviceType == 6) {
    std::sprintf(partner, "%s  Lv%d", this->partnerName.c_str(), this->partnerLevel);
  } else {
    const std::string identifier = ShortcutFilename(this->partnerId);
    std::sprintf(partner, "%s  Lv%d (%s)", this->partnerName.c_str(), this->partnerLevel, identifier.c_str());
  }
  this->DrawTextAt(63, 4, partner, 0, 0, 12, 0xB4B4B4, 0, 0);
  this->DrawTextAt(62, 3, partner, 0, 0, 12, 0, 0, 0);

  const std::string prefix = "유저인터페이스\\btn_";
  const char* buttons[2] = {"ok", "exchange"};
  const int buttonX[2] = {3, 260};
  for (int index = 0; index < 2; ++index) {
    const std::string path = prefix + buttons[index] + "_dis.bmp";
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
    this->DrawBitmap(buttonX[index], this->geometry.height - 23, bitmap, false);
  }
  for (int row = 0; row < 10; ++row) {
    DrawItemSlotBackground(*this, 12, this->m_rowHeight * row + 20);
    DrawItemSlotBackground(*this, 291, this->m_rowHeight * row + 20);
  }
  unsigned red;
  unsigned green;
  unsigned blue;
  if (this->isLocalTrader != 0) {
    SkinPaletteChannels(ResourceMgr::Instance(), 10, 6, red, green, blue);
    this->FillRegion(4, 21, 272, 310, RGB(red, green, blue));
  }
  if (this->localConfirmed != 0) {
    SkinPaletteChannels(ResourceMgr::Instance(), 10, 6, red, green, blue);
    this->FillRegion(283, 21, 272, 310, RGB(red, green, blue));
  }
  int row = 0;
  for (const ITEM_INFO& item : this->m_myItems) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) != 0 && item.damaged != 0) {
      continue;
    }
    const int y = this->m_rowHeight * row;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), 16, y + 21, item, 0, 0);
    this->DrawFittedItemName(48, y + 29, &item, 0, 0, 12, 1);
    if (item.grade > 0) {
      const std::string path = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade);
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
      this->DrawBitmap(16, y + 33, bitmap, true);
    }
    if (row == 9) {
      break;
    }
    ++row;
  }
  row = 0;
  for (const ITEM_INFO& item : this->m_otherItems) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) != 0 && item.damaged != 0) {
      continue;
    }
    const int y = this->m_rowHeight * row;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), 295, y + 21, item, 0, 0);
    this->DrawFittedItemName(327, y + 29, &item, 0, 0, 12, 1);
    if (item.grade > 0) {
      const std::string path = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade);
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
      this->DrawBitmap(295, y + 33, bitmap, true);
    }
    if (row == 9) {
      break;
    }
    ++row;
  }
  char grouped[32];
  char amount[128];
  int length = std::sprintf(amount, "%s", FormatGroupedInteger(g_session.exchangeMoney, grouped, sizeof(grouped)));
  PriceColors colors = PriceColor(g_session.exchangeMoney);
  this->DrawRightAlignedText(219, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(220, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(220, 332, amount, length, 1, 16, 0x323232, 0);
  length = std::sprintf(amount, "%s", FormatGroupedInteger(g_session.otherExchangeMoney, grouped, sizeof(grouped)));
  colors = PriceColor(g_session.otherExchangeMoney);
  this->DrawRightAlignedText(499, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(500, 331, amount, length, 1, 16, colors.primary & 0xFFFFFF, 0);
  this->DrawRightAlignedText(500, 332, amount, length, 1, 16, 0x323232, 0);
}

// OFFSET: 0x008FB960
// FUNCTION: fn_12346
int UIItemCollectionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_SETINFO: {
    this->m_placement = reinterpret_cast<WindowPlacement*>(argument1);
    ItemCollectionWndInfo* placement = static_cast<ItemCollectionWndInfo*>(this->m_placement);
    if (!placement->IsValid()) {
      placement->Reset();
      this->Move((g_renderer->viewport.width - 640) / 2 + this->m_placement->x, this->m_placement->y * g_renderer->viewport.height / 480);
      return 0;
    }
    this->Move(placement->x, placement->y);
    return 0;
  }
  case UM_COMMAND:
    switch (argument1) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(this->m_kind);
      return 0;
    case ID_CARDITEM: {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_CARDITEMILLUSTWND);
      window->SendMsg(nullptr, UM_CARDITEMILLUSTRATE, reinterpret_cast<std::intptr_t>(this->m_item.itemId.c_str()), this->m_item.identified, 0, 0);
      return 0;
    }
    case ID_BOOKITEM:
    case ID_AUTOBOOKITEM: {
      char path[260];
      if (g_serviceType == 10) {
        std::sprintf(path, "%sbook\\%s.txt", g_serverLanguage->servicePrefix.c_str(), this->m_item.itemId.c_str());
      } else {
        std::sprintf(path, "book\\%s.txt", this->m_item.itemId.c_str());
      }
      CFile file;
      if (!file.Open(path, 0, true, false)) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("책이 없습니다"), 0xFF, 0, 0, 0);
        return 0;
      }
      UIWindow* window = g_windowMgr.EnsureWindow(WID_BOOKWND);
      window->SendMsg(nullptr, UM_GETBOOKCONTENTS, reinterpret_cast<std::intptr_t>(this->m_item.itemId.c_str()), argument1, 0, 0);
      file.Close();
      return 0;
    }
    case ID_PREVIEW: {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_PREVIEWEQUIPWND);
      if (window) {
        window->SendMsg(this, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
        return 0;
      }
      break;
    }
    case ID_HELP: {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_PROBABILITYTABLE);
      if (window) {
        static_cast<CUIProbabilityTable*>(window)->SetItem(this->m_item.GetItemId());
        window->PlaceBeside(this);
        return 0;
      }
      break;
    }
    }
    return 0;
  case UM_ITEMEXPLANATION: {
    const ITEM_INFO* item = reinterpret_cast<const ITEM_INFO*>(argument1);
    if (item->itemId == this->m_item.itemId && item->refiningLevel == this->m_item.refiningLevel &&
        std::memcmp(item->cards, this->m_item.cards, sizeof(item->cards)) == 0 && argument2 == 0) {
      g_windowMgr.CloseWindow(this->m_kind);
      return 0;
    }
    this->m_petLevel = -1;
    g_session.IsPetEgg(std::atoi(item->itemId.c_str()));
    this->m_item = *item;
    this->m_helpButton->Enable(NEO_PACKAGEITEM::CPaidPackageItemMgr::GetInstance()->IsPaidItem(std::atoi(this->m_item.itemId.c_str())));
    CUIProbabilityTable* table = static_cast<CUIProbabilityTable*>(g_windowMgr.LookupWindow(WID_PROBABILITYTABLE));
    if (table && table->m_itemId != this->m_item.GetItemId()) {
      g_windowMgr.CloseWindow(WID_PROBABILITYTABLE);
    }
    this->m_cardTexts.clear();
    this->m_extraHeight = 0;
    if (this->m_item.identified == 1) {
      const PackedItemOption* options = reinterpret_cast<const PackedItemOption*>(this->m_item.options);
      for (int index = 0; index < this->m_item.optionCount; ++index) {
        const PackedItemOption& option = options[index];
        if (option.id == 0) {
          continue;
        }
        if (option.id == g_itemOptionPlaceholderId) {
          std::list<std::string> lines = {LookupMessage(MSI_MONSTERINFO_TRIBE_ANIMAL)};
          this->m_cardTexts.insert(std::make_pair(index, lines));
        } else {
          const char* name = nullptr;
          if (g_session.m_hotkeyScript->Call("GetVarOptionName", "d>s", static_cast<int>(option.id), &name) && name) {
            std::list<std::string> lines;
            std::string text = FormatOwnedString(name, static_cast<int>(option.value), option.parameter);
            this->WrapText(text, 254, lines);
            this->m_cardTexts.insert(std::make_pair(index, lines));
          }
        }
      }
      for (std::map<int, std::list<std::string>>::iterator card = this->m_cardTexts.begin(); card != this->m_cardTexts.end(); ++card) {
        this->m_extraHeight += static_cast<int>(card->second.size()) * 15 + 14;
      }
    }
    this->m_item.expireTime = item->expireTime;
    this->m_compare = this->m_item.identified == 1 && !this->m_item.IsApprenticeItem() && Equipment(this->m_item);
    this->m_optionCount = this->m_item.optionCount;
    if (this->m_compare) {
      if (this->geometry.height != 144) {
        this->SetHeight(this->m_extraHeight + 149);
        this->RefreshSnap();
      }
    } else if (this->geometry.height != 120) {
      this->SetHeight(this->m_extraHeight + 120);
      this->RefreshSnap();
    }
    this->m_bitmapName = "유저인터페이스\\collection\\" + std::string(ItemResourcePath(this->m_item)) + ".bmp";
    const std::vector<const char*>& descriptions = this->m_item.Description();
    this->m_viewButton->Move(6, argument2 != 0 ? -100 : IsItemCard(std::atoi(this->m_item.itemId.c_str())) * 100 - 94);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->HasInventoryItem(std::atoi(this->m_item.itemId.c_str()))) {
      const int y = IsBookItem(std::atoi(this->m_item.itemId.c_str())) * 100 - 94;
      for (int index = 0; index < 2; ++index) {
        this->m_readButtons[index]->Move(this->m_readButtons[index]->FrameWidth(0, 0) * index + 6, y);
      }
      if (IsBookItem(std::atoi(this->m_item.itemId.c_str()))) {
        this->m_readButtons[1]->animate = 1;
        this->m_previewTick = timeGetTime();
      } else {
        this->m_readButtons[1]->animate = 0;
        this->m_previewTick = 0;
      }
    }
    this->m_viewer->Clear();
    char text[260];
    if (this->m_item.expireTime != 0) {
      char date[128];
      std::strftime(date, sizeof(date), LookupMessage(MSI_TIME_TYPE2), _localtime32(reinterpret_cast<const __time32_t*>(&this->m_item.expireTime)));
      int color = 32;
      std::memset(text, 0, sizeof(text));
      if (g_inlineCharset) {
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%s%s", g_serverLanguage->Prefix(true), date);
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_NOTICE_TO_DELETE_TIME), text);
      } else {
        std::sprintf(text, LookupMessage(MSI_NOTICE_TO_DELETE_TIME), date);
        if (g_serviceType == 6) {
          color = 24;
        }
      }
      if (this->m_item.durationInHours) {
        const int hours = static_cast<int>(this->m_item.expireTime);
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_JOB_IMPERIAL_GUARD), hours / 24, hours % 24);
      }
      this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text), color, 0, 0);
    }
    for (unsigned index = 0; index < descriptions.size(); ++index) {
      if (g_inlineCharset) {
        std::sprintf(text, "%s%s", g_serverLanguage->Prefix(true), descriptions[index]);
        this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text), 26, 0, 0);
      } else {
        this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(descriptions[index]), g_serviceType == 6 ? 24 : 26, 0, 0);
      }
    }
    if (g_session.IsPetEgg(std::atoi(item->itemId.c_str())) == 1) {
      std::string state;
      const int* cards = reinterpret_cast<const int*>(item->cards);
      switch (cards[2]) {
      case 1:
        state = FormatOwnedString(LookupMessage(MSI_PARTYMASTER), LookupMessage(MSI_VERY_AWKWARD));
        break;
      case 2:
        state = FormatOwnedString(LookupMessage(MSI_PARTYMASTER), LookupMessage(MSI_AWKWARD));
        break;
      case 3:
        state = FormatOwnedString(LookupMessage(MSI_PARTYMASTER), LookupMessage(MSI_NORMAL));
        break;
      case 4:
        state = FormatOwnedString(LookupMessage(MSI_PARTYMASTER), LookupMessage(MSI_FRIENDLY));
        break;
      case 5:
        state = FormatOwnedString(LookupMessage(MSI_PARTYMASTER), LookupMessage(MSI_VERY_FRIENDLY));
        break;
      default:
        state = FormatOwnedString(LookupMessage(MSI_PARTYMASTER), LookupMessage(MSI_MACRO_DETECTOR_TIME_OVER));
        break;
      }
      if (g_inlineCharset == 1) {
        std::memset(text, 0, sizeof(text));
        std::sprintf(text, "%s%s", g_serverLanguage->Prefix(true), state.c_str());
        this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text), 26, 0, 0);
      } else {
        this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(state.c_str()), g_serviceType == 6 ? 24 : 26, 0, 0);
      }
    }
    this->Invalidate();
    g_windowMgr.Promote(this);
    if (argument2 != 0 && g_windowMgr.itemParamChangeDisplayWnd) {
      g_windowMgr.itemParamChangeDisplayWnd->SendMsg(nullptr, UM_PARAM, reinterpret_cast<std::intptr_t>(this->m_item.itemId.c_str()), this->m_item.identified, 0, 0);
    }
    if (this->m_kind == 12) {
      ITEM_INFO* equipped = g_session.GetEquippedItem(&this->m_item);
      if (g_serviceOptions.Get(264) && equipped && argument4 == 0) {
        UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTION_COMPARISONWND);
        if (window) {
          window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(equipped), 1, 0, 0);
        }
      } else {
        g_windowMgr.CloseWindow(WID_ITEMCOLLECTION_COMPARISONWND);
      }
    }
    const int lineCount = static_cast<int>(this->m_viewer->lines.size());
    const ItemMoveInfo& moveInfo = FindItemMoveInfo(std::atoi(this->m_item.itemId.c_str()));
    const int moveHeight = this->m_moveInfo->geometry.height + 3;
    if (lineCount > 4) {
      if (moveInfo.itemId != 0 && this->geometry.height != 120) {
        this->SetHeight((lineCount * 9 - 36) * 2 - moveHeight + this->geometry.height);
      } else {
        this->SetHeight(this->geometry.height + (lineCount - 4) * 18);
      }
    } else if (moveInfo.itemId != 0) {
      if (this->geometry.height == 120) {
        this->Resize(this->geometry.width, moveHeight + 120);
        this->m_moveInfo->m_moveIndex = std::atoi(this->m_item.itemId.c_str());
        this->m_resizer->Move(this->m_resizer->geometry.x, this->m_resizer->geometry.y + moveHeight);
      } else {
        this->SetHeight(this->geometry.height + (lineCount * 9 - 36) * 2);
      }
    }
    if (this->m_withdrawButton) {
      this->RemoveChild(this->m_withdrawButton, 1);
      this->m_withdrawButton = nullptr;
    }
    switch (item->identity[2]) {
    case EQP_HEAD_LOW:
    case EQP_GARMENT:
    case EQP_HEAD_TOP:
    case 0x101:
    case EQP_HEAD_MID:
    case 0x201:
    case 0x300:
    case 0x301:
    case EQP_COSTUME_HEAD_TOP:
    case EQP_COSTUME_HEAD_MID:
    case 0xC00:
    case EQP_COSTUME_HEAD_LOW:
    case 0x1400:
    case 0x1800:
    case 0x1C00:
    case EQP_COSTUME_GARMENT:
      break;
    default:
      return 0;
    }
    if (item->identified == 0 || item->sprite == 0) {
      return 0;
    }
    bool effectHat = false;
    g_session.m_hotkeyScript->Call(std::string("IsEffectHatItem", 15), "d>b", std::atoi(item->itemId.c_str()), &effectHat);
    if (effectHat == true) {
      return 0;
    }
    this->m_withdrawButton = new UIBasicButton();
    this->m_withdrawButton->Configure(77, LookupMessage(MSI_WITHDRAWAL));
    this->m_withdrawButton->Move(9, 117);
    this->m_withdrawButton->SetId(354);
    this->m_withdrawButton->invalidateParent = 1;
    this->AddChild(this->m_withdrawButton);
    if (this->geometry.height < 172) {
      this->SetHeight(172);
    }
    this->Invalidate();
    return 0;
  }
  case UM_SETTEXT: {
    const std::vector<const char*>& descriptions = this->m_item.Description();
    std::vector<std::string> lines;
    std::string text;
    if (this->m_petLevel != -1) {
      text = reinterpret_cast<const char*>(this->m_petName);
      lines.push_back(text);
      text = this->m_petRenamed != 0 ? "이름 변경됨" : "이름 변경되지않음";
      lines.push_back(text);
      char buffer[64];
      _itoa(this->m_petLevel, buffer, 10);
      text = "레벨 : ";
      text += buffer;
      lines.push_back(text);
      g_session.GetPetHungryStateString(this->m_petHunger, buffer);
      text = "만복도 : ";
      text += buffer;
      lines.push_back(text);
      g_session.GetPetFriendlyStateString(this->m_petFriendly, buffer);
      text = "친밀도 : ";
      text += buffer;
      lines.push_back(text);
      text = this->m_petAccessory != 0 ? "악세사리 : 장착됨" : "악세사리 : 장착안됨";
      lines.push_back(text);
    }
    for (unsigned index = 0; index < descriptions.size(); ++index) {
      lines.emplace_back(descriptions[index]);
    }
    this->m_viewButton->Move(6, IsItemCard(std::atoi(this->m_item.itemId.c_str())) * 100 - 94);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->HasInventoryItem(std::atoi(this->m_item.itemId.c_str()))) {
      const int y = IsBookItem(std::atoi(this->m_item.itemId.c_str())) * 100 - 94;
      for (int index = 0; index < 2; ++index) {
        this->m_readButtons[index]->Move(this->m_readButtons[index]->FrameWidth(0, 0) * index + 6, y);
      }
    }
    this->m_viewer->Clear();
    for (unsigned index = 0; index < lines.size(); ++index) {
      this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(lines[index].c_str()), 26, 0, 0);
    }
    this->Invalidate();
    g_windowMgr.Promote(this);
    return 0;
  }
  case UM_RESIZE:
    if (argument1 == 7) {
      this->SetHeight(argument3);
    }
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
