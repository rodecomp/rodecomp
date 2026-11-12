#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Quest/UIQuestDisplay.h"
#include "Client/Input/Mouse.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Text/ColoredText.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/UITextViewer.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <algorithm>
#include <cstring>

template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned int, RenewQuestOngoingInfo>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned int, RenewQuestOngoingInfo>>>, std::_Iterator_base0>::operator++() noexcept;

#pragma pack(push, 1)
struct PACKET_CZ_ACTIVE_QUEST {
  short packetType;
  unsigned int questId;
  unsigned char active;
};
#pragma pack(pop)

// OFFSET: 0x00948D10
// FUNCTION: fn_12881
void UIQuestDisplay::Create(int width, int height) {
  this->SetSize(width, height);
}

// OFFSET: 0x0094A400
// FUNCTION: fn_12887
bool UIQuestDisplay::IsTransmitMouseInput() {
  return true;
}

// OFFSET: 0x0094D910
// FUNCTION: fn_12911
void UIQuestDisplay::Paint() {
  this->ClearSurface(0xFF00FF);

  CRenewQuestMgr* manager = g_renewQuestMgr;
  int none = 0;
  int multiple = 0;
  int single = 0;
  for (auto it = manager->m_ongoingQuests.begin(); it != manager->m_ongoingQuests.end(); ++it) {
    const unsigned int questId = it->first;
    const RenewQuestOngoingInfo& info = it->second;
    if (info.m_active != true || info.m_simpleView != true) {
      continue;
    }

    const int top = single * 50 + (none + multiple * 2) * 40;
    std::string title = info.m_title.size() == 0 ? FormatOwnedString("Unknown Quest, ID = %d", questId) : info.m_title;
    if (questId == this->selectedQuestId) {
      this->FillRegion(-12, top - 2, this->geometry.width - 10, 40, 0);
    }
    this->DrawShadowedText(0, top, title.c_str(), 0, 0xD6E1, 0, 0, 12, 0);

    std::string description = info.m_descriptions.empty() ? std::string() : info.summary;
    description = StripColorCodes(description.c_str(), description.size());
    description = static_cast<const std::string&>(DisplayUrlTextTag(description));
    description = static_cast<const std::string&>(DisplayNavigationTextTag(description));
    description = static_cast<const std::string&>(DisplayTipBoxTextTag(description));
    description = static_cast<const std::string&>(DisplayItemTextTag(description));
    description = static_cast<const std::string&>(RemoveTextLinkInfo(description));
    if (g_windowMgr.chatWnd) {
      description = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(description);
    }
    std::string navigation;
    DisplayNavigationLink(navigation, description);
    description = std::move(navigation);
    description = static_cast<const std::string&>(DisplayQuestTextTag(description));

    if (std::strlen(description.c_str()) > 30) {
      description = this->TruncateText(description.c_str(), 30);
      description += std::string("...");
    }
    if (std::strlen(description.c_str()) != 0) {
      this->DrawShadowedText(0, top + 15, description.c_str(), 0, 0xFFFFFF, 0, 0, 12, 0);
    }

    if (info.m_huntCount == 0 || info.m_huntCount > 3) {
      ++none;
    } else {
      for (int index = 0; index < info.m_huntCount; ++index) {
        std::string hunt;
        if (info.m_huntMobId[index] == 31999) {
          hunt = static_cast<const std::string&>(
            g_renewQuestMgr->GetMonsterConditionText(info.m_huntConditionFlags[index], info.m_huntMinLevel[index], info.m_huntMaxLevel[index])
          );
        } else {
          const short minLevel = info.m_huntMinLevel[index];
          const short maxLevel = info.m_huntMaxLevel[index];
          const char* name = info.m_huntName[index];
          if (minLevel > -1 && maxLevel > -1 && (minLevel != 0 || maxLevel != 0)) {
            if (minLevel == 0) {
              hunt = static_cast<const std::string&>(FormatOwnedString("%s Lv.%d▼", name, maxLevel));
            } else if (maxLevel == 0) {
              hunt = static_cast<const std::string&>(FormatOwnedString("%s Lv.%d▲", name, minLevel));
            } else {
              hunt = static_cast<const std::string&>(FormatOwnedString("%s Lv.%d~Lv.%d", name, minLevel, maxLevel));
            }
          } else {
            hunt = static_cast<const std::string&>(FormatOwnedString("%s", name));
          }
        }

        hunt = StripColorCodes(hunt.c_str(), hunt.size());
        description = static_cast<const std::string&>(DisplayUrlTextTag(description));
        description = static_cast<const std::string&>(DisplayNavigationTextTag(description));
        description = static_cast<const std::string&>(DisplayTipBoxTextTag(description));
        description = static_cast<const std::string&>(DisplayItemTextTag(description));
        description = static_cast<const std::string&>(RemoveTextLinkInfo(description));
        if (g_windowMgr.chatWnd) {
          description = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(description);
        }
        std::string huntNavigation;
        DisplayNavigationLink(huntNavigation, description);
        description = std::move(huntNavigation);
        description = static_cast<const std::string&>(DisplayQuestTextTag(description));

        const short goal = info.m_huntMaxCount[index];
        if (goal > 0) {
          const short count = info.m_huntKillCount[index];
          if (count == goal) {
            hunt = static_cast<const std::string&>(FormatOwnedString("%s ( %s )", hunt.c_str(), LookupMessage(MSI_COMPLETE_HUNTING_QUEST)));
          } else {
            hunt = static_cast<const std::string&>(FormatOwnedString("%s ( %d / %d )", hunt.c_str(), count, goal));
          }

          const int y = index * 13 + top;
          this->DrawTextAt(11, y + 29, hunt.c_str(), 0, 0, 12, 0, 0, 0);
          this->DrawTextAt(10, y + 28, hunt.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
        }
      }

      if (info.m_huntCount > 1) {
        ++multiple;
      } else {
        ++single;
      }
    }

    if (single + multiple + none >= 5) {
      break;
    }
  }
}

// OFFSET: 0x0094F1D0
// FUNCTION: fn_12934
void UIQuestDisplay::OnRBtnUp(int x, int y) {
  const int previous = this->m_selectedIndex;
  int selected = -1;
  int single = 0;
  int none = 0;
  int multiple = 0;
  int index = 0;
  int offset = 0;
  const auto end = g_renewQuestMgr->m_ongoingQuests.cend();
  for (auto it = g_renewQuestMgr->m_ongoingQuests.cbegin(); it != end; ++it, ++index) {
    if (single + multiple + none >= 5) {
      break;
    }

    const int top = offset + (none + multiple * 2) * 40;
    const RenewQuestOngoingInfo& info = it->second;
    if (info.m_active && info.m_simpleView) {
      if (y >= top && y <= top + 10) {
        selected = index;
        break;
      }

      if (info.m_huntCount == 0) {
        ++none;
      } else if (info.m_huntCount > 1) {
        ++multiple;
      } else {
        ++single;
        offset += 50;
      }
    }
  }

  this->m_selectedIndex = selected;
  if (previous != this->m_selectedIndex) {
    this->Invalidate();
  }

  if (this->m_selectedIndex != -1) {
    this->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
  }
}

// OFFSET: 0x009515D0
// FUNCTION: fn_12953
void UIQuestDisplay::StoreInfo() {
  if (this->m_visibleSetting) {
    *this->m_visibleSetting = this->geometry.visible;
  }
}

// OFFSET: 0x00950BC0
// FUNCTION: fn_12947
int UIQuestDisplay::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_SETINFO: {
    this->m_visibleSetting = reinterpret_cast<int*>(argument1);
    QuestDisplayInfo* info = reinterpret_cast<QuestDisplayInfo*>(argument1);
    if (!info->IsValid()) {
      info->Reset();
    }
    this->Show(*this->m_visibleSetting);
    break;
  }
  case UM_MENU_SELECTED: {
    int index = 0;
    const auto end = g_renewQuestMgr->m_ongoingQuests.end();
    for (auto it = g_renewQuestMgr->m_ongoingQuests.begin(); it != end; ++it, ++index) {
      if (index == this->m_selectedIndex) {
        switch (this->m_menuItems[argument1].value) {
        case 48: {
          PACKET_CZ_ACTIVE_QUEST packet;
          packet.packetType = ID_CZ_ACTIVE_QUEST;
          packet.questId = it->first;
          packet.active = !it->second.m_active;
          GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
          this->Invalidate();
          break;
        }
        case 49:
          g_renewQuestMgr->ShowOngoingQuestInfo(it->first);
          break;
        }
        break;
      }
    }
    break;
  }
  case UM_MAKEMENU: {
    this->m_menuItems.erase(this->m_menuItems.begin(), this->m_menuItems.end());
    if (g_windowMgr.menuWnd != 0) {
      this->Show(0);
    }
    if (this->m_selectedIndex != -1) {
      this->m_menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_VIEW_QUEST_INFO_MSG)).c_str(), 49));
      this->m_menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_DELETE)).c_str(), 48));
    }
    if (this->m_menuItems.size() != 0) {
      UIWindow* menu = g_windowMgr.EnsureWindow(WID_MENUWND);
      for (size_t i = 0; i < this->m_menuItems.size(); ++i) {
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->m_menuItems[i].text.c_str()), 0, 0, 0);
      }
      menu->SendMsg(nullptr, UM_SET_CALLER_WID, 204, 0, 0, 0);
      menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);

      int x = std::max(g_mouse.x + 1, 1);
      int maxX = g_renderer->viewport.width - menu->geometry.width;
      if (x > maxX) {
        x = maxX;
      }
      int y = std::max(g_mouse.y + 1, 1);
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

// OFFSET: 0x00946EA0
// FUNCTION: fn_12855
UIQuestDisplay::UIQuestDisplay() : UIWindow(0) {
}
