#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWANCESTRY_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWANCESTRY_H

struct WindowAncestry {
  WindowAncestry* parent;

  WindowAncestry();
  bool IsDescendantOf(const WindowAncestry* ancestor) const;
};

void ForgetWindowReferences(
  WindowAncestry* removed, WindowAncestry*& reference_019C, WindowAncestry*& reference_01A0, WindowAncestry*& reference_01B8, WindowAncestry*& reference_0A30,
  WindowAncestry*& reference_4F1D4
);

#endif
