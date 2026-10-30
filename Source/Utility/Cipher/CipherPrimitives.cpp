// GLOBAL: gl_25029
unsigned char g_cryptonPermutation0[16] = {0x0F, 0x09, 0x06, 0x08, 0x09, 0x09, 0x04, 0x0C, 0x06, 0x02, 0x06, 0x0A, 0x01, 0x03, 0x05, 0x0F};

// GLOBAL: gl_25030
unsigned char g_cryptonPermutation1[16] = {0x0A, 0x0F, 0x04, 0x07, 0x05, 0x02, 0x0E, 0x06, 0x09, 0x03, 0x0C, 0x08, 0x0D, 0x01, 0x0B, 0x00};

// GLOBAL: gl_25031
unsigned char g_cryptonPermutation2[16] = {0x00, 0x04, 0x08, 0x04, 0x02, 0x0F, 0x08, 0x0D, 0x01, 0x01, 0x0F, 0x07, 0x02, 0x0B, 0x0E, 0x0F};

// GLOBAL: gl_25032
int g_cryptonTablesReady;

// GLOBAL: gl_25033
unsigned char g_cryptonSbox[256];

// GLOBAL: gl_25034
unsigned char g_cryptonInverseSbox[256];

// GLOBAL: gl_25035
unsigned int g_cryptonTable0[256];

// GLOBAL: gl_25036
unsigned int g_cryptonTable1[256];

// GLOBAL: gl_25037
unsigned int g_cryptonTable2[256];

// GLOBAL: gl_25038
unsigned int g_cryptonTable3[256];

// GLOBAL: gl_25039
unsigned int g_cryptonDecryptKeys[52];

// GLOBAL: gl_25040
unsigned int g_cryptonEncryptKeys[52];

// GLOBAL: gl_25041
unsigned int* g_cryptonEncryptKey = g_cryptonEncryptKeys;

// GLOBAL: gl_25042
unsigned int* g_cryptonDecryptKey = g_cryptonDecryptKeys;

// GLOBAL: gl_25043
unsigned int g_cryptonKeyConstants[8] = {0xBB67AE85u, 0x3C6EF372u, 0xA54FF53Au, 0x510E527Fu, 0x9B05688Cu, 0x1F83D9ABu, 0x5BE0CD19u, 0xCBBB9D5Du};

// GLOBAL: gl_25017
unsigned short g_sboxEntries[8][64] = {
  {
    0x2, 0x0, 0x0, 0x3, 0x3, 0x1, 0x1, 0x0, 0x0, 0x2, 0x3, 0x0, 0x3, 0x3, 0x2, 0x1,
    0x1, 0x2, 0x2, 0x0, 0x0, 0x2, 0x2, 0x3, 0x1, 0x3, 0x3, 0x1, 0x0, 0x1, 0x1, 0x2,
    0x0, 0x3, 0x1, 0x2, 0x2, 0x2, 0x2, 0x0, 0x3, 0x0, 0x0, 0x3, 0x0, 0x1, 0x3, 0x1,
    0x3, 0x1, 0x2, 0x3, 0x3, 0x1, 0x1, 0x2, 0x1, 0x2, 0x2, 0x0, 0x1, 0x0, 0x0, 0x3,
  },
  {
    0xC, 0x4, 0x4, 0xC, 0x8, 0x0, 0x8, 0x4, 0x0, 0xC, 0xC, 0x0, 0x4, 0x8, 0x0, 0x8,
    0xC, 0x8, 0x4, 0x0, 0x0, 0x4, 0xC, 0x8, 0x8, 0x0, 0x0, 0xC, 0x4, 0xC, 0x8, 0x4,
    0x0, 0xC, 0x8, 0x8, 0x4, 0x8, 0xC, 0x4, 0x8, 0x4, 0x0, 0xC, 0xC, 0x0, 0x4, 0x0,
    0x4, 0xC, 0x8, 0x0, 0x8, 0x4, 0x0, 0x8, 0xC, 0x0, 0x4, 0x4, 0x0, 0x8, 0xC, 0xC,
  },
  {
    0x20, 0x30, 0x0, 0x10, 0x30, 0x0, 0x20, 0x30, 0x0, 0x10, 0x10, 0x0, 0x30, 0x0, 0x10, 0x20,
    0x10, 0x0, 0x30, 0x20, 0x20, 0x10, 0x10, 0x20, 0x30, 0x20, 0x0, 0x30, 0x0, 0x30, 0x20, 0x10,
    0x30, 0x10, 0x0, 0x20, 0x0, 0x30, 0x30, 0x0, 0x20, 0x0, 0x30, 0x30, 0x10, 0x20, 0x0, 0x10,
    0x30, 0x0, 0x10, 0x30, 0x0, 0x20, 0x20, 0x10, 0x10, 0x30, 0x20, 0x10, 0x20, 0x0, 0x10, 0x20,
  },
  {
    0x40, 0xC0, 0xC0, 0x80, 0x80, 0xC0, 0x40, 0x40, 0x0, 0x0, 0x0, 0xC0, 0xC0, 0x0, 0x80, 0x40,
    0x40, 0x0, 0x0, 0x40, 0x80, 0x0, 0x40, 0x80, 0xC0, 0x40, 0x80, 0x80, 0x0, 0x80, 0xC0, 0xC0,
    0x80, 0x40, 0x0, 0xC0, 0xC0, 0x0, 0x0, 0x0, 0x80, 0x80, 0xC0, 0x40, 0x40, 0xC0, 0xC0, 0x80,
    0xC0, 0xC0, 0x40, 0x0, 0x40, 0x40, 0x80, 0xC0, 0x40, 0x80, 0x0, 0x40, 0x80, 0x0, 0x0, 0x80,
  },
  {
    0x0, 0x200, 0x200, 0x300, 0x0, 0x0, 0x100, 0x200, 0x100, 0x0, 0x200, 0x100, 0x300, 0x300, 0x0, 0x100,
    0x200, 0x100, 0x100, 0x0, 0x100, 0x300, 0x300, 0x200, 0x300, 0x100, 0x0, 0x300, 0x200, 0x200, 0x300, 0x0,
    0x0, 0x300, 0x0, 0x200, 0x100, 0x200, 0x300, 0x100, 0x200, 0x100, 0x300, 0x200, 0x100, 0x0, 0x200, 0x300,
    0x300, 0x0, 0x300, 0x300, 0x200, 0x0, 0x100, 0x300, 0x0, 0x200, 0x100, 0x0, 0x0, 0x100, 0x200, 0x100,
  },
  {
    0x800, 0x800, 0x400, 0xC00, 0x800, 0x0, 0xC00, 0x0, 0xC00, 0x400, 0x0, 0x800, 0x0, 0xC00, 0x800, 0x400,
    0x0, 0x0, 0xC00, 0x400, 0x400, 0xC00, 0x0, 0x800, 0x800, 0x0, 0x400, 0xC00, 0x400, 0x400, 0xC00, 0x800,
    0xC00, 0x0, 0x800, 0x400, 0xC00, 0x0, 0x400, 0x800, 0x0, 0xC00, 0x800, 0x400, 0x800, 0xC00, 0x400, 0x800,
    0x400, 0xC00, 0x0, 0x800, 0x0, 0x400, 0x800, 0x400, 0x400, 0x0, 0xC00, 0x0, 0xC00, 0x800, 0x0, 0xC00,
  },
  {
    0x0, 0x3000, 0x3000, 0x0, 0x0, 0x3000, 0x2000, 0x1000, 0x3000, 0x0, 0x0, 0x3000, 0x2000, 0x1000, 0x3000, 0x2000,
    0x1000, 0x2000, 0x2000, 0x1000, 0x3000, 0x1000, 0x1000, 0x2000, 0x1000, 0x0, 0x2000, 0x3000, 0x0, 0x2000, 0x1000, 0x0,
    0x1000, 0x0, 0x0, 0x3000, 0x3000, 0x3000, 0x3000, 0x2000, 0x2000, 0x1000, 0x1000, 0x0, 0x1000, 0x2000, 0x2000, 0x1000,
    0x2000, 0x3000, 0x3000, 0x1000, 0x0, 0x0, 0x2000, 0x3000, 0x0, 0x2000, 0x1000, 0x0, 0x3000, 0x1000, 0x0, 0x2000,
  },
  {
    0xC000, 0x4000, 0x0, 0xC000, 0x8000, 0xC000, 0x0, 0x8000, 0x0, 0x8000, 0xC000, 0x4000, 0xC000, 0x4000, 0x4000, 0x0,
    0x8000, 0x8000, 0xC000, 0x4000, 0x4000, 0x0, 0x8000, 0xC000, 0x4000, 0x0, 0x0, 0x8000, 0x8000, 0xC000, 0x4000, 0x0,
    0x4000, 0x0, 0xC000, 0x4000, 0x0, 0x8000, 0x4000, 0x4000, 0xC000, 0x0, 0x8000, 0x8000, 0x8000, 0x8000, 0x0, 0xC000,
    0x0, 0xC000, 0x0, 0x8000, 0x8000, 0xC000, 0xC000, 0x0, 0xC000, 0x4000, 0x4000, 0x4000, 0x4000, 0x0, 0x8000, 0xC000,
  },
};

