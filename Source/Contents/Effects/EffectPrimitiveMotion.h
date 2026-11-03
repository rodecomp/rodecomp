#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVEMOTION_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVEMOTION_H

struct RenderVertex;
struct matrix;
struct vector3d;

RenderVertex TransformVert(const matrix& view, const vector3d& position);

#endif
