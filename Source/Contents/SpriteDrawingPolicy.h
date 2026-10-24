#ifndef RAGNAROK_SOURCE_CONTENTS_SPRITEDRAWINGPOLICY_H
#define RAGNAROK_SOURCE_CONTENTS_SPRITEDRAWINGPOLICY_H

class LuaScript;

// Borrowed sprite-compositor fields. Arguments retain the original script
// order: +0xBC, +0x18, +0x10, +0x40, +0x44. No result is cached here.
struct SpriteDrawingPolicy {
  LuaScript& script;
  const unsigned char& newDrawOnTop;
  int& argument00BC;
  const int& argument0018;
  const int& argument0010;
  const int& argument0040;
  const int& argument0044;
  bool& helmRobe;

  // Only mode byte 1 selects _New_DrawOnTop; other values use DrawOnTop.
  // The original initializes the result true and ignores the call status.
  bool DrawOnTop() const;
  // Publishes +0xBC before querying the actual TB_Layer_Priority table.
  // A failed call retains the prior +0xC0 helm-robe flag.
  bool SetRobe(int robe);
};

#endif
