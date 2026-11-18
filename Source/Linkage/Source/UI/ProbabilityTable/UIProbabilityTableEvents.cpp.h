#pragma once

#include "UI/ProbabilityTable/UIProbabilityTable.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/NeoPackageItemMgr.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Network/TextEncoding.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Text/UIStaticTextControl.h"
#include <algorithm>
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

template __declspec(noinline) std::list<ProbabilityTable::CItemData>::iterator std::next(std::list<ProbabilityTable::CItemData>::iterator, std::ptrdiff_t);
template __declspec(noinline) void std::_List_node<ProbabilityTable::CItemData, void*>::_Free_non_head(
  std::allocator<std::_List_node<ProbabilityTable::CItemData, void*>>&, std::_List_node<ProbabilityTable::CItemData, void*>*) noexcept;
