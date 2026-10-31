#include "UI/Window/UISerachStoreInfoWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

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
  /* 0000 */ unsigned char unknown_0000[0x60];
  /* 0060 */ ITEM_INFO item;
};

static_assert(sizeof(SearchStoreResult) == sizeof(SessionEntry5BD4));

// OFFSET: 0x009121F0
// FUNCTION: fn_12527
UISerachStoreInfoWnd::UISerachStoreInfoWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0), unknown_00DC(1), unknown_00E0(-1),
      unknown_00E4(0), unknown_00E8(25), unknown_00EC(0), unknown_00ED(1) {
  this->unknown_00F0.clear();
}

// OFFSET: 0x0091BBC0
// FUNCTION: fn_12608
ITEM_INFO UISerachStoreInfoWnd::ItemAt(int x, int y) {
  ITEM_INFO item(0);
  this->unknown_00E0 = -1;
  if (g_session.unknown_5BD4.empty()) {
    return item;
  }

  if (x > 5 && x < this->geometry.width - 10 && y > 100 && y < (this->unknown_00E8 + 10) * 10) {
    const int row = (y - 100) / this->unknown_00E8;
    this->unknown_00E0 = row;
    const unsigned int index = row + (this->unknown_00DC - 1) * 10;
    if (index >= g_session.unknown_5BD4.size()) {
      return item;
    }

    std::vector<SessionEntry5BD4>::iterator it = g_session.unknown_5BD4.begin() + index;
    if (it != g_session.unknown_5BD4.end()) {
      item = reinterpret_cast<const SearchStoreResult&>(*it).item;
    }
  }
  return item;
}

// OFFSET: 0x009296E0
// FUNCTION: fn_12653
void UISerachStoreInfoWnd::Slot15(int width, int height) {
  UIRadioBtn** radio = this->m_radios;
  for (int x = 10; x < 90; x += 40) {
    *radio = new UIRadioBtn(*this->m_context, *this->m_controls);
    (*radio)->SetSize(this->geometry.width / 2, 12);
    (*radio)->Move(x, 40);
    this->AddChild(*radio);
    ++radio;
  }
  this->m_radios[0]->SetText(LookupMessage(0x6F2));
  this->m_radios[1]->SetText(LookupMessage(0x6F3));
  this->m_radios[this->unknown_00E4]->SendMsg(this, 0xD, 1, 0, 0, 0);

  this->m_edits[1] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[1]->SetSize(0x4C, 0x10);
  this->m_edits[1]->Move(100, 0x24);
  this->m_edits[1]->maximum = 0x32;
  this->m_edits[1]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[1]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[1]->flat = 0;
  this->AddChild(this->m_edits[1]);

  this->m_edits[0] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[0]->SetSize(0x4C, 0x10);
  this->m_edits[0]->Move(0xBB, 0x24);
  this->m_edits[0]->maximum = 0x32;
  this->m_edits[0]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[0]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[0]->flat = 0;
  UIEditCtrl* edit = this->m_edits[0];
  if (const char* tooltip = LookupMessage(0x709)) {
    edit->tooltip = tooltip;
  }
  this->m_edits[0]->disabled = 1;
  this->m_edits[0]->Invalidate();
  this->m_edits[0]->invalidatesParent = 1;
  this->AddChild(this->m_edits[0]);

  this->m_edits[2] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[2]->SetSize(0x4C, 0x10);
  this->m_edits[2]->Move(0x113, 0x24);
  this->m_edits[2]->maximum = 0x32;
  this->m_edits[2]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[2]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[2]->flat = 0;
  this->m_edits[2]->disabled = 1;
  this->m_edits[2]->Invalidate();
  this->AddChild(this->m_edits[2]);

  this->m_edits[3] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[3]->SetSize(0x4C, 0x10);
  this->m_edits[3]->Move(0x16C, 0x24);
  this->m_edits[3]->maximum = 0x32;
  this->m_edits[3]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[3]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[3]->flat = 0;
  this->m_edits[3]->disabled = 1;
  this->m_edits[3]->Invalidate();
  this->AddChild(this->m_edits[3]);

  UICheckBox* check = new UICheckBox(*this->m_context, *this->m_controls);
  check->SetSize(0xA0, 0xC);
  check->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(0x6FC));
  check->Move(100, 0x3C);
  check->SetChecked(this->unknown_00ED);
  this->AddChild(check);

  {
    std::string path = "유저인터페이스\\";
    this->m_findButton = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_findButton->SetBitmap((path + "btn_find" + ".bmp").c_str(), 0, 0);
    this->m_findButton->SetBitmap((path + "btn_find_a" + ".bmp").c_str(), 1, 0);
    this->m_findButton->SetBitmap((path + "btn_find_b" + ".bmp").c_str(), 2, 0);
    this->m_findButton->SetSize(this->m_findButton->BitmapWidth(), this->m_findButton->BitmapHeight());
    this->m_findButton->Move(this->geometry.width - 0x2D, 0x38);
    this->m_findButton->SetId(0x16D);
    this->AddChild(this->m_findButton);
  }

  this->m_prevButton = new UITextButton(*this->m_context, *this->m_controls);
  this->m_prevButton->Configure(LookupMessage(0x41D), 0x8E8E8E, 0, 0xC, 0, 0);
  this->m_prevButton->m_hoverColor = 0xEFB700;
  this->m_prevButton->Move(
      this->geometry.width / 2 - this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0x41D), 0, 0, 0xC, 0, 0) - 0x14,
      this->geometry.height - 0x15);
  this->m_prevButton->SetTooltip(LookupMessage(0x41D));
  this->m_prevButton->SetId(0x13C);
  this->AddChild(this->m_prevButton);

  this->m_nextButton = new UITextButton(*this->m_context, *this->m_controls);
  this->m_nextButton->Configure(LookupMessage(0x41E), 0x8E8E8E, 0, 0xC, 0, 0);
  this->m_nextButton->m_hoverColor = 0xEFB700;
  this->m_nextButton->Move(this->geometry.width / 2 + 0x14, this->geometry.height - 0x15);
  this->m_nextButton->SetTooltip(LookupMessage(0x41E));
  this->m_nextButton->SetId(0x13D);
  this->AddChild(this->m_nextButton);

  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "", 0);
  this->SendMsg(this, 0x3C, 0, 0, 0, 0);
  InputFocus{g_windowMgr.hoverWindow}.Set(this->m_edits[1]);
  this->m_message0 = 0x16D;
}

