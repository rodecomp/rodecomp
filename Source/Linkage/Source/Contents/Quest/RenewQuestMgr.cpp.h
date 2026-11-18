#pragma once

#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Script/LuaInstance.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/TextEscapes.h"
#include "UI/Quest/UIOngoingQuestInfo.h"
#include "UI/Quest/UIRenewQuestUI.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Session/Session.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Quest/UIQuestDisplay.h"
#include "UI/Status/UIBasicInfoWnd.h"
extern "C" {
#include <lua.h>
#include <lauxlib.h>
}
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <new>
#include <utility>

template __declspec(noinline) bool std::string_view::_Equal(const std::string_view) const noexcept;
template __declspec(noinline) std::list<std::string>::const_iterator std::find<std::list<std::string>::const_iterator, std::string_view>(std::list<std::string>::const_iterator, const std::list<std::string>::const_iterator, const std::string_view&);
