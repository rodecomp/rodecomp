#pragma once

#include "Contents/Hotkeys/HotkeyInfo.h"

#include <map>

template __declspec(noinline) HotkeyInfo& std::map<int, HotkeyInfo>::operator[](const int&);
