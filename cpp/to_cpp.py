import csv
import json
import os
import sys

NAMESPACE = "MAPI_GAME_ADDRESS_TABLE"

VERSIONS = [
    "1_00",
    "1_01",
    "1_02",
    "1_03",
    "1_04B_AND_C",
    "1_05",
    "1_05B",
    "1_06",
    "1_06B",
    "1_07_BETA",
    "1_07",
    "1_08",
    "1_09",
    "1_09B",
    "1_09D",
    "1_10_BETA",
    "1_10S_BETA",
    "1_10",
    "1_11",
    "1_11B",
    "1_12A",
    "1_13A_BETA",
    "1_13C",
    "1_13D",
    "CLASSIC_1_14A",
    "LOD_1_14A",
    "CLASSIC_1_14B",
    "LOD_1_14B",
    "CLASSIC_1_14C",
    "LOD_1_14C",
    "CLASSIC_1_14D",
    "LOD_1_14D"
]

LIBRARY_ID_FROM_LIBRARY_FILES = {
    "BNClient.dll": "::d2::DefaultLibrary::kBNClient",
    "D2CMP.dll": "::d2::DefaultLibrary::kD2CMP",
    "D2Client.dll": "::d2::DefaultLibrary::kD2Client",
    "D2Common.dll": "::d2::DefaultLibrary::kD2Common",
    "D2DDraw.dll": "::d2::DefaultLibrary::kD2DDraw",
    "D2Direct3D.dll": "::d2::DefaultLibrary::kD2Direct3D",
    "D2Game.dll": "::d2::DefaultLibrary::kD2Game",
    "D2GDI.dll": "::d2::DefaultLibrary::kD2GDI",
    "D2GFX.dll": "::d2::DefaultLibrary::kD2GFX",
    "D2Glide.dll": "::d2::DefaultLibrary::kD2Glide",
    "D2Lang.dll": "::d2::DefaultLibrary::kD2Lang",
    "D2Launch.dll": "::d2::DefaultLibrary::kD2Launch",
    "D2MCPClient.dll": "::d2::DefaultLibrary::kD2MCPClient",
    "D2Multi.dll": "::d2::DefaultLibrary::kD2Multi",
    "D2Net.dll": "::d2::DefaultLibrary::kD2Net",
    "D2Server.dll": "::d2::DefaultLibrary::kD2Server",
    "D2Sound.dll": "::d2::DefaultLibrary::kD2Sound",
    "D2Win.dll": "::d2::DefaultLibrary::kD2Win",
    "Fog.dll": "::d2::DefaultLibrary::kFog",
    "Storm.dll": "::d2::DefaultLibrary::kStorm",
}

ENTRY_TEXT_FORMAT = (
    "::mapi::GameAddressTableEntry( \\\n"
        + "    ::std::tuple({library}, {address_name}), \\\n"
        + "    ::mapi::GameAddressLocator({address_locator_type}({library}, {address_locator_value})) \\\n"
        + "),"
)

EMPTY_TABLE_TEXT = (
    "::mapi::GameAddressTableEntry( \\\n"
        + "    ::std::tuple( \\\n"
        + "        static_cast<::d2::DefaultLibrary>(-1), \\\n"
        + "        \"\" \\\n"
        + "    ), \\\n"
        + "    ::mapi::GameAddressLocator( \\\n"
        + "        ::mapi::GameOffsetLocator( \\\n"
        + "            static_cast<::d2::DefaultLibrary>(-1), \\\n"
        + "            0 \\\n"
        + "        ) \\\n"
        + "    ) \\\n"
        + "),"
)

def main():
    if len(sys.argv) < 2:
        print("Missing address directory.")
        exit()

    # Determine if the supplied address files are valid.
    address_dir_name = sys.argv[1]
    print(f"Converting {address_dir_name} to C++ source file...")

    if not os.path.exists(address_dir_name):
        print(f"Path {address_dir_name} does not exist.")
        exit()

    if not os.path.isdir(address_dir_name):
        print(f"Path {address_dir_name} is not a directory.")
        exit()

    address_files = []

    for file in os.listdir(address_dir_name):
        if file.endswith(".txt"):
            address_files.append(file)

    # Convert the files into address table info.
    game_address_table_dict = { v: EMPTY_TABLE_TEXT for v in VERSIONS }

    print("Converting the following files:")
    print(address_files)

    for address_file_path in address_files:
        version_name = address_file_path[:-4].replace(".", "_").replace(" ", "_").upper()

        with open(os.path.join(address_dir_name, address_file_path), "r") as address_file:
            reader = csv.reader(address_file, delimiter='\t')
            address_file_lines = list(reader)

        del address_file_lines[0]

        converted_address_file_text = ""
        for line in address_file_lines:
            library_path = line[0]
            address_name = line[1]
            address_locator_type = line[2]
            address_locator_value = line[3]

            if address_locator_type == "N/A":
                continue
            elif address_locator_type == "Offset":
                address_locator_type = "::mapi::GameOffsetLocator"
            elif address_locator_type == "Ordinal":
                address_locator_type = "::mapi::GameOrdinalLocator"
            elif address_locator_type == "Decorated Name":
                address_locator_type = "::mapi::GameDecoratedNameLocator"
                address_locator_value = f"\"{address_locator_value}\""

            converted_address_file_text = "\\\n".join((
                converted_address_file_text,
                ENTRY_TEXT_FORMAT.format(
                    library=LIBRARY_ID_FROM_LIBRARY_FILES[library_path],
                    address_name=f"\"{address_name}\"",
                    address_locator_type=address_locator_type,
                    address_locator_value=address_locator_value
                )
            ))

        game_address_table_dict[version_name] = converted_address_file_text

    # Output the file.
    define_text = ""
    for version_name in game_address_table_dict:
        define_text += f"#define {NAMESPACE}_{version_name} {{{game_address_table_dict[version_name]}}} \n"

    with open("./game_address_table_define.hpp", "w") as game_address_table_output:
        game_address_table_output.write(define_text)

if __name__ == "__main__":
    main()
