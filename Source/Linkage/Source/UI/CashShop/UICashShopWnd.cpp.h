#pragma once

#include <cstddef>
#include <string>
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/CashShop/UICashShopWnd.h"
#include "UI/Window/UIEnums.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Registry.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "UI/Buttons/UIStaticBitmap.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Login/UIPasswordWnd.h"
#include <algorithm>
#include <array>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <utility>
#include <windows.h>
#include <shellapi.h>
#include <timeapi.h>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) char* std::allocator<char>::allocate(const std::size_t);
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
