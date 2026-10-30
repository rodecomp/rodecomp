#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/ReplayRecordOption.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A81C90
// FUNCTION: fn_16985
bool UIWindow::IsDescendantOf(const UIWindow* ancestor) const {
  for (const UIWindow* node = this->parent; node; node = node->parent) {
    if (node == ancestor) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00A8C200
// FUNCTION: fn_17130
void UIWindowMgr::ForgetWindowReferences(UIWindow* removed) {
  if (this->inputWindow && (this->inputWindow == removed || this->inputWindow->IsDescendantOf(removed))) {
    this->inputWindow = nullptr;
  }

  if (this->hoverWindow && (this->hoverWindow == removed || this->hoverWindow->IsDescendantOf(removed))) {
    this->hoverWindow = nullptr;
  }

  if (this->window_01B8 && (this->window_01B8 == removed || this->window_01B8->IsDescendantOf(removed))) {
    this->window_01B8 = nullptr;
  }

  if (this->window_0A30 && this->window_0A30 == removed) {
    this->window_0A30 = nullptr;
  }

  if (this->window_4F1D4 && (this->window_4F1D4 == removed || this->window_4F1D4->IsDescendantOf(removed))) {
    this->window_4F1D4 = nullptr;
  }
}

// OFFSET: 0x00AA8F30
// FUNCTION: fn_17250
void UIWindowMgr::ClearWindow01B8() {
  this->window_01B8 = nullptr;
}

// OFFSET: 0x00AA8110
// FUNCTION: fn_17241
void UIWindowMgr::CloseWindowExcept251(int id) {
  if (id != 251) {
    this->CloseWindow(id);
  }
}

// OFFSET: 0x00AA8A90
// FUNCTION: fn_17246
void UIWindowMgr::RecordWindowPacket(int argument) {
  if (Replay::Instance().state == 2 &&
      Replay::Instance().header.options.Get(ReplayRecordOption::Item) &&
      Replay::Instance().header.options.Get(ReplayRecordOption::BasicInfo) &&
      Replay::Instance().header.options.Get(ReplayRecordOption::Status)) {
#pragma pack(push, 1)
    struct {
      std::uint32_t header;
      std::uint16_t length;
      std::uint32_t value;
    } packet;
#pragma pack(pop)
    static_assert(sizeof(packet) == 10);
    packet.header = 0x038D08AE;
    packet.length = 10;
    packet.value = static_cast<std::uint32_t>(argument);
    reinterpret_cast<ReplayPacketRecorder*>(&Replay::Instance())->Record(10, &packet, 42, 0);
  }
}

// OFFSET: 0x00AA8F40
// FUNCTION: fn_17251
void UIWindowMgr::Nullsub370() {
}

// OFFSET: 0x00A7AA30
// FUNCTION: fn_16924
void UIWindow::PropagateMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (this != sender && !this->SendMsg(sender, message, argument1, argument2, argument3, argument4)) {
    for (UIWindow* child : this->m_children) {
      child->PropagateMsg(sender, message, argument1, argument2, argument3, argument4);
    }
  }
}

// OFFSET: 0x00A81D70
// FUNCTION: fn_16989
void UIWindow::BroadcastMsg4() {
  this->SendMsg(nullptr, 4, 0, 0, 0, 0);
  for (std::list<UIWindow*>::iterator it = this->m_children.begin(); it != this->m_children.end(); ++it) {
    if (*it) {
      (*it)->SendMsg(nullptr, 4, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00A81DC0
// FUNCTION: fn_16990
void UIWindow::BroadcastMsg3() {
  this->SendMsg(nullptr, 3, 0, 0, 0, 0);
  for (std::list<UIWindow*>::iterator it = this->m_children.begin(); it != this->m_children.end(); ++it) {
    if (*it) {
      (*it)->SendMsg(nullptr, 3, 0, 0, 0, 0);
    }
  }
}
