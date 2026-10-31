#include "UI/ProbabilityTable/UIProbabilityTable.h"

#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/NeoPackageItemMgr.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/TextEncoding.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Text/UIStaticTextControl.h"

#include <atlconv.h>
#include <cmath>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <windows.h>

// OFFSET: 0x00A4E3B0
// FUNCTION: fn_16206
template <>
bool CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::RebindIndex(int index) {
  if (index != -1) {
    for (ProbabilityTable::CUIItem* item : this->items) {
      if (index == item->index) {
        if (!this->binder) {
          return false;
        }
        this->binder(item, *std::next(this->data.begin(), index));
        return true;
      }
    }
  }
  return false;
}

// OFFSET: 0x00A4E490
// FUNCTION: fn_16207
int CUIProbabilityTable::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 177) {
    this->m_searchEdit->SetFocus();
  }
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x00A4E4D0
// FUNCTION: fn_16208
void CUIProbabilityTable::SetItem(int itemId) {
  this->m_itemId = itemId;

  {
    ITEM_INFO item;
    item.SetItemId(itemId);
    CUIStaticText* label = this->m_itemName;
    const std::string name = item.DisplayName(0);
    label->SetText(name.c_str(), name.size());
  }

  this->m_list->ClearData();

  if (!Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::s_instance) {
    Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::s_instance = new (std::nothrow) NEO_PACKAGEITEM::CNeoPackageItemMgr;
  }

  std::optional<std::map<int, NEO_PACKAGEITEM::PackageGroup>*> package = Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::s_instance->FindPackage(itemId);
  if (!package) {
    return;
  }

  std::uint32_t color = 0xFFFFEFE7;
  for (const auto& group : **package) {
    color = color == 0xFFFFEFE7 ? 0xFFFFFFFF : 0xFFFFEFE7;

    std::list<NEO_PACKAGEITEM::PackageEntry> entries = group.second.items;
    entries.sort([](const NEO_PACKAGEITEM::PackageEntry& left, const NEO_PACKAGEITEM::PackageEntry& right) { return left.probability < right.probability; });

    for (const NEO_PACKAGEITEM::PackageEntry& entry : entries) {
      long long value = static_cast<long long>(std::pow(10.0, 6.0));
      if (group.first != 0) {
        value = (static_cast<long long>(std::pow(10.0, 7.0) * entry.probability / group.second.totalProbability) + 5) / 10;
      }

      ProbabilityTable::CItemData data;
      data.color = color;
      data.name = entry.name;
      data.probability = static_cast<double>(value) / 10000.0;
      this->m_list->data.push_back(std::move(data));
    }
  }

  this->m_list->UpdateScrollBar();
}

// OFFSET: 0x00A4EBD0
// FUNCTION: fn_16210
int HighlightKeyword(std::string& text, std::string_view keyword, std::string_view prefix, std::string_view suffix) {
  int count = 0;
  if (FindSubstring(text.c_str(), static_cast<int>(text.size()), keyword.data()) == -1) {
    return 0;
  }

  std::wstring wideText(ATL::CA2WEX<128>(text.c_str(), CP_THREAD_ACP));
  std::wstring lowerText = LowercaseWide(text);
  std::wstring lowerKeyword = LowercaseWide(keyword.data());
  std::wstring widePrefix(ATL::CA2WEX<128>(prefix.data(), CP_THREAD_ACP));
  std::wstring wideSuffix(ATL::CA2WEX<128>(suffix.data(), CP_THREAD_ACP));

  std::size_t position = 0;
  while ((position = lowerText.find(lowerKeyword, position)) != std::wstring::npos) {
    lowerText.insert(position, widePrefix.c_str(), widePrefix.size());
    wideText.insert(position, widePrefix.c_str(), widePrefix.size());
    position += lowerKeyword.size() + widePrefix.size();
    lowerText.insert(position, wideSuffix.c_str(), wideSuffix.size());
    wideText.insert(position, wideSuffix.c_str(), wideSuffix.size());
    position += suffix.size();
    ++count;
  }

  text = std::string(ATL::CW2AEX<128>(wideText.c_str(), CP_THREAD_ACP));
  return count;
}