// GLOBAL: gl_25014
unsigned char g_mistyS7[128] = {
  0x1F, 0x24, 0x13, 0x6C, 0x37, 0x2E, 0x3F, 0x4A, 0x5D, 0x0F, 0x40, 0x56, 0x25, 0x51, 0x1C, 0x04,
  0x0B, 0x46, 0x20, 0x0D, 0x7B, 0x35, 0x44, 0x42, 0x2B, 0x1E, 0x41, 0x14, 0x4B, 0x79, 0x15, 0x6F,
  0x01, 0x6D, 0x6E, 0x63, 0x18, 0x77, 0x23, 0x05, 0x26, 0x76, 0x00, 0x31, 0x2D, 0x7A, 0x7F, 0x61,
  0x19, 0x12, 0x65, 0x2F, 0x30, 0x39, 0x08, 0x68, 0x5F, 0x78, 0x2A, 0x4C, 0x64, 0x45, 0x75, 0x3D,
  0x59, 0x48, 0x03, 0x57, 0x7C, 0x4F, 0x62, 0x3C, 0x1D, 0x21, 0x5E, 0x27, 0x6A, 0x70, 0x4D, 0x3A,
  0x1B, 0x32, 0x33, 0x5A, 0x3B, 0x10, 0x17, 0x54, 0x5B, 0x1A, 0x72, 0x73, 0x6B, 0x2C, 0x66, 0x49,
  0x0E, 0x55, 0x09, 0x36, 0x74, 0x0C, 0x67, 0x53, 0x28, 0x0A, 0x7E, 0x38, 0x02, 0x07, 0x60, 0x29,
  0x50, 0x22, 0x11, 0x06, 0x47, 0x16, 0x52, 0x4E, 0x71, 0x3E, 0x69, 0x43, 0x34, 0x5C, 0x58, 0x7D,
};

