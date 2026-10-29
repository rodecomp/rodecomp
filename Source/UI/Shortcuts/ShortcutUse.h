#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTUSE_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTUSE_H

#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/UserData/NormalItemSlots.h"

class ModeEvents;
class ButtonServices;

struct ShortcutUse {
  ClientSkills& skills;
  const std::list<ITEM_INFO>& inventory;
  ItemNaming& naming;
  ModeEvents*& mode;
  ButtonServices& dialogs;
  const int& itemConfirmationState;

  void Use(const NormalItemSlot& slot);
};

#endif
