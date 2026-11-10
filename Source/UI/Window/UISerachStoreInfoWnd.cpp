#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UISerachStoreInfoWnd.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/NoticeEnums.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>

#pragma pack(push, 1)
struct SearchStoreRequestHeader {
  /* 0000 */ std::int16_t packetType;
};

struct SearchStoreQueryPacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char searchType;
  /* 0005 */ int maxPrice;
  /* 0009 */ int minPrice;
  /* 000D */ unsigned char itemCount;
  /* 000E */ unsigned char cardCount;
  /* 000F */ int ids[508];
};
#pragma pack(pop)

static_assert(offsetof(SearchStoreQueryPacket, ids) == 0xF);

struct SearchStoreResult {
  /* 0000 */ int storeId;
  /* 0004 */ int accountId;
  /* 0008 */ unsigned char padding_0008[0x58];
  /* 0060 */ ITEM_INFO item;
};

static_assert(sizeof(SearchStoreResult) == sizeof(SessionEntry5BD4));

// OFFSET: 0x009121F0
// FUNCTION: fn_12527
UISerachStoreInfoWnd::UISerachStoreInfoWnd()
    : UIFrameWnd(0), m_page(1), m_hoverRow(-1),
      m_searchType(0), m_rowHeight(25), m_searchPending(0), m_matchOption(1), m_statusText("") {
}

// OFFSET: 0x0091BBC0
// FUNCTION: fn_12608
ITEM_INFO UISerachStoreInfoWnd::ItemAt(int x, int y) {
  ITEM_INFO item(0);
  this->m_hoverRow = -1;
  if (g_session.storeSearchResults.empty()) {
    return item;
  }

  if (x > 5 && x < this->geometry.width - 10 && y > 100 && y < (this->m_rowHeight + 10) * 10) {
    const int row = (y - 100) / this->m_rowHeight;
    this->m_hoverRow = row;
    const unsigned int index = row + (this->m_page - 1) * 10;
    if (index >= g_session.storeSearchResults.size()) {
      return item;
    }

    std::vector<SessionEntry5BD4>::iterator it = g_session.storeSearchResults.begin() + index;
    if (it != g_session.storeSearchResults.end()) {
      item = reinterpret_cast<const SearchStoreResult&>(*it).item;
    }
  }
  return item;
}

