#pragma once

#include "Contents/Skills/ClientSkills.h"
#include "Contents/Actors/GameActor.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/ClientState.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Map/MapPropertyMgr.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/Tokenizer.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/EquipmentPropertiesMgr.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Skill/UINewSkillListWnd.h"
#include "UI/Skill/UISkillListWnd.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>

template __declspec(noinline) std::_Tree_find_result<std::map<std::int64_t, std::int64_t>::_Mybase::_Nodeptr> std::map<std::int64_t, std::int64_t>::_Mybase::_Find_lower_bound<std::int64_t>(const std::int64_t&) const;
template __declspec(noinline) std::pair<std::map<int, SessionRecord6144>::_Mybase::_Nodeptr, bool> std::map<int, SessionRecord6144>::_Mybase::_Emplace<std::pair<int, SessionRecord6144>>(std::pair<int, SessionRecord6144>&&);
