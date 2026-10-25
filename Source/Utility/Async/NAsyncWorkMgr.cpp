#include "Utility/Async/NAsyncWorkMgr.h"

#include <new>

// OFFSET: 0x007313C0
// FUNCTION: fn_07547
NAsyncWorkMgr::NAsyncWorkMgr(std::uint32_t workers) : m_enabled(false), m_workerCount(workers), m_requests(nullptr), m_responses(nullptr) {
  if (this->Initialize(workers)) {
    this->m_enabled = true;
  }
}

// OFFSET: 0x00731530
// FUNCTION: fn_07549
// OFFSET: 0x007315F0
// FUNCTION: fn_07550
NAsyncWorkMgr::~NAsyncWorkMgr() {
  for (auto worker = this->m_workers.begin(); worker != this->m_workers.end(); ++worker) {
    (*worker)->Stop();
  }

  for (auto worker = this->m_workers.begin(); worker != this->m_workers.end(); ++worker) {
    delete *worker;
  }

  if (this->m_requests) {
    delete this->m_requests;
    this->m_requests = nullptr;
  }

  if (this->m_responses) {
    delete this->m_responses;
    this->m_responses = nullptr;
  }
}

// OFFSET: 0x007316D0
// FUNCTION: fn_07551
void NAsyncWorkMgr::DestroyQueues() {
  if (this->m_requests) {
    delete this->m_requests;
    this->m_requests = nullptr;
  }

  if (this->m_responses) {
    delete this->m_responses;
    this->m_responses = nullptr;
  }
}

// OFFSET: 0x00731740
// FUNCTION: fn_07552
bool NAsyncWorkMgr::Initialize(std::uint32_t workers) {
  this->m_requests = new (std::nothrow) NAsyncWorkRequestQueue;
  if (!this->m_requests) {
    return false;
  }

  if (!this->m_requests->m_enabled) {
    delete this->m_requests;
    this->m_requests = nullptr;
    return false;
  }

  this->m_responses = new (std::nothrow) NAsyncWorkResponseQueue;
  if (!this->m_responses) {
    return false;
  }

  if (!this->m_responses->m_enabled) {
    delete this->m_responses;
    this->m_responses = nullptr;
    return false;
  }

  for (std::uint32_t i = 0; i < workers; ++i) {
    NAsyncWorkThread* worker = new (std::nothrow) NAsyncWorkThread(this->m_requests, this->m_responses);
    if (!worker) {
      return false;
    }

    if (!worker->m_registered) {
      delete worker;
      return false;
    }

    this->m_workers.push_back(worker);
  }

  return true;
}

// OFFSET: 0x00731920
// FUNCTION: fn_07553
void NAsyncWorkMgr::Update(std::uint32_t maximum) {
  for (std::uint32_t i = 0; i < maximum; ++i) {
    NAsyncWorkResponse response = this->m_responses->Pop();
    if (!response.valid) {
      break;
    }

    if (response.work.Get()) {
      response.work.Get()->Complete();
    }
  }
}

// OFFSET: 0x007319D0
// FUNCTION: fn_07554
void NAsyncWorkMgr::Submit(NAsyncWorkPtr& work) {
  this->m_requests->PushBack(work);
}

// OFFSET: 0x007319E0
// FUNCTION: fn_07555
bool NAsyncWorkMgr::Start() {
  for (auto worker = this->m_workers.begin(); worker != this->m_workers.end(); ++worker) {
    if (!(*worker)->Start()) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00731A10
// FUNCTION: fn_07556
void NAsyncWorkMgr::Stop() {
  for (auto worker = this->m_workers.begin(); worker != this->m_workers.end(); ++worker) {
    (*worker)->Stop();
  }
}
