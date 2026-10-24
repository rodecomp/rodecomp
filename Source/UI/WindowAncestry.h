#ifndef RAGNAROK_SOURCE_UI_WINDOWANCESTRY_H
#define RAGNAROK_SOURCE_UI_WINDOWANCESTRY_H

// The parent field of UIWindow at +0x10. Source pointers refer to this parent
// subobject, not binary-layout-compatible pointers into the original image.
struct WindowAncestry {
  WindowAncestry* parent;

  WindowAncestry();
  bool IsDescendantOf(const WindowAncestry* ancestor) const;
};

// Actual manager fields are noncontiguous. Keep their established locations
// explicit instead of inventing a second packed owner/global manager.
void ForgetWindowReferences(
  WindowAncestry* removed,
  WindowAncestry*& reference_019C,
  WindowAncestry*& reference_01A0,
  WindowAncestry*& reference_01B8,
  WindowAncestry*& reference_0A30,
  WindowAncestry*& reference_4F1D4
);

#endif
