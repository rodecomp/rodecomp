#pragma once

#include "Client/Mode/CMode.h"
#include "Client/Session/Session.h"
#include "UI/Window/UIControl.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Lists/UIThumb.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Mode/ModeMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIPadding.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Lists/UIVScrollBar.h"
#include "Render/Text/FontCache.h"
#include "Script/LuaScript.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Client/Input/InputCharset.h"
#include "Render/Text/TextContext.h"
#include "File/File.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Utility/Text/Base64.h"
#include "Utility/Threading/NSyncObject.h"
#include "Render/Renderer.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Window/FrameSkin.h"
#include "UI/Text/CreateLineInfo.h"
#include <algorithm>
#include <cmath>
#include <new>
#include <format>
#include <windows.h>

template __declspec(noinline) char* std::allocator<char>::allocate(const std::size_t);
template __declspec(noinline) std::function<void(CUIListItem*)>& std::function<void(CUIListItem*)>::operator=(const std::function<void(CUIListItem*)>&);
template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char* const, std::size_t, const char* const, std::size_t) noexcept;
template __declspec(noinline) std::map<int, CResPtr<CBitmapRes>>::const_iterator std::map<int, CResPtr<CBitmapRes>>::_Mybase::find(const int&) const;
template void CUIListBox<AdventureGuide::CUITab>::Resize(int, int);
template void CUIListBox<AdventureGuide::CUISubTab>::Resize(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::OnScroll(int);
template void CUIListBox<AdventureGuide::CUISubTab>::Paint();
template void CUIListBox<AdventureGuide::CUITab>::Paint();
template void CUIListBox<AdventureGuide::CUISubTab>::OnWheel(int);
template void CUIListBox<AdventureGuide::CUITab>::OnWheel(int);
template CUIRegisterMgr* Singleton<CUIRegisterMgr>::GetInstance();
