#include "Contents/MessageTable.h"
#include "Client/Mode/CMode.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UISlotBmpButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/FuncDrawWNDChar.h"
#include "Client/Registry.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CLoginMode.h"
#include "Utility/Timestamp.h"
#include "Utility/Text/FormatString.h"
#include "Client/Session.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Contents/Actors/Pc.h"
#include "Contents/SpriteRidingOffset.h"
#include "UI/UIStaticBitmap.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIChangeNameWnd.h"

#include <windows.h>
#include <mmsystem.h>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>

// GLOBAL: gl_25006
int g_sub_1319eb4 = 0;

// GLOBAL: gl_25007
int g_normalSlotCount;

// GLOBAL: gl_25008
int g_premiumSlotCount;

// GLOBAL: gl_25009
int g_billingSlotCount;

// GLOBAL: gl_25010
int g_producibleSlotCount;

// GLOBAL: gl_25011
int g_validSlotCount;

// GLOBAL: gl_25012
int g_doramSex;

// GLOBAL: gl_25013
char g_selectedSlotIndex;

// GLOBAL: gl_24792
void* g_charViewMode;

extern const char* LookupMessage(int id);

#pragma pack(push, 1)
struct BlockedCharacterEntry {
  /* 0000 */ std::uint32_t gid;
  /* 0004 */ char expireDate[20];
};

struct CharacterGidPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t gid;
};

struct CharacterMovePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t from;
  /* 0004 */ std::int16_t to;
  /* 0006 */ std::int16_t count;
};
#pragma pack(pop)

static_assert(sizeof(BlockedCharacterEntry) == 0x18);
static_assert(sizeof(CharacterGidPacket) == 6);
static_assert(sizeof(CharacterMovePacket) == 8);


// OFFSET: 0x007D4190
// FUNCTION: fn_09577
UINewSelectCharWnd::UINewSelectCharWnd() : UIFrameWnd(0) {
  this->m_slotsPerPage = 15;
  this->m_unknown13C = false;
  this->m_window100 = nullptr;
  this->m_textButton = nullptr;
  this->m_button416 = nullptr;
  this->m_button417 = nullptr;
  this->m_windowC0 = nullptr;
  this->m_windowC4 = nullptr;

  this->m_slotButtons.resize(12);
  this->m_slotButtons2.resize(this->m_slotsPerPage);
  this->m_characterWindows.resize(this->m_slotsPerPage);

  this->m_unknown118 = false;
  this->m_page = 0;
  this->m_pageSlot = 0;
  this->m_totalCharacters = this->m_maxCharacters = g_normalSlotCount + g_premiumSlotCount + g_billingSlotCount;

  const int slotCount = std::max(static_cast<CLoginMode*>(g_modeMgr.GetLoginMode())->GetMaxCharacterSlot() + 1, this->m_maxCharacters);
  this->m_pageCount = slotCount / this->m_slotsPerPage;
  if (slotCount % this->m_slotsPerPage != 0) {
    this->m_pageCount++;
  }
  this->m_totalSlots = this->m_slotsPerPage * this->m_pageCount;

  this->m_charSlotInfos = new CharSlotInfo[this->m_totalSlots];
  this->m_slotStatus = new char[this->m_totalSlots];
  this->m_slotShorts = new short[this->m_totalSlots];
  for (int i = 0; i < this->m_totalSlots; i++) {
    this->m_slotStatus[i] = 1;
  }

  this->m_lastTick = timeGetTime();
  this->m_unknown138 = -1;
}

UINewSelectCharWnd::~UINewSelectCharWnd() = default;

// OFFSET: 0x007D4520
// FUNCTION: fn_09578
CharSlotInfo::CharSlotInfo() = default;

// OFFSET: 0x007D4A00
// FUNCTION: fn_09583
CharSlotInfo::~CharSlotInfo() = default;

// OFFSET: 0x007D5000
// FUNCTION: fn_09586
void Nullsub243() {
}

// OFFSET: 0x007D50D0
// FUNCTION: fn_09587
std::string UIStaticText::GetText() const {
  return std::string(this->text.c_str(), this->text.size());
}

// OFFSET: 0x007D5130
// FUNCTION: fn_09588
void UINewSelectCharWnd::DisableButtonById(int id) {
  switch (id) {
  case 416:
    this->m_button416->Enable(0);
    break;
  case 417:
    this->m_button417->Enable(0);
    break;
  }
}

