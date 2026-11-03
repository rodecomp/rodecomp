#ifndef RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONMINIWND_H
#define RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONMINIWND_H

#include "UI/Window/UIWindow.h"

#include <array>

class CashEmotionCommand;
class ICashEmotionAction;
class UICashEmotionPiece;
class UIBitmapButton;

class UICashEmotionMiniWnd : public UIWindow {
public:
  UICashEmotionMiniWnd();
  ~UICashEmotionMiniWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  unsigned int Select(int actionId);
  void Select(CashEmotionCommand* command);
  void Previous();
  void Refresh(int first);
  void Next();
  void SetIndex(int index);

protected:
  /* 007C */ CashEmotionCommand* m_selected;
  /* 0080 */ std::array<UICashEmotionPiece*, 3> m_pieces;
  /* 008C */ UIBitmapButton* m_previous;
  /* 0090 */ UIBitmapButton* m_next;
  /* 0094 */ ICashEmotionAction* m_action;
  /* 0098 */ int m_page;
  /* 009C */ int m_selectedIndex;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UICashEmotionMiniWnd) == 0xA0);
#endif

#endif
