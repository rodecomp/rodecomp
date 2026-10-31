#ifndef RAGNAROK_SOURCE_UI_WINDOW_FUNCDRAWWNDCHAR_H
#define RAGNAROK_SOURCE_UI_WINDOW_FUNCDRAWWNDCHAR_H

#include <list>
#include <map>
#include <string>
#include <utility>

class CActRes;
class CSprRes;
class UIWindow;
struct ActionFrame;

struct FuncDrawOffset {
  // INLINED
  FuncDrawOffset() : x(0), y(0) {
  }

  /* 0000 */ int x;
  /* 0004 */ int y;
};

// VTABLE: vt_00803
class FuncDrawWNDChar {
public:
  FuncDrawWNDChar(
    UIWindow* window, int x, int y, unsigned char sex, short job, int hairColor, short body, short head, short bottomAccessory, short topAccessory,
    short midAccessory, int robe, unsigned char flags, int shield, int bodyStyle, int headPalette, int action, int motion, int time
  );
  virtual ~FuncDrawWNDChar();
  void Draw(int option);
  bool DrawOnTop();
  std::list<int> DrawOrder();
  int IsSpecialLayer(int layer);
  FuncDrawOffset BodyAnchorOffset(const ActionFrame* frame, bool keepScale);
  FuncDrawOffset LayerAnchorOffset(int layer, const ActionFrame** frame, CActRes* act);

  /* 0004 */ UIWindow* m_window;
  /* 0008 */ int m_x;
  /* 000C */ int m_y;
  /* 0010 */ unsigned char m_sex;
  /* 0012 */ short m_job;
  /* 0014 */ int m_hairColor;
  /* 0018 */ short m_body;
  /* 001A */ short m_head;
  /* 001C */ short m_bottomAccessory;
  /* 001E */ short m_topAccessory;
  /* 0020 */ short m_midAccessory;
  /* 0024 */ int m_robe;
  /* 0028 */ unsigned char m_flags;
  /* 002C */ int m_shield;
  /* 0030 */ int m_bodyStyle;
  /* 0034 */ int m_headPalette;
  /* 0038 */ int m_action;
  /* 003C */ int m_motion;
  /* 0040 */ int m_time;
  /* 0044 */ bool m_helmRobe;
  /* 0045 */ bool m_drawOnTop;
  /* 0048 */ std::map<int, std::pair<CSprRes*, CActRes*>> m_resources;
  /* 0050 */ std::string unknown_0050;
  /* 0068 */ std::string unknown_0068;
  /* 0080 */ std::map<int, int> m_layers;
  /* 0088 */ unsigned char unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ unsigned char unknown_0094;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FuncDrawWNDChar) == 152);
#endif

#endif
