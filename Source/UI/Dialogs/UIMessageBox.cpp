#include "Render/DeviceContext/DCSurface.h"
#include "Contents/Skin/SkinMgr.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Render/Text/FontCache.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/CombatTypes.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Dialogs/UICntWnd.h"
#include "UI/Chat/UIChatRoomWnd.h"
#include "UI/Shop/UIBank_NewWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Session/Session.h"
#include "Utility/Text/GroupedInteger.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Login/UILoginWnd.h"
#include "UI/Dialogs/UIMessageBox.h"
#include "UI/Dialogs/UINoticeWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Replay/UIReplayListBox.h"
#include "UI/Replay/UISelectReplayDataWnd.h"
#include "UI/Login/UISelectServerWnd.h"
#include "UI/Buttons/UIStaticBitmap.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Status/UIMerInfoWnd.h"
#include "UI/Dialogs/UIMessageBoxAutoreturn.h"
#include "UI/Monster/UIMonsterInfoWnd.h"
#include "UI/Dialogs/UITalkboxTrapInputWnd.h"
#include "UI/Status/UIHomunInfoWnd.h"
#include "UI/Status/UIPetInfoWnd.h"
#include "UI/Replay/UIReplayRECControlWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Registry.h"
#include "Utility/Cipher/XorBytes.h"
#include "UI/Login/UILoginOTPWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Buttons/UIButton.h"
#include "Render/Text/TextContext.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Text/ColoredText.h"
#include "Client/Mode/CMode.h"
#include "UI/CashShop/UIPayConfirmWnd.h"
#include "UI/Replay/UIReplayControlWnd.h"
#include "UI/Login/UIMakeCharWnd.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Replay/Replay.h"
#include "Client/Application/Launcher.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIMemberListBox.h"
#include "UI/Window/UIResizer.h"
#include "Client/Application/ApplicationWindow.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Gauges/UIBarGraph.h"
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
#include "UI/Lists/UIComboBox.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Items/UIItemWnd.h"
#include "Contents/Sound/SoundMaker.h"
#include "UI/Buttons/UICheckBox.h"
#include "Contents/ClientState.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "UI/Dialogs/UINormalMB.h"
#include "UI/Buttons/UINoneVisibleButton.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include <mbstring.h>
#include "Contents/Actors/Pc.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/Sprites/ImfResource.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Resource/MSGSTRINGID.h"

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

// GLOBAL: gl_25153
static const std::string s_messageBoxItemLinkOpening = "<ITEML>";

// GLOBAL: gl_25154
static const std::string s_messageBoxItemLinkClosing = "</ITEML>";

namespace {
  constexpr char prefix[] = "유저인터페이스\\";

  // INLINED
  int TileCount(int length, int tile) {
    const std::div_t tiles = std::div(length, tile);
    return tiles.quot + (tiles.rem != 0);
  }

  struct HangameJoinUrl {
    EncryptedName<80, 0xF3CB6F7711A71B6DULL>& operator()() const;
  };
}

// OFFSET: 0x008A39C0
// FUNCTION: fn_11661
UIMakeCharWnd::UIMakeCharWnd()
    : UIFrameWnd(0), m_rotateLeftButton(0), m_rotateRightButton(0), m_hairStyleBar(0), m_hairColorBar(0), m_hairStyle(0), m_hairColor(0) {
  this->geometry.animationFrame = 0;
  this->m_nameEdit = nullptr;
  this->m_info.sex = static_cast<unsigned char>(g_session.m_sex);

  const int* preset = reinterpret_cast<const int*>(g_modeMgr.gameMode->SendMsg(LMM_GET_CHAR_PARAM, 0, 0, 0, 0));
  this->m_info.stats[0] = static_cast<unsigned char>(preset[0]);
  this->m_info.stats[1] = static_cast<unsigned char>(preset[1]);
  this->m_info.stats[2] = static_cast<unsigned char>(preset[2]);
  this->m_info.stats[3] = static_cast<unsigned char>(preset[3]);
  this->m_info.stats[4] = static_cast<unsigned char>(preset[4]);
  this->m_info.stats[5] = static_cast<unsigned char>(preset[5]);
  this->m_info.headPalette = static_cast<std::int16_t>(preset[6]);
  this->m_info.head = static_cast<std::int16_t>(preset[7]);

  this->m_info.name[0] = 0;
  this->m_info.job = 0;
  this->m_info.jobPoint = 0;
  this->m_info.hp = 100;
  this->m_info.maxHp = 100;
  this->m_info.sp = 100;
  this->m_info.maxSp = 100;
  this->m_info.exp = 0;
  this->m_info.level = 1;
  this->m_info.skillPoint = 1;
  this->m_info.slot = static_cast<unsigned char>(g_selectedSlotIndex);
  this->m_info.headPalette = 0;
  this->m_info.bodyStyle = 0;
  this->m_info.shield = 0;
  this->m_info.accessory2 = 0;
  this->m_info.accessory3 = 0;
  this->m_view.actionBase = 0;
  this->m_view.action = 0;
  this->m_view.frame = 0;
  this->m_view.x = 70;
  this->m_view.y = 123;

  this->m_view.actionNames[0].assign(g_session.GetBodyActionResourceName(this->m_info.job, this->m_info.sex, 0, 0));
  this->m_view.spriteNames[0].assign(g_session.GetBodySprName(this->m_info.job, this->m_info.sex, 0, 0));

  if (this->m_info.sex == 1 && 13 == this->m_info.head) {
    this->m_info.head = 14;
  }

  int head = this->m_info.head;
  this->m_view.actionNames[1].assign(g_session.GetHeadActName(this->m_info.job, this->m_info.sex, &head));
  this->m_view.spriteNames[1].assign(g_session.GetHeadSprName(this->m_info.job, this->m_info.sex, &head));

  const int bodyPalette = this->m_info.bodyStyle;
  this->m_info.head = static_cast<std::int16_t>(head);
  this->m_view.bodyPalette = bodyPalette;
  if (bodyPalette) {
    this->m_view.bodyPaletteName.assign(g_session.GetBodyPalName(this->m_info.job, this->m_info.sex, bodyPalette, this->m_info.bodyCostume, 0, 0));
  }

  const int headPalette = this->m_info.headPalette;
  this->m_view.headPalette = headPalette;
  if (headPalette) {
    this->m_view.headPaletteName.assign(g_session.GetHeadPalName(head, this->m_info.job, this->m_info.sex, headPalette));
  }

  this->m_view.imfName = g_session.GetBodyImfName(this->m_info.job, this->m_info.sex, this->m_info.head, this->m_info.bodyCostume);
  this->geometry.currentAlpha = 0x80;
  this->geometry.targetAlpha = 0xFF;
}

// OFFSET: 0x008A3F60
// FUNCTION: fn_11662
UIMerInfoWnd::UIMerInfoWnd() : UIFrameWnd(0) {
}

// OFFSET: 0x008A3FC0
// FUNCTION: fn_11663
UIMessageBox::UIMessageBox() : UIFrameWnd(0) {
  this->counting = 0;
  this->protectedMessage = 0;
}

// OFFSET: 0x008A40B0
// FUNCTION: fn_11664
UIMessageBoxAutoreturn::UIMessageBoxAutoreturn() : UIFrameWnd(0) {
  this->counting = 0;
}

// OFFSET: 0x008A4170
// FUNCTION: fn_11665
UIMonsterInfoWnd::UIMonsterInfoWnd()
    : UIFrameWnd(0) {
  std::memset(&this->info, 0, offsetof(MonsterInfo, resistances) + sizeof(this->info.resistances));
  this->actorId = 0;
}

// INLINED
UIReplayRECControlWnd::~UIReplayRECControlWnd() {
  this->recordStart = 0;
  this->mode = 0;
}

// OFFSET: 0x008A5220
// FUNCTION: fn_11680
UIMessageBox::~UIMessageBox() = default;

// OFFSET: 0x008B8E60
// FUNCTION: fn_11768
void UIMessageBox::Paint() {
  this->DrawTitleBar(true, this->caption.c_str(), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x008AC410
// FUNCTION: fn_11729
bool UIBank_NewWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x008AC420
// FUNCTION: fn_11730
bool UIReplayControlWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x008AC430
// FUNCTION: fn_11731
bool UIReplayRECControlWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x008B7A90
// FUNCTION: fn_11756
void UIMessageBox::OnDestroy() {
}

// OFFSET: 0x008B7AA0
// FUNCTION: fn_11757
void UINoticeWnd::OnDestroy() {
}

// OFFSET: 0x008B7AB0
// FUNCTION: fn_11758
void UISelectReplayDataWnd::OnDestroy() {
}

// OFFSET: 0x008B7AC0
// FUNCTION: fn_11759
void UISelectServerWnd::OnDestroy() {
}

// OFFSET: 0x008B8610
// FUNCTION: fn_11765
void UILoginWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x008B8620
// FUNCTION: fn_11766
void UIMakeCharWnd::Paint() {
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\login_interface\\win_make2.bmp")));
  this->DrawBitmap(0, 17, background, false);
  this->DrawTitleBar(true, LookupMessage(MSI_SELECT_SEX), 0);
  this->DrawButtonBar(2, 0, 1);

  SpriteAtlasRecord record{};
  for (int i = 0; i < 2; ++i) {
    CImfRes* offsets = static_cast<CImfRes*>(ResourceMgr::Instance().Load(this->m_view.imfName.c_str()));
    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_view.actionNames[i].c_str()));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->m_view.spriteNames[i].c_str()));
    const ActionFrame& frame = action->Frame(this->m_view.action, this->m_view.frame);
    POINT offset = offsets->GetLongPoint(i, this->m_view.action, this->m_view.frame);

    if (i == 1) {
      CActRes* body = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_view.actionNames[0].c_str()));
      const ActionFrame& bodyFrame = body->Frame(this->m_view.action, this->m_view.frame);
      if (frame.anchorCount != 0 && bodyFrame.anchorCount != 0 && frame.anchors[0].attachmentId == bodyFrame.anchors[0].attachmentId) {
        offset.x += bodyFrame.anchors[0].x - frame.anchors[0].x;
        offset.y += bodyFrame.anchors[0].y - frame.anchors[0].y;
      }
    }

    const std::uint32_t* palette = sprite->palette;
    switch (i) {
    case 0:
      if (this->m_view.bodyPalette) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_view.bodyPaletteName.c_str()))->palette;
      }
      break;
    case 1:
      if (this->m_view.headPalette) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_view.headPaletteName.c_str()))->palette;
      }
      break;
    }

    const SpriteFrameCell& cell = frame.Cell(i);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(image, palette, record);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    }

    this->geometry.surface->DrawSurface(
      this->m_view.x + cell.x + offset.x, this->m_view.y + cell.y + offset.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->doubleWidth + 1, image->doubleHeight + 1, 0
    );
  }
}

// OFFSET: 0x008B88F0
// FUNCTION: fn_11767
void UIMerInfoWnd::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\homuninfo_bg.bmp")));
  this->DrawBitmap(0, 0, background, false);
  char title[50] = {};
  const unsigned kind = g_session.m_mercenaryClass;
  const int titleId = kind - 6017 <= 9 ? 1181 : kind - 6037 <= 9 ? 1182 : kind - 6027 <= 9 ? 1183 : 1367;
  std::sprintf(title, LookupMessage(titleId));
  this->DrawTextAt(5, 2, title, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 24, LookupMessage(MSI_NAME), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 47, LookupMessage(MSI_LEVEL), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 111, LookupMessage(MSI_LEFT_TIME), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 135, LookupMessage(MSI_MER_FRIENDLY), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 159, LookupMessage(MSI_MER_CALLCOUNT), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(230, 133, LookupMessage(MSI_MER_KILL), 0, 0, 12, 0, 0, 0);
  {
    char number[24] = {};
    int bytes = std::sprintf(number, "%d", g_session.m_mercenaryStats[0]);
    this->DrawRightAlignedText(79, 24, number, bytes, 1, 14, 0, 0);
    bytes = std::sprintf(number, "%d", g_session.m_mercenaryStats[1]);
    this->DrawRightAlignedText(79, 42, number, bytes, 1, 14, 0, 0);
    bytes = std::sprintf(number, "%d", g_session.m_mercenaryStats[2]);
    this->DrawRightAlignedText(79, 60, number, bytes, 1, 14, 0, 0);
    bytes = std::sprintf(number, "%d", g_session.m_mercenaryStats[3]);
    this->DrawRightAlignedText(79, 78, number, bytes, 1, 14, 0, 0);
    bytes = std::sprintf(number, "%d", g_session.m_mercenaryStats[4]);
    this->DrawRightAlignedText(79, 96, number, bytes, 1, 14, 0, 0);
    bytes = std::sprintf(number, "%d", g_session.m_mercenaryStats[5]);
    this->DrawRightAlignedText(79, 114, number, bytes, 1, 14, 0, 0);
    bytes = std::sprintf(number, "%d", g_session.m_mercenaryStats[6]);
    this->DrawRightAlignedText(79, 132, number, bytes, 1, 14, 0, 0);
    bytes = std::sprintf(number, "%d", (2000 - g_session.m_mercenaryStats[7]) / 10);
    this->DrawRightAlignedText(79, 150, number, bytes, 1, 14, 0, 0);
  }
  this->DrawTextAt(136, 24, g_session.m_mercenaryName, 0, 0, 12, 0x646464, 0, 0);
  this->DrawTextAt(135, 24, g_session.m_mercenaryName, 0, 0, 12, 0, 0, 0);
  {
    char level[12] = {};
    _itoa(g_session.m_mercenaryLevel, level, 10);
    this->DrawTextAt(135, 47, level, 0, 0, 12, 0, 0, 0);
  }
  if (g_session.m_mercenaryClass == -1) {
    return;
  }
  char text[32] = {};
  int bytes = std::sprintf(text, "%d / %d", g_session.m_mercenaryHp, g_session.m_mercenaryMaxHp);
  this->DrawTextAt(218, 70, text, bytes, 1, 14, 0, 0, 0);
  bytes = std::sprintf(text, "%d / %d", g_session.m_mercenarySp, g_session.m_mercenaryMaxSp);
  this->DrawTextAt(218, 85, text, bytes, 1, 14, 0, 0, 0);
  std::strftime(text, 128, LookupMessage(MSI_TIME_TYPE2), _localtime32(&g_session.m_mercenaryExpiration));
  this->DrawTextAt(155, 111, text, static_cast<int>(std::strlen(text)), 0, 12, 255, 0, 0);
  bytes = std::sprintf(text, "%d", g_session.m_mercenaryLoyalty);
  this->DrawTextAt(170, 133, text, bytes, 1, 14, 0, 0, 0);
  bytes = std::sprintf(text, "%d", g_session.m_mercenarySummons);
  this->DrawTextAt(170, 157, text, bytes, 1, 14, 0, 0, 0);
  bytes = std::sprintf(text, "%d", g_session.m_mercenaryKills);
  this->DrawTextAt(245, 157, text, bytes, 1, 14, 0, 0, 0);
}

// OFFSET: 0x008A6E20
// FUNCTION: fn_11712
long long UIBank_NewWnd::TransactionAmount(unsigned char deposit) {
  std::string text = this->m_amountEdit->Text();
  this->m_amountEdit->SetText("");
  if (text == std::string(LookupMessage(MSI_BANK_1_MINI))) {
    const long long cash = g_session.m_zeny;
    const long long balance = g_session.m_bankBalance;
    const long long excess = cash + balance > 0x7FFFFFFF ? cash + balance - 0x7FFFFFFF : 0;
    if (deposit) {
      if (balance >= 0x7FFFFFFF) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEMMOVEINFO_CART)), 255, 0, 0, 0);
        this->m_status = LookupMessage(MSI_BANK_6_MINI);
        return 0;
      }
      if (g_session.m_zeny == 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BANK_3)), 255, 0, 0, 0);
        this->m_status = LookupMessage(MSI_BANK_3_MINI);
        return 0;
      }
      const long long amount = cash - excess;
      if (amount < 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOT_VALID_AUTH_TICKET)), 255, 0, 0, 0);
        this->m_status = LookupMessage(MSI_BANK_3_MINI);
        return 0;
      }
      return amount;
    }
    if (balance == 0) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BANK_3)), 255, 0, 0, 0);
      this->m_status = LookupMessage(MSI_BANK_3_MINI);
      return 0;
    }
    const long long amount = balance - excess;
    return cash + amount > 0x7FFFFFFF ? static_cast<long long>(0x7FFFFFFF - g_session.m_zeny) : amount;
  }
  if (text.empty()) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BANK_2_MINI)), 255, 0, 0, 0);
    this->m_status = LookupMessage(MSI_BANK_4_MINI);
    return 0;
  }
  if (!this->IsNumeric(text)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_COMPARISON)), 255, 0, 0, 0);
    this->m_status = LookupMessage(MSI_BANK_5_MINI);
    return 0;
  }
  const long long amount = _atoi64(text.c_str());
  if (amount > 0x7FFFFFFF) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BANK_1)), 255, 0, 0, 0);
    this->m_status = LookupMessage(MSI_BANK_6_MINI);
    return 0;
  }
  if (amount < 1) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BANK_2)), 255, 0, 0, 0);
    this->m_status = LookupMessage(MSI_BANK_7_MINI);
    return 0;
  }
  return amount;
}

