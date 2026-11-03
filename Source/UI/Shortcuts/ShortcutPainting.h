#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTPAINTING_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTPAINTING_H

#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIWindow;
class ResourceMgr;

struct ShortcutPainting {
  RenderCommandPool& commands;
  CRenderSystemBase& system;
};

#endif
