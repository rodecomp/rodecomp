#pragma once

#include "Contents/Items/JobCodeForItem.h"
#include "Contents/Jobs/JobId.h"
#include <new>

template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId,
                                            JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId);
template CJobCodeForItem* Singleton<CJobCodeForItem>::GetInstance();
