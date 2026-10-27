#include "Archive/PakFile.h"
#include "Archive/GPakUpdate.h"
#include "Archive/PakCipher.h"

#include <algorithm>
#include <cstring>
#include <zlib.h>

namespace {
#pragma pack(push, 1)

  struct PakTableHeader {
    /* 0000 */ std::uint32_t auxiliarySize;
    /* 0004 */ std::uint32_t compressedSize;
    /* 0008 */ std::uint32_t originalSize;
  };

  struct PakEntryRecord {
    /* 0000 */ std::uint32_t compressedSize;
    /* 0004 */ std::uint32_t alignedSize;
    /* 0008 */ std::uint32_t originalSize;
    /* 000C */ std::uint8_t flags;
    /* 000D */ std::uint64_t dataOffset;
  };

#pragma pack(pop)

  struct PakHeaderTable {
    /* 0000 */ std::uint64_t tableOffset;
    /* 0008 */ std::uint32_t entryCount;
    /* 000C */ std::uint32_t version;
  };

  struct PakUsedBlock {
    /* 0000 */ std::uint64_t offset;
    /* 0008 */ std::uint32_t size;

    auto operator<=>(const PakUsedBlock& other) const = default;
  };

  static_assert(sizeof(PakTableHeader) == 12);
  static_assert(sizeof(PakEntryRecord) == 21);
  static_assert(offsetof(PakEntryRecord, dataOffset) == 13);
  static_assert(sizeof(PakHeaderTable) == 16);
  static_assert(sizeof(PakUsedBlock) == 16);

  constexpr std::uint32_t kPakHeaderSize = 46;
  constexpr std::uint64_t kPakHeaderTablePosition = 30;
  constexpr std::uint32_t kPakVersion = 0x300;

  // INLINED
  int CountDigits(std::uint32_t value) {
    if (value >= 1000000000) {
      return 10;
    }
    if (value >= 100000000) {
      return 9;
    }
    if (value >= 10000000) {
      return 8;
    }
    if (value >= 1000000) {
      return 7;
    }
    if (value >= 100000) {
      return 6;
    }
    if (value >= 10000) {
      return 5;
    }
    if (value >= 1000) {
      return 4;
    }
    if (value >= 100) {
      return 3;
    }
    if (value >= 10) {
      return 2;
    }
    return 1;
  }
}

// OFFSET: 0x005EF660
// FUNCTION: fn_04009
CPakFile::~CPakFile() = default;

namespace myGPack {
  // OFFSET: 0x005F7790
  // FUNCTION: fn_04046
  CReadPakFile::CReadPakFile() : m_handle(INVALID_HANDLE_VALUE), m_size(0) {
  }

  // OFFSET: 0x005EF690
  // FUNCTION: fn_04010
  CReadPakFile::~CReadPakFile() {
    this->Close();
  }

  // OFFSET: 0x005EF710
  // FUNCTION: fn_04011
  CWritePakFile::~CWritePakFile() = default;

  // OFFSET: 0x005EF850
  // FUNCTION: fn_04013
  void CGPakUpdate::Close() {
    this->m_output.Close();
    this->Reset();

    for (CGPakRead* source : this->m_sources) {
      delete source;
    }

    this->m_sources.clear();
    this->m_names.clear();
    this->m_added.clear();
    this->m_freeBlocks.clear();
    this->m_auxiliary.clear();

    this->m_scratch.shrink_to_fit();
    this->m_entries.shrink_to_fit();
    this->m_added.shrink_to_fit();
  }

