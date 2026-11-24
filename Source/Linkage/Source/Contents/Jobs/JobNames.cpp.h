#pragma once

#include <map>

#include "Client/Session/Session.h"

template __declspec(noinline) std::_Tree_find_result<std::map<unsigned, const char*>::_Mybase::_Nodeptr>
std::map<unsigned, const char*>::_Mybase::_Find_lower_bound<unsigned>(const unsigned&) const;
template __declspec(noinline) std::map<unsigned, const char*>::iterator std::map<unsigned, const char*>::_Mybase::find(const unsigned&);