// OFFSET: 0x007D52B0
// FUNCTION: fn_09590
void UINewSelectCharWnd::Slot15(int width, int height) {
  HKEY key;
  const LSTATUS opened = RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key);
  this->m_page = 0;
  this->m_pageSlot = 0;
  if (opened == ERROR_SUCCESS) {
    RegCloseKey(key);
    HKEY slotKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &slotKey) == ERROR_SUCCESS) {
      DWORD type = REG_DWORD;
      DWORD size = sizeof(DWORD);
      DWORD slot;
      RegQueryValueExA(slotKey, "CURSLOT", nullptr, &type, reinterpret_cast<LPBYTE>(&slot), &size);
      RegCloseKey(slotKey);
      this->m_page = slot / this->m_slotsPerPage;
      this->m_pageSlot = slot % this->m_slotsPerPage;
      if (this->m_page >= this->m_pageCount || this->m_page < 0) {
        this->m_page = 0;
      }
    }
  }

  g_session.m_options.Set(6, 1);
  g_selectedSlotIndex = static_cast<char>(this->m_slotsPerPage * this->m_page + this->m_pageSlot);

  for (int index = 0; index < this->m_slotsPerPage; ++index) {
    const int slot = this->m_page * this->m_slotsPerPage + index;
    UISlotBmpButton* button = new UISlotBmpButton(index, true);
    button->SetBitmap("유저인터페이스\\select_character_ver3\\img_slot2_normal.bmp", 0, 0);
    button->SetBitmap("유저인터페이스\\select_character_ver3\\img_slot2_over.bmp", 1, 0);
    button->SetBitmap("유저인터페이스\\select_character_ver3\\img_slot2_press.bmp", 2, 0);
    button->SetBitmap("유저인터페이스\\select_character_ver3\\img_slot_lock.bmp", 3, 0);
    button->SetSize(0xA1, 0xC5);
    button->Move((index % 5) * 0x9D + 10, (index / 5) * 0xC3 + 0x15);
    button->SetId(0xCC);
    button->SetSlot(slot, -1);
    this->AddChild(button);
    this->m_slotButtons2[index] = button;
    button->SendMsg(this, 13, index == this->m_pageSlot, 0, 0, 0);
  }

  UIStaticBitmap* info = new UIStaticBitmap();
  info->SetBitmap("유저인터페이스\\select_character_ver3\\img_info.bmp");
  info->SetSize(info->geometry.width, info->geometry.height);
  info->Move(0x329, 0x1A);
  this->AddChild(info);

  UIBitmapButton* closeButton = new UIBitmapButton();
  closeButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_close2_normal.bmp", 0, 0);
  closeButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_close2_over.bmp", 1, 0);
  closeButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_close2_press.bmp", 2, 0);
  closeButton->SetSize(closeButton->BitmapWidth(), closeButton->BitmapHeight());
  closeButton->Move(this->geometry.width - 0x34, 5);
  closeButton->SetId(0xB9);
  this->AddChild(closeButton);

  this->m_textButton = new UIBmpTextButton();
  this->m_textButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_normal.bmp", 0, 0);
  this->m_textButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_over.bmp", 1, 0);
  this->m_textButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_press.bmp", 2, 0);
  this->m_textButton->SetSize(this->m_textButton->BitmapWidth(), this->m_textButton->BitmapHeight());
  this->m_textButton->Move(0x33E, 0x15F);
  this->m_textButton->SetText(LookupMessage(0xD10));
  this->m_textButton->AddTextStyle(0, 0xC, 0x313131, 0, 1, -1000, -1000);
  this->m_textButton->AddTextStyle(1, 0xC, 0x313131, 0, 1, -1000, -1000);
  this->m_textButton->AddTextStyle(2, 0xC, 0x313131, 0, 1, -1000, -1000);
  this->m_textButton->SetId(0x11E);
  this->m_textButton->Enable(0);
  this->AddChild(this->m_textButton);

  UIBmpTextButton* startButton = new UIBmpTextButton();
  this->m_windowC0 = startButton;
  startButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_normal.bmp", 0, 0);
  startButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_over.bmp", 1, 0);
  startButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_press.bmp", 2, 0);
  startButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_normal.bmp", 3, 0);
  startButton->SetSize(startButton->BitmapWidth(), startButton->BitmapHeight());
  startButton->Move(0x33E, 0x142);
  startButton->SetText(LookupMessage(0xD0B));
  startButton->AddTextStyle(0, 0xC, 0x313131, 0, 1, -1000, -1000);
  startButton->AddTextStyle(1, 0xC, 0x313131, 0, 1, -1000, -1000);
  startButton->AddTextStyle(2, 0xC, 0x313131, 0, 1, -1000, -1000);
  startButton->AddTextStyle(3, 0xC, 0x909090, 0, 0, -1000, -1000);
  startButton->SetId(0x197);
  this->AddChild(startButton);
  if (this->m_slotStatus[this->m_slotsPerPage * this->m_page + this->m_pageSlot] == 1) {
    startButton->SetDisabled(1);
    startButton->Invalidate();
  }

  UIBmpTextButton* cancelDeleteButton = new UIBmpTextButton();
  this->m_windowC4 = cancelDeleteButton;
  cancelDeleteButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_normal.bmp", 0, 0);
  cancelDeleteButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_over.bmp", 1, 0);
  cancelDeleteButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_press.bmp", 2, 0);
  cancelDeleteButton->SetSize(cancelDeleteButton->BitmapWidth(), cancelDeleteButton->BitmapHeight());
  cancelDeleteButton->Move(0x33E, 0x142);
  cancelDeleteButton->SetText(LookupMessage(0xD0C));
  cancelDeleteButton->AddTextStyle(0, 0xC, 0x313131, 0, 1, -1000, -1000);
  cancelDeleteButton->AddTextStyle(1, 0xC, 0x313131, 0, 1, -1000, -1000);
  cancelDeleteButton->AddTextStyle(2, 0xC, 0x313131, 0, 1, -1000, -1000);
  cancelDeleteButton->SetId(0x198);
  cancelDeleteButton->Enable(0);
  this->AddChild(cancelDeleteButton);

  UIBmpTextButton* gameStartButton = new UIBmpTextButton();
  this->m_windowFC = gameStartButton;
  gameStartButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_gameStart_off.bmp", 0, 0);
  gameStartButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_gameStart_over.bmp", 1, 0);
  gameStartButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_gameStart_press.bmp", 2, 0);
  gameStartButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_gameStart_off.bmp", 3, 0);
  gameStartButton->SetSize(gameStartButton->BitmapWidth(), gameStartButton->BitmapHeight());
  gameStartButton->Move(0x330, 0x197);
  gameStartButton->SetId(0xB8);
  gameStartButton->SetText(LookupMessage(0xD0D));
  gameStartButton->AddTextStyle(0, 0x12, 0xFFFFFF, 1, 1, -1000, 0x39);
  gameStartButton->AddTextStyle(1, 0x12, 0xFFFFFF, 1, 1, -1000, 0x39);
  gameStartButton->AddTextStyle(2, 0x12, 0xFFFFFF, 1, 1, -1000, 0x39);
  gameStartButton->AddTextStyle(3, 0x12, 0x7F74DC, 1, 0, -1000, 0x39);
  this->AddChild(gameStartButton);

  UIBmpTextButton* deleteButton = new UIBmpTextButton();
  this->m_window100 = deleteButton;
  deleteButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_normal.bmp", 0, 0);
  deleteButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_over.bmp", 1, 0);
  deleteButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_info_press.bmp", 2, 0);
  deleteButton->SetSize(deleteButton->BitmapWidth(), deleteButton->BitmapHeight());
  deleteButton->Move(0x33E, 0x15F);
  deleteButton->SetText(LookupMessage(0xD0E));
  deleteButton->AddTextStyle(0, 0xC, 0x313131, 0, 1, -1000, -1000);
  deleteButton->AddTextStyle(1, 0xC, 0x313131, 0, 1, -1000, -1000);
  deleteButton->AddTextStyle(2, 0xC, 0x313131, 0, 1, -1000, -1000);
  deleteButton->SetId(0xD3);
  deleteButton->Enable(0);
  this->AddChild(deleteButton);

  this->m_button416 = new UIBitmapButton();
  this->m_button416->SetBitmap("유저인터페이스\\btn_make.bmp", 0, 0);
  this->m_button416->SetBitmap("유저인터페이스\\btn_make_a.bmp", 1, 0);
  this->m_button416->SetBitmap("유저인터페이스\\btn_make_b.bmp", 2, 0);
  this->m_button416->SetSize(this->m_button416->BitmapWidth(), this->m_button416->BitmapHeight());
  this->m_button416->Move(200, 0x14E);
  this->m_button416->SetId(0x1A0);
  this->m_button416->Enable(0);
  this->AddChild(this->m_button416);

  this->m_button417 = new UIBitmapButton();
  this->m_button417->SetBitmap("유저인터페이스\\btn_del.bmp", 0, 0);
  this->m_button417->SetBitmap("유저인터페이스\\btn_del_a.bmp", 1, 0);
  this->m_button417->SetBitmap("유저인터페이스\\btn_del_b.bmp", 2, 0);
  this->m_button417->SetSize(this->m_button417->BitmapWidth(), this->m_button417->BitmapHeight());
  this->m_button417->Move(200, 0x14E);
  this->m_button417->SetId(0x1A1);
  this->m_button417->Enable(0);
  this->AddChild(this->m_button417);
  this->m_button416->Enable(0);
  this->m_button417->Enable(0);

  this->m_prevPageButton = new UIBitmapButton();
  const char* prevNormal;
  const char* prevOver;
  const char* prevPress;
  if (this->m_page == 0) {
    prevPress = "유저인터페이스\\select_character_ver3\\bt_left_off.bmp";
    prevOver = prevPress;
    prevNormal = prevPress;
  } else {
    prevPress = "유저인터페이스\\select_character_ver3\\bt_left_press.bmp";
    prevOver = "유저인터페이스\\select_character_ver3\\bt_left_over.bmp";
    prevNormal = "유저인터페이스\\select_character_ver3\\bt_left_on.bmp";
  }
  this->m_prevPageButton->SetBitmap(prevNormal, 0, 0);
  this->m_prevPageButton->SetBitmap(prevOver, 1, 0);
  this->m_prevPageButton->SetBitmap(prevPress, 2, 0);
  this->m_prevPageButton->SetSize(this->m_prevPageButton->BitmapWidth(), this->m_prevPageButton->BitmapHeight());
  this->m_prevPageButton->Move(0x33E, 0x22E);
  this->m_prevPageButton->SetId(0x13C);
  this->AddChild(this->m_prevPageButton);

  this->m_nextPageButton = new UIBitmapButton();
  const char* nextNormal;
  const char* nextOver;
  const char* nextPress;
  if (this->m_page < this->m_pageCount - 1) {
    nextPress = "유저인터페이스\\select_character_ver3\\bt_right_press.bmp";
    nextOver = "유저인터페이스\\select_character_ver3\\bt_right_over.bmp";
    nextNormal = "유저인터페이스\\select_character_ver3\\bt_right_on.bmp";
  } else {
    nextPress = "유저인터페이스\\select_character_ver3\\bt_right_off.bmp";
    nextOver = nextPress;
    nextNormal = nextPress;
  }
  this->m_nextPageButton->SetBitmap(nextNormal, 0, 0);
  this->m_nextPageButton->SetBitmap(nextOver, 1, 0);
  this->m_nextPageButton->SetBitmap(nextPress, 2, 0);
  this->m_nextPageButton->SetSize(this->m_nextPageButton->BitmapWidth(), this->m_nextPageButton->BitmapHeight());
  this->m_nextPageButton->Move(0x3AB, 0x22E);
  this->m_nextPageButton->SetId(0x13D);
  this->AddChild(this->m_nextPageButton);

  UIStaticText* pageTitle = new UIStaticText();
  const int titleWidth = this->TextWidth(LookupMessage(0xD0F), 0, 0, 0xC, 1, 0);
  pageTitle->SetSize(titleWidth, this->TextHeight(LookupMessage(0xD0F), 0, 0, 0xC, 1, 0));
  pageTitle->SetText(LookupMessage(0xD0F), 0);
  pageTitle->SetColor(0x57, 0x66, 0x91);
  pageTitle->Move(0x380 - titleWidth / 2, 0x21A);
  this->AddChild(pageTitle);

  char pageText[16] = {};
  std::sprintf(pageText, "%d / %d", this->m_page + 1, this->m_pageCount);
  this->m_pageText = new UIStaticText();
  const int pageWidth = this->TextWidth(pageText, 0, 0, 0x12, 1, 0);
  this->m_pageText->SetSize(pageWidth, this->TextHeight(pageText, 0, 0, 0x12, 1, 0));
  this->m_pageText->SetText(pageText, 0);
  this->m_pageText->SetFont(0, 0x12, 1, 0);
  this->m_pageText->SetColor(0xFF, 0xFF, 0xFF);
  this->m_pageText->Move(0x380 - pageWidth / 2, 0x234);
  this->AddChild(this->m_pageText);

  for (int index = 0; index < 12; ++index) {
    this->m_slotButtons[index] = new UIStaticText();
    static_cast<UIStaticText*>(this->m_slotButtons[index])->SetFont(0, 0xB, 0, 0);
    this->m_slotButtons[index]->SetSize(0x6B, 0xF);
    if (index == 11) {
      this->m_slotButtons[index]->Move(0x364, 0x74);
    } else {
      this->m_slotButtons[index]->Move(0x364, index * 0x11 + 0x85);
    }
    this->AddChild(this->m_slotButtons[index]);
  }

  for (int index = 0; index < this->m_totalSlots; ++index) {
    CharacterInfo* character = reinterpret_cast<CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, index, 0, 0, 0));
    if (character == nullptr) {
      continue;
    }

    if (character->sex == 'c') {
      character->sex = static_cast<unsigned char>(g_doramSex);
    }

    if (g_session.IsListedId(character->job, character->sex) == 1) {
      char message[260];
      std::memset(message, 0, sizeof(message));
      std::sprintf(message, LookupMessage(0x974), index, 0x9C41);
      g_windowMgr.Confirm(message, 0, true, false, 0, "Message", 0x118, 0x78, 0);
      static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
      break;
    }

    this->m_slotMap[index] = character->gid;
    const int slot = character->slot;
    if (slot >= this->m_totalSlots) {
      continue;
    }

    this->m_slotStatus[slot] = 0;
    const int page = slot / this->m_slotsPerPage;
    const int column = page != 0 ? index % (page * this->m_slotsPerPage) : slot;
    CharSlotInfo& slotInfo = this->m_charSlotInfos[slot];
    slotInfo.x = (column % 5) * 0x9D + 0x5A;
    slotInfo.y = (column / 5) * 0xC3 + 0x9E;
    slotInfo.job = character->job;
    slotInfo.robe = character->robe;
    if (character->deleteDate > 0) {
      slotInfo.unknown_0148 = 0x10;
      slotInfo.action = 0x10;
    } else {
      slotInfo.unknown_0148 = 0;
      slotInfo.action = 0;
    }
    slotInfo.frame = 0;

    slotInfo.actionNames[0] = GetBodyActionResourceName(character->job, character->sex, character->unknown_0058, 0);
    slotInfo.spriteNames[0] = g_session.GetBodySprName(character->job, character->sex, character->unknown_0058, 0);
    int head = character->head;
    slotInfo.actionNames[1] = g_session.GetHeadActName(character->job, character->sex, &head);
    slotInfo.spriteNames[1] = g_session.GetHeadSprName(character->job, character->sex, &head);
    slotInfo.actionNames[2] = g_session.GetAccessoryActName(character->job, character->sex, head, character->accessory3);
    slotInfo.spriteNames[2] = g_session.GetAccessorySprName(character->job, character->sex, head, character->accessory3);
    character->head = static_cast<std::int16_t>(head);
    slotInfo.actionNames[3] = g_session.GetAccessoryActName(character->job, character->sex, head, character->accessory2);
    slotInfo.spriteNames[3] = g_session.GetAccessorySprName(character->job, character->sex, head, character->accessory2);
    this->m_slotShorts[slot] = character->accessory2;
    slotInfo.actionNames[4] = g_session.GetAccessoryActName(character->job, character->sex, head, character->accessory);
    slotInfo.spriteNames[4] = g_session.GetAccessorySprName(character->job, character->sex, head, character->accessory);

    slotInfo.bodyPalette = g_session.GetBodyStyle(character->job, character->bodyStyle, character->unknown_0058);
    if (slotInfo.bodyPalette != 0) {
      slotInfo.bodyPaletteName = g_session.GetBodyPalName(character->job, character->sex, slotInfo.bodyPalette, character->unknown_0058, 0, 0);
    }

    slotInfo.headPalette = character->headPalette;
    if (slotInfo.headPalette != 0) {
      slotInfo.headPaletteName = g_session.GetHeadPalName(head, character->job, character->sex, slotInfo.headPalette);
    }

    slotInfo.imfName = GetBodyImfName(character->job, character->sex, character->head, character->unknown_0058);
  }

  for (int index = 0; index < this->m_totalSlots; ++index) {
    if (this->m_page != index / this->m_slotsPerPage) {
      continue;
    }

    const int column = this->m_page != 0 ? index % (this->m_page * this->m_slotsPerPage) : index;
    UIStaticText* name = new UIStaticText();
    name->SetSize(100, 0x14);
    const int x = (column % 5) * 0x9D;
    const int y = (column / 5) * 0xC3;
    name->Move(x + 0x58, y + 0x15);
    name->SetFont(0, 0xB, 1, 0);
    name->SetColor(0x37, 0x41, 0x77);
    this->AddChild(name);
    this->m_characterWindows[column] = name;

    const CharacterInfo* character = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, index, 0, 0, 0));
    if (character != nullptr) {
      name->SetTextFitted(character->name);
      name->Move(x + 0x58 - this->TextWidth(name->GetText().c_str(), 0, 0, 0xC, 0, 0) / 2, y + 0xBD);
    }
  }

  this->sub_7D9310();
  this->m_message0 = 0xB8;
}

