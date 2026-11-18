#pragma once

#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Quest/UIQuestDisplay.h"
#include "Client/Input/Mouse.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Text/ColoredText.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/UITextViewer.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <algorithm>
#include <cstring>

template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned int, RenewQuestOngoingInfo>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const unsigned int, RenewQuestOngoingInfo>>>, std::_Iterator_base0>::operator++() noexcept;
