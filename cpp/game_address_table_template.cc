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
  const std::unordered_map<
      std::string_view,
      std::unordered_map<std::string_view, GameAddress>
  > all_address_table = {
      ALL_ADDRESS_TABLE
  };

  std::string_view running_game_version_name =
      d2::GetRunningGameVersionName();

  return all_address_table.at(running_game_version_name);
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
