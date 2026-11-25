#pragma once

#include "Network/Packets/NoticeEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Items/ItemType.h"
#include "Contents/CombatTypes.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Jobs/JobId.h"
#include "Script/LuaInstance.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Buttons/UITextButton.h"
#include "Resource/World/TerrainResource.h"
#include "Contents/Actors/Pc.h"
#include "Client/Application/ErrorDialog.h"
#include "UI/CashShop/UIMileageWnd.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/World/World.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Events/SpecialPopupMgr.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Render/Text/EmbedFont.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/CryptName.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <format>
#include <list>
#include <map>
#include <new>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
extern "C" {
#include <lualib.h>
}
#include "Contents/Effects/Core/EffectTypes.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Gauges/UIPcGage.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Contents/Messenger/MessengerWindows.h"
#include "Contents/Messenger/EquipmentPropertyStore.h"
#include "Contents/Messenger/EquipmentPropertiesMgr.h"
#include <fmt/format.h>
#include <fmt/xchar.h>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
using SessionShopTree = std::_Tree<std::_Tmap_traits<
  int, SessionShop, std::less<int>, std::allocator<std::pair<const int, SessionShop>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, SessionShop>, void*>*>
SessionShopTree::_Find_lower_bound<int>(const int&) const;
extern template void std::list<CSkillInfo>::clear() noexcept;
using NameToIdTree = std::_Tree<std::_Tmap_traits<
  std::string, int, std::less<std::string>, std::allocator<std::pair<const std::string, int>>, false>>;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, int>, void*>*>
NameToIdTree::_Find_lower_bound<std::string>(const std::string&) const;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::unordered_map<int, EquipmentValueC>::unordered_map();
template __declspec(noinline) std::unordered_map<int, int>::unordered_map();
template __declspec(noinline) std::unordered_map<unsigned int, int>::unordered_map();
template __declspec(noinline) std::unordered_map<int, int>::~unordered_map();
template __forceinline std::set<unsigned int>::_Mybase::~_Tree() noexcept;
template __forceinline std::map<int, int>::_Mybase::~_Tree() noexcept;
template __forceinline void std::_Tree_val<std::_Tree_simple_types<unsigned int>>::_Erase_head(std::allocator<std::_Tree_node<unsigned int, void*>>&) noexcept;
template __forceinline void std::_Tree_val<std::_Tree_simple_types<std::pair<const int, int>>>::_Erase_head(std::allocator<std::_Tree_node<std::pair<const int, int>, void*>>&) noexcept;
template __forceinline void std::_Tree_val<std::_Tree_simple_types<std::pair<const std::int64_t, EquipmentPropertyDefinition*>>>::_Erase_head(std::allocator<std::_Tree_node<std::pair<const std::int64_t, EquipmentPropertyDefinition*>, void*>>&) noexcept;
template __forceinline std::set<unsigned int>::~set() noexcept;
template __forceinline std::map<int, int>::~map() noexcept;
template __declspec(noinline) std::string fmt::format<const char*&, int&>(fmt::format_string<const char*&, int&>, const char*&, int&);
template __declspec(noinline) std::string fmt::format<int&, const char*&>(fmt::format_string<int&, const char*&>, int&, const char*&);
template __declspec(noinline) std::string fmt::format<double&, int&>(fmt::format_string<double&, int&>, double&, int&);
template __declspec(noinline) std::wstring fmt::format<std::wstring, std::wstring>(fmt::wformat_string<std::wstring, std::wstring>, std::wstring&&, std::wstring&&);
template __declspec(noinline) std::_Hash<std::_Umap_traits<int, int, std::_Uhash_compare<int, EquipmentProperties::CEquipmentValueHash, std::equal_to<int>>, std::allocator<std::pair<const int, int>>, false>>::~_Hash();
template __forceinline void std::_Hash<std::_Umap_traits<std::string, std::string, std::_Uhash_compare<std::string, std::hash<std::string>, std::equal_to<std::string>>, std::allocator<std::pair<const std::string, std::string>>, false>>::clear() noexcept;
template __forceinline std::_Hash_find_last_result<std::_List_node<std::pair<const int, int>, void*>*> std::_Hash<std::_Umap_traits<int, int, std::_Uhash_compare<int, EquipmentProperties::CEquipmentValueHash, std::equal_to<int>>, std::allocator<std::pair<const int, int>>, false>>::_Find_last<int>(const int&, const std::size_t) const;
template __forceinline void std::_Tree<std::_Tset_traits<unsigned int, std::less<unsigned int>, std::allocator<unsigned int>, true>>::clear() noexcept;
template __declspec(noinline) std::string fmt::format<int, const char*&>(fmt::format_string<int, const char*&>, int&&, const char*&);
template __declspec(noinline) std::string fmt::format<int, const char*&, std::string&>(fmt::format_string<int, const char*&, std::string&>, int&&, const char*&, std::string&);
template __declspec(noinline) std::string fmt::format<const char*&, int>(fmt::format_string<const char*&, int>, const char*&, int&&);
template __declspec(noinline) std::string fmt::format<unsigned int&, int>(fmt::format_string<unsigned int&, int>, unsigned int&, int&&);
