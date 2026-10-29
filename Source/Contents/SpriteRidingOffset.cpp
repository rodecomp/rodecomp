#include "Contents/SpriteRidingOffset.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Jobs/JobNames.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"

// OFFSET: 0x00DDE340
// FUNCTION: fn_23394
static std::string GetBodyActionResourceName(int job, int sex, int costumeJob, int costume) {
  return g_equipmentResourceMgr->GetBodyResourceName(0, job, sex, costumeJob, costume);
}

// OFFSET: 0x00DAAC70
// FUNCTION: fn_22576
int SpriteRidingOffset(const ActionFrame* frame, unsigned animation, unsigned motion, int job, int sex, int costumeJob) {
  if (!frame || !&frame->Cell(1)) {
    return 0;
  }

  const std::string name = GetBodyActionResourceName(JobNames::BaseJob(job), sex, costumeJob, 0);
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(name));
  if (!action) {
    return 0;
  }

  const ActionFrame* baseFrame = &action->Frame(animation, motion);
  if (!baseFrame || baseFrame->anchors.empty() || frame->anchors.empty()) {
    return 0;
  }

  const ActionAnchor& base = baseFrame->anchors.front();
  const ActionAnchor& riding = frame->anchors.front();
  if (base.unknown_0008 != riding.unknown_0008) {
    return 0;
  }

  return base.y - riding.y;
}