// OFFSET: 0x009296E0
// FUNCTION: fn_12653
void UISerachStoreInfoWnd::OnCreate(int width, int height) {
  UIRadioBtn** radio = this->m_radios;
  for (int x = 10; x < 90; x += 40) {
    *radio = new UIRadioBtn();
    (*radio)->SetSize(this->geometry.width / 2, 12);
    (*radio)->Move(x, 40);
    this->AddChild(*radio);
    ++radio;
  }
  this->m_radios[0]->SetText(LookupMessage(MSI_SELL));
  this->m_radios[1]->SetText(LookupMessage(MSI_BUY));
  this->m_radios[this->m_searchType]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);

  this->m_edits[1] = new UIEditCtrl();
  this->m_edits[1]->SetSize(0x4C, 0x10);
  this->m_edits[1]->Move(100, 0x24);
  this->m_edits[1]->maximum = 0x32;
  this->m_edits[1]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[1]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[1]->flat = 0;
  this->AddChild(this->m_edits[1]);

  this->m_edits[0] = new UIEditCtrl();
  this->m_edits[0]->SetSize(0x4C, 0x10);
  this->m_edits[0]->Move(0xBB, 0x24);
  this->m_edits[0]->maximum = 0x32;
  this->m_edits[0]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[0]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[0]->flat = 0;
  UIEditCtrl* edit = this->m_edits[0];
  if (const char* tooltip = LookupMessage(MSI_SSI_TOOLTIP_OF_SERACHING_THE_CARD)) {
    edit->tooltip = tooltip;
  }
  edit = this->m_edits[0];
  edit->disabled = 1;
  edit->Invalidate();
  this->m_edits[0]->invalidatesParent = 1;
  this->AddChild(this->m_edits[0]);

  this->m_edits[2] = new UIEditCtrl();
  this->m_edits[2]->SetSize(0x4C, 0x10);
  this->m_edits[2]->Move(0x113, 0x24);
  this->m_edits[2]->maximum = 0x32;
  this->m_edits[2]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[2]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[2]->flat = 0;
  edit = this->m_edits[2];
  edit->disabled = 1;
  edit->Invalidate();
  this->AddChild(this->m_edits[2]);

  this->m_edits[3] = new UIEditCtrl();
  this->m_edits[3]->SetSize(0x4C, 0x10);
  this->m_edits[3]->Move(0x16C, 0x24);
  this->m_edits[3]->maximum = 0x32;
  this->m_edits[3]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[3]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[3]->flat = 0;
  edit = this->m_edits[3];
  edit->disabled = 1;
  edit->Invalidate();
  this->AddChild(this->m_edits[3]);

  UICheckBox* check = new UICheckBox();
  check->SetSize(0xA0, 0xC);
  check->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_SEARCH_INCLUDED));
  check->Move(100, 0x3C);
  check->SetChecked(this->m_matchOption);
  this->AddChild(check);

  {
    std::string path = "유저인터페이스\\";
    this->m_findButton = new UIBitmapButton();
    this->m_findButton->SetBitmap((path + "btn_find" + ".bmp").c_str(), 0, 0);
    this->m_findButton->SetBitmap((path + "btn_find_a" + ".bmp").c_str(), 1, 0);
    this->m_findButton->SetBitmap((path + "btn_find_b" + ".bmp").c_str(), 2, 0);
    this->m_findButton->SetSize(this->m_findButton->BitmapWidth(), this->m_findButton->BitmapHeight());
    this->m_findButton->Move(this->geometry.width - 0x2D, 0x38);
    this->m_findButton->SetId(0x16D);
    this->AddChild(this->m_findButton);
  }

  this->m_prevButton = new UITextButton();
  this->m_prevButton->Configure(LookupMessage(MSI_PREV_PAGE), 0x8E8E8E, 0, 0xC, 0, 0);
  this->m_prevButton->m_hoverColor = 0xEFB700;
  this->m_prevButton->Move(
      this->geometry.width / 2 - this->TextWidth(LookupMessage(MSI_PREV_PAGE), 0, 0, 0xC, 0, 0) - 0x14,
      this->geometry.height - 0x15);
  this->m_prevButton->SetTooltip(LookupMessage(MSI_PREV_PAGE));
  this->m_prevButton->SetId(0x13C);
  this->AddChild(this->m_prevButton);

  this->m_nextButton = new UITextButton();
  this->m_nextButton->Configure(LookupMessage(MSI_NEXT_PAGE), 0x8E8E8E, 0, 0xC, 0, 0);
  this->m_nextButton->m_hoverColor = 0xEFB700;
  this->m_nextButton->Move(this->geometry.width / 2 + 0x14, this->geometry.height - 0x15);
  this->m_nextButton->SetTooltip(LookupMessage(MSI_NEXT_PAGE));
  this->m_nextButton->SetId(0x13D);
  this->AddChild(this->m_nextButton);

  this->AddBaseButton();
  this->AddCloseButton("", 0);
  this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
  g_windowMgr.SetFocus(this->m_edits[1]);
  this->m_message0 = 0x16D;
}

