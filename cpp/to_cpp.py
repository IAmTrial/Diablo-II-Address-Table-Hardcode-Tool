import os
import sys

def main():
    if len(sys.argv) < 2:
        print("Missing address directory.")
        exit()

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

    print("Converting the following files:")
    print(address_files)

if __name__ == "__main__":
    main()
