#include "UI/Quest/UIRecommendedQuestInfo.h"

#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "UI/Quest/UIRenewQuestWnd.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>

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

// OFFSET: 0x00A570F0
// FUNCTION: fn_16323
void CUIRecommendedQuestInfo::Slot15(int width, int height) {
  this->titleBar = new CUITitleBar(*this->m_context);
  this->titleBar->SetSize(342, 17);
  this->titleBar->Move(0, 0);
  this->titleBar->SetImages(
    std::string("유저인터페이스\\basic_interface\\titlebar_left.bmp"), std::string("유저인터페이스\\basic_interface\\titlebar_mid.bmp"),
    std::string("유저인터페이스\\basic_interface\\titlebar_right.bmp")
  );
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText("QUEST", 5);
  title->SetFitText(0);
  title->SetAlign(0, 6);
  title->SetFont(std::string("Gulim"));
  title->SetFontHeight(13);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(0, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* firstButton = new CUIImageButton(*this->m_context);
  firstButton->SetSize(11, 11);
  firstButton->Move(3, 3);
  firstButton->SetOnClick([this]() { this->OnFirstAction(); });
  firstButton->disabled = 0;
  firstButton->Invalidate();
  firstButton->SetImages(
    std::string("유저인터페이스\\basic_interface\\sys_base_off.bmp"), std::string("유저인터페이스\\basic_interface\\sys_base_on.bmp"),
    std::string("유저인터페이스\\basic_interface\\sys_base_on.bmp"), std::string("유저인터페이스\\basic_interface\\sys_base_off.bmp")
  );
  this->titleBar->AddChild(firstButton);

  this->background = new CUIStaticImage(*this->m_context);
  this->background->SetSize(342, 395);
  this->background->Move(0, 17);
  this->background->SetImage(std::string("유저인터페이스\\Renew_QuestUI\\bg_detail_recommended.bmp"));
  this->AddChild(this->background);

  this->titleText = new CUIStaticText(*this->m_context);
  this->titleText->SetSize(342, 12);
  this->titleText->Move(0, 39);
  this->titleText->SetText("", 0);
  this->titleText->SetFitText(0);
  this->titleText->SetAlign(6, 0);
  this->titleText->SetFont(std::string("Gulim"));
  this->titleText->SetFontHeight(12);
  this->titleText->SetBold(1);
  this->titleText->SetColor(0x55321D);
  this->titleText->SetShadow(0, 0xFFFFFF);
  this->titleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->titleText);

  this->scrollWindow = new RecommendedQuestInfo::CUIScrollWindow(*this->m_context);
  this->scrollWindow->SetSize(342, 322);
  this->scrollWindow->Move(0, 62);
  this->AddChild(this->scrollWindow);

  this->closeButton = new CUIImageButton(*this->m_context);
  this->closeButton->SetSize(42, 20);
  this->closeButton->Move(288, 388);
  this->closeButton->SetOnClick([this]() { this->OnClose(); });
  this->closeButton->disabled = 0;
  this->closeButton->Invalidate();
  this->closeButton->SetImages(
    std::string("유저인터페이스\\basic_interface\\btn_close.bmp"), std::string("유저인터페이스\\basic_interface\\btn_close_b.bmp"),
    std::string("유저인터페이스\\basic_interface\\btn_close_a.bmp"), std::string("유저인터페이스\\basic_interface\\btn_close.bmp")
  );
  this->AddChild(this->closeButton);
}

// OFFSET: 0x00A58BA0
// FUNCTION: fn_16346
void CUIRecommendedQuestInfo::OnFirstAction() {
}

// OFFSET: 0x00A58BB0
// FUNCTION: fn_45469
void CUIRecommendedQuestInfo::OnClose() {
  g_windowMgr.CloseWindow(0x271A);
}

