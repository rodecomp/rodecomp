#ifndef RAGNAROK_SOURCE_UI_SHORTCUTUSE_H
#define RAGNAROK_SOURCE_UI_SHORTCUTUSE_H

#include "Client/ClientOptions.h"
#include "Contents/ClientSkills.h"
#include "Contents/ItemInfo.h"
#include "Contents/ItemNaming.h"
#include "Contents/NormalItemSlots.h"

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
