#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTATTACHMENTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTATTACHMENTS_H

#include <list>

struct RagEffectState;

// Original CAbleToMakeEffect fields +0x144..+0x153. List nodes are owned;
// effect objects are borrowed from the active game-mode object owner.
struct EffectAttachments {
  std::list<RagEffectState*>& effects;
  RagEffectState*& first;
  RagEffectState*& second;

  bool HasId(int id) const;
  bool Contains(const RagEffectState* effect) const;
  void Unlink(RagEffectState* effect);
  void DetachAll();
};

#endif
