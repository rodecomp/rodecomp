#pragma once

#include <cstddef>
#include <string>
#include "UI/Window/UIEnums.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Items/UIEquipmentPropertiesWnd.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/EquipmentPropertiesMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/StringUtil.h"
#include "Resource/MSGSTRINGID.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <new>
#include <windows.h>

extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::function<void(CUIListItem*)>& std::function<void(CUIListItem*)>::operator=(const std::function<void(CUIListItem*)>&);
template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Paint();
template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::OnWheel(int);
template void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::ClearItems();
