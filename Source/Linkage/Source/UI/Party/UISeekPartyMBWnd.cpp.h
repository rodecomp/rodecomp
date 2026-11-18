#pragma once

#include <string>
#include "UI/Party/UISeekPartyMBWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Party/UISeekPartyWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern template std::string::basic_string(const std::string&);
template __declspec(noinline) std::pair<std::map<int, std::vector<std::pair<std::string, int>>>::_Mybase::_Nodeptr, bool> std::map<int, std::vector<std::pair<std::string, int>>>::_Mybase::_Emplace<std::pair<const int, std::vector<std::pair<std::string, int>>>>(std::pair<const int, std::vector<std::pair<std::string, int>>>&&);
