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

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
using SessionShopTree = std::_Tree<std::_Tmap_traits<
  int, SessionShop, std::less<int>, std::allocator<std::pair<const int, SessionShop>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, SessionShop>, void*>*>
SessionShopTree::_Find_lower_bound<int>(const int&) const;
extern template void std::list<CSkillInfo>::clear() noexcept;
using NameToIdTree = std::_Tree<std::_Tmap_traits<
  std::string, int, std::less<std::string>, std::allocator<std::pair<const std::string, int>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, int>, void*>*>
NameToIdTree::_Find_lower_bound<std::string>(const std::string&) const;
