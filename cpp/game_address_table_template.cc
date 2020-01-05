#ifndef D2ATHT_CPP_GAME_ADDRESS_TABLE_HPP_
#define D2ATHT_CPP_GAME_ADDRESS_TABLE_HPP_

#include "game_address_table.hpp"

#include <windows.h>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <nowide/convert.hpp>
#include <fmt/format.h>

#include "config.hpp"
#include "../../include/cxx/game_address.hpp"
#include "../../include/cxx/default_game_library.hpp"
#include "game_library.hpp"
#include "../../include/cxx/game_version.hpp"
#include "../wide_macro.h"

namespace mapi {
namespace {

std::unordered_map<std::string_view, GameAddress>
GetRunningGameVersionAddressTable() {
  // Format marking!!!
  switch (d2::GetRunningGameVersionId()) {
    case d2::GameVersion::k1_00: {
      return ADDRESS_TABLE_1_00;
    }

    case d2::GameVersion::k1_01: {
      return ADDRESS_TABLE_1_01;
    }

    case d2::GameVersion::k1_02: {
      return ADDRESS_TABLE_1_02;
    }

    case d2::GameVersion::k1_03: {
      return ADDRESS_TABLE_1_03;
    }

    case d2::GameVersion::k1_04B_C: {
      return ADDRESS_TABLE_1_04B_AND_C;
    }

    case d2::GameVersion::k1_05: {
      return ADDRESS_TABLE_1_05;
    }

    case d2::GameVersion::k1_05B: {
      return ADDRESS_TABLE_1_05B;
    }

    case d2::GameVersion::k1_06: {
      return ADDRESS_TABLE_1_06;
    }

    case d2::GameVersion::k1_06B: {
      return ADDRESS_TABLE_1_06B;
    }

    case d2::GameVersion::k1_07Beta: {
      return ADDRESS_TABLE_1_07_BETA;
    }

    case d2::GameVersion::k1_07: {
      return ADDRESS_TABLE_1_07;
    }

    case d2::GameVersion::k1_08: {
      return ADDRESS_TABLE_1_08;
    }

    case d2::GameVersion::k1_09: {
      return ADDRESS_TABLE_1_09;
    }

    case d2::GameVersion::k1_09B: {
      return ADDRESS_TABLE_1_09B;
    }

    case d2::GameVersion::k1_09D: {
      return ADDRESS_TABLE_1_09D;
    }

    case d2::GameVersion::k1_10Beta: {
      return ADDRESS_TABLE_1_10_BETA;
    }

    case d2::GameVersion::k1_10SBeta: {
      return ADDRESS_TABLE_1_10S_BETA;
    }

    case d2::GameVersion::k1_10: {
      return ADDRESS_TABLE_1_10;
    }

    case d2::GameVersion::k1_11: {
      return ADDRESS_TABLE_1_11;
    }

    case d2::GameVersion::k1_11B: {
      return ADDRESS_TABLE_1_11B;
    }

    case d2::GameVersion::k1_12A: {
      return ADDRESS_TABLE_1_12A;
    }

    case d2::GameVersion::k1_13ABeta: {
      return ADDRESS_TABLE_1_13A_BETA;
    }

    case d2::GameVersion::k1_13C: {
      return ADDRESS_TABLE_1_13C;
    }

    case d2::GameVersion::k1_13D: {
      return ADDRESS_TABLE_1_13D;
    }

    case d2::GameVersion::kClassic1_14A: {
      return ADDRESS_TABLE_CLASSIC_1_14A;
    }

    case d2::GameVersion::kLod1_14A: {
      return ADDRESS_TABLE_LOD_1_14A;
    }

    case d2::GameVersion::kClassic1_14B: {
      return ADDRESS_TABLE_CLASSIC_1_14B;
    }

    case d2::GameVersion::kLod1_14B: {
      return ADDRESS_TABLE_LOD_1_14B;
    }

    case d2::GameVersion::kClassic1_14C: {
      return ADDRESS_TABLE_CLASSIC_1_14C;
    }

    case d2::GameVersion::kLod1_14C: {
      return ADDRESS_TABLE_LOD_1_14D;
    }

    case d2::GameVersion::kClassic1_14D: {
      return ADDRESS_TABLE_CLASSIC_1_14D;
    }

    case d2::GameVersion::kLod1_14D: {
      return ADDRESS_TABLE_LOD_1_14D;
    }
  }
}

const std::unordered_map<std::string_view, GameAddress>&
GetAddressTable() {
  static std::unordered_map<
      std::string_view,
      GameAddress
  > game_address_table(
      GetRunningGameVersionAddressTable()
  );

  return game_address_table;
}

} // namespace

const GameAddress&
GetGameAddress(
    std::string_view address_name
) {
  try {
    return GetAddressTable().at(address_name);
  } catch (const std::out_of_range& e) {
    constexpr std::wstring_view kErrorFormatMessage =
        L"File: {} \n"
        L"Line: {} \n"
        L"\n"
        L"Address not defined for {}.";

    std::wstring full_message = fmt::format(
        kErrorFormatMessage,
        __FILEW__,
        __LINE__,
        nowide::widen(address_name.data())
    );

    MessageBoxW(
        nullptr,
        full_message.data(),
        L"Address Not Defined",
        MB_OK | MB_ICONERROR
    );

    std::exit(0);
  }
}

std::intptr_t
GetRawAddress(
    std::string_view address_name
) {
  return GetGameAddress(address_name).raw_address();
}

} // namespace mapi

#endif // D2ATHT_CPP_GAME_ADDRESS_TABLE_HPP_