// OFFSET: 0x008A6C20
// FUNCTION: fn_11710
long long UIBank_NewWnd::AdjustAmount(int amount) {
  std::string text = this->m_amountEdit->Text();
  this->m_amountEdit->SetText("");
  long long result;
  if (text.empty()) {
    result = amount;
  } else if (!this->IsNumeric(text)) {
    result = amount;
  } else {
    const long long value = _atoi64(text.c_str());
    result = amount + value;
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
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\bank\\bg_bank.bmp")));
  this->DrawBitmap(0, this->DrawTitleBar(true, LookupMessage(MSI_BANK_4), 0), background, false);

  char number[260] = {};
  char text[260] = {};
  this->DrawTextAt(30, 29, LookupMessage(MSI_BANK_5), 0, 0, 12, 0, 0, 0);
  const int x = this->VerticalTextHeight(LookupMessage(MSI_BANK_5), 0, 0, 12, 0) + 43;
  FormatGroupedInteger(g_session.m_bankBalance, number, 260);
  std::sprintf(text, "%s z", number);
  this->DrawTextAt(x, 29, text, 0, 0, 11, 0, 0, 0);
  this->DrawTextAt(30, 46, LookupMessage(MSI_BANK_6), 0, 0, 12, 0, 0, 0);
  FormatGroupedInteger(g_session.m_zeny, number, 260);
  std::sprintf(text, "%s z", number);
  this->DrawTextAt(x, 46, text, 0, 0, 11, 0, 0, 0);
  if (!this->m_status.empty()) {
    const int textWidth = this->VerticalTextHeight(this->m_status.c_str(), 0, 0, 12, 0);
    const float statusX = this->geometry.width * 0.5f - textWidth * 0.5f;
    this->DrawTextAt(static_cast<int>(statusX), 112, this->m_status.c_str(), 0, 0, 11, 255, 0, 0);
  }
}

// OFFSET: 0x008B7CF0
// FUNCTION: fn_11761
void UIChatRoomWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(20, 6, red, green, blue);
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
  const int columns = TileCount(innerWidth, 14);
  for (int i = 0; i < columns; ++i) {
    auto* bitmap = load("유저인터페이스\\sysbox_mu.bmp");
    this->DrawBitmap(x + 14 + i * 14, y, bitmap, false);
  }
  auto* bitmap = load("유저인터페이스\\sysbox_lu.bmp");
  this->DrawBitmap(x, y, bitmap, false);
  bitmap = load("유저인터페이스\\sysbox_ru.bmp");
  this->DrawBitmap(x + width - bitmap->image.width, y, bitmap, false);
  const int innerHeight = height - 28;
  const int rows = TileCount(innerHeight, 14);
  for (int i = 0; i < rows; ++i) {
    bitmap = load("유저인터페이스\\sysbox_lm.bmp");
    this->DrawBitmap(x, y + 14 + i * 14, bitmap, false);
  }
  for (int i = 0; i < rows; ++i) {
    bitmap = load("유저인터페이스\\sysbox_rm.bmp");
    this->DrawBitmap(x + width - bitmap->image.width, y + 14 + i * 14, bitmap, false);
  }
  for (int i = 0; i < columns; ++i) {
    bitmap = load("유저인터페이스\\sysbox_md.bmp");
    this->DrawBitmap(x + 14 + i * 14, y + height - bitmap->image.height, bitmap, false);
  }
  bitmap = load("유저인터페이스\\sysbox_ld.bmp");
  this->DrawBitmap(x, y + height - bitmap->image.height, bitmap, false);
  bitmap = load("유저인터페이스\\sysbox_rd.bmp");
  this->DrawBitmap(x + width - bitmap->image.width, y + height - bitmap->image.height, bitmap, false);
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(x + 14, y + 14, innerWidth, innerHeight, RGB(red, green, blue));
}

// INLINED
__forceinline void UIMessageBox::FormatCountdown() {
  char text[512];
  std::sprintf(text, this->format.c_str(), this->remaining);
  this->messageText = text;
}

// OFFSET: 0x008A7350
// FUNCTION: fn_11713
void UIMessageBox::Configure(
  const char* text, int type, int defaultAccept, int timeout, const char* title, int width, int height, int modalValue, const char* icon
) {
  this->caption = title;
  this->messageText = text;
  this->format = text;
  this->started = timeGetTime();
  this->deadline = timeGetTime() + static_cast<unsigned>(timeout);
  this->remaining = (this->deadline - this->started) / 1000 + ((this->deadline - this->started) % 1000 != 0);

  if (timeout) {
    this->counting = 1;
    this->FormatCountdown();
  } else {
    this->counting = 0;
  }

  this->modal = modalValue;
  this->SetSize(width, height);
  this->OnCreate(width, height);
  this->OnSize(width, height);
  this->Move((g_renderer->viewport.width - 640) / 2 + 185, (g_renderer->viewport.height - 480) / 2 + 180);
  int inset = 0;

  if (icon) {
    auto* image = new UIStaticBitmap();
    image->SetBitmap(icon);
    image->SetSize(image->geometry.width, image->geometry.height);
    image->Move(10, 22);
    this->AddChild(image);
    inset = image->geometry.width + image->geometry.x;
  }

  this->viewer = new UITextViewer();
  this->viewer->Configure(inset + 10, 22, width - inset - 20, height - 55, 0);
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->viewer->backgroundColor = 0xFF000000u | RGB(red, green, blue);
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
  this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->messageText.data()), static_cast<int>(static_cast<double>(width) / 7.5), 0, 0);
  this->m_message0 = type == 2 ? 188 : 185;

  const std::string prefix("유저인터페이스\\");
  int positions[3][2] = {{189, height - 24}, {189, height - 24}, {234, height - 24}};
  const char* names[3][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[3] = {184, 187, 188};

  switch (type) {
  case 4: {
    auto* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[2][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[2][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[2][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(width - 46, height - 24);
    button->SetId(185);
    this->AddChild(button);
    this->m_message0 = 185;
    break;
  }
  case 0: {
    auto* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[0][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[0][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[0][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(width - 46, height - 24);
    button->SetId(184);
    this->AddChild(button);
    this->m_message0 = 184;
    break;
  }
  case 2: {
    int x = width - 91;

    for (int index = 1; index < 3; ++index) {
      positions[index][0] = x;
      auto* button = new UIBitmapButton();
      button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[index][0], positions[index][1]);
      button->SetId(ids[index]);
      this->AddChild(button);
      x += 45;
    }

    this->m_message0 = defaultAccept ? 187 : 188;
    this->m_message1 = 188;
    break;
  }
  case 5: {
    auto* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[0][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[0][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[0][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(width - 46, height - 24);
    button->SetId(184);
    this->AddChild(button);
    this->m_message0 = 184;

    auto* link = new UITextButton();
    link->Configure(LookupMessage(MSI_FRIEND_ALREADY_FRIEND), 0x00F4934A, 0, 12, 0, 0);
    link->m_hoverColor = 0x00FF7864;
    link->Move(14, 70);
    link->SetId(426);
    link->SetDisabled(0);
    link->Invalidate();
    this->AddChild(link);
    break;
  }
  default:
    break;
  }
}

// OFFSET: 0x008BADC0
// FUNCTION: fn_11791
void UIMessageBox::OnProcess() {
  if (this->counting == 0) {
    return;
  }

  if (timeGetTime() > this->deadline) {
    g_windowMgr.modalResponse = 187;
    this->counting = 0;
    return;
  }

  const unsigned previous = this->remaining;
  const unsigned fraction = (this->deadline - timeGetTime()) % 1000 ? 1 : 0;
  this->remaining = (this->deadline - timeGetTime()) / 1000 + fraction;

  if (previous != this->remaining) {
    this->FormatCountdown();
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->messageText.data()), 38, 0, 0);
  }
}

// OFFSET: 0x008BF950
// FUNCTION: fn_11808
int UIMessageBox::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SET_MODAL:
    this->modal = static_cast<int>(a);
    return 0;
  case UM_SETCOUNTMSG:
    this->format = reinterpret_cast<const char*>(a);
    return 0;
  case UM_CHANGEMSG:
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, a, 38, 0, 0);
    return 0;
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);

    if (this->modal != 0) {
      g_windowMgr.modalResponse = static_cast<int>(a);
    } else {
      switch (a) {
      case ID_OK:
      case ID_YES:
      case ID_NO:
        g_windowMgr.Schedule(this);
      }
    }
    break;
  case UM_TEXT_LBTN_UP:
    if (a == 426) {
      ShellExecuteA(hWnd, "open", LookupMessage(MSI_FRIEND_ALREADY_FRIEND), nullptr, nullptr, SW_SHOWNORMAL);
    }
    break;
  case UM_SET_MESSAGE_PROTECTED:
    this->protectedMessage = a > 0;
    break;
  case UM_IS_MESSAGE_PROTECTED:
    return this->protectedMessage == 1;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x008BFB70
// FUNCTION: fn_11809
int UIMessageBoxAutoreturn::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  g_windowMgr.modalResponse = static_cast<int>(a);
  return 0;
}

// OFFSET: 0x008BFBA0
// FUNCTION: fn_11810
int UIMonsterInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    if (a == ID_OK) {
      g_windowMgr.Schedule(this);
    }

    return 0;
  case UM_SET_MONSTER_INFO:
    if (a) {
      std::memcpy(&this->info, reinterpret_cast<const void*>(a), offsetof(MonsterInfo, resistances) + sizeof(this->info.resistances));
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
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      TalkboxTrapRequest request;
      request.target.skillId = this->target.skillId;
      request.target.cellX = this->target.cellX;
      request.target.cellY = this->target.cellY;
      request.target.skillLevel = this->target.skillLevel;
      request.text = this->edit->Text();
      g_modeMgr.GetGameMode()->SendMsg(MM_USE_SKILL_TOGROUND_WITH_TALKBOX, reinterpret_cast<int>(&request), 0, 0, 0);
    }
      [[fallthrough]];
    case ID_CANCEL:
      g_windowMgr.Schedule(this);
    }
    return 0;
  case UM_SET_SKILL_INFO:
  {
    const auto* target = reinterpret_cast<const TalkboxTrapTarget*>(a);
    this->target.skillId = target->skillId;
    this->target.cellX = target->cellX;
    this->target.cellY = target->cellY;
    this->target.skillLevel = target->skillLevel;
  }
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008C1ED0
// FUNCTION: fn_11818
void UIWindowMgr::SetChatInputText(const char* text) {
  this->chatInputText = text;
}

// OFFSET: 0x008C35A0
// FUNCTION: fn_11824
void UIChatRoomWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x008C35E0
// FUNCTION: fn_11825
void UIHomunInfoWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008B0CA0
// FUNCTION: fn_11744
void UIMerInfoWnd::OnCreate(int, int) {
  this->AddCloseButton("", 0);
  {
    const std::string name("유저인터페이스\\btn_fired");
    this->button = new UIBitmapButton();
    this->button->SetBitmap((name + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((name + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(187, 44);
    this->button->SetId(350);
    this->AddChild(this->button);
  }
  {
    const std::string name("유저인터페이스\\btn_skill");
    this->button = new UIBitmapButton();
    this->button->SetBitmap((name + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((name + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(232, 44);
    this->button->SetId(349);
    this->AddChild(this->button);
  }

  this->hpBar = new UIBarGraphPlayer();
  this->hpBar->SetValues([]() -> std::int64_t { return g_session.m_mercenaryHp; }, []() -> std::int64_t { return g_session.m_mercenaryMaxHp; });
  this->hpBar->SetSize(85, 9);
  this->hpBar->Move(117, 73);
  this->AddChild(this->hpBar);

  this->spBar = new UIBarGraphPlayer();
  this->spBar->SetValues([]() -> std::int64_t { return g_session.m_mercenarySp; }, []() -> std::int64_t { return g_session.m_mercenaryMaxSp; });
  this->spBar->SetSize(85, 9);
  this->spBar->Move(117, 88);
  this->AddChild(this->spBar);

  this->m_message0 = 201;
}

// OFFSET: 0x008C3630
// FUNCTION: fn_11827
void UIMerInfoWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008BF720
// FUNCTION: fn_11807
int UIMerInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DEFPUSH:
    return 0;
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);

    switch (a) {
    case ID_OK:
      this->Invalidate();
      return 0;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_MERINFOWND);
      return 0;
    case ID_OPEN_MERSKILLLIST:
      if (!g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_1)) {
        g_windowMgr.EnsureWindow(WID_SKILLLISTWND_SKILLTAB_1);
        return 0;
      }

      return 0;
    case ID_FIRE_MER: {
      CMode* mode = g_modeMgr.GetGameMode();

      if (g_windowMgr.Confirm(LookupMessage(MSI_DELETE_MER), 2, false, false, 0, "Message", 280, 120, 0) == ID_YES) {
        mode->SendMsg(MM_COMMAND_MER, 2, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_MERINFOWND);
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_1);
        g_session.ResetMercenaryStats();
        g_session.mercenaryAlive = 0;
        return 0;
      }

      return 0;
    }
    default:
      return 0;
    }
  case UM_SETINFO:
    this->saved = reinterpret_cast<MerInfoWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008C3670
// FUNCTION: fn_11828
void UIPetInfoWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x008C36B0
// FUNCTION: fn_11829
void UIReplayRECControlWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x008A6DC0
// FUNCTION: fn_11711
void UIReplayRECControlWnd::AnimateLoading() {
  for (int index = 0; index < 4; ++index) {
    this->tabs[index]->Move(-100, -100);
  }

  this->tabs[static_cast<int>(timeGetTime() % 1000) / 250]->Move(6, 5);
}

// OFFSET: 0x008AE8A0
// FUNCTION: fn_11741
void UILoginOTPWnd::OnCreate(int, int) {
  const auto prefixSource = OBFUSCATE_SOURCE("유저인터페이스\\", 0x793F370985ED674FULL);
  thread_local EncryptedName<sizeof(prefixSource), 0x793F370985ED674FULL> prefixName(prefixSource.data);
  const std::string prefix(prefixName.Text());

  this->authCodeEdit = new UIEditCtrl();
  this->authCodeEdit->SetSize(80, 16);
  this->authCodeEdit->maximum = 8;
  this->authCodeEdit->Move(99, 95);
  this->authCodeEdit->mask = '*';
  this->authCodeEdit->Background(243, 243, 243);
  this->AddChild(this->authCodeEdit);
  this->authCodeEdit->SetText("");
  g_windowMgr.SetFocus(this->authCodeEdit);

  UIStaticText* label = new UIStaticText();
  label->SetSize(80, 20);
  label->Move(141 - this->TextWidth(LookupMessage(MSI_ID), 0, 0, 12, 0, 0), 67);
  label->SetText(LookupMessage(MSI_ID), 0);
  this->AddChild(label);

  this->timerText = new UIStaticText();
  this->timerText->SetSize(80, 20);
  this->timerText->Move(150, 66);
  this->timerText->SetText(LookupMessage(MSI_ID), 0);
  this->timerText->SetFont(0, 14, 1, 0);
  this->timerText->SetColor(205, 19, 41);
  this->AddChild(this->timerText);

  label = new UIStaticText();
  label->SetSize(200, 40);
  label->SetText(LookupMessage(MSI_JOBNAME_TAEKWON), 0);
  label->Move(static_cast<int>(141.0f - this->TextWidth(LookupMessage(MSI_JOBNAME_TAEKWON), 0, 0, 12, 0, 0) * 0.5f), 28);
  this->AddChild(label);

  label = new UIStaticText();
  label->SetSize(200, 40);
  label->SetText(LookupMessage(MSI_TO_BLOCK_WHISPER), 0);
  label->Move(static_cast<int>(141.0f - this->TextWidth(LookupMessage(MSI_TO_BLOCK_WHISPER), 0, 0, 12, 0, 0) * 0.5f), 42);
  this->AddChild(label);

  label = new UIStaticText();
  label->SetSize(80, 20);
  label->Move(74 - this->TextWidth(LookupMessage(MSI_SERVER_PROPERTY_ADULT_NO_COUNT), 0, 0, 12, 0, 0), 98);
  label->SetText(LookupMessage(MSI_SERVER_PROPERTY_ADULT_NO_COUNT), 0);
  label->SetFont(0, 12, 1, 0);
  label->SetColor(64, 64, 64);
  this->AddChild(label);

  UIBmpTextButton* button = new UIBmpTextButton();
  EncryptedName<49, 0xE55723712993051DULL>& normalName = OBFUSCATE_STRING("유저인터페이스\\login_interface\\bt_otp_normal.bmp", 0xE55723712993051DULL);
  button->SetBitmap(normalName.Text(), 0, 0);
  EncryptedName<47, 0x7DE349D717D7F123ULL>& hoverName = OBFUSCATE_STRING("유저인터페이스\\login_interface\\bt_otp_over.bmp", 0x7DE349D717D7F123ULL);
  button->SetBitmap(hoverName.Text(), 1, 0);
  EncryptedName<48, 0x0F15B16B5BAFD57FULL>& pressedName = OBFUSCATE_STRING("유저인터페이스\\login_interface\\bt_otp_press.bmp", 0x0F15B16B5BAFD57FULL);
  button->SetBitmap(pressedName.Text(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(186, 92);
  button->SetId(382);
  {
    const std::string text(LookupMessage(MSI_PRIVATE_AIRPLANE_WARNNING_SELECT_NPC));
    button->text = text;
    button->Invalidate();
  }
  button->AddTextStyle(0, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(1, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(2, 12, 0x313131, 1, 1, -1000, -1000);
  this->AddChild(button);
  this->m_message0 = 382;
}

// OFFSET: 0x008B84B0
// FUNCTION: fn_11764
void UILoginOTPWnd::Paint() {
  auto& resources = ResourceMgr::Instance();
  auto* background = static_cast<CBitmapRes*>(resources.Load(Resolve(OBFUSCATE_TEXT("유저인터페이스\\login_interface\\bg_newotp1.bmp", 0x2FEB8739E5975BD1ULL))));
  this->DrawBitmap(0, 0, background, false);
  this->DrawTitleBar(true, LookupMessage(MSI_PARTY_DO_YOU_WANT_BREAKING_UP), 280);
}

// OFFSET: 0x008C3620
// FUNCTION: fn_11826
void UILoginWnd::StoreInfo() {
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

  EncryptedName<3, 0xC517FB932D95BB6DULL>& idName = OBFUSCATE_STRING("ID", 0xC517FB932D95BB6DULL);
  RegSetValueExA(key, idName.Text(), 0, REG_SZ, reinterpret_cast<const BYTE*>(id), 51);

  EncryptedName<10, 0x03772763F96D9DA5ULL>& checkName = OBFUSCATE_STRING("CHECKSAVE", 0x03772763F96D9DA5ULL);
  RegSetValueExA(key, checkName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&saveId), 4);

  int bitwise = 1;
  EncryptedName<8, 0x0B7109397F6B1387ULL>& bitwiseName = OBFUSCATE_STRING("BITWISE", 0x0B7109397F6B1387ULL);
  RegSetValueExA(key, bitwiseName.Text(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&bitwise), 4);
  RegCloseKey(key);
}

// OFFSET: 0x008C1F00
// FUNCTION: fn_11819
void UIReplayRECControlWnd::ApplyMode() {
  this->Resize(194, 34);

  if (this->mode == 0) {
    this->recordStopButton->Move(-100, -100);

    for (int index = 0; index < 5; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->confirmFilenameButton->Move(-100, -100);
    this->filenameEdit->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->optionsTitleText->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->userFilenameCheckBox->Move(-100, -100);
    this->rejectExistingNameCheckBox->Move(-100, -100);
    this->filenameEdit->Move(-100, -100);
    this->recordStartButton->Move(110, 5);
    this->button_00C8->Move(136, 5);
    {
      UIButton* button = this->button_00C8;
      button->SetDisabled(0);
      button->Invalidate();
    }
    this->button_00D0->Move(162, 5);
    {
      UIButton* button = this->button_00D0;
      button->SetDisabled(0);
      button->Invalidate();
    }
    this->recordTimeText->Move(32, 12);
    this->tabs[0]->Move(6, 5);
  } else if (this->mode == 1) {
    this->recordStart = timeGetTime();
    this->recordStartButton->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->tabs[4]->Move(-100, -100);
    this->closeOptionsButton->Move(-100, -100);
    this->options[7]->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->userFilenameCheckBox->Move(-100, -100);
    this->rejectExistingNameCheckBox->Move(-100, -100);
    this->confirmFilenameButton->Move(-100, -100);
    this->filenameEdit->Move(-100, -100);
    this->recordStopButton->Move(110, 5);
    this->button_00C8->Move(136, 5);
    {
      UIButton* button = this->button_00C8;
      button->SetDisabled(1);
      button->Invalidate();
    }
    this->button_00D0->Move(162, 5);
    {
      UIButton* button = this->button_00D0;
      button->SetDisabled(1);
      button->Invalidate();
    }
    this->recordTimeText->Move(32, 12);
    this->tabs[0]->Move(6, 5);
  } else if (this->mode == 2) {
    this->Resize(194, 200);
    this->recordStartButton->Move(-100, -100);
    this->recordStopButton->Move(-100, -100);
    this->button_00C8->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->recordTimeText->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->confirmFilenameButton->Move(-100, -100);
    this->filenameEdit->Move(-100, -100);
    this->tabs[4]->Move(6, 5);
    this->closeOptionsButton->Move(162, 5);
    this->optionsTitleText->Move(42, 12);
    this->options[0]->Move(17, 63);
    this->options[1]->Move(17, 81);
    this->options[2]->Move(17, 99);
    this->options[3]->Move(17, 117);
    this->options[4]->Move(-100, -100);
    this->options[5]->Move(104, 81);
    this->options[6]->Move(104, 99);
    this->options[7]->Move(104, 117);
    this->options[9]->Move(104, 63);
    this->userFilenameCheckBox->Move(17, 160);
    this->rejectExistingNameCheckBox->Move(17, 178);
  } else if (this->mode == 3) {
    this->recordStartButton->Move(-100, -100);
    this->recordStopButton->Move(-100, -100);
    this->button_00C8->Move(-100, -100);
    this->button_00D0->Move(-100, -100);
    this->recordTimeText->Move(-100, -100);

    for (int index = 0; index < 4; ++index) {
      this->tabs[index]->Move(-100, -100);
    }

    this->tabs[4]->Move(-100, -100);
    this->closeOptionsButton->Move(-100, -100);
    this->options[7]->Move(-100, -100);

    for (int index = 0; index < 10; ++index) {
      this->options[index]->Move(-100, -100);
    }

    this->userFilenameCheckBox->Move(-100, -100);
    this->rejectExistingNameCheckBox->Move(-100, -100);
    this->confirmFilenameButton->Move(162, 5);
    this->filenameEdit->Move(7, 8);
    g_windowMgr.SetFocus(this->filenameEdit);
  }

  this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
}

// OFFSET: 0x008B9790
// FUNCTION: fn_11771
void UINoticeWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  const int leftTiles = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int index = 0; index < leftTiles; ++index) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, 8 * index + 17, left, false);
  }

  const int rightTiles = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int index = 0; index < rightTiles; ++index) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, 8 * index + 17, right, false);
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, this->geometry.width - 40, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(
    20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue)
  );
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x008BFC20
// FUNCTION: fn_11811
int UINoticeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DEFPUSH:
    this->SendMsg(nullptr, UM_COMMAND, ID_OK, 0, 0, 0);
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      FILE* file = std::fopen("notice_read.txt", "wb");
      const int index = this->m_index;
      unsigned char position;

      if (index <= 0 && this->m_scroll <= 255) {
        position = static_cast<unsigned char>(this->m_scroll);

        if (position == this->m_bottomPositions[index] - 19) {
          position = 255;
        } else if (position != 255) {
          position = static_cast<unsigned char>(this->m_scroll + 19);
          const int limit = this->m_bottomPositions[index] - 19;

          if (this->m_scroll + 19 > limit) {
            position = limit < 0 ? 0 : static_cast<unsigned char>(this->m_bottomPositions[index] - 19);
          }
        }
      } else {
        position = 255;
      }

      std::fwrite(&position, 1, 1, file);
      std::fclose(file);
      g_windowMgr.CloseWindow(WID_NOTICEWND);
      break;
    }
    case ID_NEXT:
      ++this->m_index;

      if (this->m_index > this->m_noticeCount) {
        this->m_index = this->m_noticeCount;
        return 0;
      }

      this->m_scroll = this->m_scrollPositions[this->m_index];
      this->LoadNotice();
      break;
    case ID_PREV:
      --this->m_index;

      if (this->m_index < 0) {
        this->m_index = 0;
        return 0;
      }

      this->m_scroll = this->m_scrollPositions[this->m_index];
      this->LoadNotice();
      break;
    }

    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_scroll;
    std::intptr_t delta = a;

    if (message == UM_SCROLLVERTPAGEUP) {
      delta = -1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      delta = 1;
    }

    this->m_scroll = previous + delta;

    if (this->m_scroll > this->m_bottomPositions[this->m_index] - 19) {
      this->m_scroll = this->m_bottomPositions[this->m_index] - 19;
    }

    if (this->m_scroll < this->m_scrollPositions[this->m_index]) {
      this->m_scroll = this->m_scrollPositions[this->m_index];
    }

    this->LoadNotice();
    return this->m_scroll - previous;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008BB070
// FUNCTION: fn_11793
void UINoticeWnd::OnWheel(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, 6, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, -6, 0, 0, 0);
  }
}