// GLOBAL: gl_25015
unsigned int g_mistyS9[512] = {
  0x03D, 0x1D1, 0x080, 0x0A8, 0x057, 0x1B9, 0x162, 0x148, 0x0D9, 0x105, 0x062, 0x07A, 0x021, 0x1FF, 0x112, 0x108,
  0x149, 0x1C7, 0x174, 0x03E, 0x13B, 0x1B7, 0x08E, 0x1C6, 0x0AE, 0x010, 0x095, 0x1EF, 0x04E, 0x0F2, 0x1FD, 0x085,
  0x0C7, 0x14A, 0x037, 0x07E, 0x0EB, 0x164, 0x193, 0x1D8, 0x0A3, 0x11E, 0x055, 0x02C, 0x01D, 0x1A2, 0x163, 0x118,
  0x001, 0x09D, 0x0F8, 0x1A0, 0x16D, 0x1F3, 0x01C, 0x146, 0x07D, 0x0D1, 0x082, 0x1EA, 0x183, 0x12D, 0x0F4, 0x19E,
  0x1D3, 0x0DD, 0x1E2, 0x128, 0x1E0, 0x0EC, 0x059, 0x091, 0x011, 0x12F, 0x026, 0x0DC, 0x0B0, 0x18C, 0x10F, 0x1F7,
  0x0E7, 0x16C, 0x0B6, 0x0F9, 0x0D8, 0x151, 0x101, 0x14C, 0x103, 0x0B8, 0x154, 0x12B, 0x1AE, 0x017, 0x071, 0x00C,
  0x047, 0x058, 0x07F, 0x1A4, 0x134, 0x129, 0x084, 0x15D, 0x19D, 0x1B2, 0x1A3, 0x048, 0x07C, 0x051, 0x1CA, 0x023,
  0x1AA, 0x143, 0x17B, 0x056, 0x18D, 0x166, 0x0D4, 0x1FB, 0x14D, 0x194, 0x19A, 0x087, 0x1F8, 0x123, 0x0A7, 0x1B8,
  0x1C0, 0x0A9, 0x11D, 0x1B0, 0x1A6, 0x0CD, 0x0F3, 0x05C, 0x102, 0x05B, 0x1D9, 0x144, 0x1F6, 0x0AD, 0x0A5, 0x03A,
  0x00E, 0x1F0, 0x077, 0x04D, 0x17A, 0x086, 0x08B, 0x0B3, 0x171, 0x0BF, 0x10E, 0x104, 0x097, 0x15B, 0x160, 0x168,
  0x065, 0x068, 0x099, 0x150, 0x008, 0x007, 0x17C, 0x0B7, 0x024, 0x019, 0x0DE, 0x127, 0x0DB, 0x0E4, 0x1A9, 0x052,
  0x109, 0x090, 0x19C, 0x1C1, 0x028, 0x1B3, 0x135, 0x16A, 0x176, 0x0DF, 0x1E5, 0x188, 0x0C5, 0x16E, 0x1DE, 0x1B1,
  0x0C3, 0x1DF, 0x036, 0x0EE, 0x1EE, 0x0F0, 0x093, 0x049, 0x09A, 0x1B6, 0x069, 0x081, 0x125, 0x00B, 0x05E, 0x0B4,
  0x018, 0x177, 0x064, 0x0CF, 0x06D, 0x100, 0x199, 0x130, 0x15A, 0x005, 0x120, 0x1BB, 0x1BD, 0x0E0, 0x04F, 0x0D6,
  0x1C3, 0x0CB, 0x153, 0x19F, 0x1E3, 0x0E9, 0x0FB, 0x035, 0x181, 0x0B9, 0x117, 0x1EB, 0x133, 0x009, 0x02D, 0x0D3,
  0x0FD, 0x0F6, 0x0A0, 0x16F, 0x083, 0x08A, 0x156, 0x09B, 0x13C, 0x107, 0x167, 0x098, 0x1D0, 0x1E9, 0x003, 0x1FE,
  0x0BD, 0x122, 0x089, 0x0D2, 0x18F, 0x012, 0x033, 0x06A, 0x142, 0x0ED, 0x170, 0x11B, 0x0E2, 0x14F, 0x158, 0x131,
  0x147, 0x05D, 0x113, 0x1CD, 0x079, 0x161, 0x1A5, 0x179, 0x09E, 0x1B4, 0x0CC, 0x022, 0x132, 0x01A, 0x0E8, 0x004,
  0x187, 0x1ED, 0x197, 0x039, 0x1BF, 0x1D7, 0x027, 0x18B, 0x0C6, 0x09C, 0x0D0, 0x14E, 0x06C, 0x034, 0x1F2, 0x06E,
  0x0CA, 0x025, 0x0BA, 0x191, 0x0FE, 0x013, 0x106, 0x02F, 0x1AD, 0x172, 0x1DB, 0x0C0, 0x10B, 0x1D6, 0x0F5, 0x1EC,
  0x10D, 0x076, 0x114, 0x1AB, 0x075, 0x10C, 0x1E4, 0x159, 0x054, 0x11F, 0x04B, 0x0C4, 0x1BE, 0x0F7, 0x029, 0x0A4,
  0x0BC, 0x0C2, 0x0C9, 0x173, 0x189, 0x1F5, 0x074, 0x1CC, 0x1E6, 0x1A8, 0x195, 0x01F, 0x041, 0x00D, 0x1BA, 0x032,
  0x0D7, 0x0BB, 0x066, 0x1CE, 0x0FC, 0x092, 0x1C5, 0x06F, 0x016, 0x04A, 0x0A1, 0x139, 0x0AF, 0x0F1, 0x190, 0x00A,
  0x13D, 0x1A7, 0x165, 0x03B, 0x042, 0x0DA, 0x192, 0x0CE, 0x0C1, 0x06B, 0x09F, 0x1F1, 0x12C, 0x184, 0x0FA, 0x196,
  0x141, 0x03C, 0x1F9, 0x140, 0x02A, 0x155, 0x11A, 0x1A1, 0x198, 0x0D5, 0x126, 0x1AF, 0x061, 0x12E, 0x157, 0x1DC,
  0x072, 0x18A, 0x0AA, 0x096, 0x115, 0x0EF, 0x045, 0x07B, 0x08D, 0x145, 0x053, 0x05F, 0x178, 0x0B2, 0x02E, 0x020,
  0x1D5, 0x03F, 0x1C9, 0x1E7, 0x1AC, 0x044, 0x038, 0x014, 0x0B1, 0x16B, 0x0AB, 0x0B5, 0x05A, 0x182, 0x1C8, 0x1D4,
  0x1CB, 0x136, 0x17F, 0x046, 0x0E1, 0x01E, 0x1DD, 0x0E6, 0x137, 0x1FA, 0x185, 0x08C, 0x08F, 0x040, 0x1B5, 0x0BE,
  0x13F, 0x1C4, 0x12A, 0x015, 0x006, 0x0FF, 0x19B, 0x0A6, 0x043, 0x088, 0x050, 0x15F, 0x1E8, 0x121, 0x073, 0x17E,
  0x14B, 0x152, 0x1D2, 0x00F, 0x02B, 0x030, 0x13A, 0x0E5, 0x111, 0x138, 0x18E, 0x063, 0x0E3, 0x0C8, 0x1F4, 0x01B,
  0x1E1, 0x169, 0x17D, 0x031, 0x180, 0x10A, 0x094, 0x1DA, 0x186, 0x13E, 0x11C, 0x060, 0x175, 0x1CF, 0x067, 0x119,
  0x078, 0x000, 0x0AC, 0x110, 0x15E, 0x124, 0x002, 0x1BC, 0x0A2, 0x0EA, 0x070, 0x1FC, 0x116, 0x15C, 0x04C, 0x1C2,
};

// GLOBAL: gl_25016
int g_sboxBitSelect[8][6] = {
  {2, 5, 6, 9, 11, 13},
  {1, 4, 7, 10, 8, 14},
  {3, 6, 8, 13, 0, 15},
  {12, 14, 1, 2, 4, 10},
  {0, 10, 3, 14, 6, 12},
  {7, 8, 12, 15, 1, 5},
  {9, 15, 5, 11, 2, 7},
  {11, 13, 0, 4, 3, 9},
};

// GLOBAL: gl_25019
int g_fealRotateTableDirty = 1;

// GLOBAL: gl_25018
unsigned short g_sboxLookup[0x10000];

// GLOBAL: gl_25020
unsigned char g_fealRotateTable[256];

struct KeyedBoxContext {
  /* 0000 */ int blockSize;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int startRound;
  /* 0014 */ unsigned char boxes[1][256];
};

struct SealContext {
  /* 0000 */ unsigned int t[520];
  /* 0820 */ unsigned int s[265];
  /* 0C44 */ unsigned int r[20];
  /* 0C94 */ unsigned int counter;
  /* 0C98 */ unsigned int keystream[1024];
  /* 1C98 */ int position;
};

static_assert(sizeof(SealContext) == 0x1C9C);

// GLOBAL: gl_25067
int g_keyedBoxRound;

// INLINED
static unsigned int CryptonRotate(unsigned int value, unsigned int shift) {
  return (value << shift) | (value >> (32 - shift));
}

// INLINED
static unsigned int CryptonKeyTransform(unsigned int value, unsigned int lane) {
  switch (lane) {
  case 0:
    return (CryptonRotate(value, 24) & 0xFCCFFCCFu) ^ (CryptonRotate(value, 16) & 0xF33FF33Fu) ^
           (CryptonRotate(value, 8) & 0xCFFCCFFCu) ^ (value & 0x3FF33FF3u);
  case 1:
    return (CryptonRotate(value, 24) & 0xF33FF33Fu) ^ (CryptonRotate(value, 16) & 0xCFFCCFFCu) ^
           (CryptonRotate(value, 8) & 0x3FF33FF3u) ^ (value & 0xFCCFFCCFu);
  case 2:
    return (CryptonRotate(value, 24) & 0xCFFCCFFCu) ^ (CryptonRotate(value, 16) & 0x3FF33FF3u) ^
           (CryptonRotate(value, 8) & 0xFCCFFCCFu) ^ (value & 0xF33FF33Fu);
  default:
    return (CryptonRotate(value, 24) & 0x3FF33FF3u) ^ (CryptonRotate(value, 16) & 0xFCCFFCCFu) ^
           (CryptonRotate(value, 8) & 0xF33FF33Fu) ^ (value & 0xCFFCCFFCu);
  }
}