// OFFSET: 0x007D7330
// FUNCTION: fn_09591
void UINewSelectCharWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);

  for (int index = 0; index < this->m_slotsPerPage; ++index) {
    const int slot = this->m_page * this->m_slotsPerPage + index;
    if (slot >= this->m_totalSlots) {
      continue;
    }

    const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, slot, 0, 0, 0));
    const int column = slot % this->m_slotsPerPage;
    const char* label = nullptr;
    if (g_premiumSlotCount > 0 && slot >= g_normalSlotCount && slot < g_normalSlotCount + g_premiumSlotCount) {
      label = "Premium ";
    } else if (g_billingSlotCount > 0 && slot >= g_normalSlotCount + g_premiumSlotCount && slot < g_billingSlotCount + g_normalSlotCount + g_premiumSlotCount) {
      label = "Billing ";
    }

    if (label) {
      this->DrawTextAt(column * 163 + 136, 50, label, 0, 1, 15, 0xFF, 0, 0);
      this->DrawTextAt(column * 163 + 142, 63, "Service", 0, 1, 15, 0xFF, 0, 0);
    }

    if (this->m_slotStatus[slot] == 0 && info) {
      const CharSlotInfo& slotInfo = this->m_charSlotInfos[slot];
      const unsigned char sex = info->sex == 99 ? static_cast<unsigned char>(g_doramSex) : info->sex;
      FuncDrawWNDChar(
        this, slotInfo.x, slotInfo.y, sex, info->job, info->job, info->unknown_0058, info->head, info->accessory, info->accessory2, info->accessory3, info->robe,
        0, 0, info->bodyStyle, info->headPalette, slotInfo.action, slotInfo.frame, this->m_lastTick
      )
        .Draw(true);
    }

    const int page = slot / this->m_slotsPerPage;
    const int buttonIndex = page != 0 ? slot % (page * this->m_slotsPerPage) : slot;
    if (buttonIndex < this->m_slotsPerPage) {
      if (slot >= this->m_totalSlots) {
        continue;
      }

      static_cast<UIButton*>(this->m_slotButtons2[buttonIndex])->Enable(1);
      if (slot < g_producibleSlotCount ? this->m_slotStatus[slot] == 1 && slot >= g_validSlotCount : this->m_slotStatus[slot] == 1) {
        auto* button = static_cast<UIButton*>(this->m_slotButtons2[buttonIndex]);
        button->SetDisabled(1);
        button->Invalidate();
      }
    }

    if (slot >= this->m_totalSlots || !info) {
      continue;
    }

    if (info->deleteDate == 0) {
      this->m_charSlotInfos[slot].unknown_0148 = 0;
      this->m_charSlotInfos[slot].action = 0;
    } else {
      Timestamp deleteDate(info->deleteDate);
      if (slot == this->m_pageSlot && deleteDate.IsBeforeNow() == 1 && static_cast<UIButton*>(this->m_window100)->Disabled() == 1) {
        static_cast<UIButton*>(this->m_window100)->Enable(1);
      }
      this->m_charSlotInfos[slot].unknown_0148 = 16;
      this->m_charSlotInfos[slot].action = 16;
    }
  }

  const CharacterInfo* selected = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
  if (selected && selected->deleteDate > 0) {
    Timestamp deleteDate(selected->deleteDate);
    auto* button = static_cast<UIButton*>(this->m_window100);
    if (deleteDate.IsBeforeNow() == 1) {
      button->SetDisabled(0);
    } else {
      button->SetDisabled(1);
    }
    button->Invalidate();
  }
}

