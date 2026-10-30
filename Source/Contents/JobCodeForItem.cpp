#include "Contents/JobCodeForItem.h"
#include "Contents/Jobs/JobId.h"

#include <new>

// OFFSET: 0x006C7720
// OFFSET: 0x006C7920
// OFFSET: 0x006C7B30
// OFFSET: 0x006C7D40
// OFFSET: 0x006C7F60
// OFFSET: 0x006C8180
// OFFSET: 0x006C83A0
// OFFSET: 0x006C85D0
// OFFSET: 0x006C8810
// FUNCTION: fn_06421
// FUNCTION: fn_06422
// FUNCTION: fn_06423
// FUNCTION: fn_06424
// FUNCTION: fn_06425
// FUNCTION: fn_06426
// FUNCTION: fn_06427
// FUNCTION: fn_06428
// FUNCTION: fn_06429
template <typename... Groups>
void CJobCodeForItem::AddJobCodeFromGroups(unsigned long code, Groups... groups) {
  std::set<unsigned int> groupSet = {static_cast<unsigned int>(groups)...};
  if (!groupSet.empty()) {
    std::set<int> jobs;
    for (const unsigned int& group : groupSet) {
      auto groupIt = this->m_jobGroups.find(group);
      if (groupIt != this->m_jobGroups.end()) {
        jobs.merge(std::set<int>(groupIt->second));
      }
    }

    auto it = this->m_jobCodes.find(code);
    if (it == this->m_jobCodes.end()) {
      this->m_jobCodes[code] = jobs;
    } else {
      it->second.merge(jobs);
    }
  }
}

template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int);

// OFFSET: 0x006C8A50
// OFFSET: 0x006C8B10
// OFFSET: 0x006C8BD0
// OFFSET: 0x006C8CB0
// OFFSET: 0x006C8D90
// OFFSET: 0x006C8F60
// OFFSET: 0x006C9050
// FUNCTION: fn_06430
// FUNCTION: fn_06431
// FUNCTION: fn_06432
// FUNCTION: fn_06433
// FUNCTION: fn_06434
// FUNCTION: fn_06435
// FUNCTION: fn_06436
template <typename... Jobs>
void CJobCodeForItem::AddJobCode(unsigned long code, Jobs... jobs) {
  std::set<int> jobSet = {jobs...};
  if (!jobSet.empty()) {
    auto it = this->m_jobCodes.find(code);
    if (it == this->m_jobCodes.end()) {
      this->m_jobCodes[code] = jobSet;
    } else {
      it->second.merge(jobSet);
    }
  }
}

template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId);

// OFFSET: 0x006C9270
// OFFSET: 0x006C9350
// OFFSET: 0x006C9430
// OFFSET: 0x006C9510
// OFFSET: 0x006C9600
// OFFSET: 0x006C96F0
// OFFSET: 0x006C98F0
// OFFSET: 0x006C9A00
// FUNCTION: fn_06437
// FUNCTION: fn_06438
// FUNCTION: fn_06439
// FUNCTION: fn_06440
// FUNCTION: fn_06441
// FUNCTION: fn_06442
// FUNCTION: fn_06443
// FUNCTION: fn_06444
template <typename... Jobs>
void CJobCodeForItem::AddJobGroup(unsigned int group, Jobs... jobs) {
  std::set<int> jobSet = {jobs...};
  if (!jobSet.empty()) {
    auto it = this->m_jobGroups.find(group);
    if (it == this->m_jobGroups.end()) {
      this->m_jobGroups[group] = jobSet;
    } else {
      it->second.merge(jobSet);
    }
  }
}

template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId,
                                            JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId);

// GLOBAL: gl_24704
template <>
CJobCodeForItem* Singleton<CJobCodeForItem>::s_instance = nullptr;

// OFFSET: 0x00E5CED0
