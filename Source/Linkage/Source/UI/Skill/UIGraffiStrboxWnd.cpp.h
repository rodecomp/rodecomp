#pragma once

#include <cstddef>
#include <string>
#include "UI/Skill/UIGraffiStrboxWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);

template std::basic_string<char>& std::basic_string<char>::operator=(std::basic_string<char>&&) noexcept;
