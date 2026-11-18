#pragma once

#include <string>
#include "UI/Window/WindowId.h"
#include "Contents/Effects/Core/EffectMessageId.h"
#include "Script/LuaScript.h"
#include "Archive/PakRegistry.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/EmitterEffect.h"
#include "Contents/Effects/Skills/EvilsPawEffect.h"
#include "Contents/Effects/Skills/WindEffect.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Skills/SkillTreeScript.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Script/LuaFileOperations.h"
#include "UI/CashShop/UICashShopWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindow.h"
#include "UI/Events/UICheckAttendanceWnd.h"
#include "UI/Items/UILapineScrollBar.h"
#include "UI/Items/UILapineUpgradeBoxWnd.h"
#include "UI/Window/UIWindowMgr.h"
extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <utility>
#include <vector>
#include <windows.h>

extern template std::string& std::string::insert(std::string::size_type, const char*, std::string::size_type);
template __declspec(noinline) std::_Tree_find_result<std::map<int, std::int64_t>::_Mybase::_Nodeptr> std::map<int, std::int64_t>::_Mybase::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) SubChatWndInfo& std::map<int, SubChatWndInfo>::operator[](int&&);
template __declspec(noinline) std::vector<std::string>& std::map<std::string, std::vector<std::string>>::operator[](const std::string&);
template __declspec(noinline) MiniMapPosition& std::map<std::string, MiniMapPosition>::operator[](const std::string&);
