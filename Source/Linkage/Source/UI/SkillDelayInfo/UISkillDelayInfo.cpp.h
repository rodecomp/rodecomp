#pragma once

#include <string>
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/SkillDelayInfo/SkillDelayInfo.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Script/LuaScript.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <map>
#include <vector>
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

extern template std::string std::operator+(std::string&&, const char* const);
template __declspec(noinline) void std::_List_node<SkillDelayInfo::CDelayItemData, void*>::_Free_non_head(
  std::allocator<std::_List_node<SkillDelayInfo::CDelayItemData, void*>>&, std::_List_node<SkillDelayInfo::CDelayItemData, void*>*) noexcept;
template __declspec(noinline) std::list<SkillDelayInfo::CDelayItemData>::~list() noexcept;
template __declspec(noinline) std::vector<std::string>::~vector() noexcept;
