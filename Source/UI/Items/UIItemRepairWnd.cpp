#include "UI/Items/UIItemRepairWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/PlayerNameDictionary.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/MessageTable.h"
#include "UI/Items/UIMakeTargetListWnd.h"
#include "UI/Items/UIMakeTargetProcessWnd.h"
#include "UI/Items/UIMakeTargetResultWnd.h"
#include "UI/Items/UIWeaponRefineWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIListBox.h"

#include <cstdint>
#include <cstring>
#include <string>

// OFFSET: 0x009A3860
// FUNCTION: fn_13609
void UIItemRepairWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(812), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x009A0B00
// FUNCTION: fn_13604
void UIItemRepairWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(12, 22, width - 24, height - 55, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
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

// OFFSET: 0x009A1590
// FUNCTION: fn_13605
void UIMakeTargetListWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(12, 22, width - 24, height - 55, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
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

// OFFSET: 0x009A2020
// FUNCTION: fn_13606
void UIMakeTargetProcessWnd::Slot15(int width, int height) {
  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
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

// OFFSET: 0x009A29B0
// FUNCTION: fn_13607
void UIMakeTargetResultWnd::Slot15(int width, int height) {
  const std::string prefix("유저인터페이스\\");

  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 91, height - 24);
  button->SetId(184);
  this->AddChild(button);

  this->m_message0 = 184;
}

// OFFSET: 0x009A2DD0
// FUNCTION: fn_13608
void UIWeaponRefineWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(12, 22, width - 24, height - 55, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
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

// OFFSET: 0x009A3DE0
// FUNCTION: fn_13615
int UIItemRepairWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  ITEM_INFO item;

  switch (message) {
  case 6: {
    const int response = static_cast<int>(a);
    this->m_recorder->Record(this->geometry.id, response, b, c, this->m_list->selected);

    if (response == 184) {
      if (g_replayAnimationScaling) {
        this->m_list->selected = static_cast<int>(d);
        this->m_list->Invalidate();
      }

      UIListBox* list = this->m_list;
      if (list->LineCount() == 0) {
        g_modeMgr.gameMode->SendMsg(174, -1, 0, 0, 0);
        this->m_frameContext->windows.CloseWindow(107);
        this->m_frameContext->windows.CloseWindow(107);
        return 0;
      }

      const int selected = list->selected;
      if (selected >= 0) {
        for (UIItemRepairEntry& entry : this->entries) {
          if (entry.index == this->indices[selected]) {
            g_modeMgr.gameMode->SendMsg(174, reinterpret_cast<int>(&entry), 0, 0, 0);
            break;
          }
        }

        this->m_frameContext->windows.CloseWindow(107);
        return 0;
      }
    } else if (response == 185) {
      g_modeMgr.gameMode->SendMsg(174, -1, 0, 0, 0);
      this->m_frameContext->windows.CloseWindow(107);
      return 0;
    }

    return 0;
  }

  case 18:
    this->m_list->SendMsg(nullptr, 18, 0, 0, 0, 0);
    return 0;

  case 19:
    this->m_list->SendMsg(nullptr, 19, 0, 0, 0, 0);
    return 0;

  case 31: {
    const UIItemRepairEntry& entry = *reinterpret_cast<const UIItemRepairEntry*>(a);
    this->entries.push_back(entry);
    this->itemIds.push_back(entry.itemId);
    this->indices.push_back(entry.index);

    item.SetItemId(entry.itemId);
    item.unknown_0060 = 0;
    item.identified = 1;
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    this->m_list->AddLine(naming.DisplayName(item, 0).c_str());
    return 0;
  }

  case 123: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    std::uint16_t marker;
    std::memcpy(&marker, bytes, sizeof(marker));
    unsigned offset = 6;

    if (marker == 22150) {
      while (static_cast<unsigned>(b) > offset) {
        std::int16_t kind;
        std::uint32_t size;
        std::memcpy(&kind, bytes + offset, sizeof(kind));
        std::memcpy(&size, bytes + offset + 2, sizeof(size));
        offset += 6;

        if (kind == 22151) {
          break;
        }

        if (kind == 22152) {
          UIItemRepairEntry entry;
          std::memcpy(&entry, bytes + offset, size);
          offset += size;
          this->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(&entry), 0, 0, 0);
        } else {
          offset += size;
        }
      }
    }

    return 0;
  }
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009A4C50
// FUNCTION: fn_13618
int UIMakeTargetResultWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    this->m_frameContext->windows.CloseWindow(81);
    return 0;
  }

  if (message != 40) {
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->makeType = static_cast<int>(a);
  if (b) {
    this->succeeded = true;
  }

  this->Invalidate();
  return 0;
}