// OFFSET: 0x008A8AD0
// FUNCTION: fn_11715
void UIPayConfirmWnd::Configure(const std::string& text, int itemId) {
  reinterpret_cast<UIMessageBox*>(this)->Configure(text.c_str(), 2, 187, 0, LookupMessage(MSI_TITLE_QUEST_BOARD_INFO), 0xDC, 0x85, 1, nullptr);

  this->viewer->Resize(0xA5, 0x50);
  this->viewer->Move(7, 0x17);
  this->viewer->SetBackgroundColor(0xFFFF00FF);

  auto accept = std::find_if(this->m_children.begin(), this->m_children.end(), [](UIWindow* child) { return child->geometry.id == ID_YES; });
  if (this->m_children.end() != accept) {
    (*accept)->Move(0x59, 0x6D);
  }

  auto cancel = std::find_if(this->m_children.begin(), this->m_children.end(), [](UIWindow* child) { return child->geometry.id == ID_NO; });
  if (this->m_children.end() != cancel) {
    auto* button = static_cast<UIBitmapButton*>(*cancel);
    button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 1, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 2, 0);
    button->Resize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->geometry.width - 0xE, 3);
  }

  std::string resource(FindItemDescription(itemId).identifiedResource);
  if (!resource.empty()) {
    this->m_image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(std::format("유저인터페이스\\collection\\{}.bmp", resource)));
  }
}

// OFFSET: 0x008B9940
// FUNCTION: fn_11772
void UIPayConfirmWnd::Paint() {
  this->DrawTitleBar(true, this->m_title.c_str(), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);

  if (this->m_image) {
    this->DrawBitmapScaled(158, 27, 56, 75, this->m_image, 0);
  }
}

// OFFSET: 0x008B9E90
// FUNCTION: fn_11774
void UIReplayControlWnd::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\replay_interface\\replay_base.bmp")));
  this->DrawBitmap(0, 0, background, false);
}

// OFFSET: 0x008B9ED0
// FUNCTION: fn_11775
void UIReplayRECControlWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawFilledRoundedOutline(0, 0, this->geometry.width, 34, 0xF5B899);
  this->DrawRoundedOutline(0, 0, this->geometry.width, 34, 0x945A63);
  this->DrawRoundedOutline(1, 1, this->geometry.width - 2, 32, 0xF7C6AD);
  this->DrawRoundedOutline(2, 2, this->geometry.width - 4, 30, 0xFFFFFF);
  this->DrawRoundedOutline(3, 3, this->geometry.width - 6, 28, 0xAD7B84);

  if (this->mode == 2) {
    this->DrawFilledRoundedOutline(0, 34, this->geometry.width, this->geometry.height - 34, 0xDEDEDE);
    this->DrawFilledRoundedOutline(4, 38, this->geometry.width - 8, 95, 0xFFFFFF);
    this->DrawFilledRoundedOutline(4, 138, this->geometry.width - 8, 57, 0xFFFFFF);
    this->DrawOutlinedText(10, 42, LookupMessage(MSI_REPLAY_SETREPLAYSAVEDATA), 0, 0x7F3C00, 0xFFFFFF, 0, 12, 1, 0);
    this->DrawOutlinedText(10, 142, LookupMessage(MSI_REPLAY_SETRECOPTION), 0, 0x7F3C00, 0xFFFFFF, 0, 12, 1, 0);
  }
}

