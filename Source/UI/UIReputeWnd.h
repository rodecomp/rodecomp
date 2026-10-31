#ifndef RAGNAROK_SOURCE_UI_UIREPUTEWND_H
#define RAGNAROK_SOURCE_UI_UIREPUTEWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/ReputeMgr.h"

#include <cstdint>
#include <string>
#include <vector>

struct ReputeInfo;
class UIBitmapButton;
class UIComboBoxDefWnd;
class UIEditCtrl;
struct UIButtonContext;
struct UIComboBoxHeaderContext;
struct UIEditContext;

class UIReputeWnd : public UIFrameWnd {
public:
  UIReputeWnd(UIWindowContext& windows, UIFrameContext& frames);
  bool Slot02() override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void FilterByGroup(const std::string& group);
  void SetPage(unsigned int page);
  void ClearUpdated();

  UIComboBoxDefWnd* m_groupCombo;
  UIEditCtrl* m_searchEdit;
  UIBitmapButton* m_prevButton;
  UIBitmapButton* m_nextButton;
  unsigned int m_perPage;
  unsigned int m_page;
  unsigned int m_pageCount;
  bool m_groupSelected;
  std::int64_t m_totalPoints;
  std::vector<const ReputeInfo*> m_reputes;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
  UIComboBoxHeaderContext* m_comboContext;
};

#include "Contents/ReputeMgrLoad.inl"

#endif

