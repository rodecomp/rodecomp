#pragma once

#include "Client/Session/Session.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/AdventureGuide/AdventureGuideMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "Render/Renderer.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/Items/ItemDescription.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include <cstring>
#include <format>
#include <list>
#include <regex>
#include <utility>
#include "Utility/Cipher/Obfuscate.h"

using StatusRequestTree = std::_Tree<std::_Tset_traits<unsigned int, std::less<unsigned int>, std::allocator<unsigned int>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<unsigned int, void*>*>
StatusRequestTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;
extern template AdventureGuide::CUIQuestInfo** std::allocator<AdventureGuide::CUIQuestInfo*>::allocate(std::size_t);
extern template void std::_List_node<AdventureGuide::CItemData, void*>::_Free_non_head(
  std::allocator<std::_List_node<AdventureGuide::CItemData, void*>>&, std::_List_node<AdventureGuide::CItemData, void*>*) noexcept;
extern template void std::_List_node<AdventureGuide::CQuestInfo, void*>::_Free_non_head(
  std::allocator<std::_List_node<AdventureGuide::CQuestInfo, void*>>&, std::_List_node<AdventureGuide::CQuestInfo, void*>*) noexcept;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<unsigned int, void*>*>
StatusRequestTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;
template __declspec(noinline) std::string std::sub_match<std::string::const_iterator>::str() const;
template __declspec(noinline) void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();
template __declspec(noinline) void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template __declspec(noinline) std::function<void(CUIListItem*)>& std::function<void(CUIListItem*)>::operator=(const std::function<void(CUIListItem*)>&);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUITab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::OnCreate(int, int);
template void CUIListBox<AdventureGuide::CUITab>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::OnCreate(int, int);
template __forceinline void std::vector<std::string>::_Buy_nonzero(const std::size_t);
template __forceinline void std::vector<std::string>::_Buy_raw(const std::size_t);
