#include "UI/Item/UIMerchantItemLogWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"

// OFFSET: 0x0099EF50
// FUNCTION: fn_13578
void UIMerchantItemLogWnd::Slot19() {
  bool changed = false;
  for (auto it = this->logs.begin(); it != this->logs.end(); ++it) {
    if (it->name.compare(LookupMessage(581)) == 0) {
      it->name = LookupPlayerName(it->characterId);
      changed = true;
    }
  }

  if (changed) {
    this->Invalidate();
  }
}
