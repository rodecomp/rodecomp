#pragma once

#include "Client/Replay/Replay.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/StylingShop/UIStylingShopWnd.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Session/Session.h"
#include "Network/Packets/PacketId.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Tables/MessageTable.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindowMgr.h"
#include <cstring>
#include <mbstring.h>
#include "Resource/MSGSTRINGID.h"

template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) std::string& std::string::operator=(std::string&&) noexcept;
template __forceinline bool std::operator==<char, std::char_traits<char>, std::allocator<char>>(const std::string&, const char* const);
template __declspec(noinline) std::vector<int>::~vector() noexcept;
template __forceinline std::_Tree<std::_Tmap_traits<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, false>>::iterator std::_Tree<std::_Tmap_traits<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, false>>::find(const int&);
