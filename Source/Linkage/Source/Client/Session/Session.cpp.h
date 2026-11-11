#pragma once

#include "Client/Session/Session.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/CombatTypes.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Guild/GuildMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/World/World.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/TextEncoding.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Status/UIBasicInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Client/Application/ErrorDialog.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/XorBytes.h"
#include "Script/LuaScript.h"
#include "Contents/Map/MapPropertyMgr.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Application/ApplicationWindow.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "File/CommonObject.h"
#include "Contents/World/WorldViewData.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Events/CheckAttendanceMgr.h"
#include "Contents/Items/BarterMarketItemMgr.h"
#include "Contents/Items/ChangeMaterialMgr.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "Contents/Map/PrivateAirplaneMgr.h"
#include "Contents/Events/SpecialPopupMgr.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <mbstring.h>
#include <list>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
using SessionShopTree = std::_Tree<std::_Tmap_traits<
  int, SessionShop, std::less<int>, std::allocator<std::pair<const int, SessionShop>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, SessionShop>, void*>*>
SessionShopTree::_Find_lower_bound<int>(const int&) const;
using NameToIdTree = std::_Tree<std::_Tmap_traits<
  std::string, int, std::less<std::string>, std::allocator<std::pair<const std::string, int>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, int>, void*>*>
NameToIdTree::_Find_lower_bound<std::string>(const std::string&) const;
using ShopEntryTree = std::_Tree<std::_Tmap_traits<
  int, std::pair<int, int>, std::less<int>, std::allocator<std::pair<const int, std::pair<int, int>>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::pair<int, int>>, void*>*>
ShopEntryTree::_Find_lower_bound<int>(const int&) const;
using ActorIdTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
ActorIdTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
extern template void std::vector<std::string>::resize(size_type, const std::string&);
extern template void std::vector<unsigned char>::_Tidy() noexcept;
extern template std::string& std::string::operator=(std::string&&) noexcept;
using PendingActorNameTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
PendingActorNameTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