  // OFFSET: 0x005EF9A0
  // FUNCTION: fn_04014
  bool CGPakUpdate::RemoveObsoleteEntries() {
    std::list<std::string> names;
    names.push_back("data\\texture\\유저인터페이스\\map\\1@20cn1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@20cn2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4cdn.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4drk.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4igd.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4inq.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4mag.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4sac.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4tro.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@4win.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@adv.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@advs.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@air1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@air2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ant01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@bamq.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@begi.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@cata.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ch_t.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ch_u.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@colo.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@crd.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@def01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@def02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@def03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@dime.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@dorado.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@drdo.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@dth1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@dth2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@dth3.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@emd.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@eom.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@exds.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@exhn.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@exnw.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@exse.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@exsh.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@exsr.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@face.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ffp.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@gef.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@gef_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ge_sn.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ge_st.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@gl_he2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@gl_prq.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@gol1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@gol2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@halo.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@herbs.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@infi.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@izd02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@jorchs.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@jorlab.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@jtb.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@lab.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@lhz.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@lvcb.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ma_b.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ma_c.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@ma_h.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@md_gef.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@mir.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@orcs.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@os_b.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@pda.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@pdb.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@pop1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@pop2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@pop3.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@rev.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@rgsr.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@sewb2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@slw.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@soul.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@spa.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@spa2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@sthb.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@sthc.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@sthd.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@swat.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@tcamp.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@tfd1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@tfd2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@tnm1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@tnm2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@tnm3.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@tower.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@twsd.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@uns.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@vrac1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@vrac2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@vrcas.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@vrgen.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@vrhha.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@vrpo.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@whl.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@xm_d.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\1@xm_d2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@ch_t.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@exds.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@mir.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@nyr.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@orcs.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@tower.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\2@vrclo.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\3@ch_t.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\3@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\3@tower.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\4@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\4@tower.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\5@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\5@tower.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\6@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\6@tower.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\7@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\8@thts.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\airplane.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\airport.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\air_if.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\alberta_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\alb_ship.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeba_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_c01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_c02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_c03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_c04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_c05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_cas01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_cas02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_cas03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_cas04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\aldeg_cas05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ama_dun01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ama_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ama_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\amicit02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\arena_room.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\arug_cas01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\arug_cas02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\arug_cas03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\arug_que01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\auction_01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\auction_02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ayo_dun01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ayo_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ayo_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ba_chess.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ba_go.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ba_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\brz_god.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\cmd_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\cmd_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\conch_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\crazy_i.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\door001.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\d_mond01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\d_mond02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\d_mond03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ecl_hub01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ecl_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ecl_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ecl_in03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ecl_in04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ein_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ep19.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\eternity.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\first_sp.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\fx_test.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\fx_test01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\geffen_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_c01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_c02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_c03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_c04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_c05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_cas01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_cas02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_cas03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_cas04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gefg_cas05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gef_tower.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gon_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gon_test.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\gosea.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\grademk.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\harboro2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\har_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\hu_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\icas_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ice_dun04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land06.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land07.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\int_land08.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\in_moc_16.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\in_orcs01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\izlude_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac01_e.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac01_f.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac01_g.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac01_h.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac02_e.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac02_f.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac02_g.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ac02_h.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int06.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int07.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_int08.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\iz_ng01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\jawaii_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job3_gen01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job3_guil01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job4_bio.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job4_mag.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job4_tro.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job4_vio.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job_gun.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job_ko.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job_soul.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\job_star.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\jor_dun03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\jupe_area1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\jupe_area2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\jupe_ele.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\jupe_ele_r.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\jupe_gate.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\kth_test.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lasa_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lhz_cube.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lhz_d_n2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lhz_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lhz_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lhz_in03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lhz_que01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lou_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\lou_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\mal_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\memohall.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\moc_castle.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\moc_para01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\monk_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\morocc_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\moro_cav.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\mosk_fild01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\mosk_ship.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\nakhyang.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\new_zone02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\new_zone04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\nif_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ordeal_a00.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ordeal_a02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\paramk.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_c01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_c02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_c03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_c04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_c05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_cas01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_cas02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_cas03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_cas04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payg_cas05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payon_01_q.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payon_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payon_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\payon_in03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt2014.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt2014_ch.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_c01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_c02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_c03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_c04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_c05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_cas01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_cas02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_cas03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_cas04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prtg_cas05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_are01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_are_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_cas.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_castle.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_cas_q.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_church.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_elib.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_evt_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_in_r.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_lib.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_lib_q.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\prt_pri00.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\pub_cat.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\p_track01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_avan01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_bingo.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_dan01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_god01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_god02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_hugel.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_lhz.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qaru01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qaru02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qaru03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qaru04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qaru05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qsch01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qsch02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qsch03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qsch04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_qsch05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_swat.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_thor.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\que_thr.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\quiz_00.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\quiz_01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\quiz_02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ra_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ra_temin.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ra_temsky.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rebel_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rgsr_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rockmi2.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena04.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena07.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena08.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena09.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena10.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena11.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\rwc_arena12.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\schg_cas01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\schg_cas02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\schg_cas03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\sch_lab.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\siege_test.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\silk_lair.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\soundtest.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\star_frst.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\star_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\sword_1-1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\sword_2-1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\s_atelier.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\temp.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\test0102.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\test0206.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\test0228.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\test_avi.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\thana_boss.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\thana_step.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\tha_scene01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\tha_t07.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\tha_t08.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\tur_dun05.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\tur_dun06.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\um_in.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\uno1.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\un_bk_q.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\un_bunker.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\un_myst.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\valkyrie.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\ver_tunn.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\voidfruit.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\vr_bob.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\yggdrasil01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\yuno_in01.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\yuno_in02.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\yuno_in03.bmp");
    names.push_back("data\\texture\\유저인터페이스\\map\\yuno_pre.bmp");

    std::int64_t removed = 0;

    if (names.size() > 0) {
      for (auto name = names.begin(); name != names.end(); ++name) {
        for (auto entry = this->m_entries.begin(); entry != this->m_entries.end();) {
          if (std::strcmp(entry->name.m_name, name->c_str()) == 0) {
            entry = this->m_entries.erase(entry);
            ++removed;
          } else {
            ++entry;
          }
        }
      }
    }

    return removed > 0;
  }