// OFFSET: 0x008BA010
// FUNCTION: fn_11776
void UISelectReplayDataWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_NOT_MOVEABLE_TRADE), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->FillRegion(6, 32, 268, 77, 0xFFE7E7E7);
  this->DrawTextAt(6, 19, LookupMessage(MSI_GUILD_DISBAND), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(6, 112, LookupMessage(MSI_PUT_DISBAND_GUILDNAME), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008BA0B0
// FUNCTION: fn_11777
void UISelectServerWnd::Paint() {
  const char* title = LookupMessage(MSI_REPLAY_STR_FILEINFO);

  if (g_modeMgr.GetLoginMode()->cursorType == 30) {
    title = LookupMessage(MSI_REPLAY_STR_FILELIST);
  }

  this->DrawTitleBar(true, title, 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x008BA110
// FUNCTION: fn_11778
void UITalkboxTrapInputWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(17, 3, LookupMessage(MSI_TALKBOX_WINDOW), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(16, 2, LookupMessage(MSI_TALKBOX_WINDOW), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(17, 23, LookupMessage(MSI_TALKBOX_WINDOW_MSG), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(16, 22, LookupMessage(MSI_TALKBOX_WINDOW_MSG), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008BA660
// FUNCTION: fn_11787
void UILoginWnd::OnMouseMove(int, int) {
}

// OFFSET: 0x008BADB0
// FUNCTION: fn_11790
void UIMakeCharWnd::OnProcess() {
}

// OFFSET: 0x008AC490
// FUNCTION: fn_11733
void UIMakeCharWnd::InitHairControls() {
  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int positions[2][2] = {{57, 262}, {104, 262}};
  const char* tooltips[2] = {"", ""};
  int ids[2] = {184, 185};

  for (int index = 0; index < 2; ++index) {
    auto* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], this->geometry.height - button->BitmapHeight() - 5);
    button->SetId(ids[index]);
    button->SetTooltip(tooltips[index]);
    this->AddChild(button);
  }

  this->m_rotateLeftButton = new (std::nothrow) UIBitmapButton();
  this->m_rotateLeftButton->SetBitmap("유저인터페이스\\scroll1left.bmp", 0, 0);
  this->m_rotateLeftButton->SetBitmap("유저인터페이스\\scroll1left.bmp", 1, 0);
  this->m_rotateLeftButton->SetBitmap("유저인터페이스\\scroll1left.bmp", 2, 0);
  this->m_rotateLeftButton->SetSize(this->m_rotateLeftButton->BitmapWidth(), this->m_rotateLeftButton->BitmapHeight());
  this->m_rotateLeftButton->Move(20, 80);
  this->m_rotateLeftButton->SetId(227);
  this->m_rotateLeftButton->SetTooltip(LookupMessage(MSI_VIEW_BEFORE_WORLDINFO_TITLE));
  this->AddChild(this->m_rotateLeftButton);

  this->m_rotateRightButton = new (std::nothrow) UIBitmapButton();
  this->m_rotateRightButton->SetBitmap("유저인터페이스\\scroll1right.bmp", 0, 0);
  this->m_rotateRightButton->SetBitmap("유저인터페이스\\scroll1right.bmp", 1, 0);
  this->m_rotateRightButton->SetBitmap("유저인터페이스\\scroll1right.bmp", 2, 0);
  this->m_rotateRightButton->SetSize(this->m_rotateLeftButton->BitmapWidth(), this->m_rotateLeftButton->BitmapHeight());
  this->m_rotateRightButton->Move(110, 80);
  this->m_rotateRightButton->SetId(226);
  this->m_rotateRightButton->SetTooltip(LookupMessage(MSI_VIEW_BEFORE_SERVERINFO));
  this->AddChild(this->m_rotateRightButton);

  this->m_hairStyle = this->m_info.head - 1;
  if (this->m_hairStyle < 0) {
    this->m_hairStyle = 0;
  }
  this->m_hairColor = this->m_view.headPalette >= 0 ? this->m_view.headPalette : 0;

  this->m_hairStyleBar = new UIScrollBar();
  this->m_hairStyleBar->Configure(10, 190, 0, 130);
  this->AddChild(this->m_hairStyleBar);

  this->m_hairColorBar = new UIScrollBar();
  this->m_hairColorBar->Configure(10, 230, 0, 130);
  this->AddChild(this->m_hairColorBar);

  if (this->m_hairStyleBar) {
    this->m_hairStyleBar->SetRange(22, 1, 23);
    this->m_hairStyleBar->Invalidate();
  }
  if (this->m_hairColorBar) {
    this->m_hairColorBar->SetRange(8, 1, 9);
    this->m_hairColorBar->Invalidate();
  }
  this->m_hairStyleBar->SetPosition(this->m_hairStyle);
  this->m_hairColorBar->SetPosition(this->m_hairColor);
}

// OFFSET: 0x008B0B60
// FUNCTION: fn_11743
void UIMakeCharWnd::OnCreate(int, int) {
  g_modeMgr.gameMode->SendMsg(LMM_GET_MAKING_CHAR_NAME, 0, 0, 0, 0);
  this->m_nameEdit = new UIBitmapEditCtrl;
  this->m_nameEdit->SetBitmapName("유저인터페이스\\login_interface\\name-edit.bmp");
  this->m_nameEdit->SetSize(this->m_nameEdit->geometry.width, this->m_nameEdit->geometry.height);
  this->m_nameEdit->Move(40, 142);
  this->m_nameEdit->maximum = 24;
  this->m_nameEdit->SetId(184);
  this->AddChild(this->m_nameEdit);
  this->InitHairControls();
  g_windowMgr.SetFocus(this->m_nameEdit);
  this->m_message0 = 184;
}

// OFFSET: 0x008AD130
// FUNCTION: fn_11734
void UIChatRoomWnd::Move(int x, int y) {
  this->geometry.Move(x, y);
  this->RefreshSnap();
}

// OFFSET: 0x008AD3C0
// FUNCTION: fn_11738
void UIChatRoomWnd::OnCreate(int, int) {
  this->m_message0 = 184;

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  this->resizer->SetDirection(7);
  this->resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->edit = new UIChatEditCtrl();
  this->edit->SetSize(this->geometry.width - 30, 16);
  this->edit->Move(3, this->geometry.height - 18);
  this->edit->maximum = 70;
  this->edit->Background(242, 242, 242);
  this->AddChild(this->edit);
  g_windowMgr.SetFocus(this->edit);
  this->edit->SetId(494);

  UIChatHisBox2* history = new UIChatHisBox2();
  this->history = history;
  history->Configure(0, 0, this->geometry.width - this->geometry.width * 56 / 280 - 3, this->geometry.height - 38, 0);
  this->history->Move(1, 17);
  this->history->SetId(184);
  this->AddChild(this->history);

  this->memberList = new UIMemberListBox();
  this->memberList->Configure(0, 0, this->geometry.width * 56 / 280, this->geometry.height - 38, 0);
  this->memberList->Move(this->geometry.width - this->geometry.width * 56 / 280 - 1, 17);
  this->memberList->SetId(184);
  this->AddChild(this->memberList);

  this->closeButton = this->AddCloseButton("/q", 0);

  this->titleText = new UIStaticText();
  this->titleText->SetSize(this->geometry.width - 30, 15);
  this->titleText->Move(9, 2);
  this->titleText->SetTextFitted("");
  this->titleText->SetFont(0, 12, 0, 0);
  this->AddChild(this->titleText);
}

// OFFSET: 0x008B1760
// FUNCTION: fn_11746
void UINoticeWnd::OnCreate(int, int) {
  const std::string prefix("유저인터페이스\\");
  const char* names[3][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_back", "btn_back_a", "btn_back_b"}, {"btn_next", "btn_next_a", "btn_next_b"}};
  int ids[3] = {184, 246, 218};
  int positions[3][2] = {{534, 376}, {244, 376}, {294, 376}};

  for (int index = 0; index < 3; ++index) {
    auto* button = new UIBitmapButton();
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
  this->lineCount = 0;
  for (int i = 0; i < patchSize; ++i) {
    const char c = patchText[i];
    if (c == '\n') {
      patchText[i] = 0;
      lines[lineCount] = i + 1;
      ++lineCount;
      if (lineCount > 20) {
        lineCount = 20;
      }
      ++this->lineCount;
    } else if (c == '\r') {
      patchText[i] = 0;
    } else if (c == '^') {
      const int previous = lineCount - 1;
      this->m_bottomPositions[this->m_noticeCount] = this->lineCount - 1;
      ++this->m_noticeCount;
      this->m_scrollPositions[this->m_noticeCount] = this->lineCount + 1;
      lineCount = previous < 0 ? 0 : previous;
    } else if (i < checkSize && static_cast<unsigned int>(i) < 1000 && c != checkText[i]) {
      ++changed;
    }
  }

  if (lineCount > 19) {
    lineCount = 19;
  }

  this->m_bottomPositions[this->m_noticeCount] = this->lineCount;

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

  this->m_scrollBar = new UIScrollBar();
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

// OFFSET: 0x008B42D0
// FUNCTION: fn_11749
void UIReplayRECControlWnd::OnCreate(int, int) {
  const std::string prefix("유저인터페이스\\replay_interface\\");
  const char* names[11] = {
    "btn_start_",      "btn_stop_",       "btn_start_",      "btn_save_",       "btn_setup_",      "btn_close_",
    "replay_recrun_1", "replay_recrun_2", "replay_recrun_3", "replay_recrun_4", "replay_recrun_5",
  };
  const char* tooltips[16] = {
    LookupMessage(MSI_REPLAY_START), LookupMessage(MSI_REPLAY_STOP), LookupMessage(MSI_REPLAY_START2), LookupMessage(MSI_REPLAY_OPENOPTION), LookupMessage(MSI_REPLAY_CLOSEOPION), LookupMessage(MSI_REPLAY_END),
    LookupMessage(MSI_REPLAY_TIME), LookupMessage(MSI_CHARACTER_INFO),  LookupMessage(MSI_EQUIPED_ITEM),  LookupMessage(MSI_ITEM),  LookupMessage(MSI_SKILLLIST), LookupMessage(MSI_REPLAY_PARTYFRIEND),
    LookupMessage(MSI_REPLAY_CHAT), LookupMessage(MSI_REPLAY_SHORTCUTS), LookupMessage(MSI_REPLAY_AUTOMATICFILENAME), LookupMessage(MSI_REPLAY_CHECKINGFILE),
  };
  int ids[6] = {420, 421, 422, 423, 424, 425};

  for (int index = 0; index < 2; ++index) {
    this->buttons[index] = new UIReplayButton();
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 0, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 1, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "b.bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(this->buttons[index]->BitmapWidth(), this->buttons[index]->BitmapHeight());
    this->buttons[index]->Move(-100, -100);
    this->buttons[index]->SetId(ids[index]);
    this->buttons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->buttons[index]);
  }

  for (int index = 2; index < 6; ++index) {
    this->buttons[index] = new UIBitmapButton();
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 0, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "a.bmp").c_str(), 1, 0);
    this->buttons[index]->SetBitmap((prefix + names[index] + "b.bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(this->buttons[index]->BitmapWidth(), this->buttons[index]->BitmapHeight());
    this->buttons[index]->Move(-100, -100);
    this->buttons[index]->SetId(ids[index]);
    this->buttons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->buttons[index]);
  }

  for (int index = 0; index < 5; ++index) {
    this->tabs[index] = new UIStaticBitmap();
    static_cast<UIStaticBitmap*>(this->tabs[index])->SetBitmap((prefix + names[index + 6] + ".bmp").c_str());
    this->tabs[index]->SetSize(this->tabs[index]->geometry.width, this->tabs[index]->geometry.height);
    this->tabs[index]->Move(-100, -100);
    this->tabs[index]->SetId(reinterpret_cast<int>(tooltips[index]));
    this->AddChild(this->tabs[index]);
  }

  int textSizes[2][2] = {{50, 12}, {80, 12}};
  const char* labels[2] = {"000:00:00", LookupMessage(MSI_REPLAY_REPLAYOPTIONSETTING)};
  for (int index = 0; index < 2; ++index) {
    this->texts[index] = new UIStaticText();
    this->texts[index]->SetSize(textSizes[index][0], textSizes[index][1]);
    this->texts[index]->SetText(labels[index], 0);
    this->texts[index]->Move(-100, -100);
    this->texts[index]->SetColor(255, 255, 255);
    this->AddChild(this->texts[index]);
  }

  std::memcpy(this->recordOptions, &Replay::Instance().header.options, sizeof(this->recordOptions));
  const char* optionNames[12] = {
    "Basicinfo", "Equip", "Item", "Skill", "Guild", "Party", "Chatting", "Shortcut", "Status", "ALL", "User Defined File Name", "Repeated File Check",
  };
  for (int index = 0; index < 10; ++index) {
    this->options[index] = new UICheckBox();
    this->options[index]->SetSize(70, 12);
    static_cast<UICheckBox*>(this->options[index])->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", optionNames[index]);
    this->options[index]->Move(-100, -100);
    static_cast<UICheckBox*>(this->options[index])->SetChecked(Replay::Instance().header.options.Get(static_cast<ReplayRecordOption>(index)));
    this->AddChild(this->options[index]);
  }

  for (int index = 0; index < 2; ++index) {
    this->fileOptions[index] = new UICheckBox();
    this->fileOptions[index]->SetSize(150, 12);
    this->fileOptions[index]->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", optionNames[index + 10]);
    this->fileOptions[index]->Move(-100, -100);
    this->fileOptions[index]->SetChecked(0);
    this->AddChild(this->fileOptions[index]);
  }

  const bool pathEnabled = ReplayFileList::Instance().pathEnabled;
  this->fileOptions[0]->SetChecked(pathEnabled);
  this->replayPathEnabled = pathEnabled;
  const bool option119 = ReplayFileList::Instance().rejectExistingName;
  this->fileOptions[1]->SetChecked(option119);
  this->replayOption119 = option119;

  this->filenameEdit = new UIEditCtrl();
  this->filenameEdit->SetSize(151, 18);
  this->filenameEdit->Move(-100, -100);
  this->filenameEdit->maximum = 80;
  this->filenameEdit->Background(232, 232, 232);
  this->filenameEdit->SetText("");
  std::string& tooltip = this->filenameEdit->tooltip;
  if (const char* text = LookupMessage(MSI_REPLAY_ENTERTHEFILENAME)) {
    tooltip = text;
  }
  this->AddChild(this->filenameEdit);
  this->filenameEdit->SelectAll();
  this->ApplyMode();
  this->Invalidate();
}

// OFFSET: 0x008B5A50
// FUNCTION: fn_11750
void UISelectReplayDataWnd::OnCreate(int width, int height) {
  this->m_list = new UIReplayListBox();
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

  std::string name;
  for (int index = 0; index < 4; ++index) {
    auto* button = new UIBitmapButton();
    name.assign(prefix);
    name += names[index][0];
    name += ".bmp";
    button->SetBitmap(name.c_str(), 0, 0);
    name.assign(prefix);
    name += names[index][1];
    name += ".bmp";
    button->SetBitmap(name.c_str(), 1, 0);
    name.assign(prefix);
    name += names[index][2];
    name += ".bmp";
    button->SetBitmap(name.c_str(), 2, 0);
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
    *text = new UIStaticText();
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
void UITalkboxTrapInputWnd::OnCreate(int, int) {
  this->edit = new UIEditCtrl();
  this->edit->SetSize(240, 16);
  this->edit->Move(22, 50);
  this->edit->maximum = 78;
  this->edit->Background(242, 242, 242);
  this->AddChild(this->edit);
  g_windowMgr.SetFocus(this->edit);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{189, 96}, {234, 96}};

  for (int index = 0; index < 2; ++index) {
    auto* button = new UIBitmapButton();
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
void UILoginOTPWnd::OnDestroy() {
}

// OFFSET: 0x008B7A70
// FUNCTION: fn_11754
void UILoginWnd::OnDestroy() {
}

// OFFSET: 0x008B7A80
// FUNCTION: fn_11755
void UIMakeCharWnd::OnDestroy() {
}

// OFFSET: 0x008B8E90
// FUNCTION: fn_11769
void UIMessageBoxAutoreturn::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox.bmp")));
  this->DrawBitmap(0, 0, background, false);
}

// OFFSET: 0x008BA670
// FUNCTION: fn_11788
void UILoginOTPWnd::OnProcess() {
  auto* login = static_cast<CLoginMode*>(g_modeMgr.GetLoginMode());
  if (login == nullptr) {
    return;
  }

  const unsigned remaining = static_cast<unsigned>(60.0 - (timeGetTime() - login->otpStarted) * 0.001);
  UIStaticText* timer = this->timerText;
  timer->SetText(FormatOwnedString(OBFUSCATE_TEXT_NAMED("%02d : %02d", 0x7941D76335CFA14FULL, format), remaining / 60, remaining % 60).c_str(), 0);
  this->Invalidate();
}

// OFFSET: 0x008BAEF0
// FUNCTION: fn_11792
void UIMessageBoxAutoreturn::OnProcess() {
  if (this->counting == 0) {
    return;
  }

  if (timeGetTime() > this->deadline) {
    g_windowMgr.modalResponse = 187;
    this->counting = 0;
    return;
  }

  const unsigned previous = this->remaining;
  const unsigned fraction = (this->deadline - timeGetTime()) % 1000 ? 1 : 0;
  this->remaining = (this->deadline - timeGetTime()) / 1000 + fraction;

  if (previous != this->remaining) {
    char text[512];
    std::sprintf(text, this->format.c_str(), this->remaining);
    this->messageText = text;
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->messageText.data()), 38, 0, 0);
  }

  UIBarGraph2* gauge = this->gauge;
  const unsigned duration = this->seconds * 1000;
  const unsigned progress = (timeGetTime() + (duration - this->deadline)) * 100 / duration;
  gauge->SetValue(static_cast<float>(progress));
}

// OFFSET: 0x008AA9D0
// FUNCTION: fn_11723
void UIItemWnd::DrawNewItemMark(int x, int y) {
  this->DrawBitmap(x, y, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\new_item.bmp"))), false);
}

// OFFSET: 0x008AAA10
// FUNCTION: fn_11724
void UIMonsterInfoWnd::DrawResistance(int x, int y, const char* label, int value, std::uint32_t border, std::uint32_t fill) {
  char text[64];
  wsprintfA(text, "%s:%d", label, value);
  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(22, 2, red, green, blue);
  this->FillRegion(x, y, 70, 16, border);
  this->FillRegion(x + 1, y + 1, 68, 14, fill);
  this->DrawTextAt((70 - this->TextWidth(text, 0, 0, 12, 0, 0)) / 2 + x, y + 2, text, 0, 0, 12, RGB(red, blue, green), 0, 0);
}

// OFFSET: 0x008B8ED0
// FUNCTION: fn_11770
void UIMonsterInfoWnd::Paint() {
  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  const std::uint32_t fill = RGB(red, green, blue);
  this->ClearSurface(fill);
  this->DrawPanelBackground(this->DrawTitleBar(false, "", 0), 0, "");
  this->DrawButtonBar(2, 0, 1);
  g_skinInfo.PaletteChannels(22, 2, red, green, blue);
  const std::uint32_t border = RGB(red, green, blue);

  this->DrawTextAt(17, 3, LookupMessage(MSI_MONSTER_INFO_WINDOW), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(16, 2, LookupMessage(MSI_MONSTER_INFO_WINDOW), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(120, 30, LookupMessage(MSI_NAME), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(160, 30, this->name.c_str(), 0, 0, 12, 0, 0, 0);

  char text[256];
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.level);
  this->DrawTextAt(120, 50, LookupMessage(MSI_LEVEL), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(160, 50, text, 0, 0, 12, 0, 0, 0);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.hp);
  this->DrawTextAt(120, 70, LookupMessage(MSI_HP), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(160, 70, text, 0, 0, 12, 0, 0, 0);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.defense);
  this->DrawTextAt(120, 90, LookupMessage(MSI_ZEROQUEST_ALLSELECT), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(160, 90, text, 0, 0, 12, 0, 0, 0);

  this->DrawTextAt(260, 30, LookupMessage(MSI_SIZE), 0, 0, 12, 0, 0, 0);
  switch (this->info.size) {
  case SZ_SMALL:
    this->DrawTextAt(300, 30, LookupMessage(MSI_MONSTERINFO_DEF), 0, 0, 12, 0, 0, 0);
    break;
  case SZ_MEDIUM:
    this->DrawTextAt(300, 30, LookupMessage(MSI_BAN_TEENAGER), 0, 0, 12, 0, 0, 0);
    break;
  case SZ_BIG:
    this->DrawTextAt(300, 30, LookupMessage(MSI_STYLINGSHOP_COLOR_RESET), 0, 0, 12, 0, 0, 0);
    break;
  }

  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.race);
  this->DrawTextAt(260, 50, LookupMessage(MSI_RACETYPE), 0, 0, 12, 0, 0, 0);
  switch (this->info.race) {
  case RC_FORMLESS:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_TRIBE_PLANT), 0, 0, 12, 0, 0, 0);
    break;
  case RC_UNDEAD:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_TRIBE_INSECT), 0, 0, 12, 0, 0, 0);
    break;
  case RC_BRUTE:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_TRIBE_MARINE), 0, 0, 12, 0, 0, 0);
    break;
  case RC_PLANT:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_TRIBE_DEVIL), 0, 0, 12, 0, 0, 0);
    break;
  case RC_INSECT:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_TRIBE_HUMAN), 0, 0, 12, 0, 0, 0);
    break;
  case RC_FISH:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_TRIBE_ANGEL), 0, 0, 12, 0, 0, 0);
    break;
  case RC_DEMON:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_TRIBE_DRAGON), 0, 0, 12, 0, 0, 0);
    break;
  case RC_DEMIHUMAN:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_PROPERTY_NEUTURAL), 0, 0, 12, 0, 0, 0);
    break;
  case RC_ANGEL:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_PROPERTY_WATER), 0, 0, 12, 0, 0, 0);
    break;
  case RC_DRAGON:
    this->DrawTextAt(300, 50, LookupMessage(MSI_MONSTERINFO_PROPERTY_EARTH), 0, 0, 12, 0, 0, 0);
    break;
  }

  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d", this->info.magicDefense);
  this->DrawTextAt(260, 90, LookupMessage(MSI_MDEFPOWER), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(300, 90, text, 0, 0, 12, 0, 0, 0);

  this->DrawTextAt(260, 70, LookupMessage(MSI_PROPERTY), 0, 0, 12, 0, 0, 0);
  switch (this->info.element % 20) {
  case ELE_NEUTRAL:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_PROPERTY_FIRE), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_WATER:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_PROPERTY_WIND), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_EARTH:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_PROPERTY_POISON), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_FIRE:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_PROPERTY_SAINT), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_WIND:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_PROPERTY_DARK), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_POISON:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_PROPERTY_MENTAL), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_HOLY:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_PROPERTY_UNDEAD), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_DARK:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_SIZE_SMALL), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_GHOST:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_SIZE_MIDDLE), 0, 0, 12, 0, 0, 0);
    break;
  case ELE_UNDEAD:
    this->DrawTextAt(300, 70, LookupMessage(MSI_MONSTERINFO_SIZE_BIG), 0, 0, 12, 0, 0, 0);
    break;
  }

  this->DrawResistance(120, 110, LookupMessage(MSI_MONSTERINFO_PROPERTY_WIND), this->info.resistances[0], border, fill);
  this->DrawResistance(120, 128, LookupMessage(MSI_MONSTERINFO_PROPERTY_POISON), this->info.resistances[1], border, fill);
  this->DrawResistance(120, 146, LookupMessage(MSI_MONSTERINFO_PROPERTY_SAINT), this->info.resistances[2], border, fill);
  this->DrawResistance(200, 110, LookupMessage(MSI_MONSTERINFO_PROPERTY_DARK), this->info.resistances[3], border, fill);
  this->DrawResistance(200, 128, LookupMessage(MSI_MONSTERINFO_PROPERTY_MENTAL), this->info.resistances[4], border, fill);
  this->DrawResistance(200, 146, LookupMessage(MSI_MONSTERINFO_PROPERTY_UNDEAD), this->info.resistances[5], border, fill);
  this->DrawResistance(280, 110, LookupMessage(MSI_MONSTERINFO_SIZE_SMALL), this->info.resistances[6], border, fill);
  this->DrawResistance(280, 128, LookupMessage(MSI_MONSTERINFO_SIZE_MIDDLE), this->info.resistances[7], border, fill);
  this->DrawResistance(280, 146, LookupMessage(MSI_MONSTERINFO_SIZE_BIG), this->info.resistances[8], border, fill);

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

  this->tabs[static_cast<int>(timeGetTime() % 1000) / 250]->Move(6, 5);
  if (timeGetTime() - this->recordStart <= 200) {
    return;
  }

  const int elapsed = static_cast<int>(timeGetTime() - Replay::Instance().started) / 1000;
  const int totalMinutes = elapsed / 60;
  const int seconds = elapsed % 60;
  const int hours = totalMinutes / 60;
  const int minutes = totalMinutes % 60;
  if (hours == 100) {
    this->recordTimeText->Move(124, 47);
  } else if (hours == 1000) {
    this->recordTimeText->Move(116, 47);
  }

  char text[100] = {};
  std::sprintf(text, "%03d:%02d:%02d", hours, minutes, seconds);
  this->recordTimeText->SetTextFitted(text);
  this->Invalidate();
  this->InvalidateChildren();
  this->recordStart = timeGetTime();
}

// OFFSET: 0x008C0DF0
// FUNCTION: fn_11814
int UIReplayRECControlWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<ReplayRECControlWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_START_REPLAY_RECORDING:
      if (!this->replayPathEnabled) {
        this->mode = 1;
        g_modeMgr.gameMode->SendMsg(MM_START_REPLAY_RECORDING, 0, 0, 0, 0);
        this->ApplyMode();
        return 0;
      }

      this->mode = 3;
      this->ApplyMode();
      return 0;
    case ID_STOP_REPLAY_RECORDING:
      this->mode = 0;
      this->ApplyMode();
      g_modeMgr.gameMode->SendMsg(MM_STOP_REPLAY_RECORDING, 0, 0, 0, 0);
      return 0;
    case ID_CONFIRM_REPLAY_RECORDING_PATH: {
      auto* edit = static_cast<UIEditCtrl*>(this->filenameEdit);
      if (edit->Length() <= 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_REPLAY_ENTERTHEFILENAME), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }

      if (_mbsstr(reinterpret_cast<const unsigned char*>(edit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkOpening.c_str())) ||
          _mbsstr(reinterpret_cast<const unsigned char*>(edit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkClosing.c_str()))) {
        g_windowMgr.Confirm(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }

      ReplayFileList::Instance().SetPath(edit->Text());
      g_windowMgr.SetFocus(nullptr);
      edit->SetText("");
      g_modeMgr.gameMode->SendMsg(MM_START_REPLAY_RECORDING, 0, 0, 0, 0);

      if (Replay::Instance().state == 1) {
        return 0;
      }

      this->mode = 1;
      this->ApplyMode();
      return 0;
    }
    case ID_REPLAY_RECORDING_OPTIONS:
      this->mode = 2;
      this->ApplyMode();
      return 0;
    case ID_CANCEL_REPLAY_RECORDING_OPTIONS:
      this->mode = 0;
      this->ApplyMode();
      return 0;
    case ID_CLOSE_REPLAY_RECORDING:
      this->mode = 0;
      g_windowMgr.CloseWindow(WID_REPLAYRECCONTROLWND);
      return 0;
    case ID_CHECK:
      if (sender == this->userFilenameCheckBox) {
        ReplayFileList::Instance().pathEnabled = b != 0;
        this->replayPathEnabled = ReplayFileList::Instance().pathEnabled;
        return 0;
      }

      if (sender == this->rejectExistingNameCheckBox) {
        ReplayFileList::Instance().rejectExistingName = b != 0;
        this->replayOption119 = ReplayFileList::Instance().rejectExistingName;
        return 0;
      }

      if (sender == this->options[9]) {
        if (b == 1) {
          for (int index = 0; index < 10; ++index) {
            if (index != 9) {
              this->recordOptions[index] = 1;
              static_cast<UICheckBox*>(this->options[index])->SetChecked(1);
            }
          }
        }

        this->recordOptions[9] = b != 0;
      } else if (sender == this->options[0]) {
        if (b == 0) {
          for (int index = 1; index < 10; ++index) {
            this->recordOptions[index] = 0;
            static_cast<UICheckBox*>(this->options[index])->SetChecked(0);
          }
        }

        this->recordOptions[0] = b != 0;
      } else {
        if (b == 1) {
          if (this->options[0]->geometry.state == 0) {
            static_cast<UICheckBox*>(this->options[0])->SetChecked(1);
            this->recordOptions[0] = 1;
          }
        } else if (this->options[9]->geometry.state == 1) {
          static_cast<UICheckBox*>(this->options[9])->SetChecked(0);
          this->recordOptions[9] = 0;
        }

        for (int index = 1; index < 9; ++index) {
          if (sender == this->options[index]) {
            this->recordOptions[index] = b != 0;
          }
        }
      }

      Replay::Instance().SetOptions(this->recordOptions, 10);
      return 0;
    default:
      return 0;
    }
  case UM_UPDATE_RECORD_TIME:
    this->UpdateRecordTime();
    return 0;
  case UM_SETPOS:
    Replay::Instance().recordingState = this->geometry.x;
    Replay::Instance().playbackState = this->geometry.y;
    return 0;
  case UM_REFRESH:
    this->Invalidate();
    this->InvalidateChildren();
    return 0;
  case UM_REPLAY_FRAMED_DATA:
  case 251:
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008BA820
// FUNCTION: fn_11789
void UILoginWnd::OnProcess() {
  if (g_windowMgr.GetHoverWindow() == this->idEdit) {
    this->idEdit->TextColor(0, 0, 0, 0xFFFFFFFF);
    if (this->idPlaceholder == true) {
      EncryptedName<3, 0x2D51E3B7236175EDULL>& placeholder = OBFUSCATE_STRING("ID", 0x2D51E3B7236175EDULL);
      if (std::strcmp(placeholder.Text(), this->idEdit->Text()) == 0) {
        this->idEdit->SetText("");
        goto password;
      }
    }
    this->idPlaceholder = false;
  } else {
    if (std::strcmp("", this->idEdit->Text()) == 0) {
      this->idEdit->TextColor(156, 156, 156, 0xFFFFFFFF);
      UIEditCtrl_Custom* edit = this->idEdit;
      this->idPlaceholder = true;
      edit->SetText(OBFUSCATE_STRING("ID", 0x0F41EBBDA32D1B17ULL).Text());
    }
    if (this->idPlaceholder == false) {
      this->idEdit->TextColor(0, 0, 0, 0xFFFFFFFF);
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
    this->passwordEdit->TextColor(0, 0, 0, 0xFFFFFFFF);
    if (this->passwordPlaceholder == true) {
      EncryptedName<9, 0x7115D9F973F913C9ULL>& placeholder = OBFUSCATE_STRING("PASSWORD", 0x7115D9F973F913C9ULL);
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
      this->passwordEdit->TextColor(156, 156, 156, 0xFFFFFFFF);
      auto* edit = this->passwordEdit;
      edit->SetText(OBFUSCATE_STRING("PASSWORD", 0xCB1D253B7FE1F563ULL).Text());
      this->passwordEdit->mask = 0;
    }
    if (this->passwordPlaceholder != false) {
      return;
    }
    this->passwordEdit->TextColor(0, 0, 0, 0xFFFFFFFF);
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
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF00, 0, 0, 0);
  }
}

