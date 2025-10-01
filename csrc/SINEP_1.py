import os
import sys
import subprocess

os.makedirs("parsed", exist_ok=True)

def parse():
    files = os.listdir(sys.argv[2])
    for i in files:
        path = sys.argv[2] + '/' + i
        result = subprocess.run(["parse.exe", path, i])

def stat():
    banlist = sys.argv[2]
    files = os.listdir("parsed")
    for i in files:
        path = "parsed/" + i
        result = subprocess.run(["stat.exe", path, banlist, i])

def main():
    if len(sys.argv) != 3:
        show_docs()
    match sys.argv[1]:
        case "parse": parse()
        case "stat": stat()
        case _:
            print(f"Invalid command: {sys.argv[1]}")
            show_docs()

def show_docs():
    print("-------- Valid Commands --------\n"
          " parse <dir path>  Parse the text in the files within the provided directory"
          " stat <file path>  File to a csv 'banlist' print stats of parsed files"
          )
    quit()

main()