// OFFSET: 0x009331F0
// FUNCTION: fn_12723
void UISerachStoreInfoWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.itemId.size() != 0 && ((x > 160 && x < 330) || static_cast<unsigned int>(x - 6) <= 43)) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0093A9F0
// FUNCTION: fn_12759
int UISerachStoreInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case ID_CLOSE: {
      SearchStoreRequestHeader packet;
      packet.packetType = ID_CZ_CLOSE_SEARCH_STORE_INFO;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      g_session.storeSearchX = -1;
      g_session.storeSearchY = -1;
      g_session.storeSearchOwnerAid = 0xFFFFFFFF;
      g_session.storeSearchResults.clear();
      g_windowMgr.CloseWindow(WID_SEARCH_STORE_INFO);
      break;
    }
    case ID_CHECK:
      this->m_matchOption = argument2 != 0;
      break;
    case ID_RADIOBTN:
      for (int index = 0; index < 2; ++index) {
        this->m_radios[index]->Invalidate();
        int checked;
        if (sender == this->m_radios[index]) {
          this->m_searchType = index;
          checked = 1;
        } else {
          checked = 0;
        }
        this->m_radios[index]->SendMsg(this, UM_SETSTATE, checked, 0, 0, 0);
      }
      break;
    case 365: {
      this->m_searchPending = 1;
      this->m_findButton->SetDisabled(1);
      this->m_findButton->Invalidate();
      std::string cardName = this->m_edits[0]->Text();
      std::string itemName = this->m_edits[1]->Text();
      if (itemName.size() == 0) {
        this->m_searchPending = 0;
        this->m_statusText = LookupMessage(MSI_SSI_FAILED_ENTER_THE_NAME_OF_ITEM);
      }
      if (this->m_searchPending != 0) {
        std::list<int> itemIds;
        std::list<int> cardIds;
        itemIds = SearchItemIds(itemName, 0, this->m_matchOption);
        if (itemIds.size() == 0) {
          this->m_searchPending = 0;
          this->m_statusText = LookupMessage(MSI_SSI_FAILED_ITEM_DOES_NOT_EXIST);
        } else if (cardName.size() != 0) {
          cardIds = SearchItemIds(cardName, 1, this->m_matchOption);
          if (cardIds.size() == 0) {
            this->m_searchPending = 0;
            this->m_statusText = LookupMessage(MSI_SSI_FAILED_NAME_OF_CARD_DOES_NOT_EXIST);
          }
        }

        const int count = static_cast<int>(itemIds.size() + cardIds.size());
        if (this->m_searchPending != 0) {
          if (count > 20) {
            this->SendMsg(this, UM_REFRESH, 1, 1, 0, 0);
            return 0;
          }

          const int minPrice = std::atoi(this->m_edits[2]->Text());
          const int maxPrice = std::atoi(this->m_edits[3]->Text());
          const int maxLength = static_cast<int>(std::strlen(this->m_edits[3]->Text()));
          if (minPrice > maxPrice && maxLength > 0) {
            this->m_searchPending = 0;
          }
          if (std::strlen(this->m_edits[2]->Text()) != 0) {
            std::string text = FormatOwnedString("%d", minPrice);
            if (text != this->m_edits[2]->Text()) {
              this->m_searchPending = 0;
            }
          }
          if (maxLength > 0) {
            std::string text = FormatOwnedString("%d", maxPrice);
            if (text != this->m_edits[3]->Text()) {
              this->m_searchPending = 0;
            }
          }
          if (this->m_searchPending == 0) {
            this->m_statusText = LookupMessage(MSI_SSI_FAILED_PRICE_ISNOT_VALID);
          }
          if (this->m_searchPending != 0) {
            const short length = static_cast<short>(count * 4 + 15);
            const int maximum = std::atoi(this->m_edits[3]->Text());
            const int minimum = std::atoi(this->m_edits[2]->Text());
            const unsigned char searchType = static_cast<unsigned char>(this->m_searchType);
            if (length > 0x800) {
              this->m_searchPending = 0;
            } else if (this->m_searchPending != 0) {
              SearchStoreQueryPacket packet;
              packet.maxPrice = maximum;
              packet.packetType = ID_CZ_SEARCH_STORE_INFO;
              packet.packetLength = length;
              packet.searchType = searchType;
              packet.minPrice = minimum;
              packet.itemCount = static_cast<unsigned char>(itemIds.size());
              packet.cardCount = static_cast<unsigned char>(cardIds.size());
              int* id = packet.ids;
              for (std::list<int>::iterator it = itemIds.begin(); it != itemIds.end(); ++it) {
                *id++ = *it;
              }
              id = packet.ids + itemIds.size();
              for (std::list<int>::iterator it = cardIds.begin(); it != cardIds.end(); ++it) {
                *id++ = *it;
              }
              GetRagConnection().Send(static_cast<short>(count * 4 + 15), &packet);
            }
          }
        }
      }
      if (this->m_searchPending == 0) {
        this->m_findButton->SetDisabled(0);
        this->m_findButton->Invalidate();
        this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
      }
      break;
    }
    }
    break;

  case UM_REFRESH:
    if (argument1 != 0) {
      this->m_statusText = "";
      switch (argument2) {
      case 387:
        this->m_page = 1;
        break;
      case SSI_FAILED_NOTHING_SEARCH_ITEM:
        this->m_page = 1;
        g_session.storeSearchResults.clear();
        reinterpret_cast<unsigned char*>(&g_session.storeSearchPageFlags)[1] = 0;
        this->m_statusText = LookupMessage(MSI_SSI_FAILED_SEARCH_STORE);
        --g_session.storeSearchCount;
        break;
      case SSI_FAILED_OVER_MAXCOUNT:
        this->m_statusText = LookupMessage(MSI_SSI_FAILED_OVER_MAXCOUNT);
        break;
      case SSI_FAILED_SEARCH_CNT:
        this->m_statusText = LookupMessage(MSI_SSI_FAILED_SEARCH_CNT);
        break;
      case SSI_FAILED_LIMIT_SEARCH_TIME:
        g_windowMgr.Confirm(LookupMessage(MSI_SSI_FAILED_LIMIT_SERACH_TIME), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      case SSI_FAILED_SSILIST_CLICK_TO_OPEN_STORE:
        g_session.storeSearchX = -1;
        g_session.storeSearchY = -1;
        g_session.storeSearchOwnerAid = 0xFFFFFFFF;
        g_windowMgr.Confirm(LookupMessage(MSI_FAIL_OPEN_STORE), 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
      this->m_searchPending = 0;
      this->m_findButton->SetDisabled(0);
      this->m_findButton->Invalidate();
    } else if (g_session.storeSearchResults.empty() && this->m_searchPending != 0) {
      this->m_statusText = LookupMessage(MSI_SEARCHING_ITEM);
    }
    this->m_prevButton->Enable(this->m_page != 1);
    this->m_nextButton->Enable(reinterpret_cast<unsigned char*>(&g_session.storeSearchPageFlags)[1]);
    if (static_cast<unsigned int>(this->m_page * 10) < g_session.storeSearchResults.size()) {
      this->m_nextButton->Enable(1);
    }
    this->Invalidate();
    return 0;

  case UM_TEXT_LBTN_UP:
    switch (argument1) {
    case 316:
      if (this->m_page > 1) {
        this->m_hoverRow = -1;
        --this->m_page;
        this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
        return 0;
      }
      break;
    case 317: {
      const int page = this->m_page;
      this->m_hoverRow = -1;
      if (static_cast<unsigned int>(page * 10) < g_session.storeSearchResults.size()) {
        this->m_page = page + 1;
        this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
        return 0;
      }
      if (reinterpret_cast<unsigned char*>(&g_session.storeSearchPageFlags)[1] != 0) {
        this->m_page = page + 1;
        SearchStoreRequestHeader packet;
        packet.packetType = ID_CZ_SEARCH_STORE_INFO_NEXT_PAGE;
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        return 0;
      }
      break;
    }
    }
    return 0;
  }
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

#pragma pack(push, 1)
struct SearchStoreClickPacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ int accountId;
  /* 0006 */ int storeId;
  /* 000A */ int itemId;
};
#pragma pack(pop)

// OFFSET: 0x00930780
// FUNCTION: fn_12688
void UISerachStoreInfoWnd::OnLBtnDblClk(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.itemId.size() == 0 || static_cast<unsigned int>(x - 0x33) > 0x6C) {
    return;
  }

  const unsigned int index = this->m_hoverRow + (this->m_page - 1) * 10;
  std::vector<SessionEntry5BD4>::iterator it = g_session.storeSearchResults.begin() + index;
  if (it == g_session.storeSearchResults.end()) {
    return;
  }

  if (reinterpret_cast<std::int16_t*>(&g_session.storeSearchPageFlags)[1] != 0 &&
      g_windowMgr.Confirm(LookupMessage(MSI_OPEN_SEARCH_STORE), 2, true, false, 0, "Message", 0x118, 0x78, 0) != 0xBB) {
    return;
  }

  const SearchStoreResult* entry = reinterpret_cast<const SearchStoreResult*>(&*it);

  g_session.storeSearchX = -1;
  g_session.storeSearchY = -1;
  g_session.storeSearchOwnerAid = 0xFFFFFFFF;

  SearchStoreClickPacket packet;
  packet.packetType = ID_CZ_SSILIST_ITEM_CLICK;
  packet.accountId = entry->accountId;
  packet.storeId = entry->storeId;
  packet.itemId = entry->item.GetItemId();
  GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
  g_session.storeSearchOwnerAid = entry->accountId;
}
