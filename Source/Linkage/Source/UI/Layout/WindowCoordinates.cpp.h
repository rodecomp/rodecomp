#pragma once

#include "UI/Window/UIWindow.h"
#include "UI/Layout/WindowCoordinates.h"
#include <cstdint>

template std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);