  // OFFSET: 0x005F4CB0
  // FUNCTION: fn_04015
  void CGPakUpdate::BuildFreeBlocks() {
    std::set<PakUsedBlock> used;
    used.insert({0, kPakHeaderSize});
    used.insert({this->m_tableOffset, this->m_tableSize});

    for (const PakEntry& entry : this->m_entries) {
      if (std::strrchr(entry.name.m_name, '.')) {
        used.insert({entry.dataOffset + kPakHeaderSize, entry.alignedSize});
      }
    }

    this->m_freeBlocks.clear();

    std::uint64_t position = 0;

    for (const PakUsedBlock& block : used) {
      if (position < block.offset) {
        this->m_freeBlocks.insert({static_cast<std::uint32_t>(block.offset - position), position});
      }

      position = block.offset + block.size;
    }

    if (position < this->m_output.GetSize()) {
      this->m_freeBlocks.insert({static_cast<std::uint32_t>(this->m_output.GetSize() - position), position});
    }
  }

  // OFFSET: 0x005F4FD0
  // FUNCTION: fn_04016
  bool CGPakRead::ReadEntry(PakEntry* entry, void* buffer) {
    const std::uint8_t* data = this->m_file->Read(entry->dataOffset + kPakHeaderSize, entry->alignedSize);
    if (!data) {
      return false;
    }

    if (entry->flags & 0x80) {
      entry->dataOffset = 0;
      std::memcpy(buffer, data, entry->alignedSize);
      return true;
    }

    int length = 0;
    std::uint8_t key[8];
    PakCipher cipher(DerivePakKey(entry->name.m_name, key));

    if (entry->flags & 2) {
      cipher.Decode(data, entry->alignedSize, this->m_scratch.data(), &length, CountDigits(entry->compressedSize));
      data = this->m_scratch.data();
    } else if (entry->flags & 4) {
      cipher.DecodePrefix(data, entry->alignedSize, this->m_scratch.data(), &length);
      data = this->m_scratch.data();
    } else {
      length = entry->compressedSize;
    }

    entry->flags &= 1;
    entry->alignedSize = entry->compressedSize;
    entry->dataOffset = 0;
    std::memcpy(buffer, data, entry->compressedSize);
    return true;
  }

