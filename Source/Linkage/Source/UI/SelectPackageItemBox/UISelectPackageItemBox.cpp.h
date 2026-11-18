#pragma once

#include "UI/SelectPackageItemBox/SelectPackageItemBox.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Render/Renderer.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <new>

template __declspec(noinline) std::string::basic_string(const std::string&);
template __declspec(noinline) std::function<void(CUIListItem*)>&
std::function<void(CUIListItem*)>::operator=(const std::function<void(CUIListItem*)>&);
template __declspec(noinline) std::list<ITEM_INFO>::~list() noexcept;
template __declspec(noinline) std::list<SelectPackageItemBox::CTabInfoData>::~list() noexcept;
