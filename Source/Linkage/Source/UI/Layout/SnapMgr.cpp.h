#pragma once

#include "UI/Layout/SnapMgr.h"
#include "UI/Layout/SnapObject.h"
#include "UI/Window/UIWindow.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"
#include <cstdlib>

template __declspec(noinline) void std::list<SnapObject*>::push_back(SnapObject* const&);
