#pragma once

#include <string>
#include <vector>

template __forceinline void std::vector<std::string>::_Construct_n_copies_of_ty(const std::size_t, const std::_Value_init_tag&);
template __forceinline std::vector<std::string>::vector(const std::size_t, const std::allocator<std::string>&);
template __forceinline void std::vector<std::string>::_Buy_nonzero(const std::size_t);
template __forceinline void std::vector<std::string>::_Buy_raw(const std::size_t);
