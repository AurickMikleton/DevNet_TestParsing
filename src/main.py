import sys
import os
import text_parsing

# book_file = open("../data/book.txt", 'r', encoding='utf-8')
# print(text_parsing.parse(book_file.read()))
# book_file.close()


def main():
    if len(sys.argv) != 3:
        show_docs()
    if sys.argv[1] != "parse":
        print(f"Invalid command: {sys.argv[1]}")
        show_docs()
    if os.path.isfile(sys.argv[2]) is False:
        print("Invalid file path")
        quit()
    book_file = open(sys.argv[2], 'r', encoding='utf-8')
    text_parsing.parse(book_file.read())
    book_file.close()


def show_docs():
    print("-------- Valid Commands --------\n"
          " parse <path>  Parse the text in a txt file"
          )
    quit()


if __name__ == "__main__":
    main()
