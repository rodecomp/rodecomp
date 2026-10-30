#include "UI/Quest/UIQuestDisplay.h"
#include "Client/Input/Mouse.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>

#pragma pack(push, 1)
struct PACKET_CZ_ACTIVE_QUEST {
  short packetType;
  unsigned int questId;
  unsigned char active;
};
#pragma pack(pop)

// OFFSET: 0x009515D0
// FUNCTION: fn_12953
void UIQuestDisplay::Slot11() {
  if (this->m_visibleSetting) {
    *this->m_visibleSetting = this->geometry.visible;
  }
}

// OFFSET: 0x00950BC0
// FUNCTION: fn_12947
int UIQuestDisplay::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 34: {
    this->m_visibleSetting = reinterpret_cast<int*>(argument1);
    QuestDisplayInfo* info = reinterpret_cast<QuestDisplayInfo*>(argument1);
    if (!info->IsValid()) {
      info->Reset();
    }
    this->Show(*this->m_visibleSetting);
    break;
  }
  case 48: {
    int index = 0;
    for (auto it = g_renewQuestMgr->m_ongoingQuests.begin(); it != g_renewQuestMgr->m_ongoingQuests.end(); ++it, ++index) {
      if (index == this->m_selectedIndex) {
        int command = this->m_menuItems[argument1].command;
        if (command == 48) {
          PACKET_CZ_ACTIVE_QUEST packet;
          packet.packetType = 0x2B6;
          packet.questId = it->first;
          packet.active = !it->second.m_active;
          GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
          this->Invalidate();
        } else if (command == 49) {
          g_renewQuestMgr->sub_67B620(it->first);
        }
        break;
      }
    }
    break;
  }
  case 49: {
    this->m_menuItems.clear();
    if (g_windowMgr.unknown_0210 != 0) {
      this->Show(0);
    }
    if (this->m_selectedIndex != -1) {
      std::string text1 = LookupMessage(1622);
      this->m_menuItems.push_back({text1.c_str(), 49});
      std::string text2 = LookupMessage(351);
      this->m_menuItems.push_back({text2.c_str(), 48});
    }
    if (!this->m_menuItems.empty()) {
      UIWindow* menu = reinterpret_cast<WindowManagerWindows*>(&g_windowMgr)->EnsureWindow(18);
      for (size_t i = 0; i < this->m_menuItems.size(); ++i) {
        menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(this->m_menuItems[i].text.c_str()), 0, 0, 0);
      }
      menu->SendMsg(nullptr, 83, 204, 0, 0, 0);
      menu->SendMsg(nullptr, 30, 0, 0, 0, 0);

      int x = std::max(1, g_mouse.x + 1);
      int maxX = g_renderer->viewport.width - menu->geometry.width;
      if (x > maxX) {
        x = maxX;
      }
      int y = std::max(1, g_mouse.y + 1);
      int maxY = g_renderer->viewport.height - menu->geometry.height;
      if (y > maxY) {
        y = maxY;
      }
      menu->Move(x, y);
    }
    break;
  }
  default:
    return this->UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}
