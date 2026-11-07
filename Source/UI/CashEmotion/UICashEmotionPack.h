#ifndef RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPACK_H
#define RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPACK_H

#include "UI/CashEmotion/UICashEmotionPiece.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

class Timestamp;
class UIStaticText;

class UICashEmotionPack : public UIWindow {
  friend class UICashEmotionListChildWnd_Purchase;
  friend class UICashEmotionListChildWnd_UnPurchased;

public:
  UICashEmotionPack();
  ~UICashEmotionPack() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnWheel(int argument) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual int SetPack(int packId, bool showTooltip, bool hoverable);
  virtual void UpdateExpiry(Timestamp* expiry);
  virtual void ShowExpiry(__time32_t expiry);

  std::string GetPriceText(int packId);
  void SetSaleBadge(char type);
  void SetTitle(const char* title);

protected:
  /* 007C */ std::vector<UICashEmotionPiece*> m_pieces;
  /* 0088 */ UIStaticText* unknown_0088;
  /* 008C */ UIStaticText* m_titleText;
  /* 0090 */ std::string m_background;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ std::string m_title;
  /* 00C4 */ std::string m_saleBadge;
  /* 00DC */ bool m_limited;
};

#endif
