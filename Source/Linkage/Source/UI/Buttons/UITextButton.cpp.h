#pragma once

#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Client/Replay/Replay.h"
#include "UI/Window/WindowPolicy.h"
#include "Render/Text/FontCache.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Gauges/UIBarGraph.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "Render/Text/TextContext.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Items/UIItemMoveInfo.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Lists/UIImgMenu.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Lists/UITreeView.h"
#include "UI/Buttons/UIStaticItem.h"
#include "Client/Session/Session.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Window/UIBasicCtrl.h"
#include "Utility/Text/FormatString.h"
#include "UI/Lists/UIMemberListBox.h"
#include "UI/Buttons/UIBitmapButton2.h"
#include "UI/Buttons/UISlotBmpButton.h"
#include "UI/Buttons/UIStaticBitmap.h"
#include "UI/Buttons/UITVBitmapButton.h"
#include "UI/Buttons/UITVTextButton.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIComboBoxV2Wnd.h"
#include "UI/Items/UIItemStoreComboBox.h"
#include "UI/SelectPackageItemBox/UIPackageBoxNameList.h"
#include "UI/Lists/UIListBoxEx.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "UI/Lists/TableBox.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Text/UIString.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "Client/Input/Language.h"
#include "Utility/Text/GroupedInteger.h"
#include "Client/Input/Mouse.h"
#include "Contents/Items/ItemDescription.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Navigation/UIListBoxNavi.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/WindowBorder.h"
#include "Resource/Bitmap/SkinPalette.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Resource/MSGSTRINGID.h"
#include <format>

extern template std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);
using IntIntTree = std::_Tree<std::_Tmap_traits<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, int>, void*>*>
IntIntTree::_Find_lower_bound<int>(const int&) const;
using IdListLineTree = std::_Tree<std::_Tmap_traits<int, IdListLine, std::less<int>, std::allocator<std::pair<const int, IdListLine>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, IdListLine>, void*>*>
IdListLineTree::_Find_lower_bound<int>(const int&) const;
using ExtendedListLineTree = std::_Tree<std::_Tmap_traits<int, ExtendedListLine, std::less<int>, std::allocator<std::pair<const int, ExtendedListLine>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, ExtendedListLine>, void*>*>
ExtendedListLineTree::_Find_lower_bound<int>(const int&) const;
extern template UITextButton** std::allocator<UITextButton*>::allocate(const std::size_t);
extern template std::string std::format<const std::string&>(const std::_Fmt_string<const std::string&>, const std::string&);
extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
