#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_SPRITEDRAWINGPOLICY_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_SPRITEDRAWINGPOLICY_H

class LuaScript;

struct SpriteDrawingPolicy {
  LuaScript& script;
  const unsigned char& newDrawOnTop;
  int& argument00BC;
  const int& argument0018;
  const int& argument0010;
  const int& argument0040;
  const int& argument0044;
  bool& helmRobe;

  bool DrawOnTop() const;
  bool SetRobe(int robe);
};

#endif
