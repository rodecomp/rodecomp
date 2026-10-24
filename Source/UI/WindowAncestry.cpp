#include "UI/WindowAncestry.h"

// OFFSET: 0x00A79A90
WindowAncestry::WindowAncestry() : parent(nullptr) {
}

// OFFSET: 0x00A81C90
// FUNCTION: fn_16985
bool WindowAncestry::IsDescendantOf(const WindowAncestry* ancestor) const {
  for (const WindowAncestry* node = this->parent; node; node = node->parent) {
    if (node == ancestor) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00A8C200
// FUNCTION: fn_17130
void ForgetWindowReferences(
  WindowAncestry* removed,
  WindowAncestry*& reference_019C,
  WindowAncestry*& reference_01A0,
  WindowAncestry*& reference_01B8,
  WindowAncestry*& reference_0A30,
  WindowAncestry*& reference_4F1D4
) {
  if (reference_019C && (reference_019C == removed || reference_019C->IsDescendantOf(removed))) {
    reference_019C = nullptr;
  }
  if (reference_01A0 && (reference_01A0 == removed || reference_01A0->IsDescendantOf(removed))) {
    reference_01A0 = nullptr;
  }
  if (reference_01B8 && (reference_01B8 == removed || reference_01B8->IsDescendantOf(removed))) {
    reference_01B8 = nullptr;
  }
  if (reference_0A30 && reference_0A30 == removed) {
    reference_0A30 = nullptr;
  }
  if (reference_4F1D4 && (reference_4F1D4 == removed || reference_4F1D4->IsDescendantOf(removed))) {
    reference_4F1D4 = nullptr;
  }
}