// OFFSET: 0x007D7740
// FUNCTION: fn_09592
void __stdcall Nullsub244(int, int) {
}

// OFFSET: 0x007D7750
// FUNCTION: fn_09593
void UINewSelectCharWnd::Slot19() {
  if (g_sub_1319eb4 == 3) {
    if (g_windowMgr.unknown_0210 != 0) {
      g_windowMgr.CloseWindow(0x12);
    }
  }
  if (g_sub_1319eb4 != 0) {
    this->Invalidate();
  }

  UIWindow* wnd = g_windowMgr.LookupWindow(93);
  if (wnd) {
    g_windowMgr.Promote(wnd);
  }

  if (this->m_pageSlot >= 0 && this->m_pageSlot < static_cast<int>(this->m_slotButtons2.size())) {
    UIWindow* btn = this->m_slotButtons2[this->m_pageSlot];
    if (btn) {
      btn->Slot19();
      btn->InvalidateChildren();
    }
  }
}

// OFFSET: 0x007D77D0
// FUNCTION: fn_09594
int UINewSelectCharWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (g_windowMgr.unknown_03BC != nullptr || g_windowMgr.unknown_03B8 != nullptr || g_windowMgr.LookupWindow(0x116) != nullptr) {
    return 0;
  }

  switch (message) {
  case 6:
    switch (argument1) {
    case 0xB8: {
      if (this->m_unknown13C) {
        break;
      }

      CharacterInfo* info =
        reinterpret_cast<CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
      if (info == nullptr) {
        this->SendMsg(nullptr, 6, 0xCB, this->m_pageSlot, 0, 0);
        break;
      }

      if (info->deleteDate > 0) {
        break;
      }

      g_session.SetPlayerNameKey(reinterpret_cast<const unsigned char*>(info->name));
      g_session.SetFlag(info->sex == 'c' ? static_cast<char>(g_doramSex) : static_cast<char>(info->sex));
      this->m_unknown118 = true;
      const BlockedCharacterEntry* blocked = reinterpret_cast<const BlockedCharacterEntry*>(&g_session.unknown_07F4[0x40]);
      for (int index = 0; index < g_session.unknown_59D0; ++index) {
        if (info->gid == blocked[index].gid) {
          char text[260];
          std::memset(text, 0, sizeof(text));
          std::sprintf(text, LookupMessage(0x337), blocked[index].expireDate);
          g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 0x118, 0x78, 0);
          this->m_unknown118 = false;
          return 0;
        }
      }

      g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      static_cast<CMode*>(g_charViewMode)->SendMsg(0, 0x2712, 0, 0, 0);
      this->m_unknown13C = true;
      if (this->m_windowFC != nullptr) {
        UIBitmapButton* button = static_cast<UIBitmapButton*>(this->m_windowFC);
        button->SetBitmap("유저인터페이스\\select_character_ver3\\bt_gameStart_on.bmp", 0, 0);
        button->SetBitmap("유저인터페이스\\select_character_ver3\\bt_gameStart_on.bmp", 1, 0);
        button->SetBitmap("유저인터페이스\\select_character_ver3\\bt_gameStart_on.bmp", 2, 0);
      }
      break;
    }
    case 0xB9:
      g_accountLinkedUserDataMgr->Save(
        *reinterpret_cast<EmotionHotkeys*>(&g_session.unknown_0FF8), *g_luaScript, g_session.m_aid, GetRenderSystemHolder()->system,
        reinterpret_cast<const char*>(&g_session.unknown_807C[8]), *g_asyncWorkMgr
      );
      if (g_loginModeFlag == 1) {
        if (g_windowMgr.Confirm(LookupMessage(0xD68), 2, true, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
          static_cast<CMode*>(g_charViewMode)->SendMsg(0x271B, 0, 0, 0, 0);
        }
      } else if (g_windowMgr.Confirm(LookupMessage(0x11), 2, true, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
        g_windowMgr.Schedule(this);
      }
      break;
    case 0xCB:
      if (this->m_pageSlot != argument2) {
        this->m_pageSlot = argument2;
        this->SendMsg(this->m_slotButtons2[argument2], 6, 0xCC, 0, 0, 0);
      }
      if (this->m_slotStatus[static_cast<unsigned char>(g_selectedSlotIndex)] == 1 && static_cast<unsigned char>(g_selectedSlotIndex) < g_producibleSlotCount &&
          g_windowMgr.EnsureWindow(0x116) != nullptr) {
        g_windowMgr.CloseWindow(0x115);
      }
      break;
    case 0xCC: {
      if (this->m_unknown13C || g_windowMgr.HasModalWindow() == 1) {
        break;
      }

      for (int index = 0; index < this->m_slotsPerPage; ++index) {
        UIWindow* button = this->m_slotButtons2[index];
        if (sender != button) {
          button->SendMsg(this, 13, 0, 0, 0, 0);
          continue;
        }

        this->m_pageSlot = index;
        g_selectedSlotIndex = static_cast<char>(this->m_page * this->m_slotsPerPage + index);
        this->m_slotButtons2[index]->SendMsg(this, 13, 1, 0, 0, 0);
        if (argument2 != 0xE2 && this->m_slotStatus[static_cast<unsigned char>(g_selectedSlotIndex)] != 0 &&
            static_cast<UISlotBmpButton*>(this->m_slotButtons2[index])->unknown_0128 == 0) {
          this->SendMsg(nullptr, 6, 0xCB, this->m_pageSlot, 0, 0);
          break;
        }
      }

      if (argument2 == 0xE2) {
        bool shown = false;
        const CharacterInfo* info =
          reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
        const int selected = static_cast<unsigned char>(g_selectedSlotIndex);
        if (this->m_unknown138 != selected && selected < this->m_maxCharacters) {
          if (info == nullptr) {
            if (this->m_unknown138 != -1) {
              static_cast<CLoginMode*>(g_modeMgr.GetLoginMode())->ShowNotice(0x2E, false);
            }
          } else {
            if (info->unknown_00A6 > 0) {
              shown = static_cast<CLoginMode*>(g_modeMgr.GetLoginMode())->ShowNotice(0x2D, false);
            }
            if (info->unknown_00AA > 0) {
              static_cast<CLoginMode*>(g_modeMgr.GetLoginMode())->ShowNotice(0x2F, shown);
            }
          }
        } else if (info != nullptr && info->unknown_00AA > 0) {
          static_cast<CLoginMode*>(g_modeMgr.GetLoginMode())->ShowNotice(0x2F, shown);
        }
      }

      this->sub_7D9310();
      this->SaveCurrentSlot();
      this->Invalidate();
      break;
    }
    case 0xD3: {
      if (this->m_slotStatus[static_cast<unsigned char>(g_selectedSlotIndex)] == 1) {
        break;
      }

      const CharacterInfo* info =
        reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
      const BlockedCharacterEntry* blocked = reinterpret_cast<const BlockedCharacterEntry*>(&g_session.unknown_07F4[0x40]);
      for (int index = 0; index < g_session.unknown_59D0; ++index) {
        if (info->gid == blocked[index].gid) {
          char text[260];
          std::memset(text, 0, sizeof(text));
          std::sprintf(text, LookupMessage(0x337), blocked[index].expireDate);
          g_windowMgr.Confirm(text, 0, true, true, 0, "Message", 0x118, 0x78, 0);
          return 0;
        }
      }

      if (g_windowMgr.Confirm(LookupMessage(0x13), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
        this->m_unknown118 = true;
        if (g_windowMgr.RunEmailAddress() == 0xB8) {
          static_cast<CMode*>(g_charViewMode)->SendMsg(0, 0x271A, 0, 0, 0);
          g_windowMgr.Schedule(this);
        }
        this->m_unknown118 = false;
      }
      break;
    }
    case 0x11C:
    case 0x11E: {
      char text[260];
      std::memset(text, 0, sizeof(text));
      static_cast<CMode*>(g_charViewMode)->SendMsg(0x2724, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
      g_windowMgr.Confirm(text, 0, true, false, 0, "Message", 0x118, 0x78, 0);
      break;
    }
    case 0x13C: {
      if (this->m_page <= 0) {
        break;
      }

      --this->m_page;
      const char* prevNormal;
      const char* prevOver;
      const char* prevPress;
      if (this->m_page == 0) {
        prevPress = "유저인터페이스\\select_character_ver3\\bt_left_off.bmp";
        prevOver = prevPress;
        prevNormal = prevPress;
      } else {
        prevPress = "유저인터페이스\\select_character_ver3\\bt_left_press.bmp";
        prevOver = "유저인터페이스\\select_character_ver3\\bt_left_over.bmp";
        prevNormal = "유저인터페이스\\select_character_ver3\\bt_left_on.bmp";
      }
      this->m_prevPageButton->SetBitmap(prevNormal, 0, 0);
      this->m_prevPageButton->SetBitmap(prevOver, 1, 0);
      this->m_prevPageButton->SetBitmap(prevPress, 2, 0);
      this->m_nextPageButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_right_on.bmp", 0, 0);
      this->m_nextPageButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_right_over.bmp", 1, 0);
      this->m_nextPageButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_right_press.bmp", 2, 0);
      if (this->m_pageText != nullptr) {
        char pageText[16] = {};
        std::sprintf(pageText, "%d / %d", this->m_page + 1, this->m_pageCount);
        this->m_pageText->SetText(pageText, 0);
      }

      for (unsigned index = 0; index < static_cast<unsigned>(this->m_totalSlots); ++index) {
        if (static_cast<unsigned>(this->m_page) != index / this->m_slotsPerPage) {
          continue;
        }

        const int column = this->m_page != 0 ? index % (this->m_page * this->m_slotsPerPage) : index;
        UISlotBmpButton* button = static_cast<UISlotBmpButton*>(this->m_slotButtons2[column]);
        if (this->m_slotStatus[index] == 1) {
          button->SetSlot(-1, index);
          this->m_characterWindows[column]->SetText("", 0);
          if (index < static_cast<unsigned>(g_producibleSlotCount)) {
            button->SendMsg(nullptr, 0x1AF, 0, 0, 0, 0);
          } else {
            button->SetDisabled(1);
            button->Invalidate();
          }
          continue;
        }

        button->SetSlot(index, -1);
        button->SendMsg(nullptr, 0x1AF, 1, 0, 0, 0);
        const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, index, 0, 0, 0));
        if (info != nullptr) {
          this->m_characterWindows[column]->SetTextFitted(info->name);
          UIStaticText* name = this->m_characterWindows[column];
          name->Move(
            (column % 5) * 0x9D - this->TextWidth(name->GetText().c_str(), 0, 0, 0xC, 0, 0) / 2 + 0x58,
            (column / 5) * 0xC3 + 0xBD
          );
        }
      }

      this->InvalidateChildren();
      this->sub_7D9310();
      break;
    }
    case 0x13D: {
      if (this->m_page >= this->m_pageCount - 1) {
        break;
      }

      ++this->m_page;
      const char* nextNormal;
      const char* nextOver;
      const char* nextPress;
      if (this->m_page < this->m_pageCount - 1) {
        nextPress = "유저인터페이스\\select_character_ver3\\bt_right_press.bmp";
        nextOver = "유저인터페이스\\select_character_ver3\\bt_right_over.bmp";
        nextNormal = "유저인터페이스\\select_character_ver3\\bt_right_on.bmp";
      } else {
        nextPress = "유저인터페이스\\select_character_ver3\\bt_right_off.bmp";
        nextOver = nextPress;
        nextNormal = nextPress;
      }
      this->m_nextPageButton->SetBitmap(nextNormal, 0, 0);
      this->m_nextPageButton->SetBitmap(nextOver, 1, 0);
      this->m_nextPageButton->SetBitmap(nextPress, 2, 0);
      this->m_prevPageButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_left_on.bmp", 0, 0);
      this->m_prevPageButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_left_over.bmp", 1, 0);
      this->m_prevPageButton->SetBitmap("유저인터페이스\\select_character_ver3\\bt_left_press.bmp", 2, 0);
      if (this->m_pageText != nullptr) {
        char pageText[16] = {};
        std::sprintf(pageText, "%d / %d", this->m_page + 1, this->m_pageCount);
        this->m_pageText->SetText(pageText, 0);
      }

      for (unsigned index = 0; index < static_cast<unsigned>(this->m_totalSlots); ++index) {
        if (static_cast<unsigned>(this->m_page) != index / this->m_slotsPerPage) {
          continue;
        }

        const int column = this->m_page != 0 ? index % (this->m_page * this->m_slotsPerPage) : index;
        UISlotBmpButton* button = static_cast<UISlotBmpButton*>(this->m_slotButtons2[column]);
        if (this->m_slotStatus[index] == 1) {
          button->SetSlot(-1, index);
          this->m_characterWindows[column]->SetText("", 0);
          if (index < static_cast<unsigned>(g_producibleSlotCount)) {
            button->SendMsg(nullptr, 0x1AF, 0, 0, 0, 0);
          } else {
            button->SetDisabled(1);
            button->Invalidate();
          }
          continue;
        }

        button->SetSlot(index, -1);
        button->SendMsg(nullptr, 0x1AF, 1, 0, 0, 0);
        const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, index, 0, 0, 0));
        if (info != nullptr) {
          this->m_characterWindows[column]->SetTextFitted(info->name);
          UIStaticText* name = this->m_characterWindows[column];
          name->Move(
            (column % 5) * 0x9D - this->TextWidth(name->GetText().c_str(), 0, 0, 0xC, 0, 0) / 2 + 0x58,
            (column / 5) * 0xC3 + 0xBD
          );
        }
      }

      this->InvalidateChildren();
      this->sub_7D9310();
      break;
    }
    case 0x15F: {
      if (g_windowMgr.window_03DC != nullptr) {
        g_windowMgr.CloseWindow(0x80);
      }
      const std::intptr_t info = static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0);
      UIWindow* window = g_windowMgr.EnsureWindow(0x80);
      if (window != nullptr) {
        window->SendMsg(nullptr, 0x22, info, 0, 0, 0);
      }
      break;
    }
    case 0x160: {
      CharacterInfo* info =
        reinterpret_cast<CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
      if (info->gid != g_renameGid) {
        break;
      }

      std::memcpy(info->name, g_renameName, sizeof(info->name));
      const int column = static_cast<unsigned char>(g_selectedSlotIndex) - this->m_slotsPerPage * this->m_page;
      this->m_characterWindows[column]->SetTextFitted(info->name);
      UIStaticText* name = this->m_characterWindows[column];
      name->Move(
        (column % 5) * 0x9D - this->TextWidth(name->GetText().c_str(), 0, 0, 0xC, 0, 0) / 2 + 0x58,
        (column / 5) * 0xC3 + 0xBD
      );
      break;
    }
    case 0x197: {
      if (this->m_slotStatus[static_cast<unsigned char>(g_selectedSlotIndex)] == 1) {
        break;
      }

      const CharacterInfo* info =
        reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
      CharacterGidPacket packet;
      packet.id = 0x827;
      packet.gid = info->gid;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      break;
    }
    case 0x198: {
      CharacterGidPacket packet;
      packet.id = 0x82B;
      packet.gid = this->m_slotMap[static_cast<unsigned char>(g_selectedSlotIndex)];
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      break;
    }
    case 0x1A0:
      g_windowMgr.EnsureWindow(200);
      g_windowMgr.unknown_03BC->SendMsg(nullptr, 6, 0x124, 0, 0, 0);
      g_windowMgr.unknown_03BC->SendMsg(nullptr, 6, 0xB8, 1, 0, 0);
      break;
    case 0x1A1:
      g_windowMgr.EnsureWindow(200);
      break;
    case 0x1A2: {
      CharacterGidPacket packet;
      packet.id = 0x8C5;
      packet.gid = g_session.m_aid;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      break;
    }
    case 0x1B0:
      this->m_unknown138 = static_cast<unsigned char>(g_selectedSlotIndex);
      break;
    case 0x1B1: {
      const unsigned char selected = static_cast<unsigned char>(g_selectedSlotIndex);
      if (g_windowMgr.Confirm(LookupMessage(0x7E0), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
        const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, this->m_unknown138, 0, 0, 0));
        if (info != nullptr) {
          CharacterMovePacket packet;
          packet.id = 0x8D4;
          packet.from = static_cast<std::int16_t>(this->m_unknown138);
          packet.to = selected;
          packet.count = static_cast<std::int16_t>(info->unknown_00A6);
          GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
        }
      }
      this->m_unknown138 = -1;
      break;
    }
    case 0x1B2:
      this->m_slotButtons2[0]->Slot25(0, 0);
      this->SendMsg(nullptr, 6, 0x1B0, 0, 0, 0);
      break;
    case 0x1B3:
      this->m_slotButtons2[1]->Slot25(0, 0);
      this->SendMsg(nullptr, 6, 0x1B0, 0, 0, 0);
      break;
    case 0x1B4:
      this->m_slotButtons2[2]->Slot25(0, 0);
      this->SendMsg(nullptr, 6, 0x1B0, 0, 0, 0);
      break;
    }
    break;
  case 13:
    if (!this->m_unknown13C) {
      this->m_pageSlot = argument1;
    }
    break;
  case 0x12:
    if (this->m_unknown13C) {
      break;
    }
    if (g_windowMgr.unknown_0210 != nullptr) {
      g_windowMgr.CloseWindow(0x12);
    }
    if (g_windowMgr.HasModalWindow() != 1 && this->m_unknown118 != true) {
      const int previous = this->m_pageSlot;
      this->m_pageSlot -= 5;
      if (this->m_pageSlot < 0) {
        if (this->m_page <= 0) {
          this->m_pageSlot = previous;
          break;
        }
        this->SendMsg(nullptr, 6, 0x13C, 0, 0, 0);
        this->m_pageSlot += this->m_slotsPerPage;
      }
      for (int index = 0; index < this->m_slotsPerPage; ++index) {
        this->m_slotButtons2[index]->SendMsg(this, 13, index == this->m_pageSlot, 0, 0, 0);
      }
      g_selectedSlotIndex = static_cast<char>(this->m_page * this->m_slotsPerPage + this->m_pageSlot);
      this->sub_7D9310();
      this->SaveCurrentSlot();
    }
    break;
  case 0x13:
    if (this->m_unknown13C) {
      break;
    }
    if (g_windowMgr.unknown_0210 != nullptr) {
      g_windowMgr.CloseWindow(0x12);
    }
    if (g_windowMgr.HasModalWindow() != 1 && this->m_unknown118 != true) {
      const int previous = this->m_pageSlot;
      this->m_pageSlot = previous + 5;
      if (this->m_page * this->m_slotsPerPage + this->m_pageSlot >= this->m_maxCharacters) {
        this->m_pageSlot = previous;
        break;
      }
      if (this->m_pageSlot >= this->m_slotsPerPage) {
        if (this->m_page < this->m_pageCount - 1) {
          this->SendMsg(nullptr, 6, 0x13D, 0, 0, 0);
        }
        this->m_pageSlot %= this->m_slotsPerPage * this->m_page;
      }
      for (int index = 0; index < this->m_slotsPerPage; ++index) {
        this->m_slotButtons2[index]->SendMsg(this, 13, index == this->m_pageSlot, 0, 0, 0);
      }
      g_selectedSlotIndex = static_cast<char>(this->m_page * this->m_slotsPerPage + this->m_pageSlot);
      this->sub_7D9310();
      this->SaveCurrentSlot();
    }
    break;
  case 0x14:
    if (this->m_unknown13C) {
      break;
    }
    if (g_windowMgr.unknown_0210 != nullptr) {
      g_windowMgr.CloseWindow(0x12);
    }
    if (g_windowMgr.HasModalWindow() != 1 && this->m_unknown118 != true) {
      if (--this->m_pageSlot < 0) {
        if (this->m_page <= 0) {
          this->m_pageSlot = 0;
          break;
        }
        this->SendMsg(nullptr, 6, 0x13C, 0, 0, 0);
        this->m_pageSlot = this->m_slotsPerPage - 1;
      }
      for (int index = 0; index < this->m_slotsPerPage; ++index) {
        this->m_slotButtons2[index]->SendMsg(this, 13, index == this->m_pageSlot, 0, 0, 0);
      }
      g_selectedSlotIndex = static_cast<char>(this->m_page * this->m_slotsPerPage + this->m_pageSlot);
      this->sub_7D9310();
      this->SaveCurrentSlot();
    }
    break;
  case 0x15:
    if (this->m_unknown13C) {
      break;
    }
    if (g_windowMgr.unknown_0210 != nullptr) {
      g_windowMgr.CloseWindow(0x12);
    }
    if (g_windowMgr.HasModalWindow() != 1 && this->m_unknown118 != true) {
      const int previous = this->m_pageSlot;
      this->m_pageSlot = previous + 1;
      if (this->m_page * this->m_slotsPerPage + this->m_pageSlot >= this->m_maxCharacters) {
        this->m_pageSlot = previous;
        break;
      }
      if (this->m_pageSlot >= this->m_slotsPerPage) {
        if (this->m_page < this->m_pageCount - 1) {
          this->SendMsg(nullptr, 6, 0x13D, 0, 0, 0);
        }
        this->m_pageSlot %= this->m_slotsPerPage * this->m_page;
      }
      for (int index = 0; index < this->m_slotsPerPage; ++index) {
        this->m_slotButtons2[index]->SendMsg(this, 13, index == this->m_pageSlot, 0, 0, 0);
      }
      g_selectedSlotIndex = static_cast<char>(this->m_page * this->m_slotsPerPage + this->m_pageSlot);
      this->sub_7D9310();
      this->SaveCurrentSlot();
    }
    break;
  case 0x21:
    this->SendMsg(nullptr, 6, 0xD3, 0, 0, 0);
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x007D9090
// FUNCTION: fn_09595
void UINewSelectCharWnd::EnableButtonById(int id) {
  switch (id) {
  case 416:
    this->m_button416->Enable(1);
    break;
  case 417:
    this->m_button417->Enable(1);
    break;
  }
}

