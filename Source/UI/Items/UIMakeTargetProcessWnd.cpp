#include "UI/Items/UIMakeTargetProcessWnd.h"

#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdint>
#include <cstring>
#include <vector>


// OFFSET: 0x009A0090
// FUNCTION: fn_13598
void UIMakeTargetProcessWnd::Slot10(int x, int y, std::intptr_t value) {
  switch (this->makeType) {
  case 994:
  case 995:
  case 996:
  case 997:
  case 998:
  case 999:
  case 1000:
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 1) {
    bool hasElementalStone = false;
    int emptySlot = 3;
    for (int index = 0; index < 3; ++index) {
      if (this->materials[index].unknown_0000[1] != 0) {
        hasElementalStone |= this->materials[index].IsElementalStone();
      } else if (emptySlot == 3) {
        emptySlot = index;
      }
    }

    if (emptySlot < 3) {
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, drag->id);
      if (item.unknown_0000[1] != 0 && item.IsForgeMaterial() && (!hasElementalStone || !item.IsElementalStone())) {
        this->materials[emptySlot] = item;
        this->materials[emptySlot].quantity = 1;
        g_session.RemoveItem1710(item.unknown_0000[1], 1);
        if (g_windowMgr.unknown_01D4) {
          reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01D4)->SendMsg(nullptr, 23, 0, 0, 0, 0);
        }
        this->Invalidate();
      }
    }
  }

  g_modeMgr.gameMode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x009A0690
// FUNCTION: fn_13601
std::vector<unsigned char> UIMakeTargetProcessWnd::Slot44() {
  std::vector<unsigned char> records;
  {
    const auto record = BuildReplayTaggedRecord(22000, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(22002, &this->makeType, sizeof(this->makeType));
    records.insert(records.end(), record.begin(), record.end());
  }
  {
    const auto record = BuildReplayTaggedRecord(22001, nullptr, 0);
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x009A09E0
// FUNCTION: fn_13603
ITEM_INFO UIMakeTargetProcessWnd::GetItemAt(int x, int y) {
  switch (this->makeType) {
  case 994:
  case 995:
  case 996:
  case 997:
  case 998:
  case 999:
  case 1000:
    return ITEM_INFO(0);
  }

  if (y > this->geometry.height - 56 && y < this->geometry.height - 32) {
    if (x >= 4 && x <= 34) {
      return this->materials[0];
    }
    if (x >= 36 && x <= 66) {
      return this->materials[1];
    }
    if (x >= 68 && x <= 98) {
      return this->materials[2];
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x009A38E0
// FUNCTION: fn_13611
void UIMakeTargetProcessWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(false, "", 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);

  switch (this->makeType) {
  case 994:
  case 995:
  case 996:
  case 997:
  case 998:
  case 999:
  case 1000:
    break;
  default: {
    const ITEM_INFO* material = this->materials;
    for (int x = 8; x < 104; x += 32, ++material) {
      auto* bar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\btnbar_right2.bmp")));
      DrawItemSlotBackground(*this, x - 4, this->geometry.height - bar->image.height - 32);
      if (material->unknown_0000[1] != 0) {
        DrawItemIcon(*this, ResourceMgr::Instance(), x, this->geometry.height - bar->image.height - 28, *material, 0);
      }
    }
    break;
  }
  }

  ITEM_INFO item;
  item.SetItemId(this->makeType);
  item.unknown_0060 = 0;
  item.identified = 1;
  const int nameWidth = this->DrawFittedItemName(16, 2, &item, 0, 0, 12, 0);
  this->DrawShadowedText(nameWidth + 16, 2, LookupMessage(426), 0, 0, 0xFFFFFF, 0, 12, 0);
  const int requiredWidth = this->DrawFittedItemName(16, 22, &item, 0, 0, 12, 0);
  this->DrawShadowedText(requiredWidth + 16, 22, LookupMessage(427), 0, 0, 0xFFFFFF, 0, 12, 0);

  const std::vector<const char*>& lines = FindMetalProcessItems(this->makeType);
  int y = 42;
  for (unsigned int index = 0; index < lines.size(); ++index, y += 20) {
    this->DrawTextAt(16, y, lines[index], 0, 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x009A3D10
// FUNCTION: fn_13614
void UIMakeTargetProcessWnd::Slot25(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.unknown_0000[1] != 0) {
    g_modeMgr.gameMode->SendMsg(134, reinterpret_cast<int>(item.itemId.c_str()), 0, 0, 0);
  } else {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x009A48C0
// FUNCTION: fn_13617
int UIMakeTargetProcessWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 40:
    this->makeType = static_cast<int>(a);
    this->Invalidate();
    return 0;

  case 75: {
    const char* name = reinterpret_cast<const char*>(a);
    for (int index = 2; index >= 0; --index) {
      ITEM_INFO& material = this->materials[index];
      if (material.unknown_0000[1] > 0 && material.itemId == name) {
        g_session.AddItem1710(material, 1);
        material.Reset();
        this->Invalidate();
        return 0;
      }
    }

    return 0;
  }

  case 6: {
    const int response = static_cast<int>(a);
    this->RecordResponse(response, b, c, d);

    switch (response) {
    case 184:
      g_modeMgr.GetGameMode()->SendMsg(130, this->makeType, reinterpret_cast<int>(this->materials), 0, 0);
      g_windowMgr.CloseWindow(80);
      return 0;

    case 185: {
      g_modeMgr.GetGameMode()->SendMsg(130, 0, reinterpret_cast<int>(this->materials), 0, 0);
      bool restored = false;
      for (int index = 0; index < 3; ++index) {
        if (this->materials[index].unknown_0000[1] > 0) {
          restored = true;
          g_session.AddItem1710(this->materials[index], 0);
        }
      }

      if (restored) {
        RefreshItemWindows(&g_windowMgr);
      }

      if (g_windowMgr.unknown_01D4) {
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01D4)->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }

      g_windowMgr.CloseWindow(80);
      return 0;
    }
    }

    return 0;
  }

  case 123: {
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

    if (tag.id == 22000) {
      while (static_cast<unsigned>(b) > offset) {
        std::memcpy(&tag, bytes + offset, sizeof(tag));
        offset += sizeof(tag);

        if (tag.id == 22001) {
          break;
        }

        if (tag.id == 22002) {
          std::memcpy(&this->makeType, bytes + offset, tag.bytes);
          offset += tag.bytes;
          this->SendMsg(nullptr, 40, this->makeType, 0, 0, 0);
        } else {
          offset += tag.bytes;
        }
      }
    }

    return 0;
  }
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
