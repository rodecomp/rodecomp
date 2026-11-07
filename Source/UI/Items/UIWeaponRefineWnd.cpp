#include "UI/Items/UIWeaponRefineWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdint>
#include <cstring>
#include <vector>

// OFFSET: 0x009A07C0
// FUNCTION: fn_13602
std::vector<unsigned char> UIWeaponRefineWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(22200, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  const int count = static_cast<int>(this->itemIds.size());
  for (int index = 0; index < count; ++index) {
    int values[2] = {this->itemIds[index], this->indices[index]};
    const auto record = BuildReplayTaggedRecord(22202, values, sizeof(values));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(22201, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x009A3CD0
// FUNCTION: fn_13613
void UIWeaponRefineWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(910), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

#pragma pack(push, 1)
struct UIWeaponRefineRecordHeader {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t bytes;

  // INLINED
  UIWeaponRefineRecordHeader() : id(0), bytes(0) {
  }

  // INLINED
  ~UIWeaponRefineRecordHeader() {
    this->bytes = 0;
    this->id = 0;
  }
};
#pragma pack(pop)

// OFFSET: 0x009A4CA0
// FUNCTION: fn_13619
int UIWeaponRefineWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  ITEM_INFO item;

  switch (message) {
  case 31: {
    const int itemId = static_cast<int>(a);
    this->itemIds.push_back(itemId);
    this->indices.push_back(b);

    item.SetItemId(itemId);
    item.unknown_0060 = 0;
    item.identified = 1;
    this->m_list->AddLine(item.DisplayName(0).c_str());
    return 0;
  }

  case 18:
    this->m_list->SendMsg(nullptr, 18, 0, 0, 0, 0);
    return 0;

  case 19:
    this->m_list->SendMsg(nullptr, 19, 0, 0, 0, 0);
    return 0;

  case 6: {
    const int response = static_cast<int>(a);
    this->RecordResponse(response, b, c, this->m_list->selected);

    switch (response) {
    case 184: {
      if (g_replayAnimationScaling) {
        this->m_list->selected = static_cast<int>(d);
        this->m_list->Invalidate();
      }

      UIListBox* list = this->m_list;
      if (list->LineCount() == 0) {
        g_modeMgr.gameMode->SendMsg(182, -1, 0, 0, 0);
        g_windowMgr.CloseWindow(111);
        g_windowMgr.CloseWindow(111);
        return 0;
      }

      const int selected = list->selected;
      if (selected >= 0) {
        g_modeMgr.gameMode->SendMsg(182, this->indices[selected], 0, 0, 0);
        g_windowMgr.CloseWindow(111);
        return 0;
      }
      break;
    }
    case 185:
      g_modeMgr.gameMode->SendMsg(182, -1, 0, 0, 0);
      g_windowMgr.CloseWindow(111);
      return 0;
    }

    return 0;
  }

  case 123: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    UIWeaponRefineRecordHeader header;
    unsigned offset = 6;
    header = *reinterpret_cast<const UIWeaponRefineRecordHeader*>(bytes);

    if (header.id == 22200) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const UIWeaponRefineRecordHeader*>(bytes + offset);
        offset += 6;

        switch (header.id) {
        case 22201:
          return 0;

        case 22202: {
          int values[2] = {};
          std::memcpy(values, bytes + offset, header.bytes);
          offset += header.bytes;
          this->SendMsg(nullptr, 31, values[0], values[1], 0, 0);
          break;
        }

        default:
          offset += header.bytes;
          break;
        }
      }
    }

    return 0;
  }
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