// OFFSET: 0x007D90E0
// FUNCTION: fn_09596
void UINewSelectCharWnd::sub_7D90E0() {
  const int slot = this->m_slotsPerPage * this->m_page + this->m_pageSlot;
  if (slot >= this->m_totalSlots) {
    return;
  }

  if (this->m_slotStatus[slot] == 1) {
    static_cast<UIButton*>(this->m_windowC0)->SetDisabled(1);
    this->m_windowC0->Invalidate();
    static_cast<UIButton*>(this->m_windowC4)->Enable(0);
    static_cast<UIButton*>(this->m_window100)->Enable(0);
    return;
  }

  const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, slot, 0, 0, 0));
  if (!info) {
    return;
  }

  if (info->deleteDate > 0) {
    static_cast<UIButton*>(this->m_windowC0)->SetDisabled(1);
    this->m_windowC0->Invalidate();
    static_cast<UIButton*>(this->m_window100)->Enable(1);
    static_cast<UIButton*>(this->m_windowC4)->Enable(1);
    Timestamp deleteDate(info->deleteDate);
    static_cast<UIButton*>(this->m_window100)->Enable(deleteDate.IsBeforeNow() == 1 ? 1 : 0);
  } else {
    static_cast<UIButton*>(this->m_windowC0)->SetDisabled(0);
    this->m_windowC0->Invalidate();
    static_cast<UIButton*>(this->m_window100)->Enable(0);
    static_cast<UIButton*>(this->m_windowC4)->Enable(0);
  }
}

