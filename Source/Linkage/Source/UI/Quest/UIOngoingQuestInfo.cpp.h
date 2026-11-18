#pragma once

#include "UI/Window/WindowId.h"
#include "UI/Quest/UIOngoingQuestInfo.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Render/Renderer.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Quest/UIRenewQuestWnd.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstring>
#include <format>
#include <string>

template __declspec(noinline) std::function<void(CUIListItem*)>& std::function<void(CUIListItem*)>::operator=(const std::function<void(CUIListItem*)>&);
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, RenewQuestOngoingInfo>, void*>*>
std::map<unsigned int, RenewQuestOngoingInfo>::_Mybase::_Find_lower_bound<unsigned int>(const unsigned int&) const;
