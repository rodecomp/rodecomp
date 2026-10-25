#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTATTACHMENTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTATTACHMENTS_H

#include <list>

struct RagEffectState;

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
