#pragma once

#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

template __declspec(noinline) void std::deque<std::uint32_t>::pop_front() noexcept;
template __declspec(noinline) std::map<std::uint32_t, CPlayerNameDictionary::Entry>::_Mybase::iterator
std::map<std::uint32_t, CPlayerNameDictionary::Entry>::_Mybase::find(const std::uint32_t&);
