#ifndef RAGNAROK_SOURCE_UTILITY_STACKALLOC_H
#define RAGNAROK_SOURCE_UTILITY_STACKALLOC_H

#include <malloc.h>

#define STACK_ALLOC(size)                                                              \
  (_MallocaComputeSize(size) != 0                                                      \
       ? (((_MallocaComputeSize(size) <= _ALLOCA_S_THRESHOLD)                          \
               ? _MarkAllocaS(_alloca(_MallocaComputeSize(size)), _ALLOCA_S_STACK_MARKER) \
               : _MarkAllocaS(malloc(_MallocaComputeSize(size)), _ALLOCA_S_HEAP_MARKER))) \
       : NULL)

#endif
