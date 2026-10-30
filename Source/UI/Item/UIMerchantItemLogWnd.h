#ifndef RAGNAROK_SOURCE_UI_ITEM_UIMERCHANTITEMLOGWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIMERCHANTITEMLOGWND_H

#include "UI/Item/UIItemBaseWnd.h"

#include <ctime>
#include <cstdint>
#include <string>
#include <vector>

class UIMerchantItemLogWnd : public UIItemBaseWnd {
public:
  // VTABLE: vt_01196
  class lcData {
  public:
    // INLINED
    virtual ~lcData() {
    }

    /* 0004 */ std::string name;
    /* 001C */ std::string unknown_001C;
    /* 0034 */ __time32_t time;
    /* 0038 */ std::uint32_t characterId;
  };

  void Slot19() override;

  /* 00F0 */ std::vector<lcData> logs;
  /* 00FC */ int unknown_00FC;
};

#endif
