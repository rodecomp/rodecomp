#pragma once

#include <list>

class UIWindow;

extern template std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);

#include <map>

struct SavedWindowRectangle;

template __declspec(noinline) std::_Tree_find_result<std::map<int, SavedWindowRectangle*>::_Mybase::_Nodeptr>
std::map<int, SavedWindowRectangle*>::_Mybase::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow*>(std::_List_node<UIWindow*, void*>* const, UIWindow*&&);