  // OFFSET: 0x005F51B0
  // FUNCTION: fn_04017
  void CGPakRead::CollectNames(std::set<std::string>& names) const {
    for (const PakEntry& entry : this->m_entries) {
      const char* name = entry.name.m_name;
      if (std::strrchr(name, '.')) {
        names.emplace(name);
      }
    }
  }

  // OFFSET: 0x005F5210
  // FUNCTION: fn_04018
  std::uint64_t CGPakUpdate::Allocate(std::uint32_t size) {
    auto found = this->m_freeBlocks.lower_bound({size, 0});
    if (found == this->m_freeBlocks.end()) {
      return this->m_output.GetSize();
    }

    const std::uint64_t offset = found->offset;
    const std::uint32_t remaining = found->size - size;
    if (remaining != 0) {
      this->m_freeBlocks.insert({remaining, offset + size});
    }

    this->m_freeBlocks.erase(found);
    return offset;
  }

  // OFFSET: 0x005F53D0
  // FUNCTION: fn_04019
  bool CGPakUpdate::Commit(CNotifier* notifier) {
    if (!this->RemoveObsoleteEntries()) {
      return true;
    }

    if (!notifier) {
      notifier = &this->m_notifier;
    }

    this->BuildFreeBlocks();

    std::uint64_t copied = 0;
    const std::size_t total = this->m_names.size();

    for (const std::string& name : this->m_names) {
      PakName key;
      key.Assign(name.c_str());
      if (!this->CopyEntry(key)) {
        return false;
      }

      ++copied;
      notifier->Notify(copied, total);
    }

    this->m_entries.insert(this->m_entries.end(), this->m_added.begin(), this->m_added.end());
    std::sort(this->m_entries.begin(), this->m_entries.end(), [](const PakEntry& first, const PakEntry& second) { return first.name < second.name; });

    std::vector<std::uint8_t> table;
    table.reserve(this->m_entries.size() * 101);

    for (const PakEntry& entry : this->m_entries) {
      const char* name = entry.name.m_name;
      table.insert(table.end(), name, name + std::strlen(name) + 1);

      PakEntryRecord record;
      record.compressedSize = entry.compressedSize;
      record.alignedSize = entry.alignedSize;
      record.originalSize = entry.originalSize;
      record.flags = entry.flags;
      record.dataOffset = entry.dataOffset;
      const auto* bytes = reinterpret_cast<const std::uint8_t*>(&record);
      table.insert(table.end(), bytes, bytes + sizeof(record));
    }

    uLongf compressedLength = compressBound(static_cast<uLong>(table.size()));
    std::vector<std::uint8_t> compressed(compressedLength);
    bool result;

    if (compress(compressed.data(), &compressedLength, table.data(), static_cast<uLong>(table.size())) != Z_OK) {
      result = false;
    } else {
      std::vector<std::uint8_t> auxiliary;

      for (auto source = this->m_sources.begin(); source != this->m_sources.end(); ++source) {
        (*source)->ReadAuxiliary(auxiliary);
        if (!auxiliary.empty()) {
          break;
        }
      }

      if (auxiliary.empty()) {
        auxiliary = this->m_auxiliary;
      }

      PakTableHeader header;
      header.auxiliarySize = static_cast<std::uint32_t>(auxiliary.size());
      header.compressedSize = compressedLength;
      header.originalSize = static_cast<std::uint32_t>(table.size());

      const std::uint64_t offset = this->Allocate(header.auxiliarySize + compressedLength + sizeof(header));
      result = false;

      if (this->m_output.Write(offset, &header, sizeof(header)) && this->m_output.Write(offset + sizeof(header), compressed.data(), compressedLength) &&
          this->m_output.Write(offset + compressedLength + sizeof(header), auxiliary.data(), header.auxiliarySize) &&
          this->m_output.Write(0, "Event Horizon", 14)) {
        PakHeaderTable information;
        information.tableOffset = offset - kPakHeaderSize;
        information.entryCount = static_cast<std::uint32_t>(this->m_entries.size());
        information.version = kPakVersion;

        if (this->m_output.Write(kPakHeaderTablePosition, &information, sizeof(information))) {
          result = true;
        }
      }
    }

    return result;
  }

