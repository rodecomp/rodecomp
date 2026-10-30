#include "UI/Items/UIMakeTargetProcessWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdint>
#include <cstring>

struct ClientItemLists {
  bool AddItem1710(const ITEM_INFO& item, unsigned char refresh);
};

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
  case 6: {
    const int response = static_cast<int>(a);
    this->m_recorder->Record(this->geometry.id, response, b, c, d);

    if (response == 184) {
      g_modeMgr.GetGameMode()->SendMsg(130, this->makeType, reinterpret_cast<int>(this->materials), 0, 0);
      this->m_frameContext->windows.CloseWindow(80);
      return 0;
    }

    if (response == 185) {
      g_modeMgr.GetGameMode()->SendMsg(130, 0, reinterpret_cast<int>(this->materials), 0, 0);
      bool restored = false;
      for (ITEM_INFO& material : this->materials) {
        if (material.unknown_0000[1] != 0) {
          restored = true;
          reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(material, 0);
        }
      }

      if (restored) {
        RefreshItemWindows(&g_windowMgr);
      }

      if (g_windowMgr.unknown_01D4) {
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01D4)->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }

      this->m_frameContext->windows.CloseWindow(80);
      return 0;
    }

    return 0;
  }

  case 40:
    this->makeType = static_cast<int>(a);
    this->Invalidate();
    return 0;

  case 75: {
    const char* name = reinterpret_cast<const char*>(a);
    for (int index = 2; index >= 0; --index) {
      ITEM_INFO& material = this->materials[index];
      if (material.unknown_0000[1] != 0 && material.itemId == name) {
        reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(material, 1);
        material.Reset();
        this->Invalidate();
        return 0;
      }
    }

    return 0;
  }

  case 123: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    std::uint16_t marker;
    std::memcpy(&marker, bytes, sizeof(marker));
    unsigned offset = 6;

    if (marker == 22000) {
      while (static_cast<unsigned>(b) > offset) {
        std::int16_t kind;
        std::uint32_t size;
        std::memcpy(&kind, bytes + offset, sizeof(kind));
        std::memcpy(&size, bytes + offset + 2, sizeof(size));
        offset += 6;

        if (kind == 22001) {
          break;
        }

        if (kind == 22002) {
          std::memcpy(&this->makeType, bytes + offset, size);
          offset += size;
          this->SendMsg(nullptr, 40, this->makeType, 0, 0, 0);
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
