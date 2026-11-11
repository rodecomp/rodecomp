#pragma once

#include "Network/Packets/NoticeEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/CombatTypes.h"
#include "Contents/Items/InventoryType.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/ClientState.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "Contents/World/World.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <new>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

extern template std::list<ITEM_INFO>::iterator std::list<ITEM_INFO>::erase(std::list<ITEM_INFO>::const_iterator) noexcept;
extern template void std::_List_node<MessengerMember, void*>::_Free_non_head(std::allocator<std::_List_node<MessengerMember, void*>>&, std::_List_node<MessengerMember, void*>*) noexcept;
