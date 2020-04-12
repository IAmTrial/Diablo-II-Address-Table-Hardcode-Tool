#include "game_address_table_impl.h"

#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "../../../include/c/default_game_library.h"
#include "../../../include/c/game_address.h"
#include "../../../include/c/game_version.h"
#include "../../wide_macro.h"
#include "error_handling.h"
#include "game_library.h"

static struct MAPI_GameAddressTable game_address_table = {
  .entries = NULL,
  .num_elements = 0,
  .capacity = 0,
};

/**
 * Doubles the size of the game libraries collection.
 */
static void ResizeGameAddressTable(void) {
  game_address_table.capacity = (game_address_table.capacity == 0)
        ? 4
        : game_address_table.capacity * 2;

  game_address_table.entries = (struct MAPI_GameAddressTableEntry**) realloc(
      game_address_table.entries,
      game_address_table.capacity * sizeof(game_address_table.entries[0])
  );

  if (game_address_table.entries == NULL) {
    ExitOnAllocationFailure(__FILEW__, __LINE__);
  }
}

/**
 * Adds a game address to the table.
 */
static void AddGameAddress(
    const char* library_path,
    const char* address_name,
    const char* locator_type,
    union LocatorValue locator_value
) {
  if (game_address_table.num_elements == game_address_table.capacity) {
    ResizeGameAddressTable();
  }

  struct MAPI_GameAddressTableEntry* new_entry =
      (struct MAPI_GameAddressTableEntry*) malloc(sizeof(*new_entry));
  if (new_entry == NULL) {
    ExitOnAllocationFailure(__FILEW__, __LINE__);
  }

  new_entry->library_path = library_path;
  new_entry->address_name = address_name;

  if (strcmp(locator_type, "Offset") == 0) {
    new_entry->locator_type = LOCATOR_TYPE_OFFSET;
  } else if (strcmp(locator_type, "Ordinal") == 0) {
    new_entry->locator_type = LOCATOR_TYPE_ORDINAL;
  } else if (strcmp(locator_type, "Decorated Name") == 0) {
    new_entry->locator_type = LOCATOR_TYPE_DECORATED_NAME;
  } else {
    const wchar_t* kErrorFormat =
        L"Unknown locator type specified. \n"
        L"\n"
        L"Library Path: %s \n"
        L"Address Name: %s \n"
        L"Locator Type: %s";

    wchar_t full_message[256];

    swprintf(
        full_message,
        sizeof(full_message) / sizeof(full_message[0]),
        kErrorFormat,
        library_path,
        address_name,
        locator_type
    );

    ExitOnGeneralFailure(
        full_message,
        L"Unknown Locator Type",
        __FILEW__,
        __LINE__
    );
  }

  game_address_table.entries[game_address_table.num_elements] = new_entry;

  game_address_table.num_elements += 1;
}

/**
 * Clears the game address table of all entries.
 */
static void ClearGameAddressTable(void) {
  for (size_t i = 0; i < game_address_table.num_elements; i += 1) {
    free(&game_address_table.entries[i]);
  }

  free(game_address_table.entries);

  game_address_table.entries = 0;
  game_address_table.num_elements = 0;
  game_address_table.capacity = 0;
}

struct MAPI_GameAddressTable* LoadGameAddressTable(void) {
  // Format marking!!!
  switch (D2_GetRunningGameVersionId()) {
    case VERSION_1_00: {
      ADDRESS_TABLE_1_00
    }

    case VERSION_1_01: {
      ADDRESS_TABLE_1_01
    }

    case VERSION_1_02: {
      ADDRESS_TABLE_1_02
    }

    case VERSION_1_03: {
      ADDRESS_TABLE_1_03
    }

    case VERSION_1_04B_C: {
      ADDRESS_TABLE_1_04B_AND_C
    }

    case VERSION_1_05: {
      ADDRESS_TABLE_1_05
    }

    case VERSION_1_05B: {
      ADDRESS_TABLE_1_05B
    }

    case VERSION_1_06: {
      ADDRESS_TABLE_1_06
    }

    case VERSION_1_06B: {
      ADDRESS_TABLE_1_06B
    }

    case VERSION_1_07_BETA: {
      ADDRESS_TABLE_1_07_BETA
    }

    case VERSION_1_07: {
      ADDRESS_TABLE_1_07
    }

    case VERSION_1_08: {
      ADDRESS_TABLE_1_08
    }

    case VERSION_1_09: {
      ADDRESS_TABLE_1_09
    }

    case VERSION_1_09B: {
      ADDRESS_TABLE_1_09B
    }

    case VERSION_1_09D: {
      ADDRESS_TABLE_1_09D
    }

    case VERSION_1_10_BETA: {
      ADDRESS_TABLE_1_10_BETA
    }

    case VERSION_1_10S_BETA: {
      ADDRESS_TABLE_1_10S_BETA
    }

    case VERSION_1_10: {
      ADDRESS_TABLE_1_10
    }

    case VERSION_1_11: {
      ADDRESS_TABLE_1_11
    }

    case VERSION_1_11B: {
      ADDRESS_TABLE_1_11B
    }

    case VERSION_1_12A: {
      ADDRESS_TABLE_1_12A
    }

    case VERSION_1_13A_BETA: {
      ADDRESS_TABLE_1_13A_BETA
    }

    case VERSION_1_13C: {
      ADDRESS_TABLE_1_13C
    }

    case VERSION_1_13D: {
      ADDRESS_TABLE_1_13D
    }

    case CLASSIC_1_14A: {
      ADDRESS_TABLE_CLASSIC_1_14A
    }

    case LOD_1_14A: {
      ADDRESS_TABLE_LOD_1_14A
    }

    case CLASSIC_1_14B: {
      ADDRESS_TABLE_CLASSIC_1_14B
    }

    case LOD_1_14B: {
      ADDRESS_TABLE_LOD_1_14B
    }

    case CLASSIC_1_14C: {
      ADDRESS_TABLE_CLASSIC_1_14C
    }

    case LOD_1_14C: {
      ADDRESS_TABLE_LOD_1_14C
    }

    case CLASSIC_1_14D: {
      ADDRESS_TABLE_CLASSIC_1_14D
    }

    case LOD_1_14D: {
      ADDRESS_TABLE_LOD_1_14D
    }
  }

  return &game_address_table;
}
