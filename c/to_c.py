import csv
import json
import os
import sys

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
    game_address_table_dict = {}

    print("Converting the following files:")
    print(address_files)

    for address_file_path in address_files:
        version_name = address_file_path[:-4].replace(".", "_").replace(" ", "_").upper()

        address_file_lines = []
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

            converted_address_file_text = "\n".join((converted_address_file_text,
                f"struct MAPI_GameAddress {library_path[:-4]}_{address_name}_game_address;",
                f"MAPI_GameAddress_InitFromLibraryPathAnd{locator_type}(&{library_path[:-4]}_{address_name}_game_address, \"{library_path}\", {locator_value});",
                f"AddGameAddress(\"{library_path}\", \"{address_name}\", &{library_path[:-4]}_{address_name}_game_address);"
            ))

        game_address_table_dict[version_name] = f"{converted_address_file_text}"

    # Output the file.
    output_text = ""
    with open("./game_address_table_impl_template.c", "r") as game_address_table_template:
        output_text = "".join(game_address_table_template.readlines())

    for version_name in game_address_table_dict:
        output_text = output_text.replace(
            "ADDRESS_TABLE_{};".format(version_name),
            "{}".format(game_address_table_dict[version_name])
        )

    # Any unimplemented versions returns the empty map.
    output_text = output_text.replace("ADDRESS_TABLE_", "break; // ")

    with open("./game_address_table_impl.c", "w") as game_address_table_output:
        game_address_table_output.write(output_text)

if __name__ == "__main__":
    main()
