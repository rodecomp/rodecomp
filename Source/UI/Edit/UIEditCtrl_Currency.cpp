#include "UI/Edit/UIEditCtrl_Currency.h"

// OFFSET: 0x0086D250
// FUNCTION: fn_11043
UIEditCtrl_Custom::UIEditCtrl_Custom()
    : UIEditCtrl2(), labelBold(0), labelOffsetX(0), labelOffsetY(0), labelAlignment(0) {
}

// OFFSET: 0x0086D1B0
// FUNCTION: fn_11042
UIEditCtrl_Currency::UIEditCtrl_Currency() : UIEditCtrl_Custom() {
  this->labelAlignment = 1;
  this->formattedLength = 0;
}
