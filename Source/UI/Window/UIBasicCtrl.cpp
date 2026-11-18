#include "UI/Window/UIBasicCtrl.h"

#include "Client/Session/Session.h"

// OFFSET: 0x0086C410
// FUNCTION: fn_11026
UIBasicItemCtrl::UIBasicItemCtrl()
    : UIWindow(0), m_itemType(-1), m_itemMask(-1), m_identified(0), m_tooltip(""), m_item(0) {
}

// OFFSET: 0x0086C4C0
// FUNCTION: fn_11027
UIBasicSkillCtrl::UIBasicSkillCtrl()
    : UIWindow(0), m_skillId(-1), m_level(0) {
}

// OFFSET: 0x008701A0
// FUNCTION: fn_45652
UIBasicItemCtrl::~UIBasicItemCtrl() = default;

// OFFSET: 0x00870230
// FUNCTION: fn_45653
UIBasicSkillCtrl::~UIBasicSkillCtrl() = default;

// OFFSET: 0x0089BE60
// FUNCTION: fn_45638
void UIBasicItemCtrl::SetItemById(int id, unsigned char identified) {
  this->m_item = g_session.LookupClientItem(id, 0);
  this->m_item.identified = identified;
  if (this->m_item.identity[1] <= 0) {
    this->m_item.SetItemId(id);
  }
  this->Invalidate();
}

// OFFSET: 0x0089BF30
// FUNCTION: fn_45639
void UIBasicItemCtrl::SetItemByIdentity(std::uint32_t identity, unsigned char identified) {
  this->m_item = g_session.LookupClientItemByIdentity(identity);
  this->m_item.identified = identified;
  this->Invalidate();
}
