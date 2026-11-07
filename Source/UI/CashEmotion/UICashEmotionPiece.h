#ifndef RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPIECE_H
#define RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPIECE_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

class UICashEmotionSprBtn;

class UICashEmotionPiece : public UIWindow {
public:
  __declspec(noinline) UICashEmotionPiece();
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDblClk(int x, int y) override;
  void OnWheel(int argument) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void OnMouseEnter() override;
  void OnMouseLeave() override;

  void Set(int packId, int emotionId, bool showTooltip, bool hoverable, bool locked);

protected:
  /* 007C */ int m_packId;
  /* 0080 */ int m_emotionId;
  /* 0084 */ UICashEmotionSprBtn* m_button;
  /* 0088 */ bool m_hoverable;
  /* 0089 */ bool m_hovered;
};

#endif
