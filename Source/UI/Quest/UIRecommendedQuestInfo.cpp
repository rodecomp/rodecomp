#include "UI/Quest/UIRecommendedQuestInfo.h"

// OFFSET: 0x00A568E0
// FUNCTION: fn_16321
void CUIRecommendedQuestInfo::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(342, 412);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A58BA0
// FUNCTION: fn_16346
void CUIRecommendedQuestInfo::OnFirstAction() {
}

namespace RecommendedQuestInfo {

// OFFSET: 0x00A58BC0
// FUNCTION: fn_16347
void CUIQuestInfo::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A58BD0
// FUNCTION: fn_16348
void CUIQuestInfo::Slot16() {
}

// OFFSET: 0x00A569F0
// FUNCTION: fn_16322
void CUIQuestInfo::Slot15(int width, int height) {
  this->textViewer = new CUITextViewer(*this->m_context);
  this->textViewer->SetSize(300, 150);
  this->textViewer->Move(0, 0);
  this->textViewer->SetTextOffset(0, 0);
  this->textViewer->SetFont("Gulim");
  this->textViewer->SetFontHeight(12);
  this->textViewer->SetBold(0);
  this->textViewer->SetTextColor(0);
  this->textViewer->SetOutline(0, 0xFFFFFF);
  this->textViewer->SetLineSpacing(6);
  this->textViewer->SetBackgroundColor(0xFFFF00FF);
  this->textViewer->SetPadding(0, 0, 0, 0);
  this->textViewer->SetScrollBarSize(7, 150);
  this->textViewer->SetScrollStep(15);
  this->textViewer->SetScrollBarButtonImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_upper.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_down.bmp"
  );
  this->textViewer->SetScrollBarBackground("");
  this->textViewer->SetScrollBarThumbImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_up.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_mid.bmp",
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_down.bmp"
  );
  this->AddChild(this->textViewer);
}

}
