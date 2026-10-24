#ifndef RAGNAROK_SOURCE_UI_WINDOWPOLICY_H
#define RAGNAROK_SOURCE_UI_WINDOWPOLICY_H

// Manager table+2612,8 words per ID, IDs0..10040 inclusive.
int WindowPolicy(const int* table, bool restricted, unsigned action, unsigned id);

#endif