// OFFSET: 0x007D9260
// FUNCTION: fn_09597
void UINewSelectCharWnd::sub_7D9260() {
  for (int index = 0; index < this->m_slotsPerPage; ++index) {
    const int slot = this->m_page * this->m_slotsPerPage + index;
    if (slot >= this->m_totalSlots) {
      continue;
    }

    static_cast<UISlotBmpButton*>(this->m_slotButtons2[index])->SetSlot(this->m_slotStatus[slot] ? -1 : slot, slot);
    if (slot < g_producibleSlotCount && this->m_slotStatus[slot] == 1) {
      this->m_slotButtons2[index]->SendMsg(nullptr, 431, slot >= g_validSlotCount ? 1 : 0, 0, 0, 0);
    } else {
      this->m_slotButtons2[index]->SendMsg(nullptr, 431, 1, 0, 0, 0);
    }
  }

  this->InvalidateChildren();
}

// OFFSET: 0x007D9310
// FUNCTION: fn_09598
void UINewSelectCharWnd::sub_7D9310() {
  const int current = this->m_page * this->m_slotsPerPage + this->m_pageSlot;
  if (current >= this->m_totalSlots) {
    return;
  }

  const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, current, 0, 0, 0));
  if (info) {
    static_cast<UIStaticText*>(this->m_slotButtons[0])->SetTextFitted(g_session.GetJobName(info->job, info->sex));
    static_cast<UIStaticText*>(this->m_slotButtons[1])->SetTextFitted(FormatOwnedString("%d", info->level).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[2])->SetTextFitted(FormatOwnedString("%lld", info->exp).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[3])->SetTextFitted(FormatOwnedString("%lld", info->hp).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[4])->SetTextFitted(FormatOwnedString("%lld", info->sp).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[5])->SetTextFitted(FormatOwnedString("%d", info->stats[0]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[6])->SetTextFitted(FormatOwnedString("%d", info->stats[1]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[7])->SetTextFitted(FormatOwnedString("%d", info->stats[2]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[8])->SetTextFitted(FormatOwnedString("%d", info->stats[3]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[9])->SetTextFitted(FormatOwnedString("%d", info->stats[4]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[10])->SetTextFitted(FormatOwnedString("%d", info->stats[5]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[11])->SetTextFitted(g_session.GetMapDisplayName(info->mapName));
  } else {
    for (int index = 0; index < 12; ++index) {
      static_cast<UIStaticText*>(this->m_slotButtons[index])->SetText("", 0);
    }
  }

  auto* button = static_cast<UIButton*>(this->m_windowFC);
  if (this->m_slotStatus[current] == 1) {
    button->SetDisabled(1);
  } else {
    button->SetDisabled(0);
  }
  button->Invalidate();

  this->sub_7D9260();
  this->sub_7D90E0();
}

// OFFSET: 0x007D97B0
// FUNCTION: fn_09599
void UINewSelectCharWnd::SaveCurrentSlot() {
  HKEY key;
  DWORD disposition = 0;
  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
    DWORD slot = this->m_slotsPerPage * this->m_page + this->m_pageSlot;
    RegSetValueExA(key, "CURSLOT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&slot), sizeof(slot));
    RegCloseKey(key);
  }
}