// OFFSET: 0x009331F0
// FUNCTION: fn_12723
void UISerachStoreInfoWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.itemId.size() != 0 && ((x > 160 && x < 330) || static_cast<unsigned int>(x - 6) <= 43)) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 0x18, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0093A9F0
// FUNCTION: fn_12759
int UISerachStoreInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 201: {
      SearchStoreRequestHeader packet;
      packet.packetType = 0x83B;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      g_session.unknown_5BE8 = -1;
      g_session.unknown_5BEC = -1;
      g_session.unknown_5BF0 = 0xFFFFFFFF;
      g_session.unknown_5BD4.clear();
      g_windowMgr.CloseWindow(181);
      break;
    }
    case 213:
      this->unknown_00ED = argument2 != 0;
      break;
    case 215:
      for (int index = 0; index < 2; ++index) {
        this->m_radios[index]->Invalidate();
        int checked;
        if (sender == this->m_radios[index]) {
          this->unknown_00E4 = index;
          checked = 1;
        } else {
          checked = 0;
        }
        this->m_radios[index]->SendMsg(this, 13, checked, 0, 0, 0);
      }
      break;
    case 365: {
      this->unknown_00EC = 1;
      this->m_findButton->SetDisabled(1);
      this->m_findButton->Invalidate();
      std::string cardName = this->m_edits[0]->Text();
      std::string itemName = this->m_edits[1]->Text();
      if (itemName.size() == 0) {
        this->unknown_00EC = 0;
        this->unknown_00F0 = LookupMessage(1809);
      }
      if (this->unknown_00EC != 0) {
        std::list<int> itemIds;
        std::list<int> cardIds;
        itemIds = SearchItemIds(itemName, 0, this->unknown_00ED);
        if (itemIds.size() == 0) {
          this->unknown_00EC = 0;
          this->unknown_00F0 = LookupMessage(1799);
        } else if (cardName.size() != 0) {
          cardIds = SearchItemIds(cardName, 1, this->unknown_00ED);
          if (cardIds.size() == 0) {
            this->unknown_00EC = 0;
            this->unknown_00F0 = LookupMessage(1812);
          }
        }

        const int count = static_cast<int>(itemIds.size() + cardIds.size());
        if (this->unknown_00EC != 0) {
          if (count > 20) {
            this->SendMsg(this, 60, 1, 1, 0, 0);
            return 0;
          }

          const int minPrice = std::atoi(this->m_edits[2]->Text());
          const int maxPrice = std::atoi(this->m_edits[3]->Text());
          const int maxLength = static_cast<int>(std::strlen(this->m_edits[3]->Text()));
          if (minPrice > maxPrice && maxLength > 0) {
            this->unknown_00EC = 0;
          }
          if (std::strlen(this->m_edits[2]->Text()) != 0) {
            std::string text = FormatOwnedString("%d", minPrice);
            if (text != this->m_edits[2]->Text()) {
              this->unknown_00EC = 0;
            }
          }
          if (maxLength > 0) {
            std::string text = FormatOwnedString("%d", maxPrice);
            if (text != this->m_edits[3]->Text()) {
              this->unknown_00EC = 0;
            }
          }
          if (this->unknown_00EC == 0) {
            this->unknown_00F0 = LookupMessage(1804);
          }
          if (this->unknown_00EC != 0) {
            const short length = static_cast<short>(count * 4 + 15);
            const int maximum = std::atoi(this->m_edits[3]->Text());
            const int minimum = std::atoi(this->m_edits[2]->Text());
            const unsigned char searchType = static_cast<unsigned char>(this->unknown_00E4);
            if (length > 0x800) {
              this->unknown_00EC = 0;
            } else if (this->unknown_00EC != 0) {
              SearchStoreQueryPacket packet;
              packet.maxPrice = maximum;
              packet.packetType = 0x835;
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
      if (this->unknown_00EC == 0) {
        this->m_findButton->SetDisabled(0);
        this->m_findButton->Invalidate();
        this->SendMsg(this, 60, 0, 0, 0, 0);
      }
      break;
    }
    }
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);

  case 60:
    if (argument1 != 0) {
      this->unknown_00F0 = "";
      if (argument2 == 387) {
        this->unknown_00DC = 1;
      } else if (argument2 < 387) {
        switch (argument2) {
        case 0:
          this->unknown_00DC = 1;
          g_session.unknown_5BD4.clear();
          reinterpret_cast<unsigned char*>(&g_session.unknown_5BE4)[1] = 0;
          this->unknown_00F0 = LookupMessage(1803);
          --g_session.unknown_5BE0;
          break;
        case 1:
          this->unknown_00F0 = LookupMessage(1784);
          break;
        case 2:
          this->unknown_00F0 = LookupMessage(1798);
          break;
        case 3:
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1800), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        case 4:
          g_session.unknown_5BE8 = -1;
          g_session.unknown_5BEC = -1;
          g_session.unknown_5BF0 = 0xFFFFFFFF;
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1797), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }
      }
      this->unknown_00EC = 0;
      this->m_findButton->SetDisabled(0);
      this->m_findButton->Invalidate();
    } else if (g_session.unknown_5BD4.empty() && this->unknown_00EC != 0) {
      this->unknown_00F0 = LookupMessage(1102);
    }
    this->m_prevButton->Enable(this->unknown_00DC != 1);
    this->m_nextButton->Enable(reinterpret_cast<unsigned char*>(&g_session.unknown_5BE4)[1]);
    if (static_cast<unsigned int>(this->unknown_00DC * 10) < g_session.unknown_5BD4.size()) {
      this->m_nextButton->Enable(1);
    }
    this->Invalidate();
    return 0;

  case 98:
    if (argument1 == 316) {
      if (this->unknown_00DC > 1) {
        this->unknown_00E0 = -1;
        --this->unknown_00DC;
        this->SendMsg(this, 60, 0, 0, 0, 0);
        return 0;
      }
    } else if (argument1 == 317) {
      const int page = this->unknown_00DC;
      this->unknown_00E0 = -1;
      if (static_cast<unsigned int>(page * 10) < g_session.unknown_5BD4.size()) {
        this->unknown_00DC = page + 1;
        this->SendMsg(this, 60, 0, 0, 0, 0);
        return 0;
      }
      if (reinterpret_cast<unsigned char*>(&g_session.unknown_5BE4)[1] != 0) {
        this->unknown_00DC = page + 1;
        SearchStoreRequestHeader packet;
        packet.packetType = 0x838;
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        return 0;
      }
    }
    return 0;
  }
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