// OFFSET: 0x008AAAF0
// FUNCTION: fn_11725
std::vector<unsigned char> UIChatRoomWnd::SerializeReplayData() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(0x5014, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(0x5016, this->saved, 0x10);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(0x5017, this->title.data(), static_cast<std::uint32_t>(this->title.size()));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(0x5018, this->roomSettingsText.data(), static_cast<std::uint32_t>(this->roomSettingsText.size()));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(0x5019, this->roomStatusValues, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(0x501A, this->roomStatusValues + 4, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(0x501B, this->roomStatusValues + 8, 4);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(0x501C, this->roomStatusValues + 12, 4);
    records.insert(records.end(), record.begin(), record.end());
  }

  for (const auto& entry : this->memberRoles) {
    if (entry.second == 0) {
      const auto record = BuildReplayTaggedRecord(0x501E, entry.first.data(), static_cast<std::uint32_t>(entry.first.size()));
      records.insert(records.end(), record.begin(), record.end());
    }
  }

  for (const auto& entry : this->memberRoles) {
    if (entry.second != 0) {
      const auto record = BuildReplayTaggedRecord(0x501F, entry.first.data(), static_cast<std::uint32_t>(entry.first.size()));
      records.insert(records.end(), record.begin(), record.end());
    }
  }

  {
    const auto record = BuildReplayTaggedRecord(0x5015, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x008ABDB0
// FUNCTION: fn_11728
void UILoginWnd::LoadLogin() {
  HKEY probe;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &probe) == ERROR_SUCCESS) {
    RegCloseKey(probe);
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS) {
      return;
    }

    DWORD type = REG_DWORD;
    DWORD size = 4;
    int saveId;
    EncryptedName<10, 0xF14F79170939C7F1ULL>& checkName = OBFUSCATE_STRING("CHECKSAVE", 0xF14F79170939C7F1ULL);
    RegQueryValueExA(key, checkName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(&saveId), &size);
    if (saveId != 1) {
      saveId = 0;
    }

    int bitwise;
    EncryptedName<8, 0xA5FD29D7AB6DC59FULL>& bitwiseName = OBFUSCATE_STRING("BITWISE", 0xA5FD29D7AB6DC59FULL);
    RegQueryValueExA(key, bitwiseName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(&bitwise), &size);

    char id[52];
    std::memset(id, 0, sizeof(id));
    char password[26] = {};
    type = REG_SZ;
    size = 25;
    EncryptedName<9, 0xDD3913D5D7F97BE1ULL>& passwordName = OBFUSCATE_STRING("PASSWORD", 0xDD3913D5D7F97BE1ULL);
    RegQueryValueExA(key, passwordName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(password), &size);
    password[24] = 0;

    type = REG_SZ;
    size = 51;
    EncryptedName<3, 0x050505679903579BULL>& idName = OBFUSCATE_STRING("ID", 0x050505679903579BULL);
    RegQueryValueExA(key, idName.Text(), nullptr, &type, reinterpret_cast<BYTE*>(id), &size);
    id[50] = 0;
    password[24] = 0;
    this->saveId = saveId;

    if (saveId != 0) {
      unsigned char shade;
      bool placeholder = std::strcmp("", id) == 0;
      if (!placeholder) {
        EncryptedName<3, 0x83EFBFBB37EDBDBDULL>& compareName = OBFUSCATE_STRING("ID", 0x83EFBFBB37EDBDBDULL);
        placeholder = std::strcmp(compareName.Text(), id) == 0;
      }

      if (!placeholder) {
        shade = 0;
        this->idPlaceholder = false;
      } else {
        this->idPlaceholder = true;
        UIEditCtrl_Custom* edit = this->idEdit;
        EncryptedName<3, 0xBF2B4B31397FF5A7ULL>& textName = OBFUSCATE_STRING("ID", 0xBF2B4B31397FF5A7ULL);
        edit->SetText(textName.Text());
        shade = 156;
      }

      this->idEdit->TextColor(shade, shade, shade, 0xFFFFFFFF);
      this->idEdit->SetText(id);
      this->passwordEdit->SetText("");
    }

    RegCloseKey(key);
  } else {
    this->idEdit->SetText("");
    this->passwordEdit->SetText("");
  }
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
UINoticeWnd::UINoticeWnd() : UIFrameWnd(0) {
  this->m_patchFile = "notice_patch.txt";
  for (int i = 0; i < 19; ++i) {
    this->m_texts[i] = new UIStaticText();
  }
  this->lineCount = 20;
  this->padding_010C = 0;
  this->m_noticeCount = 0;
  this->m_index = 0;
  this->m_scrollPositions[0] = 0;
  this->m_bottomPositions[0] = 0;
  this->m_scroll = 0;
  g_windowMgr.SetFocus(this);
}

// OFFSET: 0x008AD190
// FUNCTION: fn_11737
void UIBank_NewWnd::OnCreate(int, int) {
  this->AddCloseButton("Exit", 0);
  this->AddBitmapButton(462, 209, 77, "bank\\btn_deposit", LookupMessage(MSI_BANK_7));
  this->AddBitmapButton(463, 209, 102, "bank\\btn_withdraw", LookupMessage(MSI_BANK_MAX));
  this->AddBitmapButton(279, 177, 89, "bank\\btn_upper", LookupMessage(MSI_BANK_NOMSG));
  this->AddBitmapButton(280, 177, 99, "bank\\btn_buttom", LookupMessage(MSI_BANK_0_MINI));
  this->AddBitmapButton(392, 189, 132, "bank\\btn_10mil", "+ 100,000");
  this->AddBitmapButton(391, 141, 132, "bank\\btn_100mil", "+ 1,000,000");
  this->AddBitmapButton(390, 93, 132, "bank\\btn_1000mil", "+ 10,000,000");
  this->AddBitmapButton(389, 45, 132, "bank\\btn_max", LookupMessage(MSI_BANK_1_MINI));

  this->m_amountEdit = new UIEditCtrl();
  this->m_amountEdit->SetSize(99, 17);
  this->m_amountEdit->maximum = 10;
  this->m_amountEdit->Move(78, 90);
  this->m_amountEdit->Background(247, 247, 247);
  this->m_amountEdit->Border(198, 198, 198);
  this->m_amountEdit->flat = 0;
  this->m_amountEdit->topPadding = 1;
  this->AddChild(this->m_amountEdit);
  g_windowMgr.SetFocus(this->m_amountEdit);
}

// OFFSET: 0x008AD770
// FUNCTION: fn_11739
void UICntWnd::OnCreate(int, int) {
  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(80, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 6;
  this->m_edit->Background(232, 232, 232);
  this->AddChild(this->m_edit);
  g_windowMgr.SetFocus(this->m_edit);
  this->m_edit->SelectAll();

  this->m_text = new UIStaticText();
  this->m_text->SetSize(this->geometry.width - 30, 15);
  this->m_text->Move(15, 6);
  this->AddChild(this->m_text);

  {
    const std::string prefix("유저인터페이스\\");
    const int x = this->geometry.width - 55;
    const int y = this->geometry.height - 32;
    UIBitmapButton* button = new UIBitmapButton();
    button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(x, y);
    button->SetId(184);
    this->AddChild(button);
  }
  this->m_message0 = 184;
}

// OFFSET: 0x008C0B20
// FUNCTION: fn_11813
int UIReplayControlWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case ID_PAUSE_REPLAY:
      g_modeMgr.gameMode->SendMsg(MM_PAUSE_REPLAY, 0, 0, 0, 0);
      return 0;
    case ID_RESUME_REPLAY:
      g_modeMgr.gameMode->SendMsg(MM_RESUME_REPLAY, 0, 0, 0, 0);
      return 0;
    case ID_RESTART_REPLAY:
      g_modeMgr.gameMode->SendMsg(MM_RESTART_REPLAY, 0, 0, 0, 0);
      Replay::Instance().speed = 1.0f;
      return 0;
    case ID_EXIT_REPLAY:
      g_windowMgr.Schedule(this);
      g_modeMgr.gameMode->SendMsg(MM_EXIT_REPLAY, 0, 0, 0, 0);
      Replay::Instance().speed = 1.0f;
      return 0;
    case ID_DECREASE_REPLAY_SPEED:
      g_modeMgr.gameMode->SendMsg(MM_DECREASE_REPLAY_SPEED, 0, 0, 0, 0);
      this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      return 0;
    case ID_INCREASE_REPLAY_SPEED:
      g_modeMgr.gameMode->SendMsg(MM_INCREASE_REPLAY_SPEED, 0, 0, 0, 0);
      this->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      return 0;
    }
    return 0;
  case UM_REFRESHBUTTON:
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
  case UM_REFRESH:
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
  const int elapsed = static_cast<int>(Replay::Instance().elapsed) / 1000;
  const int totalMinutes = elapsed / 60;
  const int seconds = elapsed % 60;
  const int hours = totalMinutes / 60;
  const int minutes = totalMinutes % 60;
  const std::uint32_t duration = Replay::Instance().Duration();
  const int durationHours = duration & 0xFFFF;
  const int durationMinutes = static_cast<char>(duration >> 16);
  const int durationSeconds = static_cast<char>(duration >> 24);
  char text[100];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, LookupMessage(MSI_REPLAY_ELAPSEDTIME), hours, minutes, seconds, durationHours, durationMinutes, durationSeconds);
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

// OFFSET: 0x008C14F0
// FUNCTION: fn_11815
int UISelectReplayDataWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETCURITEM:
    this->m_list->SendMsg(nullptr, UM_SETCURITEM, a, 0, 0, 0);
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case UM_UPPUSH:
    this->m_list->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case UM_DOWNPUSH:
    this->m_list->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_CANCEL:
    case ID_SELECT_REPLAY:
      Replay::Instance().SetState(1);
      g_windowMgr.Schedule(this);
      g_modeMgr.gameMode->SendMsg(LMM_REPLAY_SELECT_SERVER, 0, 0, 0, 0);
      return 0;
    case ID_OK:
      if (this->m_list->selected != -1) {
        this->UpdateInfo();
        this->Invalidate();
        if (ReplayFileList::Instance().Open(32, 0, 0) == 1) {
          if (Replay::Instance().MatchesService(g_serviceMode)) {
            g_modeMgr.gameMode->SendMsg(LMM_START_REPLAY, 0, 0, 0, 0);
            g_windowMgr.Schedule(this);
            g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
            return 0;
          }
          Replay::Instance().CloseFile();
          g_windowMgr.Confirm(LookupMessage(MSI_REPLAY_SERVERNOMATCH), 0, true, true, 0, "Message", 280, 120, 0);
        } else {
          g_windowMgr.Confirm(LookupMessage(MSI_REPLAY_NOREPLAYFILE), 0, true, true, 0, "Message", 280, 120, 0);
        }
      }
      break;
    case ID_EXIT:
      if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT), 2, true, false, 0, "Message", 280, 120, 0) == ID_YES) {
        g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
        return 0;
      }
      break;
    }
    break;
  case UM_SET_TARGET:
    this->UpdateInfo();
    this->Invalidate();
    return 0;
  case 89:
    this->m_serverButton->Move(234, this->m_serverButton->geometry.y);
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008C2460
// FUNCTION: fn_11820
bool UISelectReplayDataWnd::UpdateInfo() {
  if (ReplayFileList::Instance().Select(this->m_list->selected)) {
    if (ReplayFileList::Instance().Open(32, 0, 0) == 1) {
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
        this->m_texts[1]->SetTextFitted(LookupMessage(MSI_REPLAY_NOREPLAYFILE));
        this->m_texts[2]->SetTextFitted("");
        this->m_texts[3]->SetTextFitted("");
        this->m_texts[4]->SetTextFitted("");
        return false;
      }

      std::sprintf(buffer, LookupMessage(MSI_REPLAY_CHRVICEINFO), service.c_str());
      this->m_texts[0]->SetTextFitted(buffer);
      std::memset(buffer, 0, sizeof(buffer));

      char name[64];
      std::memcpy(name, Replay::Instance().header.characterName, sizeof(name));
      std::sprintf(buffer, LookupMessage(MSI_REPLAY_CHARACTERNAME), name);
      this->m_texts[1]->SetTextFitted(buffer);

      std::sprintf(buffer, LookupMessage(MSI_REPLAY_MAPNAME), Replay::Instance().header.mapName);
      this->m_texts[2]->SetTextFitted(buffer);

      const std::uint32_t date = Replay::Instance().header.key0;
      const std::uint32_t time = Replay::Instance().header.key1;
      const int second = static_cast<char>(time >> 24);
      const int minute = static_cast<char>(time >> 16);
      const int hour = static_cast<char>(time >> 8);
      const int day = static_cast<char>(date >> 24);
      const int month = static_cast<char>(date >> 16);
      const int year = date & 0xFFFF;
      const char* recordTimeFormat = LookupMessage(MSI_REPLAY_RECORDTIME);
      std::sprintf(buffer, recordTimeFormat, year, month, day, hour, minute, second);
      this->m_texts[3]->SetTextFitted(buffer);

      const std::uint32_t duration = Replay::Instance().Duration();
      const int seconds = static_cast<char>(duration >> 24);
      const int minutes = static_cast<char>(duration >> 16);
      const int hours = duration & 0xFFFF;
      const char* playTimeFormat = LookupMessage(MSI_REPLAY_PLAYTIME);
      std::sprintf(buffer, playTimeFormat, hours, minutes, seconds);
      this->m_texts[4]->SetTextFitted(buffer);
    } else {
      this->m_texts[0]->SetTextFitted("");
      this->m_texts[1]->SetTextFitted(LookupMessage(MSI_REPLAY_NOREPLAYFILE));
      this->m_texts[2]->SetTextFitted("");
      this->m_texts[3]->SetTextFitted("");
      this->m_texts[4]->SetTextFitted("");
    }
  } else {
    this->m_texts[0]->SetTextFitted("");
    this->m_texts[1]->SetTextFitted("");
    this->m_texts[2]->SetTextFitted("");
    this->m_texts[3]->SetTextFitted("");
    this->m_texts[4]->SetTextFitted("");
  }
  return true;
}

