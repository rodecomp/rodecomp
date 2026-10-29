#ifndef RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H

#include "UI/Text/UIBalloonText.h"

#include <string>

class UINameBalloonText : public UIBalloonText {
public:
  void Slot19() override;

  /* 00A0 */ std::string iconName;
};

#endif
