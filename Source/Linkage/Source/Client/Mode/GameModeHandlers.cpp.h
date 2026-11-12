#pragma once

#include "Client/Mode/CGameMode.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Actors/ActorEnums.h"
#include "Network/Packets/NoticeEnums.h"
#include "Contents/Skills/SkillUnitId.h"
#include "Network/Packets/ModeResultEnums.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Client/Replay/Replay.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Clan/ClanInfo.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Skills/GraffiSkill.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Skills/MonsterSkillEffects.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Rodex/RodexMailStore.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "File/FileCreation.h"
#include "Script/LuaScript.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Rodex/UIRodexInboxWnd.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Gauges/UIBarGraph.h"
#include "UI/Items/UIGradeEnchantWnd.h"
#include "UI/Status/UIHomunInfoWnd.h"
#include "UI/Party/UISeekPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <map>
#include <new>
#include <string>
#include <utility>
#include <vector>
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <timeapi.h>
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include "UI/Text/UIBroadcastBalloon.h"

extern template std::uint32_t& std::map<std::uint32_t, std::uint32_t>::operator[](const std::uint32_t&);
using RodexMailTree = std::_Tree<std::_Tmap_traits<
  std::int64_t, RodexMail, std::greater<std::int64_t>, std::allocator<std::pair<const std::int64_t, RodexMail>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::int64_t, RodexMail>, void*>*>
RodexMailTree::_Find_lower_bound<std::int64_t>(const std::int64_t&) const;
extern template decltype(auto) std::list<BroadcastBalloonLine>::emplace_back<std::string&, std::uint32_t&>(std::string&, std::uint32_t&);
extern template std::list<BroadcastBalloonLine>::~list();