// OFFSET: 0x008C18C0
// FUNCTION: fn_11816
int UISelectServerWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETCURITEM:
    this->m_serverList->SendMsg(nullptr, UM_SETCURITEM, this->m_selectedServer, 0, 0, 0);
    return 0;
  case UM_UPPUSH:
    this->m_serverList->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
    return 0;
  case UM_DOWNPUSH:
    this->m_serverList->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    return 0;
  case UM_SET_TARGET: {
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
  case UM_COMMAND:
    switch (a) {
    case ID_SELECT_REPLAY:
      ReplayFileList::Instance().SetReplay(&Replay::Instance());
      ReplayFileList::Instance().selectionState = g_session.m_aid;
      ReplayFileList::Instance().dialogState = g_session.GetCurrentSex();
      g_modeMgr.gameMode->SendMsg(LMM_SELECT_REPLAY_DATA, 0, 0, 0, 0);
      g_windowMgr.Schedule(this);
      return 0;
    case ID_OK:
      if (this->m_serverList->selected != -1) {
        g_windowMgr.Schedule(this);
        g_modeMgr.gameMode->SendMsg(this->m_event, this->m_serverList->selected, 0, 0, 0);
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
    case ID_CANCEL:
      g_windowMgr.Schedule(this);
      g_modeMgr.gameMode->SendMsg(LMM_CANCEL, 0, 0, 0, 0);
      return 0;
    case ID_EXIT:
      if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT), 2, true, false, 0, "Message", 280, 120, 0) == ID_YES) {
        g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
        return 0;
      }
      break;
    }
    break;
  case 89:
    this->m_okButton->Move(234, this->m_okButton->geometry.y);
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x008B99A0
// FUNCTION: fn_11773
void UIPetInfoWnd::Paint() {
  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  this->DrawPanelBackground(this->DrawTitleBar(false, "", 0), 0, "");

  auto* leftCorner = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftCorner->image.height, leftCorner, false);
  auto* rightCorner = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightCorner->image.width, this->geometry.height - rightCorner->image.height, rightCorner, false);

  char title[32];
  StringFormat(title, 32, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_PET_INFO));
  this->DrawTextAt(5, 2, title, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 24, LookupMessage(MSI_NAME), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 44, LookupMessage(MSI_LEVEL), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 69, LookupMessage(MSI_PET_HUNGRY), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 89, LookupMessage(MSI_PET_FRIENDLY), 0, 0, 12, 0, 0, 0);

  char level[8];
  char hunger[24];
  if (g_session.petHunger >= 0 && g_session.petHunger <= 10) {
    std::strcpy(hunger, LookupMessage(MSI_VERY_HUNGRY));
  } else if (g_session.petHunger > 10 && g_session.petHunger <= 25) {
    std::strcpy(hunger, LookupMessage(MSI_HUNGRY));
  } else if (g_session.petHunger > 25 && g_session.petHunger <= 75) {
    std::strcpy(hunger, LookupMessage(MSI_NORMAL));
  } else if (g_session.petHunger > 75 && g_session.petHunger <= 90) {
    std::strcpy(hunger, LookupMessage(MSI_REPLETE));
  } else if (g_session.petHunger > 90 && g_session.petHunger <= 100) {
    std::strcpy(hunger, LookupMessage(MSI_VERY_REPLETE));
  } else {
    std::strcpy(hunger, LookupMessage(MSI_UNKNOWN));
  }

  _itoa(g_session.petLevel, level, 10);
  this->DrawTextAt(135, 44, level, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(145, 69, hunger, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(145, 89, this->friendlyText.c_str(), 0, 0, 12, 0, 0, 0);

  const int job = g_session.petJob;

  if (job != -1 && ((job > 1000 && job < 3999) || (job >= 20021 && job <= 31999))) {
    char illust[260];
    std::memset(illust, 0, sizeof(illust));
    g_session.GetPetIllustName(job, g_session.petAccessory, illust);

    if (auto* image = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(illust)))) {
      this->DrawBitmap(5, 21, image, false);
    }

    char command[] = "< Command >";
    this->FillRegion(160, 44, 110, 18, 0xFAE6E6);
    const int width = this->TextWidth(command, 0, 0, 12, 0, 0);
    this->DrawTextAt(206 - width / 2, 48, command, 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(100, 119, LookupMessage(MSI_PET_ACCESSARY), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(100, 139, LookupMessage(g_session.petAccessory == 0 ? MSI_ITEM_UNEQUIPED : MSI_ITEM_EQUIPED), 0, 0, 12, 0, 0, 0);

    if (this->autoFeedAvailable == 1) {
      this->DrawTextAt(100, 159, LookupMessage(MSI_PET_AUTO_FEEDING_OFF), 0, 0, 12, 0, 0, 0);
    }
  }
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
      this->friendlyText = LookupMessage(MSI_VERY_AWKWARD);
      low = 0;
      high = 100;
    }
  } else if (g_session.m_petFriendly > 100 && g_session.m_petFriendly <= 250) {
    this->friendlyText = LookupMessage(MSI_AWKWARD);
    low = 100;
    high = 250;
  } else if (g_session.m_petFriendly > 250 && g_session.m_petFriendly <= 750) {
    this->friendlyText = LookupMessage(MSI_NORMAL);
    low = 250;
    high = 750;
  } else if (g_session.m_petFriendly > 750 && g_session.m_petFriendly <= 900) {
    this->friendlyText = LookupMessage(MSI_FRIENDLY);
    low = 750;
    high = 900;
  } else if (g_session.m_petFriendly > 900 && g_session.m_petFriendly <= 1000) {
    this->friendlyText = LookupMessage(MSI_VERY_FRIENDLY);
    low = 900;
    high = 1000;
  } else {
    this->friendlyText = LookupMessage(MSI_UNKNOWN);
    low = 0;
    high = 0;
  }
  const float ratio = static_cast<float>(static_cast<unsigned>(g_session.m_petFriendly - low)) / static_cast<float>(static_cast<unsigned>(high - low));
  this->friendlyGauge->SetValue(static_cast<unsigned>(ratio * 100.0f));
  if (!(previous == this->friendlyText)) {
    this->Invalidate();
  }
}

// OFFSET: 0x008B1240
// FUNCTION: fn_11745
void UIMonsterInfoWnd::OnCreate(int, int) {
  const std::string prefix("유저인터페이스\\");
  auto* button = new UIBitmapButton();
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(this->geometry.width - 46, this->geometry.height - 24);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;

  this->actorId = g_session.m_monsterInfoActorId;
  {
    const std::string actorName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(g_session.m_monsterInfoActorId);
    this->name = actorName;
  }
  const std::size_t separator = this->name.find("#");
  if (separator != std::string::npos) {
    this->name.erase(separator, this->name.size());
  }
  this->name = this->FitDisplayText(100, this->name.c_str(), 0, 12, 0);
}

// OFFSET: 0x008A4300
// FUNCTION: fn_11667
UIPetInfoWnd::UIPetInfoWnd()
    : UIFrameWnd(0), friendlyGauge(nullptr), friendlyText("") {
  this->autoFeedAvailable = 0;
  this->autoFeedCheck = 0;
  this->nameEdit = nullptr;
  this->saved = nullptr;
  this->renameButton = 0;
  this->statusButton = 0;
  this->feedConfirmationWnd = nullptr;
}

// OFFSET: 0x008A43E0
// FUNCTION: fn_11668
UIReplayButton::UIReplayButton() : UIBitmapButton() {
}

// OFFSET: 0x008A4440
// FUNCTION: fn_11669
UIReplayControlWnd::UIReplayControlWnd() : UIFrameWnd(0) {
  this->m_paused = false;
}

// OFFSET: 0x008A4500
// FUNCTION: fn_11670
UIReplayRECControlWnd::UIReplayRECControlWnd(int mode)
    : UIFrameWnd(0), mode(mode), recordStart(0) {
  for (int i = 0; i < 10; ++i) {
    this->options[i] = nullptr;
    this->recordOptions[i] = 0;
  }
}

// OFFSET: 0x008A45A0
// FUNCTION: fn_11671
UISelectReplayDataWnd::UISelectReplayDataWnd() : UIFrameWnd(0) {
  this->m_list = nullptr;
  this->m_serverButton = nullptr;
  this->m_texts[0] = nullptr;
  this->m_texts[1] = nullptr;
  this->m_texts[2] = nullptr;
  this->m_texts[3] = nullptr;
}

// OFFSET: 0x008A4640
// FUNCTION: fn_11672
UISelectServerWnd::UISelectServerWnd()
    : UIFrameWnd(0) {
  this->m_serverList = nullptr;
  this->m_okButton = nullptr;
}

// OFFSET: 0x008A46E0
// FUNCTION: fn_11673
UITalkboxTrapInputWnd::UITalkboxTrapInputWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008A4E00
// FUNCTION: fn_11677
UICntWnd::~UICntWnd() = default;

// OFFSET: 0x008A5050
// FUNCTION: fn_11679
__declspec(noinline) UILoginWnd::~UILoginWnd() = default;

// OFFSET: 0x008A54C0
// FUNCTION: fn_11681
UIPetInfoWnd::~UIPetInfoWnd() {
  if (this->feedConfirmationWnd) {
    g_windowMgr.Schedule(this->feedConfirmationWnd);
  }
}

// OFFSET: 0x008A5CC0
// FUNCTION: fn_11686
EncryptedName<80, 0xF3CB6F7711A71B6DULL>& HangameJoinUrl::operator()() const {
  constexpr auto sourceText = OBFUSCATE_SOURCE("http://ro.hangame.com/login/loginstep.asp?prevURL=/NHNCommon/NHN/Memberjoin.asp", 0xF3CB6F7711A71B6DULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
  return name;
}

// OFFSET: 0x008BE490
// FUNCTION: fn_11805
int UILoginWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMBOBOX_SELECTED:
    if (sender == this->loginTypeCombo) {
      switch (a) {
      case 0: {
        this->loginExtra = OBFUSCATE_TEXT("G000", 0x9FED7DA189490D5DULL);
        break;
      }
      case 1: {
        this->loginExtra = OBFUSCATE_TEXT("G007", 0xFBD599E117C547C1ULL);
        break;
      }
      default: {
        g_windowMgr.Confirm(OBFUSCATE_TEXT("unknown account class found!", 0x176F138D5DD7F365ULL), 0, false, false, 0, "Message", 280, 120, 0);
        goto quit;
      }
      }
    }
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_MAKEACCOUNT:
      if (g_serviceType == 0) {
        if (g_session.m_channelCopId == 1) {
          g_modeMgr.Quit();
          const char* url = HangameJoinUrl()().Text();
          ShellExecuteA(hWnd, OBFUSCATE_TEXT_NAMED("open", 0x839B27DF8F5967B3ULL, verbName), url, nullptr, nullptr, SW_SHOWNORMAL);
          break;
        }
        if (g_windowMgr.Confirm(LookupMessage(MSI_SELECTCHARWND_RESERVE_TO_DEL_CHAR), 2, true, false, 0, "Message", 280, 120, 0) != ID_YES) {
          break;
        }
      }
      g_modeMgr.Quit();
      this->OpenWebPage();
      break;
    case ID_LOGIN: {
      g_soundMgr->PlayWave(OBFUSCATE_TEXT_NAMED("버튼소리.wav", 0x452FEF0D7FC95565ULL, soundName), 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (this->passwordEdit->Length() >= 6 && this->idEdit->Length() >= 4) {
        g_modeMgr.gameMode->SendMsg(LMM_CONNECT_TO_ACSVR, 0, 0, 0, 0);
        StringFormat(g_launchAccountPassword, 24, static_cast<std::size_t>(-1), "%s", this->idEdit->Text());
        g_modeMgr.gameMode->SendMsg(LMM_PASSWORD, reinterpret_cast<int>(this->passwordEdit->Text()), 0, 0, 0);
        g_modeMgr.gameMode->SendMsg(LMM_ID, reinterpret_cast<int>(this->idEdit->Text()), 0, 0, 0);
        g_modeMgr.gameMode->SendMsg(LMM_SET_LOGIN_EXTRA, reinterpret_cast<int>(this->loginExtra), 0, 0, 0);
        g_windowMgr.CloseWindow(WID_LOGINWND);
        this->SaveLogin();
      } else if (this->idEdit->Length() < 4) {
        g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_BASIC_EXP_MSG2_EXCEPT_PREMIUM), 4).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_SWAP_EQUIPITEM_CANNOT_ADD_ITEM), 6).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
      }
      break;
    }
    case ID_CANCEL:
      g_loginCancelRequested = 1;
      g_modeMgr.gameMode->SendMsg(LMM_GOTOSELECTACCOUNT, 0, 0, 0, 0);
      break;
    case ID_EXIT:
      this->SaveLogin();
      if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT), 2, true, false, 0, "Message", 280, 120, 0) == ID_YES) {
        goto quit;
      }
      break;
    case ID_CHECK:
      this->saveId = b;
      this->SaveLogin();
      break;
    case ID_CLASSCOMBO: {
      if (g_windowMgr.comboBoxWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_COMBOBOXWND);
      }
      UIWindow* list = g_windowMgr.EnsureWindow(WID_COMBOBOXWND);
      this->accountListOpen = 1;
      const int count = static_cast<int>(this->savedAccounts.size());
      for (unsigned int index = 0; index < this->savedAccounts.size(); ++index) {
        list->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->savedAccounts[index].c_str()), 0, 0, 0);
      }
      list->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
      int height = count * 16;
      if (height > 48) {
        height = 48;
      }
      list->Resize(74, height);
      list->SendMsg(nullptr, UM_SET_CALLER_WID, 3, 0, 0, 0);
      int x = 60;
      int y = 96;
      this->ToScreen(x, y);
      list->Move(x + 1, y);
      break;
    }
    case 359:
      g_session.m_channelCopId = 0;
      this->Invalidate();
      break;
    case 360:
      g_session.m_channelCopId = 2;
      this->Invalidate();
      break;
    }
    return 0;
  case UM_ENABLE_CANCEL:
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

quit:
  g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
  g_windowMgr.Schedule(this);
  return 0;
}

// OFFSET: 0x008BE200
// FUNCTION: fn_11804
int UILoginOTPWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND: {
    switch (a) {
    case ID_CANCEL:
      if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT), 2, true, false, 0, "Message", 280, 120, 0) == ID_YES) {
        g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
      }
      break;

    case 382: {
      CSoundMgr* const soundMgr = g_soundMgr;
      constexpr auto soundSourceText = OBFUSCATE_SOURCE("버튼소리.wav", 0x7321C3814F8799E1ULL);
      const auto& soundSource = soundSourceText.data;
      thread_local EncryptedName<sizeof(soundSource), 0x7321C3814F8799E1ULL> soundName(soundSource);
      soundMgr->PlayWave(soundName.Text(), 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (this->authCodeEdit->Length() == 8) {
        g_modeMgr.gameMode->SendMsg(LMM_SET_AUTH_CODE, reinterpret_cast<int>(this->authCodeEdit->Text()), 0, 0, 0);
        g_modeMgr.gameMode->SendMsg(LMM_SEND_LOGIN_AUTH_CODE, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_LOGINOTPWND);
      } else {
        g_windowMgr.Confirm(LookupMessage(MSI_FAIL_SERVICE_CHECKING_ERROR), 0, true, false, 0, "Message", 280, 120, 0);
      }
      break;
    }
    }

    break;
  }
  case UM_REFRESH: {
    this->authCodeEdit->SetText("");
    return 0;
  }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x008ADC80
// FUNCTION: fn_11740
void UIHomunInfoWnd::OnCreate(int, int) {
  this->AddCloseButton("", 0);
  this->nameEdit = new UIEditCtrl2();
  this->nameEdit->SetSize(80, 16);
  char* const name = g_session.m_homunName;
  if (std::strcmp(name, "") == 0) {
    std::sprintf(name, g_session.GetJobName(g_session.m_homunJob, -1));
  }
  this->nameEdit->Move(135, 22);
  this->nameEdit->SetText(name);
  this->nameEdit->maximum = 50;
  this->nameEdit->Background(232, 232, 232);
  this->AddChild(this->nameEdit);
  {
    const std::string bitmap("유저인터페이스\\btn_del");
    this->button = new UIBitmapButton();
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(187, 44);
    this->button->SetId(291);
    this->AddChild(this->button);
  }
  {
    const std::string bitmap("유저인터페이스\\btn_skill");
    this->button = new UIBitmapButton();
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(232, 44);
    this->button->SetId(289);
    this->AddChild(this->button);
  }
  {
    const std::string bitmap("유저인터페이스\\btn_feed");
    this->button = new UIBitmapButton();
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(210, 155);
    this->button->SetId(290);
    this->AddChild(this->button);
    this->button->Move(210, 105);
  }
  {
    const std::string bitmap("유저인터페이스\\btn_rewrite");
    this->button = new UIBitmapButton();
    this->button->SetBitmap((bitmap + ".bmp").c_str(), 0, 0);
    this->button->SetBitmap((bitmap + "_a.bmp").c_str(), 1, 0);
    this->button->SetBitmap((bitmap + "_b.bmp").c_str(), 2, 0);
    this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
    this->button->Move(232, 21);
    this->button->SetId(184);
    this->AddChild(this->button);
  }

  this->hpBar = new UIBarGraphHomun();
  this->hpBar->SetValues([]() -> std::int64_t { return g_session.m_homunHp; },
                         []() -> std::int64_t { return g_session.m_homunMaxHp; });
  this->hpBar->SetSize(157, 9);
  this->hpBar->Move(117, 73);
  this->AddChild(this->hpBar);

  this->spBar = new UIBarGraphHomun();
  this->spBar->SetValues([]() -> std::int64_t { return g_session.m_homunSp; }, []() -> std::int64_t { return g_session.homunMaxSp; });
  this->spBar->SetSize(157, 9);
  this->spBar->Move(117, 88);
  this->AddChild(this->spBar);

  this->expBar = new UIINT64BarGraph();
  this->expBar->SetValues(g_session.m_homunExp, g_session.m_homunMaxExp);
  this->expBar->m_plainColor = 1;
  this->expBar->SetSize(102, 6);
  this->expBar->Move(101, 119);
  this->AddChild(this->expBar);

  this->intimacyBar = new UIBarGraph4();
  this->intimacyBar->m_plainColor = false;
  this->intimacyBar->SetSize(102, 6);
  this->intimacyBar->Move(101, 145);
  this->intimacyBar->SetValues(&g_session.homunHunger, &g_session.homunMaxHunger);
  this->AddChild(this->intimacyBar);
  this->m_message0 = 201;

  this->autoFeedCheckBox = new UICheckBox();
  this->autoFeedCheckBox->SetSize(12, 12);
  this->autoFeedCheckBox->Move(210, 142);
  this->autoFeedCheckBox->SetBitmaps("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp");
  this->autoFeedCheckBox->SetChecked(g_session.m_homunculusAutoFeed);
  this->AddChild(this->autoFeedCheckBox);
}

