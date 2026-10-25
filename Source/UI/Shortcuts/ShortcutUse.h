#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTUSE_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTUSE_H

#include "Client/ClientOptions.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/UserData/NormalItemSlots.h"

class ModeEvents;
class ButtonServices;

struct ShortcutUse {
  ClientSkills& skills;
  const std::list<ItemInfo>& inventory;
  ItemNaming& naming;
  ClientOptions& options;
  ModeEvents*& mode;
  ButtonServices& dialogs;
  const int& itemConfirmationState;

  void Use(const NormalItemSlot& slot);
};

#endif
