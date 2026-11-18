#pragma once

#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"
#include "UI/Window/UIWindow.h"
#include <algorithm>
#include <cstring>
#include <vector>
#include <windows.h>
#include <timeapi.h>

template __declspec(noinline) void std::vector<unsigned char>::_Buy_raw(size_type);
