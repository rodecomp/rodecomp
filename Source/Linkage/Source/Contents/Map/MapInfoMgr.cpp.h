#pragma once

#include "Contents/Map/MapInfoMgr.h"

template __forceinline MapInfo& std::map<std::string, MapInfo>::operator[](const std::string&);