// OFFSET: 0x00A58BE0
// FUNCTION: fn_16349
void CUIRecommendedQuestInfo::Slot16() {
  const char* title = LookupMessage(0xD48);
  this->titleBar->SetTitle(title, std::strlen(title));
  UIWindow* window = g_windowMgr.LookupWindow(0x2718);
  if (window) {
    this->Move(window->geometry.x + window->geometry.width, window->geometry.y);
  } else {
    this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  }
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

// OFFSET: 0x00A57EF0
// FUNCTION: fn_16324
void CUIScrollWindow::Slot15(int width, int height) {
  this->background = new CUIStaticImage(*this->m_context);
  this->background->SetSize(336, 318);
  this->background->Move(3, 2);
  this->background->SetImage(std::string());
  this->AddChild(this->background);

  this->topImage = new CUIStaticImage(*this->m_context);
  this->topImage->SetSize(324, 138);
  this->topImage->Move(9, 8);
  this->topImage->SetImage("유저인터페이스\\Renew_QuestUI\\bg_quest01.png");
  this->AddChild(this->topImage);

  this->bottomImage = new CUIStaticImage(*this->m_context);
  this->bottomImage->SetSize(208, 92);
  this->bottomImage->Move(9, 221);
  this->bottomImage->SetImage("유저인터페이스\\Renew_QuestUI\\bg_quest02.png");
  this->AddChild(this->bottomImage);

  for (int count = 3; count != 0; --count) {
    CUIQuestInfo* info = new CUIQuestInfo(*this->m_context);
    info->SetSize(300, 150);
    this->AddChild(info);
    this->questInfos.push_back(info);
  }

  this->questInfos[0]->Resize(306, 120);
  this->questInfos[0]->Move(18, 17);
  this->questInfos[0]->textViewer->Resize(306, 120);
  this->questInfos[0]->textViewer->SetScrollBarSize(7, 120);

  this->questInfos[1]->Resize(213, 60);
  this->questInfos[1]->Move(16, 153);
  this->questInfos[1]->textViewer->Resize(213, 60);
  this->questInfos[1]->textViewer->SetTextOffset(0, 6);
  this->questInfos[1]->textViewer->SetBold(true);
  this->questInfos[1]->textViewer->SetTextColor(0x2A2A2A);
  this->questInfos[1]->textViewer->SetOutline(true, 0xFFFFFF);
  this->questInfos[1]->textViewer->SetScrollBarSize(7, 60);

  this->questInfos[2]->Resize(182, 76);
  this->questInfos[2]->Move(22, 229);
  this->questInfos[2]->textViewer->Resize(182, 76);
  this->questInfos[2]->textViewer->SetScrollBarSize(7, 76);
}

// OFFSET: 0x00A58C70
// FUNCTION: fn_16350
void CUIScrollWindow::Slot16() {
  this->npcSprite = new UINpcSprWnd(*this->m_context);
  this->npcSprite->SetSize(75, 145);
  this->npcSprite->Show(0);
  this->AddChild(this->npcSprite);
}

}

namespace RecommendedQuestInfo {

// OFFSET: 0x00A58640
// FUNCTION: fn_16339
CUIQuestInfo::CUIQuestInfo(UIWindowContext& context) : CUIControl(context), textViewer(nullptr) {
}

// OFFSET: 0x00A587C0
// FUNCTION: fn_16341
CUIScrollWindow::CUIScrollWindow(UIWindowContext& context)
    : CUIControl(context), background(nullptr), topImage(nullptr), bottomImage(nullptr), questInfos(), npcSprite(nullptr) {
}

}

// OFFSET: 0x00A586C0
// FUNCTION: fn_16340
CUIRecommendedQuestInfo::CUIRecommendedQuestInfo()
    : titleBar(nullptr), background(nullptr), titleText(nullptr), scrollWindow(nullptr), closeButton(nullptr) {
}

// OFFSET: 0x00A56860
// FUNCTION: fn_16320
CUIFrameWnd* CreateRecommendedQuestInfo() {
  return new CUIRecommendedQuestInfo();
}