// OFFSET: 0x008B7DB0
// FUNCTION: fn_11763
void UIHomunInfoWnd::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\homuninfo_bg.bmp")));
  this->DrawBitmap(0, 0, background, false);
  DrawFilledRoundedBorder(*this, 0, this->geometry.height - 40, this->geometry.width, 40, 0xFFFFFF);
  char title[32];
  std::sprintf(title, LookupMessage(MSI_HOMUN_INFO));
  this->DrawTextAt(5, 2, title, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 24, LookupMessage(MSI_NAME), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 47, LookupMessage(MSI_LEVEL), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 105, LookupMessage(MSI_HOMUN_EXP), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 131, LookupMessage(MSI_PET_HUNGRY), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(100, 159, LookupMessage(MSI_PET_FRIENDLY), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(140, 159, ":", 0, 0, 12, 0, 0, 0);
  const int* const stats = g_session.m_homunStats;
  char number[12];
  int bytes = std::sprintf(number, "%d", stats[0]);
  this->DrawRightAlignedText(79, 24, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[1]);
  this->DrawRightAlignedText(79, 42, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[2]);
  this->DrawRightAlignedText(79, 60, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[3]);
  this->DrawRightAlignedText(79, 78, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[4]);
  this->DrawRightAlignedText(79, 96, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[5]);
  this->DrawRightAlignedText(79, 114, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", stats[6]);
  this->DrawRightAlignedText(79, 132, number, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(number, "%d", (2000 - stats[7]) / 10);
  this->DrawRightAlignedText(79, 150, number, bytes, 1, 14, 0, 0);
  char level[12];
  _itoa(g_session.m_homunLevel, level, 10);
  this->DrawTextAt(135, 47, level, 0, 0, 12, 0, 0, 0);
  if (g_session.m_homunJob == -1) {
    return;
  }

  char text[24];
  bytes = std::sprintf(text, "%lld", g_session.m_homunMaxExp);
  this->DrawRightAlignedText(205, 104, text, bytes, 1, 14, 0, 0);
  bytes = std::sprintf(text, "%d / %d", g_session.homunHunger, 100);
  this->DrawRightAlignedText(200, 130, text, bytes, 1, 14, 0, 0);
  const int intimacy = g_session.homunFriendly;
  if (intimacy >= 0 && intimacy <= 3) {
    std::strcpy(text, LookupMessage(MSI_VERYHATE));
  } else if (intimacy > 3 && intimacy <= 10) {
    std::strcpy(text, LookupMessage(MSI_HATE));
  } else if (intimacy > 10 && intimacy <= 100) {
    std::strcpy(text, LookupMessage(MSI_VERY_AWKWARD));
  } else if (intimacy > 100 && intimacy <= 250) {
    std::strcpy(text, LookupMessage(MSI_AWKWARD));
  } else if (intimacy > 250 && intimacy <= 750) {
    std::strcpy(text, LookupMessage(MSI_NORMAL));
  } else if (intimacy > 750 && intimacy <= 910) {
    std::strcpy(text, LookupMessage(MSI_FRIENDLY));
  } else if (intimacy > 910 && intimacy <= 1000) {
    std::strcpy(text, LookupMessage(MSI_VERY_FRIENDLY));
  } else {
    std::strcpy(text, LookupMessage(MSI_UNKNOWN));
  }
  this->DrawTextAt(150, 159, text, 0, 0, 12, 0, 0, 0);
  this->DrawWrappedFormattedText(225, 136, std::string(LookupMessage(MSI_HOMUN_SELF_FEEDING_ON)), 0, 0, 0, 0, 12, 0, 0);
  this->DrawWrappedFormattedText(8, 180, std::string(LookupMessage(MSI_HOMUN_TARGET_NOT_EXIST_HOMUN_TARGAT)), 0, 0, 0, 0, 12, 0xFF, 0);
}

// OFFSET: 0x008BDD10
// FUNCTION: fn_11803
int UIHomunInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (this->feedConfirmation && sender != this->feedConfirmation) {
    return 0;
  }

  switch (message) {
  case UM_DEFPUSH:
    return 0;
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);

    switch (a) {
    case ID_OK: {
      char* const name = g_session.m_homunName;
      if (std::strlen(this->nameEdit->Text()) >= 24) {
        g_windowMgr.Confirm(LookupMessage(MSI_HOMUN_NAME_IN23), 0, true, false, 0, "Message", 280, 120, 0);
      } else if (_mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkOpening.c_str())) ||
                 _mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkClosing.c_str()))) {
        g_windowMgr.Confirm(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        char text[512];
        std::sprintf(text, LookupMessage(MSI_BLOCK_CALL_NOTIFY), this->nameEdit->Text());
        if (g_windowMgr.Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) != ID_NO) {
          if (g_forbiddenWords.Contains(this->nameEdit->Text()) != true) {
            g_modeMgr.GetGameMode()->SendMsg(MM_MER_RENAME, reinterpret_cast<int>(this->nameEdit->Text()), 0, 0, 0);
            std::sprintf(name, this->nameEdit->Text());
            this->Invalidate();
            return 0;
          }

          g_windowMgr.Confirm(LookupMessage(MSI_CANNOT_SELL_IN_PRIVATE_TAB), 0, true, false, 0, "Message", 280, 120, 0);
        }

        this->nameEdit->SetText(name);
        this->nameEdit->Invalidate();
      }
      return 0;
    }
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_HOMUNINFOWND);
      return 0;
    case ID_CHECK:
      if (sender == this->autoFeedCheckBox) {
        if (b > 0) {
          g_session.SendHomunculusAutoFeedOption(1);
        } else {
          g_session.SendHomunculusAutoFeedOption(0);
        }
      }
      return 0;
    case ID_OPEN_HOMUNSKILLLIST:
      if (!g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_0)) {
        g_windowMgr.EnsureWindow(WID_SKILLLISTWND_SKILLTAB_0);
      }
      return 0;
    case ID_FEED_TO_HOMUN:
      this->feedConfirmation = new UINormalMB();
      this->feedConfirmation->Create(this, LookupMessage(MSI_SURE_TO_FEED_PET), 2, true, nullptr, 280, 120, nullptr);
      g_windowMgr.AddRendered(this->feedConfirmation);
      return 0;
    case ID_DELETE_HOMUN: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (g_windowMgr.Confirm(LookupMessage(MSI_DELETE_HOMUN), 2, false, false, 0, "Message", 280, 120, 0) == ID_YES) {
        mode->SendMsg(MM_COMMAND_HOMUN, 2, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_HOMUNINFOWND);
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_0);
        g_session.ClearHomunStatus();
        g_session.homunAlive = 0;
      }
      return 0;
    }
    case 488: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (sender == this->feedConfirmation && b == ID_YES) {
        if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXCHANGED_FAILED)), 255, 0, 0, 0);
        } else {
          mode->SendMsg(MM_COMMAND_HOMUN, 1, 0, 0, 0);
        }
      }
      g_windowMgr.Schedule(this->feedConfirmation);
      this->feedConfirmation = nullptr;
      return 0;
    }
    default:
      return 0;
    }
  case UM_SETINFO:
    this->saved = reinterpret_cast<HomunInfoWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008B2820
// FUNCTION: fn_11747
void UIPetInfoWnd::OnCreate(int, int) {
  this->AddCloseButton("", 0);
  this->nameEdit = new UIEditCtrl2();
  this->nameEdit->SetSize(80, 16);
  if (std::strcmp(g_session.petName, "") == 0) {
    std::sprintf(g_session.petName, g_session.GetJobName(g_session.petJob, -1));
  }
  this->nameEdit->Move(135, 24);
  this->nameEdit->SetText(g_session.petName);
  this->nameEdit->maximum = 50;
  this->nameEdit->Background(232, 232, 232);
  this->AddChild(this->nameEdit);
  {
    const std::string prefix("유저인터페이스\\");
    this->renameButton = new UIBitmapButton();
    this->renameButton->SetBitmap((prefix + "btn_rewrite" + ".bmp").c_str(), 0, 0);
    this->renameButton->SetBitmap((prefix + "btn_rewrite_a" + ".bmp").c_str(), 1, 0);
    this->renameButton->SetBitmap((prefix + "btn_rewrite_b" + ".bmp").c_str(), 2, 0);
    this->renameButton->SetSize(this->renameButton->BitmapWidth(), this->renameButton->BitmapHeight());
    this->renameButton->Move(230, 21);
    this->renameButton->SetId(184);
    this->AddChild(this->renameButton);
  }

  const char tooltip[] = "Pet Command";
  this->statusButton = new UINoneVisibleButton();
  this->statusButton->SetSize(120, 18);
  this->statusButton->Move(150, 44);
  this->statusButton->SetId(268);
  this->AddChild(this->statusButton);
  {
    const std::string prefix("유저인터페이스\\basic_interface\\");
    UIBitmapButton* button = new UIBitmapButton();
    button->SetBitmap((prefix + "txtbox_btn_a" + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + "txtbox_btn_b" + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + "txtbox_btn_c" + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(270 - button->BitmapWidth(), 44);
    button->SetTooltip(tooltip);
    button->SetId(268);
    this->AddChild(button);
  }

  this->friendlyText = LookupMessage(MSI_UNKNOWN);
  this->friendlyGauge = new UIPetInfoBarGraph();
  this->friendlyGauge->SetSize(120, 9);
  this->friendlyGauge->Move(145, 105);
  this->AddChild(this->friendlyGauge);
  this->UpdateFriendlyGauge();
  this->m_message0 = 201;

  const int petEgg = std::atoi(g_session.LookupClientItemByIdentity(g_session.petEggIndex).itemId.c_str());
  this->autoFeedAvailable = g_petEvolutionMgr->IsAutoFeeding(petEgg);
  if (this->autoFeedAvailable == true) {
    this->autoFeedCheck = new UICheckBox();
    this->autoFeedCheck->SetSize(12, 12);
    this->autoFeedCheck->Move(this->TextWidth(LookupMessage(MSI_PET_AUTO_FEEDING_OFF), 0, 0, 12, 0, 0) + 110, 159);
    this->autoFeedCheck->SetBitmaps("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp");
    this->autoFeedCheck->SetChecked(g_session.m_petAutoFeed);
    this->AddChild(this->autoFeedCheck);
  }
}

// OFFSET: 0x008BFE80
// FUNCTION: fn_11812
int UIPetInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (this->feedConfirmationWnd && sender != this->feedConfirmationWnd) {
    return 0;
  }

  switch (message) {
  case UM_DEFPUSH:
    return 0;
  case UM_COMBOBOX_SELECTED: {
    CMode* mode = g_modeMgr.GetGameMode();
    switch (a) {
    case 467:
      if (g_windowMgr.rodexComposeWnd) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REFINING_FAIL_ITEM_DISAPPEAR)), 255, 0, 0, 0);
        break;
      }
      this->feedConfirmationWnd = new UINormalMB();
      this->feedConfirmationWnd->Create(this, LookupMessage(MSI_SURE_TO_FEED_PET), 2, true, nullptr, 280, 120, nullptr);
      g_windowMgr.AddRendered(this->feedConfirmationWnd);
      g_windowMgr.CloseWindow(WID_COMBOBOXIDWND);
      return 0;
    case 468:
      mode->SendMsg(MM_COMMAND_PET, 2, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_COMBOBOXIDWND);
      return 0;
    case 469:
      mode->SendMsg(MM_COMMAND_PET, 4, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_COMBOBOXIDWND);
      return 0;
    case 470:
      mode->SendMsg(MM_COMMAND_PET, 3, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_COMBOBOXIDWND);
      return 0;
    case 471:
      if (g_session.m_petFriendly > 900) {
        g_windowMgr.CloseWindow(WID_COMBOBOXIDWND);
        UIWindow* window = g_windowMgr.EnsureWindow(WID_PETEVOLUTIONWND);
        const int petEgg = g_session.LookupClientItemByIdentity(g_session.petEggIndex).GetItemId();
        window->SendMsg(nullptr, UM_SET_PET_EVOLUTION_MATERIALS, petEgg, b, 0, 0);
        this->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_COMBOBOXIDWND);
        return 0;
      }
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_AUTO_FEEDING_ON)), 255, 0, 0, 0);
      break;
    }
    g_windowMgr.CloseWindow(WID_COMBOBOXIDWND);
    return 0;
  }
  case UM_COMMAND:
    if (a == ID_OK || a == ID_CLOSE) {
      this->RecordResponse(static_cast<int>(a), b, c, d);
    }

    switch (a) {
    case ID_OK:
      if (std::strlen(this->nameEdit->Text()) >= 24) {
        g_windowMgr.Confirm(LookupMessage(MSI_PETNAME_IN23), 0, true, false, 0, "Message", 280, 120, 0);
      } else if (_mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkOpening.c_str())) ||
                 _mbsstr(reinterpret_cast<const unsigned char*>(this->nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkClosing.c_str()))) {
        g_windowMgr.Confirm(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        char text[512];
        std::sprintf(text, LookupMessage(MSI_STORE_ASSISTANT_TRADE_CNT), this->nameEdit->Text());
        if (g_windowMgr.Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) != ID_NO) {
          if (g_forbiddenWords.Contains(this->nameEdit->Text()) != true) {
            g_modeMgr.GetGameMode()->SendMsg(MM_PET_RENAME, reinterpret_cast<int>(this->nameEdit->Text()), 0, 0, 0);
            std::sprintf(g_session.petName, this->nameEdit->Text());
            this->Invalidate();
            return 0;
          }

          g_windowMgr.Confirm(LookupMessage(MSI_CANNOT_SELL_IN_PRIVATE_TAB), 0, true, false, 0, "Message", 280, 120, 0);
        }

        this->nameEdit->SetText(g_session.petName);
        this->nameEdit->Invalidate();
      }
      return 0;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_PETINFOWND);
      return 0;
    case ID_CHECK:
      if (sender == this->autoFeedCheck) {
        if (b > 0) {
          g_session.SendPetAutoFeedOption(1);
        } else {
          g_session.SendPetAutoFeedOption(0);
        }
      }
      return 0;
    case 268: {
      UIWindow* menu = g_windowMgr.EnsureWindow(WID_COMBOBOXIDWND);
      std::string feed;
      std::string performance;
      std::string returnToEgg;
      std::string unequip;
      feed = LookupMessage(MSI_PET_FEEDING);
      performance = LookupMessage(MSI_PET_PERFORMANCE);
      returnToEgg = LookupMessage(MSI_PET_RETURN_EGG);
      unequip = LookupMessage(MSI_PET_ACC_OFF);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(feed.c_str()), 467, 0, 0);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(performance.c_str()), 468, 0, 0);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(unequip.c_str()), 469, 0, 0);
      menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(returnToEgg.c_str()), 470, 0, 0);
      const int petEgg = g_session.LookupClientItemByIdentity(g_session.petEggIndex).GetItemId();
      const std::pair<bool, std::vector<int>> targets = g_petEvolutionMgr->GetEvolutionTargets(petEgg);
      if (targets.first == true) {
        for (auto it = targets.second.begin(); it != targets.second.end(); ++it) {
          const int job = g_session.GetPetJobByEggItem(*it);
          std::string text;
          const char* name = g_session.GetPetString(job);
          if (name == nullptr) {
            text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_PET_EVOLUTION_FAIL_UNKNOWN), g_session.GetJobName(job, -1)));
          } else {
            text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_PET_EVOLUTION_FAIL_UNKNOWN), name));
          }
          menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(text.c_str()), 471, *it, 0);
        }
      }
      menu->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
      menu->Resize(110, (static_cast<int>(targets.second.size()) + 4) * 16);
      menu->SendMsg(nullptr, UM_SET_CALLER_WID, 88, 0, 0, 0);
      int x = 160;
      int y = 44;
      this->ToScreen(x, y);
      menu->Move(x, y);
      return 0;
    }
    case 488: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (sender == this->feedConfirmationWnd && b == ID_YES) {
        if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BARTERITEM_NAME_COUNT)), 255, 0, 0, 0);
          this->feedConfirmationWnd = nullptr;
          return 0;
        }
        mode->SendMsg(MM_COMMAND_PET, 1, 0, 0, 0);
      }
      this->feedConfirmationWnd = nullptr;
      return 0;
    }
    default:
      return 0;
    }
  case UM_SETINFO:
    this->saved = reinterpret_cast<PetInfoWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008B6520
// FUNCTION: fn_11751
void UISelectServerWnd::OnCreate(int width, int height) {
  this->m_serverList = new UIServerListBox();
  this->m_serverList->Configure(12, 22, width - 24, height - 55, 0);
  this->m_serverList->backgroundRed = 240;
  this->m_serverList->backgroundGreen = 240;
  this->m_serverList->backgroundBlue = 240;
  this->m_serverList->SetId(184);
  this->AddChild(this->m_serverList);

  const std::string prefix("유저인터페이스\\");
  const int ids[4] = {184, 185, 221, 400};
  const char* const names[4][3] = {
      {"btn_ok", "btn_ok_a", "btn_ok_b"},
      {"btn_cancel", "btn_cancel_a", "btn_cancel_b"},
      {"login_interface\\btn_exit", "login_interface\\btn_exit_a", "login_interface\\btn_exit_b"},
      {"replay_interface\\btn_replay", "replay_interface\\btn_replay_a", "replay_interface\\btn_replay_b"},
  };
  int positions[4][2] = {{189, height - 24}, {234, height - 24}, {-100, -100}, {130, height - 24}};
  if (g_modeMgr.GetLoginMode()->cursorType == 30) {
    positions[3][0] = -100;
    positions[3][1] = -100;
  }

  for (int i = 0; i < 4; ++i) {
    if (!g_launchPassCommandLine && ids[i] == 400) {
      continue;
    }

    UIBitmapButton* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
    if (ids[i] == 221) {
      this->m_okButton = button;
    }
  }

  this->m_message0 = 184;
}

// OFFSET: 0x008B31F0
// FUNCTION: fn_11748
void UIReplayControlWnd::OnCreate(int, int) {
  const std::string prefix("유저인터페이스\\replay_interface\\");
  const int ids[6] = {403, 404, 405, 406, 401, 402};
  const int positions[6][2] = {{101, 89}, {149, 89}, {53, 89}, {53, 111}, {5, 89}, {-100, -100}};
  const char* const names[6] = {"replay_stop_", "replay_end_", "replay_speedadd_", "replay_speedmin_", "replay_start_", "replay_pause_"};

  for (int i = 0; i < 4; ++i) {
    this->m_buttons[i] = new UIBitmapButton();
    UIBitmapButton* button = static_cast<UIBitmapButton*>(this->m_buttons[i]);
    button->SetBitmap((prefix + names[i] + "a.bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i] + "a.bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i] + "b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_playButton = new UIReplayButton();
  this->m_playButton->SetBitmap((prefix + names[4] + "a.bmp").c_str(), 0, 0);
  this->m_playButton->SetBitmap((prefix + names[4] + "a.bmp").c_str(), 1, 0);
  this->m_playButton->SetBitmap((prefix + names[4] + "b.bmp").c_str(), 2, 0);
  this->m_playButton->SetSize(this->m_playButton->BitmapWidth(), this->m_playButton->BitmapHeight());
  this->m_playButton->Move(positions[4][0], positions[4][1]);
  this->m_playButton->SetId(ids[4]);
  this->AddChild(this->m_playButton);

  this->m_pauseButton = new UIReplayButton();
  this->m_pauseButton->SetBitmap((prefix + names[5] + "a.bmp").c_str(), 0, 0);
  this->m_pauseButton->SetBitmap((prefix + names[5] + "a.bmp").c_str(), 1, 0);
  this->m_pauseButton->SetBitmap((prefix + names[5] + "b.bmp").c_str(), 2, 0);
  this->m_pauseButton->SetSize(this->m_pauseButton->BitmapWidth(), this->m_pauseButton->BitmapHeight());
  this->m_pauseButton->Move(positions[5][0], positions[5][1]);
  this->m_pauseButton->SetId(ids[5]);
  this->AddChild(this->m_pauseButton);

  HotkeyInfo hotkey;
  hotkey = g_session.GetHotkeyInfo(1, 39);
  this->m_playButton->SetTooltip(hotkey.text.c_str());
  this->m_pauseButton->SetTooltip(hotkey.text.c_str());
  hotkey = g_session.GetHotkeyInfo(1, 40);
  static_cast<UIButton*>(this->m_buttons[2])->SetTooltip(hotkey.text.c_str());
  hotkey = g_session.GetHotkeyInfo(1, 41);
  static_cast<UIButton*>(this->m_buttons[3])->SetTooltip(hotkey.text.c_str());

  this->m_progress = new UIBarGraph4();
  this->m_progress->m_plainColor = true;
  this->m_progress->SetSize(188, 11);
  this->m_progress->Move(5, 72);
  this->m_progress->SetValues(reinterpret_cast<const int*>(&Replay::Instance().elapsed), reinterpret_cast<const int*>(&Replay::Instance().header.duration));
  this->AddChild(this->m_progress);

  const int textPositions[4][2] = {{8, 26}, {8, 40}, {-100, -100}, {8, 54}};
  for (int i = 0; i < 4; ++i) {
    this->m_texts[i] = new UIStaticText();
    this->m_texts[i]->SetSize(188, 14);
    this->m_texts[i]->SetText("", 0);
    this->m_texts[i]->Move(textPositions[i][0], textPositions[i][1]);
    this->m_texts[i]->SetColor(100, 100, 200);
    this->AddChild(this->m_texts[i]);
  }

  this->RefreshInfo();
}

// OFFSET: 0x008A9D40
// FUNCTION: fn_11722
void UIFrameWnd::DrawSkinBox(const char* name, int x, int y, int width, int height, bool tiled) {
  std::string prefix("유저인터페이스\\sysbox_");
  if (name) {
    prefix = name;
  }
  if (width <= 0) {
    width = this->geometry.width;
  }
  if (height <= 0) {
    height = this->geometry.height;
  }

  const int innerWidth = width - 28;
  const int left = x + 14;
  const int top = y + 14;
  if (tiled == true) {
    const int columns = TileCount(innerWidth, 14);
    const int rows = TileCount(height - 28, 14);
    for (int row = 0; row < rows; ++row) {
      for (int column = 0; column < columns; ++column) {
        auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "bg.bmp").c_str())));
        this->DrawBitmap(left + column * 14, top + row * 14, bitmap, false);
      }
    }
  } else {
    unsigned red, green, blue;
    g_skinInfo.PaletteChannels(2, 2, red, green, blue);
    this->FillRegion(left, top, innerWidth, height - 28, RGB(red, green, blue));
  }

  const int columns = TileCount(innerWidth, 14);
  for (int i = 0; i < columns; ++i) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "mu.bmp").c_str())));
    this->DrawBitmap(left + i * 14, y, bitmap, false);
  }

  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "lu.bmp").c_str())));
  this->DrawBitmap(x, y, bitmap, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "ru.bmp").c_str())));
  if (bitmap) {
    this->DrawBitmap(x - bitmap->image.width + width, y, bitmap, false);
  }

  const int rows = TileCount(height - 28, 14);
  for (int i = 0; i < rows; ++i) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "lm.bmp").c_str())));
    this->DrawBitmap(x, top + i * 14, bitmap, false);
  }
  for (int i = 0; i < rows; ++i) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "rm.bmp").c_str())));
    if (bitmap) {
      this->DrawBitmap(x - bitmap->image.width + width, top + i * 14, bitmap, false);
    }
  }
  for (int i = 0; i < columns; ++i) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "md.bmp").c_str())));
    if (bitmap) {
      this->DrawBitmap(left + i * 14, y - bitmap->image.height + height, bitmap, false);
    }
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "ld.bmp").c_str())));
  if (bitmap) {
    this->DrawBitmap(x, y - bitmap->image.height + height, bitmap, false);
  }
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((prefix + "rd.bmp").c_str())));
  if (bitmap) {
    this->DrawBitmap(x - bitmap->image.width + width, y - bitmap->image.height + height, bitmap, false);
  }
}

