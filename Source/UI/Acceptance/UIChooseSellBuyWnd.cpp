#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Skills/SkillInfo.h"
#include "File/File.h"
#include "UI/Items/UISpellListWnd.h"
#include "Client/Registry.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeEvents.h"
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
#include "UI/Buttons/UISpriteButton.h"
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
#include "UI/Items/UICardItemIllustWnd.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Items/UIExchangeWnd.h"
#include "UI/Items/UIItemCollectionWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Items/UIItemParamChangeDisplayWnd.h"
#include "UI/Items/UINotifyItemObtainWnd.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Party/UIPartySettingWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Shop/UInCash_CallWnd.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Skill/UISkillNameChangeWnd.h"
#include "UI/Status/UIStatusWnd.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIMonsterDisplayWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextTokens.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <direct.h>
#include <io.h>
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
