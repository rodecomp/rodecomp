#include "UI/Edit/UIEditCtrl_Currency.h"

// OFFSET: 0x0086D250
// FUNCTION: fn_11043
UIEditCtrl_Custom::UIEditCtrl_Custom()
    : UIEditCtrl2(), unknown_011C(0), unknown_0120(0), unknown_0124(0), unknown_0128(0) {
}

// OFFSET: 0x0086D1B0
// FUNCTION: fn_11042
UIEditCtrl_Currency::UIEditCtrl_Currency() : UIEditCtrl_Custom() {
  this->unknown_0128 = 1;
  this->formattedLength = 0;
}
