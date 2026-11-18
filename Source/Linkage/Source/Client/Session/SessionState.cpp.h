#pragma once

#include <cstdint>
#include <set>
#include <xtree>
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/World/World.h"
#include "Script/LuaScript.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Status/UIBasicInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Chat/UINewChatWnd.h"
extern "C" {
#include <lua.h>
}
#include <cstddef>
#include <cstring>
#include <map>
#include <string>
#include <vector>

using PendingActorNameTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
PendingActorNameTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
PendingActorNameTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
