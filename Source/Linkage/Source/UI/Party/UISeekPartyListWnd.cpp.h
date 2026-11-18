#pragma once

#include "UI/Party/UISeekPartyListWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Party/PartyBookingMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Party/UISeekPartyWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <cstdlib>
#include <cstring>

template __declspec(noinline) std::pair<std::map<int, std::vector<SeekPartyMapEntry>>::_Mybase::_Nodeptr, bool> std::map<int, std::vector<SeekPartyMapEntry>>::_Mybase::_Emplace<std::pair<const int, std::vector<SeekPartyMapEntry>>>(std::pair<const int, std::vector<SeekPartyMapEntry>>&&);
template __declspec(noinline) bool std::operator==(const std::string&, const char* const);
