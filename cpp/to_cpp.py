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
    game_address_table_text = "{"

    print("Converting the following files:")
    print(address_files)

    for address_file_path in address_files:
        version_name = address_file_path[:-4]

        address_file_lines = []
        with open(os.path.join(address_dir_name, address_file_path), "r") as address_file:
            reader = csv.reader(address_file, delimiter='\t')
            address_file_lines = [line for line in reader]

        address_file_lines = address_file_lines[1:]

        converted_address_file_text = ""
        for line in address_file_lines:
            library_path = line[0]
            address_name = line[1]
            locator_type = line[2]
            locator_value = line[3]

            if locator_type == "N/A":
                continue

            converted_address_file_text += f"{{ \"{library_path[:-4]}_{address_name}\", GameAddress::From{locator_type}(\"{library_path}\", {locator_value})\n }},\n"

        address_file_text = f"{{\n\"{version_name}\", {{\n{converted_address_file_text}\n}}\n }},"
        game_address_table_text += address_file_text

    game_address_table_text += "}"

    # Output the file.
    template_text = ""
    with open("./game_address_table_template.cc", "r") as game_address_table_template:
        template_text = "".join(game_address_table_template.readlines())

    output_text = template_text.replace("ALL_ADDRESS_TABLE", game_address_table_text)

    with open("./game_address_table.hpp", "w") as game_address_table_output:
        game_address_table_output.write(output_text)

if __name__ == "__main__":
    main()
