#include "UI/Items/UIMakeTargetListWnd.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/WindowManagerWindows.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <cstring>
#include <vector>

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
  const ReplayTaggedRecordHeader header{id, bytes};
  std::memcpy(record.data(), &header, sizeof(header));
  if (bytes != 0) {
    std::memcpy(record.data() + sizeof(header), data, bytes);
  }
  return record;
}
}

// INLINED
static void AppendReplayRecord(std::vector<unsigned char>& records, const std::vector<unsigned char>& record) {
  records.insert(records.end(), record.begin(), record.end());
}

// OFFSET: 0x009A0490
// FUNCTION: fn_13600
std::vector<unsigned char> UIMakeTargetListWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, BuildReplayTaggedRecord(22050, nullptr, 0));
  for (auto it = this->itemIds.begin(); it != this->itemIds.end(); ++it) {
    AppendReplayRecord(records, MakeReplayTaggedRecord(22052, &*it, sizeof(*it)));
  }
  AppendReplayRecord(records, BuildReplayTaggedRecord(22051, nullptr, 0));
  return records;
}

// OFFSET: 0x009A38A0
// FUNCTION: fn_13610
void UIMakeTargetListWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_MAKE_LIST), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x009A42E0
// FUNCTION: fn_13616
int UIMakeTargetListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  int itemId = static_cast<int>(a);
  ITEM_INFO item;

  switch (message) {
  case UM_ADDITEM: {
    this->itemIds.push_back(itemId);

    item.SetItemId(itemId);
    item.refiningLevel = 0;
    item.identified = 1;
    this->m_list->AddLine(item.DisplayName(0).c_str());
    return 0;
  }

  case UM_UPPUSH:
    this->m_list->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
    return 0;

  case UM_DOWNPUSH:
    this->m_list->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    return 0;

  case UM_COMMAND: {
    const int response = static_cast<int>(a);
    this->RecordResponse(response, b, c, this->m_list->selected);

    switch (response) {
    case ID_OK: {
      if (g_replayAnimationScaling) {
        UIListBox* replayList = this->m_list;
        replayList->selected = static_cast<int>(d);
        replayList->Invalidate();
      }

      UIListBox* list = this->m_list;
      if (list->LineCount() == 0) {
        g_windowMgr.CloseWindow(WID_MAKETARGETLISTWND);
        g_windowMgr.CloseWindow(WID_MAKETARGETLISTWND);
        return 0;
      }

      if (list->selected >= 0) {
        switch (g_session.m_job) {
        case JT_RUNE_KNIGHT:
        case JT_RUNE_KNIGHT_H:
        case JT_RUNE_KNIGHT_B:
        case JT_DRAGON_KNIGHT: {
          {
            ITEM_INFO materials[3];
            g_modeMgr.GetGameMode()->SendMsg(MM_REQ_MAKING_ITEM, this->itemIds[this->m_list->selected], reinterpret_cast<int>(materials), 0, 0);
          }
          g_windowMgr.CloseWindow(WID_MAKETARGETLISTWND);
          return 0;
        }

        default: {
          UIWindow* window = g_windowMgr.EnsureWindow(WID_MAKETARGETPROCESSWND);
          window->SendMsg(this, UM_SETCURITEM, this->itemIds[this->m_list->selected], 0, 0, 0);
          window->Move(this->geometry.x, this->geometry.y);
          g_windowMgr.CloseWindow(WID_MAKETARGETLISTWND);
          return 0;
        }
        }
      }
      break;
    }
    case ID_CANCEL: {
      ITEM_INFO materials[3];
      g_modeMgr.GetGameMode()->SendMsg(MM_REQ_MAKING_ITEM, 0, reinterpret_cast<int>(materials), 0, 0);
      g_windowMgr.CloseWindow(WID_MAKETARGETLISTWND);
      return 0;
    }
    }

    return 0;
  }

  case UM_REPLAY_FRAMED_DATA: {
#pragma pack(push, 1)
    struct ReplayTag {
      /* 0000 */ std::int16_t id;
      /* 0002 */ std::uint32_t bytes;

      // INLINED
      ReplayTag() : id(0), bytes(0) {}

      // INLINED
      ~ReplayTag() {
        this->bytes = 0;
        this->id = 0;
      }
    };
#pragma pack(pop)

    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ReplayTag tag;
    std::memcpy(&tag, bytes, sizeof(tag));
    unsigned offset = sizeof(tag);

    if (tag.id == 22050) {
      while (static_cast<unsigned>(b) > offset) {
        std::memcpy(&tag, bytes + offset, sizeof(tag));
        offset += sizeof(tag);

        switch (tag.id) {
        case 22051:
          return 0;

        case 22052:
          itemId = 0;
          std::memcpy(&itemId, bytes + offset, tag.bytes);
          offset += tag.bytes;
          this->SendMsg(nullptr, UM_ADDITEM, itemId, 0, 0, 0);
          break;

        default:
          offset += tag.bytes;
          break;
        }
      }
    }

    return 0;
  }
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
