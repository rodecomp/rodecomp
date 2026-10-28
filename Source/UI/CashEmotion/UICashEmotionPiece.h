#ifndef RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPIECE_H
#define RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONPIECE_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

struct UIButtonContext;
struct TextureNames;
class UICashEmotionSprBtn;

struct UICashEmotionPieceContext {
  UIButtonContext& buttons;
  TextureNames& textureNames;
};

class UICashEmotionPiece : public UIWindow {
public:
  UICashEmotionPiece(UIWindowContext& windows, UICashEmotionPieceContext& controls);
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot26(int x, int y) override;
  void Slot35(int argument) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Slot47() override;
  void Slot48() override;

  void Set(int packId, int emotionId, bool showTooltip, bool hoverable, bool locked);

protected:
  /* 007C */ int m_packId;
  /* 0080 */ int m_emotionId;
  /* 0084 */ UICashEmotionSprBtn* m_button;
  /* 0088 */ bool m_hoverable;
  /* 0089 */ bool m_hovered;
  UICashEmotionPieceContext* m_controls;
};

#endif
