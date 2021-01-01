#if !defined(SGMAPI_READ_ADDRESS_FROM_TXT_TABLE)

#include "game_address_table_impl.hpp"

#include <string_view>

#include "../../../../include/cxx/game_version.hpp"
#include "game_address_locator/game_decorated_name_locator.hpp"
#include "game_address_locator/game_offset_locator.hpp"
#include "game_address_locator/game_ordinal_locator.hpp"

namespace mapi {

GameAddressTable LoadGameAddressTable() {
  GameAddressTable game_address_table;

  // Format marking!!!
  switch (d2::GetRunningGameVersionId()) {
    case d2::GameVersion::k1_00: {
      ADDRESS_TABLE_1_00
    }

    case d2::GameVersion::k1_01: {
      ADDRESS_TABLE_1_01
    }

    case d2::GameVersion::k1_02: {
      ADDRESS_TABLE_1_02
    }

    case d2::GameVersion::k1_03: {
      ADDRESS_TABLE_1_03
    }

    case d2::GameVersion::k1_04B_C: {
      ADDRESS_TABLE_1_04B_AND_C
    }

    case d2::GameVersion::k1_05: {
      ADDRESS_TABLE_1_05
    }

    case d2::GameVersion::k1_05B: {
      ADDRESS_TABLE_1_05B
    }

    case d2::GameVersion::k1_06: {
      ADDRESS_TABLE_1_06
    }

    case d2::GameVersion::k1_06B: {
      ADDRESS_TABLE_1_06B
    }

    case d2::GameVersion::k1_07Beta: {
      ADDRESS_TABLE_1_07_BETA
    }

    case d2::GameVersion::k1_07: {
      ADDRESS_TABLE_1_07
    }

    case d2::GameVersion::k1_08: {
      ADDRESS_TABLE_1_08
    }

    case d2::GameVersion::k1_09: {
      ADDRESS_TABLE_1_09
    }

    case d2::GameVersion::k1_09B: {
      ADDRESS_TABLE_1_09B
    }

    case d2::GameVersion::k1_09D: {
      ADDRESS_TABLE_1_09D
    }

    case d2::GameVersion::k1_10Beta: {
      ADDRESS_TABLE_1_10_BETA
    }

    case d2::GameVersion::k1_10SBeta: {
      ADDRESS_TABLE_1_10S_BETA
    }

    case d2::GameVersion::k1_10: {
      ADDRESS_TABLE_1_10
    }

    case d2::GameVersion::k1_11: {
      ADDRESS_TABLE_1_11
    }

    case d2::GameVersion::k1_11B: {
      ADDRESS_TABLE_1_11B
    }

    case d2::GameVersion::k1_12A: {
      ADDRESS_TABLE_1_12A
    }

    case d2::GameVersion::k1_13ABeta: {
      ADDRESS_TABLE_1_13A_BETA
    }

    case d2::GameVersion::k1_13C: {
      ADDRESS_TABLE_1_13C
    }

    case d2::GameVersion::k1_13D: {
      ADDRESS_TABLE_1_13D
    }

    case d2::GameVersion::kClassic1_14A: {
      ADDRESS_TABLE_CLASSIC_1_14A
    }

    case d2::GameVersion::kLod1_14A: {
      ADDRESS_TABLE_LOD_1_14A
    }

    case d2::GameVersion::kClassic1_14B: {
      ADDRESS_TABLE_CLASSIC_1_14B
    }

    case d2::GameVersion::kLod1_14B: {
      ADDRESS_TABLE_LOD_1_14B
    }

    case d2::GameVersion::kClassic1_14C: {
      ADDRESS_TABLE_CLASSIC_1_14C
    }

    case d2::GameVersion::kLod1_14C: {
      ADDRESS_TABLE_LOD_1_14D
    }

    case d2::GameVersion::kClassic1_14D: {
      ADDRESS_TABLE_CLASSIC_1_14D
    }

    case d2::GameVersion::kLod1_14D: {
      ADDRESS_TABLE_LOD_1_14D
    }
  }

  return game_address_table;
}

} // namespace mapi

#endif // !defined(SGMAPI_READ_ADDRESS_FROM_TXT_TABLE)
