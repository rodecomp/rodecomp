#pragma once

#include "Utility/Random/RandomBytes.h"
#include <random>

template __declspec(noinline) std::mt19937::mersenne_twister_engine(std::mt19937::result_type);
