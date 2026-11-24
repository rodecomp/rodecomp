#pragma once

#include "Contents/Items/EnchantMgr.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Network/Packets/PacketId.h"
#include "Client/Session/Session.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Items/ItemDBToIDMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Enchant/EnchantUI.h"
#include "UI/Window/UIWindowMgr.h"
#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <new>

template CEnchantMgr* Singleton<CEnchantMgr>::GetInstance();
template __declspec(noinline) EnchantInfo& std::map<uint64_t, EnchantInfo>::operator[](uint64_t&&);
template __forceinline std::map<int, EnchantPerfectInfo>& std::map<int, std::map<int, EnchantPerfectInfo>>::operator[](const int&);
