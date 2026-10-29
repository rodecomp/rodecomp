#include "UI/UIBarGraph.h"

// OFFSET: 0x0089B540
// FUNCTION: fn_11567
void UIPetInfoBarGraph::SetValue(int value) {
  this->unknown_007C = value;
  this->Invalidate();
}
