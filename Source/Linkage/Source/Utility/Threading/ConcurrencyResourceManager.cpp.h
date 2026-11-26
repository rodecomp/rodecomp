#pragma once

#include "Utility/Threading/ConcurrencyResourceManager.h"

#include <map>

using ResourceMap = std::map<unsigned int, Concurrency::details::Resource*>;
using ResourceMapNode = std::_Tree_node<ResourceMap::value_type, void*>;
using ResourceMapVal = std::_Tree_val<std::_Tree_simple_types<ResourceMap::value_type>>;

// OFFSET: 0x00E88130
// FUNCTION: fn_25446
template __declspec(noinline) Concurrency::details::Resource*& ResourceMap::operator[](const unsigned int&);
// OFFSET: 0x00E91CE0
// FUNCTION: fn_25527
template __declspec(noinline) ResourceMapNode* ResourceMapVal::_Insert_node(const std::_Tree_id<ResourceMapNode*>, ResourceMapNode* const) noexcept;
// OFFSET: 0x00E88210
// FUNCTION: fn_25447
template __declspec(noinline) std::_Tree_unchecked_const_iterator<ResourceMapVal, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<ResourceMapVal, std::_Iterator_base0>::operator++() noexcept;
