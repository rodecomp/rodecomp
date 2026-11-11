#pragma once

#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Quest/UIRenewQuestUI.h"
#include "UI/Window/UIEnums.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Client/Replay/Replay.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Quest/UIOngoingQuestInfo.h"
#include "UI/Quest/UIRecommendedQuestInfo.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Utility/Cipher/CryptName.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>

using RenewQuestOngoingTree = std::_Tree<std::_Tmap_traits<
  unsigned int, RenewQuestOngoingInfo, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, RenewQuestOngoingInfo>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, RenewQuestOngoingInfo>, void*>*>
RenewQuestOngoingTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;
using RenewQuestRecommendedTree = std::_Tree<std::_Tmap_traits<
  unsigned int, RenewQuestRecommendedInfo, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, RenewQuestRecommendedInfo>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, RenewQuestRecommendedInfo>, void*>*>
RenewQuestRecommendedTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;
extern template std::function<void()>& std::function<void()>::operator=(const std::function<void()>&);
