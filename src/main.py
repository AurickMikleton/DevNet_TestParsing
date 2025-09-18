import text_parsing
import sys

book_file = open("../data/book.txt", 'r', encoding='utf-8')

print(text_parsing.parse(book_file.read()))
print(sys.argv[1:])

book_file.close()
