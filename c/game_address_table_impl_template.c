#include "game_address_table_impl.h"

#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include <mdc/container/map.h>
#include <mdc/std/stdbool.h>
#include <mdc/std/threads.h>
#include <mdc/string/basic_string.h>
#include "../../../../include/c/default_game_library.h"
#include "../../../../include/c/game_address.h"
#include "../../../../include/c/game_version.h"
#include "../../../wide_macro.h"
#include "../error_handling.h"
#include "../game_library.h"
#include "game_address_locator.h"
#include "map_path_map_string_game_address_locator.h"
#include "map_string_game_address_locator.h"

struct Mapi_Impl_GameAddressTableEntryLiteral {
  enum D2_GameVersion game_version;
  const wchar_t* library_path_cstr;
  const char* address_name_cstr;
  enum Mapi_Impl_LocatorType locator_type;
  union Mapi_Impl_LocatorValueLiteral locator_value;
};

void* Mapi_LocatorValue_InitFromLiteralAsVoid(
    void* locator_value_as_void,
    void* entry_literal_as_void
) {
  union Mapi_Impl_LocatorValue* locator_value;
  union Mapi_Impl_LocatorValue* init_locator_value;

  const struct Mapi_Impl_GameAddressTableEntryLiteral* entry_literal;

  locator_value = locator_value_as_void;
  entry_literal = entry_literal_as_void;

  init_locator_value = Mapi_LocatorValue_InitFromLiteral(
      locator_value,
      entry_literal->locator_type,
      &entry_literal->locator_value
  );

  if (init_locator_value != locator_value) {
    goto return_bad;
  }

  return locator_value;

return_bad:
  return NULL;
}

static const struct Mapi_Impl_GameAddressTableEntryLiteral
kGameAddressTableSrcs[] = {
    ADDRESS_TABLE_1_00
    ADDRESS_TABLE_1_00
    ADDRESS_TABLE_1_01
    ADDRESS_TABLE_1_02
    ADDRESS_TABLE_1_03
    ADDRESS_TABLE_1_04B_AND_C
    ADDRESS_TABLE_1_05
    ADDRESS_TABLE_1_05B
    ADDRESS_TABLE_1_06
    ADDRESS_TABLE_1_06B
    ADDRESS_TABLE_1_07_BETA
    ADDRESS_TABLE_1_07
    ADDRESS_TABLE_1_08
    ADDRESS_TABLE_1_09
    ADDRESS_TABLE_1_09B
    ADDRESS_TABLE_1_09D
    ADDRESS_TABLE_1_10_BETA
    ADDRESS_TABLE_1_10S_BETA
    ADDRESS_TABLE_1_10
    ADDRESS_TABLE_1_11
    ADDRESS_TABLE_1_11B
    ADDRESS_TABLE_1_12A
    ADDRESS_TABLE_1_13A_BETA
    ADDRESS_TABLE_1_13C
    ADDRESS_TABLE_1_13D
    ADDRESS_TABLE_CLASSIC_1_14A
    ADDRESS_TABLE_LOD_1_14A
    ADDRESS_TABLE_CLASSIC_1_14B
    ADDRESS_TABLE_LOD_1_14B
    ADDRESS_TABLE_CLASSIC_1_14C
    ADDRESS_TABLE_LOD_1_14C
    ADDRESS_TABLE_CLASSIC_1_14D
    ADDRESS_TABLE_LOD_1_14D
};

enum {
  kGameAddressTableSrcsCount = sizeof(kGameAddressTableSrcs)
      / sizeof(kGameAddressTableSrcs[0])
};

static struct Mdc_Map game_address_table;
static once_flag game_address_table_init_flag = ONCE_FLAG_INIT;

static void Mapi_Impl_InitGameAddressTable(void) {
  size_t i;

  struct Mdc_Map* init_game_address_table;
  struct Mdc_Map* game_address_table_path_entry;
  const struct Mapi_Impl_GameAddressTableEntryLiteral* entry_literal;

  enum D2_GameVersion current_running_game_version;

  struct Mdc_Fs_Path library_path;
  struct Mdc_Fs_Path* init_library_path;
  bool is_library_path_equal;

  struct Mdc_BasicString address_name;
  struct Mdc_BasicString* init_address_name;

  init_game_address_table = Mdc_MapPathMapStringGameAddressLocator_InitEmpty(
      &game_address_table
  );

  if (init_game_address_table != &game_address_table) {
    goto return_bad;
  }

  current_running_game_version = D2_GetRunningGameVersionId();

  init_library_path = Mdc_Fs_Path_InitEmpty(&library_path);

  if (init_library_path != &library_path) {
    ExitOnMdcFunctionFailure(L"Mdc_Fs_Path_InitEmpty", __FILEW__, __LINE__);
    goto return_bad;
  }

  for (i = 0; i < kGameAddressTableSrcsCount; i += 1) {
    entry_literal = &kGameAddressTableSrcs[i];

    if (entry_literal->game_version != current_running_game_version) {
      continue;
    }

    /* Add the path to the address table. */
    is_library_path_equal = Mdc_Fs_Path_EqualNativeCStr(
        &library_path,
        entry_literal->library_path_cstr
    );

    if (!is_library_path_equal) {
      Mdc_Fs_Path_Deinit(&library_path);

      init_library_path = Mdc_Fs_Path_InitFromCWStr(
          &library_path,
          entry_literal->library_path_cstr
      );

      if (init_library_path != &library_path) {
        goto return_bad;
      }
    }

    Mdc_Map_EmplaceKeyCopy(
        &game_address_table,
        &library_path,
        &Mdc_Map_InitEmpty,
        Mdc_MapStringGameAddressLocator_GetMapMetadata()
    );

    /* Add the entries for the path in the address table. */
    game_address_table_path_entry = Mdc_Map_At(
        &game_address_table,
        &library_path
    );

    if (game_address_table_path_entry != &game_address_table) {
      ExitOnMdcFunctionFailure(L"Mdc_Map_At", __FILEW__, __LINE__);
      goto return_bad;
    }

    init_address_name = Mdc_BasicString_InitFromCStr(
        &address_name,
        Mdc_CharTraitsChar_GetCharTraits(),
        entry_literal->address_name_cstr
    );

    if (init_address_name != &address_name) {
      ExitOnMdcFunctionFailure(
          L"Mdc_BasicString_InitFromCStr",
          __FILEW__,
          __LINE__
      );

      goto return_bad;
    }

    Mdc_Map_Emplace(
        game_address_table_path_entry,
        &address_name,
        &Mapi_LocatorValue_InitFromLiteralAsVoid,
        entry_literal
    );

    Mdc_BasicString_Deinit(&address_name);
  }

  Mdc_Fs_Path_Deinit(&library_path);

return_bad:
  return;
}

static void InitStatic(void) {
  call_once(
      &game_address_table_init_flag,
      &Mapi_Impl_InitGameAddressTable
  );
}

struct Mdc_Map* Mapi_Impl_GetGameAddressTable(void) {
  InitStatic();

  return &game_address_table;
}