  // OFFSET: 0x005F59C0
  // FUNCTION: fn_04020
  bool CGPakUpdate::Open(const char* filename) {
    if (!this->m_output.Open(filename) || !CGPak::Open(&this->m_output)) {
      return false;
    }

    this->ReadAuxiliary(this->m_auxiliary);
    return true;
  }

  // OFFSET: 0x005F6320
  // FUNCTION: fn_04021
  bool CGPakUpdate::CopyEntry(const PakName& name) {
    PakEntry entry;

    for (CGPakRead* source : this->m_sources) {
      if (!source->Find(name, &entry)) {
        continue;
      }

      std::vector<std::uint8_t> buffer(entry.compressedSize);
      if (!source->ReadEntry(&entry, buffer.data())) {
        return false;
      }

      const std::uint64_t offset = this->Allocate(entry.alignedSize);
      if (!this->m_output.Write(offset, buffer.data(), entry.alignedSize)) {
        return false;
      }

      entry.dataOffset = offset - kPakHeaderSize;

      auto found = std::lower_bound(this->m_entries.begin(), this->m_entries.end(), entry.name,
                                    [](const PakEntry& candidate, const PakName& key) { return candidate.name < key; });
      if (found != this->m_entries.end() && found->name == entry.name) {
        *found = entry;
      } else {
        this->m_added.push_back(entry);
      }

      return true;
    }

    return false;
  }

  // OFFSET: 0x005F7780
  // FUNCTION: fn_04045
  std::uint64_t CReadPakFile::GetSize() const {
    return this->m_size;
  }

  // OFFSET: 0x005F7810
  // FUNCTION: fn_04047
  void CReadPakFile::Close() {
    if (this->m_handle) {
      CloseHandle(this->m_handle);
      this->m_handle = nullptr;
    }
  }

  // OFFSET: 0x005F7890
  // FUNCTION: fn_04048
  bool CWritePakFile::Open(const char* filename) {
    this->m_handle = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH | FILE_ATTRIBUTE_NORMAL, nullptr);
    if (this->m_handle == INVALID_HANDLE_VALUE) {
      return false;
    }

    LARGE_INTEGER size;

    GetFileSizeEx(this->m_handle, &size);
    this->m_size = static_cast<std::uint64_t>(size.QuadPart);
    return true;
  }

  // OFFSET: 0x005F78F0
  // FUNCTION: fn_04049
  std::uint8_t* CReadPakFile::Read(std::uint64_t offset, std::uint32_t length) {
    if (offset + length > this->m_size) {
      return nullptr;
    }

    this->m_buffer.resize(length);

    LARGE_INTEGER position;
    position.QuadPart = static_cast<std::int64_t>(offset);

    SetFilePointerEx(this->m_handle, position, nullptr, FILE_BEGIN);

    DWORD read;
    if (ReadFile(this->m_handle, this->m_buffer.data(), length, &read, nullptr) && read == length) {
      return this->m_buffer.data();
    }

    return nullptr;
  }

  // OFFSET: 0x005F79C0
  // FUNCTION: fn_04050
  bool CWritePakFile::Write(std::uint64_t offset, const void* bytes, std::uint32_t length) {
    LARGE_INTEGER position;
    position.QuadPart = static_cast<std::int64_t>(offset);
    SetFilePointerEx(this->m_handle, position, nullptr, FILE_BEGIN);

    DWORD written;
    if (!WriteFile(this->m_handle, bytes, length, &written, nullptr) || written != length) {
      return false;
    }

    std::uint64_t end = offset + length;
    if (this->m_size < end) {
      this->m_size = end;
    }

    return true;
  }
}
