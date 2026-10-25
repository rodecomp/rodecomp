#include "Utility/Async/NAsyncWorkThread.h"

// OFFSET: 0x00732980
// FUNCTION: fn_07576
NAsyncWorkThread::NAsyncWorkThread(NAsyncWorkRequestQueue* requests, NAsyncWorkResponseQueue* responses)
    : NThread("AsyncWorkThread"), m_registered(false), m_requests(requests), m_responses(responses) {
  g_messageMgr->Register(this->m_objectId, static_cast<NInter*>(this));
  this->m_registered = true;
}

// OFFSET: 0x00732A80
// FUNCTION: fn_07577
// OFFSET: 0x00732B40
// FUNCTION: fn_07581
NAsyncWorkThread::~NAsyncWorkThread() {
  g_messageMgr->Unregister(this->m_objectId, static_cast<NInter*>(this));
}

// OFFSET: 0x00732C40
// FUNCTION: fn_07584
void NAsyncWorkThread::Receive(std::uint64_t argument, std::uint32_t id) {
  this->m_messages.Push(argument, id);
}

// OFFSET: 0x00732C20
// FUNCTION: fn_07583
void NAsyncWorkThread::RequestStop() {
  this->m_messages.Push(0x50000, 0);
}

// OFFSET: 0x00732C50
// FUNCTION: fn_07585
bool NAsyncWorkThread::Start() {
  return this->NThread::Start();
}

// OFFSET: 0x00732C60
// FUNCTION: fn_07586
void NAsyncWorkThread::Stop() {
  this->NThread::Stop();
}

// OFFSET: 0x00732C70
// FUNCTION: fn_07587
unsigned NAsyncWorkThread::Run() {
  for (;;) {
    HANDLE handles[2] = {this->m_messages.m_semaphore, this->m_requests->m_semaphore};
    DWORD ready = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    if (ready == WAIT_OBJECT_0) {
      NMessage message;
      this->m_messages.Pop(&message);

      if (message.valid && message.argumentLow == 0x50000) {
        return 0;
      }
    } else if (ready == WAIT_OBJECT_0 + 1) {
      NAsyncWorkPtr work = this->m_requests->Pop();
      if (work.Get()) {
        auto group = work.Get()->GroupKey();
        if (work.Get()->Execute()) {
          this->m_requests->PushFront(work);
        } else {
          this->m_responses->Push(work);
        }

        this->m_requests->Complete(group.first, group.second);
      }
    }
  }
}