// OFFSET: 0x008A8280
// FUNCTION: fn_11714
void UIMessageBoxAutoreturn::Configure(const char* text, int timeout) {
  this->messageText = text;
  this->format = text;
  this->started = timeGetTime();
  this->deadline = timeGetTime() + static_cast<unsigned>(timeout);
  const unsigned fraction = (this->deadline - this->started) % 1000 ? 1 : 0;
  const unsigned seconds = (this->deadline - this->started) / 1000 + fraction;
  this->remaining = seconds;
  this->seconds = seconds;

  if (timeout != 0) {
    this->counting = 1;
    char buffer[512];
    std::sprintf(buffer, this->format.c_str(), seconds);
    this->messageText = buffer;
  } else {
    this->counting = 0;
  }

  this->SetSize(280, 120);
  this->OnCreate(280, 120);
  this->OnSize(280, 120);
  this->Move((g_renderer->viewport.width - 640) / 2 + 185, (g_renderer->viewport.height - 480) / 2 + 180);

  this->viewer = new UITextViewer();
  this->viewer->Configure(10, 22, 260, 48, 0);
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->viewer->backgroundColor = RGB(red, green, blue) | 0xFF000000u;
  this->viewer->Invalidate();
  this->AddChild(this->viewer);
  this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->messageText.c_str()), 38, 0, 0);

  this->gauge = new UIBarGraph2();
  this->gauge->SetSize(240, 15);
  this->gauge->Move(20, this->geometry.height - 50);
  this->gauge->SetValue(0);
  g_skinInfo.PaletteChannels(18, 2, red, green, blue);
  this->gauge->m_backgroundRed = red;
  this->gauge->m_backgroundGreen = green;
  this->gauge->m_backgroundBlue = blue;
  g_skinInfo.PaletteChannels(22, 2, red, green, blue);
  this->gauge->m_fillRed = red;
  this->gauge->m_fillGreen = green;
  this->gauge->m_fillBlue = blue;
  this->AddChild(this->gauge);

  const std::string root(prefix);
  auto* button = new UIBitmapButton();
  button->SetBitmap((root + "btn_cancel" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((root + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((root + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(234, 96);
  button->SetId(185);
  this->AddChild(button);
  this->m_message0 = 185;
}

// OFFSET: 0x008BEE20
// FUNCTION: fn_11806
int UIMakeCharWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      if (_mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkOpening.c_str())) ||
          _mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_messageBoxItemLinkClosing.c_str()))) {
        g_windowMgr.Confirm(LookupMessage(MSI_STYLING_SHOP_OPEN_COUPON), 0, true, false, 0, "Message", 280, 120, 0);
        this->m_nameEdit->SetText("");
        return 0;
      }

      if (this->m_nameEdit->Length() < 4) {
        g_windowMgr.Confirm(LookupMessage(MSI_ENTER_NAME_MORE_THAN_4_CHAR), 0, true, false, 0, "Message", 280, 120, 0);
      } else {
        const bool allowed = g_forbiddenWords.Allows(this->m_nameEdit->Text());
        bool valid = true;
        if (g_serviceType == 1) {
          valid = ValidateCharacterName(this->m_nameEdit->Text());
        }

        const bool control = g_session.HasControlCharacter(this->m_nameEdit->Text());
        const bool blank = g_session.IsBlankString(this->m_nameEdit->Text());
        if (control || !allowed || !valid || blank) {
          if (control || !allowed) {
            g_windowMgr.Confirm(LookupMessage(MSI_BAD_NAME), 0, true, false, 0, "Message", 280, 120, 0);
          }
          if (!valid) {
            g_windowMgr.Confirm(LookupMessage(MSI_ENGLISHONLY), 0, true, false, 0, "Message", 280, 120, 0);
          }
          if (blank) {
            g_windowMgr.Confirm(LookupMessage(MSI_NO_SPACE_IN_NAME), 0, true, false, 0, "Message", 280, 120, 0);
          }
        } else if (this->m_nameEdit->Length() >= 24) {
          g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
          this->m_nameEdit->SetText("");
          return 0;
        } else {
          char name[24] = {};
          std::strcpy(name, this->m_nameEdit->Text());
          std::memcpy(this->m_info.name, name, sizeof(this->m_info.name));
          memcpy_s(&g_charInfo, sizeof(g_charInfo), &this->m_info, sizeof(this->m_info));
          if (std::strstr(g_charInfo.name, "그라비티")) {
            return 0;
          }

          _strlwr(name);
          if (std::strstr(name, "gravity")) {
            return 0;
          }
          if ((name[0] == '[' && name[1] == 'g' && name[2] == 'm') || (name[1] == '[' && name[2] == 'g' && name[3] == 'm') ||
              (name[2] == '[' && name[3] == 'g' && name[4] == 'm')) {
            return 0;
          }

          g_modeMgr.gameMode->SendMsg(LMM_SENDCHARINFO, 0, 0, 0, 0);
          g_windowMgr.Schedule(this);
        }
      }

      g_modeMgr.gameMode->SendMsg(LMM_SET_MAKING_CHAR_NAME, reinterpret_cast<int>(this->m_nameEdit->Text()), 0, 0, 0);
      int values[8];
      values[0] = this->m_info.stats[0];
      values[1] = this->m_info.stats[1];
      values[2] = this->m_info.stats[2];
      values[3] = this->m_info.stats[3];
      values[4] = this->m_info.stats[4];
      values[5] = this->m_info.stats[5];
      values[6] = this->m_info.headPalette;
      values[7] = this->m_info.head;
      g_modeMgr.gameMode->SendMsg(LMM_SET_CHAR_PARAM, reinterpret_cast<int>(values), 0, 0, 0);
      return 0;
    }
    case ID_CANCEL:
      g_windowMgr.Schedule(this);
      g_modeMgr.gameMode->SendMsg(MM_COMMAND, 10004, 0, 0, 0);
      return 0;
    case ID_LEFT: {
      int action = this->m_view.action + 1;
      if (action >= this->m_view.actionBase + 8) {
        action = this->m_view.actionBase;
      }
      this->m_view.action = action;
      this->Invalidate();
      return 0;
    }
    case ID_RIGHT:
      if (--this->m_view.action < this->m_view.actionBase) {
        this->m_view.action = this->m_view.actionBase + 7;
      }
      this->Invalidate();
      return 0;
    default:
      return 0;
    }
  case UM_DEFPUSH: {
    char name[25] = {};
    std::strcpy(name, this->m_nameEdit->Text());
    std::memcpy(this->m_info.name, name, sizeof(this->m_info.name));
    this->SendMsg(nullptr, UM_COMMAND, this->m_message0, 0, 0, 0);
    return 0;
  }
  case UM_SCROLLHORZ:
  case UM_SCROLLHORZPAGEUP:
  case UM_SCROLLHORZPAGEDOWN: {
    const int sex = this->m_info.sex;
    if (sender == this->m_hairStyleBar) {
      const int previous = this->m_hairStyle;
      if (message == UM_SCROLLHORZPAGEUP) {
        this->m_hairStyle = previous - 1;
      } else if (message == UM_SCROLLHORZPAGEDOWN) {
        this->m_hairStyle = previous + 1;
      } else {
        this->m_hairStyle = a + previous;
      }
      if (this->m_hairStyle < 0) {
        this->m_hairStyle = 0;
      } else if (this->m_hairStyle > 22) {
        this->m_hairStyle = 22;
      }
      this->m_hairStyleBar->SetPosition(this->m_hairStyle);
      this->m_hairStyleBar->Invalidate();
      this->Invalidate();

      this->m_info.head = static_cast<std::int16_t>(this->m_hairStyle + 1);
      int head = this->m_info.head;
      this->m_view.actionNames[1].assign(g_session.GetHeadActName(this->m_info.job, sex, &head));
      this->m_view.spriteNames[1].assign(g_session.GetHeadSprName(this->m_info.job, sex, &head));
      this->m_info.head = static_cast<std::int16_t>(head);
      this->m_view.imfName = g_session.GetBodyImfName(this->m_info.job, sex, this->m_info.head, this->m_info.bodyCostume);
      if (this->m_view.headPalette) {
        this->m_view.headPaletteName.assign(g_session.GetHeadPalName(head, this->m_info.job, sex, this->m_view.headPalette));
      }
      return this->m_hairStyle - previous;
    }

    if (sender == this->m_hairColorBar) {
      const int previous = this->m_hairColor;
      if (message == UM_SCROLLHORZPAGEUP) {
        this->m_hairColor = previous - 1;
      } else if (message == UM_SCROLLHORZPAGEDOWN) {
        this->m_hairColor = previous + 1;
      } else {
        this->m_hairColor = a + previous;
      }
      if (this->m_hairColor < 0) {
        this->m_hairColor = 0;
      } else if (this->m_hairColor > 8) {
        this->m_hairColor = 8;
      }
      this->m_hairColorBar->SetPosition(this->m_hairColor);
      this->m_hairColorBar->Invalidate();
      this->Invalidate();

      this->m_view.headPalette = this->m_hairColor;
      if (this->m_view.headPalette) {
        this->m_view.headPaletteName = g_session.GetHeadPalName(this->m_info.head, this->m_info.job, sex, this->m_view.headPalette);
      }
      this->m_info.headPalette = static_cast<std::int16_t>(this->m_view.headPalette);
      return this->m_hairColor - previous;
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008AF190
// FUNCTION: fn_11742
void UILoginWnd::OnCreate(int, int) {
  this->backgroundWindow = g_windowMgr.EnsureWindow(WID_RPIMAGEWND2);
  UIRPImageWnd2* backgroundWindow = static_cast<UIRPImageWnd2*>(this->backgroundWindow);
  backgroundWindow->SetImage(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bg_login.tga", 0x13E33FA1A591AB67ULL, backgroundName), 5);

  const std::string uiPath(OBFUSCATE_TEXT_NAMED("유저인터페이스\\", 0x17A9E5BF39AD4737ULL, uiPathName));
  const std::string loginPath(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\", 0x9949BFD527532943ULL, loginPathName));
  this->passwordPlaceholder = true;

  UIStaticText* label = new UIStaticText();
  label->SetSize(72, 22);
  label->SetText(LookupMessage(MSI_CREATE_RAG_ACCOUNT), 0);
  label->SetColor(51, 51, 51);
  label->Move(73, 87);
  label->SetFont(0, 11, 0, 0);
  this->AddChild(label);

  UIBmpTextButton* button;
  button = new UIBmpTextButton();
  button->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_close_normal.bmp", 0xCD8F077123FB7D6FULL, closeNormalName), 0, 0);
  button->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_close_over.bmp", 0x637B959D55771F69ULL, closeOverName), 1, 0);
  button->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_close_press.bmp", 0xD1413DBB01CD8B51ULL, closePressName), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight() + 2);
  button->Move(13, 104);
  button->SetId(285);
  {
    const std::string text(LookupMessage(MSI_ENST_BLESSING2));
    button->text = text;
    button->Invalidate();
  }
  button->AddTextStyle(0, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(1, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(2, 12, 0x313131, 1, 1, -1000, -1000);
  this->AddChild(button);

  button = new UIBmpTextButton();
  button->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_join_normal.bmp", 0x758F1D0B4F7705B9ULL, joinNormalName), 0, 0);
  button->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_join_over.bmp", 0x891B232D09451965ULL, joinOverName), 1, 0);
  button->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_join_press.bmp", 0xF161095D0DE18F95ULL, joinPressName), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight() + 2);
  button->Move(87, 104);
  button->SetId(267);
  {
    const std::string text(LookupMessage(MSI_ADVENTURER_AGENCY_REGIST_FAILED));
    button->text = text;
    button->Invalidate();
  }
  button->AddTextStyle(0, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(1, 12, 0x313131, 1, 1, -1000, -1000);
  button->AddTextStyle(2, 12, 0x313131, 1, 1, -1000, -1000);
  this->AddChild(button);

  UIBitmapButton* bitmapButton;
  bitmapButton = new UIBitmapButton();
  bitmapButton->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_start_normal.bmp", 0xEDC74F6D13DF71EFULL, startNormalName), 0, 0);
  bitmapButton->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_start_over.bmp", 0x75ADC57D87EFBD8FULL, startOverName), 1, 0);
  bitmapButton->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_start_press.bmp", 0x2BAD156DE3E1F9D9ULL, startPressName), 2, 0);
  bitmapButton->SetSize(bitmapButton->BitmapWidth(), bitmapButton->BitmapHeight());
  bitmapButton->Move(192, 32);
  bitmapButton->SetId(186);
  this->AddChild(bitmapButton);

  bitmapButton = new UIBitmapButton();
  bitmapButton->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_close2_normal.bmp", 0x2FE993D1DDE1F197ULL, exitNormalName), 0, 0);
  bitmapButton->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_close2_over.bmp", 0x39FF7FB50DEB998FULL, exitOverName), 1, 0);
  bitmapButton->SetBitmap(OBFUSCATE_TEXT_NAMED("유저인터페이스\\login_interface\\bt_close2_press.bmp", 0x2F8B0F37118D157FULL, exitPressName), 2, 0);
  bitmapButton->SetSize(bitmapButton->BitmapWidth(), bitmapButton->BitmapHeight());
  bitmapButton->Move(273, 15);
  bitmapButton->SetId(221);
  this->AddChild(bitmapButton);
  this->m_message0 = 186;

  this->passwordEdit = new UIEditCtrl_Custom();
  this->passwordEdit->SetSize(153, 16);
  this->passwordEdit->Move(15, 64);
  this->passwordEdit->labelAlignment = 2;
  this->passwordEdit->Background(255, 255, 255);
  this->passwordEdit->SetText(OBFUSCATE_TEXT_NAMED("PASSWORD", 0xAD075B8DB599F1CDULL, placeholderName));
  this->passwordEdit->TextColor(156, 156, 156, 0xFFFFFFFF);
  this->passwordEdit->mask = '*';
  this->AddChild(this->passwordEdit);

  this->idEdit = new UIEditCtrl_Custom();
  this->idEdit->SetSize(153, 16);
  this->idEdit->Move(15, 40);
  this->idEdit->labelAlignment = 2;
  this->idEdit->Background(255, 255, 255);
  this->idEdit->TextColor(156, 156, 156, 0xFFFFFFFF);
  this->AddChild(this->idEdit);
  this->LoadLogin();

  UICheckBox* saveIdCheck = new UICheckBox();
  const std::string checkedImage = loginPath + std::string(OBFUSCATE_TEXT_NAMED("checkbox_on.bmp", 0x4941FD9F21BBB9FDULL, checkedImageName));
  const std::string uncheckedImage = loginPath + std::string(OBFUSCATE_TEXT_NAMED("checkbox_off.bmp", 0x35E5656B5DEDCD75ULL, uncheckedImageName));
  saveIdCheck->SetBitmaps(checkedImage.c_str(), uncheckedImage.c_str());
  saveIdCheck->SetSize(saveIdCheck->geometry.width, saveIdCheck->geometry.height);
  saveIdCheck->Move(56, 85);
  saveIdCheck->SetChecked(this->saveId);
  this->AddChild(saveIdCheck);

  this->loginTypeCombo = new UIComboBoxDef2Wnd();
  this->loginTypeCombo->skin = 1;
  this->loginTypeCombo->alignment = 1;
  this->loginTypeCombo->dropdownHeight = 18;
  this->loginTypeCombo->textOffset = 3;
  this->loginTypeCombo->SetSize(160, 22);
  this->loginTypeCombo->Move(13, 10);
  this->AddChild(this->loginTypeCombo);
  this->loginTypeCombo->AddOption(LookupMessage(MSI_ENTER_THE_SEARCH_WORD), 0);
  this->loginTypeCombo->AddOption(LookupMessage(MSI_MOTP), 0);
  this->loginTypeCombo->Clear();
  this->loginTypeCombo->AddOption(LookupMessage(MSI_ENTER_THE_SEARCH_WORD), 0);
  this->loginTypeCombo->Select(0);
  this->SendMsg(this->loginTypeCombo, UM_COMBOBOX_SELECTED, 0, 0, 0, 0);
  this->idEdit->maximum = 24;
  this->passwordEdit->maximum = 24;

  if (std::strcmp(this->idEdit->Text(), "") != 0 && std::strcmp(this->idEdit->Text(), "ID") != 0) {
    g_windowMgr.SetFocus(this->passwordEdit);
  } else {
    g_windowMgr.SetFocus(this->idEdit);
  }
}
