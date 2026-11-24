#pragma once

#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skin/SkinMgr.h"
#include "File/File.h"
#include "Contents/Sound/SoundMgr.h"
#include "File/ResourcePath.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourcePreload.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Items/UIItemWnd.h"
#include "UI/Events/UIRoulletteWnd.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Contents/World/World.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session/Session.h"
#include "Network/Packets/ClientOptionPacket.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Text/UITransBalloonText.h"
#include "Script/LuaInstance.h"
#include "Script/LuaScript.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Singleton.h"
#include "Utility/Text/StringUtil.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include "UI/Navigation/UIMinimapZoomWnd.h"
extern "C" {
#include <lua.h>
}
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <functional>
#include <list>
#include <optional>
#include <utility>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

template CGameSettingsMgr* Singleton<CGameSettingsMgr>::GetInstance();
template __forceinline std::_Hash_find_last_result<std::_List_node<std::pair<const int, std::function<void(int, bool, bool, int, int)>>, void*>*> std::_Hash<std::_Umap_traits<int, std::function<void(int, bool, bool, int, int)>, std::_Uhash_compare<int, std::hash<int>, std::equal_to<int>>, std::allocator<std::pair<const int, std::function<void(int, bool, bool, int, int)>>>, false>>::_Find_last<int>(const int&, const std::size_t) const;
template __forceinline std::_Hash_find_last_result<std::_List_node<std::pair<const int, std::function<void()>>, void*>*> std::_Hash<std::_Umap_traits<int, std::function<void()>, std::_Uhash_compare<int, std::hash<int>, std::equal_to<int>>, std::allocator<std::pair<const int, std::function<void()>>>, false>>::_Find_last<int>(const int&, const std::size_t) const;
template __forceinline bool std::operator==(const std::string&, const char* const);
