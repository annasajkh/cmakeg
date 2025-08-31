import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Cmake builder script")
    parser.add_argument("--version", type=str, help="The version of the program")

    args = parser.parse_args()
    
    if args.version is None:
        print("Error: --version is not provided")
        exit(-1)

    file = open("version.txt", "w")
    file.write(args.version)
    file.close()