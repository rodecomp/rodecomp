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

public:
  explicit UICashEmotionPack(UIWindowContext& windows);
  ~UICashEmotionPack() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot35(int argument) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual int Slot53(int packId, bool showTooltip, bool hoverable) = 0;
  virtual void UpdateExpiry(Timestamp* expiry);
  virtual void ShowExpiry(__time32_t expiry);

  std::string GetPriceText(int packId);
  void SetSaleBadge(char type);
  void SetTitle(const char* title);

protected:
  /* 007C */ std::vector<UICashEmotionPiece*> m_pieces;
  /* 0088 */ UIWindow* unknown_0088;
  /* 008C */ UIStaticText* m_titleText;
  /* 0090 */ std::string m_background;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ std::string m_title;
  /* 00C4 */ std::string m_saleBadge;
  /* 00DC */ bool m_limited;
};

#endif
