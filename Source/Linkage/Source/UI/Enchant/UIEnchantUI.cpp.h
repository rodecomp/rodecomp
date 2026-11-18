#pragma once

#include <string>
#include <vector>
#include "UI/Enchant/EnchantUI.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Contents/Items/EnchantMgr.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <iterator>
#include <new>

extern template char* std::_UIntegral_to_buff<char, unsigned int>(char*, unsigned int);
extern template std::vector<int>::~vector();
template __declspec(noinline) std::map<int, EnchantPerfectInfo>::iterator std::map<int, EnchantPerfectInfo>::_Mybase::find(const int&);
template __declspec(noinline) std::map<int, std::map<int, EnchantPerfectInfo>>::iterator std::map<int, std::map<int, EnchantPerfectInfo>>::_Mybase::find(
    const int&);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnCreate(int, int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnCreate(int, int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Paint();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Paint();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnWheel(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnWheel(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearItems();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::ClearItems();
template __declspec(noinline) int CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::AddData(const EnchantUI::CEnchantItemData&, bool);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearData();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::UpdateScrollBar();
