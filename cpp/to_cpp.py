import csv
import json
import os
import sys

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
    "BNClient.dll": "::mapi::DefaultLibrary::kBNClient",
    "D2Client.dll": "::mapi::DefaultLibrary::kD2Client",
    "D2CMP.dll": "::mapi::DefaultLibrary::kD2CMP",
    "D2Common.dll": "::mapi::DefaultLibrary::kD2Common",
    "D2DDraw.dll": "::mapi::DefaultLibrary::kD2DDraw",
    "D2Direct3D.dll": "::mapi::DefaultLibrary::kD2Direct3D",
    "D2Game.dll": "::mapi::DefaultLibrary::kD2Game",
    "D2GDI.dll": "::mapi::DefaultLibrary::kD2GDI",
    "D2GFX.dll": "::mapi::DefaultLibrary::kD2GFX",
    "D2Glide.dll": "::mapi::DefaultLibrary::kD2Glide",
    "D2Lang.dll": "::mapi::DefaultLibrary::kD2Lang",
    "D2Launch.dll": "::mapi::DefaultLibrary::kD2Launch",
    "D2MCPClient.dll": "::mapi::DefaultLibrary::kD2MCPClient",
    "D2Multi.dll": "::mapi::DefaultLibrary::kD2Multi",
    "D2Net.dll": "::mapi::DefaultLibrary::kD2Net",
    "D2Server.dll": "::mapi::DefaultLibrary::kD2Server",
    "D2Sound.dll": "::mapi::DefaultLibrary::kD2Sound",
    "D2Win.dll": "::mapi::DefaultLibrary::kD2Win",
    "Fog.dll": "::mapi::DefaultLibrary::kFog",
    "Storm.dll": "::mapi::DefaultLibrary::kStorm",
}

def main():
    if len(sys.argv) < 2:
        print("Missing address directory.")
        exit()

    # Determine if the supplied address files are valid.
    address_dir_name = sys.argv[1]
    print(f"Converting {address_dir_name} to C++ file...")

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
    game_address_table_dict = { v: "" for v in VERSIONS }

    print("Converting the following files:")
    print(address_files)

    for address_file_path in address_files:
        version_name = address_file_path[:-4].replace(".", "_").replace(" ", "_").upper()

        with open(os.path.join(address_dir_name, address_file_path), "r") as address_file:
            reader = csv.reader(address_file, delimiter='\t')
            address_file_lines = [line for line in reader]

        del address_file_lines[0]

        converted_address_file_text = ""
        for line in address_file_lines:
            library_path = line[0]
            address_name = line[1]
            locator_type = line[2]
            locator_value = line[3]

            if library_path in LIBRARY_ID_FROM_LIBRARY_FILES:
                library_id = LIBRARY_ID_FROM_LIBRARY_FILES[library_path]
            else:
                library_id = f"\"{library_path}\""

            if locator_type == "N/A":
                continue
            elif locator_type == "Offset":
                locator_type_name = "GameOffsetLocator"
            elif locator_type == "Ordinal":
                locator_type_name = "GameOrdinalLocator"
            elif locator_type == "Decorated Name":
                locator_type_name = "GameDecoratedNameLocator"
                locator_value = f"\"{locator_value}\""

            converted_address_file_text = "\\\n".join((converted_address_file_text,
                f"game_address_table[\"{library_path}\"][\"{address_name}\"] = " +
                    f"std::make_unique<{locator_type_name}>({library_id}, {locator_value});"
            ))

        game_address_table_dict[version_name] = converted_address_file_text

    # Output the file.
    output_text = ""
    with open("./game_address_table_impl_template.cc", "r") as game_address_table_template:
        output_text = "".join(game_address_table_template.readlines())

    define_text = ""
    for version_name in game_address_table_dict:
        define_text += "#define ADDRESS_TABLE_{} {} \\\n".format(version_name, game_address_table_dict[version_name])
        define_text += "break; \n"

    with open("./game_address_table_impl.cc", "w") as game_address_table_output:
        game_address_table_output.write(define_text)
        game_address_table_output.write(output_text)

if __name__ == "__main__":
    main()