// INLINED
static void CryptonRound(unsigned int& a, unsigned int& b, unsigned int& c, unsigned int& d, const unsigned int* key, bool alternate) {
  unsigned int x;
  unsigned int y;
  unsigned int z;
  unsigned int w;
  if (!alternate) {
    x = g_cryptonTable0[a & 255] ^ g_cryptonTable1[b & 255] ^ g_cryptonTable2[c & 255] ^ g_cryptonTable3[d & 255];
    y = g_cryptonTable1[(a >> 8) & 255] ^ g_cryptonTable2[(b >> 8) & 255] ^ g_cryptonTable3[(c >> 8) & 255] ^ g_cryptonTable0[(d >> 8) & 255];
    z = g_cryptonTable2[(a >> 16) & 255] ^ g_cryptonTable3[(b >> 16) & 255] ^ g_cryptonTable0[(c >> 16) & 255] ^ g_cryptonTable1[(d >> 16) & 255];
    w = g_cryptonTable3[a >> 24] ^ g_cryptonTable0[b >> 24] ^ g_cryptonTable1[c >> 24] ^ g_cryptonTable2[d >> 24];
  } else {
    x = g_cryptonTable1[a & 255] ^ g_cryptonTable2[b & 255] ^ g_cryptonTable3[c & 255] ^ g_cryptonTable0[d & 255];
    y = g_cryptonTable2[(a >> 8) & 255] ^ g_cryptonTable3[(b >> 8) & 255] ^ g_cryptonTable0[(c >> 8) & 255] ^ g_cryptonTable1[(d >> 8) & 255];
    z = g_cryptonTable3[(a >> 16) & 255] ^ g_cryptonTable0[(b >> 16) & 255] ^ g_cryptonTable1[(c >> 16) & 255] ^ g_cryptonTable2[(d >> 16) & 255];
    w = g_cryptonTable0[a >> 24] ^ g_cryptonTable1[b >> 24] ^ g_cryptonTable2[c >> 24] ^ g_cryptonTable3[d >> 24];
  }
  a = x ^ key[0];
  b = y ^ key[1];
  c = z ^ key[2];
  d = w ^ key[3];
}

// INLINED
static void CryptonSubstitute(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int* output, bool alternate) {
  for (unsigned int lane = 0; lane < 4; ++lane) {
    const unsigned int shift = lane * 8;
    if ((lane & 1) == alternate) {
      output[lane] = g_cryptonSbox[(a >> shift) & 255] |
                     (static_cast<unsigned int>(g_cryptonInverseSbox[(b >> shift) & 255]) << 8) |
                     (static_cast<unsigned int>(g_cryptonSbox[(c >> shift) & 255]) << 16) |
                     (static_cast<unsigned int>(g_cryptonInverseSbox[(d >> shift) & 255]) << 24);
    } else {
      output[lane] = g_cryptonInverseSbox[(a >> shift) & 255] |
                     (static_cast<unsigned int>(g_cryptonSbox[(b >> shift) & 255]) << 8) |
                     (static_cast<unsigned int>(g_cryptonInverseSbox[(c >> shift) & 255]) << 16) |
                     (static_cast<unsigned int>(g_cryptonSbox[(d >> shift) & 255]) << 24);
    }
  }
}

#define RIPEMD128_F(b, c, d) ((b) ^ (c) ^ (d))
#define RIPEMD128_G(b, c, d) (((b) & (c)) | (~(b) & (d)))
#define RIPEMD128_H(b, c, d) (((b) | ~(c)) ^ (d))
#define RIPEMD128_I(b, c, d) (((b) & (d)) | ((c) & ~(d)))
#define RIPEMD128_STEP(a, b, c, d, x, shift, function, constant) \
  do { \
    (a) += function(b, c, d) + (x) + (constant); \
    (a) = ((a) << (shift)) | ((a) >> (32 - (shift))); \
  } while (false)

