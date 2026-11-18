#pragma once

#include <list>
#include "UI/Window/UIEnums.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Render/Textures/Surface.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Dialogs/UIChooseWnd.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Items/UIMakeTargetListWnd.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include "UI/Login/UIChangeNameWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Login/UISelCharForUServerWnd.h"
#include "UI/Login/UIWaitDBSaveWnd.h"
#include "UI/Navigation/UIChooseWarpWnd.h"
#include <algorithm>

class UIWindow;

extern template std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);
template __declspec(noinline) std::_Tree_find_result<std::map<UIWindow*, SnapRectangle>::_Mybase::_Nodeptr> std::map<UIWindow*, SnapRectangle>::_Mybase::_Find_lower_bound<UIWindow*>(UIWindow* const&) const;
