#pragma once

#include "Contents/RuneSystem/RuneSystem.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Messenger/EquipmentPropertiesMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaFileOperations.h"
#include "Script/LuaScript.h"
#include "Script/NLuaState.h"
#include "Utility/Text/FormatString.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <algorithm>
#include <format>
#include <new>
#include <string>
#include <windows.h>

template __declspec(noinline) std::string std::operator+(std::string&&, const char* const);
