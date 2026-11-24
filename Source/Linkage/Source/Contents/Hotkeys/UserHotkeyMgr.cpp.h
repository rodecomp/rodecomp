#pragma once

#include "Contents/Hotkeys/UserHotkeyMgr.h"

#include <list>
#include <string>

template std::basic_string<char>& std::basic_string<char>::operator=(std::basic_string<char>&&) noexcept;
template __forceinline std::basic_string<char>::basic_string(const std::basic_string<char>&);
template __declspec(noinline) std::list<CUserHotkeyMgr::Hotkey>::list(const std::list<CUserHotkeyMgr::Hotkey>&);
