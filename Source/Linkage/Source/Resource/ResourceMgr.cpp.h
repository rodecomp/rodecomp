#pragma once

#include "Resource/ResourceMgr.h"
#include <map>
#include <utility>

template __forceinline std::pair<std::map<const ResourceName*, CRes*, ResourceNameLess>::iterator, bool> std::map<const ResourceName*, CRes*, ResourceNameLess>::_Mybase::insert<0, 0>(std::pair<const ResourceName* const, CRes*>&&);
template __forceinline std::pair<std::map<const ResourceName*, CRes*, ResourceNameLess>::_Mybase::_Nodeptr, bool> std::map<const ResourceName*, CRes*, ResourceNameLess>::_Mybase::_Emplace<std::pair<const ResourceName* const, CRes*>>(std::pair<const ResourceName* const, CRes*>&&);
template __forceinline std::_Tree_temp_node<std::map<const ResourceName*, CRes*, ResourceNameLess>::_Mybase::_Alnode>::_Tree_temp_node(std::map<const ResourceName*, CRes*, ResourceNameLess>::_Mybase::_Alnode&, std::map<const ResourceName*, CRes*, ResourceNameLess>::_Mybase::_Nodeptr, std::pair<const ResourceName* const, CRes*>&&);
