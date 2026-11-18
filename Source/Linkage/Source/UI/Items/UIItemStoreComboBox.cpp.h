#pragma once

#include <cstddef>
#include <string>
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Items/UIItemStoreComboBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIWindow.h"
#include <cstdio>

extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
