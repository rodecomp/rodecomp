#pragma once

#include "Contents/Items/SelectPackageItemMgr.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Network/TextEncoding.h"
#include "Script/LuaInstance.h"
#include "UI/SelectPackageItemBox/SelectPackageItemBox.h"
#include "UI/Window/UIWindowMgr.h"
extern "C" {
#include <lua.h>
}
#include <cstdlib>
#include <new>
#include <windows.h>

template CSelectPackageItemMgr* Singleton<CSelectPackageItemMgr>::GetInstance();