// OFFSET: 0x00B0DE90
// FUNCTION: fn_18404
void Ripemd128Transform(unsigned int* state, const unsigned int* block) {
  unsigned int a = state[0];
  unsigned int b = state[1];
  unsigned int c = state[2];
  unsigned int d = state[3];
  unsigned int aa = a;
  unsigned int bb = b;
  unsigned int cc = c;
  unsigned int dd = d;

  RIPEMD128_STEP(a, b, c, d, block[0], 11, RIPEMD128_F, 0u);
  RIPEMD128_STEP(d, a, b, c, block[1], 14, RIPEMD128_F, 0u);
  RIPEMD128_STEP(c, d, a, b, block[2], 15, RIPEMD128_F, 0u);
  RIPEMD128_STEP(b, c, d, a, block[3], 12, RIPEMD128_F, 0u);
  RIPEMD128_STEP(a, b, c, d, block[4], 5, RIPEMD128_F, 0u);
  RIPEMD128_STEP(d, a, b, c, block[5], 8, RIPEMD128_F, 0u);
  RIPEMD128_STEP(c, d, a, b, block[6], 7, RIPEMD128_F, 0u);
  RIPEMD128_STEP(b, c, d, a, block[7], 9, RIPEMD128_F, 0u);
  RIPEMD128_STEP(a, b, c, d, block[8], 11, RIPEMD128_F, 0u);
  RIPEMD128_STEP(d, a, b, c, block[9], 13, RIPEMD128_F, 0u);
  RIPEMD128_STEP(c, d, a, b, block[10], 14, RIPEMD128_F, 0u);
  RIPEMD128_STEP(b, c, d, a, block[11], 15, RIPEMD128_F, 0u);
  RIPEMD128_STEP(a, b, c, d, block[12], 6, RIPEMD128_F, 0u);
  RIPEMD128_STEP(d, a, b, c, block[13], 7, RIPEMD128_F, 0u);
  RIPEMD128_STEP(c, d, a, b, block[14], 9, RIPEMD128_F, 0u);
  RIPEMD128_STEP(b, c, d, a, block[15], 8, RIPEMD128_F, 0u);

  RIPEMD128_STEP(a, b, c, d, block[7], 7, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(d, a, b, c, block[4], 6, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(c, d, a, b, block[13], 8, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(b, c, d, a, block[1], 13, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(a, b, c, d, block[10], 11, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(d, a, b, c, block[6], 9, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(c, d, a, b, block[15], 7, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(b, c, d, a, block[3], 15, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(a, b, c, d, block[12], 7, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(d, a, b, c, block[0], 12, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(c, d, a, b, block[9], 15, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(b, c, d, a, block[5], 9, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(a, b, c, d, block[2], 11, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(d, a, b, c, block[14], 7, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(c, d, a, b, block[11], 13, RIPEMD128_G, 0x5A827999u);
  RIPEMD128_STEP(b, c, d, a, block[8], 12, RIPEMD128_G, 0x5A827999u);

  RIPEMD128_STEP(a, b, c, d, block[3], 11, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(d, a, b, c, block[10], 13, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(c, d, a, b, block[14], 6, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(b, c, d, a, block[4], 7, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(a, b, c, d, block[9], 14, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(d, a, b, c, block[15], 9, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(c, d, a, b, block[8], 13, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(b, c, d, a, block[1], 15, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(a, b, c, d, block[2], 14, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(d, a, b, c, block[7], 8, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(c, d, a, b, block[0], 13, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(b, c, d, a, block[6], 6, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(a, b, c, d, block[13], 5, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(d, a, b, c, block[11], 12, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(c, d, a, b, block[5], 7, RIPEMD128_H, 0x6ED9EBA1u);
  RIPEMD128_STEP(b, c, d, a, block[12], 5, RIPEMD128_H, 0x6ED9EBA1u);

  RIPEMD128_STEP(a, b, c, d, block[1], 11, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(d, a, b, c, block[9], 12, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(c, d, a, b, block[11], 14, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(b, c, d, a, block[10], 15, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(a, b, c, d, block[0], 14, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(d, a, b, c, block[8], 15, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(c, d, a, b, block[12], 9, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(b, c, d, a, block[4], 8, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(a, b, c, d, block[13], 9, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(d, a, b, c, block[3], 14, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(c, d, a, b, block[7], 5, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(b, c, d, a, block[15], 6, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(a, b, c, d, block[14], 8, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(d, a, b, c, block[5], 6, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(c, d, a, b, block[6], 5, RIPEMD128_I, 0x8F1BBCDCu);
  RIPEMD128_STEP(b, c, d, a, block[2], 12, RIPEMD128_I, 0x8F1BBCDCu);

  RIPEMD128_STEP(aa, bb, cc, dd, block[5], 8, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[14], 9, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[7], 9, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[0], 11, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[9], 13, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[2], 15, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[11], 15, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[4], 5, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[13], 7, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[6], 7, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[15], 8, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[8], 11, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[1], 14, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[10], 14, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[3], 12, RIPEMD128_I, 0x50A28BE6u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[12], 6, RIPEMD128_I, 0x50A28BE6u);

  RIPEMD128_STEP(aa, bb, cc, dd, block[6], 9, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[11], 13, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[3], 15, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[7], 7, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[0], 12, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[13], 8, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[5], 9, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[10], 11, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[14], 7, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[15], 7, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[8], 12, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[12], 7, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[4], 6, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[9], 15, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[1], 13, RIPEMD128_H, 0x5C4DD124u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[2], 11, RIPEMD128_H, 0x5C4DD124u);

  RIPEMD128_STEP(aa, bb, cc, dd, block[15], 9, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[5], 7, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[1], 15, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[3], 11, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[7], 8, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[14], 6, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[6], 6, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[9], 14, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[11], 12, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[8], 13, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[12], 5, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[2], 14, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[10], 13, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[0], 13, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[4], 7, RIPEMD128_G, 0x6D703EF3u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[13], 5, RIPEMD128_G, 0x6D703EF3u);

  RIPEMD128_STEP(aa, bb, cc, dd, block[8], 15, RIPEMD128_F, 0u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[6], 5, RIPEMD128_F, 0u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[4], 8, RIPEMD128_F, 0u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[1], 11, RIPEMD128_F, 0u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[3], 14, RIPEMD128_F, 0u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[11], 14, RIPEMD128_F, 0u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[15], 6, RIPEMD128_F, 0u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[0], 14, RIPEMD128_F, 0u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[5], 6, RIPEMD128_F, 0u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[12], 9, RIPEMD128_F, 0u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[2], 12, RIPEMD128_F, 0u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[13], 9, RIPEMD128_F, 0u);
  RIPEMD128_STEP(aa, bb, cc, dd, block[9], 12, RIPEMD128_F, 0u);
  RIPEMD128_STEP(dd, aa, bb, cc, block[7], 5, RIPEMD128_F, 0u);
  RIPEMD128_STEP(cc, dd, aa, bb, block[10], 15, RIPEMD128_F, 0u);
  RIPEMD128_STEP(bb, cc, dd, aa, block[14], 8, RIPEMD128_F, 0u);

  const unsigned int result = state[1] + c + dd;
  state[1] = state[2] + d + aa;
  state[2] = state[3] + a + bb;
  state[3] = state[0] + b + cc;
  state[0] = result;
}

#undef RIPEMD128_STEP
#undef RIPEMD128_F
#undef RIPEMD128_G
#undef RIPEMD128_H
#undef RIPEMD128_I

// OFFSET: 0x00B0F710
// FUNCTION: fn_18405
void CryptonEncryptBlock(const unsigned int* input, unsigned int* output) {
  unsigned int* key = g_cryptonEncryptKey;
  unsigned int a = input[0] ^ key[0];
  unsigned int b = input[1] ^ key[1];
  unsigned int c = input[2] ^ key[2];
  unsigned int d = input[3] ^ key[3];
  CryptonRound(a, b, c, d, key + 4, false);
  CryptonRound(a, b, c, d, key + 8, true);
  CryptonRound(a, b, c, d, key + 12, false);
  CryptonRound(a, b, c, d, key + 16, true);
  CryptonRound(a, b, c, d, key + 20, false);
  CryptonRound(a, b, c, d, key + 24, true);
  CryptonRound(a, b, c, d, key + 28, false);
  CryptonRound(a, b, c, d, key + 32, true);
  CryptonRound(a, b, c, d, key + 36, false);
  CryptonRound(a, b, c, d, key + 40, true);
  CryptonRound(a, b, c, d, key + 44, false);
  unsigned int result[4];
  CryptonSubstitute(a, b, c, d, result, true);
  output[0] = result[0] ^ key[48];
  output[1] = result[1] ^ g_cryptonEncryptKey[49];
  output[2] = result[2] ^ g_cryptonEncryptKey[50];
  output[3] = result[3] ^ g_cryptonEncryptKey[51];
}

// OFFSET: 0x00B103A0
// FUNCTION: fn_18406
unsigned int* CryptonSetKey(const unsigned int* input, unsigned int keyBits) {
  if (!g_cryptonTablesReady) {
    for (unsigned int i = 0; i < 256; ++i) {
      const unsigned int high = i >> 4;
      const unsigned int low = i & 15;
      const unsigned int first = g_cryptonPermutation0[low];
      const unsigned int second = g_cryptonPermutation1[first ^ high] ^ low;
      const unsigned int value = second | ((g_cryptonPermutation2[second] ^ first ^ high) << 4);
      g_cryptonSbox[i] = static_cast<unsigned char>(value);
      g_cryptonInverseSbox[value] = static_cast<unsigned char>(i);
      g_cryptonTable0[i] = (value * 0x01010101u) & 0x3FCFF3FCu;
      g_cryptonTable1[value] = (i * 0x01010101u) & 0xFC3FCFF3u;
      g_cryptonTable2[i] = (value * 0x01010101u) & 0xF3FC3FCFu;
      g_cryptonTable3[value] = (i * 0x01010101u) & 0xCFF3FC3Fu;
    }
    g_cryptonTablesReady = 1;
  }

  g_cryptonEncryptKey[7] = 0;
  g_cryptonEncryptKey[6] = 0;
  g_cryptonEncryptKey[3] = 0;
  g_cryptonEncryptKey[2] = 0;
  switch ((keyBits + 63) >> 6) {
  case 4:
    g_cryptonEncryptKey[3] = input[6];
    g_cryptonEncryptKey[7] = input[7];
  case 3:
    g_cryptonEncryptKey[2] = input[4];
    g_cryptonEncryptKey[6] = input[5];
  case 2:
    g_cryptonEncryptKey[0] = input[0];
    g_cryptonEncryptKey[4] = input[1];
    g_cryptonEncryptKey[1] = input[2];
    g_cryptonEncryptKey[5] = input[3];
  }

  unsigned int* key = g_cryptonEncryptKey;
  unsigned int a = key[0];
  unsigned int b = key[1];
  unsigned int c = key[2];
  unsigned int d = key[3];
  unsigned int x = (a & 0x3FCFF3FCu) ^ (b & 0xFC3FCFF3u) ^ (c & 0xF3FC3FCFu) ^ (d & 0xCFF3FC3Fu) ^ g_cryptonKeyConstants[0];
  unsigned int y = (a & 0xFC3FCFF3u) ^ (b & 0xF3FC3FCFu) ^ (c & 0xCFF3FC3Fu) ^ (d & 0x3FCFF3FCu) ^ g_cryptonKeyConstants[1];
  unsigned int z = (a & 0xF3FC3FCFu) ^ (b & 0xCFF3FC3Fu) ^ (c & 0x3FCFF3FCu) ^ (d & 0xFC3FCFF3u) ^ g_cryptonKeyConstants[2];
  unsigned int w = (a & 0xCFF3FC3Fu) ^ (b & 0x3FCFF3FCu) ^ (c & 0xFC3FCFF3u) ^ (d & 0xF3FC3FCFu) ^ g_cryptonKeyConstants[3];
  CryptonSubstitute(x, y, z, w, key, false);

  a = key[4];
  b = key[5];
  c = key[6];
  d = key[7];
  x = (a & 0xFC3FCFF3u) ^ (b & 0xF3FC3FCFu) ^ (c & 0xCFF3FC3Fu) ^ (d & 0x3FCFF3FCu) ^ g_cryptonKeyConstants[4];
  y = (a & 0xF3FC3FCFu) ^ (b & 0xCFF3FC3Fu) ^ (c & 0x3FCFF3FCu) ^ (d & 0xFC3FCFF3u) ^ g_cryptonKeyConstants[5];
  z = (a & 0xCFF3FC3Fu) ^ (b & 0x3FCFF3FCu) ^ (c & 0xFC3FCFF3u) ^ (d & 0xF3FC3FCFu) ^ g_cryptonKeyConstants[6];
  w = (a & 0x3FCFF3FCu) ^ (b & 0xFC3FCFF3u) ^ (c & 0xF3FC3FCFu) ^ (d & 0xCFF3FC3Fu) ^ g_cryptonKeyConstants[7];
  CryptonSubstitute(x, y, z, w, key + 4, true);

  const unsigned int first = key[0] ^ key[1] ^ key[2] ^ key[3];
  const unsigned int second = key[4] ^ key[5] ^ key[6] ^ key[7];
  for (unsigned int i = 0; i < 4; ++i) {
    key[i] ^= second;
    key[i + 4] ^= first;
  }

  key[8] = CryptonRotate(key[0], 8);
  key[9] = key[1] ^ 0x01010101u;
  key[10] = CryptonRotate(key[2], 16);
  key[11] = key[3] ^ 0x01010101u;
  key[12] = key[4] ^ 0x01010101u;
  key[13] = CryptonRotate(key[5], 16);
  key[14] = key[6] ^ 0x01010101u;
  key[15] = CryptonRotate(key[7], 24);
  key[16] = key[8] ^ 0x02020202u;
  key[17] = CryptonRotate(key[9], 24);
  key[18] = key[10] ^ 0x02020202u;
  key[19] = CryptonRotate(key[11], 8);
  key[20] = CryptonRotate(key[12], 8);
  key[21] = key[13] ^ 0x02020202u;
  key[22] = CryptonRotate(key[14], 16);
  key[23] = key[15] ^ 0x02020202u;
  key[24] = CryptonRotate(key[16], 16);
  key[25] = key[17] ^ 0x04040404u;
  key[26] = CryptonRotate(key[18], 24);
  key[27] = key[19] ^ 0x04040404u;
  key[28] = key[20] ^ 0x04040404u;
  key[29] = CryptonRotate(key[21], 24);
  key[30] = key[22] ^ 0x04040404u;
  key[31] = CryptonRotate(key[23], 8);
  key[32] = key[24] ^ 0x08080808u;
  key[33] = CryptonRotate(key[25], 8);
  key[34] = key[26] ^ 0x08080808u;
  key[35] = CryptonRotate(key[27], 16);
  key[36] = CryptonRotate(key[28], 16);
  key[37] = key[29] ^ 0x08080808u;
  key[38] = CryptonRotate(key[30], 24);
  key[39] = key[31] ^ 0x08080808u;
  key[40] = CryptonRotate(key[32], 24);
  key[41] = key[33] ^ 0x10101010u;
  key[42] = CryptonRotate(key[34], 8);
  key[43] = key[35] ^ 0x10101010u;
  key[44] = key[36] ^ 0x10101010u;
  key[45] = CryptonRotate(key[37], 8);
  key[46] = key[38] ^ 0x10101010u;
  key[47] = CryptonRotate(key[39], 16);
  key[48] = key[40] ^ 0x20202020u;
  key[49] = CryptonRotate(key[41], 16);
  key[50] = key[42] ^ 0x20202020u;
  key[51] = CryptonRotate(key[43], 24);

  for (unsigned int group = 0; group < 13; ++group) {
    for (unsigned int lane = 0; lane < 4; ++lane) {
      g_cryptonDecryptKey[(12 - group) * 4 + lane] = CryptonKeyTransform(key[group * 4 + lane], (lane + (group & 1 ? 3 : 0)) & 3);
    }
  }
  for (unsigned int lane = 0; lane < 4; ++lane) {
    g_cryptonDecryptKey[48 + lane] = CryptonKeyTransform(g_cryptonDecryptKey[48 + lane], lane);
  }
  for (unsigned int lane = 0; lane < 4; ++lane) {
    key[48 + lane] = CryptonKeyTransform(key[48 + lane], lane);
  }
  return g_cryptonDecryptKeys;
}

// OFFSET: 0x00B0BDD0
// FUNCTION: fn_18399
void StoreLittleEndian32(unsigned int value, unsigned char* bytes) {
  bytes[0] = static_cast<unsigned char>(value);
  bytes[1] = static_cast<unsigned char>(value >> 8);
  bytes[2] = static_cast<unsigned char>(value >> 16);
  bytes[3] = static_cast<unsigned char>(value >> 24);
}

// OFFSET: 0x00B0C3D0
// FUNCTION: fn_18400
unsigned int LoadLittleEndian32(const unsigned char* bytes) {
  return ((((static_cast<unsigned int>(bytes[3]) << 8) | bytes[2]) << 8 | bytes[1]) << 8) | bytes[0];
}

// OFFSET: 0x00B0C440
// FUNCTION: fn_18401
__declspec(noinline) unsigned char FealRotate2(unsigned char value) {
  if (g_fealRotateTableDirty) {
    int low = 0;
    int high = 0;
    for (int i = 0; i < 256; ++i) {
      g_fealRotateTable[i] = static_cast<unsigned char>(high + low);
      low += 4;
      if (low > 255) {
        low = 0;
        ++high;
      }
    }
    g_fealRotateTableDirty = 0;
  }
  return g_fealRotateTable[value];
}

// OFFSET: 0x00B0C4A0
// FUNCTION: fn_18402
__declspec(noinline) unsigned char FealS0(unsigned char a, unsigned char b) {
  return FealRotate2(static_cast<unsigned char>(a + b));
}

// OFFSET: 0x00B0C4C0
// FUNCTION: fn_18403
__declspec(noinline) unsigned char FealS1(unsigned char a, unsigned char b) {
  return FealRotate2(static_cast<unsigned char>(a + 1 + b));
}

// OFFSET: 0x00B11070
// FUNCTION: fn_18407
unsigned int FealF(unsigned int alpha, unsigned int beta) {
  const unsigned char* a = reinterpret_cast<const unsigned char*>(&alpha);
  const unsigned char* b = reinterpret_cast<const unsigned char*>(&beta);
  unsigned char f[4];
  unsigned char f2 = a[3] ^ a[2] ^ b[1];
  unsigned char f1 = FealS1(a[1] ^ b[0] ^ a[0], f2);
  f2 = FealS0(f2, f1);
  f[1] = f1;
  f[2] = f2;
  f[0] = FealS0(a[0], f1);
  f[3] = FealS1(a[3], f2);
  return *reinterpret_cast<unsigned int*>(f);
}

// OFFSET: 0x00B11430
// FUNCTION: fn_18408
unsigned int MistyFL(const unsigned int* ek, unsigned int input, unsigned char k) {
  unsigned int d0 = input >> 16;
  unsigned int d1 = input & 0xFFFF;
  if (k % 2) {
    unsigned char t = static_cast<unsigned char>((k - 1) / 2);
    d1 ^= d0 & ek[((t + 2) % 8) + 8];
    d0 ^= d1 | ek[(t + 4) % 8];
  } else {
    unsigned char t = k / 2;
    d1 ^= d0 & ek[t];
    d0 ^= d1 | ek[((t + 6) % 8) + 8];
  }
  return (d0 << 16) | d1;
}

#define MISTY_FI(input, key, output)                                      {                                                                         unsigned int d9 = ((input) >> 7) & 0x1FF;                               unsigned int d7 = (input) & 0x7F;                                       d9 = g_mistyS9[d9] ^ d7;                                                d7 = (g_mistyS7[d7] ^ ((key) >> 9) ^ d9) & 0x7F;                        d9 = g_mistyS9[((key) & 0x1FF) ^ d9] ^ d7;                              (output) = d9 | (d7 << 9);                                            }

// OFFSET: 0x00B11510
// FUNCTION: fn_18409
unsigned int MistyFO(const unsigned int* ek, unsigned int input, unsigned char k) {
  unsigned int t0 = input >> 16;
  unsigned int t1 = input & 0xFFFF;
  t0 ^= ek[k];
  MISTY_FI(t0, ek[((k + 5) % 8) + 8], t0);
  t0 ^= t1;
  t1 ^= ek[(k + 2) % 8];
  MISTY_FI(t1, ek[((k + 1) % 8) + 8], t1);
  t1 ^= t0;
  t0 ^= ek[(k + 7) % 8];
  MISTY_FI(t0, ek[((k + 3) % 8) + 8], t0);
  t0 ^= t1;
  t1 ^= ek[(k + 4) % 8];
  return (t1 << 16) | t0;
}

// OFFSET: 0x00B11650
// FUNCTION: fn_18410
int ShaTransform(const unsigned char* state, unsigned int word, unsigned int* digest) {
  unsigned int w[80];
  unsigned int h0 = ((((static_cast<unsigned int>(state[0]) << 8) | state[1]) << 8 | state[2]) << 8) | state[3];
  unsigned int h1 = ((((static_cast<unsigned int>(state[4]) << 8) | state[5]) << 8 | state[6]) << 8) | state[7];
  unsigned int h2 = ((((static_cast<unsigned int>(state[8]) << 8) | state[9]) << 8 | state[10]) << 8) | state[11];
  unsigned int h3 = ((((static_cast<unsigned int>(state[12]) << 8) | state[13]) << 8 | state[14]) << 8) | state[15];
  unsigned int h4 = ((((static_cast<unsigned int>(state[16]) << 8) | state[17]) << 8 | state[18]) << 8) | state[19];
  int i;

  w[0] = word;
  for (i = 1; i < 16; ++i) {
    w[i] = 0;
  }
  for (i = 16; i < 80; ++i) {
    w[i] = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
  }

  unsigned int a = h0;
  unsigned int b = h1;
  unsigned int c = h2;
  unsigned int d = h3;
  unsigned int e = h4;
  unsigned int temp;
  for (i = 0; i < 20; ++i) {
    temp = ((a << 5) | (a >> 27)) + ((b & c) | (~b & d)) + e + w[i] + 0x5A827998;
    e = d;
    d = c;
    c = (b << 30) | (b >> 2);
    b = a;
    a = temp;
  }
  for (i = 20; i < 40; ++i) {
    temp = ((a << 5) | (a >> 27)) + (b ^ c ^ d) + e + w[i] + 0x6EF9EBA1;
    e = d;
    d = c;
    c = (b << 30) | (b >> 2);
    b = a;
    a = temp;
  }
  for (i = 40; i < 60; ++i) {
    temp = ((a << 5) | (a >> 27)) + (((b | c) & d) | (b & c)) + e + w[i] + 0x7F1CBCDC;
    e = d;
    d = c;
    c = (b << 30) | (b >> 2);
    b = a;
    a = temp;
  }
  for (i = 60; i < 80; ++i) {
    temp = ((a << 5) | (a >> 27)) + (b ^ c ^ d) + e + w[i] + 0xAA62D1D6;
    e = d;
    d = c;
    c = (b << 30) | (b >> 2);
    b = a;
    a = temp;
  }

  digest[0] = h0 + a;
  digest[1] = h1 + b;
  digest[2] = h2 + c;
  digest[3] = h3 + d;
  digest[4] = h4 + e;
  return 0;
}

// OFFSET: 0x00B11B80
// FUNCTION: fn_18411
void BuildKeyedBoxes(unsigned char* boxes, const unsigned char* key, int keyLength, int count) {
  if (count <= 0) {
    return;
  }

  for (int box = 0; box < count; ++box) {
    for (int i = 0; i < 256; ++i) {
      boxes[box * 256 + i] = static_cast<unsigned char>(i);
    }
  }

  int j = keyLength;
  int keyIndex = 0;
  int base = 0;
  for (int box = 0; box < count; ++box) {
    int offset = 0;
    for (int pass = 0; pass < 8; ++pass) {
      for (int i = 0; i < 256; ++i) {
        j = (boxes[base + (offset + i) % 256] + j + key[keyIndex]) % 256;
        unsigned char swap = boxes[base + i];
        boxes[base + i] = boxes[base + j];
        boxes[base + j] = swap;
        keyIndex = (keyIndex + 1) % keyLength;
      }
      offset += box;
    }
    base += 256;
  }
}

#define SBOX_ROUND(a, b, c)                                                     ((g_sboxLookup[((a) & 0x36) | ((c) & 0x6900) | ((b) & 0x6C0)] & 0xF) |         (g_sboxLookup[((c) & 0x22A) | ((a) & 0x2980) | ((b) & 0x9011)] & 0xCC00) |    (g_sboxLookup[((b) & 0x4828) | ((c) & 0x10C4) | ((a) & 0x8601)] & 0x3300) |    (g_sboxLookup[((c) & 0x8411) | ((a) & 0x5048) | ((b) & 0x2106)] & 0xF0))

// OFFSET: 0x00B120B0
// FUNCTION: fn_18412
void SboxEncryptBlock(unsigned char* block, const unsigned short* key) {
  unsigned short x0 = static_cast<unsigned short>((block[1] << 8) | block[0]);
  unsigned short x1 = static_cast<unsigned short>((block[3] << 8) | block[2]);
  unsigned short x2 = static_cast<unsigned short>((block[5] << 8) | block[4]);
  unsigned short x3 = static_cast<unsigned short>((block[7] << 8) | block[6]);

  for (int round = 8; round != 0; --round) {
    unsigned int a;
    unsigned int b;
    unsigned int c;
    a = static_cast<unsigned short>(key[0] ^ x1);
    b = static_cast<unsigned short>(key[1] ^ x2);
    c = static_cast<unsigned short>(key[2] ^ x3);
    x0 ^= SBOX_ROUND(a, b, c);
    a = static_cast<unsigned short>(key[3] ^ x2);
    b = static_cast<unsigned short>(key[4] ^ x3);
    c = static_cast<unsigned short>(key[5] ^ x0);
    x1 ^= SBOX_ROUND(a, b, c);
    a = static_cast<unsigned short>(key[6] ^ x3);
    b = static_cast<unsigned short>(key[7] ^ x0);
    c = static_cast<unsigned short>(key[8] ^ x1);
    x2 ^= SBOX_ROUND(a, b, c);
    a = static_cast<unsigned short>(key[9] ^ x0);
    b = static_cast<unsigned short>(key[10] ^ x1);
    c = static_cast<unsigned short>(key[11] ^ x2);
    x3 ^= SBOX_ROUND(a, b, c);
    key += 12;
  }

  block[0] = static_cast<unsigned char>(x0);
  block[1] = static_cast<unsigned char>(x0 >> 8);
  block[2] = static_cast<unsigned char>(x1);
  block[3] = static_cast<unsigned char>(x1 >> 8);
  block[4] = static_cast<unsigned char>(x2);
  block[5] = static_cast<unsigned char>(x2 >> 8);
  block[6] = static_cast<unsigned char>(x3);
  block[7] = static_cast<unsigned char>(x3 >> 8);
}

#define SBOX_INDEX(value, box)                                           ((((((((value) >> g_sboxBitSelect[box][5]) & 1) * 2 |                       (((value) >> g_sboxBitSelect[box][4]) & 1)) * 2 |                     (((value) >> g_sboxBitSelect[box][3]) & 1)) * 2 |                     (((value) >> g_sboxBitSelect[box][2]) & 1)) * 2 |                     (((value) >> g_sboxBitSelect[box][1]) & 1)) * 2 |                     (((value) >> g_sboxBitSelect[box][0]) & 1))

// OFFSET: 0x00B124F0
// FUNCTION: fn_18413
void BuildSboxLookup() {
  for (unsigned int value = 0; value < 0x10000; ++value) {
    g_sboxLookup[value] = static_cast<unsigned short>(
      g_sboxEntries[0][SBOX_INDEX(value, 0)] | g_sboxEntries[1][SBOX_INDEX(value, 1)] |
      g_sboxEntries[2][SBOX_INDEX(value, 2)] | g_sboxEntries[3][SBOX_INDEX(value, 3)] |
      g_sboxEntries[4][SBOX_INDEX(value, 4)] | g_sboxEntries[5][SBOX_INDEX(value, 5)] |
      g_sboxEntries[6][SBOX_INDEX(value, 6)] | g_sboxEntries[7][SBOX_INDEX(value, 7)]);
  }
}

// OFFSET: 0x00B12FC0
// FUNCTION: fn_18414
int KeyedBoxFeistel(const unsigned char* input, unsigned char* output, int length, const KeyedBoxContext* context) {
  if (length == 2) {
    const int round = g_keyedBoxRound;
    unsigned char right = context->boxes[round][input[0]] ^ input[1];
    output[1] = right;
    unsigned char left = context->boxes[round + 1][right] ^ input[0];
    output[0] = left;
    g_keyedBoxRound = round + 4;
    right = context->boxes[round + 2][left] ^ right;
    output[1] = right;
    output[0] = context->boxes[round + 3][right] ^ left;
    return 0;
  }

  const int half = length / 2;
  unsigned char buffer[8];
  int i;

  KeyedBoxFeistel(input, buffer, half, context);
  for (i = 0; i < half; ++i) {
    output[half + i] = input[half + i] ^ buffer[i];
  }

  KeyedBoxFeistel(output + half, buffer, half, context);
  for (i = 0; i < half; ++i) {
    output[i] = input[i] ^ buffer[i];
  }

  KeyedBoxFeistel(output, buffer, half, context);
  for (i = 0; i < half; ++i) {
    output[half + i] ^= buffer[i];
  }

  KeyedBoxFeistel(output + half, buffer, half, context);
  for (i = 0; i < half; ++i) {
    output[i] ^= buffer[i];
  }

  return 0;
}

#define SEAL_ROT8(x) (((x) >> 8) | ((x) << 24))
#define SEAL_ROT9(x) (((x) >> 9) | ((x) << 23))
#define SEAL_ROT16(x) (((x) >> 16) | ((x) << 16))
#define SEAL_ROT24(x) (((x) >> 24) | ((x) << 8))

// OFFSET: 0x00B134A0
// FUNCTION: fn_18415
int SealGenerate(const SealContext* key, unsigned int in, unsigned int* out) {
  unsigned int a = in ^ key->r[12];
  unsigned int b = SEAL_ROT8(in) ^ key->r[13];
  unsigned int c = SEAL_ROT16(in) ^ key->r[14];
  unsigned int d = SEAL_ROT24(in) ^ key->r[15];
  unsigned int n1;
  unsigned int n2;
  unsigned int n3;
  unsigned int n4;
  unsigned short p;
  unsigned short q;

  for (int j = 0; j < 2; ++j) {
    p = a & 0x7FC;
    b += key->t[p / 4];
    a = SEAL_ROT9(a);
    p = b & 0x7FC;
    c += key->t[p / 4];
    b = SEAL_ROT9(b);
    p = c & 0x7FC;
    d += key->t[p / 4];
    c = SEAL_ROT9(c);
    p = d & 0x7FC;
    a += key->t[p / 4];
    d = SEAL_ROT9(d);

    n1 = d;
    n2 = b;
    n3 = a;
    n4 = c;

    p = a & 0x7FC;
    b += key->t[p / 4];
    a = SEAL_ROT9(a);
    p = b & 0x7FC;
    c += key->t[p / 4];
    b = SEAL_ROT9(b);
    p = c & 0x7FC;
    d += key->t[p / 4];
    c = SEAL_ROT9(c);
    p = d & 0x7FC;
    a += key->t[p / 4];
    d = SEAL_ROT9(d);

    for (int i = 0; i < 64; ++i) {
      p = a & 0x7FC;
      b += key->t[p / 4];
      a = SEAL_ROT9(a);
      b ^= a;
      q = b & 0x7FC;
      c ^= key->t[q / 4];
      b = SEAL_ROT9(b);
      c += b;
      p = (p + c) & 0x7FC;
      d += key->t[p / 4];
      c = SEAL_ROT9(c);
      d ^= c;
      q = (q + d) & 0x7FC;
      a ^= key->t[q / 4];
      d = SEAL_ROT9(d);
      a += d;
      p = (p + a) & 0x7FC;
      b ^= key->t[p / 4];
      a = SEAL_ROT9(a);
      q = (q + b) & 0x7FC;
      c += key->t[q / 4];
      b = SEAL_ROT9(b);
      p = (p + c) & 0x7FC;
      d ^= key->t[p / 4];
      c = SEAL_ROT9(c);
      q = (q + d) & 0x7FC;
      a += key->t[q / 4];
      d = SEAL_ROT9(d);

      *out++ = b + key->s[4 * i];
      *out++ = c ^ key->s[4 * i + 1];
      *out++ = d + key->s[4 * i + 2];
      *out++ = a ^ key->s[4 * i + 3];

      if (i & 1) {
        a += n3;
        c += n4;
      } else {
        a += n1;
        c += n2;
      }
    }
  }

  return 0;
}
