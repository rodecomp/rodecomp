#pragma once

#include "UI/Dialogs/UIAccountLimitedToolWnd.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMessage.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

template __declspec(noinline) std::string::basic_string(const std::string&);
template std::string& std::string::operator=(std::string&&) noexcept;
