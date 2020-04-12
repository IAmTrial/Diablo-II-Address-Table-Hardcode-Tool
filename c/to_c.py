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

def main():
    if len(sys.argv) < 2:
        print("Missing address directory.")
        exit()

    # Determine if the supplied address files are valid.
    address_dir_name = sys.argv[1]
    print(f"Converting {address_dir_name} to C source file...")

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
            address_file_lines = list(reader)

        del address_file_lines[0]

        converted_address_file_text = ""
        for line in address_file_lines:
            library_path = line[0]
            address_name = line[1]
            locator_type = line[2]
            locator_value = line[3]

            if locator_type == "N/A":
                continue
            elif locator_type == "Offset":
                locator_type_member_id = "offset"
            elif locator_type == "Ordinal":
                locator_type_member_id = "ordinal"
            elif locator_type == "Decorated Name":
                locator_type_member_id = "decorated_name"

            locator_value_var_id = f"{library_path[:-4]}_{address_name}_locator_value"

            converted_address_file_text = "\\\n".join((converted_address_file_text,
                f"union LocatorValue {locator_value_var_id} = {{ .{locator_type_member_id} = {locator_value} }};",
                f"AddGameAddress(\"{library_path}\", \"{address_name}\", \"{locator_type}\", {locator_value_var_id});"
            ))

        game_address_table_dict[version_name] = f"{converted_address_file_text}"

    # Output the file.
    output_text = ""
    with open("./game_address_table_impl_template.c", "r") as game_address_table_template:
        output_text = "".join(game_address_table_template.readlines())

    define_text = ""
    for version_name in game_address_table_dict:
        define_text += "#define ADDRESS_TABLE_{} {} \n".format(version_name, game_address_table_dict[version_name])

    with open("./game_address_table_impl.c", "w") as game_address_table_output:
        game_address_table_output.write(define_text)
        game_address_table_output.write(output_text)

if __name__ == "__main__":
    main()
