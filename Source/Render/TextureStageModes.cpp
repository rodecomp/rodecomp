#include "Render/TextureStageModes.h"

#include <d3d9.h>

namespace {
  using StageWrite = TextureStageWrite;
  // Ordered writes. Missing fields retain prior state.
  constexpr StageWrite mode0[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 1},
  };

  constexpr StageWrite mode2[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 1},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 1},
  };

  constexpr StageWrite mode3[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 7},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 1},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 1},
  };

  constexpr StageWrite mode4[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 13}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 1},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 1},
  };

  constexpr StageWrite mode5[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode6[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 18},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 18},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode7[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 2},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 34}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode8[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 7},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode9[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 13}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode10[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 12}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode11[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 7},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode12[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 18},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 7},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode13[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 7},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode14[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 5},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };

  constexpr StageWrite mode15[] = {
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 0}, {0, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 4},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 0},
    {0, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 3},  {0, static_cast<D3DTEXTURESTAGESTATETYPE>(6), 0},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(11), 1}, {1, static_cast<D3DTEXTURESTAGESTATETYPE>(2), 2},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(1), 8},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(3), 1},
    {1, static_cast<D3DTEXTURESTAGESTATETYPE>(5), 2},  {1, static_cast<D3DTEXTURESTAGESTATETYPE>(4), 2},
  };
}

std::span<const TextureStageWrite> TextureStageModeWrites(int mode) {
  switch (mode) {
  case 0:
    return mode0;
  case 1:
  case 2:
    return mode2;
  case 3:
    return mode3;
  case 4:
    return mode4;
  case 5:
    return mode5;
  case 6:
    return mode6;
  case 7:
    return mode7;
  case 8:
    return mode8;
  case 9:
    return mode9;
  case 10:
    return mode10;
  case 11:
    return mode11;
  case 12:
    return mode12;
  case 13:
    return mode13;
  case 14:
    return mode14;
  case 15:
    return mode15;
  default:
    return {};
  }
}
