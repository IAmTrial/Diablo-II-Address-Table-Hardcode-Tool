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
    "LOD_1_14D",
]

LIBRARY_ID_FROM_LIBRARY_FILES = {
    "BNClient.dll": "D2_DefaultLibrary_kBNClient",
    "D2CMP.dll": "D2_DefaultLibrary_kD2CMP",
    "D2Client.dll": "D2_DefaultLibrary_kD2Client",
    "D2Common.dll": "D2_DefaultLibrary_kD2Common",
    "D2DDraw.dll": "D2_DefaultLibrary_kD2DDraw",
    "D2Direct3D.dll": "D2_DefaultLibrary_kD2Direct3D",
    "D2GDI.dll": "D2_DefaultLibrary_kD2GDI",
    "D2GFX.dll": "D2_DefaultLibrary_kD2GFX",
    "D2Game.dll": "D2_DefaultLibrary_kD2Game",
    "D2Glide.dll": "D2_DefaultLibrary_kD2Glide",
    "D2Lang.dll": "D2_DefaultLibrary_kD2Lang",
    "D2Launch.dll": "D2_DefaultLibrary_kD2Launch",
    "D2MCPClient.dll": "D2_DefaultLibrary_kD2MCPClient",
    "D2Multi.dll": "D2_DefaultLibrary_kD2Multi",
    "D2Net.dll": "D2_DefaultLibrary_kD2Net",
    "D2Server.dll": "D2_DefaultLibrary_kD2Server",
    "D2Sound.dll": "D2_DefaultLibrary_kD2Sound",
    "D2Win.dll": "D2_DefaultLibrary_kD2Win",
    "Fog.dll": "D2_DefaultLibrary_kFog",
    "Storm.dll": "D2_DefaultLibrary_kStorm",
}

ENTRY_TEXT_FORMAT = (
    "{{ \\\n"
        + "  {library}, {address_name}, \\\n"
        + "  {address_locator_type}, {address_locator_value} \\\n"
        + "}},"
)

def main():
    if len(sys.argv) < 2:
        print("Missing address directory.")
        exit()

    # Determine if the supplied address files are valid.
    address_dir_name = sys.argv[1]
    print(f"Converting {address_dir_name} to source file...")

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
    game_address_table_dict = { v: "0" for v in VERSIONS }

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
                address_locator_type = "Mapi_GameAddressLocatorType_kOffset"
            elif address_locator_type == "Ordinal":
                address_locator_type = "Mapi_GameAddressLocatorType_kOrdinal"
            elif address_locator_type == "Decorated Name":
                address_locator_type = "Mapi_GameAddressLocatorType_kExportedName"
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

        game_address_table_dict[version_name] = f"{converted_address_file_text}"

    # Output the file.
    define_text = ""
    for version_name in game_address_table_dict:
        define_text += f"#define {NAMESPACE}_{version_name} {{{game_address_table_dict[version_name]}}} \n"

    with open("./game_address_table_define.h", "w") as game_address_table_output:
        game_address_table_output.write(define_text)

if __name__ == "__main__":
    main()
