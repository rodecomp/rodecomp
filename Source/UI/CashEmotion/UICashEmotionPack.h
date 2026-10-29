#ifndef RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPACK_H
#define RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPACK_H

#include "UI/CashEmotion/UICashEmotionPiece.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>
#include <vector>

class UICashEmotionPack : public UIWindow {
public:
  ~UICashEmotionPack() override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot35(int argument) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void SetSaleBadge(char type);
  void SetTitle(const char* title);

protected:
  /* 007C */ std::vector<UICashEmotionPiece*> m_pieces;
  /* 0088 */ UIWindow* unknown_0088;
  /* 008C */ UIWindow* m_titleText;
  /* 0090 */ std::string m_background;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ std::string m_title;
  /* 00C4 */ std::string m_saleBadge;
  /* 00DC */ bool m_limited;
};

#endif
