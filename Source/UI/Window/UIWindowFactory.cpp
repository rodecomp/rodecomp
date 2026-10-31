#include "UI/Window/UIWindowFactory.h"

#include "Contents/Configuration/ScriptOptions.h"
#include "Render/Renderer.h"
#include "Script/LuaScript.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Shortcuts/UIQuickSlotWnd.h"
#include "UI/Window/UIHotKeyWnd.h"
#include "UI/Window/UISerachStoreInfoWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <string>

// GLOBAL: gl_25184
bool g_searchStoreInfoCreated;

// OFFSET: 0x00AA1350
