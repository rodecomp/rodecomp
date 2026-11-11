#pragma once

#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Registry.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Textures/Surface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Login/UIEmailAddressWnd.h"
#include "UI/Navigation/UIMemorialDunWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Dialogs/UIMessageBox.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Events/UIRoulletteWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <windows.h>

extern template std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, UIWindow*>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, UIWindow*>>>, std::_Iterator_base0>::operator++() noexcept;
extern template std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);
